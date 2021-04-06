#include "glog/logging.h"
#include "techmap_node.h"
#include "utils/stopwatch.hpp"
#include <iomanip>
#include <lorina/lorina.hpp>
#include <unordered_map>

using lorina::blif_reader;
using lorina::read_blif;

class Blif2DAGReader : public blif_reader {
public:
    void on_model(const std::string &model_name) const override {
        name2node_.clear();
        leaves_.clear();
        auto node              = detail::makeNode();
        node.name()            = model_name;
        node.is_model()        = true;
        name2node_[model_name] = node;
        model_node_            = node;
    }
    void on_input(const std::string &name) const override {
        auto node      = detail::makeNode();
        node.name()    = name;
        node.sources() = {};
        node.is_leaf() = true;
        leaves_.emplace_back(node);
        name2node_[name] = node;
    }
    void on_output(const std::string &name) const override {
        auto node   = detail::makeNode();
        node.name() = name;
        model_node_.sources().emplace_back(node);
        name2node_[name] = node;
    }
    void on_gate(const std::vector<std::string> &inputs, const std::string &output,
                 const output_cover_t &cover) const override {
        FMT_ASSERT(inputs.size() <= 2, "Only support 0/1/2-input gate!😢");
        DLOG_ASSERT(cover.size() == 1);
        Node node;
        if (name2node_.find(output) == name2node_.end()) {
            node               = detail::makeNode();
            name2node_[output] = node;
        } else {
            node = name2node_[output];
        }
        node.name() = output;
        if (inputs.empty()) {
            node.sources()         = {};
            node.cover_out_state() = cover[0].second[0] - '0';
            return;
        }
        for (auto &input : inputs) {
            DLOG_ASSERT(name2node_.find(input) != name2node_.end());
            auto source = name2node_[input];
            node.sources().emplace_back(source);
            source.num_fan_out()++;
        }
        auto &cover_a_state   = node.cover_a_state();
        auto &cover_b_state   = node.cover_b_state();
        auto &cover_out_state = node.cover_out_state();

        cover_a_state   = cover[0].first[0] - '0';
        cover_out_state = cover[0].second[0] - '0';

        if (inputs.size() == 1) { return; }

        cover_b_state = cover[0].first[1] - '0';
        if (cover_out_state) {
            node.ori_func() = [cover_a_state, cover_b_state](bool a, bool b) {
                return a == cover_a_state && b == cover_b_state;
            };
        } else {
            node.ori_func() = [cover_a_state, cover_b_state](bool a, bool b) {
                return !(a == cover_a_state && b == cover_b_state);
            };
        }
    }

    Node &             model_name() { return model_node_; }
    std::vector<Node> &leaves() { return leaves_; }

protected:
    mutable std::unordered_map<std::string, Node> name2node_;
    mutable Node                                  model_node_;
    mutable std::vector<Node>                     leaves_;
};

bool releaseState(DpState &a, DpState b) {
    if (b.min_cell_cnt() < a.min_cell_cnt() ||
        (b.min_cell_cnt() == a.min_cell_cnt() && b.max_depth() < a.max_depth())) {
        a = b;
        return true;
    }
    return false;
}

bool isAOI21(Node p, Node left_son, int32_t inv_in, bool is_virtually_inverse) {
    bool inv_a = inv_in & 1;
    bool inv_b = (inv_in >> 1) & 1;
    bool inv_c = (inv_in >> 2) & 1;
    for (int32_t in = 0; in < (1 << 3); in++) {
        bool a            = in & 1;
        bool b            = (in >> 1) & 1;
        bool c            = (in >> 2) & 1;
        bool ground_truth = p.ori_func()(left_son.ori_func()(a, b), c) ^ is_virtually_inverse;
        bool rv           = (!((a ^ inv_a) & (b ^ inv_b))) & (!(c ^ inv_c));
        if (ground_truth != rv) { return false; }
    }
    return true;
}

