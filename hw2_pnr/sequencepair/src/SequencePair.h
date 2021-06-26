#ifndef SP_SEQUENCE_PAIR_H_
#define SP_SEQUENCE_PAIR_H_

#include "global.h"

PROJECT_NAMESPACE_BEGIN

class SequencePair {
    public:
        SequencePair() {}
        SequencePair(int num) : num_modules_(num) {
            pos_seq_.reserve(num);
            neg_seq_.reserve(num);
        }

        /* getter */
        int num_modules() { return num_modules_; }
        const std::vector<int>& pos_seq() const { return pos_seq_; }
        const std::vector<int>& neg_seq() const { return neg_seq_; }

        /* setter */
        void set_num_modules(int num) { num_modules_ = num; }
        void add_pos_seq(int idx) { pos_seq_.emplace_back(idx); }
        void add_neg_seq(int idx) { neg_seq_.emplace_back(idx); }
    private:
        int num_modules_;
        std::vector<int> pos_seq_;
        std::vector<int> neg_seq_;
};

PROJECT_NAMESPACE_END

#endif // SP_SEQUENCE_PAIR_H_
