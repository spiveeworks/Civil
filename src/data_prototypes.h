
#line 1 "event.dpr"
typedef FxPtNum<-4, signed long> time;

class Event;
struct WhenEvent;
struct EventQueue;

#line 1 "EventObjects.dpr"
struct SeeEvent;

#line 1 "ent.dpr"
class Entity;

enum Image;
enum ObserveOrgan;

template<Image I>
class VisibleEntity;
class HM_Plant;
class HM_WaterGlob;


#line 1 "space.dpr"
typedef unsigned char SpaceIndex;

class Space;
