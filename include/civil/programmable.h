

class script
{
};

class condition // to begin with implement as list of comparisons, any of which can be met. A list of these, all of which must be met, gives potentially anything, but at what cost?
//triggers are just conditions about visual data instead of internal data
{
    typedef std::vector<condition> list;
    typedef list::size_type index;
};


struct mechanism
{
    std::vector<condition::index> conds_ex, conds_in;
    PropertyFormat *trigger; // e.g. visual_data format for visual triggers
    script todo;
    
    typedef std::vector<mechanism> list;
    typedef list::size_type index;
};

class mechanisms
{
    std::map<PropertyFormat*, condition::list> conditions;
    
    mechanism::list mechanisms;
    struct exec_instance
    {
        std::vector<bool> mechanisms_run;
        std::vector<bool> triggers;
        exec_instance(mechanism_num, triggers_c):
            mechanisms_run(mechanism_num),
            triggers(triggers_c),
            {}
    };
    exec_instance begin(/*tiggerargs*/) const //not to be confused with the STL usage of .begin()
    {
        std::vector<bool> trigger_states;
        trigger_states.reserve(triggers.size());
        for (trigger const &each: triggers)
            trigger_states.push_back(each.test(/*triggerargs*/));
        return exec_instance(mechanisms.size(), trigger_states);
    }
};
