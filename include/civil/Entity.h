#pragma once
#include <utility>

#include "EntitySignals.h"
typedef signed long time;
class Space;
typedef unsigned char SpaceIndex;

#include "Event.forward"

class Component;

class Entity 
{
public:
    virtual ~Entity () {}
    
    virtual Image render (ObserveOrgan const &seer) 
        {return NULLIMG;} //note: invisible by default
    virtual ObserveOrgan Eyes() 
        {return NULLORGN;}//blind by default
    virtual Component* GetComponent (CompId which)
        {return nullptr;}
    virtual bool isGrabbedBy (Touch signal)
        {return false;}
        
    //note: Eyes() must be overwritten before any entities need reveal themselves
    virtual void Show(Space& place, SpaceIndex location, Image image, EyesWhy detail) 
        {}; 
    virtual ChildEvent React(Space& place) // need a broader name 
        {return END_CHAIN;};
    
    virtual Touch TouchReaction(Touch signal) 
        {return NULLTOUCH;}
    virtual Touch GrabReaction(Touch signal) 
        {return TouchReaction(signal);}
    
    virtual ChildEvent TouchAct (Space& where, Touch signal)
        {return END_CHAIN;}
    virtual ChildEvent GrabAct (Space& where, Touch signal)
        {return END_CHAIN;}
    virtual ChildEvent TouchReact (Space& where, Touch signal)
        {return END_CHAIN;}
    virtual ChildEvent GrabReact (Space& where, Touch signal)
        {return END_CHAIN;}
};

/*
 * Entity needs to start using species classes and putting responsibilities such as Eyes() there
 * 
 * once entities are dynamically defined render() will also need to belong to species, with entities being passed to the species
 * this applies to any object-specific behaviour (basically all object behaviour)
 */

struct Component
{
    Entity *content = nullptr;
    /*virtual*/ Entity* SwapContent(Entity *new_content)
    {
        std::swap(content, new_content);
        return new_content;
    }
};