bool isAOI12(Node p, Node right_son, int32_t inv_in, bool is_virtually_inverse) {
    bool inv_a = inv_in & 1;
    bool inv_b = (inv_in >> 1) & 1;
    bool inv_c = (inv_in >> 2) & 1;
    for (int32_t in = 0; in < (1 << 3); in++) {
        bool a            = in & 1;
        bool b            = (in >> 1) & 1;
        bool c            = (in >> 2) & 1;
        bool ground_truth = p.ori_func()(a, right_son.ori_func()(b, c)) ^ is_virtually_inverse;
        bool rv           = (!((c ^ inv_c) & (b ^ inv_b))) & (!(a ^ inv_a));
        if (ground_truth != rv) { return false; }
    }
    return true;
}

bool isAOI22(Node p, Node left_son, Node right_son, int32_t inv_in, bool is_virtually_inverse) {
    bool inv_a = inv_in & 1;
    bool inv_b = (inv_in >> 1) & 1;
    bool inv_c = (inv_in >> 2) & 1;
    bool inv_d = (inv_in >> 3) & 1;
    for (int32_t in = 0; in < (1 << 4); in++) {
        bool a            = in & 1;
        bool b            = (in >> 1) & 1;
        bool c            = (in >> 2) & 1;
        bool d            = (in >> 3) & 1;
        bool ground_truth = p.ori_func()(left_son.ori_func()(a, b), right_son.ori_func()(c, d)) ^
                            is_virtually_inverse;
        bool rv = (!((a ^ inv_a) & (b ^ inv_b))) & (!((c ^ inv_c) & (d ^ inv_d)));
        if (ground_truth != rv) { return false; }
    }
    return true;
}


DpState DFS(Node p, bool is_inv);

