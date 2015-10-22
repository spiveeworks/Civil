
template<typename T>
struct Duple 
{
    T first, second;
	
};

constexpr int ORD_EXP = -4;
typedef Duple<FxPtNum<ORD_EXP,int>> Point;


