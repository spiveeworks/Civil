

class script
{
};

class trigger
{
    typedef std::vector<trigger> list;
    typedef list::size_type index;
};

class condition
{
    typedef std::vector<condition> list;
    typedef list::size_type index;
};

struct mechanism
{
    trigger::index trigger_id;
    std::vector<bool> condition_set;
    script todo;
    
    typedef std::vector<mechanism> list;
    typedef list::size_type index;
};

class mechanisms
{
    trigger::list triggers;
    condition::list conditions;
    mechanism::list mechanisms;
};

class exec_instance
{
    std::vector<bool> mechanisms_run;
};