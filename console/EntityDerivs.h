#pragma once
#include <Civil/Entity.h>

template<Image I>
class VisibleEntity: public Entity {
    Image render (ObserveOrgan const &seer)
        {return seer ? I : NULLIMG;}
};

class GraspableEntity: public Entity {
    
};

class HM_Plant: public VisibleEntity<PLANT> {
    int growth_stage;
    bool watered;
};

class HM_WaterGlob: public VisibleEntity<WATER> {
    int volume;
};

class HM_Human: public Entity {
    void Show(SpaceIndex location, Image image, EyesWhy detail);
    ObserveOrgan Eyes()
        {return EYES;}
};