void transferState(Node p, DpState &state, bool is_virtually_inverse) {
    {
        auto t1_state = detail::makeDpState();
        auto t2_state = detail::makeDpState();
        // binary function
        int32_t cover_state =
                (p.cover_a_state() << 2) + (p.cover_b_state() << 1) + p.cover_out_state();
        if (is_virtually_inverse) { cover_state ^= 1; }
        switch (cover_state) {
            case 0b001:   // (!a)&(!b)
                t1_state.name()      = p.name() + "_NOR2";
                t1_state.cell_type() = CellType::NOR2;
                t1_state.sons()      = {DFS(p.sources()[0], false), DFS(p.sources()[1], false)};
                t1_state.update();
                releaseState(state, t1_state);
                break;
            case 0b011:   // (!a)&b
                t1_state.name()      = p.name() + "_NOR2";
                t1_state.cell_type() = CellType::NOR2;
                t1_state.sons()      = {DFS(p.sources()[0], false), DFS(p.sources()[1], true)};
                t1_state.update();
                releaseState(state, t1_state);
                break;
            case 0b101:   // a&(!b)
                t1_state.name()      = p.name() + "_NOR2";
                t1_state.cell_type() = CellType::NOR2;
                t1_state.sons()      = {DFS(p.sources()[0], true), DFS(p.sources()[1], false)};
                t1_state.update();
                releaseState(state, t1_state);
                break;
            case 0b111:   // a&b
                t1_state.name()      = p.name() + "_NAND2";
                t1_state.cell_type() = CellType::NAND2;
                t1_state.sons()      = {DFS(p.sources()[0], false), DFS(p.sources()[1], false)};
                t1_state.update();
                t2_state.name()      = p.name() + "_INV";
                t2_state.cell_type() = CellType::INV;
                t2_state.sons()      = {t1_state};
                t2_state.update();
                releaseState(state, t2_state);
                break;
            case 0b000:   // a|b
                t1_state.name()      = p.name() + "_NOR2";
                t1_state.cell_type() = CellType::NOR2;
                t1_state.sons()      = {DFS(p.sources()[0], false), DFS(p.sources()[1], false)};
                t1_state.update();
                t2_state.name()      = p.name() + "_INV";
                t2_state.cell_type() = CellType::INV;
                t2_state.sons()      = {t1_state};
                t2_state.update();
                releaseState(state, t2_state);
                break;
            case 0b010:   // a|(!b)
                t1_state.name()      = p.name() + "_NAND2";
                t1_state.cell_type() = CellType::NAND2;
                t1_state.sons()      = {DFS(p.sources()[0], true), DFS(p.sources()[1], false)};
                t1_state.update();
                releaseState(state, t1_state);
                break;
            case 0b100:   // (!a)|b
                t1_state.name()      = p.name() + "_NAND2";
                t1_state.cell_type() = CellType::NAND2;
                t1_state.sons()      = {DFS(p.sources()[0], false), DFS(p.sources()[1], true)};
                t1_state.update();
                releaseState(state, t1_state);
                break;
            case 0b110:   // (!a)|(!b)
                t1_state.name()      = p.name() + "_NAND2";
                t1_state.cell_type() = CellType::NAND2;
                t1_state.sons()      = {DFS(p.sources()[0], false), DFS(p.sources()[1], false)};
                t1_state.update();
                releaseState(state, t1_state);
                break;
            default:
                DLOG_ASSERT(false);
        }
    }
    if (!p.sources()[0].is_leaf() && p.sources()[0].num_fan_out() == 1 &&
        p.sources()[0].num_sources() == 2) {
        auto aoi21_state   = detail::makeDpState();
        aoi21_state.name() = p.name() + "_AIO21";
        Node left_son      = p.sources()[0];
        Node right_son     = p.sources()[1];
        for (int32_t inv_in = 0; inv_in < (1 << 3); inv_in++) {
            if (isAOI21(p, left_son, inv_in, is_virtually_inverse)) {
                aoi21_state.cell_type() = CellType::AOI21;
                aoi21_state.sons()      = {DFS(left_son.sources()[0], inv_in & 1),
                                      DFS(left_son.sources()[1], (inv_in >> 1) & 1),
                                      DFS(right_son, (inv_in >> 2) & 1)};
                aoi21_state.update();
                releaseState(state, aoi21_state);
            }
        }
    }
    if (!p.sources()[1].is_leaf() && p.sources()[1].num_fan_out() == 1 &&
        p.sources()[1].num_sources() == 2) {
        auto aoi12_state   = detail::makeDpState();
        aoi12_state.name() = p.name() + "_AIO12";
        Node left_son      = p.sources()[0];
        Node right_son     = p.sources()[1];
        for (int32_t inv_in = 0; inv_in < (1 << 3); inv_in++) {
            if (isAOI12(p, right_son, inv_in, is_virtually_inverse)) {
                aoi12_state.cell_type() = CellType::AOI12;
                aoi12_state.sons()      = {DFS(left_son, inv_in & 1),
                                      DFS(right_son.sources()[0], (inv_in >> 1) & 1),
                                      DFS(right_son.sources()[1], (inv_in >> 2) & 1)};
                aoi12_state.update();
                releaseState(state, aoi12_state);
            }
        }
    }
    if (!p.sources()[0].is_leaf() && !p.sources()[1].is_leaf() &&
        p.sources()[0].num_fan_out() == 1 && p.sources()[1].num_fan_out() == 1 &&
        p.sources()[0].num_sources() == 2 && p.sources()[1].num_sources() == 2) {
        auto aoi22_state   = detail::makeDpState();
        aoi22_state.name() = p.name() + "_AOI22";
        Node left_son      = p.sources()[0];
        Node right_son     = p.sources()[1];
        for (int32_t inv_in = 0; inv_in < (1 << 4); inv_in++) {
            if (isAOI22(p, left_son, right_son, inv_in, is_virtually_inverse)) {
                aoi22_state.cell_type() = CellType::AOI22;
                aoi22_state.sons()      = {DFS(left_son.sources()[0], inv_in & 1),
                                      DFS(left_son.sources()[1], (inv_in >> 1) & 1),
                                      DFS(right_son.sources()[0], (inv_in >> 2) & 1),
                                      DFS(right_son.sources()[1], (inv_in >> 3) & 1)};
                aoi22_state.update();
                releaseState(state, aoi22_state);
            }
        }
    }
}

