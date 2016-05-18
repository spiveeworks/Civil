#include <Civil/EventObjects.h>
#include <Civil/Entity.h>
#include <Civil/Space.h>

extern EventQueue *action;

namespace EventObjects
    {

ChildEvent See::Execute()
{
    if (what)
        who.Show(world, where, what, why);
    return END_CHAIN;
}

See::See (Space& world_c, SpaceIndex where_c, Entity& who_c, EyesWhy why_c):
    world(world_c), where(where_c), who(who_c), why(why_c),
    what(world.getImage(who, where))
    {}

ChildEvent SeeAll::Execute()
{
    for (Sight const &each: what)
    {
        if (each.second)
            who.Show(world, each.first, each.second, why); // how many times do I really need to check for NULLIMGs
    }
    return END_CHAIN;
}

SeeAll::SeeAll (Space& world_c, Entity& who_c, EyesWhy why_c):
        world(world_c), who(who_c), why(why_c),
        what( world_c.getAllImages(who_c) )
    {}

ChildEvent Sleep::Execute()
    {return self.React(where);}

ChildEvent Contact::Execute()
{
    Entity *target = where[target_loc];
    Component *grab_component = self.GetComponent(grab_with);
    if(grab_component && target && target->isGrabbedBy(signal)) // if grab_with is valid and the target can be picked up
    {
        where.GiveTo(*grab_component, target_loc);
        action->QueueEvent(new GrabAct{where, *target, signal});
        return ChildEvent{0, new GrabReact{where, self, target->GrabReaction(signal)}};
    }
    else if (target)
    {
        action->QueueEvent(new TouchAct{where, *target, signal}); // objects don't know location of target's origin?
        return ChildEvent{0, new TouchReact{where, self, target->TouchReaction(signal)}};
    }
    else
        return ChildEvent{0, new TouchReact{where, self, NULLTOUCH}};
}

ChildEvent TouchAct::Execute()
    {return self.TouchAct(where, signal);}
    
ChildEvent TouchReact::Execute()
    {return self.TouchReact(where, signal);}
    
ChildEvent GrabAct::Execute()
    {return self.GrabAct(where, signal);}
    
ChildEvent GrabReact::Execute()
    {return self.GrabReact(where, signal);}

    }
