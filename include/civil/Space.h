#pragma once
#include <map>
#include <vector>

#include <Civil/EntitySignals.h>

class Entity;
class Component;
class EventQueue;

typedef unsigned char SpaceIndex;

class SpaceGeometry 
{
protected:
    typedef std::map<SpaceIndex, Entity*> map_type; //ideally forward_list :P (actually that might not work at all when iterators start dangling)
    map_type ents; // maps indeces to entity references.
    SpaceIndex next = 0; // lowest index never used
    Entity* withdraw (SpaceIndex it); // removes and returns an entity reference, rendering it no longer within
    SpaceIndex deposit (Entity *toad); // adds an entity reference, returning the index at which it can now be found
    
public:

//Container Things
    Entity* operator[] (SpaceIndex at) const; // returns either the entity reference at a location, or a nullptr
};
/*
class SpaceTransmit: public SpaceGeometry 
{
public:
    void SeeAllPassive (Entity &seer); // shows every object to seer
    void VisualUpdatePassive (SpaceIndex seen_at, EyesWhy detail); // Acknowledges a visual change of seen_at to every entity with eyes nearby
};*/
class SpaceTransmit: public SpaceGeometry
{
public:
    typedef std::pair<SpaceIndex, Image> Sight;

    Image getImage (Entity& seer, SpaceIndex seen_at) const;
    std::vector<Sight> getAllImages (Entity& seer) const; // how many ampersands do I need and where for this to be efficient? (how does move() work o.o)
};

class Space: public SpaceTransmit 
{
public:
    //void Destroy (SpaceIndex at); // queues an event for an object to disappear, and then for the object to be deleted
    //at the moment 'destroying' objects can be done by delete RemoveEntity(at, DISAPPARITION)
    SpaceIndex AddEntity (Entity *toad, EyesWhy why_seen = APPARITION); // adds an object to the space and then queues relevant events for it to appear visually
    Entity* RemoveEntity (SpaceIndex at, EyesWhy why_unseen = DISAPPARITION); // pops an object from the space and then queues relevant events for it to disappear visually
    SpaceIndex GiveTo (Space &to, SpaceIndex where_from); // removes an entity from here and adds it to another space using above methods (thus the object will visually disappear and reappear from the respective spaces)
    SpaceIndex TakeFrom (Space &from, SpaceIndex where_from) // opposite of GiveTo() :P
        {return from.GiveTo(*this, where_from);}
    
    SpaceIndex GiveTo (Component &to, SpaceIndex where_from); //Take an entity out of the space, put it in the component, queue visual events, and then effectively run TakeFrom() with the previous content of the component 
    SpaceIndex TakeFrom (Component &from); // remove the contents of the component and add it to the space, queueing visual events
    
    void Debug();
};