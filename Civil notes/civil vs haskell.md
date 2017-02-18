

Learning the basics of haskell a long time ago has influenced the way that I think about programming.

I have recently properly learned haskell and have discovered that a lot of my desires from things like Civil (but also MetaJump) already exist in haskell and likely other lazy/functional languages.

This document will be a comparison of concepts I have wanted to implement, and their associated haskell concepts.  
I will try and focus on differences, to better understand my goals and also to see if much of it is really worth implementing.



# Modules - Typeclasses

Modules and typeclasses are very similar, they are both designed to abstractly represent common concepts across independantly implemented objects.  

Modules are similar to type constructors, however I have envisioned the instances as being 'within' the module, and visualized them as machines with inputs and outputs.  
The classes of these inputs and outputs are the typeclasses themselves, and an input is analogous to a type contructor argument, and an output is analogous to an instance.  

Modules are potentially capable of implementing the same output in multiple ways, but external adaptor modules are still possible.


# Tax - Algebraic Data Types

TRGs are very similar to data in haskell, although the description system does not have a haskell analogue.



