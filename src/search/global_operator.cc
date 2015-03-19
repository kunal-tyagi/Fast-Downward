#include "global_operator.h"

#include "globals.h"
#include "utilities.h"

#include <cassert>
#include <iostream>

using namespace std;


static void check_fact(int var, int val) {
    if (!in_bounds(var, g_variable_domain)) {
        cerr << "Invalid variable id: " << var << endl;
        exit_with(EXIT_INPUT_ERROR);
    }
    if (val < 0 || val >= g_variable_domain[var]) {
        cerr << "Invalid value for variable " << var << ": " << val << endl;
        exit_with(EXIT_INPUT_ERROR);
    }
}

GlobalCondition::GlobalCondition(istream &in) {
    in >> var >> val;
    check_fact(var, val);
}

GlobalCondition::GlobalCondition(int variable, int value)
    : var(variable),
      val(value) {
    check_fact(var, val);
}

// TODO if the input file format has been changed, we would need something like this
// Effect::Effect(istream &in) {
//    int cond_count;
//    in >> cond_count;
//    for (int i = 0; i < cond_count; ++i)
//        cond.push_back(Condition(in));
//    in >> var >> post;
//}

GlobalEffect::GlobalEffect(int variable, int value, const vector<GlobalCondition> &conds)
    : var(variable),
      val(value),
      conditions(conds) {
    check_fact(var, val);
}

void GlobalOperator::read_pre_post(istream &in) {
    int cond_count, var, pre, post;
    in >> cond_count;
    vector<GlobalCondition> conditions;
    conditions.reserve(cond_count);
    for (int i = 0; i < cond_count; ++i)
        conditions.push_back(GlobalCondition(in));
    in >> var >> pre >> post;
    if (pre != -1)
        check_fact(var, pre);
    check_fact(var, post);
    if (pre != -1)
        preconditions.push_back(GlobalCondition(var, pre));
    effects.push_back(GlobalEffect(var, post, conditions));
}

GlobalOperator::GlobalOperator(istream &in, bool axiom) {
    marked = false;

    is_an_axiom = axiom;
    if (!is_an_axiom) {
        check_magic(in, "begin_operator");
        in >> ws;
        getline(in, name);
        int count;
        in >> count;
        for (int i = 0; i < count; ++i)
            preconditions.push_back(GlobalCondition(in));
        in >> count;
        for (int i = 0; i < count; ++i)
            read_pre_post(in);

        int op_cost;
        in >> op_cost;
        cost = g_use_metric ? op_cost : 1;

        g_min_action_cost = min(g_min_action_cost, cost);
        g_max_action_cost = max(g_max_action_cost, cost);

        check_magic(in, "end_operator");
    } else {
        name = "<axiom>";
        cost = 0;
        check_magic(in, "begin_rule");
        read_pre_post(in);
        check_magic(in, "end_rule");
    }
}

void GlobalOperator::set_cost(int new_cost) {
    assert(new_cost >= 0);
    cost = new_cost;
}

void rename_fact_in_conditions(int variable, int before, int after, vector<GlobalCondition> &conditions) {
    // TODO: Break out of loop when var > variable once conditions are sorted.
    for (size_t j = 0; j < conditions.size(); ++j) {
        GlobalCondition &cond = conditions[j];
        if (cond.var == variable && cond.val == before) {
            cond.val = after;
            // Each variable appears in at most one effect condition.
            break;
        }
    }
}

void GlobalOperator::rename_fact(int variable, int before, int after) {
    // TODO: Break out of loop when var > variable once conditions are sorted.
    rename_fact_in_conditions(variable, before, after, preconditions);
    for (size_t i = 0; i < effects.size(); ++i) {
        GlobalEffect &effect = effects[i];
        if (effect.var == variable) {
            if (effect.val == before)
                effect.val = after;
        }
        rename_fact_in_conditions(variable, before, after, effect.conditions);
    }
}

void GlobalOperator::keep_single_effect(int var, int value) {
    effects.clear();
    vector<GlobalCondition> effect_conditions;
    GlobalEffect effect(var, value, effect_conditions);
    effects.push_back(effect);
}

void GlobalCondition::dump() const {
    cout << g_variable_name[var] << ": " << val;
}

void GlobalEffect::dump() const {
    cout << g_variable_name[var] << ":= " << val;
    if (!conditions.empty()) {
        cout << " if";
        for (size_t i = 0; i < conditions.size(); ++i) {
            cout << " ";
            conditions[i].dump();
        }
    }
}

void GlobalOperator::dump() const {
    cout << name << ":";
    for (size_t i = 0; i < preconditions.size(); ++i) {
        cout << " [";
        preconditions[i].dump();
        cout << "]";
    }
    for (size_t i = 0; i < effects.size(); ++i) {
        cout << " [";
        effects[i].dump();
        cout << "]";
    }
    cout << endl;
}
