if(NOT BUILD_QUESTSCRIPTS )
  message("\tQuestScripts was skipped because BUILD_QUESTSCRIPTS is unchecked.")
  return()
endif(NOT BUILD_QUESTSCRIPTS)

PROJECT(QuestScripts)

SET( prefix ${SCRIPTS_PATH}/${PROJECT_NAME} )

SET( sources
Setup.cpp 
../Common/Base.cpp 
Quest_ArathiHighlands.cpp 
Quest_Azshara.cpp 
Quest_Azuremyst_Isle.cpp 
Quest_BladeEdge_Mountains.cpp 
Quest_BlastedLands.cpp 
Quest_BloodmystIsle.cpp 
Quest_BoreanTundra.cpp 
Quest_BurningSteppes.cpp 
Quest_Darkshore.cpp 
Quest_DeathKnight.cpp 
Quest_Desolace.cpp 
Quest_Dragonblight.cpp 
Quest_Druid.cpp 
Quest_Duskwood.cpp 
Quest_Dustwallow_Marsh.cpp 
Quest_Eastern_Plaguelands.cpp 
Quest_EversongWoods.cpp 
Quest_FirstAid.cpp 
Quest_Ghostlands.cpp 
Quest_Hellfire_Peninsula.cpp 
Quest_HillsbradFoothills.cpp 
Quest_Isle_of_QuelDanas.cpp 
Quest_Howling_Fjord.cpp 
Quest_LochModan.cpp 
Quest_Mage.cpp 
Quest_Mulgore.cpp 
Quest_Nagrand.cpp 
Quest_Netherstorm.cpp 
Quest_Paladin.cpp 
QuestGossip.cpp 
QuestHooks.cpp 
Quest_RedridgeMountains.cpp 
Quest_ShadowMoon.cpp 
Quest_SholazarBasin.cpp 
Quest_Silithus.cpp 
Quest_SilvermoonCity.cpp 
Quest_Silverpine_Forest.cpp 
Quest_Stormwind.cpp 
Quest_StranglethornVale.cpp 
Quest_Tanaris.cpp 
Quest_Teldrassil.cpp 
Quest_Terrokar_Forest.cpp 
Quest_The_Barrens.cpp 
Quest_TheStormPeaks.cpp 
Quest_ThousandNeedles.cpp 
Quest_TirisfalGlades.cpp 
Quest_Undercity.cpp 
Quest_UnGoro.cpp 
Unsorted.cpp 
Quest_Warrior.cpp 
Quest_Westfall.cpp 
Quest_Wetlands.cpp
Quest_Winterspring.cpp 
Quest_Zangarmarsh.cpp )

SET( headers
Setup.h
../Common/Base.h
../Common/EasyFunctions.h
../Common/Instance_Base.h )

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)
foreach(src IN ITEMS ${headers} )
  SET( HEADERS ${HEADERS} ${prefix}/${src} )
endforeach(src)

include_directories( ${include_dirs} ${prefix} )
add_library( ${PROJECT_NAME} SHARED ${SRCS} ${HEADERS} )
add_dependencies( ${PROJECT_NAME} ${dependancies} )
target_link_libraries( ${PROJECT_NAME} ${linkages} )

include(PCHSupport)
IF( CMAKE_GENERATOR MATCHES Visual*)
	ADD_NATIVE_PRECOMPILED_HEADER(${PROJECT_NAME} Setup.h ${prefix}/Setup.cpp)
ENDIF()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${ARCEMU_SCRIPTLIB_PATH} LIBRARY DESTINATION ${ARCEMU_SCRIPTLIB_PATH})
#Cleap up our variables(Not required.)
unset( prefix)
unset( SRCS )
unset( sources )
