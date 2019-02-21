# First programming assignment in C as a second language

This is a tutorial-style assignment that assumes at least basic knowledge of another programming language such as Java or Python. In particular, it assumes familiarity with variables and data types, including arrays; common operators; conditionals and loops; functions, including `main`; and general program structure.

If such knowledge is lacking, then a self-study tutorial like [learn-c](https://www.learn-c.org/) can supply the basics.

The topics that will be explored are some of the most important in C and account for a great deal of its power and popularity. They can be considered _"advanced basics"_.

---

Working notes:
1. Part I: C ramp up
   1. ~~Pointers, esp. passing them around as arguments and return values.~~
   2. ~~Array-pointer duality.~~
   3. ~~Working with arrays in functions.~~
   4. ~~Structures.~~
   5. ~~(Optional) The `typedef` keyword.~~
   6. ~~Allocation on the stack (aka _static_ allocation) vs allocation on the heap (aka _dynamic_ allocation). The `malloc` and `free` functions.~~
2. Part II: Sorting algorithms
   1. Program organization.
   2. A short selection of functions from the C Standard Library.
   3. Writing useful functions, esp. sensible encapsulation & meaningful contracts (e.g. who initializes array arguments).
   4. Encapsulation of comparison functions (e.g. for sorting algorithms).
   5. Toward ADT: Declare a structure type and comparison function for it, exposing only pointers in the header.

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
       | other data...      |
       |____________________|
       | iarray             |
       |--------------------|   stack frame for main
       | other data...      |
       |____________________|

       (bottom of the stack)
       ```
       The called function `get_array` has the local array allocated in its stack frame, and returns a pointer to it to the calling function `main`. When it returns, its stack frame is _"popped"_ and any data there is wiped out (or just reclaimed for use by the next function `main` may call). Ooops! Now `main` has a pointer to memory that is _no longer valid_! This is the situation on the stack the morning after...
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
       
4. [_Structures_](https://en.cppreference.com/w/c/language/struct) are C's way of packing data of several different types into monolithic records. 

   1. Structures, along with the _abstract data type_ technique in C (rather advanced and beyond the scope of this intro), are the predecessors of _classes_ in C++. The `struct` keyword is used to declare a named block of code which contains only variable declarations: primitive types, pointer types, or even other `struct` types. Incidentally, the `struct` and `class` keywords are equivalent and interchangeable for C++ classes. Let's see an example:
   
      ```C
      struct student_record {
          char first_name[10];      // these are called members or fields
          char last_name[10];       // they can be of any type
          char nine_hundred[10];
          int year_first_enrolled;
      };
      ```
      `struct` goes first, then the arbitrary name of the structure type, and finally the `{}` code block containing the structure _members_ (aka _fields_), terminated with the customary semi-colon `;`.
      
      Declare some structure types of your own.
      
   2. The member access syntax for structures will be familiar to anyone who has worked with object-oriented languages. Here's a quick example:
   
      ```C
      struct student_record jm123 = { "john", "michaels", "900123456", 1994 };
      printf("%s-%s-%s-%d\n", jm123.first_name, 
                              jm123.last_name, 
                              jm123.nine_hundred, 
                              jm123.year_first_enrolled);
      ```
      
      The first line is a declaration of a variable of type `struct student_record`. The name `jm123` is arbitrary. The assignment of values within curly braces `{}` is called _initialization_. This is the easiest way to assign values to non-pointer members. If you want to assign string-type members one-by-one, you may find the functions in the `<string.h>` library very handy. More on this later. If you have pointer-type members, you will need to handle the _dynamic allocation_ yourself. More on that later, too.
      
      Play around with structures, initialization with suitable values, and printing them out.  
   
   3. Pointers can point to anything, and structures are no exception. Here is an example:
   
      ```C
      struct student_record *student_pointer = &jm123;
      printf("%s-%s-%s-%d\n", student_pointer->first_name, 
                              student_pointer->last_name, 
                              student_pointer->nine_hundred, 
                              student_pointer->year_first_enrolled);
      
      ```
      
      Since `student_pointer` is of type `struct student_record *` (that is, pointer to a structure of type `struct student_record`), we need to dereference it before we can access the members of the structure it points to. The proper way to do that, due to [_operator precedence_](https://en.cppreference.com/w/c/language/operator_precedence), is as follows:
      ```C
      struct student_record *student_pointer = &jm123;
      printf("%s-%s-%s-%d\n", (*student_pointer).first_name,           // parentheses have among the
                              (*student_pointer).last_name,            // highest precedences of C
                              (*student_pointer).nine_hundred,         // operators, so to dereference
                              (*student_pointer).year_first_enrolled); // first, and only then access
                                                                       // a member, we put the derefe-
                                                                       // rencing *student_pointer in
                                                                       // parentheses      
      ```
      
      However, using this syntax is laborious and error-prone, especially in the cases of multiple levels of indirection. So, a substitute operator `->` is used to access members from a pointer to a struct, as shown above. Notice that its precedence is higher than that of the dereferencing (indirection) operator `*` (not to be mistaken with multiplication!).
      
   
5. (Optional) The `typedef` keyword is used to define aliases for types. It is optional, because it adds no further feature, just convenience.

   1. Typing `struct student_record` for each declaration of a student record or pointer to a student record might feel laborious. We can shorten our load by using `typedef` as follows:
   
      ```C
      typedef struct student_record student_t; // typedef [type] [type alias]
                                               
                                               // the trailing _t is a good
                                               // practice when declaring
                                               // type aliases
      ```
      From now on we can do the following declarations and the C compiler will resolve them correctly:
      ```C
      student_t lm245 = { "logan", "monaghan", "900987321", 2015 };
      student_t *student_handle = &lm245;
      ```
      
   2. This can actually be done right at the first type declaration, as follows:
   
      ```C
      typedef struct {              // the name is optional
          char first_name[10];      
          char last_name[10];       
          char nine_hundred[10];
          int year_first_enrolled;
      } student_t, *student_p;      // notice the `*` for the pointer type
                                    
                                    // the use of a trailing _p or _ptr
                                    // just like the trailing _t for the
                                    // non-pointer type alias is a good
                                    // practice and improves readability
                                    // (and sanity, trust me :D)
      ```
      Notice the following:
        - `typedef` precedes the `struct` keyword
        - we are defining aliases for the structure and pointers to the structure
        - there is no `student_record` name, because it's now redundant
        
6. Allocation on the stack (aka _static_ allocation) vs allocation on the heap (aka _dynamic_ allocation).

   1. A program, whether in source or binary format, is just a file on the hard drive. It needs to be _activated_ to run. The activation involves several steps, a simplified list of which is:
      - it is read in from disk to main memory
      - it is allocated, in virtual memory 4KB _pages_, several different memory segments: for the code, for the initialized data, for the uninitialized data, for constants, for the _stack_, and for the _heap_
      - the virtual memory _page_ containing the _entry point_ for the program is loaded into a physical memory _frame_
      - the process record (containing various data required for running the program) is added to the queue for READY processes
      
      Most of these details are beyond the scope of this intro, and would be presented in-depth in a course on Operating Systems. For now, all we need to be aware of is that each program, when activated, is allocated several memory segments, each fulfilling a different role in the execution of the process. (For the our purposes, _process_ is an _activated program_.) Here is a list with a brief description:
        - **code**: this is where the compiled and linked machine code (in binary format) of your program resides, including all the functions you created
        - **initialized data**: a segment that holds static (more on this later) variables to which you have assigned initial values
        - **constant data** (may be part of **initialized data**): a memory segments holding constants you have declared, not expected to be written, but only read (this ensures speed and gives a hint for caching)
        - **uninitialized data**: a segment that holds static variables to which you have not assigned initial values
        - **stack**: already presented, a stack-like memory segment, which holds the activation records (aka stack frames) of all function calls
        - **heap**: a segment of memory for use for user-managed data allocations, characterized by the following:
          - the allocations are performed by calls to the `malloc()`, `calloc()`, `realloc()`, and `free()` functions
          - they are collectively known as _dynamic allocation_ (as opposed to _static_, more on which later)
          - their sizes are known only at runtime (as opposed to compile-time)
          - each _dynamically allocated_ datum is manipulated through **pointers** (so, pointers are all about dynamic memory management on the heap)
      
      Here is a basic example of allocating an integer and an integer array on the heap:
      ```C
      #include <stdlib.h>
      #include <stdio.h>
      
      int main() {
          // explicit allocation of an integer on the heap
          int *array_size = (int *) malloc(sizeof(int));
          int size = 8;

          // check if the allocation was successful
          // if it was, the pointer will be non-NULL
          if (array_size == NULL) {
              printf("ERROR: Memory allocation failed!\n");
              return 1;
          } else {
              *array_size = 10;
          }

          // explicit allocation of an integer array on the heap
          int *int_array = (int *) malloc(*array_size * sizeof(int));

          // check if the allocation was successful
          // if it was, the pointer will be non-NULL
          if (int_array == NULL) {
              printf("ERROR: Memory allocation failed!\n");
              return 0;
          } else {
              for (int i = 0; i < *array_size; i ++) {
                  int_array[i] = i * *array_size;
                  printf("Array element %d holds %d\n", i, int_array[i]);
              }
          }
          
          // free the allocated memory:
          // dynamically allocated memory
          // has to be released explicitly
          free(array_size);
          array_size = NULL; // this is good practice in case 
                             // the pointer might get reused

          free(int_array);
          int_array == NULL;
      
          return 0;
      }
      ```
      Things to notice:
        - the library header `stdlib.h` is included, to give access to the memory management functions `malloc`, `free`, etc.
        - the pointers that hold a dynamically allocated integer and a dynamically allocated integer array are of the same type, pointer-to-integer (int *)
        - the function `malloc` takes an argument of type `size_t` and which expresses the size of memory to be allocated, in _bytes_
        - the [operator](https://en.cppreference.com/w/cpp/language/sizeof) `sizeof` is used to get the size of the base type, both for the sole integer and the integer array
        - the argument in the call to `malloc` for the integer array is the size of the array times the size of the base type
        - there is no difference in how we manipulate a dynamically and a non-dynamically (that is, statically) allocated array
        - the function `free` takes a pointer-type argument, specifically a pointer that received the return value of a call to `malloc`, `calloc`, or `realloc`
        - `malloc` returns a _generic pointer_ of the type `void *`, and so it has to be explicitly _cast_ into the type of the receiving pointer, by specifying the latter in parentheses right before the call to `malloc`
        - checking if the result of `malloc` is non-`NULL` is essential (that is, the memory allocation succeeded), because dereferencing a `NULL` pointer is an automatic `Segmentation fault` error
        - if the memory allocation does not succeed, it might make sense to exit the program altogether, in which case we can do a premature `return` and return a non-zero value, indicating error
        
      Run this code and experiment with dynamically allocated arrays. Develop the discipline to release the allocated memory!

   2. Arrays can have primitive, pointer, and complex base types. Here is an example of one of each, all dynamically allocated:
      ```C
      float *data = (float *) malloc(sizeof(float) * 1000); // primitive
      if (data != NULL) {
          // use data...
      }
      free(data);
      data = NULL;
      ```
      A two-dimensional array is an array of pointers (to one-dimensional arrays).
      ```C
      double **square_table;  // the type of square_table is double **,
                              // that is, a double pointer to double (LOL!)
      square_table = (double **) malloc(sizeof(double *) * 100);    // allocate the 1st dimension
                                                                     // notice the cast and base size!!
      if (square_table != NULL) {
          for (int i=0; i<100; i++)
              square_table[i] = (double *) malloc(sizeof(double) * 100); // allocate the 2nd dimension 
          // use the 2D array
          // technically, you should check if the allocations worked
      }
      // for the second dimension, you need to call free() in a loop!!
      for (int i=0; i<1000; i++) {
          free(square_table[i]);
          square_table[i] = NULL;
      }
      // now you can free the first dimension
      free(square_table);
      square_table = NULL;
      ``` 
      Finally, arrays can have `struct` base types. We'll use our `student_t` and `student_p` types from before.
      ```C
      student_p roster = (student_p) malloc(sizeof(student_t) * 35); // notice the base type
                                                                     // and the pointer (array) type
      if (roster) {  // notice that "if (roster) {}" is equivalent to "if (roster != NULL) {}"
          // use student roster...
      }
      free (roster);
      ```
      Experiment with dynamically allocated arrays, especially with complex (i.e. structure) base types. This is one of the most powerful features of the C language.
   
   3. One has to pay special attention to the base types when allocating dynamic arrays. Notice the base types for the dynamically-allocated 2D data table shown previously. Let's take this one step further and have a 3D data table, to show the regularity of the return types of the `malloc` allocations and the the base types indicated by the `sizeof` operator. The NULL-checking is omitted for clarity, but should NOT be forgotten!
      ```C
      double ***data_cube;
      
      // allocate 1st dimension                       
      data_cube = (double ***) malloc(sizeof(double **) * 1000);
      
      // allocate 2nd dimension
      for (int i=0; i<1000; i++)
          data_cube[i] = (double **) malloc(sizeof(double *) * 1000); 

      // allocate 3rd dimension
      for (int i=0; i<1000; i++)
          for (int j=0; j<1000; j++)
              data_cube[i][j] = (double *) malloc(sizeof(double) * 1000); 
      
      // use the data cube...

      // free 3rd dimension
      for (int i=0; i<1000; i++)
          for (int j=0; j<1000; j++)
              free(data_cube[i][j]); 

      // free 2nd dimension
      for (int i=0; i<1000; i++)
          free(data_cube[i]);

      // free 1st dimension
      free(data_cube);
      ``` 
      Notice the **reverse order** of releasing the allocated memory! If you mess this one up, you may either leave dangling pointers, try to free NULL pointers, or both.  
      
      Try this yourself. Visualize it for a small cube (say, dimension 3). Assign and then print out some values, so you can test that it works. 
      
   4. Here we'll summarize the two memory allocation types in C: _static_ and _dynamic_. _Dynamic_ allocation is done with `malloc` and its variants, and is released with `free`. _Dynamic_ allocations are made on the _heap_, and their sizes are known at _runtime_. _Dynamic_ finally indicates that, since the data is manipulated through pointers, their sizes can change dynamically during _runtime_.
   
      Every other variable declaration is considered _static_. This includes global variables (that is, variables that are outside of any function), local variables (that is, variables declared in functionsl), function arguments, constants, etc. _Static_ allocations are made on the stack or data segments, and their sizes are known at _compile time_. _Static_ allocations persist throughout the running program and are not lost, moved, or changed until it terminates.
      
      Take a look at the variable allocations throughout this tutorial and try to determine if the variables have been allocated statically or dynamically. **A quiz question:** Are the pointers pointing to `malloc`-ed data _statically_ or _dynamically_ allocated? Or maybe both? Can you show examples?

   5. We will wrap up with two programming guidelines that emerge from this setup. The first one is that it is ill advised to declare large _static_ data structures, especially in functions. Since all local function data goes on the stack, the stack frame may become severely inflated and cause stack overflow problems. Here is an example contrasting a static and a dynamic array in a function:
      ```C
      // a function declaring a static local array
      // it may cause the stack to overflow, but also
      // slows down the program, because creating such
      // a large stack frame takes time
      void foo() {
          int iarr[1000][1000][1000];  // don't do that!
          
          // use iarr
      }
      
      // instead, consider passing a dynamically allocated
      // array as an argument
      // this will keep the stack frame at minimal size
      void bar(int ***data_cube) {
         // use data_cube
         
      }
      ```  
      Think of a scenario where you might want to have a setup like this. Implement a small example to show it work.
           
   6. Finally, we'll see two techniques for dynamic allocation from within a function.
      1. **Pointer-to-pointer argument**. Since C passes arguments by value, pointers are also passed by value. So, if we want to change what a pointer, passes as argument to a function, points to, we need to pass a pointer to the pointer. Here's an illustration:
         ```C
         void allocate_array(int **a) {
             *a = (int *) malloc(sizeof(int) * 100;
         }
         
         int main() {
             int *int_array;
             
             allocate_array(&int_array);
             
             if (int_array) {
                 // use int_array...
             }
             
             return 0;
         }
         ```          
      2. **Return-and-assign**. Alternatively, we can mimic what `malloc` does and return a pointer to an allocated array.
         ```C
         int *allocate_array() {
             return (int *) malloc(sizeof(int) * 100;
         }
         
         int main() {
             int *int_array; = allocate_array();
             
             if (int_array) {
                 // use int_array...
             }
             
             return 0;
         }
         ```
      Keep these techniques in mind. Can you think of some pros and cons for each one? In each case, who is responsible for _memory management_, the _caller_ or the _callee_? _Hint: Think of the _symmetry_ of allocation and deallocation/release/freeing of memory._
