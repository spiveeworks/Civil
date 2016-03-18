#pragma once
#include "EntitySignals.h"

class Entity {
public:
    virtual ~Entity () {}
    
    virtual Image render (ObserveOrgan const &seer) 
        {return NULLIMG;} //note: invisible by default
    virtual ObserveOrgan Eyes() 
        {return NULLORGN;}//blind by default
        
    //note: Eyes() must be overwritten before any entities need reveal themselves
    virtual void Show(SpaceIndex location, Image image, EyesWhy detail) 
        {}; 
};

/*
 * Entity needs to start using species classes and putting responsibilities such as Eyes() there
 * 
 * once entities are dynamically defined render() will also need to belong to species, with entities being passed to the species
 * this applies to any object-specific behaviour (basically all object behaviour)
 */

struct Grasper {
    Entity *grasp;
    virtual GrasperOrgan type() const
        {return NULLGRASP;}
};
