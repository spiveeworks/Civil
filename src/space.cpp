// ******************** //
// * Container Things * //
// ******************** //

Entity* Space::withdraw (SpaceIterator const &it)
{
    if (it.under == ents.end()) return nullptr;
    Entity* ret = it->second;
    ents.erase(it);
    return ret;
}

SpaceIterator Space::deposit (EntityReference const &toad) // hints? return an iterator? with hints?
{
    SpaceIndex ret = next++;
    if (toad.ptr()) 
    {
        ents[ret]=toad.ptr();
        return --ents.end();
    }
    return ents.end();
}

Entity* Space::operator[] (SpaceIndex at) 
{
    auto it = ents.find(at); 
    return (it == ents.end()) ? nullptr : it->second;
}

void Space::destroy (SpaceIndex at)
{
    auto it = e
    delete remove(at);
}

SpaceIndex Space::addEntity (EventQueue &action, EntityReference const &toad)
{
    appear(watch, )
    SpaceIndex ret = next++;
    if (toad.ptr()) 
    {
        map[ret]=toad.ptr();
    }
    return ret;
}

SpaceIndex Space::TakeFrom (EventQueue &action, Space &from, SpaceIndex where_from)
{
    where_to = next++;
    auto it_from = ents.find(where_from);
    from.disappear(watch, it_from);
    toad = from.withdraw(where_from);
    if (toad)
        ents[where_to] = toad;
    return where_to;
}

SpaceIndex Space::GiveTo (EventQueue &action, Space &to, SpaceIterator it_from)
{
    where_to = next++;
    disappear(watch, it_from);
    toad = from.withdraw(it_from);
    if (toad)
        ents[where_to] = toad;
    return where_to;
}

// ********************** //
// * Observation Things * //
// ********************** //

void Space::SeeAll (EventQueue &action, Entity &seer) 
{
    ObserveOrgan eye_type = seer.Eyes();
    for (auto thing: ents)
    {
        Image img = thing->second->render(eye_type);
        if (img)
        {
            action.QueueEvent(new SeeEvent {thing->first,img,&seer});
        }
    }
}