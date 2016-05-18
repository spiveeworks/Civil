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

SpaceIndex Space::AddEntity (Entity *toad, EyesWhy why_seen)
{
    SpaceIndex ret = deposit (toad);
    if (!toad)
        return ret;
    action->QueueEvent(new EventObjects::SeeAll{*this, *toad, why_seen == APPARITION ? INTERNAL : UNBLOCKED});
    for (auto each: ents) // implement twice
        if (each.first != ret)
            action->QueueEvent(new EventObjects::See{*this, ret, *each.second, why_seen});
    return ret;
}

Entity* Space::RemoveEntity (SpaceIndex at, EyesWhy why_unseen)
{
    Entity *todo = operator[](at);
    if (!todo) // I'm not sure if I should wrap the loop in this if instead
        return nullptr;
    action->QueueEvent(new EventObjects::SeeAll{*this, *todo, BLOCKED});
    for (auto each: ents)
        if (each.first != at)
            action->QueueEvent(new EventObjects::See{*this, at, *each.second, why_unseen});
    withdraw(at); // withdraw later since it needs to be in the space while the visual events are queued
    return todo;
}

SpaceIndex Space::GiveTo (Space &to, SpaceIndex where_from)
{
    Entity *todo = RemoveEntity(where_from, VACATION);
    return to.AddEntity(todo, ENTRANCE);
}

SpaceIndex Space::GiveTo (Component& to, SpaceIndex where_from)
{
    Entity *todo = RemoveEntity(where_from, CONSUMPTION);
    return AddEntity(to.SwapContent(todo), PLACEMENT);
}

SpaceIndex Space::TakeFrom (Component& from)
{
    return AddEntity(from.SwapContent(nullptr), PLACEMENT);
}

