#include <map>
#include <civil.h>

using namespace std;

std::vector<std::string> noun_map = 
{
    "nothing",
    "plant",
    "water",
};

int main()
{
    Space world;
    map<RID_t, RenderEntity*> names;
    
    string input = "";
    while (input.substr(0,4) != "exit")
    {
        input = "";
        while (input == "") getline(cin, input);
        switch(input.cstr())
        {
            case "look":
            {
                for (auto everything = world.begin(); everything != world.end(); ++everything)
                {
                    cout << names.at((*everything).Species.RID()) << endl;
                }
            }
            break;
        }
    }


}

