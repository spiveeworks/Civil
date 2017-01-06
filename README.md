# Civil
Text game engine that will turn into a multimedia game engine once the game part exists.

Civil is built around three concepts; hopefully these will work together to create an interactive environment with very strong customizability, and depth of simulation.

## Programmable mechanics
Entities and their interactions will be driven by an event based scripting system, that should be easy to experiment and tinker with, to allow for easy modification of your and others' systems.

## Behaviour through gameplay
Playable entities will all have optional features that allow you to build a behaviour tree to automate their actions, so that you can create an NPC to play in an environment as you simultaneously learn how to play in it yourself. 
The benefits of this range from making early-game etc. less boring, to allowing for large scale user made NPC environments, which leads to the third concept.

## Deep social structures
By iteratively playing with your or community NPC civilisations, I'm hoping for it to be possible to create deep, immersive, simulated social structures for any set of game mechanics, and for this to be a strong foundation for interactive experiences centred around simulation, free decision making, and real consequences.

# What have I got so far?
At the moment I am creating the architecture described above, and am testing it in a purely text console. 
Once I have implemented what I need from a scripting system, and NPC behaviour trees, I will consider developing a graphical interface, since that will be necessary eventually.

#What other ideas do I foolishly think I can achieve in this same project?
Most of these ideas relate to things that I think will be necessary for the third concept "Deep social structures", but some relate to making more community oriented programming tools, etc.

None of these are being worked on in the forseeable future, I just suspect they will be important to do eventually.

## Modular content
This would allow for powerfully customizable game environments without much end user modification. 
Modules would contain generic code, that can be duplicated and delegated by the end user to satisfy the needs of other modules.
Then game templates can specify some needs, and the user can choose modules to achieve those needs, until a fully constructed game has been made, ready to compile.

Modules could also contain tools usable in NPC behaviour trees such as module dependant goals or actions; this would allow for portable NPCs that can react to modification of the game environment under certain conditions.

## Presimulated gameplay
When game environments get large, lots of gameplay will be expensive for the user's experience, and yet unnecessary since the user doesn't directly experience most of the environment.
Traditionally this is what the metagame is for, a shallow simulation to create the illusion of progress while the user isn't looking.

I am interested in ways of simulating an environment in advance, so that the user can do other things with their valuable time, and then play a fast, engaging game when the simulation is finished, without any losses in simulation fidelity.

Map makers could also presimulate their map before releasing so that interactions that occur outside the user's experience are only computed once before the map's release.

## Economic calculation
Large scale economic activity is one of the appealing parts of the concept of "Deep social structures", but creating behaviour trees that can adapt flexibly to changing economic environments is difficult, potentially impossible.
Creating supporting systems that deal with calculations related to path costs, risks, and foregone opportunities, would allow for more dynamic, market based environments, that can adapt and evolve naturally as situations change.

## Construction planning
Possibly the least ambitious idea, or at least the least exciting.
Creating a system that allows NPCs to plan and create buildings in new and unseen terrain, or further, that allows them to adapt plans to work around obstacles, would allow first of all, for NPCs to thrive in sandbox situations, but also for procedurally generated constructions with genuine and organic history.
