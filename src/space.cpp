#include <Civil/Space.h>
#include <Civil/Entity.h>
#include <Civil/EventObjects.h>

// ******************** //
// * Container Things * //
// ******************** //

Entity* Space::withdraw (SpaceIndex const &index)
{
    auto it = ents.find(index);
    if (it == ents.end()) return nullptr;
    Entity* ret = it->second;
    ents.erase(it);
    return ret;
}

SpaceIndex Space::deposit (Entity *toad) // hints? return an iterator? with hints?
{
    SpaceIndex ret = next++;
    if (toad) 
        ents[ret]=toad;
    return ret;
}

Entity* Space::operator[] (SpaceIndex at) 
{
    auto it = ents.find(at); 
    return (it == ents.end()) ? nullptr : it->second;
}

void Space::Destroy (EventQueue &action, SpaceIndex at)
{
    Disappear (action, at);
    delete withdraw(at);
}

SpaceIndex Space::AddEntity (EventQueue &action, Entity *toad)
{
    SpaceIndex ret = deposit (toad);
    if (toad)
        Appear(action, ret);
    return ret;
}

SpaceIndex Space::GiveTo (EventQueue &action, Space &to, SpaceIndex where_from)
{
    Disappear(action, where_from);
    return to.AddEntity(action, withdraw(where_from));
}

// ********************** //
// * Observation Things * //
// ********************** //

void Space::SeeAll (EventQueue &action, Entity &seer) 
{
    ObserveOrgan eye_type = seer.Eyes();
    for (auto thing: ents)
    {
        Image img = thing.second->render(eye_type);
        if (img)
        {
            action.QueueEvent(new EventObjects::SeeEvent {thing.first,img,&seer});
        }
    }
}

void Space::Appear (EventQueue &action, SpaceIndex seen_at) //invisible things will still attempt to show themselves to every object
{
    Entity* seen = operator[](seen_at);
    for (auto thing: ents)
    {
        Image img = seen->render(thing.second->Eyes());
        if (img)
            action.QueueEvent(new EventObjects::SeeEvent {seen_at, img, thing.second});
    }
}

void Space::Disappear (EventQueue &action, SpaceIndex seen_at) //oops implementtwice
{
    Entity* seen = operator[](seen_at);
    for (auto thing: ents)
    {
        Image img = seen->render(thing.second->Eyes());
        if (img)
            action.QueueEvent(new EventObjects::UnseeEvent {seen_at, img, thing.second});
    }
}
