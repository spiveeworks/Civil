#pragma once
#include <map>

#include <Civil/EntitySignals.h>

class Entity;
class EventQueue;

typedef unsigned char SpaceIndex;

class SpaceGeometry {
protected:
    typedef std::map<SpaceIndex, Entity*> map_type; //ideally forward_list :P (actually that might not work at all when iterators start dangling)
    map_type ents; // maps indeces to entity references.
    SpaceIndex next; // lowest index never used
    Entity* withdraw (SpaceIndex const &it); // removes and returns an entity reference, rendering it no longer within
    SpaceIndex deposit (Entity *toad); // adds an entity reference, returning the index at which it can now be found
    
public:

//Container Things
    Entity* operator[] (SpaceIndex at) const; // returns either the entity reference at a location, or a nullptr
};

class SpaceTransmit: public SpaceGeometry {
public:
    void SeeAll (Entity &seer); // shows every object to seer
    void VisualUpdate (SpaceIndex seen_at, EyesWhy detail); // Acknowledges a visual change of seen_at to every entity with eyes nearby
};

class Space: public SpaceTransmit {
public:
    void Destroy (SpaceIndex at); // queues an event for an object to disappear, and then for the object to be deleted
    SpaceIndex AddEntity (Entity *toad); // adds an object to the space and then queues an event for it to appear
    SpaceIndex GiveTo (Space &to, SpaceIndex where_from); // removes from here, queues disappear, and then puts in another space, queueing appear
    SpaceIndex TakeFrom (Space &from, SpaceIndex where_from) // opposite of GiveTo() :P
        {return from.GiveTo(*this, where_from);}
};