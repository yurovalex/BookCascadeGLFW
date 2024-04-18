cmake_minimum_required(VERSION 3.2)

project(glfw-occt-demo)
set(APP_TARGET glfwocct)

set(CMAKE_CXX_STANDARD 20)

set_target_properties(${TARGET} PROPERTIES

    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

file(GLOB SOURCES
  *.h
  *.hpp
  *.cpp
)
source_group ("Headers" FILES
  GlfwOcctView.h
  GlfwOcctWindow.h
  modelingOCCT.h
)
source_group ("Sources" FILES
  GlfwOcctView.cpp
  GlfwOcctWindow.cpp
  modelingOCCT.cpp
  main.cpp)


# Open CASCADE Technology
# Set the path to the 3rd party library
set(THIRD_PARTY_PATH /run/media/alexey/LinuxData/Cascade/occt_build780)


include_directories(${THIRD_PARTY_PATH}/inc)
link_directories(${THIRD_PARTY_PATH}/lin64/gcc/libd)

SET(OpenCASCADE_LIBS 
  TKernel TKService TKV3d TKOpenGl TKBRep
  TKGeomBase TKGeomAlgo TKG3d TKG2d TKTopAlgo
  TKPrim TKMath TKTopAlgo TKBin TKBool TKBO TKFillet
  TKOffset TKTObj TKFeat TKMesh TKOffset TKXSBase
  TKDESTEP TKDEIGES TKDESTL TKDEVRML TKLCAF
)

# glfw
find_package(glfw3 REQUIRED)
if (glfw3_FOUND)
  message (STATUS "Using glfw3 ${glfw3_VERSION}" )
  INCLUDE_DIRECTORIES(${GLFW_INCLUDE_DIRS})
  LINK_DIRECTORIES(${GLFW_LIBRARY_DIRS})
else()
  message (STATUS "glfw3 is not found." )
endif()

add_executable(${APP_TARGET} ${SOURCES})
target_link_libraries(
  ${APP_TARGET}
  ${OpenCASCADE_LIBS}
  glfw
)

