#include <IOstream>
#include <vector>
#include <list>

#include "header.h"

#include "inventory.cpp"
#include "action.cpp"


bool plant::age () {
	if (watered) 
	{
		maturity++;
		health++;
		watered = false;
		return true;
	}
	if (maturity < safeage()) return false;
	if (--health == 0)
		kill();
	return false;
}

fruit* plant::harvest () {
	fruit* ret = new fruit (ptype, quality); 
	maturity = 0; 
	health = 0; 
	quality = 0;
	ptype = 0;
	return ret;
}

/*
struct fruitstack {
	fruit base;
	void (*flow)(fruit*);
	unsigned quantity;
	bool addfruit (fruit *toad) {
		if (base == *toad)
		{
			quantity++;
			delete toad;
			return true;
		}
		(*flow)(toad);
		return false;
	}
};
/*
class field {
	struct square {
		
	};
	std::vector<plant> plants;//make this a list when possible
	std::vector<square> grid;
	unsigned wide, high;//width and height
};
*/



state character::givedestination (CN_field destiny, action *asker) {
	location = destiny;
	return 1;
}

bool character::storehand () {
	if (hand == 0) 
		return false; 
	additem (inventory, *hand); 
	return true;
}


int main () {
	field.resize(width*height);

	return 0;
}