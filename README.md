# Generic Dynamic Array Implementation in C with Type-Safe Operations

A polymorphic dynamic array implementation in C. The library offers array operations, sorting algorithms, and type-specific manipulations through a unified interface.

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

### Installation

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
