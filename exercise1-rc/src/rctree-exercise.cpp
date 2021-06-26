/**
 * @file rctree-exercise.cpp
 * @author Zizheng Guo
 * @brief reference implementation of exercise 1.
 */

#include "rctree-simplified.hpp"

void Rct::update_load(RctNode *u, RctNode *parent) {
    // TO IMPLEMENT
    u->load = 0;
    for (auto &edge : u->fanout) {
        auto &to = edge->to;
        if (to == parent) continue;
        update_load(to, u);
        u->load += to->load;
    }
    u->load += u->ncap;
}

void Rct::update_delay(RctNode *u, RctNode *parent) {
    // TO IMPLEMENT
    for (auto &edge : u->fanout) {
        auto &to  = edge->to;
        auto &res = edge->res;
        if (to == parent) continue;
        to->delay = u->delay + res * to->load;
        update_delay(to, u);
    }
}

void Rct::update_ldelay(RctNode *u, RctNode *parent) {
    // TO IMPLEMENT
    u->ldelay = 0;
    for (auto &edge : u->fanout) {
        auto &to = edge->to;
        if (to == parent) continue;
        update_ldelay(to, u);
        u->ldelay += to->ldelay;
    }
    u->ldelay += u->ncap * u->delay;
}

void Rct::update_response(RctNode *u, RctNode *parent) {
    // TO IMPLEMENT
    for (auto &edge : u->fanout) {
        auto &to  = edge->to;
        auto &res = edge->res;
        if (to == parent) continue;
        to->beta = u->beta + res * to->ldelay;
        to->impulse = to->beta * 2. - to->delay * to->delay;
        update_response(to, u);
    }
}
