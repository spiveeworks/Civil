#include <IOstream>
#include <vector>

typedef unsigned char level;

class fruit {
	level quality;
}

class plant {
	bool watered;
	level maturity;
	level health;
	level quality;
	level safeage() {return 2;}
public:
	void water() {watered = true;}
	bool age() {
		if (watered) 
		{
			maturity++;
			health++;
			watered = false;
			return true;
		}
		if (maturity < safeage) return false;
		if (--health == 0)
			kill();
		return false;
	}
};

struct fruitstack {
	fruit base;
	void *flow (fruit*);
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
}

int main () {

	return 0;
}