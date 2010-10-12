#ifndef __WORLDSTATES_H
#define __WORLDSTATES_H

/**
 * EOTS (Eye Of The Storm) World States
 */

// control points - draenei ruins
#define WORLDSTATE_EOTS_DRAENEI_RUINS_NETURAL_DISPLAY                  2731
#define WORLDSTATE_EOTS_DRAENEI_RUINS_ALLIANCE_DISPLAY                 2732
#define WORLDSTATE_EOTS_DRAENEI_RUINS_HORDE_DISPLAY                            2733

// control points - mage tower
#define WORLDSTATE_EOTS_MAGE_TOWER_NETURAL_DISPLAY                             2728
#define WORLDSTATE_EOTS_MAGE_TOWER_HORDE_DISPLAY                               2729
#define WORLDSTATE_EOTS_MAGE_TOWER_ALLIANCE_DISPLAY                            2730

// control points - fel reaver ruins
#define WORLDSTATE_EOTS_FELREAVER_RUINS_NETURAL_DISPLAY                        2725
#define WORLDSTATE_EOTS_FELREAVER_RUINS_ALLIANCE_DISPLAY               2726
#define WORLDSTATE_EOTS_FELREAVER_RUINS_HORDE_DISPLAY                  2727

// control points - blood elf tower
#define WORLDSTATE_EOTS_BLOODELF_TOWER_NEUTRAL_DISPLAY                 2722
#define WORLDSTATE_EOTS_BLOODELF_TOWER_ALLIANCE_DISPLAY                        2723
#define WORLDSTATE_EOTS_BLOODELF_TOWER_HORDE_DISPLAY                   2724

// flag
#define WORLDSTATE_EOTS_FLAG_NEUTRAL_DISPLAY                                   2757

// displays
#define WORLDSTATE_EOTS_ALLIANCE_VICTORY_POINTS_DISPLAY                        2769
#define WORLDSTATE_EOTS_HORDE_VICTORY_POINTS_DISPLAY                   2770
#define WORLDSTATE_EOTS_ALLIANCE_VICTORYPOINTS                                 2749
#define WORLDSTATE_EOTS_HORDE_VICTORYPOINTS                                            2750
#define WORLDSTATE_EOTS_ALLIANCE_BASES                                                 2752
#define WORLDSTATE_EOTS_HORDE_BASES                                                            2753

#define EOTS_WORLDSTATE_DISPLAYON 2718
#define EOTS_WORLDSTATE_DISPLAYVALUE 2719
#define EOTS_WORLDSTATE_ALLIANCE_VICTORYPOINTS 2749
#define EOTS_WORLDSTATE_HORDE_VICTORYPOINTS 2750
#define EOTS_WORLDSTATE_ALLIANCE_BASES 2752
#define EOTS_WORLDSTATE_HORDE_BASES 2753
#define EOTS_NETHERWING_FLAG_SPELL 34976

/**
 * WGS World States
 */

#define WORLDSTATE_WSG_ALLIANCE_FLAG_DISPLAY                                   0x922
#define WORLDSTATE_WSG_HORDE_FLAG_DISPLAY                                              0x923
#define WORLDSTATE_WSG_HORDE_SCORE                                                             0x62E
#define WORLDSTATE_WSG_ALLIANCE_SCORE                                                  0x62D
#define WORLDSTATE_WSG_MAX_SCORE                                                               0x641

/**
 * AB World States
 */

#define AB_CAPTURED_STABLES_ALLIANCE           0x6E7 //1767
#define AB_CAPTURED_STABLES_HORDE                 0x6E8 //1768
#define AB_CAPTURING_STABLES_ALLIANCE     0x6E9 //1769
#define AB_CAPTURING_STABLES_HORDE               0x6EA //1770
// 0x6EB is unknown
#define AB_CAPTURED_FARM_ALLIANCE                 0x6EC //1772 // 1 is captured by the alliance
#define AB_CAPTURED_FARM_HORDE                   0x6ED // 1773 / 1 is captured by the horde
#define AB_CAPTURING_FARM_ALLIANCE               0x6EE // 1774 1 is capturing by the alliance
#define AB_CAPTURING_FARM_HORDE                         0x6EF // 1775 1 is capturing by the horde

