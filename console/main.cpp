#include <Civil/Space.h>
#include <Civil/Event.h>
#include <Civil/EventObjects.h>
#include <Civil/Entity.h>

#include "EntityDerivs.h"

#include <map>
#include <vector>
#include <IOstream>

using namespace std;

std::vector<std::string> noun_map = 
{
    "nothing", //0 is nothing derr
    "plant", //1
    "water", //2
};

std::vector<std::string> explanation_map = 
{
    "You see a ", ". ", //0 is manually looking around
    "Something changes into a ", "!", //1 I think this will be very different actually but the code will go here to keep the end in mind
    "A ", " appears!", //2 apparition
    "The ", " disappears!", //3 ..dis—apparition?
    "A ", " enters the area.", //4 entrance
    "The ", " leaves the area.", //5 vacation
    "A ", " is placed in the area.", //6 placement
    "The ", " is taken away.", //7 consumption
};

void HM_Human::Show(SpaceIndex location, Image image, EyesWhy detail)
 //define a method for class HM_Human outside of its .cpp file, how hacky
    // I guess I should make an abstract class that is implemented to receive event info and convert it to text. (that was the plan for DE)
{
    if (image)
        cout 
            << explanation_map[2*detail] 
            << noun_map[image] 
            << explanation_map[2*detail + 1] 
            << endl; 
}

EventQueue* action; // this is the object that every entity writes events to (usually via the relevant Space)

int main()
{
    EventQueue action_object;
    action = &action_object;
    
    HM_Human player; // the user's entity object.
    Space world; // the space in which all objects must exist
    
    world.AddEntity(&player); // put the player's reference in the world
    world.AddEntity(new HM_Plant()); // put a generic object into the world (HM_Plant's look like 1: "plant")
    
    string input = ""; // takes lines from the console
    while (input.substr(0,4) != "exit") // main loop
    {
        if (action->Empty()) // tested before each input iteration.
            cout << "Nothing Happens" << endl;
        else // if there are events then any listed at the current moment execute.
            while (action->DoEvent()); // ask the eventqueue to do something, it will return true if there are still events which need to be done
                //do nothing, the events handle themselves
        input = ""; // reset input
        while (input == "") getline(cin, input); // get lines of input other than empty lines
        if (input == "look") // lists the names of objects 'in sight'
        {
            world.SeeAll (player); // at the moment the space handles implementation
        }
/*
        else if (input.substr(0, 5) == "grab ") // NYI, queues an event that attempts to disappear an object
        {
            Image target = noun_map.find(input.substr(5)); // convert the text after "grab " into a visual description
            SpaceIndex loc = player.firstMemoryWhichIs(target); // gets the first object location associated with the desired description (idk lol)
            world.Grab(player.Hand(), loc); // queues an event to attempt the grabbery
        }
 */
        else if (input == "wait" && !action->Empty()) // just asks the eventqueue to append the next lot of events to the 'immediate' category
        {
            time waited = action->MoveOn(); // ty EventQueue
            cout << "Waited for " << waited << " units time." << endl; // output what you did
        }
        //else if (input != "exit") {action->Debug();}
    }
}

