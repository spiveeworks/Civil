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
    "nothing",
    "plant",
    "water",
};

void HM_Human::Show(SpaceIndex location, Image image)
{
    if (image)
        cout << "You see a " << noun_map[image] << endl;
}

int main()
{
    HM_Human player;
    Space world;
    EventQueue action;
    
    world.AddEntity(action, &player);
    world.AddEntity(action, new HM_Plant());
    
    string input = "";
    while (input.substr(0,4) != "exit")
    {
        if (action.Empty())
            cout << "Nothing Happens" << endl;
        else
            while (action.DoEvent());
                //do nothing, the events handle themselves
        input = "";
        while (input == "") getline(cin, input);
        if (input == "look")
        {
            world.SeeAll (action, player);
        }
        else if (input == "wait" && !action.Empty())
        {
            time waited = action.MoveOn();
            cout << "Waited for " << waited << " units time." << endl;
        }
    }
}

