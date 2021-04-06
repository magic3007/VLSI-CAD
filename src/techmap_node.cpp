#include "techmap_node.h"
#include "glog/logging.h"

std::string &      Node::name() { return impl_->name(); }
std::vector<Node> &Node::sources() { return impl_->sources(); }
bool &             Node::is_model() { return impl_->is_model(); }
Node::IndexType &  Node::num_fan_out() { return impl_->num_fan_out(); }
DpState &Node::dp_state(bool is_inv) { return is_inv ? impl_->ori_state() : impl_->inv_state(); }
Node::BinaryFunc &Node::ori_func() { return impl_->ori_func(); }
Node::IndexType & Node::cover_a_state() { return impl_->cover_a_state(); }
Node::IndexType & Node::cover_b_state() { return impl_->cover_b_state(); }
Node::IndexType & Node::cover_out_state() { return impl_->cover_out_state(); }
Node::IndexType   Node::num_sources() const { return impl_->sources().size(); }
bool &            Node::is_leaf() { return impl_->is_leaf(); }

long double &         DpState::min_cell_cnt() { return impl_->min_cell_cnt(); }
int64_t &             DpState::max_depth() { return impl_->max_depth(); }
std::string &         DpState::name() { return impl_->name(); }
std::vector<DpState> &DpState::sons() { return impl_->sons(); }
CellType &            DpState::cell_type() { return impl_->cell_type(); }
void                  DpState::update() { impl_->update(); }
bool &                DpState::is_leaf() { return impl_->is_leaf(); }
bool &                DpState::visited() { return impl_->visited(); }

void DpStateImpl::update() {
    DCHECK(!sons_.empty());
    min_cell_cnt_ = 0;
    max_depth_    = 0;
    for (auto son : sons_) {
        min_cell_cnt_ += son.min_cell_cnt();
        max_depth_ = std::max(max_depth_, son.max_depth());
    }
    min_cell_cnt_ += 1;
    max_depth_ += 1;
}
