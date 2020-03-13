# To run the code:
#  - mkdir build && cd build
#  - cmake ..
#  - make

cmake_minimum_required(VERSION 2.6)
project(Die2Sim)                                  # creates ${PROJECT_NAME}

# target_compile_features(Die2Sim PUBLIC cxx_std_17)

set(SOURCES
  src/main.cpp
  src/ParserDef.cpp
  src/ToJosim.cpp
  src/ParserJosim.cpp
  src/genFunc.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# Ensures that the header files of the project is included
target_include_directories(${PROJECT_NAME} PRIVATE
  ${PROJECT_SOURCE_DIR}/include
)