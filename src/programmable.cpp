#import <Civil/Programmable.h>

extern QueueEvent action;

ChildEvent EntityMachine::EntityState::Script::execute
    () const
{
    ChildEvent reaction = END_CHAIN;
    unsigned i = 0;
    while (i < script.size())
    {
        switch(script[i])
        {
            
            case TOUCH:
            {
                action->QueueEvent();
            } 
            break;
        }
    }
    return END_CHAIN;
}


void EntityMachine::EntityState::SightScript::operator() 
    (Instance& self, Space& area, SpaceIndex seen, Image signal, EyesWhy detail) const
{
    
}


ChildEvent EntityMachine::EntityState::SleepScript::operator() 
    (Instance&, Space&) const;


ChildEvent EntityMachine::EntityState::ContactScript::operator() 
    (Instance&, Space&, Touch) const;


