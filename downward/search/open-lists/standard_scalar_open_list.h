#ifndef STANDARD_SCALAR_OPENLIST_H
#define STANDARD_SCALAR_OPENLIST_H

#include "open_list.h"

#include <deque>
#include <map>
#include <vector>
#include <utility>

class ScalarEvaluator;

template<class Entry>
class StandardScalarOpenList : public OpenList<Entry> {
    
	typedef std::deque<Entry> Bucket;

    std::map<int, Bucket> buckets;
    int size;
    mutable int lowest_bucket;
	
	ScalarEvaluator *evaluator;
	int last_evaluated_value;
	bool dead_end;
	bool dead_end_reliable;

protected:
    ScalarEvaluator* get_evaluator() { return evaluator; }

public:
    StandardScalarOpenList(ScalarEvaluator *eval);
    ~StandardScalarOpenList();
	
    int insert(const Entry &entry);
    Entry remove_min();
    bool empty() const;
	
	void evaluate(int g, bool preferred);
	bool is_dead_end() const;
	bool dead_end_is_reliable() const;
};

#include "standard_scalar_open_list.cc"

// HACK! Need a better strategy of dealing with templates, also in the Makefile.

#endif