#define AB_CAPTURED_BLACKSMITH_ALLIANCE         0x6F6 // 1782
#define AB_CAPTURED_BLACKSMITH_HORDE           0x6F7 //1783
#define AB_CAPTURING_BLACKSMITH_ALLIANCE       0x6F8 //1784
#define AB_CAPTURING_BLACKSMITH_HORDE     0x6F9 //1785
// 0x6FA is unknown
#define AB_CAPTURED_GOLDMINE_ALLIANCE     0x6FB //1787
#define AB_CAPTURED_GOLDMINE_HORDE               0x6FC//1788
#define AB_CAPTURING_GOLDMINE_ALLIANCE   0x6FD//1789
#define AB_CAPTURING_GOLDMINE_HORDE             0x6FE//1790
// 0x6FF is unknown
#define AB_CAPTURED_LUMBERMILL_ALLIANCE         0x700//1792
#define AB_CAPTURED_LUMBERMILL_HORDE           0x701//1793
#define AB_CAPTURING_LUMBERMILL_ALLIANCE       0x702//1794
#define AB_CAPTURING_LUMBERMILL_HORDE     0x703//1795

#define AB_SHOW_STABLE_ICON                             0x732//1842
#define AB_SHOW_GOLDMINE_ICON                     0x733//1843
#define AB_SHOW_LUMBERMILL_ICON                         0x734//1844
#define AB_SHOW_FARM_ICON                                 0x735//1845
#define AB_SHOW_BACKSMITH_ICON                   0x736//1846

/**
 * Arena World States
 */

#define ARENA_WORLD_STATE_A_PLAYER_COUNT 3600
#define ARENA_WORLD_STATE_H_PLAYER_COUNT 3601

#define WORLDSTATE_ARENA_BLADESEDGE_GREEN_PLAYER_COUNT                 3600
#define WORLDSTATE_ARENA_BLADESEDGE_GOLD_PLAYER_COUNT                  3601
#define WORLDSTATE_ARENA_BLADESEDGE_SCORE_SHOW                                 2547

#define WORLDSTATE_ARENA_NAGRAND_GREEN_PLAYER_COUNT                            3600
#define WORLDSTATE_ARENA_NAGRAND_GOLD_PLAYER_COUNT                             3601
#define WORLDSTATE_ARENA_NAGRAND_SCORE_SHOW                                            2577

#define WORLDSTATE_ARENA_LORDAERON_GREEN_PLAYER_COUNT                  3600
#define WORLDSTATE_ARENA_LORDAERON_GOLD_PLAYER_COUNT                   3601
#define WORLDSTATE_ARENA_LORDAERON_SCORE_SHOW                                  3002

#define WORLDSTATE_ARENA_WOTLK_GREEN_PLAYER_COUNT                              3600
#define WORLDSTATE_ARENA_WOTLK_GOLD_PLAYER_COUNT                               3601
#define WORLDSTATE_ARENA_WOTLK_SCORE_SHOW                                              3610

/**
* AV World States
*/

#define WORLDSTATE_AV_ALLIANCE_SCORE                                                           3127
#define WORLDSTATE_AV_HORDE_SCORE                                                                      3128
#define WORLDSTATE_AV_ALLIANCE_SCORE_DISPLAY                                           3133
#define WORLDSTATE_AV_HORDE_SCORE_DISPLAY                                                      3134

