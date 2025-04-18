# Generic Dynamic Array Implementation in C with Type-Safe Operations

A flexible and type-safe dynamic array implementation in C that provides generic container functionality with support for custom data types. The library offers comprehensive array operations, sorting algorithms, and type-specific manipulations through a unified interface.

This project implements a dynamic array container that can store and manipulate any data type through a type information system. It features automatic memory management, array operations (append, prepend, insert), sorting algorithms (bubble sort, heap sort), and type-specific operations like mapping and filtering. The implementation uses a type-safe approach through TypeInfo structures that define how to handle different data types, currently supporting strings and doubles with the ability to extend to other types.

## Repository Structure
```
.
|-------> CMakeLists.txt          # CMake build configuration
|-------> inc/                    # Header files directory
|-------------> collection.h        # Dynamic array interface definitions
|-------------> typeinfo.h         # Generic type system interface
|-------------> stringTypeinfo.h   # String type implementation
|-------------> doubleTypeinfo.h   # Double type implementation
|-------------> util.h             # Utility functions and error handling
|-------> src/                    # Source files directory
|-------------> collection.c        # Dynamic array implementation
|-------------> stringTypeinfo.c   # String type operations
|-------------> doubleTypeinfo.c   # Double type operations
|-------------> sort.c             # Sorting algorithms implementation
|-------> tests/                  # Test files directory
|-------------> testArray.cpp      # Dynamic array tests
|-------------> testString.cpp     # String type tests
|-------------> testDouble.cpp     # Double type tests
|-------------> testMain.cpp       # Test runner
```

## Usage Instructions
### Prerequisites
- C compiler supporting C11 standard
- CMake 3.10 or higher
- Google Test framework for running tests
- C++ compiler supporting C++11 (for tests only)

### Installation

<cpp_installation_instructions>
The project requires a C++11 compatible compiler for tests and C11 for the main library.

For macOS:
```bash
xcode-select --install
```

For Linux:
```bash
sudo apt-get update
sudo apt-get install build-essential cmake
```

For Windows:
```bash
# Install Visual Studio with C++ development tools or:
choco install mingw cmake
```

Build the project:
```bash
mkdir build
cd build
cmake ..
make
```
</cpp_installation_instructions>

### Quick Start
```c
#include "collection.h"
#include "doubleTypeinfo.h"

int main() {
    // Initialize a dynamic array for doubles
    DynamicArray* array = NULL;
    init(&array, getDoubleTI(), 5);

    // Add elements
    double value = 3.14;
    elemPtr elem = &value;
    append(array, &elem);

    // Use array operations
    map(array, someMapFunction);
    where(array, somePredicate);

    // Clean up
    deleteArray(array);
    return 0;
}
```

### More Detailed Examples
```c
// Creating and manipulating a string array
DynamicArray* strArray = NULL;
init(&strArray, getStringTI(), 5);

// Add strings
char* str = "Hello";
elemPtr strPtr = str;
append(strArray, &strPtr);

// Sort the array
bubbleSort(strArray, ASCENDING);

// Filter strings
where(strArray, isUpperCase);
```

### Troubleshooting
Common issues and solutions:

1. Memory Allocation Errors
```c
if (result == MEMORY_ALLOCATION_ERROR) {
    // Check system memory
    // Reduce initial capacity
    init(&array, typeInfo, MINIMUM_CAPACITY);
}
```

2. Type Mismatch Errors
```c
// Ensure correct TypeInfo is used
const TypeInfo* typeInfo = getStringTI(); // for strings
const TypeInfo* typeInfo = getDoubleTI(); // for doubles
```

Key component interactions:
1. TypeInfo structures provide type-specific operations
2. Dynamic array manages memory and element storage
3. Collection operations use TypeInfo for type-safe manipulation
4. Memory management handles automatic resizing
5. Sorting algorithms operate through TypeInfo comparators
6. Map/Where operations use type-specific predicates
7. Input/Output handled through type-specific functions