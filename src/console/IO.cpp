#include <Console/IO.h>
#include <algorithm>
#include <IOStream>

#include <Civil/Entity.h>
#include <Civil/Space.h>
#include <Civil/EventObjects.h>
 // Event.h would normally be required but since EventObjects.h is extensional it is sufficient

extern EventQueue *action;
extern bool running;

// ***************** //
// * Visual Memory * //
// ***************** //

SpaceIndex IOSight::findFirst (Image what) const
{
    auto it = find_if // get the first object that looks like what
    (
        sights.cbegin(), //iterator range stuff
        sights.cend(), 
        [what] (std::pair<SpaceIndex,Image> const &p) // lambda function for testing if the thing looks right
            {return p.second == what;} // using the captured value for what, test if the object looks right
    ); 
    if (it == sights.end()) // indicates the find algorithm found nothing
        return ~0; // no location contains this object
    else
        return it->first; // since this will be the location
}

Image IOSight::getImage (SpaceIndex where) const
{
    auto it = sights.find(where); // checks for the SpaceIndex given to see if there is a recorded sight there.
    return it == sights.end() ? NULLIMG : it->second; // either say nothing is there or say what is there
}


// *************************** //
// * Language Interpretation * //
// *************************** //

Image IOLang::interpret (std::string const &desc) const
{
    auto it = find(noun_map.begin(), noun_map.end(), desc); // find the name in the dictionary
    return it == noun_map.end() ? NULLIMG : Image(it - noun_map.begin()); // if the name is found return the index else return 0
}

// **************** //
// * Screen Input * //
// **************** //

void TextInput::Cancel(Space& where, Entity& self)
{
    if (need_input)
        return; // already cancelled and waiting for input
    
    std::string input;
    std::getline(std::cin, input);
    if (input[0] == 's') // s is for stop wach'u doin'
    {
        if (doing)
            doing->Abort();
        doing = new EventObjects::Sleep{where, self};
        action->QueueEvent(doing, reaction_time);
        need_input = true;
    }
}

ChildEvent TextInput::React(Space& world, Entity& self) // note this is not python
{
    need_input = false;
    doing = nullptr;
    time when = reaction_time;
    
    std::string input; // takes lines from the console
    while (!doing) // in this loop break usually means return END_CHAIN;
    {
        input = "";
        while (input == "") 
        {
            std::cout << ">";
            getline(std::cin, input); // get lines of input other than empty lines
        }
        if (input == "exit")
        {
            running = false;
            break;
        }
        else if (input == "look") // lists the names of objects 'in sight'
        {
            action->QueueEvent(new EventObjects::SeeAll {world, self, INTERNAL});
            doing = new EventObjects::Sleep {world, self};
            need_input = true;
        }
        /*
        else if (input.substr(0, 5) == "grab ") // NYI, queues an event that attempts to disappear an object
        {
            Image target = std::find(noun_map.begin(), noun_map.end(), input.substr(5)); // convert the text after "grab " into a visual description
            SpaceIndex loc = findFirst(target); // gets the first object location associated with the desired description
            world.Grab(self.Hand(), loc); // queues an event to attempt the grabbery, syntax uncomfirmed
        }*/
        else if (input == "wait" && !action->isEmpty()) // waits for ten times the reaction time of the person, but still allowing for interruptions by input.
        {
            doing = new EventObjects::Sleep{world, self};
            when *= 10;
        }
    }
    return ChildEvent{when, doing};
}

// ***************** //
// * Screen Output * //
// ***************** //

std::vector<std::string> TextOutput::explanation_map = 
{
    "You see a ", ". ", //0 is manually looking around
    "Something changes into a ", "!", //1 I think this will be very different actually but the code will go here to keep the end in mind
    "You see a ", ". ", //2 object comes into view
    "You lose sight of the ", ". ", //3 lose sight by moving to a new position
    "A ", " appears!", //4 apparition
    "The ", " disappears!", //5 ..dis—apparition?
    "A ", " enters the area.", //6 entrance
    "The ", " leaves the area.", //7 vacation
    "A ", " is placed in the area.", //8 placement
    "The ", " is taken away.", //9 consumption
};

void TextOutput::VisualUpdate(SpaceIndex loc, Image desc, EyesWhy detail)
{
    //if (desc)
    {
        std::cout
            << explanation_map[2*detail] 
            << ((desc >= 0 && desc <= noun_map.size()) ? noun_map[desc] : "UNDEFINED")
            << explanation_map[2*detail + 1] 
            << std::endl; 
    }
    if (!IsHere(detail)) desc = NULLIMG;
    Update(loc, desc);
}

void TextOutput::GrabUpdate(SpaceIndex loc, bool success)
{
    std::cout << "You grab the "
        << name(getImage(loc))
        << (success
        ? " and pick it up."
        : " but can't lift it.")
        << std::endl;
    if (success)
    {
        holding = getImage(loc);
        Update(loc, NULLIMG);
    }
}

