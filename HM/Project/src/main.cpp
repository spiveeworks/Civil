#include <IOstream>
#include <vector>

typedef unsigned char level;
typedef unsigned species;

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
	bool age() {
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
};

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

std::vector<plant> field;


int main () {

	return 0;
}