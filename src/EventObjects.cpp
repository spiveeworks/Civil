#include <Civil/EventObjects.h>
#include <Civil/Entity.h>

namespace EventObjects
    {

void SeeEvent::Execute(EventQueue& action)
    {whom->Show(where, what);}

    }
