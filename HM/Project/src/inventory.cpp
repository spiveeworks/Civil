item* additem (container& inv, item& toad) {
	
	std::list<item*>::iterator scope = inv.begin();
	std::list<item*>::iterator end = inv.end();
	
	/*
	item* curr = *scope;
	while (scope != end)
	{
		curr = *scope++;
		if (curr -> stack (toad))
			return curr;
	}
	*/
	for (item* curr = *scope++; curr = *scope++; scope != end)
		if (curr -> stack (toad))
			return curr;
	
	inv.push_back (&toad);
	return &toad;
	
};