DpState DFS(Node p, bool is_inv) {
    if (p.dp_state(is_inv).defined()) { return p.dp_state(is_inv); }
    if (p.is_model()) {
        DLOG_ASSERT(!is_inv);
        auto &ori_state = p.dp_state(false);
        ori_state       = detail::makeDpState();
        for (auto &source : p.sources()) ori_state.sons().emplace_back(DFS(source, false));
        for (auto &source : p.sources()) source.dp_state(false).name() = source.name();
        ori_state.update();
        ori_state.min_cell_cnt()--;
        ori_state.max_depth()--;
        return p.dp_state(is_inv);
    }
    auto &ori_state = p.dp_state(false);
    auto &inv_state = p.dp_state(true);
    ori_state       = detail::makeDpState();
    inv_state       = detail::makeDpState();

    if (p.is_leaf()) {
        ori_state.name()         = p.name();
        ori_state.is_leaf()      = true;
        ori_state.max_depth()    = 0;
        ori_state.min_cell_cnt() = 0;

        inv_state.name()      = "virtually_inverse_" + p.name();
        inv_state.cell_type() = CellType::INV;
        inv_state.sons()      = {ori_state};
        inv_state.update();
        return p.dp_state(is_inv);
    }
    if (p.num_sources() == 0) {
        DLOG_ASSERT(!p.is_leaf());
        ori_state.name()         = p.name();
        ori_state.cell_type()    = p.cover_out_state() == 1 ? CellType::ONE : CellType::ZERO;
        ori_state.max_depth()    = 1;
        ori_state.min_cell_cnt() = 1;

        inv_state.name() = "virtually_inverse_" + p.name();
        if (p.num_fan_out() > 1) {
            inv_state.cell_type() = CellType::INV;
            inv_state.sons()      = {ori_state};
            inv_state.update();
        } else {
            inv_state.cell_type()    = p.cover_out_state() == 0 ? CellType::ONE : CellType::ZERO;
            inv_state.max_depth()    = 1;
            inv_state.min_cell_cnt() = 1;
        }
        return p.dp_state(is_inv);
    }

    if (p.num_sources() == 1) {
        bool is_same          = p.cover_a_state() == p.cover_out_state();
        Node son              = p.sources()[0];
        ori_state.name()      = p.name();
        ori_state.cell_type() = CellType::SAME;
        ori_state.sons()      = {DFS(son, !is_same)};
        ori_state.update();

        inv_state.name()      = "virtually_inverse_" + p.name();
        inv_state.cell_type() = CellType::INV;
        inv_state.sons()      = {ori_state};
        inv_state.update();
        return p.dp_state(is_inv);
    }

    // * original state
    transferState(p, ori_state, false);
    transferState(p, inv_state, true);

    auto t3_state = detail::makeDpState();
    auto t4_state = detail::makeDpState();

    t3_state.name()      = p.name() + "_HEAD_INV";
    t3_state.cell_type() = CellType::INV;
    t3_state.sons()      = {inv_state};
    t3_state.update();

    t4_state.name()      = p.name() + "_HEAD_INV";
    t4_state.cell_type() = CellType::INV;
    t4_state.sons()      = {ori_state};
    t4_state.update();

    releaseState(ori_state, t3_state);
    releaseState(inv_state, t4_state);

    //    if (p.num_fan_out() == 1) { inv_state = t4_state; }
    inv_state = t4_state;

    inv_state.name() = "virtually_inverse_" + inv_state.name();
    return p.dp_state(is_inv);
}

class DpStateTraveler {
public:
    using CellTypeMap = std::unordered_map<CellType, int64_t>;

