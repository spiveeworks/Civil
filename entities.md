
Entities
========

Civil is a program that generates sequences of events, based on interactions between entities.  
An entity is a self enclosed machine that looks very similar to class-instances:  
 * entities have a data structure called their state
 * entities have an algorithm for translating their state into a description that is broadcast to other entities  
 * entities have event driven programs called mechanisms
The format of an entity's state, as well as the mechanisms and descriptive algorithm, are all shared among entities of the same type.  
The state itself is stored independantly between entities.

Mechanisms
----------

A mechanism is an external condition, an internal condition, and an effect:
 * The internal condition of a mechanism is a partial description of what the entity's state must be in order for the effect to fire.  
 * The external condition is similarly a partial description of some interaction that happens with another entity.  
   * these 'interactions' have the same format as the state of an entity
   * hence external conditions are the same as internal conditions in all ways except for how they come about
 * The effect is a list of actions to take, including
   * changes to the state
   * manipulation of other entities or creation of new ones
   * sending signals to other entities (poke them)
   * sending information to an interface such as the screen or an NPC brain

When an event occurs, any entities that witness the event will check all of their mechanisms and see which need to run:
 1. A list of mechanisms is assembled, from those that meet the following:
  * The mechanism's external condition doesn't fail
  * The mechanism either has an external condition, or the internal condition fails, (i.e. it can't already be able to run, it has presumably already fired recently)
  * The last time the mechanism fired (if any) was not in the same instant as the current event (no infinite loops)
 2. The internal conditions of each are tested
 3. The highest priority mechanism whose internal condition is met, will be removed from the list, and its effect executed.
 4. 2-3 will be repeated until no mechanisms in the list meet their internal conditions anymore
 5. Any signals broadcast during an effect will occur instantly at the end of all mechanisms' executions
 6. The entity will check to see if its description should now be different, and if so this will be broadcast to nearby entities

State and Signal Format
-----------------------

The format of state and signal information must conform to some "taxonomic regular grammar"

A taxonomic regular grammar is a set of possible 'strings' (states or signals) with a fairly strict but powerful structure.  

State and Signal Translation (Templates)
----------------------------------------

Templates are algorithms that can translate between two different string formats.  
They contain lists of possible ways of filling the output format, along with conditions that must be met by the input string.

User Language
=============

An indentation based language will be implemented to allow for the specification of entity types, their state format, descriptive algorithm, and mechanisms.

