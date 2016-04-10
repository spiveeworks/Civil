#include <Civil/Space.h>
#include <Civil/Event.h>
#include <Civil/EventObjects.h>
#include <Civil/Entity.h>

#include <Console/EntityDerivs.h>
#include <Console/IO.h>

#include <map>
#include <vector>
#include <IOstream>

using namespace std;



EventQueue* action; // this is the object that every entity writes events to (usually via the relevant Space)
bool running;

int main()
{
    EventQueue action_object; // basically the entire environment is built around this object
    action = &action_object; // this pointer is an extern used by Space and Entity. It isn't particularly safe so I guess I should go BACK to using the call stack for the same reference over and over
    
    TextIO console; // this object is a mix of four classes, handling memory, language, (partial) command interpretation, and event info output
    console.noun_map = 
    {
        "nothing", //0 is nothing derr
        "plant", //1
        "water", //2
    };
    
    Human player(console); // the user's entity object.
    Space world; // the space in which all objects must exist

    world.AddEntity(&player); // put the player's reference in the world
    world.AddEntity(new Plant()); // put a generic object into the world (Instances of Plant look like 1: "plant")
    
    running = true;
    action->QueueEvent(new EventObjects::Sleep{world, player}); // triggers the input/execute cycle of the player
    
    while (running && !action->isEmpty()) // main loop
        action->DoEvent(); // ask the eventqueue to do something
    if (running)
    {
        cout << "Nothing happened ever again!\nPress Enter to exit." << endl;
        string nul;
        getline(cin,nul); // just to wait for the user to press enter
    }
}

