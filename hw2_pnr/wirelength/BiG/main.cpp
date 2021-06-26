#include "big_wa.h"
#include <cxxabi.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

std::string toString(int32_t a) {
    std::stringstream ss;
    std::string       str;
    ss << std::setw(2) << std::setfill('0') << a;
    return ss.str();
}

inline void print_exception(const std::exception &e, std::ostream &output = std::cerr) {
    output << "Died on " << abi::__cxa_demangle(typeid(e).name(), nullptr, nullptr, nullptr) << ": "
           << e.what() << std::endl;
}

int main() {
    int32_t num_cases = 5;
    int32_t mode = 0;
    for (int32_t case_id = 0; case_id < num_cases; case_id++) {
        double      x_layout_size = 1, y_layout_size = 1;
        int32_t     num_pins     = (case_id + 1) * 100;
        double      gamma        = 0.5;
        std::string infile_path  = "../dataset/bigwa_" + toString(case_id) + ".in.txt";
        std::string outfile_path = "../dataset/bigwa_" + toString(case_id) + ".out.txt";

        std::vector<double>                    x_pin_pos(num_pins), y_pin_pos(num_pins);
        //std::mt19937                           gen(case_id);
        //std::uniform_real_distribution<double> urd(0, 1);
        //for (auto &e : x_pin_pos) e = urd(gen) * x_layout_size;
        //for (auto &e : y_pin_pos) e = urd(gen) * y_layout_size;
        try {
            if (mode == 1) {
                std::mt19937                           gen(case_id);
                std::uniform_real_distribution<double> urd(0, 1);
                for (auto &e : x_pin_pos) e = urd(gen) * x_layout_size;
                for (auto &e : y_pin_pos) e = urd(gen) * y_layout_size;

                std::vector<double> x_pos_grad, y_pos_grad;
                auto                x_wl = BigWA::forwardAndBackward(x_pin_pos, gamma, x_pos_grad);
                auto                y_wl = BigWA::forwardAndBackward(y_pin_pos, gamma, y_pos_grad);
                auto                wl   = x_wl + y_wl;

                std::ofstream in_file, out_file;
                in_file.exceptions(std::ofstream::eofbit | std::ofstream::failbit |
                        std::ofstream::badbit);
                in_file.open(infile_path);
                out_file.exceptions(std::ofstream::eofbit | std::ofstream::failbit |
                        std::ofstream::badbit);
                out_file.open(outfile_path);

                in_file << num_pins << std::endl;
                for (int32_t i = 0; i < num_pins; i++) {
                    in_file << std::scientific << x_pin_pos[i] << " " << y_pin_pos[i] << std::endl;
                }
                out_file << std::scientific << wl << std::endl;
                for (int i = 0; i < num_pins; i++) {
                    out_file << std::scientific << x_pos_grad[i] << " " << y_pos_grad[i] << std::endl;
                }
                in_file.close();
                out_file.close();
                std::cerr << "Case " << toString(case_id) << ": Success." << std::endl;
            }
            else if(mode == 0) {
                std::ifstream in_file, out_file;
                in_file.exceptions(std::ifstream::eofbit | std::ifstream::failbit |
                        std::ifstream::badbit);
                in_file.open(infile_path);
                out_file.exceptions(std::ifstream::eofbit | std::ifstream::failbit |
                        std::ifstream::badbit);
                out_file.open(outfile_path);

                double wl;

                in_file >> num_pins;
                for (int32_t i = 0; i < num_pins; i++) {
                    in_file >> std::scientific >> x_pin_pos[i] >> y_pin_pos[i]; 
                }
                out_file >> std::scientific >> wl;
                std::vector<double> x_pos_grad(num_pins), y_pos_grad(num_pins);
                for(int32_t i = 0; i < num_pins; i++) {
                    out_file >> std::scientific >> x_pos_grad[i] >> y_pos_grad[i];
                }
                in_file.close();
                out_file.close();

                std::vector<double> x_pos_grad_bigwa, y_pos_grad_bigwa;
                auto x_wl = BigWA::forwardAndBackward(x_pin_pos, gamma, x_pos_grad_bigwa);
                auto y_wl = BigWA::forwardAndBackward(y_pin_pos, gamma, y_pos_grad_bigwa);
                auto wl_bigwa = x_wl + y_wl;

                auto cmp = [](double v1, double v2) { 
                    constexpr double epsilon = 1e-4;
                    return std::fabs(v1 - v2) < epsilon;
                };

                bool check_wl = cmp(wl_bigwa, wl);
                bool check_grad_x = std::equal(x_pos_grad_bigwa.begin(), x_pos_grad_bigwa.end(), 
                        x_pos_grad.begin(), cmp);
                bool check_grad_y = std::equal(y_pos_grad_bigwa.begin(), y_pos_grad_bigwa.end(),
                        y_pos_grad.begin(), cmp);

                std::cerr << "Case " << toString(case_id) << " wl " << check_wl << " grad_x " 
                    << check_grad_x << " grad_y " << check_grad_y << std::endl;
            }
        } catch (std::exception const &e) {
            std::cerr << "Case " << toString(case_id) << ": Fail." << std::endl;
            print_exception(e);
            throw e;
        }
    }
    return 0;
}
