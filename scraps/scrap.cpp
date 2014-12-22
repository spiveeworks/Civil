class object;
class corpse;
class character;

class object {
	virtual bool iscorpse () {return false;}
};

class corpse {
	bool iscorpse () {return true;}
};

class character {
	enum goal {
		sit,
		kill,
		retrieve,
		search
	} curr_goal;
	enum phase {
		travel,
		interact,
		engage,
		transfer,
		interrogate
	} curr_phase;
	enum desire {
		f_corpse,
		f_murderer,
		f_thief,
		f_target,
		f_item
	} curr_desire;
	object* target;
public:
	void kill ();
	void observe (object& target) {
		if (target.iscorpse()) 
		{
			target = &target;
			
		}
		
	}
};

class assassin: public character {
public:
};
