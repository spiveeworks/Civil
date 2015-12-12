

namespace EventObject
    {

struct SeeEvent: public Event
{
    SpaceIndex where;
    Image what;
    Entity* who;
    
    Execute(EventQueue& action) 
        {who->Show(where, what);}
};
//maybe make a batch alternative so that looking doesn't cause event spam
 // by having an event that references a space and just asks the space then and there
 // but this complicates things if vision is possible beyond current spaces
 // maybe something that uses entity iterators of some kind
 // but that's templatey and events are polymorphicy





    }
