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
    See (Space& world_c, SpaceIndex where_c, Entity& who_c, EyesWhy why_c); // overload this to take a pointer to the object for cases when it has already been popped?
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

struct ContactBase: public Event
{
    Space &where;
    Entity& self;
    Touch signal;
    //SpaceIndex from_loc; //is necessary ty
    ContactBase(Space &where_c, Entity &self_c, Touch signal_c):
        where(where_c),
        self(self_c),
        signal(signal_c)
        {}
};

struct Contact: public ContactBase
{
    SpaceIndex target_loc;
    CompId grab_with;
    Contact(Space &where_c, Entity &self_c, Touch signal_c, SpaceIndex target_loc_c, CompId grab_with_c):
        ContactBase(where_c, self_c, signal_c),
        target_loc(target_loc_c),
        grab_with(grab_with_c)
        {}
    ChildEvent Execute();
};

struct TouchAct: public ContactBase
{
    TouchAct(Space &where_c, Entity &self_c, Touch signal_c):
        ContactBase(where_c, self_c, signal_c)
        {}
    ChildEvent Execute();
};

struct TouchReact: public ContactBase
{
    TouchReact(Space &where_c, Entity &self_c, Touch signal_c):
        ContactBase(where_c, self_c, signal_c)
        {}
    ChildEvent Execute();
};

struct GrabAct: public ContactBase
{
    //CompId grab_with; // maybe that would be useful if objects need to distinguish between simultaneous actions?
    GrabAct(Space &where_c, Entity &self_c, Touch signal_c):
        ContactBase(where_c, self_c, signal_c)
        {}
    ChildEvent Execute();
};

struct GrabReact: public ContactBase
{
    GrabReact(Space &where_c, Entity &self_c, Touch signal_c):
        ContactBase(where_c, self_c, signal_c)
        {}
    ChildEvent Execute();
};

    }
