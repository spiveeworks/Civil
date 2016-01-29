#pragma once
#include <map>

class Entity;
class EventQueue;

typedef unsigned char SpaceIndex;

class Space {
private:
    typedef std::map<SpaceIndex, Entity*> map_type; //ideally forward_list :P
    map_type ents;
    SpaceIndex next;
protected:
    Entity* withdraw (SpaceIndex const &it);
    SpaceIndex deposit (Entity *toad);
    
public:

//Container Things
    Entity* operator[] (SpaceIndex at);
    void Destroy (EventQueue& action, SpaceIndex at);
    SpaceIndex AddEntity (EventQueue &action, Entity *toad);
    SpaceIndex GiveTo (EventQueue &action, Space &to, SpaceIndex where_from);
    SpaceIndex TakeFrom (EventQueue &action, Space &from, SpaceIndex where_from)
        {return from.GiveTo(action, *this, where_from);}
    
//Observation Things
    void SeeAll (EventQueue &action, Entity &seer);
    void Appear (EventQueue &action, SpaceIndex seen_at);
    void Disappear (EventQueue &action, SpaceIndex seen_at);
};
