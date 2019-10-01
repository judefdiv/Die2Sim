# To run the code:
#  - mkdir build && cd build
#  - cmake ..
#  - make

cmake_minimum_required(VERSION 2.6)
project(Die2Sim)                                  # creates ${PROJECT_NAME}

set(SOURCES
  src/main.cpp
  src/ToGds.cpp
  src/ClassLef.cpp
  src/ParserLef.cpp
  src/ClassDef.cpp
  src/ParserDef.cpp
  src/ForgeGdsii.cpp
  src/ParserGdsii.cpp
  src/ToJosim.cpp
  src/ParserJosim.cpp
  src/genFunc.cpp
  src/ToText.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# Ensures that the header files of the project is included
target_include_directories(${PROJECT_NAME} PRIVATE 
  ${PROJECT_SOURCE_DIR}/include
)