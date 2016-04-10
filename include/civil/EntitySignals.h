#pragma once

enum Image 
{
    NULLIMG = 0,
    PLANT,
    WATER
};

enum ObserveOrgan 
{
    NULLORGN = 0,
    EYES
};

enum Touch 
{
    NULLTOUCH = 0
};

enum GrasperOrgan 
{
    NULLGRASP = 0,
    SAPIHAND
};

enum EyesWhy 
{
    INTERNAL = 0, //when an object deliberately checks its surroundings such as when it is instantiated
    CHANGE, //when an object changes visually, or is replaced by another object
    UNBLOCKED, //when an object comes into view due to player motion (or another object)
    BLOCKED, //when an object leaves view due to player motion (or another object)
    APPARITION, //when an object is instantiated
    DISAPPARITION, //when an object is annihilated
    ENTRANCE, //when an object moves here from somewhere else
    VACATION, //when an object moves somewhere else from here
    PLACEMENT, //when an object is placed by another object, usually from grasp
    CONSUMPTION, //when an object is grasped, or possibly eaten never to return
};

constexpr bool IsHere (EyesWhy why)
{
    return why == 1 || why % 2 == 0;
}

