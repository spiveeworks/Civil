


class character {
public:
	void crit_attack (character& target);
	
};

enum bodyref {
	P_null = 0,
	P_head = 1, //part
	P_jaw, //lower portion
	P_neck, //lower extreme
	P_left_arm = 4, //etc
	P_left_forearm,
	P_left_hand,
	P_right_arm = 7,
	P_right_forearm,
	P_right_hand,
	P_body = 10,
	P_chest,
	P_groin,
	P_left_leg = 13,
	P_left_calf,
	P_left_foot,
	P_right_leg = 16,
	P_right_calf,
	P_right_foot
};

enum dirtype {
	forward,
	hook,
	under,
	upper,
	
};

struct attackmanner {
	unsigned num;
	bool front () {return ! (num / 2 % 2) ;}
	bool back () {return (num / 2 % 2) ;}
	
	bool straight () {return !(num % 2);}
	bool curved () {return (num % 2);}
	
	bool upper () {return (num / 4) == 0;}
	bool lower () {return (num / 4) == 1;}
	bool alternate () {return (num / 4) == 2;}
	
	char portion () {
		switch (num / 4) 
		{
			case 0:
				return 'U';
			case 1:
				return 'L';
			case 2:
				return 'A';
		}
	}
	attackmanner () {num = 0;}
};



struct attack {
	character *ab, *ad;
	unsigned manner;
};


#include <IOstream>
int main () {
	for (attackmanner via; via.num < 12; via.num++)
	{

		std::cout << via.portion();
		std::cout << (via.front () ? 'F' : 'B');
		std::cout << (via.straight () ? 'S' : 'C');


//		std::cout << ((via.num / 2) % 2);

		std::cout << "\t" << via.num;
		std::cout << "\r\n";
	}
	std::cout << "true == " << (true ? "true" : "false");
	return 0;
}




