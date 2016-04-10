#include <Civil/EventObjects.h>
#include <Civil/Entity.h>
#include <Civil/Space.h>

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
    what(world.getImage(who_c, where))
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

    }
