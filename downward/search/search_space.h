#ifndef SEARCH_SPACE_H
#define SEARCH_SPACE_H

#include "state.h" // for state_var_t
#include <vector>
#include <ext/hash_map>
#include "state.h"
#include "state_proxy.h"
#include "search_node_info.h"

#include <vector>

class Operator;
class State;
class StateProxy;

class SearchNode {
    state_var_t *state_buffer;
    SearchNodeInfo &info;
public:
    SearchNode(state_var_t *state_buffer_, SearchNodeInfo &info_);

    state_var_t *get_state_buffer() {
        return state_buffer;
    }
    State get_state() const;

    bool is_new() const;
    bool is_open() const;
    bool is_closed() const;
    bool is_dead_end() const;

    int get_g() const;
    int get_h() const;
    const state_var_t *get_parent_buffer() const;

    void open_initial(int h);
    void open(int h, const SearchNode &parent_node,
              const Operator *parent_op);
    void reopen(const SearchNode &parent_node,
                const Operator *parent_op);
    void update_parent(const SearchNode &parent_node,
                       const Operator *parent_op);
    void close();
    void mark_as_dead_end();

    void dump();
};


class SearchSpace {
    class HashTable;
    HashTable *nodes;
public:
    SearchSpace();
    ~SearchSpace();
    int size() const;
    SearchNode get_node(const State &state);
    void trace_path(const State &goal_state,
                    std::vector<const Operator *> &path) const;

    void dump();
    void statistics() const;
};

#endif
