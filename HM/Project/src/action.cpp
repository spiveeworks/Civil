
state action_fieldtarget::execute () {
	if (!hasmaterial())
		return 5;
	if (hasaccess()) {
		runaction();
		return 1;//success
	}
	state ret = cursor -> givedestination (position, this);
	if (ret == 1)
		runaction();
	return ret; //probably either in progress (0) or success (1)
	
}
/*
state Act_watercrop::execute() {
	if (!cursor->canwater())
		return 5;//no water
	if (hasaccess()) {
		position.atcursor().water();
		return 1;//success
	}
	state ret = cursor -> givedestination (position, this);
	if (ret == 1)
		position.atcursor().water();
	return ret; //probably either in progress (0) or success (1)
	
}

state Act_harvestcrop::execute() {

	if (!cursor->canwater())
		return 5;//no water

	if (hasaccess()) {
		position.atcursor().water();
		return 1;//success
	}
	state ret = cursor -> givedestination (position, this);
	if (ret == 1)
		position.atcursor().water();
	return ret; //probably either in progress (0) or success (1)
	
}
*/


/*
 *
 * Common Failstates:
 *	0 - in progress
 *	1 - success
 *	2 - interrupted
 *	3 - spatially inaccessible
 *	4 - requires instrument (ie reusable prerequisite)
 *	5 - requires material (ie consumable prerequisite)
 *	6 - requires capability (mana, fatigue, etc.)
 *	7 - requires ability (strength, cool hat, etc.)
 *	8 - 
 *
 *	255 - invalid target
 *	anything between 128 and 255 are negative values:
 *	
 *	A negative value, (for example minus five (251)) is equivalent to the 
 *	positive value, but something is invalid.
 *	For example, minus five means that the material designated for the task is 
 *	invalid.
 *	minus four would refer to an incorrect tool, and such.
 *
 *	All negative values are technically undisclosed in terms of their advised 
 *	usage beyond this.
 *	
 */
 