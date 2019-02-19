# First programming assignment in C as a second language

This is a tutorial-style assignment that assumes at least basic knowledge of another programming language such as Java or Python. In particular, it assumes familiarity with variables and data types, including arrays; common operators; conditionals and loops; functions, including `main`; and general program structure.

If such knowledge is lacking, then a self-study tutorial like [learn-c](https://www.learn-c.org/) can supply the basics.

The topics that will be explored are some of the most important in C and account for most of its power and popularity. They can be considered _"advanced basics"_.

Notes:
Pointers, esp. passing them around as arguments and return values.
Array-pointer duality.
Useful functions, esp. sensible encapsulation & meaningful contracts (e.g. who initializes array arguments).
Structures.
`typedef`.
Compare encapsulation (e.g. for sorting algorithms).
Dynamic memory allocation & correct size calculations.

## C ramp up

1. Pointers

   Pointers hold _addresses_ to other variables, including other pointers. The type of a pointer is simply _pointer to type_. The storage type of pointers is usually 4-byte `int`. The important pointer syntax can be summarized in the following example:
   
   ```C
   int *p; // declares a pointer to integer
           // the type of p is int*, that is, pointer to integer
   *p = 6; // writes 6 in the memory location where p points to
   p = 0xffeeddccaa00; // changes what p points to (NOT RECOMMENDED)
   
   ```