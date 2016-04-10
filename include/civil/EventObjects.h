#pragma once
#include <vector>

#include "Event.h"
#include "EntitySignals.h"
typedef unsigned char SpaceIndex;
typedef std::pair<SpaceIndex, Image> Sight;

class Entity;
class Space;


namespace EventObjects
    {

struct See: public Event
{
    Space& world;
    SpaceIndex where;
    Entity& who;
    EyesWhy why;
    Image what;
    
    ChildEvent Execute();
    See (Space& world_c, SpaceIndex where_c, Entity& who_c, EyesWhy why_c);
};

struct SeeAll: public Event
{
    Space& world;
    Entity& who;
    EyesWhy why;
    std::vector<Sight> what;
    
    ChildEvent Execute();
    SeeAll (Space& world_c, Entity& who_c, EyesWhy why_c);
};

struct Sleep: public Event
{
    Space& where;
    Entity& self;
    
    ChildEvent Execute();
    Sleep (Space& where_c, Entity& self_c):
        where(where_c), self(self_c)
        {}
};

    }