#define WORLDSTATE_AV_STORMPIKE_AID_STATION_ALLIANCE_CONTROLLED                1325
#define WORLDSTATE_AV_STORMPIKE_AID_STATION_HORDE_CONTROLLED           1327
#define WORLDSTATE_AV_STORMPIKE_AID_STATION_ALLIANCE_ASSAULTING                1326
#define WORLDSTATE_AV_STORMPIKE_AID_STATION_HORDE_ASSAULTING           1328

#define WORLDSTATE_AV_STORMPIKE_GRAVE_ALLIANCE_CONTROLLED                      1333
#define WORLDSTATE_AV_STORMPIKE_GRAVE_HORDE_CONTROLLED                         1334
#define WORLDSTATE_AV_STORMPIKE_GRAVE_ALLIANCE_ASSAULTING                      1335
#define WORLDSTATE_AV_STORMPIKE_GRAVE_HORDE_ASSAULTING                         1336

#define WORLDSTATE_AV_STONEHEARTH_GRAVE_ALLIANCE_CONTROLLED                    1302
#define WORLDSTATE_AV_STONEHEARTH_GRAVE_HORDE_CONTROLLED                       1301
#define WORLDSTATE_AV_STONEHEARTH_GRAVE_ALLIANCE_ASSAULTING                    1304
#define WORLDSTATE_AV_STONEHEARTH_GRAVE_HORDE_ASSAULTING                       1303

#define WORLDSTATE_AV_SNOWFALL_GRAVE_NEUTRAL_CONTROLLED                                1966
#define WORLDSTATE_AV_SNOWFALL_GRAVE_ALLIANCE_CONTROLLED                       1341
#define WORLDSTATE_AV_SNOWFALL_GRAVE_HORDE_CONTROLLED                          1342
#define WORLDSTATE_AV_SNOWFALL_GRAVE_ALLIANCE_ASSAULTING                       1343
#define WORLDSTATE_AV_SNOWFALL_GRAVE_HORDE_ASSAULTING                          1344

#define WORLDSTATE_AV_IRONDEEP_MINE_ALLIANCE_CONTROLLED                                1358
#define WORLDSTATE_AV_IRONDEEP_MINE_HORDE_CONTROLLED                           1359
#define WORLDSTATE_AV_IRONDEEP_MINE_TROGG_CONTROLLED                           1360

#define WORLDSTATE_AV_COLDTOOTH_MINE_ALLIANCE_CONTROLLED                       1355
#define WORLDSTATE_AV_COLDTOOTH_MINE_HORDE_CONTROLLED                          1356
#define WORLDSTATE_AV_COLDTOOTH_MINE_KOBOLD_CONTROLLED                         1357

#define WORLDSTATE_AV_ICEBLOOD_GRAVE_ALLIANCE_CONTROLLED                       1346
#define WORLDSTATE_AV_ICEBLOOD_GRAVE_HORDE_CONTROLLED                          1347
#define WORLDSTATE_AV_ICEBLOOD_GRAVE_ALLIANCE_ASSAULTING                       1348
#define WORLDSTATE_AV_ICEBLOOD_GRAVE_HORDE_ASSAULTING                          1349

#define WORLDSTATE_AV_FROSTWOLF_GRAVE_ALLIANCE_CONTROLLED                      1337
#define WORLDSTATE_AV_FROSTWOLF_GRAVE_HORDE_CONTROLLED                         1338
#define WORLDSTATE_AV_FROSTWOLF_GRAVE_ALLIANCE_ASSAULTING                      1339
#define WORLDSTATE_AV_FROSTWOLF_GRAVE_HORDE_ASSAULTING                         1340

#define WORLDSTATE_AV_FROSTWOLF_RELIEF_HUT_ALLIANCE_CONTROLLED         1329
#define WORLDSTATE_AV_FROSTWOLF_RELIEF_HUT_HORDE_CONTROLLED                    1330
#define WORLDSTATE_AV_FROSTWOLF_RELIEF_HUT_ALLIANCE_ASSAULTING         1331
#define WORLDSTATE_AV_FROSTWOLF_RELIEF_HUT_HORDE_ASSAULTING                    1332

