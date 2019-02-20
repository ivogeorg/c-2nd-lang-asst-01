# First programming assignment in C as a second language

This is a tutorial-style assignment that assumes at least basic knowledge of another programming language such as Java or Python. In particular, it assumes familiarity with variables and data types, including arrays; common operators; conditionals and loops; functions, including `main`; and general program structure.

If such knowledge is lacking, then a self-study tutorial like [learn-c](https://www.learn-c.org/) can supply the basics.

The topics that will be explored are some of the most important in C and account for a great deal of its power and popularity. They can be considered _"advanced basics"_.

---

_Notes:
1. Pointers, esp. passing them around as arguments and return values.
2. Array-pointer duality.
3. Working with arrays in functions.
4. Structures.
5. `typedef`.
6. Encapsulation of `compare`(e.g. for sorting algorithms).
7. Dynamic memory allocation & correct size calculations.
8. Allocation on the stack vs the heap._
9. Useful functions, esp. sensible encapsulation & meaningful contracts (e.g. who initializes array arguments).

---

## I. C ramp up

1. Pointers

   1. Pointers hold _addresses_ to other variables, including other pointers. The type of a pointer is simply _pointer to type_. The storage type of pointers is usually 4-byte `int`. The important pointer syntax can be summarized in the following example:
   
      ```C
      int *p; // declares a pointer to integer
              // the type of p is int*, that is, pointer to integer

              // since this is a declaration without an assignment,
              // the pointer has a value of NULL (or all-zeros), and

              // address zero is reserved for the use of the operating
              // system and restricted for user programs
      
      *p = 6; // writes 6 in the memory location where p points to
              // this is called dereferencing of (the pointer) p

              // note that, if you run the above code in this sequence
              // you will get a NULL pointer exception, usually
              // called Segmentation Fault (we'll explain what this 
              // means further along in the course)
      
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
 
    1. Arrays are data structures in which elements of the same type (called the base type of the array) are stored in consecutive memory locations. Arrays can be indexed, that is, array elements can be accessed by index. Here is a summary of the basic syntax:
    
       ```C
       int iarray[10]; // declares an array of 10 integers
                       // until the elements are explicitly assigned, 
                       // their contents are undefined (read, garbage)
       
       // here's a loop that zeros out the array                
       for (int i = 0; i < 10; i ++) {
           iarray[i] = 0;
       }
       
       // here's the syntax for literal array assignment
       float farray[] = { 0.5, 8.6 };  // notice that you don't have to specify
                                       // how many elements the array should have
                                       // this is automatically calculated by the 
                                       // compiler 
       
       // character arrays are the basis of C-style strings
       char cstring[] = "abracadabra"; // remember that C-style strings are 
                                       // NULL-terminated, so the number of 
                                       // elements of this array will be the
                                       // number of characters plus one
       ```
       
       Play around with arrays and strings, to get the hang of them. We'll soon start doing more fun things with them.
       
    2. The power of arrays, apart from the vary fast traversal of the consecutive elements, is that through the indexing method, any array element can be retrieved equally fast. When a particular array element is referenced, as in `arr[46] = 123.88;`, the compiler or runtime do the following:
       - take the base address of the array (incidentally, the address of the first element)
       - subtract one from the index of the element, in this case getting `45`
       - multiply the resulting number by the base type of the array, in this case being a 4-byte `float` or an 8-byte `double`
       - add the result to the base address of the array to get the address of the referenced element
       
       Create a few arrays of different base type and number of elements, and find their size in bytes using the `sizeof` operator. Here's an example of its use:
       
       ```C
       int integer_array[] = { 23, 56, 12, 9, 4, 356, 122, 0, 2, 7 };
   
       printf("The array element at index 8 is %d\n", integer_array[8]);
   
       int arr_size = sizeof(integer_array) / sizeof(integer_array[0]);
   
       printf("Number of elements of array is %d\n", arr_size);    
       ```
       
    3. Now we come to the most interesting point in this section: arrays have pointer types! We'll illustrate this with an example:
    
       ```C
       int iarray[10]; // the type of 'iarray' is pointer-to-integer (int*)
       
       // here's a loop that zeros out the array                
       for (int i = 0; i < 10; i ++) {
           iarray[i] = 0;
       }
       
       iarray[5] = 6;  // the type of 'iarray[5]' is integer (int)
                       // the indexing is equivalent to dereferencing
                       // (including the step calculation laid out above)
                       
       // we can make this explicit
       int *jarr = iarray;
       jarr[5] = jarr[5] + 1;
       
       // note that once an array had "degenerated" to a pointer, the
       // `sizeof(jarr)` expression will only return the size of the
       // pointer in bytes (most probably 4, as mentioned earlier)
       ```
       
       Put this code into a `main` function of a file and play around with it to explore the duality of arrays and pointers. Soon, we'll see more important implications and key uses of this fact.
       
3. Working with arrays in functions.

    1. C functions can only take arguments of primitive types and can also only return primitive types. The fact that pointers are equivalent to integers make it possible to pass in pointer-type arguments to functions and return pointer-type values from functions. The fact that arrays are equivalent to pointers from the compiler's point of view allows us to pass them in as arguments and, _more dangerously_ (we'll see when and why), return arrays from functions. Here's an example that might be familiar:
    
       ```C
       #include <stdio.h>
       
       void swap_ends(int iarr[], int size) {  // passing the size of the array is good practice
                                               // usually, the type is size_t, which can hold the
                                               // largest possible size (usually equivalent to
                                               // long long (you don't need to remember this now)
           // standard safe swap code
           int tmp = iarr[0];          // remember that iarr is of type int*, but iarr[n] is int               
           iarr[0] = iarr[size - 1];
           iarr[size - 1] = tmp;
       }
       
       int main() {
           int iarray[] = { 3, 4, 7, 1, 23 };
       
           printf("Start: %d; End: %d\n", iarray[0], iarray[4]);
           swap_ends(iarray, 5);
           printf("Start: %d; End: %d\n", iarray[0], iarray[4]);
           
           return 0;
       }       
       ```
       
       Play around with this code. You can safely execute it and modify it to do a range of things. Try to make it more useful.
       
    2. This is a good place to mention the common C novice blunder of trying to return an array from a function which created it. Here's the scenario:
    
       ```C
       #include <stdio.h>
       
       // NOTENOTE: DON'T DO THIS! :D
       
       int *get_array() {
           int iarray[] = { 3, 4, 7, 1, 23 };
       
           return iarray;           
       }
       
       int main() {
           int *iarray = get_array();
       
           printf("Start: %d; End: %d\n", iarray[0], iarray[4]);
           
           return 0;
       }       
       ```
       
       First, notice the syntax: both the return type of `get_array()` and the type of the receiving array `iarray` are `int *` (pointer-to-integer). If this was a sensible thing to do, this is the correct syntax to do it. The problem is elsewhere.
       
       The problem has to do with the _call stack_, a memory segment that is allocated to every running program, where functions that are called are each allotted a record, called a _stack frame_, where the functions arguments, its local variables, and the address to which to return to when its done are all stored. As it should be obvious from the name "stack", these records are stored one on top of the other, with the frame for `main` on the "bottom" and the latest nested function call on the "top". (Frequently, program stacks are actually stored "upside-down" so `main`'s frame is at the "top".) 
       
       So, what happens here is that when `main` makes a call to `get_array` on the first line, the stack looks something like this:

       ```       
       (top of the stack)
       ______________________
       | 3 | 4 | 7 | 1 | 23 |
       |--------------------|   stack frame for get_array
       | other data         |
       |____________________|
       | iarray             |
       |--------------------|   stack frame for main
       | other data         |
       |____________________|

       (bottom of the stack)
       ```
       The called function `get_array` has the local array allocated in its stack frame, and returns a pointer to it to the calling function `main`. And then, when it returns, its stack frame is wiped out (or just reclaimed for use by the next function `main` may call). Ooops! Now `main` has a pointer to memory that is _no longer valid_! This is the situation on the stack the morning after...
       ```       
                ???          <-----\     
                                   |
       (top of the stack)          |
        ____________________       |
       | iarray    •--------|------/
       |--------------------|   stack frame for main
       | other data         |
       |____________________|
              
       (bottom of the stack)
       ```
       This is called a _dangling pointer_, and is only one of the cases in which it is likely to have one. Let this illustration serve as a reminder to check your pointers and develop good practices when writing C code.
       
       
