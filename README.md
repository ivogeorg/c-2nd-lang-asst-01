# First programming assignment in C as a second language

This is a tutorial-style assignment that assumes at least basic knowledge of another programming language such as Java or Python. In particular, it assumes familiarity with variables and data types, including arrays; common operators; conditionals and loops; functions, including `main`; and general program structure.

If such knowledge is lacking, then a self-study tutorial like [learn-c](https://www.learn-c.org/) can supply the basics.

The topics that will be explored are some of the most important in C and account for a great deal of its power and popularity. They can be considered _"advanced basics"_.

---

_Notes:
1. Pointers, esp. passing them around as arguments and return values.
2. Array-pointer duality.
3. Useful functions, esp. sensible encapsulation & meaningful contracts (e.g. who initializes array arguments).
4. Structures.
5. `typedef`.
6. Compare encapsulation (e.g. for sorting algorithms).
7. Dynamic memory allocation & correct size calculations.
8. Allocation on the stack vs the heap._

---

## I. C ramp up

1. Pointers

   1. Pointers hold _addresses_ to other variables, including other pointers. The type of a pointer is simply _pointer to type_. The storage type of pointers is usually 4-byte `int`. The important pointer syntax can be summarized in the following example:
   
      ```C
      int *p; // declares a pointer to integer
              // the type of p is int*, that is, pointer to integer
      
      *p = 6; // writes 6 in the memory location where p points to
              // this is called dereferencing of (the pointer) p
      
      p = 0xffeeddccaa00; // changes what p points to (NOT RECOMMENDED)
      
      int i = 7; // declare a static (see (8) below) integer
      p = &i; // proper way to take the address and assign a pointer
              // p now points to i
      *p = 9; // i is now 9
      
      int **p; // double pointer to int
               // syntactically, there is no depth limit for the levels of 
               // indirection, but the usefulness decreases past double pointers
      ```
   
      Declare some variables and pointers to `int`, `float`, `char`, and experiment with direct and indirect (through the pointer) changing of the variable's values. Also declare and experiment with a couple of double pointers. We'll soon see what they are useful for.
   
   2. Pointers are often passed as arguments to functions, or returned from functions. We will slowly work our way up to the full power of pointers thus used, as we go through the tutorial. At this point, the basic utility will be shown in an example:
   
      ```C
      #include <stdio>  // for printf
      
      int global_i = 9; // globals should be avoided
                        // this is just for convenience of illustration
      
      // Arguments in C functions are passed BY VALUE, including pointers,
      // so this function will not change the original variable the value
      // of which was used to assign the argument, in this case, global_i.
      // It will change the value of argument_i, but it is only a local 
      // copy of global_i (see the function call in main() below).
      void wont_change(int argument_i) {
          argument_i = 10;
      }
      
      // If we want a function to change a variable outside (the scope of)
      // the function, we need to pass it a pointer to that variable.
      // This function will change the original 
      void change_inside(int *pointer_to_global_i) {
          *pointer_to_global_i = 10; // dereference and assign a new value
      }
      
      int main() {
          printf("%d\n", global_i);
          wont_change(global_i);
          printf("%d\n", global_i);
          change_inside(global_i);
          printf("%d\n", global_i);
          
          return 0;          
      ```
      
      Run this code and play around with the variables, number of arguments, the argument types (non-pointer and pointer).
      
   3. There is a way to change the value of the global with a function without passing in a pointer to it, but instead assigning the return value of the function. Here's an example analogous to the previous:
   
      ```C
         #include <stdio>  // for printf
         
         int global_i = 9; // globals should be avoided
                           // this is just for convenience of illustration
         
         // Arguments in C functions are passed BY VALUE, including pointers,
         // so this function will not change the original variable the value
         // of which was used to assign the argument, in this case, global_i.
         // It will change the value of argument_i, but it is only a local 
         // copy of global_i (see the function call in main() below).
         int will_return_it_changed(int argument_i) {
             return argument_i + 1;
         }
         
         int main() {
             printf("%d\n", global_i);
             global_i = will_return_it_changed(global_i);
             printf("%d\n", global_i);
             
             return 0;          
         ```
         
         Play around with this setup to enrich your function manipulation arsenal. We'll see even more powerful techniques further along.
         
 2. Pointer-array duality.