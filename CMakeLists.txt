# To run the code:
#  - mkdir build && cd build
#  - cmake ..
#  - make

# cmake_minimum_required(VERSION 2.6)
cmake_minimum_required(VERSION 3.13.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_OSX_ARCHITECTURES x86_64)

project(Die2Sim VERSION 0.9)
# project(Die2Sim)                                  # creates ${PROJECT_NAME}

# target_compile_features(Die2Sim PUBLIC cxx_std_17)

set(SOURCES
  src/main.cpp
  src/ParserDef.cpp
  src/ToJosim.cpp
  src/ToVerilog.cpp
  src/ParserJosim.cpp
  src/genFunc.cpp
  src/EmitterVerilog.cpp
  src/TestPattern.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# Ensures that the header files of the project is included
target_include_directories(${PROJECT_NAME} PRIVATE
  ${PROJECT_SOURCE_DIR}/include
)