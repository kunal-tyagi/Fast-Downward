#ifndef CEGAR_CEGAR_HEURISTIC_H
#define CEGAR_CEGAR_HEURISTIC_H

#include "../heuristic.h"
#include "../option_parser.h"

class State;

namespace cegar_heuristic {

class Abstraction;

class CegarHeuristic : public Heuristic {
    int max_states_offline;
    const int h_updates;
    const bool search;
    Abstraction *abstraction;
protected:
    virtual void initialize();
    virtual int compute_heuristic(const State &state);
public:
    explicit CegarHeuristic(const Options &options);
    ~CegarHeuristic();
};
}

#endif