#define WORLDSTATE_AV_DUN_BALDAR_SOUTH_BUNKER_CONTROLLED                       1362
#define WORLDSTATE_AV_DUN_BALDAR_SOUTH_BUNKER_DESTROYED                                1371
#define WORLDSTATE_AV_DUN_BALDAR_SOUTH_BUNKER_ASSAULTING                       1379

#define WORLDSTATE_AV_DUN_BALDAR_NORTH_BUNKER_CONTROLLED                       1361
#define WORLDSTATE_AV_DUN_BALDAR_NORTH_BUNKER_DESTROYED                                1370
#define WORLDSTATE_AV_DUN_BALDAR_NORTH_BUNKER_ASSAULTING                       1378

#define WORLDSTATE_AV_ICEWING_BUNKER_CONTROLLED                                                1363
#define WORLDSTATE_AV_ICEWING_BUNKER_DESTROYED                                         1372
#define WORLDSTATE_AV_ICEWING_BUNKER_ASSAULTING                                                1380

#define WORLDSTATE_AV_STONEHEARTH_BUNKER_CONTROLLED                                    1364
#define WORLDSTATE_AV_STONEHEARTH_BUNKER_DESTROYED                                     1373
#define WORLDSTATE_AV_STONEHEARTH_BUNKER_ASSAULTING                                    1381

#define WORLDSTATE_AV_ICEBLOOD_TOWER_CONTROLLED                                                1385
#define WORLDSTATE_AV_ICEBLOOD_TOWER_DESTROYED                                         1368
#define WORLDSTATE_AV_ICEBLOOD_TOWER_ASSAULTING                                                1390

#define WORLDSTATE_AV_TOWERPOINT_CONTROLLED                                                    1384
#define WORLDSTATE_AV_TOWERPOINT_DESTROYED                                                     1367
#define WORLDSTATE_AV_TOWERPOINT_ASSAULTING                                                    1389

#define WORLDSTATE_AV_FROSTWOLF_EAST_TOWER_CONTROLLED                          1383
#define WORLDSTATE_AV_FROSTWOLF_EAST_TOWER_DESTROYED                           1366
#define WORLDSTATE_AV_FROSTWOLF_EAST_TOWER_ASSAULTING                          1388

#define WORLDSTATE_AV_FROSTWOLF_WEST_TOWER_CONTROLLED                          1382
#define WORLDSTATE_AV_FROSTWOLF_WEST_TOWER_DESTROYED                           1365
#define WORLDSTATE_AV_FROSTWOLF_WEST_TOWER_ASSAULTING                          1387

/**
 * Common World States
 */
#define WORLDSTATE_EOTS_PVP_CAPTURE_BAR_DISPLAY                                        2718
#define WORLDSTATE_EOTS_PVP_CAPTURE_BAR_VALUE                                  2719

/**
* Battleground
* Strand of the Ancients
*/

// Progress bar
#define WORLDSTATE_SOTA_CAPTURE_BAR_DISPLAY                                    3536
#define WORLDSTATE_SOTA_CAPTURE_BAR_VALUE                                      3537

// Attacker - Defender, they are same
#define WORLDSTATE_SOTA_ALLIANCE_ATTACKER                                      3564
#define WORLDSTATE_SOTA_HORDE_DEFENDER                                         3564

#define WORLDSTATE_SOTA_ALLIANCE_DEFENDER                                      3565
#define WORLDSTATE_SOTA_HORDE_ATTACKER                                         3565

// Timers
#define WORLDSTATE_SOTA_BONUS_TIME                                     3571
#define WORLDSTATE_SOTA_TIMER_1                                                3559 //first digit
#define WORLDSTATE_SOTA_TIMER_2                                                3560 //second digit
#define WORLDSTATE_SOTA_TIMER_3                                                3561 //third digit

#endif         // __WORLDSTATES_H
