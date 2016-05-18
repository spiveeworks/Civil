#pragma once
#include <vector>
#include <string>
#include <map>

#include <Civil/EntitySignals.h>

typedef unsigned char SpaceIndex;
class Space;

#include <Civil/Event.forward>

class Entity;


extern std::vector<std::string> noun_map;

class IOSight { // sight or memory or info or what??
    std::map<SpaceIndex, Image> sights;
protected:
    SpaceIndex findFirst (Image what) const;
    Image getImage (SpaceIndex where) const;
    
    void Update (SpaceIndex where, Image what)
        {if (what) sights[where]=what; else sights.erase(where);}
    Image holding;
    
};

class IOLang {
public:
    std::vector<std::string> noun_map; // public so that the list can be edited, but name() should be used instead
    std::string const& name (Image what) const
        {return noun_map[what];}
    Image interpret (std::string const &desc) const;
};

class TextInput: public virtual IOSight, public virtual IOLang {
protected:
    Event* doing = nullptr;
    bool need_input = true; // if this is true then Cancel does nothing, as the current event has already been cancelled
    time reaction_time = 100;
public:
    void Cancel(Space& where, Entity& self); // asks user if they want to cancel what they are doing, based on previous input.
    ChildEvent React(Space& place, Entity& self);
};

class TextOutput: public virtual IOSight, public virtual IOLang {
    static std::vector<std::string> explanation_map;
public:
    void VisualUpdate(SpaceIndex loc, Image desc, EyesWhy detail);
    void TouchUpdate(Touch signal, bool did_grab);
};

struct TextIO: public TextInput, public TextOutput {
};

