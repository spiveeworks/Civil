
typedef unsigned char ScriptElement;

enum ScriptOp 
{
    
};

struct EntityMachine
{
public:
    struct EntityState
    {
        std::map<ObserveOrgan, Image> image_exceptions;
        Image default_image = NULLIMG;
        
        ObserveOrgan eyes = NULLORGN;
        
        std::set lift_if;
        
        class Script
        {
            vector<unsigned char> script;
        }
        
        class SightScript: Script
        {
            void operator() (Instance&, Space&, SpaceIndex, Image, EyesWhy) const;
        }
            show;
        class SleepScript: Script
        {
            ChildEvent operator() (Instance&, Space&) const;
        }
            react;
        class ContactScript: Script
        {
            ChildEvent operator() (Instance&, Space&, Touch) const;
        }
            touch_active, 
            touch_reactive, 
            grab_active, 
            grab_reactive;
    };
    std::vector<EntityState> state_info;
    CompId comp_num;
};


class Instance: public Entity // I should branch C++ entities in the repo so that the main branch has Instances as superclass
{
public:
    EntityMachine& program;
    EntityMachine::EntityState const *current_state;
    
    std::vector<Component> components;
    
    Instance (EntityMachine& program_c): 
        program(program_c)
        {}
    
    Image render (ObserveOrgan const &seer) 
    {
        if (!seer) 
            return NULLIMG;
        auto it = current_state->image_exceptions.find(seer);
        if (it != current_state->image_exceptions.end())
            return it->second;
        return current_state->default_image;
    }
    
    ObserveOrgan Eyes() 
        {return current_state->eyes;}
    Component* GetComponent (CompId which)
        {return &components[which];}
    bool isGrabbedBy (Touch signal)
        {return current_state->lift_if.count(signal);}
        
    //note: Eyes() must be overwritten before any entities need reveal themselves
    void Show(Space& place, SpaceIndex location, Image image, EyesWhy detail) 
        {current_state->show(*this, place, location, image, detail);}
    ChildEvent React(Space& place) // need a broader name 
        {return current_state->react(*this, place);}
    
    Touch TouchReaction(Touch signal) 
        {return current_state->touch_passive(;}
    Touch GrabReaction(Touch signal) 
        {return TouchReaction(signal);}
    
    ChildEvent TouchAct (Space& where, Touch signal)
        {return current_state->touch_active(*this, where, signal);}
    ChildEvent GrabAct (Space& where, Touch signal)
        {return current_state->grab_active(*this, where, signal);}
    ChildEvent TouchReact (Space& where, Touch signal)
        {return current_state->touch_reactive(*this, where, signal);}
    ChildEvent GrabReact (Space& where, Touch signal)
        {return current_state->grab_reactive(*this, where, signal);}
};
