#include <Console/EntityDerivs.h>
#include <Console/IO.h>

void Human::Show(Space& place, SpaceIndex location, Image image, EyesWhy detail)
{
    mind->VisualUpdate(location, image, detail); // tell Output that something happened
    mind->Cancel(place, *this); // Get Input to see if current task need be stopped
}

ChildEvent Human::React (Space& place)
    {return mind->React(place, *this);}

ChildEvent Human::GrabReact (Space& place, Touch signal)
{
    mind->TouchUpdate(signal, true);
    return React(place);
}

ChildEvent Human::TouchReact (Space& place, Touch signal)
{
    mind->TouchUpdate(signal, false);
    return React(place);
}

