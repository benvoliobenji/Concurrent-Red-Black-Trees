# Concurrent Red Black Trees
## Author: Benjamin Vogel
Programming Project 2 for COMS 352, designed to utilize pthread and semaphores.

## Quick Implementation Note
This implementation of Programming Project 2 adheres to the advanced version of the new project, where each node is individually locked and unlocked.

## Quick Documentation Note
The documentation for each of the methods (ie a description of the methods, the parameters, and the return types) are in the __HEADER__ files. This is to reduce the number of lines in the .cpp files and create a better sense of readability since a user should be able to look at the header files and understand what the .cpp file is trying to accomplish.

## How to Run
- Navigate to the folder
- Type __make clobber__ (this is just in case to clean up any unneeded files)
- Type __make__
- Type __./rbtree (insert file name here)__

## What is a Red Black Tree?
A red black tree is a form of binary search tree with the added complexity of coloring nodes red or black. This is done in order to keep the tree as balanced as possible by adhering to a set of properties. When the tree is modified, the new tree is rearranged and recolored to restore the properties. The properties are as follows:
- Each node is either red or black
- The root is black
- All leaves (NIL) are black
- If a node is red, then both its children are black
- Every path from a given node to any of its descendant NIL nodes contians the same number of black nodes

## Implementation Intentions and Philosophy
Since I planned on making each of the nodes modifiable rather than the threads, I made each node adhere to a readers-writers solution. If you are unfamiliar, a readers-writers solution is where multiple readers can read a node, but only one writer can write at a node. Writers cannot write if there are readers currently reading and readers cannot read if there is a writer modifying the node. Each node has the appropriate condition variables and methods to implement a monitor-like solution to the readers-writers problem on each individual node. This allows for several threads to do simultaneous searches, insertions, and deletions at the same time, while still keeping the tree as balanced as possible.

