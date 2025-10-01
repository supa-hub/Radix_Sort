# Radix_Sort
## A Header only implementation of radix sort using concepts


This repo implements radix sort as a sup::sort() function, which takes the beginning and end iterator of a container which you want to sort. </br>

This works as a drop-in replacement for the std::sort() available in the STL, 
but works significantly faster </br>
for large containers.


This function works for all signed and unsigned integral types (for e.g: int, long long, unsigned int etc.). </br> </br> 


The function is defined in the ```radix_sort.hpp``` file.

### Concepts in the function parameter
Concepts were introduced in C++20 and in this project they're used to ensure that the types of the iterators passed to sup::sort() are 
random access and point to values of integral type. </br> 

If some of these requirements fail, you'll get a compile-time error.


### Testing
compile time tests as well as performance testing is defined in the ```tests.cpp``` file.

### Build requirements
Min C++ version: 20  (check that your compiler version supports concepts.) </br></br> 
required standard library includes:  vector, array, concepts, type_traits </br> </br> 

Recommended g++ compiler flags: </br>
```-std=c++23``` </br>
```-O3``` </br>
```-fconcepts-diagnostics-depth=5``` </br> </br>

NOTE: It is possible that depending on your compiler and development environment you'll have to define additional compiler flags or tasks in your IDE/editor.  </br></br>
For example, on VSCode with GCC, you might have to define a ```c_cpp_properties.json``` file in your ```.vscode/``` folder with additional config information for concepts to work correctly