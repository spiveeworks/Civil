#pragma once
#include <Civil/Entity.h>

class TextIO;

template<Image I>
struct VisibleEntity: public Entity 
{
    Image render (ObserveOrgan const &seer)
        {return seer ? I : NULLIMG;}
};

class GraspableEntity: public Entity 
{
    
};

class Plant: public VisibleEntity<PLANT> 
{
    int growth_stage;
    bool watered;
};

class WaterGlob: public VisibleEntity<WATER> 
{
    int volume;
};

class Human: public Entity 
{
public:
    TextIO* mind; // possibly better to use an interface to IO. In fact EntityDerivs shouldn't have so much to do with console.
    void Show(Space& place, SpaceIndex location, Image image, EyesWhy detail);
    ChildEvent React (Space& place);
    ObserveOrgan Eyes()
        {return EYES;}
        
    Human(TextIO &mind_r): mind(&mind_r) {}
};