    void travel(DpState st) {
        if (st.visited()) return;
        st.visited() = true;
        if (st.is_leaf()) return;
        for (const auto &son : st.sons()) travel(son);
        num_gates_++;
        oss_ << ".names";
        for (auto &son : st.sons()) oss_ << " " << son.name();
        oss_ << " " << st.name();
        oss_ << "       # " << ToString(st.cell_type()) << std::endl;
        cell_type_cnt_[st.cell_type()]++;
        switch (st.cell_type()) {
            case CellType::INV:
                DLOG_ASSERT(st.sons().size() == 1);
                oss_ << "0 1" << std::endl;
                break;
            case CellType::NAND2:
                DLOG_ASSERT(st.sons().size() == 2);
                oss_ << "11 0" << std::endl;
                break;
            case CellType::NOR2:
                DLOG_ASSERT(st.sons().size() == 2);
                oss_ << "00 1" << std::endl;
                break;
            case CellType::AOI21:
                DLOG_ASSERT(st.sons().size() == 3);
                oss_ << "000 1" << std::endl;
                oss_ << "010 1" << std::endl;
                oss_ << "100 1" << std::endl;
                break;
            case CellType::AOI12:
                DLOG_ASSERT(st.sons().size() == 3);
                oss_ << "000 1" << std::endl;
                oss_ << "010 1" << std::endl;
                oss_ << "001 1" << std::endl;
                break;
            case CellType::AOI22:
                DLOG_ASSERT(st.sons().size() == 4);
                oss_ << "0000 1" << std::endl;
                oss_ << "0010 1" << std::endl;
                oss_ << "0001 1" << std::endl;
                oss_ << "0100 1" << std::endl;
                oss_ << "0110 1" << std::endl;
                oss_ << "0101 1" << std::endl;
                oss_ << "1000 1" << std::endl;
                oss_ << "1010 1" << std::endl;
                oss_ << "1001 1" << std::endl;
                break;
            case CellType::ONE:
                DLOG_ASSERT(st.sons().empty());
                oss_ << "1" << std::endl;
                break;
            case CellType::ZERO:
                DLOG_ASSERT(st.sons().empty());
                oss_ << "0" << std::endl;
                break;
            case CellType::SAME:
                DLOG_ASSERT(st.sons().size() == 1);
                oss_ << "1 1" << std::endl;
                break;
            case CellType::UNDEFINED:
                DLOG_ASSERT(false);
        }
    }

private:
    CLASS_ARG(std::ostringstream, oss);
    CLASS_ARG(int64_t, num_gates) = 0;
    CLASS_ARG(CellTypeMap, cell_type_cnt);
};

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = true;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input blif file path>\n", argv[0]);
        exit(-1);
    }
    const std::string input_path(argv[1]);
    LOG(INFO) << "Running benchmarks: " << input_path;
    auto stopwatch = stopwatch::Stopwatch();
    stopwatch.start();
    Blif2DAGReader reader;
    DLOG_ASSERT(lorina::return_code::success == read_blif(input_path, reader));
    auto                model_state = DFS(reader.model_name(), false);
    DpStateTraveler     traveler;
    std::ostringstream &oss = traveler.oss();
    oss << ".model " << reader.model_name().name() << std::endl;
    oss << ".inputs";
    for (auto leaf : reader.leaves()) { oss << " " << leaf.dp_state(false).name(); }
    oss << std::endl;
    oss << ".outputs";
    for (auto son : model_state.sons()) { oss << " " << son.name(); }
    oss << std::endl;
    for (const auto &son : model_state.sons()) traveler.travel(son);
    oss << ".end" << std::endl;
    std::cout << oss.str();
    LOG(INFO) << "# of gate: " << traveler.num_gates();
    for (auto &iter : traveler.cell_type_cnt())
        LOG(INFO) << std::setfill(' ') << std::setw(10) << ToString(iter.first) << ": "
                  << iter.second;
    LOG(INFO) << "max depth: " << model_state.max_depth();
    LOG(INFO) << std::setw(10)
              << "Total Elapsed time: " << stopwatch.elapsed<stopwatch::microseconds>() / 1000.0
              << " ms.";
    return 0;
}
