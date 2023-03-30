# LonelyPartyArray

For a lot of programming tasks that use arrays, it’s not uncommon to allocate an array that is large enough to handle any worst-case scenario you might throw at your program, but which has a lot of unused, wasted memory in most cases.

This program implements a new array-like data structure, called a lonely party array (or “LPA”),
that presents a solution to the common issue of unnecessary memory allocations in arrays.

LPAs are arrays that are broken into fragments that get allocated and deallocated on as-needed basis, rather than being allocated all at once as one big array, in order to eliminate unecessary memory bloat. This data structure ameliorates several problems programmers often encounter with arrays in C. 

This program shows proficiency working with dynamic memory management, pointers, and structs in C. It also offered the opportunity to gain experience using valgrind to test for program memory leaks. It also helped gain additional experience managing programs that use custom header files and multiple source files.
