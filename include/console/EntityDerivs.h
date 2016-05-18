#pragma once
#include <Civil/Entity.h>

class TextIO;

template<Image I>
struct VisibleEntity: public Entity 
{
    Image render (ObserveOrgan const &seer)
        {return seer ? I : NULLIMG;}
};

template<Image I, Touch FEELS_LIKE = INERT>
class TangibleEntity: public VisibleEntity<I>
{
    Touch TouchReaction(Touch signal)
        {return signal ? FEELS_LIKE : NULLTOUCH;}
};

template<Image I, Touch FEELS_LIKE = INERT, Touch GRAB_COND = GRAB>
class GraspableEntity: public VisibleEntity<I>
{
    bool isGrabbedBy(Touch signal)
        {return signal == GRAB_COND;}
    Touch TouchReaction(Touch signal)
        {return isGrabbedBy(signal) ? GRABBED : signal ? FEELS_LIKE : NULLTOUCH;}
};

class Plant: public TangibleEntity<PLANT> 
    {};

class Seed: public GraspableEntity<SEED> 
    {};

class Human: public Entity 
{
public:
    TextIO* mind; // possibly better to use an interface to IO. In fact EntityDerivs shouldn't have so much to do with console.
    Component hand;
    void Show(Space& place, SpaceIndex location, Image image, EyesWhy detail);
    ChildEvent React (Space& place);
    ChildEvent GrabReact (Space& place, Touch signal); // currently no location is given
    ChildEvent TouchReact (Space& place, Touch signal); // currently no location is given
    ObserveOrgan Eyes()
        {return EYES;}
    Component* GetComponent(CompId which)
        {return which ? nullptr : &hand;}
        
    Human(TextIO &mind_r): mind(&mind_r) {}
};

