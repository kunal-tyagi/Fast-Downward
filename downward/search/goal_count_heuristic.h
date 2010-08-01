#ifndef GOAL_COUNT_HEURISTIC_H
#define GOAL_COUNT_HEURISTIC_H

#include "heuristic.h"

class GoalCountHeuristic : public Heuristic {
protected:
    virtual void initialize();
    virtual int compute_heuristic(const State &state);
public:
    GoalCountHeuristic();
    ~GoalCountHeuristic();

    static ScalarEvaluator *create(const std::vector<std::string> &config, 
                                   int start, int &end);
};

#endif
