#pragma once
#include "Event.h"

#include "EntitySignals.h"
typedef unsigned char SpaceIndex;

class Entity;

namespace EventObjects
    {

struct SeeEvent: public Event
{
    SpaceIndex where;
    Image what;
    Entity* whom;
    
    void Execute(EventQueue& action);
    SeeEvent (SpaceIndex where_c, Image what_c, Entity* whom_c):
        where(where_c), what(what_c), whom(whom_c)
        {}
};
//maybe make a batch alternative so that looking doesn't cause event spam
 // by having an event that references a space and just asks the space then and there
 // but this complicates things if vision is possible beyond current spaces
 // maybe something that uses entity iterators of some kind
 // but that's templatey and events are polymorphicy

struct UnseeEvent: public SeeEvent
{
    void Execute (EventQueue& action) {} // crying
    UnseeEvent (SpaceIndex where_c, Image what_c, Entity* whom_c):
        SeeEvent (where_c, what_c, whom_c)
        {}
};




    }
