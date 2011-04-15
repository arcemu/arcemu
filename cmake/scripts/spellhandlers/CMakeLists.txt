if(NOT BUILD_SPELLHANDLERS )
  message("\tSpellHandlers was skipped because BUILD_SPELLHANDLERS is unchecked.")
  return()
endif(NOT BUILD_SPELLHANDLERS)

PROJECT(SpellHandlers)

SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )

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

include_directories( ${include_dirs} )
add_library( ${PROJECT_NAME} SHARED ${SRCS} )
add_dependencies( ${PROJECT_NAME} ${dependancies} )
target_link_libraries( ${PROJECT_NAME} ${linkages} )
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
#Cleap up our variables(Not required.)
unset( prefix)
unset( SRCS )
unset( sources )
