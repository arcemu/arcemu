PROJECT(collision)
SET( prefix ${ROOT_PATH}/src/${PROJECT_NAME} )
SET( sources
  vmap/AABSPTree.h
  vmap/BaseModel.cpp
  vmap/BaseModel.h
  vmap/CoordModelMapping.cpp
  vmap/CoordModelMapping.h
  vmap/IVMapManager.h
  vmap/ManagedModelContainer.cpp
  vmap/ManagedModelContainer.h
  vmap/ModelContainer.cpp
  vmap/ModelContainer.h
  vmap/NodeValueAccess.h
  vmap/RayIntersectionIterator.h
  vmap/ShortBox.h
  vmap/ShortVector.h
  vmap/SubModel.cpp
  vmap/SubModel.h
  vmap/TileAssembler.cpp
  vmap/TileAssembler.h
  vmap/TreeNode.cpp
  vmap/TreeNode.h
  vmap/VMapManager.cpp
  vmap/VMapManager.h
  g3dlite/AABox.cpp
  g3dlite/Box.cpp
  g3dlite/Crypto.cpp
  g3dlite/Matrix3.cpp
  g3dlite/Plane.cpp
  g3dlite/System.cpp
  g3dlite/Triangle.cpp
  g3dlite/Vector3.cpp
  g3dlite/Vector4.cpp
  g3dlite/format.cpp
  Interface.cpp )

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

SET(include_dirs 
  ${prefix}/vmap 
  ${prefix}/g3dlite/
  ${prefix} )

ADD_DEFINITIONS(-Wno-deprecated)
include_directories( ${include_dirs} )
ADD_LIBRARY(${PROJECT_NAME} STATIC ${SRCS} )
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)

unset(sources)
unset(SRCS)
unset(prefix)

