PROJECT(g3dlite)

set(prefix ${DEPENDENCY_SOURCES}/${PROJECT_NAME} )

set(sources
  AABox.cpp
  Any.cpp
  BinaryFormat.cpp
  BinaryInput.cpp
  BinaryOutput.cpp
  Box.cpp
  Capsule.cpp
  CollisionDetection.cpp
  CoordinateFrame.cpp
  Crypto.cpp
  Cylinder.cpp
  debugAssert.cpp
  fileutils.cpp
  format.cpp
  g3dfnmatch.cpp
  g3dmath.cpp
  Line.cpp
  LineSegment.cpp
  Log.cpp
  Matrix.cpp
  Matrix3.cpp
  Matrix4.cpp
  MemoryManager.cpp
  Plane.cpp
  prompt.cpp
  Quat.cpp
  Random.cpp
  Ray.cpp
  ReferenceCount.cpp
  RegistryUtil.cpp
  Sphere.cpp
  stringutils.cpp
  System.cpp
  TextInput.cpp
  TextOutput.cpp
  Triangle.cpp
  UprightFrame.cpp
  RegistryUtil.cpp
  Vector2.cpp
  Vector3.cpp
  Vector4.cpp )

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include_directories( ${GLOBAL_INCLUDE_DIRS} )
add_library(${PROJECT_NAME} STATIC ${SRCS} )

unset(sources)
unset(SRCS)
unset(prefix)

