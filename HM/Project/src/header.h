
typedef unsigned char level;
typedef unsigned species;
typedef unsigned char state;

#include "class.h"

// * * * * * ** //
// * Main.cpp * //
// ** * * * * * //

class fruit {
	level quality;
	species ftype;
public:
	bool operator== (fruit B) {return (ftype == B.ftype && quality == B.quality);}
};

class plant {
	bool watered;
	species ptype;
	level maturity;
	level health;
	level quality;
	level safeage() {return 2;}
	bool hasdied;
public:
	void water() {watered = true;}
	void kill() {hasdied = true;}
	bool age();
};

std::vector<plant> field;
unsigned width, height;

struct CN_field {
	unsigned x, y;
	plant& atcursor () {return field.at(x + width * y);}
	bool operator== (CN_field B) {return (x == B.x && y == B.y);}
};

class character {
	CN_field location;
	unsigned waterSupply;
public:
	void water() {location.atcursor().water(); waterSupply--;}//does not actually check to see if you can water anything
	bool canwater() {return (waterSupply > 0);}
	state givedestination (CN_field destiny, action *asker);
	bool canaccess (CN_field target) {return (location == target);}
}main_player;



// * * * * * * ** //
// * Action.cpp * //
// ** * * * * * * //


class action {
public:
	virtual state execute ();
};

class Act_watercrop: public action {
	CN_field position;
	character *cursor;
public:
	state execute ();
};




