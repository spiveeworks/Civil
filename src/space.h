
class Space {
private:
    typedef std::map<SpaceIndex, Entity*> map_type;
    map_type ents;
    SpaceIndex next;
protected:
    Entity* withdraw (SpaceIterator const &it);
    SpaceIterator deposit (Entity const &toad);
    
public:

//Container Things
    Entity* operator[] (SpaceIndex at);
    void destroy (SpaceIndex at);
    SpaceIndex addEntity (EventQueue &action, Entity const &toad);
    SpaceIndex TakeFrom (EventQueue &action, Space &from, SpaceIndex where_from);
    SpaceIndex GiveTo (EventQueue &action, Space &to, SpaceIterator it_from);
    
//Observation Things
    void SeeAll (EventQueue &action, Entity &seer);
    
};
