PROJECT(SpellHandlers)

SET( prefix ${ROOT_PATH}/src/scripts/src/${PROJECT_NAME} )

#sources -contains source files for our library.
SET( sources
DeathKnightSpells.cpp
DruidSpells.cpp
HunterSpells.cpp
ItemSpells_1.cpp
MageSpells.cpp
PaladinSpells.cpp
PetAISpells.cpp
PriestSpells.cpp
QIspells.cpp
RogueSpells.cpp
Setup.cpp
ShamanSpells.cpp
WarlockSpells.cpp
WarriorSpells.cpp
MiscSpells.cpp )

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)

include_directories( ${GLOBAL_INCLUDE_DIRS} )
add_library( ${PROJECT_NAME} SHARED ${SRCS} )
add_dependencies( ${PROJECT_NAME} shared world )
add_definitions( -DSCRIPTLIB -DSKIP_ALLOCATOR_SHARING )
target_link_libraries( ${PROJECT_NAME} world )

include(PCHSupport)
IF( CMAKE_GENERATOR MATCHES Visual*)
	ADD_NATIVE_PRECOMPILED_HEADER(${PROJECT_NAME} Setup.h ${prefix}/Setup.cpp)
ENDIF()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${ARCEMU_SCRIPTLIB_PATH} LIBRARY DESTINATION ${ARCEMU_SCRIPTLIB_PATH} )
#Cleap up our variables(Not required.)
unset( prefix )
unset( SRCS )
unset( sources )
