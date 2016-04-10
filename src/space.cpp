#include <Civil/Space.h>
#include <Civil/Entity.h>
#include <Civil/EventObjects.h>

// ************ //
// * Geometry * //
// ************ //

Entity* SpaceGeometry::withdraw (SpaceIndex index)
{
    auto it = ents.find(index);
    if (it == ents.end()) return nullptr;
    Entity* ret = it->second;
    ents.erase(it);
    return ret;
}

SpaceIndex SpaceGeometry::deposit (Entity *toad) // hints? return an iterator? with hints?
{
    SpaceIndex ret = next++;
    if (toad) 
        ents[ret]=toad;
    return ret;
}

Entity* SpaceGeometry::operator[] (SpaceIndex at) const 
{
    auto it = ents.find(at); 
    return (it == ents.end()) ? nullptr : it->second;
}

// **************** //
// * Transmission * //
// **************** //

/*
void SpaceTransmit::SeeAll (Entity &seer) 
{
    ObserveOrgan eye_type = seer.Eyes();
    for (auto thing: ents)
    {
        Image img = thing.second->render(eye_type);
        if (img)
        {
            action->QueueEvent(new EventObjects::See {this, thing.first,img,INTERNAL,&seer});
        }
    }
}

void SpaceTransmit::VisualUpdate (SpaceIndex seen_at, EyesWhy detail) //invisible things will still attempt to show themselves to every object
{
    Entity* seen = operator[](seen_at);
    for (auto thing: ents)
    {
        Image img = seen->render(thing.second->Eyes());
        if (img)
            action->QueueEvent(new EventObjects::See {this, seen_at, img, detail, thing.second});
    }
}
*/
Image SpaceTransmit::getImage (Entity& seer, SpaceIndex seen_at) const
{
    Entity* seen (operator[](seen_at)); 
    return seen ? seen->render(seer.Eyes()) : NULLIMG;
}

std::vector<Sight> SpaceTransmit::getAllImages (Entity& seer) const
{
    std::vector<Sight> ret;
    ret.reserve(ents.size());
    ObserveOrgan eyes = seer.Eyes();
    for (auto each: ents)
        if (each.second) // should be unnecessary
        {
            Image what = each.second->render(eyes);
            if (what)
                ret.emplace_back (each.first, what);
        }
    return ret;
}

// ********** //
// * Action * //
// ********** //

extern EventQueue* action;

//inline void VisualUpdate(Space* world, EyesWhy why, Entity* )

void Space::Destroy (SpaceIndex at)
{
    Entity* todo = operator[](at);
    if (!todo)
        return;
    for (auto each: ents)
        if (each.first != at)
            action->QueueEvent(new EventObjects::See{*this, at, *each.second, DISAPPARITION});
    delete withdraw(at); // this is NOT safe, any thing this object was doing will now be undefined; I need to have objects delete themselves and/or use events for deletion to be safe
}

SpaceIndex Space::AddEntity (Entity *toad, bool already_existed)
{
    SpaceIndex ret = deposit (toad);
    if (!toad)
        return ret;
    action->QueueEvent(new EventObjects::SeeAll{*this, *toad, already_existed ? UNBLOCKED : INTERNAL});
    for (auto each: ents) // implement twice
        if (each.first != ret)
            action->QueueEvent(new EventObjects::See{*this, ret, *each.second, already_existed ? ENTRANCE : APPARITION});
    return ret;
}

SpaceIndex Space::GiveTo (Space &to, SpaceIndex where_from)
{
    Entity *todo = operator[](where_from);
    if (!todo)
        return to.AddEntity(nullptr); // I'm not sure if I should wrap the loop in this if instead, but it breaks implement-once so it won't matter when that is fixed
    action->QueueEvent(new EventObjects::SeeAll{*this, *todo, BLOCKED});
    for (auto each: ents) // implement thrice
        if (each.first != where_from)
            action->QueueEvent(new EventObjects::See{*this, where_from, *each.second, VACATION});
    return to.AddEntity(withdraw(where_from), true);
}
/*
void Space::GiveTo (Grasper &to, SpaceIndex where_from)
{
    VisualUpdate(where_from, CONSUMPTION);
    to.grasp = withdraw(where_from);
}*/
