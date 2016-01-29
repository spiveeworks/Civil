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
        cout << "You see a " << noun_map[image];
}

int main()
{
    HM_Human player;
    Space world;
    EventQueue action;
    
    world.AddEntity(action, new HM_Plant());
    
    string input = "";
    while (input.substr(0,4) != "exit")
    {
        input = "";
        while (input == "") getline(cin, input);
        if (input == "look")
        {
            world.SeeAll (action, player);
        }
    }
}

