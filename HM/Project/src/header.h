
typedef unsigned char level;
typedef unsigned species;
typedef unsigned char state;

#include "class.h"

typedef std::list<item*> container;

// * * * * * ** //
// * Main.cpp * //
// ** * * * * * //

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
	fruit* harvest ();
	
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
	container inventory;
	item* hand;
public:
	void water() {location.atcursor().water(); waterSupply--;}//does not actually check to see if you can water anything
	item* giveitem (item* togive) {item* ret = hand; hand = togive; return ret;}
	bool storehand ();
	
	bool canwater() {return (waterSupply > 0);}
	state givedestination (CN_field destiny, action *asker);
	bool canaccess (CN_field target) {return (location == target);}
	
	bool hasemptyhand () {return hand == 0;}
}main_player;


// * * * * * * * * * //
// * Inventory.cpp * //
// * * * * * * * * * //

item* additem (container& inv, item& toad);

class item {
public:
	unsigned quantity;
	virtual unsigned itemclass() =0;
	virtual level itemsignificance() {return 0;}
	virtual unsigned itemsubclass() {return 0;}
	
	bool operator== (item& B) {
		return (
			itemclass() == B.itemclass() && 
			itemsignificance() == B.itemsignificance() && 
			itemsubclass() == B.itemsubclass()
		);
	}
	virtual bool stack (item& B) {
		if (!operator== (B)) return false;
		quantity += B.quantity;
		B.quantity = 0;
		return true;
	}
};


class fruit: public item {
	level quality;
	species ftype;
public:
//	bool operator== (fruit B) {return (ftype == B.ftype && quality == B.quality);}
	fruit (species type, level qual) {quality = qual; ftype = type;}
	unsigned itemclass() {return 4;}
//significance being its "quality" attribute: how good the plant is
	level itemsignificance() {return quality;}
//subclass being its species
	unsigned itemsubclass() {return ftype;}
};


// * * * * * * ** //
// * Action.cpp * //
// ** * * * * * * //


class action {
protected:
	character *body;
public:
	virtual state execute ()=0;
	virtual bool hasaccess () {return true;}
	virtual bool hasmaterial () {return true;}
	virtual bool hasrequiredroom () {return true;}
	virtual void runaction () =0;
};

class action_fieldtarget: public action {
protected:
	CN_field position;
public:
	bool hasaccess () {return (body -> canaccess (position));}
	state execute ();
};

class act_watercrop: public action_fieldtarget {
protected:
public:
//	state execute ();
	void runaction () {body -> water();}
	bool hasmaterial () {return body->canwater();}
};

class act_harvestcrop: public action_fieldtarget {
public:
//	state execute ();
	void runaction () {body -> giveitem (position.atcursor().harvest());}
	bool hasrequiredroom () {return body -> hasemptyhand ();}
	
	
};

class act_storeitem: public action {
public:
	state execute() {return (body -> storehand ()) ? 1: 2;}
};


