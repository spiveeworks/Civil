
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
	fruit (species type, level qual) {quality = qual; ftype = type;}
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
	fruit* harvest () {
		fruit* ret = new fruit (ptype, quality); 
		maturity = 0; 
		health = 0; 
		quality = 0;
		ptype = 0;
		return ret;
	}
	
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
protected:
	character *cursor;
public:
	virtual state execute ()=0;
	virtual bool hasaccess ()=0;
	virtual bool hasmaterial () {return true;}
	virtual void runaction () =0;
};

class action_fieldtarget: public action {
protected:
	CN_field position;
public:
	bool hasaccess () {return (cursor -> canaccess (position));}
	state execute ();
};

class Act_watercrop: public action_fieldtarget {
protected:
public:
	state execute ();
	void runaction () {position.atcursor().water();}
	bool hasmaterial () {return cursor->canwater();}
};

class Act_harvestcrop: public action_fieldtarget {
public:
	state execute ();
	void runaction () {delete position.atcursor().harvest();}
	
	
};


