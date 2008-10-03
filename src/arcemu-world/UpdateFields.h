/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/** 2.4.3 UpdateFields as of 16/07/08
 */

#ifndef __arcemu_UPDATEFIELDS_H
#define __arcemu_UPDATEFIELDS_H

//OBJECT:
#define OBJECT_FIELD_GUID                                      	0x000	//  Size: 2, Type: GUID, Flags: 1
#define OBJECT_FIELD_GUID_01                                   	0x001	//  Size: 2, Type: GUID, Flags: 1
#define OBJECT_FIELD_TYPE                                      	0x002	//  Size: 1, Type: Int32, Flags: 1
#define OBJECT_FIELD_ENTRY                                     	0x003	//  Size: 1, Type: Int32, Flags: 1
#define OBJECT_FIELD_SCALE_X                                   	0x004	//  Size: 1, Type: Float, Flags: 1
#define OBJECT_FIELD_PADDING                                   	0x005	//  Size: 1, Type: Int32, Flags: 0
#define OBJECT_END                                              0x006

//CONTAINER:
#define CONTAINER_FIELD_NUM_SLOTS                              	ITEM_END + 0x000	//  Size: 1, Type: Int32, Flags: 1
#define CONTAINER_ALIGN_PAD                                    	ITEM_END + 0x001	//  Size: 1, Type: Bytes, Flags: 0
#define CONTAINER_FIELD_SLOT_1                                 	ITEM_END + 0x002	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_01                                	ITEM_END + 0x003	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_02                                	ITEM_END + 0x004	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_03                                	ITEM_END + 0x005	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_04                                	ITEM_END + 0x006	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_05                                	ITEM_END + 0x007	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_06                                	ITEM_END + 0x008	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_07                                	ITEM_END + 0x009	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_08                                	ITEM_END + 0x00A	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_09                                	ITEM_END + 0x00B	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_10                                	ITEM_END + 0x00C	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_11                                	ITEM_END + 0x00D	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_12                                	ITEM_END + 0x00E	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_13                                	ITEM_END + 0x00F	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_14                                	ITEM_END + 0x010	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_15                                	ITEM_END + 0x011	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_16                                	ITEM_END + 0x012	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_17                                	ITEM_END + 0x013	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_18                                	ITEM_END + 0x014	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_19                                	ITEM_END + 0x015	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_20                                	ITEM_END + 0x016	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_21                                	ITEM_END + 0x017	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_22                                	ITEM_END + 0x018	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_23                                	ITEM_END + 0x019	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_24                                	ITEM_END + 0x01A	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_25                                	ITEM_END + 0x01B	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_26                                	ITEM_END + 0x01C	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_27                                	ITEM_END + 0x01D	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_28                                	ITEM_END + 0x01E	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_29                                	ITEM_END + 0x01F	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_30                                	ITEM_END + 0x020	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_31                                	ITEM_END + 0x021	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_32                                	ITEM_END + 0x022	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_33                                	ITEM_END + 0x023	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_34                                	ITEM_END + 0x024	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_35                                	ITEM_END + 0x025	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_36                                	ITEM_END + 0x026	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_37                                	ITEM_END + 0x027	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_38                                	ITEM_END + 0x028	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_39                                	ITEM_END + 0x029	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_40                                	ITEM_END + 0x02A	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_41                                	ITEM_END + 0x02B	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_42                                	ITEM_END + 0x02C	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_43                                	ITEM_END + 0x02D	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_44                                	ITEM_END + 0x02E	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_45                                	ITEM_END + 0x02F	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_46                                	ITEM_END + 0x030	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_47                                	ITEM_END + 0x031	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_48                                	ITEM_END + 0x032	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_49                                	ITEM_END + 0x033	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_50                                	ITEM_END + 0x034	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_51                                	ITEM_END + 0x035	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_52                                	ITEM_END + 0x036	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_53                                	ITEM_END + 0x037	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_54                                	ITEM_END + 0x038	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_55                                	ITEM_END + 0x039	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_56                                	ITEM_END + 0x03A	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_57                                	ITEM_END + 0x03B	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_58                                	ITEM_END + 0x03C	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_59                                	ITEM_END + 0x03D	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_60                                	ITEM_END + 0x03E	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_61                                	ITEM_END + 0x03F	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_62                                	ITEM_END + 0x040	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_63                                	ITEM_END + 0x041	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_64                                	ITEM_END + 0x042	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_65                                	ITEM_END + 0x043	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_66                                	ITEM_END + 0x044	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_67                                	ITEM_END + 0x045	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_68                                	ITEM_END + 0x046	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_69                                	ITEM_END + 0x047	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_70                                	ITEM_END + 0x048	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_FIELD_SLOT_71                                	ITEM_END + 0x049	// Size: 72, Type: GUID, Flags: 1
#define CONTAINER_END                                          	ITEM_END + 0x04A	// Size: 1, Type: Int32, Flags: 0

//ITEM:
#define ITEM_FIELD_OWNER                                       	OBJECT_END + 0x000	//  Size: 2, Type: GUID, Flags: 1
#define ITEM_FIELD_CONTAINED                                   	OBJECT_END + 0x002	//  Size: 2, Type: GUID, Flags: 1
#define ITEM_FIELD_CREATOR                                     	OBJECT_END + 0x004	//  Size: 2, Type: GUID, Flags: 1
#define ITEM_FIELD_GIFTCREATOR                                 	OBJECT_END + 0x006	//  Size: 2, Type: GUID, Flags: 1
#define ITEM_FIELD_STACK_COUNT                                 	OBJECT_END + 0x008	//  Size: 1, Type: Int32, Flags: 20
#define ITEM_FIELD_DURATION                                    	OBJECT_END + 0x009	//  Size: 1, Type: Int32, Flags: 20
#define ITEM_FIELD_SPELL_CHARGES                               	OBJECT_END + 0x00A	//  Size: 5, Type: Int32, Flags: 20
#define ITEM_FIELD_SPELL_CHARGES_01                            	OBJECT_END + 0x00B	//  Size: 5, Type: Int32, Flags: 20
#define ITEM_FIELD_SPELL_CHARGES_02                            	OBJECT_END + 0x00C	//  Size: 5, Type: Int32, Flags: 20
#define ITEM_FIELD_SPELL_CHARGES_03                            	OBJECT_END + 0x00D	//  Size: 5, Type: Int32, Flags: 20
#define ITEM_FIELD_SPELL_CHARGES_04                            	OBJECT_END + 0x00E	//  Size: 5, Type: Int32, Flags: 20
#define ITEM_FIELD_FLAGS                                       	OBJECT_END + 0x00F	//  Size: 1, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT                                 	OBJECT_END + 0x010	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_01                              	OBJECT_END + 0x011	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_02                              	OBJECT_END + 0x012	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_03                              	OBJECT_END + 0x013	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_04                              	OBJECT_END + 0x014	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_05                              	OBJECT_END + 0x015	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_06                              	OBJECT_END + 0x016	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_07                              	OBJECT_END + 0x017	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_08                              	OBJECT_END + 0x018	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_09                              	OBJECT_END + 0x019	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_10                              	OBJECT_END + 0x01A	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_11                              	OBJECT_END + 0x01B	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_12                              	OBJECT_END + 0x01C	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_13                              	OBJECT_END + 0x01D	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_14                              	OBJECT_END + 0x01E	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_15                              	OBJECT_END + 0x01F	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_16                              	OBJECT_END + 0x020	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_17                              	OBJECT_END + 0x021	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_18                              	OBJECT_END + 0x022	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_19                              	OBJECT_END + 0x023	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_20                              	OBJECT_END + 0x024	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_21                              	OBJECT_END + 0x025	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_22                              	OBJECT_END + 0x026	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_23                              	OBJECT_END + 0x027	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_24                              	OBJECT_END + 0x028	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_25                              	OBJECT_END + 0x029	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_26                              	OBJECT_END + 0x02A	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_27                              	OBJECT_END + 0x02B	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_28                              	OBJECT_END + 0x02C	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_29                              	OBJECT_END + 0x02D	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_30                              	OBJECT_END + 0x02E	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_31                              	OBJECT_END + 0x02F	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_ENCHANTMENT_32                              	OBJECT_END + 0x030	//  Size: 33, Type: Int32, Flags: 1
#define ITEM_FIELD_PROPERTY_SEED                               	OBJECT_END + 0x031	//  Size: 1, Type: Int32, Flags: 1
#define ITEM_FIELD_RANDOM_PROPERTIES_ID                        	OBJECT_END + 0x032	//  Size: 1, Type: Int32, Flags: 1
#define ITEM_FIELD_ITEM_TEXT_ID                                	OBJECT_END + 0x033	//  Size: 1, Type: Int32, Flags: 4
#define ITEM_FIELD_DURABILITY                                  	OBJECT_END + 0x034	//  Size: 1, Type: Int32, Flags: 20
#define ITEM_FIELD_MAXDURABILITY                               	OBJECT_END + 0x035	//  Size: 1, Type: Int32, Flags: 20
#define ITEM_END                                                OBJECT_END + 0x036

//UNIT:
#define UNIT_FIELD_CHARM                                       	OBJECT_END + 0x000	//  Size: 2, Type: GUID, Flags: 1
#define UNIT_FIELD_SUMMON                                      	OBJECT_END + 0x002	//  Size: 2, Type: GUID, Flags: 1
#define UNIT_FIELD_CHARMEDBY                                   	OBJECT_END + 0x004	//  Size: 2, Type: GUID, Flags: 1
#define UNIT_FIELD_SUMMONEDBY                                  	OBJECT_END + 0x006	//  Size: 2, Type: GUID, Flags: 1
#define UNIT_FIELD_CREATEDBY                                   	OBJECT_END + 0x008	//  Size: 2, Type: GUID, Flags: 1
#define UNIT_FIELD_TARGET                                      	OBJECT_END + 0x00A	//  Size: 2, Type: GUID, Flags: 1
#define UNIT_FIELD_PERSUADED                                   	OBJECT_END + 0x00C	//  Size: 2, Type: GUID, Flags: 1
#define UNIT_FIELD_CHANNEL_OBJECT                              	OBJECT_END + 0x00E	//  Size: 2, Type: GUID, Flags: 1
#define UNIT_FIELD_HEALTH                                      	OBJECT_END + 0x010	//  Size: 1, Type: Int32, Flags: 256
#define UNIT_FIELD_POWER1                                      	OBJECT_END + 0x011	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_POWER2                                      	OBJECT_END + 0x012	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_POWER3                                      	OBJECT_END + 0x013	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_POWER4                                      	OBJECT_END + 0x014	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_POWER5                                      	OBJECT_END + 0x015	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_MAXHEALTH                                   	OBJECT_END + 0x016	//  Size: 1, Type: Int32, Flags: 256
#define UNIT_FIELD_MAXPOWER1                                   	OBJECT_END + 0x017	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_MAXPOWER2                                   	OBJECT_END + 0x018	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_MAXPOWER3                                   	OBJECT_END + 0x019	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_MAXPOWER4                                   	OBJECT_END + 0x01A	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_MAXPOWER5                                   	OBJECT_END + 0x01B	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_LEVEL                                       	OBJECT_END + 0x01C	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_FACTIONTEMPLATE                             	OBJECT_END + 0x01D	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_BYTES_0                                     	OBJECT_END + 0x01E	//  Size: 1, Type: Bytes, Flags: 1
#define UNIT_VIRTUAL_ITEM_SLOT_DISPLAY                         	OBJECT_END + 0x01F	//  Size: 3, Type: Int32, Flags: 1
#define UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01                      	OBJECT_END + 0x020	//  Size: 3, Type: Int32, Flags: 1
#define UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_02                      	OBJECT_END + 0x021	//  Size: 3, Type: Int32, Flags: 1
#define UNIT_VIRTUAL_ITEM_INFO                                 	OBJECT_END + 0x022	//  Size: 6, Type: Bytes, Flags: 1
#define UNIT_VIRTUAL_ITEM_INFO_01                              	OBJECT_END + 0x023	//  Size: 6, Type: Bytes, Flags: 1
#define UNIT_VIRTUAL_ITEM_INFO_02                              	OBJECT_END + 0x024	//  Size: 6, Type: Bytes, Flags: 1
#define UNIT_VIRTUAL_ITEM_INFO_03                              	OBJECT_END + 0x025	//  Size: 6, Type: Bytes, Flags: 1
#define UNIT_VIRTUAL_ITEM_INFO_04                              	OBJECT_END + 0x026	//  Size: 6, Type: Bytes, Flags: 1
#define UNIT_VIRTUAL_ITEM_INFO_05                              	OBJECT_END + 0x027	//  Size: 6, Type: Bytes, Flags: 1
#define UNIT_FIELD_FLAGS                                       	OBJECT_END + 0x028	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_FLAGS_2                                     	OBJECT_END + 0x029	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA                                        	OBJECT_END + 0x02A	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_01                                     	OBJECT_END + 0x02B	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_02                                     	OBJECT_END + 0x02C	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_03                                     	OBJECT_END + 0x02D	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_04                                     	OBJECT_END + 0x02E	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_05                                     	OBJECT_END + 0x02F	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_06                                     	OBJECT_END + 0x030	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_07                                     	OBJECT_END + 0x031	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_08                                     	OBJECT_END + 0x032	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_09                                     	OBJECT_END + 0x033	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_10                                     	OBJECT_END + 0x034	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_11                                     	OBJECT_END + 0x035	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_12                                     	OBJECT_END + 0x036	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_13                                     	OBJECT_END + 0x037	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_14                                     	OBJECT_END + 0x038	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_15                                     	OBJECT_END + 0x039	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_16                                     	OBJECT_END + 0x03A	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_17                                     	OBJECT_END + 0x03B	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_18                                     	OBJECT_END + 0x03C	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_19                                     	OBJECT_END + 0x03D	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_20                                     	OBJECT_END + 0x03E	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_21                                     	OBJECT_END + 0x03F	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_22                                     	OBJECT_END + 0x040	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_23                                     	OBJECT_END + 0x041	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_24                                     	OBJECT_END + 0x042	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_25                                     	OBJECT_END + 0x043	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_26                                     	OBJECT_END + 0x044	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_27                                     	OBJECT_END + 0x045	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_28                                     	OBJECT_END + 0x046	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_29                                     	OBJECT_END + 0x047	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_30                                     	OBJECT_END + 0x048	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_31                                     	OBJECT_END + 0x049	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_32                                     	OBJECT_END + 0x04A	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_33                                     	OBJECT_END + 0x04B	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_34                                     	OBJECT_END + 0x04C	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_35                                     	OBJECT_END + 0x04D	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_36                                     	OBJECT_END + 0x04E	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_37                                     	OBJECT_END + 0x04F	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_38                                     	OBJECT_END + 0x050	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_39                                     	OBJECT_END + 0x051	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_40                                     	OBJECT_END + 0x052	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_41                                     	OBJECT_END + 0x053	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_42                                     	OBJECT_END + 0x054	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_43                                     	OBJECT_END + 0x055	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_44                                     	OBJECT_END + 0x056	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_45                                     	OBJECT_END + 0x057	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_46                                     	OBJECT_END + 0x058	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_47                                     	OBJECT_END + 0x059	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_48                                     	OBJECT_END + 0x05A	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_49                                     	OBJECT_END + 0x05B	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_50                                     	OBJECT_END + 0x05C	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_51                                     	OBJECT_END + 0x05D	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_52                                     	OBJECT_END + 0x05E	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_53                                     	OBJECT_END + 0x05F	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_54                                     	OBJECT_END + 0x060	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURA_55                                     	OBJECT_END + 0x061	//  Size: 56, Type: Int32, Flags: 1
#define UNIT_FIELD_AURAFLAGS                                   	OBJECT_END + 0x062	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAFLAGS_01                                	OBJECT_END + 0x063	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAFLAGS_02                                	OBJECT_END + 0x064	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAFLAGS_03                                	OBJECT_END + 0x065	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAFLAGS_04                                	OBJECT_END + 0x066	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAFLAGS_05                                	OBJECT_END + 0x067	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAFLAGS_06                                	OBJECT_END + 0x068	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAFLAGS_07                                	OBJECT_END + 0x069	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAFLAGS_08                                	OBJECT_END + 0x06A	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAFLAGS_09                                	OBJECT_END + 0x06B	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAFLAGS_10                                	OBJECT_END + 0x06C	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAFLAGS_11                                	OBJECT_END + 0x06D	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAFLAGS_12                                	OBJECT_END + 0x06E	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAFLAGS_13                                	OBJECT_END + 0x06F	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURALEVELS                                  	OBJECT_END + 0x070	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURALEVELS_01                               	OBJECT_END + 0x071	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURALEVELS_02                               	OBJECT_END + 0x072	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURALEVELS_03                               	OBJECT_END + 0x073	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURALEVELS_04                               	OBJECT_END + 0x074	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURALEVELS_05                               	OBJECT_END + 0x075	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURALEVELS_06                               	OBJECT_END + 0x076	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURALEVELS_07                               	OBJECT_END + 0x077	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURALEVELS_08                               	OBJECT_END + 0x078	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURALEVELS_09                               	OBJECT_END + 0x079	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURALEVELS_10                               	OBJECT_END + 0x07A	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURALEVELS_11                               	OBJECT_END + 0x07B	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURALEVELS_12                               	OBJECT_END + 0x07C	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURALEVELS_13                               	OBJECT_END + 0x07D	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAAPPLICATIONS                            	OBJECT_END + 0x07E	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAAPPLICATIONS_01                         	OBJECT_END + 0x07F	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAAPPLICATIONS_02                         	OBJECT_END + 0x080	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAAPPLICATIONS_03                         	OBJECT_END + 0x081	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAAPPLICATIONS_04                         	OBJECT_END + 0x082	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAAPPLICATIONS_05                         	OBJECT_END + 0x083	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAAPPLICATIONS_06                         	OBJECT_END + 0x084	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAAPPLICATIONS_07                         	OBJECT_END + 0x085	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAAPPLICATIONS_08                         	OBJECT_END + 0x086	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAAPPLICATIONS_09                         	OBJECT_END + 0x087	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAAPPLICATIONS_10                         	OBJECT_END + 0x088	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAAPPLICATIONS_11                         	OBJECT_END + 0x089	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAAPPLICATIONS_12                         	OBJECT_END + 0x08A	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURAAPPLICATIONS_13                         	OBJECT_END + 0x08B	//  Size: 14, Type: Bytes, Flags: 1
#define UNIT_FIELD_AURASTATE                                   	OBJECT_END + 0x08C	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_BASEATTACKTIME                              	OBJECT_END + 0x08D	//  Size: 2, Type: Int32, Flags: 1
#define UNIT_FIELD_BASEATTACKTIME_01                           	OBJECT_END + 0x08E	//  Size: 2, Type: Int32, Flags: 1
#define UNIT_FIELD_RANGEDATTACKTIME                            	OBJECT_END + 0x08F	//  Size: 1, Type: Int32, Flags: 2
#define UNIT_FIELD_BOUNDINGRADIUS                              	OBJECT_END + 0x090	//  Size: 1, Type: Float, Flags: 1
#define UNIT_FIELD_COMBATREACH                                 	OBJECT_END + 0x091	//  Size: 1, Type: Float, Flags: 1
#define UNIT_FIELD_DISPLAYID                                   	OBJECT_END + 0x092	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_NATIVEDISPLAYID                             	OBJECT_END + 0x093	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_MOUNTDISPLAYID                              	OBJECT_END + 0x094	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_MINDAMAGE                                   	OBJECT_END + 0x095	//  Size: 1, Type: Float, Flags: 38
#define UNIT_FIELD_MAXDAMAGE                                   	OBJECT_END + 0x096	//  Size: 1, Type: Float, Flags: 38
#define UNIT_FIELD_MINOFFHANDDAMAGE                            	OBJECT_END + 0x097	//  Size: 1, Type: Float, Flags: 38
#define UNIT_FIELD_MAXOFFHANDDAMAGE                            	OBJECT_END + 0x098	//  Size: 1, Type: Float, Flags: 38
#define UNIT_FIELD_BYTES_1                                     	OBJECT_END + 0x099	//  Size: 1, Type: Bytes, Flags: 1
#define UNIT_FIELD_PETNUMBER                                   	OBJECT_END + 0x09A	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_PET_NAME_TIMESTAMP                          	OBJECT_END + 0x09B	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_FIELD_PETEXPERIENCE                               	OBJECT_END + 0x09C	//  Size: 1, Type: Int32, Flags: 4
#define UNIT_FIELD_PETNEXTLEVELEXP                             	OBJECT_END + 0x09D	//  Size: 1, Type: Int32, Flags: 4
#define UNIT_DYNAMIC_FLAGS                                     	OBJECT_END + 0x09E	//  Size: 1, Type: Int32, Flags: 256
#define UNIT_CHANNEL_SPELL                                     	OBJECT_END + 0x09F	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_MOD_CAST_SPEED                                    	OBJECT_END + 0x0A0	//  Size: 1, Type: Float, Flags: 1
#define UNIT_CREATED_BY_SPELL                                  	OBJECT_END + 0x0A1	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_NPC_FLAGS                                         	OBJECT_END + 0x0A2	//  Size: 1, Type: Int32, Flags: 256
#define UNIT_NPC_EMOTESTATE                                    	OBJECT_END + 0x0A3	//  Size: 1, Type: Int32, Flags: 1
#define UNIT_TRAINING_POINTS                                   	OBJECT_END + 0x0A4	//  Size: 1, Type: Chars?, Flags: 4
#define UNIT_FIELD_STAT0                                       	OBJECT_END + 0x0A5	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_STAT1                                       	OBJECT_END + 0x0A6	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_STAT2                                       	OBJECT_END + 0x0A7	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_STAT3                                       	OBJECT_END + 0x0A8	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_STAT4                                       	OBJECT_END + 0x0A9	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_POSSTAT0                                    	OBJECT_END + 0x0AA	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_POSSTAT1                                    	OBJECT_END + 0x0AB	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_POSSTAT2                                    	OBJECT_END + 0x0AC	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_POSSTAT3                                    	OBJECT_END + 0x0AD	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_POSSTAT4                                    	OBJECT_END + 0x0AE	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_NEGSTAT0                                    	OBJECT_END + 0x0AF	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_NEGSTAT1                                    	OBJECT_END + 0x0B0	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_NEGSTAT2                                    	OBJECT_END + 0x0B1	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_NEGSTAT3                                    	OBJECT_END + 0x0B2	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_NEGSTAT4                                    	OBJECT_END + 0x0B3	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_RESISTANCES                                 	OBJECT_END + 0x0B4	//  Size: 7, Type: Int32, Flags: 38
#define UNIT_FIELD_RESISTANCES_01                              	OBJECT_END + 0x0B5	//  Size: 7, Type: Int32, Flags: 38
#define UNIT_FIELD_RESISTANCES_02                              	OBJECT_END + 0x0B6	//  Size: 7, Type: Int32, Flags: 38
#define UNIT_FIELD_RESISTANCES_03                              	OBJECT_END + 0x0B7	//  Size: 7, Type: Int32, Flags: 38
#define UNIT_FIELD_RESISTANCES_04                              	OBJECT_END + 0x0B8	//  Size: 7, Type: Int32, Flags: 38
#define UNIT_FIELD_RESISTANCES_05                              	OBJECT_END + 0x0B9	//  Size: 7, Type: Int32, Flags: 38
#define UNIT_FIELD_RESISTANCES_06                              	OBJECT_END + 0x0BA	//  Size: 7, Type: Int32, Flags: 38
#define UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE                  	OBJECT_END + 0x0BB	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE_01               	OBJECT_END + 0x0BC	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE_02               	OBJECT_END + 0x0BD	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE_03               	OBJECT_END + 0x0BE	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE_04               	OBJECT_END + 0x0BF	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE_05               	OBJECT_END + 0x0C0	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE_06               	OBJECT_END + 0x0C1	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE                  	OBJECT_END + 0x0C2	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE_01               	OBJECT_END + 0x0C3	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE_02               	OBJECT_END + 0x0C4	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE_03               	OBJECT_END + 0x0C5	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE_04               	OBJECT_END + 0x0C6	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE_05               	OBJECT_END + 0x0C7	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE_06               	OBJECT_END + 0x0C8	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_BASE_MANA                                   	OBJECT_END + 0x0C9	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_BASE_HEALTH                                 	OBJECT_END + 0x0CA	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_BYTES_2                                     	OBJECT_END + 0x0CB	//  Size: 1, Type: Bytes, Flags: 1
#define UNIT_FIELD_ATTACK_POWER                                	OBJECT_END + 0x0CC	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_ATTACK_POWER_MODS                           	OBJECT_END + 0x0CD	//  Size: 1, Type: Chars?, Flags: 6
#define UNIT_FIELD_ATTACK_POWER_MULTIPLIER                     	OBJECT_END + 0x0CE	//  Size: 1, Type: Float, Flags: 6
#define UNIT_FIELD_RANGED_ATTACK_POWER                         	OBJECT_END + 0x0CF	//  Size: 1, Type: Int32, Flags: 6
#define UNIT_FIELD_RANGED_ATTACK_POWER_MODS                    	OBJECT_END + 0x0D0	//  Size: 1, Type: Chars?, Flags: 6
#define UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER              	OBJECT_END + 0x0D1	//  Size: 1, Type: Float, Flags: 6
#define UNIT_FIELD_MINRANGEDDAMAGE                             	OBJECT_END + 0x0D2	//  Size: 1, Type: Float, Flags: 6
#define UNIT_FIELD_MAXRANGEDDAMAGE                             	OBJECT_END + 0x0D3	//  Size: 1, Type: Float, Flags: 6
#define UNIT_FIELD_POWER_COST_MODIFIER                         	OBJECT_END + 0x0D4	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_POWER_COST_MODIFIER_01                      	OBJECT_END + 0x0D5	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_POWER_COST_MODIFIER_02                      	OBJECT_END + 0x0D6	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_POWER_COST_MODIFIER_03                      	OBJECT_END + 0x0D7	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_POWER_COST_MODIFIER_04                      	OBJECT_END + 0x0D8	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_POWER_COST_MODIFIER_05                      	OBJECT_END + 0x0D9	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_POWER_COST_MODIFIER_06                      	OBJECT_END + 0x0DA	//  Size: 7, Type: Int32, Flags: 6
#define UNIT_FIELD_POWER_COST_MULTIPLIER                       	OBJECT_END + 0x0DB	//  Size: 7, Type: Float, Flags: 6
#define UNIT_FIELD_POWER_COST_MULTIPLIER_01                    	OBJECT_END + 0x0DC	//  Size: 7, Type: Float, Flags: 6
#define UNIT_FIELD_POWER_COST_MULTIPLIER_02                    	OBJECT_END + 0x0DD	//  Size: 7, Type: Float, Flags: 6
#define UNIT_FIELD_POWER_COST_MULTIPLIER_03                    	OBJECT_END + 0x0DE	//  Size: 7, Type: Float, Flags: 6
#define UNIT_FIELD_POWER_COST_MULTIPLIER_04                    	OBJECT_END + 0x0DF	//  Size: 7, Type: Float, Flags: 6
#define UNIT_FIELD_POWER_COST_MULTIPLIER_05                    	OBJECT_END + 0x0E0	//  Size: 7, Type: Float, Flags: 6
#define UNIT_FIELD_POWER_COST_MULTIPLIER_06                    	OBJECT_END + 0x0E1	//  Size: 7, Type: Float, Flags: 6
#define UNIT_FIELD_MAXHEALTHMODIFIER                           	OBJECT_END + 0x0E2	//  Size: 1, Type: Float, Flags: 6
#define UNIT_FIELD_PADDING                                     	OBJECT_END + 0x0E3	//  Size: 1, Type: Int32, Flags: 0
#define UNIT_END                                                OBJECT_END + 0x0E4

//PLAYER:
#define PLAYER_DUEL_ARBITER                                    	UNIT_END + 0x000	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_FLAGS                                           	UNIT_END + 0x002	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_GUILDID                                         	UNIT_END + 0x003	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_GUILDRANK                                       	UNIT_END + 0x004	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_BYTES                                           	UNIT_END + 0x005	//  Size: 1, Type: Bytes, Flags: 1
#define PLAYER_BYTES_2                                         	UNIT_END + 0x006	//  Size: 1, Type: Bytes, Flags: 1
#define PLAYER_BYTES_3                                         	UNIT_END + 0x007	//  Size: 1, Type: Bytes, Flags: 1
#define PLAYER_DUEL_TEAM                                       	UNIT_END + 0x008	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_GUILD_TIMESTAMP                                 	UNIT_END + 0x009	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_QUEST_LOG_1_1                                   	UNIT_END + 0x00A	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_1_2                                   	UNIT_END + 0x00B	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_1_3                                   	UNIT_END + 0x00C	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_1_4                                   	UNIT_END + 0x00D	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_2_1                                   	UNIT_END + 0x00E	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_2_2                                   	UNIT_END + 0x00F	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_2_3                                   	UNIT_END + 0x010	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_2_4                                   	UNIT_END + 0x011	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_3_1                                   	UNIT_END + 0x012	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_3_2                                   	UNIT_END + 0x013	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_3_3                                   	UNIT_END + 0x014	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_3_4                                   	UNIT_END + 0x015	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_4_1                                   	UNIT_END + 0x016	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_4_2                                   	UNIT_END + 0x017	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_4_3                                   	UNIT_END + 0x018	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_4_4                                   	UNIT_END + 0x019	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_5_1                                   	UNIT_END + 0x01A	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_5_2                                   	UNIT_END + 0x01B	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_5_3                                   	UNIT_END + 0x01C	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_5_4                                   	UNIT_END + 0x01D	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_6_1                                   	UNIT_END + 0x01E	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_6_2                                   	UNIT_END + 0x01F	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_6_3                                   	UNIT_END + 0x020	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_6_4                                   	UNIT_END + 0x021	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_7_1                                   	UNIT_END + 0x022	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_7_2                                   	UNIT_END + 0x023	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_7_3                                   	UNIT_END + 0x024	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_7_4                                   	UNIT_END + 0x025	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_8_1                                   	UNIT_END + 0x026	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_8_2                                   	UNIT_END + 0x027	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_8_3                                   	UNIT_END + 0x028	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_8_4                                   	UNIT_END + 0x029	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_9_1                                   	UNIT_END + 0x02A	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_9_2                                   	UNIT_END + 0x02B	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_9_3                                   	UNIT_END + 0x02C	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_9_4                                   	UNIT_END + 0x02D	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_10_1                                  	UNIT_END + 0x02E	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_10_2                                  	UNIT_END + 0x02F	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_10_3                                  	UNIT_END + 0x030	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_10_4                                  	UNIT_END + 0x031	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_11_1                                  	UNIT_END + 0x032	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_11_2                                  	UNIT_END + 0x033	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_11_3                                  	UNIT_END + 0x034	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_11_4                                  	UNIT_END + 0x035	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_12_1                                  	UNIT_END + 0x036	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_12_2                                  	UNIT_END + 0x037	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_12_3                                  	UNIT_END + 0x038	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_12_4                                  	UNIT_END + 0x039	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_13_1                                  	UNIT_END + 0x03A	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_13_2                                  	UNIT_END + 0x03B	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_13_3                                  	UNIT_END + 0x03C	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_13_4                                  	UNIT_END + 0x03D	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_14_1                                  	UNIT_END + 0x03E	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_14_2                                  	UNIT_END + 0x03F	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_14_3                                  	UNIT_END + 0x040	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_14_4                                  	UNIT_END + 0x041	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_15_1                                  	UNIT_END + 0x042	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_15_2                                  	UNIT_END + 0x043	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_15_3                                  	UNIT_END + 0x044	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_15_4                                  	UNIT_END + 0x045	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_16_1                                  	UNIT_END + 0x046	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_16_2                                  	UNIT_END + 0x047	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_16_3                                  	UNIT_END + 0x048	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_16_4                                  	UNIT_END + 0x049	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_17_1                                  	UNIT_END + 0x04A	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_17_2                                  	UNIT_END + 0x04B	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_17_3                                  	UNIT_END + 0x04C	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_17_4                                  	UNIT_END + 0x04D	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_18_1                                  	UNIT_END + 0x04E	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_18_2                                  	UNIT_END + 0x04F	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_18_3                                  	UNIT_END + 0x050	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_18_4                                  	UNIT_END + 0x051	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_19_1                                  	UNIT_END + 0x052	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_19_2                                  	UNIT_END + 0x053	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_19_3                                  	UNIT_END + 0x054	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_19_4                                  	UNIT_END + 0x055	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_20_1                                  	UNIT_END + 0x056	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_20_2                                  	UNIT_END + 0x057	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_20_3                                  	UNIT_END + 0x058	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_20_4                                  	UNIT_END + 0x059	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_21_1                                  	UNIT_END + 0x05A	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_21_2                                  	UNIT_END + 0x05B	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_21_3                                  	UNIT_END + 0x05C	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_21_4                                  	UNIT_END + 0x05D	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_22_1                                  	UNIT_END + 0x05E	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_22_2                                  	UNIT_END + 0x05F	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_22_3                                  	UNIT_END + 0x060	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_22_4                                  	UNIT_END + 0x061	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_23_1                                  	UNIT_END + 0x062	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_23_2                                  	UNIT_END + 0x063	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_23_3                                  	UNIT_END + 0x064	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_23_4                                  	UNIT_END + 0x065	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_24_1                                  	UNIT_END + 0x066	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_24_2                                  	UNIT_END + 0x067	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_24_3                                  	UNIT_END + 0x068	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_24_4                                  	UNIT_END + 0x069	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_25_1                                  	UNIT_END + 0x06A	//  Size: 1, Type: Int32, Flags: 64
#define PLAYER_QUEST_LOG_25_2                                  	UNIT_END + 0x06B	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_QUEST_LOG_25_3                                  	UNIT_END + 0x06C	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_QUEST_LOG_25_4                                  	UNIT_END + 0x06D	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_VISIBLE_ITEM_1_CREATOR                          	UNIT_END + 0x06E	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_1_0                                	UNIT_END + 0x070	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_1_01                               	UNIT_END + 0x071	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_1_02                               	UNIT_END + 0x072	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_1_03                               	UNIT_END + 0x073	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_1_04                               	UNIT_END + 0x074	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_1_05                               	UNIT_END + 0x075	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_1_06                               	UNIT_END + 0x076	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_1_07                               	UNIT_END + 0x077	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_1_08                               	UNIT_END + 0x078	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_1_09                               	UNIT_END + 0x079	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_1_10                               	UNIT_END + 0x07A	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_1_11                               	UNIT_END + 0x07B	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_1_PROPERTIES                       	UNIT_END + 0x07C	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_1_PAD                              	UNIT_END + 0x07D	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_2_CREATOR                          	UNIT_END + 0x07E	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_2_0                                	UNIT_END + 0x080	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_2_01                               	UNIT_END + 0x081	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_2_02                               	UNIT_END + 0x082	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_2_03                               	UNIT_END + 0x083	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_2_04                               	UNIT_END + 0x084	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_2_05                               	UNIT_END + 0x085	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_2_06                               	UNIT_END + 0x086	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_2_07                               	UNIT_END + 0x087	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_2_08                               	UNIT_END + 0x088	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_2_09                               	UNIT_END + 0x089	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_2_10                               	UNIT_END + 0x08A	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_2_11                               	UNIT_END + 0x08B	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_2_PROPERTIES                       	UNIT_END + 0x08C	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_2_PAD                              	UNIT_END + 0x08D	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_3_CREATOR                          	UNIT_END + 0x08E	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_3_0                                	UNIT_END + 0x090	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_3_01                               	UNIT_END + 0x091	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_3_02                               	UNIT_END + 0x092	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_3_03                               	UNIT_END + 0x093	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_3_04                               	UNIT_END + 0x094	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_3_05                               	UNIT_END + 0x095	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_3_06                               	UNIT_END + 0x096	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_3_07                               	UNIT_END + 0x097	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_3_08                               	UNIT_END + 0x098	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_3_09                               	UNIT_END + 0x099	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_3_10                               	UNIT_END + 0x09A	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_3_11                               	UNIT_END + 0x09B	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_3_PROPERTIES                       	UNIT_END + 0x09C	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_3_PAD                              	UNIT_END + 0x09D	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_4_CREATOR                          	UNIT_END + 0x09E	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_4_0                                	UNIT_END + 0x0A0	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_4_01                               	UNIT_END + 0x0A1	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_4_02                               	UNIT_END + 0x0A2	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_4_03                               	UNIT_END + 0x0A3	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_4_04                               	UNIT_END + 0x0A4	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_4_05                               	UNIT_END + 0x0A5	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_4_06                               	UNIT_END + 0x0A6	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_4_07                               	UNIT_END + 0x0A7	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_4_08                               	UNIT_END + 0x0A8	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_4_09                               	UNIT_END + 0x0A9	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_4_10                               	UNIT_END + 0x0AA	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_4_11                               	UNIT_END + 0x0AB	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_4_PROPERTIES                       	UNIT_END + 0x0AC	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_4_PAD                              	UNIT_END + 0x0AD	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_5_CREATOR                          	UNIT_END + 0x0AE	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_5_0                                	UNIT_END + 0x0B0	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_5_01                               	UNIT_END + 0x0B1	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_5_02                               	UNIT_END + 0x0B2	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_5_03                               	UNIT_END + 0x0B3	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_5_04                               	UNIT_END + 0x0B4	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_5_05                               	UNIT_END + 0x0B5	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_5_06                               	UNIT_END + 0x0B6	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_5_07                               	UNIT_END + 0x0B7	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_5_08                               	UNIT_END + 0x0B8	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_5_09                               	UNIT_END + 0x0B9	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_5_10                               	UNIT_END + 0x0BA	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_5_11                               	UNIT_END + 0x0BB	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_5_PROPERTIES                       	UNIT_END + 0x0BC	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_5_PAD                              	UNIT_END + 0x0BD	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_6_CREATOR                          	UNIT_END + 0x0BE	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_6_0                                	UNIT_END + 0x0C0	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_6_01                               	UNIT_END + 0x0C1	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_6_02                               	UNIT_END + 0x0C2	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_6_03                               	UNIT_END + 0x0C3	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_6_04                               	UNIT_END + 0x0C4	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_6_05                               	UNIT_END + 0x0C5	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_6_06                               	UNIT_END + 0x0C6	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_6_07                               	UNIT_END + 0x0C7	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_6_08                               	UNIT_END + 0x0C8	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_6_09                               	UNIT_END + 0x0C9	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_6_10                               	UNIT_END + 0x0CA	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_6_11                               	UNIT_END + 0x0CB	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_6_PROPERTIES                       	UNIT_END + 0x0CC	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_6_PAD                              	UNIT_END + 0x0CD	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_7_CREATOR                          	UNIT_END + 0x0CE	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_7_0                                	UNIT_END + 0x0D0	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_7_01                               	UNIT_END + 0x0D1	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_7_02                               	UNIT_END + 0x0D2	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_7_03                               	UNIT_END + 0x0D3	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_7_04                               	UNIT_END + 0x0D4	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_7_05                               	UNIT_END + 0x0D5	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_7_06                               	UNIT_END + 0x0D6	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_7_07                               	UNIT_END + 0x0D7	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_7_08                               	UNIT_END + 0x0D8	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_7_09                               	UNIT_END + 0x0D9	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_7_10                               	UNIT_END + 0x0DA	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_7_11                               	UNIT_END + 0x0DB	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_7_PROPERTIES                       	UNIT_END + 0x0DC	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_7_PAD                              	UNIT_END + 0x0DD	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_8_CREATOR                          	UNIT_END + 0x0DE	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_8_0                                	UNIT_END + 0x0E0	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_8_01                               	UNIT_END + 0x0E1	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_8_02                               	UNIT_END + 0x0E2	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_8_03                               	UNIT_END + 0x0E3	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_8_04                               	UNIT_END + 0x0E4	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_8_05                               	UNIT_END + 0x0E5	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_8_06                               	UNIT_END + 0x0E6	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_8_07                               	UNIT_END + 0x0E7	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_8_08                               	UNIT_END + 0x0E8	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_8_09                               	UNIT_END + 0x0E9	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_8_10                               	UNIT_END + 0x0EA	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_8_11                               	UNIT_END + 0x0EB	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_8_PROPERTIES                       	UNIT_END + 0x0EC	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_8_PAD                              	UNIT_END + 0x0ED	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_9_CREATOR                          	UNIT_END + 0x0EE	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_9_0                                	UNIT_END + 0x0F0	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_9_01                               	UNIT_END + 0x0F1	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_9_02                               	UNIT_END + 0x0F2	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_9_03                               	UNIT_END + 0x0F3	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_9_04                               	UNIT_END + 0x0F4	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_9_05                               	UNIT_END + 0x0F5	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_9_06                               	UNIT_END + 0x0F6	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_9_07                               	UNIT_END + 0x0F7	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_9_08                               	UNIT_END + 0x0F8	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_9_09                               	UNIT_END + 0x0F9	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_9_10                               	UNIT_END + 0x0FA	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_9_11                               	UNIT_END + 0x0FB	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_9_PROPERTIES                       	UNIT_END + 0x0FC	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_9_PAD                              	UNIT_END + 0x0FD	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_10_CREATOR                         	UNIT_END + 0x0FE	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_10_0                               	UNIT_END + 0x100	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_10_01                              	UNIT_END + 0x101	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_10_02                              	UNIT_END + 0x102	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_10_03                              	UNIT_END + 0x103	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_10_04                              	UNIT_END + 0x104	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_10_05                              	UNIT_END + 0x105	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_10_06                              	UNIT_END + 0x106	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_10_07                              	UNIT_END + 0x107	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_10_08                              	UNIT_END + 0x108	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_10_09                              	UNIT_END + 0x109	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_10_10                              	UNIT_END + 0x10A	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_10_11                              	UNIT_END + 0x10B	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_10_PROPERTIES                      	UNIT_END + 0x10C	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_10_PAD                             	UNIT_END + 0x10D	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_11_CREATOR                         	UNIT_END + 0x10E	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_11_0                               	UNIT_END + 0x110	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_11_01                              	UNIT_END + 0x111	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_11_02                              	UNIT_END + 0x112	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_11_03                              	UNIT_END + 0x113	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_11_04                              	UNIT_END + 0x114	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_11_05                              	UNIT_END + 0x115	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_11_06                              	UNIT_END + 0x116	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_11_07                              	UNIT_END + 0x117	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_11_08                              	UNIT_END + 0x118	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_11_09                              	UNIT_END + 0x119	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_11_10                              	UNIT_END + 0x11A	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_11_11                              	UNIT_END + 0x11B	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_11_PROPERTIES                      	UNIT_END + 0x11C	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_11_PAD                             	UNIT_END + 0x11D	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_12_CREATOR                         	UNIT_END + 0x11E	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_12_0                               	UNIT_END + 0x120	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_12_01                              	UNIT_END + 0x121	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_12_02                              	UNIT_END + 0x122	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_12_03                              	UNIT_END + 0x123	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_12_04                              	UNIT_END + 0x124	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_12_05                              	UNIT_END + 0x125	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_12_06                              	UNIT_END + 0x126	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_12_07                              	UNIT_END + 0x127	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_12_08                              	UNIT_END + 0x128	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_12_09                              	UNIT_END + 0x129	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_12_10                              	UNIT_END + 0x12A	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_12_11                              	UNIT_END + 0x12B	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_12_PROPERTIES                      	UNIT_END + 0x12C	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_12_PAD                             	UNIT_END + 0x12D	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_13_CREATOR                         	UNIT_END + 0x12E	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_13_0                               	UNIT_END + 0x130	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_13_01                              	UNIT_END + 0x131	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_13_02                              	UNIT_END + 0x132	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_13_03                              	UNIT_END + 0x133	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_13_04                              	UNIT_END + 0x134	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_13_05                              	UNIT_END + 0x135	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_13_06                              	UNIT_END + 0x136	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_13_07                              	UNIT_END + 0x137	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_13_08                              	UNIT_END + 0x138	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_13_09                              	UNIT_END + 0x139	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_13_10                              	UNIT_END + 0x13A	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_13_11                              	UNIT_END + 0x13B	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_13_PROPERTIES                      	UNIT_END + 0x13C	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_13_PAD                             	UNIT_END + 0x13D	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_14_CREATOR                         	UNIT_END + 0x13E	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_14_0                               	UNIT_END + 0x140	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_14_01                              	UNIT_END + 0x141	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_14_02                              	UNIT_END + 0x142	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_14_03                              	UNIT_END + 0x143	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_14_04                              	UNIT_END + 0x144	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_14_05                              	UNIT_END + 0x145	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_14_06                              	UNIT_END + 0x146	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_14_07                              	UNIT_END + 0x147	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_14_08                              	UNIT_END + 0x148	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_14_09                              	UNIT_END + 0x149	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_14_10                              	UNIT_END + 0x14A	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_14_11                              	UNIT_END + 0x14B	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_14_PROPERTIES                      	UNIT_END + 0x14C	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_14_PAD                             	UNIT_END + 0x14D	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_15_CREATOR                         	UNIT_END + 0x14E	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_15_0                               	UNIT_END + 0x150	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_15_01                              	UNIT_END + 0x151	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_15_02                              	UNIT_END + 0x152	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_15_03                              	UNIT_END + 0x153	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_15_04                              	UNIT_END + 0x154	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_15_05                              	UNIT_END + 0x155	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_15_06                              	UNIT_END + 0x156	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_15_07                              	UNIT_END + 0x157	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_15_08                              	UNIT_END + 0x158	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_15_09                              	UNIT_END + 0x159	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_15_10                              	UNIT_END + 0x15A	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_15_11                              	UNIT_END + 0x15B	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_15_PROPERTIES                      	UNIT_END + 0x15C	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_15_PAD                             	UNIT_END + 0x15D	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_16_CREATOR                         	UNIT_END + 0x15E	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_16_0                               	UNIT_END + 0x160	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_16_01                              	UNIT_END + 0x161	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_16_02                              	UNIT_END + 0x162	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_16_03                              	UNIT_END + 0x163	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_16_04                              	UNIT_END + 0x164	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_16_05                              	UNIT_END + 0x165	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_16_06                              	UNIT_END + 0x166	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_16_07                              	UNIT_END + 0x167	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_16_08                              	UNIT_END + 0x168	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_16_09                              	UNIT_END + 0x169	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_16_10                              	UNIT_END + 0x16A	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_16_11                              	UNIT_END + 0x16B	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_16_PROPERTIES                      	UNIT_END + 0x16C	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_16_PAD                             	UNIT_END + 0x16D	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_17_CREATOR                         	UNIT_END + 0x16E	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_17_0                               	UNIT_END + 0x170	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_17_01                              	UNIT_END + 0x171	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_17_02                              	UNIT_END + 0x172	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_17_03                              	UNIT_END + 0x173	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_17_04                              	UNIT_END + 0x174	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_17_05                              	UNIT_END + 0x175	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_17_06                              	UNIT_END + 0x176	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_17_07                              	UNIT_END + 0x177	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_17_08                              	UNIT_END + 0x178	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_17_09                              	UNIT_END + 0x179	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_17_10                              	UNIT_END + 0x17A	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_17_11                              	UNIT_END + 0x17B	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_17_PROPERTIES                      	UNIT_END + 0x17C	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_17_PAD                             	UNIT_END + 0x17D	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_18_CREATOR                         	UNIT_END + 0x17E	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_18_0                               	UNIT_END + 0x180	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_18_01                              	UNIT_END + 0x181	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_18_02                              	UNIT_END + 0x182	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_18_03                              	UNIT_END + 0x183	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_18_04                              	UNIT_END + 0x184	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_18_05                              	UNIT_END + 0x185	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_18_06                              	UNIT_END + 0x186	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_18_07                              	UNIT_END + 0x187	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_18_08                              	UNIT_END + 0x188	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_18_09                              	UNIT_END + 0x189	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_18_10                              	UNIT_END + 0x18A	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_18_11                              	UNIT_END + 0x18B	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_18_PROPERTIES                      	UNIT_END + 0x18C	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_18_PAD                             	UNIT_END + 0x18D	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_19_CREATOR                         	UNIT_END + 0x18E	//  Size: 2, Type: GUID, Flags: 1
#define PLAYER_VISIBLE_ITEM_19_0                               	UNIT_END + 0x190	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_19_01                              	UNIT_END + 0x191	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_19_02                              	UNIT_END + 0x192	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_19_03                              	UNIT_END + 0x193	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_19_04                              	UNIT_END + 0x194	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_19_05                              	UNIT_END + 0x195	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_19_06                              	UNIT_END + 0x196	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_19_07                              	UNIT_END + 0x197	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_19_08                              	UNIT_END + 0x198	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_19_09                              	UNIT_END + 0x199	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_19_10                              	UNIT_END + 0x19A	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_19_11                              	UNIT_END + 0x19B	//  Size: 12, Type: Int32, Flags: 1
#define PLAYER_VISIBLE_ITEM_19_PROPERTIES                      	UNIT_END + 0x19C	//  Size: 1, Type: Chars?, Flags: 1
#define PLAYER_VISIBLE_ITEM_19_PAD                             	UNIT_END + 0x19D	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_CHOSEN_TITLE                                    	UNIT_END + 0x19E	//  Size: 1, Type: Int32, Flags: 1
#define PLAYER_FIELD_PAD_0                                     	UNIT_END + 0x19F	//  Size: 1, Type: Int32, Flags: 0
#define PLAYER_FIELD_INV_SLOT_HEAD                             	UNIT_END + 0x1A0	//  Size: 46, Type: GUID, Flags: 2
#define PLAYER_FIELD_PACK_SLOT_1                               	UNIT_END + 0x1CE	//  Size: 32, Type: GUID, Flags: 2
#define PLAYER_FIELD_BANK_SLOT_1                               	UNIT_END + 0x1EE	//  Size: 56, Type: GUID, Flags: 2
#define PLAYER_FIELD_BANKBAG_SLOT_1                            	UNIT_END + 0x226	//  Size: 14, Type: GUID, Flags: 2
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_1                      	UNIT_END + 0x234	//  Size: 24, Type: GUID, Flags: 2
#define PLAYER_FIELD_KEYRING_SLOT_1                            	UNIT_END + 0x24C	//  Size: 64, Type: GUID, Flags: 2
#define PLAYER_FIELD_VANITYPET_SLOT_1                          	UNIT_END + 0x28C	//  Size: 36, Type: GUID, Flags: 2
#define PLAYER_FARSIGHT                                        	UNIT_END + 0x2B0	//  Size: 2, Type: GUID, Flags: 2
#define PLAYER_FIELD_KNOWN_TITLES                             	UNIT_END + 0x2B2	//  Size: 2, Type: GUID, Flags: 2
#define PLAYER_XP                                              	UNIT_END + 0x2B4	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_NEXT_LEVEL_XP                                   	UNIT_END + 0x2B5	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_SKILL_INFO_1_1                                  	UNIT_END + 0x2B6	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_01                                 	UNIT_END + 0x2B7	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_02                                 	UNIT_END + 0x2B8	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_03                                 	UNIT_END + 0x2B9	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_04                                 	UNIT_END + 0x2BA	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_05                                 	UNIT_END + 0x2BB	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_06                                 	UNIT_END + 0x2BC	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_07                                 	UNIT_END + 0x2BD	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_08                                 	UNIT_END + 0x2BE	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_09                                 	UNIT_END + 0x2BF	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_10                                 	UNIT_END + 0x2C0	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_11                                 	UNIT_END + 0x2C1	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_12                                 	UNIT_END + 0x2C2	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_13                                 	UNIT_END + 0x2C3	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_14                                 	UNIT_END + 0x2C4	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_15                                 	UNIT_END + 0x2C5	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_16                                 	UNIT_END + 0x2C6	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_17                                 	UNIT_END + 0x2C7	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_18                                 	UNIT_END + 0x2C8	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_19                                 	UNIT_END + 0x2C9	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_20                                 	UNIT_END + 0x2CA	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_21                                 	UNIT_END + 0x2CB	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_22                                 	UNIT_END + 0x2CC	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_23                                 	UNIT_END + 0x2CD	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_24                                 	UNIT_END + 0x2CE	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_25                                 	UNIT_END + 0x2CF	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_26                                 	UNIT_END + 0x2D0	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_27                                 	UNIT_END + 0x2D1	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_28                                 	UNIT_END + 0x2D2	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_29                                 	UNIT_END + 0x2D3	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_30                                 	UNIT_END + 0x2D4	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_31                                 	UNIT_END + 0x2D5	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_32                                 	UNIT_END + 0x2D6	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_33                                 	UNIT_END + 0x2D7	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_34                                 	UNIT_END + 0x2D8	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_35                                 	UNIT_END + 0x2D9	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_36                                 	UNIT_END + 0x2DA	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_37                                 	UNIT_END + 0x2DB	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_38                                 	UNIT_END + 0x2DC	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_39                                 	UNIT_END + 0x2DD	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_40                                 	UNIT_END + 0x2DE	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_41                                 	UNIT_END + 0x2DF	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_42                                 	UNIT_END + 0x2E0	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_43                                 	UNIT_END + 0x2E1	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_44                                 	UNIT_END + 0x2E2	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_45                                 	UNIT_END + 0x2E3	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_46                                 	UNIT_END + 0x2E4	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_47                                 	UNIT_END + 0x2E5	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_48                                 	UNIT_END + 0x2E6	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_49                                 	UNIT_END + 0x2E7	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_50                                 	UNIT_END + 0x2E8	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_51                                 	UNIT_END + 0x2E9	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_52                                 	UNIT_END + 0x2EA	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_53                                 	UNIT_END + 0x2EB	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_54                                 	UNIT_END + 0x2EC	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_55                                 	UNIT_END + 0x2ED	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_56                                 	UNIT_END + 0x2EE	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_57                                 	UNIT_END + 0x2EF	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_58                                 	UNIT_END + 0x2F0	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_59                                 	UNIT_END + 0x2F1	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_60                                 	UNIT_END + 0x2F2	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_61                                 	UNIT_END + 0x2F3	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_62                                 	UNIT_END + 0x2F4	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_63                                 	UNIT_END + 0x2F5	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_64                                 	UNIT_END + 0x2F6	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_65                                 	UNIT_END + 0x2F7	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_66                                 	UNIT_END + 0x2F8	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_67                                 	UNIT_END + 0x2F9	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_68                                 	UNIT_END + 0x2FA	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_69                                 	UNIT_END + 0x2FB	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_70                                 	UNIT_END + 0x2FC	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_71                                 	UNIT_END + 0x2FD	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_72                                 	UNIT_END + 0x2FE	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_73                                 	UNIT_END + 0x2FF	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_74                                 	UNIT_END + 0x300	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_75                                 	UNIT_END + 0x301	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_76                                 	UNIT_END + 0x302	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_77                                 	UNIT_END + 0x303	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_78                                 	UNIT_END + 0x304	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_79                                 	UNIT_END + 0x305	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_80                                 	UNIT_END + 0x306	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_81                                 	UNIT_END + 0x307	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_82                                 	UNIT_END + 0x308	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_83                                 	UNIT_END + 0x309	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_84                                 	UNIT_END + 0x30A	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_85                                 	UNIT_END + 0x30B	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_86                                 	UNIT_END + 0x30C	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_87                                 	UNIT_END + 0x30D	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_88                                 	UNIT_END + 0x30E	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_89                                 	UNIT_END + 0x30F	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_90                                 	UNIT_END + 0x310	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_91                                 	UNIT_END + 0x311	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_92                                 	UNIT_END + 0x312	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_93                                 	UNIT_END + 0x313	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_94                                 	UNIT_END + 0x314	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_95                                 	UNIT_END + 0x315	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_96                                 	UNIT_END + 0x316	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_97                                 	UNIT_END + 0x317	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_98                                 	UNIT_END + 0x318	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_99                                 	UNIT_END + 0x319	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_100                                	UNIT_END + 0x31A	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_101                                	UNIT_END + 0x31B	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_102                                	UNIT_END + 0x31C	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_103                                	UNIT_END + 0x31D	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_104                                	UNIT_END + 0x31E	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_105                                	UNIT_END + 0x31F	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_106                                	UNIT_END + 0x320	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_107                                	UNIT_END + 0x321	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_108                                	UNIT_END + 0x322	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_109                                	UNIT_END + 0x323	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_110                                	UNIT_END + 0x324	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_111                                	UNIT_END + 0x325	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_112                                	UNIT_END + 0x326	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_113                                	UNIT_END + 0x327	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_114                                	UNIT_END + 0x328	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_115                                	UNIT_END + 0x329	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_116                                	UNIT_END + 0x32A	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_117                                	UNIT_END + 0x32B	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_118                                	UNIT_END + 0x32C	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_119                                	UNIT_END + 0x32D	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_120                                	UNIT_END + 0x32E	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_121                                	UNIT_END + 0x32F	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_122                                	UNIT_END + 0x330	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_123                                	UNIT_END + 0x331	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_124                                	UNIT_END + 0x332	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_125                                	UNIT_END + 0x333	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_126                                	UNIT_END + 0x334	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_127                                	UNIT_END + 0x335	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_128                                	UNIT_END + 0x336	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_129                                	UNIT_END + 0x337	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_130                                	UNIT_END + 0x338	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_131                                	UNIT_END + 0x339	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_132                                	UNIT_END + 0x33A	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_133                                	UNIT_END + 0x33B	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_134                                	UNIT_END + 0x33C	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_135                                	UNIT_END + 0x33D	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_136                                	UNIT_END + 0x33E	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_137                                	UNIT_END + 0x33F	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_138                                	UNIT_END + 0x340	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_139                                	UNIT_END + 0x341	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_140                                	UNIT_END + 0x342	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_141                                	UNIT_END + 0x343	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_142                                	UNIT_END + 0x344	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_143                                	UNIT_END + 0x345	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_144                                	UNIT_END + 0x346	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_145                                	UNIT_END + 0x347	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_146                                	UNIT_END + 0x348	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_147                                	UNIT_END + 0x349	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_148                                	UNIT_END + 0x34A	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_149                                	UNIT_END + 0x34B	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_150                                	UNIT_END + 0x34C	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_151                                	UNIT_END + 0x34D	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_152                                	UNIT_END + 0x34E	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_153                                	UNIT_END + 0x34F	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_154                                	UNIT_END + 0x350	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_155                                	UNIT_END + 0x351	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_156                                	UNIT_END + 0x352	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_157                                	UNIT_END + 0x353	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_158                                	UNIT_END + 0x354	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_159                                	UNIT_END + 0x355	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_160                                	UNIT_END + 0x356	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_161                                	UNIT_END + 0x357	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_162                                	UNIT_END + 0x358	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_163                                	UNIT_END + 0x359	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_164                                	UNIT_END + 0x35A	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_165                                	UNIT_END + 0x35B	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_166                                	UNIT_END + 0x35C	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_167                                	UNIT_END + 0x35D	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_168                                	UNIT_END + 0x35E	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_169                                	UNIT_END + 0x35F	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_170                                	UNIT_END + 0x360	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_171                                	UNIT_END + 0x361	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_172                                	UNIT_END + 0x362	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_173                                	UNIT_END + 0x363	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_174                                	UNIT_END + 0x364	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_175                                	UNIT_END + 0x365	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_176                                	UNIT_END + 0x366	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_177                                	UNIT_END + 0x367	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_178                                	UNIT_END + 0x368	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_179                                	UNIT_END + 0x369	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_180                                	UNIT_END + 0x36A	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_181                                	UNIT_END + 0x36B	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_182                                	UNIT_END + 0x36C	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_183                                	UNIT_END + 0x36D	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_184                                	UNIT_END + 0x36E	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_185                                	UNIT_END + 0x36F	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_186                                	UNIT_END + 0x370	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_187                                	UNIT_END + 0x371	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_188                                	UNIT_END + 0x372	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_189                                	UNIT_END + 0x373	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_190                                	UNIT_END + 0x374	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_191                                	UNIT_END + 0x375	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_192                                	UNIT_END + 0x376	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_193                                	UNIT_END + 0x377	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_194                                	UNIT_END + 0x378	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_195                                	UNIT_END + 0x379	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_196                                	UNIT_END + 0x37A	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_197                                	UNIT_END + 0x37B	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_198                                	UNIT_END + 0x37C	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_199                                	UNIT_END + 0x37D	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_200                                	UNIT_END + 0x37E	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_201                                	UNIT_END + 0x37F	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_202                                	UNIT_END + 0x380	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_203                                	UNIT_END + 0x381	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_204                                	UNIT_END + 0x382	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_205                                	UNIT_END + 0x383	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_206                                	UNIT_END + 0x384	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_207                                	UNIT_END + 0x385	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_208                                	UNIT_END + 0x386	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_209                                	UNIT_END + 0x387	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_210                                	UNIT_END + 0x388	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_211                                	UNIT_END + 0x389	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_212                                	UNIT_END + 0x38A	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_213                                	UNIT_END + 0x38B	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_214                                	UNIT_END + 0x38C	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_215                                	UNIT_END + 0x38D	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_216                                	UNIT_END + 0x38E	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_217                                	UNIT_END + 0x38F	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_218                                	UNIT_END + 0x390	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_219                                	UNIT_END + 0x391	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_220                                	UNIT_END + 0x392	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_221                                	UNIT_END + 0x393	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_222                                	UNIT_END + 0x394	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_223                                	UNIT_END + 0x395	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_224                                	UNIT_END + 0x396	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_225                                	UNIT_END + 0x397	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_226                                	UNIT_END + 0x398	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_227                                	UNIT_END + 0x399	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_228                                	UNIT_END + 0x39A	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_229                                	UNIT_END + 0x39B	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_230                                	UNIT_END + 0x39C	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_231                                	UNIT_END + 0x39D	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_232                                	UNIT_END + 0x39E	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_233                                	UNIT_END + 0x39F	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_234                                	UNIT_END + 0x3A0	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_235                                	UNIT_END + 0x3A1	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_236                                	UNIT_END + 0x3A2	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_237                                	UNIT_END + 0x3A3	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_238                                	UNIT_END + 0x3A4	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_239                                	UNIT_END + 0x3A5	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_240                                	UNIT_END + 0x3A6	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_241                                	UNIT_END + 0x3A7	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_242                                	UNIT_END + 0x3A8	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_243                                	UNIT_END + 0x3A9	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_244                                	UNIT_END + 0x3AA	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_245                                	UNIT_END + 0x3AB	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_246                                	UNIT_END + 0x3AC	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_247                                	UNIT_END + 0x3AD	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_248                                	UNIT_END + 0x3AE	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_249                                	UNIT_END + 0x3AF	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_250                                	UNIT_END + 0x3B0	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_251                                	UNIT_END + 0x3B1	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_252                                	UNIT_END + 0x3B2	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_253                                	UNIT_END + 0x3B3	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_254                                	UNIT_END + 0x3B4	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_255                                	UNIT_END + 0x3B5	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_256                                	UNIT_END + 0x3B6	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_257                                	UNIT_END + 0x3B7	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_258                                	UNIT_END + 0x3B8	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_259                                	UNIT_END + 0x3B9	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_260                                	UNIT_END + 0x3BA	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_261                                	UNIT_END + 0x3BB	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_262                                	UNIT_END + 0x3BC	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_263                                	UNIT_END + 0x3BD	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_264                                	UNIT_END + 0x3BE	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_265                                	UNIT_END + 0x3BF	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_266                                	UNIT_END + 0x3C0	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_267                                	UNIT_END + 0x3C1	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_268                                	UNIT_END + 0x3C2	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_269                                	UNIT_END + 0x3C3	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_270                                	UNIT_END + 0x3C4	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_271                                	UNIT_END + 0x3C5	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_272                                	UNIT_END + 0x3C6	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_273                                	UNIT_END + 0x3C7	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_274                                	UNIT_END + 0x3C8	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_275                                	UNIT_END + 0x3C9	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_276                                	UNIT_END + 0x3CA	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_277                                	UNIT_END + 0x3CB	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_278                                	UNIT_END + 0x3CC	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_279                                	UNIT_END + 0x3CD	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_280                                	UNIT_END + 0x3CE	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_281                                	UNIT_END + 0x3CF	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_282                                	UNIT_END + 0x3D0	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_283                                	UNIT_END + 0x3D1	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_284                                	UNIT_END + 0x3D2	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_285                                	UNIT_END + 0x3D3	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_286                                	UNIT_END + 0x3D4	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_287                                	UNIT_END + 0x3D5	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_288                                	UNIT_END + 0x3D6	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_289                                	UNIT_END + 0x3D7	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_290                                	UNIT_END + 0x3D8	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_291                                	UNIT_END + 0x3D9	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_292                                	UNIT_END + 0x3DA	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_293                                	UNIT_END + 0x3DB	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_294                                	UNIT_END + 0x3DC	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_295                                	UNIT_END + 0x3DD	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_296                                	UNIT_END + 0x3DE	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_297                                	UNIT_END + 0x3DF	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_298                                	UNIT_END + 0x3E0	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_299                                	UNIT_END + 0x3E1	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_300                                	UNIT_END + 0x3E2	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_301                                	UNIT_END + 0x3E3	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_302                                	UNIT_END + 0x3E4	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_303                                	UNIT_END + 0x3E5	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_304                                	UNIT_END + 0x3E6	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_305                                	UNIT_END + 0x3E7	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_306                                	UNIT_END + 0x3E8	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_307                                	UNIT_END + 0x3E9	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_308                                	UNIT_END + 0x3EA	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_309                                	UNIT_END + 0x3EB	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_310                                	UNIT_END + 0x3EC	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_311                                	UNIT_END + 0x3ED	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_312                                	UNIT_END + 0x3EE	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_313                                	UNIT_END + 0x3EF	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_314                                	UNIT_END + 0x3F0	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_315                                	UNIT_END + 0x3F1	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_316                                	UNIT_END + 0x3F2	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_317                                	UNIT_END + 0x3F3	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_318                                	UNIT_END + 0x3F4	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_319                                	UNIT_END + 0x3F5	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_320                                	UNIT_END + 0x3F6	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_321                                	UNIT_END + 0x3F7	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_322                                	UNIT_END + 0x3F8	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_323                                	UNIT_END + 0x3F9	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_324                                	UNIT_END + 0x3FA	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_325                                	UNIT_END + 0x3FB	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_326                                	UNIT_END + 0x3FC	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_327                                	UNIT_END + 0x3FD	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_328                                	UNIT_END + 0x3FE	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_329                                	UNIT_END + 0x3FF	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_330                                	UNIT_END + 0x400	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_331                                	UNIT_END + 0x401	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_332                                	UNIT_END + 0x402	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_333                                	UNIT_END + 0x403	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_334                                	UNIT_END + 0x404	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_335                                	UNIT_END + 0x405	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_336                                	UNIT_END + 0x406	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_337                                	UNIT_END + 0x407	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_338                                	UNIT_END + 0x408	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_339                                	UNIT_END + 0x409	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_340                                	UNIT_END + 0x40A	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_341                                	UNIT_END + 0x40B	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_342                                	UNIT_END + 0x40C	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_343                                	UNIT_END + 0x40D	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_344                                	UNIT_END + 0x40E	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_345                                	UNIT_END + 0x40F	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_346                                	UNIT_END + 0x410	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_347                                	UNIT_END + 0x411	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_348                                	UNIT_END + 0x412	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_349                                	UNIT_END + 0x413	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_350                                	UNIT_END + 0x414	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_351                                	UNIT_END + 0x415	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_352                                	UNIT_END + 0x416	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_353                                	UNIT_END + 0x417	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_354                                	UNIT_END + 0x418	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_355                                	UNIT_END + 0x419	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_356                                	UNIT_END + 0x41A	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_357                                	UNIT_END + 0x41B	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_358                                	UNIT_END + 0x41C	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_359                                	UNIT_END + 0x41D	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_360                                	UNIT_END + 0x41E	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_361                                	UNIT_END + 0x41F	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_362                                	UNIT_END + 0x420	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_363                                	UNIT_END + 0x421	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_364                                	UNIT_END + 0x422	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_365                                	UNIT_END + 0x423	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_366                                	UNIT_END + 0x424	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_367                                	UNIT_END + 0x425	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_368                                	UNIT_END + 0x426	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_369                                	UNIT_END + 0x427	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_370                                	UNIT_END + 0x428	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_371                                	UNIT_END + 0x429	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_372                                	UNIT_END + 0x42A	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_373                                	UNIT_END + 0x42B	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_374                                	UNIT_END + 0x42C	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_375                                	UNIT_END + 0x42D	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_376                                	UNIT_END + 0x42E	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_377                                	UNIT_END + 0x42F	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_378                                	UNIT_END + 0x430	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_379                                	UNIT_END + 0x431	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_380                                	UNIT_END + 0x432	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_381                                	UNIT_END + 0x433	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_382                                	UNIT_END + 0x434	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_SKILL_INFO_1_383                                	UNIT_END + 0x435	//  Size: 384, Type: Chars?, Flags: 2
#define PLAYER_CHARACTER_POINTS1                               	UNIT_END + 0x436	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_CHARACTER_POINTS2                               	UNIT_END + 0x437	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_TRACK_CREATURES                                 	UNIT_END + 0x438	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_TRACK_RESOURCES                                 	UNIT_END + 0x439	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_BLOCK_PERCENTAGE                                	UNIT_END + 0x43A	//  Size: 1, Type: Float, Flags: 2
#define PLAYER_DODGE_PERCENTAGE                                	UNIT_END + 0x43B	//  Size: 1, Type: Float, Flags: 2
#define PLAYER_PARRY_PERCENTAGE                                	UNIT_END + 0x43C	//  Size: 1, Type: Float, Flags: 2
#define PLAYER_EXPERTISE                                       	UNIT_END + 0x43D	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_OFFHAND_EXPERTISE                               	UNIT_END + 0x43E	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_CRIT_PERCENTAGE                                 	UNIT_END + 0x43F	//  Size: 1, Type: Float, Flags: 2
#define PLAYER_RANGED_CRIT_PERCENTAGE                          	UNIT_END + 0x440	//  Size: 1, Type: Float, Flags: 2
#define PLAYER_OFFHAND_CRIT_PERCENTAGE                         	UNIT_END + 0x441	//  Size: 1, Type: Float, Flags: 2
#define PLAYER_SPELL_CRIT_PERCENTAGE1                          	UNIT_END + 0x442	//  Size: 7, Type: Float, Flags: 2
#define PLAYER_SPELL_CRIT_PERCENTAGE01                         	UNIT_END + 0x443	//  Size: 7, Type: Float, Flags: 2
#define PLAYER_SPELL_CRIT_PERCENTAGE02                         	UNIT_END + 0x444	//  Size: 7, Type: Float, Flags: 2
#define PLAYER_SPELL_CRIT_PERCENTAGE03                         	UNIT_END + 0x445	//  Size: 7, Type: Float, Flags: 2
#define PLAYER_SPELL_CRIT_PERCENTAGE04                         	UNIT_END + 0x446	//  Size: 7, Type: Float, Flags: 2
#define PLAYER_SPELL_CRIT_PERCENTAGE05                         	UNIT_END + 0x447	//  Size: 7, Type: Float, Flags: 2
#define PLAYER_SPELL_CRIT_PERCENTAGE06                         	UNIT_END + 0x448	//  Size: 7, Type: Float, Flags: 2
#define PLAYER_SHIELD_BLOCK                                    	UNIT_END + 0x449	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_EXPLORED_ZONES_1                                	UNIT_END + 0x44A	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_01                               	UNIT_END + 0x44B	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_02                               	UNIT_END + 0x44C	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_03                               	UNIT_END + 0x44D	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_04                               	UNIT_END + 0x44E	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_05                               	UNIT_END + 0x44F	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_06                               	UNIT_END + 0x450	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_07                               	UNIT_END + 0x451	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_08                               	UNIT_END + 0x452	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_09                               	UNIT_END + 0x453	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_10                               	UNIT_END + 0x454	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_11                               	UNIT_END + 0x455	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_12                               	UNIT_END + 0x456	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_13                               	UNIT_END + 0x457	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_14                               	UNIT_END + 0x458	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_15                               	UNIT_END + 0x459	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_16                               	UNIT_END + 0x45A	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_17                               	UNIT_END + 0x45B	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_18                               	UNIT_END + 0x45C	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_19                               	UNIT_END + 0x45D	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_20                               	UNIT_END + 0x45E	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_21                               	UNIT_END + 0x45F	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_22                               	UNIT_END + 0x460	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_23                               	UNIT_END + 0x461	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_24                               	UNIT_END + 0x462	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_25                               	UNIT_END + 0x463	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_26                               	UNIT_END + 0x464	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_27                               	UNIT_END + 0x465	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_28                               	UNIT_END + 0x466	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_29                               	UNIT_END + 0x467	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_30                               	UNIT_END + 0x468	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_31                               	UNIT_END + 0x469	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_32                               	UNIT_END + 0x46A	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_33                               	UNIT_END + 0x46B	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_34                               	UNIT_END + 0x46C	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_35                               	UNIT_END + 0x46D	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_36                               	UNIT_END + 0x46E	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_37                               	UNIT_END + 0x46F	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_38                               	UNIT_END + 0x470	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_39                               	UNIT_END + 0x471	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_40                               	UNIT_END + 0x472	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_41                               	UNIT_END + 0x473	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_42                               	UNIT_END + 0x474	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_43                               	UNIT_END + 0x475	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_44                               	UNIT_END + 0x476	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_45                               	UNIT_END + 0x477	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_46                               	UNIT_END + 0x478	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_47                               	UNIT_END + 0x479	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_48                               	UNIT_END + 0x47A	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_49                               	UNIT_END + 0x47B	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_50                               	UNIT_END + 0x47C	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_51                               	UNIT_END + 0x47D	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_52                               	UNIT_END + 0x47E	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_53                               	UNIT_END + 0x47F	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_54                               	UNIT_END + 0x480	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_55                               	UNIT_END + 0x481	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_56                               	UNIT_END + 0x482	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_57                               	UNIT_END + 0x483	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_58                               	UNIT_END + 0x484	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_59                               	UNIT_END + 0x485	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_60                               	UNIT_END + 0x486	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_61                               	UNIT_END + 0x487	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_62                               	UNIT_END + 0x488	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_63                               	UNIT_END + 0x489	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_64                               	UNIT_END + 0x48A	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_65                               	UNIT_END + 0x48B	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_66                               	UNIT_END + 0x48C	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_67                               	UNIT_END + 0x48D	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_68                               	UNIT_END + 0x48E	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_69                               	UNIT_END + 0x48F	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_70                               	UNIT_END + 0x490	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_71                               	UNIT_END + 0x491	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_72                               	UNIT_END + 0x492	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_73                               	UNIT_END + 0x493	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_74                               	UNIT_END + 0x494	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_75                               	UNIT_END + 0x495	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_76                               	UNIT_END + 0x496	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_77                               	UNIT_END + 0x497	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_78                               	UNIT_END + 0x498	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_79                               	UNIT_END + 0x499	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_80                               	UNIT_END + 0x49A	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_81                               	UNIT_END + 0x49B	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_82                               	UNIT_END + 0x49C	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_83                               	UNIT_END + 0x49D	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_84                               	UNIT_END + 0x49E	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_85                               	UNIT_END + 0x49F	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_86                               	UNIT_END + 0x4A0	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_87                               	UNIT_END + 0x4A1	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_88                               	UNIT_END + 0x4A2	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_89                               	UNIT_END + 0x4A3	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_90                               	UNIT_END + 0x4A4	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_91                               	UNIT_END + 0x4A5	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_92                               	UNIT_END + 0x4A6	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_93                               	UNIT_END + 0x4A7	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_94                               	UNIT_END + 0x4A8	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_95                               	UNIT_END + 0x4A9	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_96                               	UNIT_END + 0x4AA	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_97                               	UNIT_END + 0x4AB	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_98                               	UNIT_END + 0x4AC	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_99                               	UNIT_END + 0x4AD	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_100                               UNIT_END + 0x4AE	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_101                               UNIT_END + 0x4AF	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_102                               UNIT_END + 0x4B0	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_103                               UNIT_END + 0x4B1	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_104                               UNIT_END + 0x4B2	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_105                               UNIT_END + 0x4B3	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_106                               UNIT_END + 0x4B4	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_107                               UNIT_END + 0x4B5	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_108                               UNIT_END + 0x4B6	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_109                               UNIT_END + 0x4B7	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_110                               UNIT_END + 0x4B8	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_111                               UNIT_END + 0x4B9	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_112                               UNIT_END + 0x4BA	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_113                               UNIT_END + 0x4BB	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_114                               UNIT_END + 0x4BC	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_115                               UNIT_END + 0x4BD	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_116                               UNIT_END + 0x4BE	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_117                               UNIT_END + 0x4BF	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_118                               UNIT_END + 0x4C0	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_119                               UNIT_END + 0x4C1	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_120                               UNIT_END + 0x4C2	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_121                               UNIT_END + 0x4C3	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_122                               UNIT_END + 0x4C4	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_123                               UNIT_END + 0x4C5	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_124                               UNIT_END + 0x4C6	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_125                               UNIT_END + 0x4C7	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_126                               UNIT_END + 0x4C8	//  Size: 128, Type: Bytes, Flags: 2
#define PLAYER_EXPLORED_ZONES_127                               UNIT_END + 0x4C9	//  Size: 128, Type: Bytes, Flags: 2


#define PLAYER_REST_STATE_EXPERIENCE                           	UNIT_END + 0x4CA	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_FIELD_COINAGE                                   	UNIT_END + 0x4CB	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_POS                       	UNIT_END + 0x4CC	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_POS_01                    	UNIT_END + 0x4CD	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_POS_02                    	UNIT_END + 0x4CE	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_POS_03                    	UNIT_END + 0x4CF	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_POS_04                    	UNIT_END + 0x4D0	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_POS_05                    	UNIT_END + 0x4D1	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_POS_06                    	UNIT_END + 0x4D2	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_NEG                       	UNIT_END + 0x4D3	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_NEG_01                    	UNIT_END + 0x4D4	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_NEG_02                    	UNIT_END + 0x4D5	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_NEG_03                    	UNIT_END + 0x4D6	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_NEG_04                    	UNIT_END + 0x4D7	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_NEG_05                    	UNIT_END + 0x4D8	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_NEG_06                    	UNIT_END + 0x4D9	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_PCT                       	UNIT_END + 0x4DA	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_PCT_01                    	UNIT_END + 0x4DB	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_PCT_02                    	UNIT_END + 0x4DC	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_PCT_03                    	UNIT_END + 0x4DD	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_PCT_04                    	UNIT_END + 0x4DE	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_PCT_05                    	UNIT_END + 0x4DF	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_DAMAGE_DONE_PCT_06                    	UNIT_END + 0x4E0	//  Size: 7, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_HEALING_DONE_POS                      	UNIT_END + 0x4E1	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_TARGET_RESISTANCE                     	UNIT_END + 0x4E2	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_TARGET_PHYSICAL_RESISTANCE            	UNIT_END + 0x4E3	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_FIELD_BYTES                                     	UNIT_END + 0x4E4	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_AMMO_ID                                         	UNIT_END + 0x4E5	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_SELF_RES_SPELL                                  	UNIT_END + 0x4E6	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_FIELD_PVP_MEDALS                                	UNIT_END + 0x4E7	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_PRICE_1                           	UNIT_END + 0x4E8	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_PRICE_01                          	UNIT_END + 0x4E9	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_PRICE_02                          	UNIT_END + 0x4EA	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_PRICE_03                          	UNIT_END + 0x4EB	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_PRICE_04                          	UNIT_END + 0x4EC	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_PRICE_05                          	UNIT_END + 0x4ED	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_PRICE_06                          	UNIT_END + 0x4EE	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_PRICE_07                          	UNIT_END + 0x4EF	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_PRICE_08                          	UNIT_END + 0x4F0	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_PRICE_09                          	UNIT_END + 0x4F1	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_PRICE_10                          	UNIT_END + 0x4F2	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_PRICE_11                          	UNIT_END + 0x4F3	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_1                       	UNIT_END + 0x4F4	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_01                      	UNIT_END + 0x4F5	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_02                      	UNIT_END + 0x4F6	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_03                      	UNIT_END + 0x4F7	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_04                      	UNIT_END + 0x4F8	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_05                      	UNIT_END + 0x4F9	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_06                      	UNIT_END + 0x4FA	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_07                      	UNIT_END + 0x4FB	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_08                      	UNIT_END + 0x4FC	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_09                      	UNIT_END + 0x4FD	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_10                      	UNIT_END + 0x4FE	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_11                      	UNIT_END + 0x4FF	//  Size: 12, Type: Int32, Flags: 2
#define PLAYER_FIELD_KILLS                                     	UNIT_END + 0x500	//  Size: 1, Type: Chars?, Flags: 2
#define PLAYER_FIELD_TODAY_CONTRIBUTION                        	UNIT_END + 0x501	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_FIELD_YESTERDAY_CONTRIBUTION                    	UNIT_END + 0x502	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_FIELD_LIFETIME_HONORBALE_KILLS                  	UNIT_END + 0x503	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_FIELD_BYTES2                                    	UNIT_END + 0x504	//  Size: 1, Type: Bytes, Flags: 2
#define PLAYER_FIELD_WATCHED_FACTION_INDEX                     	UNIT_END + 0x505	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_1                           	UNIT_END + 0x506	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_01                          	UNIT_END + 0x507	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_02                          	UNIT_END + 0x508	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_03                          	UNIT_END + 0x509	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_04                          	UNIT_END + 0x50A	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_05                          	UNIT_END + 0x50B	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_06                          	UNIT_END + 0x50C	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_07                          	UNIT_END + 0x50D	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_08                          	UNIT_END + 0x50E	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_09                          	UNIT_END + 0x50F	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_10                          	UNIT_END + 0x510	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_11                          	UNIT_END + 0x511	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_12                          	UNIT_END + 0x512	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_13                          	UNIT_END + 0x513	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_14                          	UNIT_END + 0x514	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_15                          	UNIT_END + 0x515	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_16                          	UNIT_END + 0x516	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_17                          	UNIT_END + 0x517	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_18                          	UNIT_END + 0x518	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_19                          	UNIT_END + 0x519	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_20                          	UNIT_END + 0x51A	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_21                          	UNIT_END + 0x51B	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_22                          	UNIT_END + 0x51C	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_COMBAT_RATING_23                          	UNIT_END + 0x51D	//  Size: 24, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_1                       	UNIT_END + 0x51E	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_01                      	UNIT_END + 0x51F	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_02                      	UNIT_END + 0x520	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_03                      	UNIT_END + 0x521	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_04                      	UNIT_END + 0x522	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_05                      	UNIT_END + 0x523	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_06                      	UNIT_END + 0x524	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_07                      	UNIT_END + 0x525	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_08                      	UNIT_END + 0x526	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_09                      	UNIT_END + 0x527	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_10                      	UNIT_END + 0x528	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_11                      	UNIT_END + 0x529	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_12                      	UNIT_END + 0x52A	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_13                      	UNIT_END + 0x52B	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_14                      	UNIT_END + 0x52C	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_15                      	UNIT_END + 0x52D	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_16                      	UNIT_END + 0x52E	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_17                      	UNIT_END + 0x52F	//  Size: 18, Type: Int32, Flags: 2
#define PLAYER_FIELD_HONOR_CURRENCY                            	UNIT_END + 0x530	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_FIELD_ARENA_CURRENCY                            	UNIT_END + 0x531	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_FIELD_MOD_MANA_REGEN                            	UNIT_END + 0x532	//  Size: 1, Type: Float, Flags: 2
#define PLAYER_FIELD_MOD_MANA_REGEN_INTERRUPT                  	UNIT_END + 0x533	//  Size: 1, Type: Float, Flags: 2
#define PLAYER_FIELD_MAX_LEVEL                                 	UNIT_END + 0x534	//  Size: 1, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_00                            UNIT_END + 0x535	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_01                           	UNIT_END + 0x536	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_02                           	UNIT_END + 0x537	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_03                           	UNIT_END + 0x538	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_04                           	UNIT_END + 0x539	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_05                           	UNIT_END + 0x53A	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_06                           	UNIT_END + 0x53B	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_07                           	UNIT_END + 0x53C	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_08                           	UNIT_END + 0x53D	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_09                           	UNIT_END + 0x53E	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_10                           	UNIT_END + 0x53F	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_11                           	UNIT_END + 0x540	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_12                           	UNIT_END + 0x541	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_13                           	UNIT_END + 0x542	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_14                           	UNIT_END + 0x543	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_15                           	UNIT_END + 0x544	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_16                           	UNIT_END + 0x545	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_17                           	UNIT_END + 0x546	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_18                           	UNIT_END + 0x547	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_19                           	UNIT_END + 0x548	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_20                           	UNIT_END + 0x549	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_21                           	UNIT_END + 0x54A	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_22                           	UNIT_END + 0x54B	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_23                           	UNIT_END + 0x54C	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_FIELD_DAILY_QUESTS_24                           	UNIT_END + 0x54D	//  Size: 25, Type: Int32, Flags: 2
#define PLAYER_END                                              UNIT_END + 0x54E

//OBJECT:
#define OBJECT_FIELD_CREATED_BY                                	OBJECT_END + 0x000	//  Size: 2, Type: GUID, Flags: 1

//GAMEOBJECT:
#define GAMEOBJECT_DISPLAYID                                   	OBJECT_END + 0x002	//  Size: 1, Type: Int32, Flags: 1
#define GAMEOBJECT_FLAGS                                       	OBJECT_END + 0x003	//  Size: 1, Type: Int32, Flags: 1
#define GAMEOBJECT_ROTATION                                    	OBJECT_END + 0x004	//  Size: 4, Type: Float, Flags: 1
#define GAMEOBJECT_ROTATION_01                                 	OBJECT_END + 0x005	//  Size: 4, Type: Float, Flags: 1
#define GAMEOBJECT_ROTATION_02                                 	OBJECT_END + 0x006	//  Size: 4, Type: Float, Flags: 1
#define GAMEOBJECT_ROTATION_03                                 	OBJECT_END + 0x007	//  Size: 4, Type: Float, Flags: 1
#define GAMEOBJECT_STATE                                       	OBJECT_END + 0x008	//  Size: 1, Type: Int32, Flags: 1
#define GAMEOBJECT_POS_X                                       	OBJECT_END + 0x009	//  Size: 1, Type: Float, Flags: 1
#define GAMEOBJECT_POS_Y                                       	OBJECT_END + 0x00A	//  Size: 1, Type: Float, Flags: 1
#define GAMEOBJECT_POS_Z                                       	OBJECT_END + 0x00B	//  Size: 1, Type: Float, Flags: 1
#define GAMEOBJECT_FACING                                      	OBJECT_END + 0x00C	//  Size: 1, Type: Float, Flags: 1
#define GAMEOBJECT_DYN_FLAGS                                   	OBJECT_END + 0x00D	//  Size: 1, Type: Int32, Flags: 256
#define GAMEOBJECT_FACTION                                     	OBJECT_END + 0x00E	//  Size: 1, Type: Int32, Flags: 1
#define GAMEOBJECT_TYPE_ID                                     	OBJECT_END + 0x00F	//  Size: 1, Type: Int32, Flags: 1
#define GAMEOBJECT_LEVEL                                       	OBJECT_END + 0x010	//  Size: 1, Type: Int32, Flags: 1
#define GAMEOBJECT_ARTKIT                                      	OBJECT_END + 0x011	//  Size: 1, Type: Int32, Flags: 1
#define GAMEOBJECT_ANIMPROGRESS                                	OBJECT_END + 0x012	//  Size: 1, Type: Int32, Flags: 256
#define GAMEOBJECT_PADDING                                     	OBJECT_END + 0x013	//  Size: 1, Type: Int32, Flags: 0
#define GAMEOBJECT_END                                          OBJECT_END + 0x014

//DYNAMICOBJECT:
#define DYNAMICOBJECT_CASTER                                   	OBJECT_END + 0x000	//  Size: 2, Type: GUID, Flags: 1
#define DYNAMICOBJECT_BYTES                                    	OBJECT_END + 0x002	//  Size: 1, Type: Bytes, Flags: 1
#define DYNAMICOBJECT_SPELLID                                  	OBJECT_END + 0x003	//  Size: 1, Type: Int32, Flags: 1
#define DYNAMICOBJECT_RADIUS                                   	OBJECT_END + 0x004	//  Size: 1, Type: Float, Flags: 1
#define DYNAMICOBJECT_POS_X                                    	OBJECT_END + 0x005	//  Size: 1, Type: Float, Flags: 1
#define DYNAMICOBJECT_POS_Y                                    	OBJECT_END + 0x006	//  Size: 1, Type: Float, Flags: 1
#define DYNAMICOBJECT_POS_Z                                    	OBJECT_END + 0x007	//  Size: 1, Type: Float, Flags: 1
#define DYNAMICOBJECT_FACING                                   	OBJECT_END + 0x008	//  Size: 1, Type: Float, Flags: 1
#define DYNAMICOBJECT_CASTTIME                                 	OBJECT_END + 0x009	//  Size: 1, Type: Int32, Flags: 1
#define DYNAMICOBJECT_END                                       OBJECT_END + 0x00A

//CORPSE:
#define CORPSE_FIELD_OWNER                                     	OBJECT_END + 0x000	//  Size: 2, Type: GUID, Flags: 1
#define CORPSE_FIELD_PARTY										OBJECT_END + 0x002	//  Size: 2, Type: LONG, Flags: 1
#define CORPSE_FIELD_FACING                                    	OBJECT_END + 0x004	//  Size: 1, Type: Float, Flags: 1
#define CORPSE_FIELD_POS_X                                     	OBJECT_END + 0x005	//  Size: 1, Type: Float, Flags: 1
#define CORPSE_FIELD_POS_Y                                     	OBJECT_END + 0x006	//  Size: 1, Type: Float, Flags: 1
#define CORPSE_FIELD_POS_Z                                     	OBJECT_END + 0x007	//  Size: 1, Type: Float, Flags: 1
#define CORPSE_FIELD_DISPLAY_ID                                	OBJECT_END + 0x008	//  Size: 1, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM                                      	OBJECT_END + 0x009	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_01                                   	OBJECT_END + 0x00A	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_02                                   	OBJECT_END + 0x00B	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_03                                   	OBJECT_END + 0x00C	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_04                                   	OBJECT_END + 0x00D	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_05                                   	OBJECT_END + 0x00E	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_06                                   	OBJECT_END + 0x00F	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_07                                   	OBJECT_END + 0x010	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_08                                   	OBJECT_END + 0x011	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_09                                   	OBJECT_END + 0x012	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_10                                   	OBJECT_END + 0x013	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_11                                   	OBJECT_END + 0x014	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_12                                   	OBJECT_END + 0x015	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_13                                   	OBJECT_END + 0x016	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_14                                   	OBJECT_END + 0x017	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_15                                   	OBJECT_END + 0x018	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_16                                   	OBJECT_END + 0x019	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_17                                   	OBJECT_END + 0x01A	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_ITEM_18                                   	OBJECT_END + 0x01B	//  Size: 19, Type: Int32, Flags: 1
#define CORPSE_FIELD_BYTES_1                                   	OBJECT_END + 0x01C	//  Size: 1, Type: Bytes, Flags: 1
#define CORPSE_FIELD_BYTES_2                                   	OBJECT_END + 0x01D	//  Size: 1, Type: Bytes, Flags: 1
#define CORPSE_FIELD_GUILD                                     	OBJECT_END + 0x01E	//  Size: 1, Type: Int32, Flags: 1
#define CORPSE_FIELD_FLAGS                                     	OBJECT_END + 0x01F	//  Size: 1, Type: Int32, Flags: 1
#define CORPSE_FIELD_DYNAMIC_FLAGS                             	OBJECT_END + 0x020	//  Size: 1, Type: Int32, Flags: 256
#define CORPSE_FIELD_PAD										OBJECT_END + 0x021	//  Size: 1, Type: Int32, Flags: 1
#define CORPSE_END                                              OBJECT_END + 0x022



#define UNIT_FIELD_STRENGTH						  UNIT_FIELD_STAT0
#define UNIT_FIELD_AGILITY                        UNIT_FIELD_STAT1
#define UNIT_FIELD_STAMINA                        UNIT_FIELD_STAT2
#define UNIT_FIELD_INTELLECT                      UNIT_FIELD_STAT3
#define UNIT_FIELD_SPIRIRT                        UNIT_FIELD_STAT4




#endif		// __arcemu_UPDATEFIELDS_H

/*
//client 3.0.1 fields
#ifndef __arcemu_UPDATEFIELDS_H
#define __arcemu_UPDATEFIELDS_H

//OBJECT_FIELD_:
#define OBJECT_FIELD_GUID                                       0x0000     
#define OBJECT_FIELD_GUID_EXTRA                                 0x0001     
#define OBJECT_FIELD_TYPE                                       0x0002     
#define OBJECT_FIELD_ENTRY                                      0x0003     
#define OBJECT_FIELD_SCALE_X                                    0x0004     
#define OBJECT_FIELD_PADDING                                    0x0005     
#define OBJECT_FIELD_END                                        0x0006     

//CONTAINER_FIELD_:
#define CONTAINER_FIELD_NUM_SLOTS                               ITEM_END + 0x0000     
#define CONTAINER_ALIGN_PAD                                     ITEM_END + 0x0001     
#define CONTAINER_FIELD_SLOT_1                                  ITEM_END + 0x0002     
#define CONTAINER_FIELD_SLOT_1X                                 ITEM_END + 0x0003     
#define CONTAINER_FIELD_SLOT_2                                  ITEM_END + 0x0004     
#define CONTAINER_FIELD_SLOT_2X                                 ITEM_END + 0x0005     
#define CONTAINER_FIELD_SLOT_3                                  ITEM_END + 0x0006     
#define CONTAINER_FIELD_SLOT_3X                                 ITEM_END + 0x0007     
#define CONTAINER_FIELD_SLOT_4                                  ITEM_END + 0x0008     
#define CONTAINER_FIELD_SLOT_4X                                 ITEM_END + 0x0009     
#define CONTAINER_FIELD_SLOT_5                                  ITEM_END + 0x000A     
#define CONTAINER_FIELD_SLOT_5X                                 ITEM_END + 0x000B     
#define CONTAINER_FIELD_SLOT_6                                  ITEM_END + 0x000C     
#define CONTAINER_FIELD_SLOT_6X                                 ITEM_END + 0x000D     
#define CONTAINER_FIELD_SLOT_7                                  ITEM_END + 0x000E     
#define CONTAINER_FIELD_SLOT_7X                                 ITEM_END + 0x000F     
#define CONTAINER_FIELD_SLOT_8                                  ITEM_END + 0x0010     
#define CONTAINER_FIELD_SLOT_8X                                 ITEM_END + 0x0011     
#define CONTAINER_FIELD_SLOT_9                                  ITEM_END + 0x0012     
#define CONTAINER_FIELD_SLOT_9X                                 ITEM_END + 0x0013     
#define CONTAINER_FIELD_SLOT_10                                 ITEM_END + 0x0014     
#define CONTAINER_FIELD_SLOT_10X                                ITEM_END + 0x0015     
#define CONTAINER_FIELD_SLOT_11                                 ITEM_END + 0x0016     
#define CONTAINER_FIELD_SLOT_11X                                ITEM_END + 0x0017     
#define CONTAINER_FIELD_SLOT_12                                 ITEM_END + 0x0018     
#define CONTAINER_FIELD_SLOT_12X                                ITEM_END + 0x0019     
#define CONTAINER_FIELD_SLOT_13                                 ITEM_END + 0x001A     
#define CONTAINER_FIELD_SLOT_13X                                ITEM_END + 0x001B     
#define CONTAINER_FIELD_SLOT_14                                 ITEM_END + 0x001C     
#define CONTAINER_FIELD_SLOT_14X                                ITEM_END + 0x001D     
#define CONTAINER_FIELD_SLOT_15                                 ITEM_END + 0x001E     
#define CONTAINER_FIELD_SLOT_15X                                ITEM_END + 0x001F     
#define CONTAINER_FIELD_SLOT_16                                 ITEM_END + 0x0020     
#define CONTAINER_FIELD_SLOT_16X                                ITEM_END + 0x0021     
#define CONTAINER_FIELD_SLOT_17                                 ITEM_END + 0x0022     
#define CONTAINER_FIELD_SLOT_17X                                ITEM_END + 0x0023     
#define CONTAINER_FIELD_SLOT_18                                 ITEM_END + 0x0024     
#define CONTAINER_FIELD_SLOT_18X                                ITEM_END + 0x0025     
#define CONTAINER_FIELD_SLOT_19                                 ITEM_END + 0x0026     
#define CONTAINER_FIELD_SLOT_19X                                ITEM_END + 0x0027     
#define CONTAINER_FIELD_SLOT_20                                 ITEM_END + 0x0028     
#define CONTAINER_FIELD_SLOT_20X                                ITEM_END + 0x0029     
#define CONTAINER_FIELD_SLOT_21                                 ITEM_END + 0x002A     
#define CONTAINER_FIELD_SLOT_21X                                ITEM_END + 0x002B     
#define CONTAINER_FIELD_SLOT_22                                 ITEM_END + 0x002C     
#define CONTAINER_FIELD_SLOT_22X                                ITEM_END + 0x002D     
#define CONTAINER_FIELD_SLOT_23                                 ITEM_END + 0x002E     
#define CONTAINER_FIELD_SLOT_23X                                ITEM_END + 0x002F     
#define CONTAINER_FIELD_SLOT_24                                 ITEM_END + 0x0030     
#define CONTAINER_FIELD_SLOT_24X                                ITEM_END + 0x0031     
#define CONTAINER_FIELD_SLOT_25                                 ITEM_END + 0x0032     
#define CONTAINER_FIELD_SLOT_25X                                ITEM_END + 0x0033     
#define CONTAINER_FIELD_SLOT_26                                 ITEM_END + 0x0034     
#define CONTAINER_FIELD_SLOT_26X                                ITEM_END + 0x0035     
#define CONTAINER_FIELD_SLOT_27                                 ITEM_END + 0x0036     
#define CONTAINER_FIELD_SLOT_27X                                ITEM_END + 0x0037     
#define CONTAINER_FIELD_SLOT_28                                 ITEM_END + 0x0038     
#define CONTAINER_FIELD_SLOT_28X                                ITEM_END + 0x0039     
#define CONTAINER_FIELD_SLOT_29                                 ITEM_END + 0x003A     
#define CONTAINER_FIELD_SLOT_29X                                ITEM_END + 0x003B     
#define CONTAINER_FIELD_SLOT_30                                 ITEM_END + 0x003C     
#define CONTAINER_FIELD_SLOT_30X                                ITEM_END + 0x003D     
#define CONTAINER_FIELD_SLOT_31                                 ITEM_END + 0x003E     
#define CONTAINER_FIELD_SLOT_31X                                ITEM_END + 0x003F     
#define CONTAINER_FIELD_SLOT_32                                 ITEM_END + 0x0040     
#define CONTAINER_FIELD_SLOT_32X                                ITEM_END + 0x0041     
#define CONTAINER_FIELD_SLOT_33                                 ITEM_END + 0x0042     
#define CONTAINER_FIELD_SLOT_33X                                ITEM_END + 0x0043     
#define CONTAINER_FIELD_SLOT_34                                 ITEM_END + 0x0044     
#define CONTAINER_FIELD_SLOT_34X                                ITEM_END + 0x0045     
#define CONTAINER_FIELD_SLOT_35                                 ITEM_END + 0x0046     
#define CONTAINER_FIELD_SLOT_35X                                ITEM_END + 0x0047     
#define CONTAINER_FIELD_SLOT_36                                 ITEM_END + 0x0048     
#define CONTAINER_FIELD_SLOT_36X                                ITEM_END + 0x0049     
#define CONTAINER_FIELD_END                                     ITEM_END + 0x004A     

//ITEM_FIELD_:
#define ITEM_FIELD_OWNER                                        OBJECT_END + 0x0000     
#define ITEM_FIELD_OWNER_EXTRA                                  OBJECT_END + 0x0001     
#define ITEM_FIELD_CONTAINED                                    OBJECT_END + 0x0002     
#define ITEM_FIELD_CONTAINED_EXTRA                              OBJECT_END + 0x0003     
#define ITEM_FIELD_CREATOR                                      OBJECT_END + 0x0004     
#define ITEM_FIELD_CREATOR_EXTRA                                OBJECT_END + 0x0005     
#define ITEM_FIELD_GIFTCREATOR                                  OBJECT_END + 0x0006     
#define ITEM_FIELD_GIFTCREATOR_EXTRA                            OBJECT_END + 0x0007     
#define ITEM_FIELD_STACK_COUNT                                  OBJECT_END + 0x0008     
#define ITEM_FIELD_DURATION                                     OBJECT_END + 0x0009     
#define ITEM_FIELD_SPELL_CHARGES                                OBJECT_END + 0x000A     
#define ITEM_FIELD_SPELL_CHARGES_2                              OBJECT_END + 0x000B     
#define ITEM_FIELD_SPELL_CHARGES_3                              OBJECT_END + 0x000C     
#define ITEM_FIELD_SPELL_CHARGES_4                              OBJECT_END + 0x000D     
#define ITEM_FIELD_SPELL_CHARGES_5                              OBJECT_END + 0x000E     
#define ITEM_FIELD_FLAGS                                        OBJECT_END + 0x000F     
#define ITEM_FIELD_ENCHANTMENT                                  OBJECT_END + 0x0010     
#define ITEM_FIELD_ENCHANTMENT_2                                OBJECT_END + 0x0011     
#define ITEM_FIELD_ENCHANTMENT_3                                OBJECT_END + 0x0012     
#define ITEM_FIELD_ENCHANTMENT_4                                OBJECT_END + 0x0013     
#define ITEM_FIELD_ENCHANTMENT_5                                OBJECT_END + 0x0014     
#define ITEM_FIELD_ENCHANTMENT_6                                OBJECT_END + 0x0015     
#define ITEM_FIELD_ENCHANTMENT_7                                OBJECT_END + 0x0016     
#define ITEM_FIELD_ENCHANTMENT_8                                OBJECT_END + 0x0017     
#define ITEM_FIELD_ENCHANTMENT_9                                OBJECT_END + 0x0018     
#define ITEM_FIELD_ENCHANTMENT_10                               OBJECT_END + 0x0019     
#define ITEM_FIELD_ENCHANTMENT_11                               OBJECT_END + 0x001A     
#define ITEM_FIELD_ENCHANTMENT_12                               OBJECT_END + 0x001B     
#define ITEM_FIELD_ENCHANTMENT_13                               OBJECT_END + 0x001C     
#define ITEM_FIELD_ENCHANTMENT_14                               OBJECT_END + 0x001D     
#define ITEM_FIELD_ENCHANTMENT_15                               OBJECT_END + 0x001E     
#define ITEM_FIELD_ENCHANTMENT_16                               OBJECT_END + 0x001F     
#define ITEM_FIELD_ENCHANTMENT_17                               OBJECT_END + 0x0020     
#define ITEM_FIELD_ENCHANTMENT_18                               OBJECT_END + 0x0021     
#define ITEM_FIELD_ENCHANTMENT_19                               OBJECT_END + 0x0022     
#define ITEM_FIELD_ENCHANTMENT_20                               OBJECT_END + 0x0023     
#define ITEM_FIELD_ENCHANTMENT_21                               OBJECT_END + 0x0024     
#define ITEM_FIELD_ENCHANTMENT_22                               OBJECT_END + 0x0025     
#define ITEM_FIELD_ENCHANTMENT_23                               OBJECT_END + 0x0026     
#define ITEM_FIELD_ENCHANTMENT_24                               OBJECT_END + 0x0027     
#define ITEM_FIELD_ENCHANTMENT_25                               OBJECT_END + 0x0028     
#define ITEM_FIELD_ENCHANTMENT_26                               OBJECT_END + 0x0029     
#define ITEM_FIELD_ENCHANTMENT_27                               OBJECT_END + 0x002A     
#define ITEM_FIELD_ENCHANTMENT_28                               OBJECT_END + 0x002B     
#define ITEM_FIELD_ENCHANTMENT_29                               OBJECT_END + 0x002C     
#define ITEM_FIELD_ENCHANTMENT_30                               OBJECT_END + 0x002D     
#define ITEM_FIELD_ENCHANTMENT_31                               OBJECT_END + 0x002E     
#define ITEM_FIELD_ENCHANTMENT_32                               OBJECT_END + 0x002F     
#define ITEM_FIELD_ENCHANTMENT_33                               OBJECT_END + 0x0030     
#define ITEM_FIELD_ENCHANTMENT_34                               OBJECT_END + 0x0031     
#define ITEM_FIELD_ENCHANTMENT_35                               OBJECT_END + 0x0032     
#define ITEM_FIELD_ENCHANTMENT_36                               OBJECT_END + 0x0033     
#define ITEM_FIELD_PROPERTY_SEED                                OBJECT_END + 0x0034     
#define ITEM_FIELD_RANDOM_PROPERTIES_ID                         OBJECT_END + 0x0035     
#define ITEM_FIELD_ITEM_TEXT_ID                                 OBJECT_END + 0x0036     
#define ITEM_FIELD_DURABILITY                                   OBJECT_END + 0x0037     
#define ITEM_FIELD_MAXDURABILITY                                OBJECT_END + 0x0038     
#define ITEM_FIELD_PAD                                          OBJECT_END + 0x0039     
#define ITEM_FIELD_END                                          OBJECT_END + 0x003A     

//UNIT_FIELD_:
#define UNIT_FIELD_CHARM                                        OBJECT_END + 0x0000     
#define UNIT_FIELD_CHARM_X                                      OBJECT_END + 0x0001     
#define UNIT_FIELD_SUMMON                                       OBJECT_END + 0x0002     
#define UNIT_FIELD_SUMMON_X                                     OBJECT_END + 0x0003     
#define UNIT_FIELD_CHARMEDBY                                    OBJECT_END + 0x0004     
#define UNIT_FIELD_CHARMEDBY_X                                  OBJECT_END + 0x0005     
#define UNIT_FIELD_SUMMONEDBY                                   OBJECT_END + 0x0006     
#define UNIT_FIELD_SUMMONEDBY_X                                 OBJECT_END + 0x0007     
#define UNIT_FIELD_CREATEDBY                                    OBJECT_END + 0x0008     
#define UNIT_FIELD_CREATEDBY_X                                  OBJECT_END + 0x0009     
#define UNIT_FIELD_TARGET                                       OBJECT_END + 0x000A     
#define UNIT_FIELD_TARGET_X                                     OBJECT_END + 0x000B     
#define UNIT_FIELD_PERSUADED                                    OBJECT_END + 0x000C     
#define UNIT_FIELD_PERSUADED_X                                  OBJECT_END + 0x000D     
#define UNIT_FIELD_CHANNEL_OBJECT                               OBJECT_END + 0x000E     
#define UNIT_FIELD_CHANNEL_OBJECT_X                             OBJECT_END + 0x000F     
#define UNIT_FIELD_HEALTH                                       OBJECT_END + 0x0010     
#define UNIT_FIELD_POWER1                                       OBJECT_END + 0x0011     
#define UNIT_FIELD_POWER2                                       OBJECT_END + 0x0012     
#define UNIT_FIELD_POWER3                                       OBJECT_END + 0x0013     
#define UNIT_FIELD_POWER4                                       OBJECT_END + 0x0014     
#define UNIT_FIELD_POWER5                                       OBJECT_END + 0x0015     
#define UNIT_FIELD_POWER6                                       OBJECT_END + 0x0016     
#define UNIT_FIELD_POWER7                                       OBJECT_END + 0x0017     
#define UNIT_FIELD_MAXHEALTH                                    OBJECT_END + 0x0018     
#define UNIT_FIELD_MAXPOWER1                                    OBJECT_END + 0x0019     
#define UNIT_FIELD_MAXPOWER2                                    OBJECT_END + 0x001A     
#define UNIT_FIELD_MAXPOWER3                                    OBJECT_END + 0x001B     
#define UNIT_FIELD_MAXPOWER4                                    OBJECT_END + 0x001C     
#define UNIT_FIELD_MAXPOWER5                                    OBJECT_END + 0x001D     
#define UNIT_FIELD_MAXPOWER6                                    OBJECT_END + 0x001E     
#define UNIT_FIELD_MAXPOWER7                                    OBJECT_END + 0x001F     
#define UNIT_FIELD_LEVEL                                        OBJECT_END + 0x0020     
#define UNIT_FIELD_FACTIONTEMPLATE                              OBJECT_END + 0x0021     
#define UNIT_FIELD_BYTES_0                                      OBJECT_END + 0x0022     
#define UNIT_VIRTUAL_ITEM_SLOT_DISPLAY                          OBJECT_END + 0x0023     
#define UNIT_VIRUTAL_ITEM_SLOT_DISPLAY_2                        OBJECT_END + 0x0024     
#define UNIT_VIRUTAL_ITEM_SLOT_DISPLAY_3                        OBJECT_END + 0x0025     
#define UNIT_VIRTUAL_ITEM_INFO                                  OBJECT_END + 0x0026     
#define UNIT_VIRTUAL_ITEM_INFO_2                                OBJECT_END + 0x0027     
#define UNIT_VIRTUAL_ITEM_INFO_3                                OBJECT_END + 0x0028     
#define UNIT_VIRTUAL_ITEM_INFO_4                                OBJECT_END + 0x0029     
#define UNIT_VIRTUAL_ITEM_INFO_5                                OBJECT_END + 0x002A     
#define UNIT_VIRTUAL_ITEM_INFO_6                                OBJECT_END + 0x002B     
#define UNIT_FIELD_FLAGS                                        OBJECT_END + 0x002C     
#define UNIT_FIELD_FLAGS_2                                      OBJECT_END + 0x002D     
#define UNIT_FIELD_AURA                                         OBJECT_END + 0x002E     
#define UNIT_FIELD_AURA_2                                       OBJECT_END + 0x002F     
#define UNIT_FIELD_AURA_3                                       OBJECT_END + 0x0030     
#define UNIT_FIELD_AURA_4                                       OBJECT_END + 0x0031     
#define UNIT_FIELD_AURA_5                                       OBJECT_END + 0x0032     
#define UNIT_FIELD_AURA_6                                       OBJECT_END + 0x0033     
#define UNIT_FIELD_AURA_7                                       OBJECT_END + 0x0034     
#define UNIT_FIELD_AURA_8                                       OBJECT_END + 0x0035     
#define UNIT_FIELD_AURA_9                                       OBJECT_END + 0x0036     
#define UNIT_FIELD_AURA_10                                      OBJECT_END + 0x0037     
#define UNIT_FIELD_AURA_11                                      OBJECT_END + 0x0038     
#define UNIT_FIELD_AURA_12                                      OBJECT_END + 0x0039     
#define UNIT_FIELD_AURA_13                                      OBJECT_END + 0x003A     
#define UNIT_FIELD_AURA_14                                      OBJECT_END + 0x003B     
#define UNIT_FIELD_AURA_15                                      OBJECT_END + 0x003C     
#define UNIT_FIELD_AURA_16                                      OBJECT_END + 0x003D     
#define UNIT_FIELD_AURA_17                                      OBJECT_END + 0x003E     
#define UNIT_FIELD_AURA_18                                      OBJECT_END + 0x003F     
#define UNIT_FIELD_AURA_19                                      OBJECT_END + 0x0040     
#define UNIT_FIELD_AURA_20                                      OBJECT_END + 0x0041     
#define UNIT_FIELD_AURA_21                                      OBJECT_END + 0x0042     
#define UNIT_FIELD_AURA_22                                      OBJECT_END + 0x0043     
#define UNIT_FIELD_AURA_23                                      OBJECT_END + 0x0044     
#define UNIT_FIELD_AURA_24                                      OBJECT_END + 0x0045     
#define UNIT_FIELD_AURA_25                                      OBJECT_END + 0x0046     
#define UNIT_FIELD_AURA_26                                      OBJECT_END + 0x0047     
#define UNIT_FIELD_AURA_27                                      OBJECT_END + 0x0048     
#define UNIT_FIELD_AURA_28                                      OBJECT_END + 0x0049     
#define UNIT_FIELD_AURA_29                                      OBJECT_END + 0x004A     
#define UNIT_FIELD_AURA_30                                      OBJECT_END + 0x004B     
#define UNIT_FIELD_AURA_31                                      OBJECT_END + 0x004C     
#define UNIT_FIELD_AURA_32                                      OBJECT_END + 0x004D     
#define UNIT_FIELD_AURA_33                                      OBJECT_END + 0x004E     
#define UNIT_FIELD_AURA_34                                      OBJECT_END + 0x004F     
#define UNIT_FIELD_AURA_35                                      OBJECT_END + 0x0050     
#define UNIT_FIELD_AURA_36                                      OBJECT_END + 0x0051     
#define UNIT_FIELD_AURA_37                                      OBJECT_END + 0x0052     
#define UNIT_FIELD_AURA_38                                      OBJECT_END + 0x0053     
#define UNIT_FIELD_AURA_39                                      OBJECT_END + 0x0054     
#define UNIT_FIELD_AURA_40                                      OBJECT_END + 0x0055     
#define UNIT_FIELD_AURA_41                                      OBJECT_END + 0x0056     
#define UNIT_FIELD_AURA_42                                      OBJECT_END + 0x0057     
#define UNIT_FIELD_AURA_43                                      OBJECT_END + 0x0058     
#define UNIT_FIELD_AURA_44                                      OBJECT_END + 0x0059     
#define UNIT_FIELD_AURA_45                                      OBJECT_END + 0x005A     
#define UNIT_FIELD_AURA_46                                      OBJECT_END + 0x005B     
#define UNIT_FIELD_AURA_47                                      OBJECT_END + 0x005C     
#define UNIT_FIELD_AURA_48                                      OBJECT_END + 0x005D     
#define UNIT_FIELD_AURA_49                                      OBJECT_END + 0x005E     
#define UNIT_FIELD_AURA_50                                      OBJECT_END + 0x005F     
#define UNIT_FIELD_AURA_51                                      OBJECT_END + 0x0060     
#define UNIT_FIELD_AURA_52                                      OBJECT_END + 0x0061     
#define UNIT_FIELD_AURA_53                                      OBJECT_END + 0x0062     
#define UNIT_FIELD_AURA_54                                      OBJECT_END + 0x0063     
#define UNIT_FIELD_AURA_55                                      OBJECT_END + 0x0064     
#define UNIT_FIELD_AURA_56                                      OBJECT_END + 0x0065     
#define UNIT_FIELD_AURAFLAGS                                    OBJECT_END + 0x0066     
#define UNIT_FIELD_AURAFLAGS_2                                  OBJECT_END + 0x0067     
#define UNIT_FIELD_AURAFLAGS_3                                  OBJECT_END + 0x0068     
#define UNIT_FIELD_AURAFLAGS_4                                  OBJECT_END + 0x0069     
#define UNIT_FIELD_AURAFLAGS_5                                  OBJECT_END + 0x006A     
#define UNIT_FIELD_AURAFLAGS_6                                  OBJECT_END + 0x006B     
#define UNIT_FIELD_AURAFLAGS_7                                  OBJECT_END + 0x006C     
#define UNIT_FIELD_AURAFLAGS_8                                  OBJECT_END + 0x006D     
#define UNIT_FIELD_AURAFLAGS_9                                  OBJECT_END + 0x006E     
#define UNIT_FIELD_AURAFLAGS_10                                 OBJECT_END + 0x006F     
#define UNIT_FIELD_AURAFLAGS_11                                 OBJECT_END + 0x0070     
#define UNIT_FIELD_AURAFLAGS_12                                 OBJECT_END + 0x0071     
#define UNIT_FIELD_AURAFLAGS_13                                 OBJECT_END + 0x0072     
#define UNIT_FIELD_AURAFLAGS_14                                 OBJECT_END + 0x0073     
#define UNIT_FIELD_AURALEVELS                                   OBJECT_END + 0x0074     
#define UNIT_FIELD_AURALEVELS_2                                 OBJECT_END + 0x0075     
#define UNIT_FIELD_AURALEVELS_3                                 OBJECT_END + 0x0076     
#define UNIT_FIELD_AURALEVELS_4                                 OBJECT_END + 0x0077     
#define UNIT_FIELD_AURALEVELS_5                                 OBJECT_END + 0x0078     
#define UNIT_FIELD_AURALEVELS_6                                 OBJECT_END + 0x0079     
#define UNIT_FIELD_AURALEVELS_7                                 OBJECT_END + 0x007A     
#define UNIT_FIELD_AURALEVELS_8                                 OBJECT_END + 0x007B     
#define UNIT_FIELD_AURALEVELS_9                                 OBJECT_END + 0x007C     
#define UNIT_FIELD_AURALEVELS_10                                OBJECT_END + 0x007D     
#define UNIT_FIELD_AURALEVELS_11                                OBJECT_END + 0x007E     
#define UNIT_FIELD_AURALEVELS_12                                OBJECT_END + 0x007F     
#define UNIT_FIELD_AURALEVELS_13                                OBJECT_END + 0x0080     
#define UNIT_FIELD_AURALEVELS_14                                OBJECT_END + 0x0081     
#define UNIT_FIELD_AURAAPPLICATIONS                             OBJECT_END + 0x0082     
#define UNIT_FIELD_AURAAPPLICATIONS_2                           OBJECT_END + 0x0083     
#define UNIT_FIELD_AURAAPPLICATIONS_3                           OBJECT_END + 0x0084     
#define UNIT_FIELD_AURAAPPLICATIONS_4                           OBJECT_END + 0x0085     
#define UNIT_FIELD_AURAAPPLICATIONS_5                           OBJECT_END + 0x0086     
#define UNIT_FIELD_AURAAPPLICATIONS_6                           OBJECT_END + 0x0087     
#define UNIT_FIELD_AURAAPPLICATIONS_7                           OBJECT_END + 0x0088     
#define UNIT_FIELD_AURAAPPLICATIONS_8                           OBJECT_END + 0x0089     
#define UNIT_FIELD_AURAAPPLICATIONS_9                           OBJECT_END + 0x008A     
#define UNIT_FIELD_AURAAPPLICATIONS_10                          OBJECT_END + 0x008B     
#define UNIT_FIELD_AURAAPPLICATIONS_11                          OBJECT_END + 0x008C     
#define UNIT_FIELD_AURAAPPLICATIONS_12                          OBJECT_END + 0x008D     
#define UNIT_FIELD_AURAAPPLICATIONS_13                          OBJECT_END + 0x008E     
#define UNIT_FIELD_AURAAPPLICATIONS_14                          OBJECT_END + 0x008F     
#define UNIT_FIELD_AURASTATE                                    OBJECT_END + 0x0090     
#define UNIT_FIELD_BASEATTACKTIME                               OBJECT_END + 0x0091     
#define UNIT_FIELD_BASEATTACKTIME_2                             OBJECT_END + 0x0092     
#define UNIT_FIELD_RANGEDATTACKTIME                             OBJECT_END + 0x0093     
#define UNIT_FIELD_BOUNDINGRADIUS                               OBJECT_END + 0x0094     
#define UNIT_FIELD_COMBATREACH                                  OBJECT_END + 0x0095     
#define UNIT_FIELD_DISPLAYID                                    OBJECT_END + 0x0096     
#define UNIT_FIELD_NATIVEDISPLAYID                              OBJECT_END + 0x0097     
#define UNIT_FIELD_MOUNTDISPLAYID                               OBJECT_END + 0x0098     
#define UNIT_FIELD_MINDAMAGE                                    OBJECT_END + 0x0099     
#define UNIT_FIELD_MAXDAMAGE                                    OBJECT_END + 0x009A     
#define UNIT_FIELD_MINOFFHANDDAMAGE                             OBJECT_END + 0x009B     
#define UNIT_FIELD_MAXOFFHANDDAMAGE                             OBJECT_END + 0x009C     
#define UNIT_FIELD_BYTES_1                                      OBJECT_END + 0x009D     
#define UNIT_FIELD_PETNUMBER                                    OBJECT_END + 0x009E     
#define UNIT_FIELD_PET_NAME_TIMESTAMP                           OBJECT_END + 0x009F     
#define UNIT_FIELD_PETEXPERIENCE                                OBJECT_END + 0x00A0     
#define UNIT_FIELD_PETNEXTLEVELEXP                              OBJECT_END + 0x00A1     
#define UNIT_DYNAMIC_FLAGS                                      OBJECT_END + 0x00A2     
#define UNIT_CHANNEL_SPELL                                      OBJECT_END + 0x00A3     
#define UNIT_MOD_CAST_SPEED                                     OBJECT_END + 0x00A4     
#define UNIT_CREATED_BY_SPELL                                   OBJECT_END + 0x00A5     
#define UNIT_NPC_FLAGS                                          OBJECT_END + 0x00A6     
#define UNIT_NPC_EMOTESTATE                                     OBJECT_END + 0x00A7     
#define UNIT_TRAINING_POINTS                                    OBJECT_END + 0x00A8     
#define UNIT_FIELD_STAT0                                        OBJECT_END + 0x00A9     
#define UNIT_FIELD_STAT1                                        OBJECT_END + 0x00AA     
#define UNIT_FIELD_STAT2                                        OBJECT_END + 0x00AB     
#define UNIT_FIELD_STAT3                                        OBJECT_END + 0x00AC     
#define UNIT_FIELD_STAT4                                        OBJECT_END + 0x00AD     
#define UNIT_FIELD_POSSTAT0                                     OBJECT_END + 0x00AE     
#define UNIT_FIELD_POSSTAT1                                     OBJECT_END + 0x00AF     
#define UNIT_FIELD_POSSTAT2                                     OBJECT_END + 0x00B0     
#define UNIT_FIELD_POSSTAT3                                     OBJECT_END + 0x00B1     
#define UNIT_FIELD_POSSTAT4                                     OBJECT_END + 0x00B2     
#define UNIT_FIELD_NEGSTAT0                                     OBJECT_END + 0x00B3     
#define UNIT_FIELD_NEGSTAT1                                     OBJECT_END + 0x00B4     
#define UNIT_FIELD_NEGSTAT2                                     OBJECT_END + 0x00B5     
#define UNIT_FIELD_NEGSTAT3                                     OBJECT_END + 0x00B6     
#define UNIT_FIELD_NEGSTAT4                                     OBJECT_END + 0x00B7     
#define UNIT_FIELD_RESISTANCES                                  OBJECT_END + 0x00B8     
#define UNIT_FIELD_RESISTANCES_2                                OBJECT_END + 0x00B9     
#define UNIT_FIELD_RESISTANCES_3                                OBJECT_END + 0x00BA     
#define UNIT_FIELD_RESISTANCES_4                                OBJECT_END + 0x00BB     
#define UNIT_FIELD_RESISTANCES_5                                OBJECT_END + 0x00BC     
#define UNIT_FIELD_RESISTANCES_6                                OBJECT_END + 0x00BD     
#define UNIT_FIELD_RESISTANCES_7                                OBJECT_END + 0x00BE     
#define UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE                   OBJECT_END + 0x00BF     
#define UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE_2                 OBJECT_END + 0x00C0     
#define UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE_3                 OBJECT_END + 0x00C1     
#define UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE_4                 OBJECT_END + 0x00C2     
#define UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE_5                 OBJECT_END + 0x00C3     
#define UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE_6                 OBJECT_END + 0x00C4     
#define UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE_7                 OBJECT_END + 0x00C5     
#define UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE                   OBJECT_END + 0x00C6     
#define UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE_2                 OBJECT_END + 0x00C7     
#define UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE_3                 OBJECT_END + 0x00C8     
#define UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE_4                 OBJECT_END + 0x00C9     
#define UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE_5                 OBJECT_END + 0x00CA     
#define UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE_6                 OBJECT_END + 0x00CB     
#define UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE_7                 OBJECT_END + 0x00CC     
#define UNIT_FIELD_BASE_MANA                                    OBJECT_END + 0x00CD     
#define UNIT_FIELD_BASE_HEALTH                                  OBJECT_END + 0x00CE     
#define UNIT_FIELD_BYTES_2                                      OBJECT_END + 0x00CF     
#define UNIT_FIELD_ATTACK_POWER                                 OBJECT_END + 0x00D0     
#define UNIT_FIELD_ATTACK_POWER_MODS                            OBJECT_END + 0x00D1     
#define UNIT_FIELD_ATTACK_POWER_MULTIPLIER                      OBJECT_END + 0x00D2     
#define UNIT_FIELD_RANGED_ATTACK_POWER                          OBJECT_END + 0x00D3     
#define UNIT_FIELD_RANGED_ATTACK_POWER_MODS                     OBJECT_END + 0x00D4     
#define UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER               OBJECT_END + 0x00D5     
#define UNIT_FIELD_MINRANGEDDAMAGE                              OBJECT_END + 0x00D6     
#define UNIT_FIELD_MAXRANGEDDAMAGE                              OBJECT_END + 0x00D7     
#define UNIT_FIELD_POWER_COST_MODIFIER                          OBJECT_END + 0x00D8     
#define UNIT_FIELD_POWER_COST_MODIFIER_2                        OBJECT_END + 0x00D9     
#define UNIT_FIELD_POWER_COST_MODIFIER_3                        OBJECT_END + 0x00DA     
#define UNIT_FIELD_POWER_COST_MODIFIER_4                        OBJECT_END + 0x00DB     
#define UNIT_FIELD_POWER_COST_MODIFIER_5                        OBJECT_END + 0x00DC     
#define UNIT_FIELD_POWER_COST_MODIFIER_6                        OBJECT_END + 0x00DD     
#define UNIT_FIELD_POWER_COST_MODIFIER_7                        OBJECT_END + 0x00DE     
#define UNIT_FIELD_POWER_COST_MULTIPLIER                        OBJECT_END + 0x00DF     
#define UNIT_FIELD_POWER_COST_MULTIPLIER_2                      OBJECT_END + 0x00E0     
#define UNIT_FIELD_POWER_COST_MULTIPLIER_3                      OBJECT_END + 0x00E1     
#define UNIT_FIELD_POWER_COST_MULTIPLIER_4                      OBJECT_END + 0x00E2     
#define UNIT_FIELD_POWER_COST_MULTIPLIER_5                      OBJECT_END + 0x00E3     
#define UNIT_FIELD_POWER_COST_MULTIPLIER_6                      OBJECT_END + 0x00E4     
#define UNIT_FIELD_POWER_COST_MULTIPLIER_7                      OBJECT_END + 0x00E5     
#define UNIT_FIELD_MAXHEALTHMODIFIER                            OBJECT_END + 0x00E6     
#define UNIT_FIELD_PADDING                                      OBJECT_END + 0x00E7     
#define UNIT_FIELD_END                                          OBJECT_END + 0x00E8     

//PLAYER_FIELD_:
#define PLAYER_DUEL_ARBITER                                     UNIT_END + 0x0000     
#define PLAYER_DUEL_ARBITER_X                                   UNIT_END + 0x0001     
#define PLAYER_FLAGS                                            UNIT_END + 0x0002     
#define PLAYER_GUILDID                                          UNIT_END + 0x0003     
#define PLAYER_GUILDRANK                                        UNIT_END + 0x0004     
#define PLAYER_BYTES                                            UNIT_END + 0x0005     
#define PLAYER_BYTES_2                                          UNIT_END + 0x0006     
#define PLAYER_BYTES_3                                          UNIT_END + 0x0007     
#define PLAYER_DUEL_TEAM                                        UNIT_END + 0x0008     
#define PLAYER_GUILD_TIMESTAMP                                  UNIT_END + 0x0009     
#define PLAYER_QUEST_LOG_1_1                                    UNIT_END + 0x000A     
#define PLAYER_QUEST_LOG_1_2                                    UNIT_END + 0x000B     
#define PLAYER_QUEST_LOG_1_3                                    UNIT_END + 0x000C     
#define PLAYER_QUEST_LOG_1_4                                    UNIT_END + 0x000D     
#define PLAYER_QUEST_LOG_2_1                                    UNIT_END + 0x000E     
#define PLAYER_QUEST_LOG_2_2                                    UNIT_END + 0x000F     
#define PLAYER_QUEST_LOG_2_3                                    UNIT_END + 0x0010     
#define PLAYER_QUEST_LOG_2_4                                    UNIT_END + 0x0011     
#define PLAYER_QUEST_LOG_3_1                                    UNIT_END + 0x0012     
#define PLAYER_QUEST_LOG_3_2                                    UNIT_END + 0x0013     
#define PLAYER_QUEST_LOG_3_3                                    UNIT_END + 0x0014     
#define PLAYER_QUEST_LOG_3_4                                    UNIT_END + 0x0015     
#define PLAYER_QUEST_LOG_4_1                                    UNIT_END + 0x0016     
#define PLAYER_QUEST_LOG_4_2                                    UNIT_END + 0x0017     
#define PLAYER_QUEST_LOG_4_3                                    UNIT_END + 0x0018     
#define PLAYER_QUEST_LOG_4_4                                    UNIT_END + 0x0019     
#define PLAYER_QUEST_LOG_5_1                                    UNIT_END + 0x001A     
#define PLAYER_QUEST_LOG_5_2                                    UNIT_END + 0x001B     
#define PLAYER_QUEST_LOG_5_3                                    UNIT_END + 0x001C     
#define PLAYER_QUEST_LOG_5_4                                    UNIT_END + 0x001D     
#define PLAYER_QUEST_LOG_6_1                                    UNIT_END + 0x001E     
#define PLAYER_QUEST_LOG_6_2                                    UNIT_END + 0x001F     
#define PLAYER_QUEST_LOG_6_3                                    UNIT_END + 0x0020     
#define PLAYER_QUEST_LOG_6_4                                    UNIT_END + 0x0021     
#define PLAYER_QUEST_LOG_7_1                                    UNIT_END + 0x0022     
#define PLAYER_QUEST_LOG_7_2                                    UNIT_END + 0x0023     
#define PLAYER_QUEST_LOG_7_3                                    UNIT_END + 0x0024     
#define PLAYER_QUEST_LOG_7_4                                    UNIT_END + 0x0025     
#define PLAYER_QUEST_LOG_8_1                                    UNIT_END + 0x0026     
#define PLAYER_QUEST_LOG_8_2                                    UNIT_END + 0x0027     
#define PLAYER_QUEST_LOG_8_3                                    UNIT_END + 0x0028     
#define PLAYER_QUEST_LOG_8_4                                    UNIT_END + 0x0029     
#define PLAYER_QUEST_LOG_9_1                                    UNIT_END + 0x002A     
#define PLAYER_QUEST_LOG_9_2                                    UNIT_END + 0x002B     
#define PLAYER_QUEST_LOG_9_3                                    UNIT_END + 0x002C     
#define PLAYER_QUEST_LOG_9_4                                    UNIT_END + 0x002D     
#define PLAYER_QUEST_LOG_10_1                                   UNIT_END + 0x002E     
#define PLAYER_QUEST_LOG_10_2                                   UNIT_END + 0x002F     
#define PLAYER_QUEST_LOG_10_3                                   UNIT_END + 0x0030     
#define PLAYER_QUEST_LOG_10_4                                   UNIT_END + 0x0031     
#define PLAYER_QUEST_LOG_11_1                                   UNIT_END + 0x0032     
#define PLAYER_QUEST_LOG_11_2                                   UNIT_END + 0x0033     
#define PLAYER_QUEST_LOG_11_3                                   UNIT_END + 0x0034     
#define PLAYER_QUEST_LOG_11_4                                   UNIT_END + 0x0035     
#define PLAYER_QUEST_LOG_12_1                                   UNIT_END + 0x0036     
#define PLAYER_QUEST_LOG_12_2                                   UNIT_END + 0x0037     
#define PLAYER_QUEST_LOG_12_3                                   UNIT_END + 0x0038     
#define PLAYER_QUEST_LOG_12_4                                   UNIT_END + 0x0039     
#define PLAYER_QUEST_LOG_13_1                                   UNIT_END + 0x003A     
#define PLAYER_QUEST_LOG_13_2                                   UNIT_END + 0x003B     
#define PLAYER_QUEST_LOG_13_3                                   UNIT_END + 0x003C     
#define PLAYER_QUEST_LOG_13_4                                   UNIT_END + 0x003D     
#define PLAYER_QUEST_LOG_14_1                                   UNIT_END + 0x003E     
#define PLAYER_QUEST_LOG_14_2                                   UNIT_END + 0x003F     
#define PLAYER_QUEST_LOG_14_3                                   UNIT_END + 0x0040     
#define PLAYER_QUEST_LOG_14_4                                   UNIT_END + 0x0041     
#define PLAYER_QUEST_LOG_15_1                                   UNIT_END + 0x0042     
#define PLAYER_QUEST_LOG_15_2                                   UNIT_END + 0x0043     
#define PLAYER_QUEST_LOG_15_3                                   UNIT_END + 0x0044     
#define PLAYER_QUEST_LOG_15_4                                   UNIT_END + 0x0045     
#define PLAYER_QUEST_LOG_16_1                                   UNIT_END + 0x0046     
#define PLAYER_QUEST_LOG_16_2                                   UNIT_END + 0x0047     
#define PLAYER_QUEST_LOG_16_3                                   UNIT_END + 0x0048     
#define PLAYER_QUEST_LOG_16_4                                   UNIT_END + 0x0049     
#define PLAYER_QUEST_LOG_17_1                                   UNIT_END + 0x004A     
#define PLAYER_QUEST_LOG_17_2                                   UNIT_END + 0x004B     
#define PLAYER_QUEST_LOG_17_3                                   UNIT_END + 0x004C     
#define PLAYER_QUEST_LOG_17_4                                   UNIT_END + 0x004D     
#define PLAYER_QUEST_LOG_18_1                                   UNIT_END + 0x004E     
#define PLAYER_QUEST_LOG_18_2                                   UNIT_END + 0x004F     
#define PLAYER_QUEST_LOG_18_3                                   UNIT_END + 0x0050     
#define PLAYER_QUEST_LOG_18_4                                   UNIT_END + 0x0051     
#define PLAYER_QUEST_LOG_19_1                                   UNIT_END + 0x0052     
#define PLAYER_QUEST_LOG_19_2                                   UNIT_END + 0x0053     
#define PLAYER_QUEST_LOG_19_3                                   UNIT_END + 0x0054     
#define PLAYER_QUEST_LOG_19_4                                   UNIT_END + 0x0055     
#define PLAYER_QUEST_LOG_20_1                                   UNIT_END + 0x0056     
#define PLAYER_QUEST_LOG_20_2                                   UNIT_END + 0x0057     
#define PLAYER_QUEST_LOG_20_3                                   UNIT_END + 0x0058     
#define PLAYER_QUEST_LOG_20_4                                   UNIT_END + 0x0059     
#define PLAYER_QUEST_LOG_21_1                                   UNIT_END + 0x005A     
#define PLAYER_QUEST_LOG_21_2                                   UNIT_END + 0x005B     
#define PLAYER_QUEST_LOG_21_3                                   UNIT_END + 0x005C     
#define PLAYER_QUEST_LOG_21_4                                   UNIT_END + 0x005D     
#define PLAYER_QUEST_LOG_22_1                                   UNIT_END + 0x005E     
#define PLAYER_QUEST_LOG_22_2                                   UNIT_END + 0x005F     
#define PLAYER_QUEST_LOG_22_3                                   UNIT_END + 0x0060     
#define PLAYER_QUEST_LOG_22_4                                   UNIT_END + 0x0061     
#define PLAYER_QUEST_LOG_23_1                                   UNIT_END + 0x0062     
#define PLAYER_QUEST_LOG_23_2                                   UNIT_END + 0x0063     
#define PLAYER_QUEST_LOG_23_3                                   UNIT_END + 0x0064     
#define PLAYER_QUEST_LOG_23_4                                   UNIT_END + 0x0065     
#define PLAYER_QUEST_LOG_24_1                                   UNIT_END + 0x0066     
#define PLAYER_QUEST_LOG_24_2                                   UNIT_END + 0x0067     
#define PLAYER_QUEST_LOG_24_3                                   UNIT_END + 0x0068     
#define PLAYER_QUEST_LOG_24_4                                   UNIT_END + 0x0069     
#define PLAYER_QUEST_LOG_25_1                                   UNIT_END + 0x006A     
#define PLAYER_QUEST_LOG_25_2                                   UNIT_END + 0x006B     
#define PLAYER_QUEST_LOG_25_3                                   UNIT_END + 0x006C     
#define PLAYER_QUEST_LOG_25_4                                   UNIT_END + 0x006D     
#define PLAYER_VISIBLE_ITEM_1_CREATOR                           UNIT_END + 0x006E     
#define PLAYER_VISIBLE_ITEM_1_CREATOR_X                         UNIT_END + 0x006F     
#define PLAYER_VISIBLE_ITEM_1_0                                 UNIT_END + 0x0070     
#define PLAYER_VISIBLE_ITEM_1_1                                 UNIT_END + 0x0071     
#define PLAYER_VISIBLE_ITEM_1_2                                 UNIT_END + 0x0072     
#define PLAYER_VISIBLE_ITEM_1_3                                 UNIT_END + 0x0073     
#define PLAYER_VISIBLE_ITEM_1_4                                 UNIT_END + 0x0074     
#define PLAYER_VISIBLE_ITEM_1_5                                 UNIT_END + 0x0075     
#define PLAYER_VISIBLE_ITEM_1_6                                 UNIT_END + 0x0076     
#define PLAYER_VISIBLE_ITEM_1_7                                 UNIT_END + 0x0077     
#define PLAYER_VISIBLE_ITEM_1_8                                 UNIT_END + 0x0078     
#define PLAYER_VISIBLE_ITEM_1_9                                 UNIT_END + 0x0079     
#define PLAYER_VISIBLE_ITEM_1_10                                UNIT_END + 0x007A     
#define PLAYER_VISIBLE_ITEM_1_11                                UNIT_END + 0x007B     
#define PLAYER_VISIBLE_ITEM_1_12                                UNIT_END + 0x007C     
#define PLAYER_VISIBLE_ITEM_1_PROPERTIES                        UNIT_END + 0x007D     
#define PLAYER_VISIBLE_ITEM_1_SEED                              UNIT_END + 0x007E     
#define PLAYER_VISIBLE_ITEM_1_PAD                               UNIT_END + 0x007F     
#define PLAYER_VISIBLE_ITEM_2_CREATOR                           UNIT_END + 0x0080     
#define PLAYER_VISIBLE_ITEM_2_CREATOR_X                         UNIT_END + 0x0081     
#define PLAYER_VISIBLE_ITEM_2_0                                 UNIT_END + 0x0082     
#define PLAYER_VISIBLE_ITEM_2_1                                 UNIT_END + 0x0083     
#define PLAYER_VISIBLE_ITEM_2_2                                 UNIT_END + 0x0084     
#define PLAYER_VISIBLE_ITEM_2_3                                 UNIT_END + 0x0085     
#define PLAYER_VISIBLE_ITEM_2_4                                 UNIT_END + 0x0086     
#define PLAYER_VISIBLE_ITEM_2_5                                 UNIT_END + 0x0087     
#define PLAYER_VISIBLE_ITEM_2_6                                 UNIT_END + 0x0088     
#define PLAYER_VISIBLE_ITEM_2_7                                 UNIT_END + 0x0089     
#define PLAYER_VISIBLE_ITEM_2_8                                 UNIT_END + 0x008A     
#define PLAYER_VISIBLE_ITEM_2_9                                 UNIT_END + 0x008B     
#define PLAYER_VISIBLE_ITEM_2_10                                UNIT_END + 0x008C     
#define PLAYER_VISIBLE_ITEM_2_11                                UNIT_END + 0x008D     
#define PLAYER_VISIBLE_ITEM_2_12                                UNIT_END + 0x008E     
#define PLAYER_VISIBLE_ITEM_2_PROPERTIES                        UNIT_END + 0x008F     
#define PLAYER_VISIBLE_ITEM_2_SEED                              UNIT_END + 0x0090     
#define PLAYER_VISIBLE_ITEM_2_PAD                               UNIT_END + 0x0091     
#define PLAYER_VISIBLE_ITEM_3_CREATOR                           UNIT_END + 0x0092     
#define PLAYER_VISIBLE_ITEM_3_CREATOR_X                         UNIT_END + 0x0093     
#define PLAYER_VISIBLE_ITEM_3_0                                 UNIT_END + 0x0094     
#define PLAYER_VISIBLE_ITEM_3_1                                 UNIT_END + 0x0095     
#define PLAYER_VISIBLE_ITEM_3_2                                 UNIT_END + 0x0096     
#define PLAYER_VISIBLE_ITEM_3_3                                 UNIT_END + 0x0097     
#define PLAYER_VISIBLE_ITEM_3_4                                 UNIT_END + 0x0098     
#define PLAYER_VISIBLE_ITEM_3_5                                 UNIT_END + 0x0099     
#define PLAYER_VISIBLE_ITEM_3_6                                 UNIT_END + 0x009A     
#define PLAYER_VISIBLE_ITEM_3_7                                 UNIT_END + 0x009B     
#define PLAYER_VISIBLE_ITEM_3_8                                 UNIT_END + 0x009C     
#define PLAYER_VISIBLE_ITEM_3_9                                 UNIT_END + 0x009D     
#define PLAYER_VISIBLE_ITEM_3_10                                UNIT_END + 0x009E     
#define PLAYER_VISIBLE_ITEM_3_11                                UNIT_END + 0x009F     
#define PLAYER_VISIBLE_ITEM_3_12                                UNIT_END + 0x00A0     
#define PLAYER_VISIBLE_ITEM_3_PROPERTIES                        UNIT_END + 0x00A1     
#define PLAYER_VISIBLE_ITEM_3_SEED                              UNIT_END + 0x00A2     
#define PLAYER_VISIBLE_ITEM_3_PAD                               UNIT_END + 0x00A3     
#define PLAYER_VISIBLE_ITEM_4_CREATOR                           UNIT_END + 0x00A4     
#define PLAYER_VISIBLE_ITEM_4_CREATOR_X                         UNIT_END + 0x00A5     
#define PLAYER_VISIBLE_ITEM_4_0                                 UNIT_END + 0x00A6     
#define PLAYER_VISIBLE_ITEM_4_1                                 UNIT_END + 0x00A7     
#define PLAYER_VISIBLE_ITEM_4_2                                 UNIT_END + 0x00A8     
#define PLAYER_VISIBLE_ITEM_4_3                                 UNIT_END + 0x00A9     
#define PLAYER_VISIBLE_ITEM_4_4                                 UNIT_END + 0x00AA     
#define PLAYER_VISIBLE_ITEM_4_5                                 UNIT_END + 0x00AB     
#define PLAYER_VISIBLE_ITEM_4_6                                 UNIT_END + 0x00AC     
#define PLAYER_VISIBLE_ITEM_4_7                                 UNIT_END + 0x00AD     
#define PLAYER_VISIBLE_ITEM_4_8                                 UNIT_END + 0x00AE     
#define PLAYER_VISIBLE_ITEM_4_9                                 UNIT_END + 0x00AF     
#define PLAYER_VISIBLE_ITEM_4_10                                UNIT_END + 0x00B0     
#define PLAYER_VISIBLE_ITEM_4_11                                UNIT_END + 0x00B1     
#define PLAYER_VISIBLE_ITEM_4_12                                UNIT_END + 0x00B2     
#define PLAYER_VISIBLE_ITEM_4_PROPERTIES                        UNIT_END + 0x00B3     
#define PLAYER_VISIBLE_ITEM_4_SEED                              UNIT_END + 0x00B4     
#define PLAYER_VISIBLE_ITEM_4_PAD                               UNIT_END + 0x00B5     
#define PLAYER_VISIBLE_ITEM_5_CREATOR                           UNIT_END + 0x00B6     
#define PLAYER_VISIBLE_ITEM_5_CREATOR_X                         UNIT_END + 0x00B7     
#define PLAYER_VISIBLE_ITEM_5_0                                 UNIT_END + 0x00B8     
#define PLAYER_VISIBLE_ITEM_5_1                                 UNIT_END + 0x00B9     
#define PLAYER_VISIBLE_ITEM_5_2                                 UNIT_END + 0x00BA     
#define PLAYER_VISIBLE_ITEM_5_3                                 UNIT_END + 0x00BB     
#define PLAYER_VISIBLE_ITEM_5_4                                 UNIT_END + 0x00BC     
#define PLAYER_VISIBLE_ITEM_5_5                                 UNIT_END + 0x00BD     
#define PLAYER_VISIBLE_ITEM_5_6                                 UNIT_END + 0x00BE     
#define PLAYER_VISIBLE_ITEM_5_7                                 UNIT_END + 0x00BF     
#define PLAYER_VISIBLE_ITEM_5_8                                 UNIT_END + 0x00C0     
#define PLAYER_VISIBLE_ITEM_5_9                                 UNIT_END + 0x00C1     
#define PLAYER_VISIBLE_ITEM_5_10                                UNIT_END + 0x00C2     
#define PLAYER_VISIBLE_ITEM_5_11                                UNIT_END + 0x00C3     
#define PLAYER_VISIBLE_ITEM_5_12                                UNIT_END + 0x00C4     
#define PLAYER_VISIBLE_ITEM_5_PROPERTIES                        UNIT_END + 0x00C5     
#define PLAYER_VISIBLE_ITEM_5_SEED                              UNIT_END + 0x00C6     
#define PLAYER_VISIBLE_ITEM_5_PAD                               UNIT_END + 0x00C7     
#define PLAYER_VISIBLE_ITEM_6_CREATOR                           UNIT_END + 0x00C8     
#define PLAYER_VISIBLE_ITEM_6_CREATOR_X                         UNIT_END + 0x00C9     
#define PLAYER_VISIBLE_ITEM_6_0                                 UNIT_END + 0x00CA     
#define PLAYER_VISIBLE_ITEM_6_1                                 UNIT_END + 0x00CB     
#define PLAYER_VISIBLE_ITEM_6_2                                 UNIT_END + 0x00CC     
#define PLAYER_VISIBLE_ITEM_6_3                                 UNIT_END + 0x00CD     
#define PLAYER_VISIBLE_ITEM_6_4                                 UNIT_END + 0x00CE     
#define PLAYER_VISIBLE_ITEM_6_5                                 UNIT_END + 0x00CF     
#define PLAYER_VISIBLE_ITEM_6_6                                 UNIT_END + 0x00D0     
#define PLAYER_VISIBLE_ITEM_6_7                                 UNIT_END + 0x00D1     
#define PLAYER_VISIBLE_ITEM_6_8                                 UNIT_END + 0x00D2     
#define PLAYER_VISIBLE_ITEM_6_9                                 UNIT_END + 0x00D3     
#define PLAYER_VISIBLE_ITEM_6_10                                UNIT_END + 0x00D4     
#define PLAYER_VISIBLE_ITEM_6_11                                UNIT_END + 0x00D5     
#define PLAYER_VISIBLE_ITEM_6_12                                UNIT_END + 0x00D6     
#define PLAYER_VISIBLE_ITEM_6_PROPERTIES                        UNIT_END + 0x00D7     
#define PLAYER_VISIBLE_ITEM_6_SEED                              UNIT_END + 0x00D8     
#define PLAYER_VISIBLE_ITEM_6_PAD                               UNIT_END + 0x00D9     
#define PLAYER_VISIBLE_ITEM_7_CREATOR                           UNIT_END + 0x00DA     
#define PLAYER_VISIBLE_ITEM_7_CREATOR_X                         UNIT_END + 0x00DB     
#define PLAYER_VISIBLE_ITEM_7_0                                 UNIT_END + 0x00DC     
#define PLAYER_VISIBLE_ITEM_7_1                                 UNIT_END + 0x00DD     
#define PLAYER_VISIBLE_ITEM_7_2                                 UNIT_END + 0x00DE     
#define PLAYER_VISIBLE_ITEM_7_3                                 UNIT_END + 0x00DF     
#define PLAYER_VISIBLE_ITEM_7_4                                 UNIT_END + 0x00E0     
#define PLAYER_VISIBLE_ITEM_7_5                                 UNIT_END + 0x00E1     
#define PLAYER_VISIBLE_ITEM_7_6                                 UNIT_END + 0x00E2     
#define PLAYER_VISIBLE_ITEM_7_7                                 UNIT_END + 0x00E3     
#define PLAYER_VISIBLE_ITEM_7_8                                 UNIT_END + 0x00E4     
#define PLAYER_VISIBLE_ITEM_7_9                                 UNIT_END + 0x00E5     
#define PLAYER_VISIBLE_ITEM_7_10                                UNIT_END + 0x00E6     
#define PLAYER_VISIBLE_ITEM_7_11                                UNIT_END + 0x00E7     
#define PLAYER_VISIBLE_ITEM_7_12                                UNIT_END + 0x00E8     
#define PLAYER_VISIBLE_ITEM_7_PROPERTIES                        UNIT_END + 0x00E9     
#define PLAYER_VISIBLE_ITEM_7_SEED                              UNIT_END + 0x00EA     
#define PLAYER_VISIBLE_ITEM_7_PAD                               UNIT_END + 0x00EB     
#define PLAYER_VISIBLE_ITEM_8_CREATOR                           UNIT_END + 0x00EC     
#define PLAYER_VISIBLE_ITEM_8_CREATOR_X                         UNIT_END + 0x00ED     
#define PLAYER_VISIBLE_ITEM_8_0                                 UNIT_END + 0x00EE     
#define PLAYER_VISIBLE_ITEM_8_1                                 UNIT_END + 0x00EF     
#define PLAYER_VISIBLE_ITEM_8_2                                 UNIT_END + 0x00F0     
#define PLAYER_VISIBLE_ITEM_8_3                                 UNIT_END + 0x00F1     
#define PLAYER_VISIBLE_ITEM_8_4                                 UNIT_END + 0x00F2     
#define PLAYER_VISIBLE_ITEM_8_5                                 UNIT_END + 0x00F3     
#define PLAYER_VISIBLE_ITEM_8_6                                 UNIT_END + 0x00F4     
#define PLAYER_VISIBLE_ITEM_8_7                                 UNIT_END + 0x00F5     
#define PLAYER_VISIBLE_ITEM_8_8                                 UNIT_END + 0x00F6     
#define PLAYER_VISIBLE_ITEM_8_9                                 UNIT_END + 0x00F7     
#define PLAYER_VISIBLE_ITEM_8_10                                UNIT_END + 0x00F8     
#define PLAYER_VISIBLE_ITEM_8_11                                UNIT_END + 0x00F9     
#define PLAYER_VISIBLE_ITEM_8_12                                UNIT_END + 0x00FA     
#define PLAYER_VISIBLE_ITEM_8_PROPERTIES                        UNIT_END + 0x00FB     
#define PLAYER_VISIBLE_ITEM_8_SEED                              UNIT_END + 0x00FC     
#define PLAYER_VISIBLE_ITEM_8_PAD                               UNIT_END + 0x00FD     
#define PLAYER_VISIBLE_ITEM_9_CREATOR                           UNIT_END + 0x00FE     
#define PLAYER_VISIBLE_ITEM_9_CREATOR_X                         UNIT_END + 0x00FF     
#define PLAYER_VISIBLE_ITEM_9_0                                 UNIT_END + 0x0100     
#define PLAYER_VISIBLE_ITEM_9_1                                 UNIT_END + 0x0101     
#define PLAYER_VISIBLE_ITEM_9_2                                 UNIT_END + 0x0102     
#define PLAYER_VISIBLE_ITEM_9_3                                 UNIT_END + 0x0103     
#define PLAYER_VISIBLE_ITEM_9_4                                 UNIT_END + 0x0104     
#define PLAYER_VISIBLE_ITEM_9_5                                 UNIT_END + 0x0105     
#define PLAYER_VISIBLE_ITEM_9_6                                 UNIT_END + 0x0106     
#define PLAYER_VISIBLE_ITEM_9_7                                 UNIT_END + 0x0107     
#define PLAYER_VISIBLE_ITEM_9_8                                 UNIT_END + 0x0108     
#define PLAYER_VISIBLE_ITEM_9_9                                 UNIT_END + 0x0109     
#define PLAYER_VISIBLE_ITEM_9_10                                UNIT_END + 0x010A     
#define PLAYER_VISIBLE_ITEM_9_11                                UNIT_END + 0x010B     
#define PLAYER_VISIBLE_ITEM_9_12                                UNIT_END + 0x010C     
#define PLAYER_VISIBLE_ITEM_9_PROPERTIES                        UNIT_END + 0x010D     
#define PLAYER_VISIBLE_ITEM_9_SEED                              UNIT_END + 0x010E     
#define PLAYER_VISIBLE_ITEM_9_PAD                               UNIT_END + 0x010F     
#define PLAYER_VISIBLE_ITEM_10_CREATOR                          UNIT_END + 0x0110     
#define PLAYER_VISIBLE_ITEM_10_CREATOR_X                        UNIT_END + 0x0111     
#define PLAYER_VISIBLE_ITEM_10_0                                UNIT_END + 0x0112     
#define PLAYER_VISIBLE_ITEM_10_1                                UNIT_END + 0x0113     
#define PLAYER_VISIBLE_ITEM_10_2                                UNIT_END + 0x0114     
#define PLAYER_VISIBLE_ITEM_10_3                                UNIT_END + 0x0115     
#define PLAYER_VISIBLE_ITEM_10_4                                UNIT_END + 0x0116     
#define PLAYER_VISIBLE_ITEM_10_5                                UNIT_END + 0x0117     
#define PLAYER_VISIBLE_ITEM_10_6                                UNIT_END + 0x0118     
#define PLAYER_VISIBLE_ITEM_10_7                                UNIT_END + 0x0119     
#define PLAYER_VISIBLE_ITEM_10_8                                UNIT_END + 0x011A     
#define PLAYER_VISIBLE_ITEM_10_9                                UNIT_END + 0x011B     
#define PLAYER_VISIBLE_ITEM_10_10                               UNIT_END + 0x011C     
#define PLAYER_VISIBLE_ITEM_10_11                               UNIT_END + 0x011D     
#define PLAYER_VISIBLE_ITEM_10_12                               UNIT_END + 0x011E     
#define PLAYER_VISIBLE_ITEM_10_PROPERTIES                       UNIT_END + 0x011F     
#define PLAYER_VISIBLE_ITEM_10_SEED                             UNIT_END + 0x0120     
#define PLAYER_VISIBLE_ITEM_10_PAD                              UNIT_END + 0x0121     
#define PLAYER_VISIBLE_ITEM_11_CREATOR                          UNIT_END + 0x0122     
#define PLAYER_VISIBLE_ITEM_11_CREATOR_X                        UNIT_END + 0x0123     
#define PLAYER_VISIBLE_ITEM_11_0                                UNIT_END + 0x0124     
#define PLAYER_VISIBLE_ITEM_11_1                                UNIT_END + 0x0125     
#define PLAYER_VISIBLE_ITEM_11_2                                UNIT_END + 0x0126     
#define PLAYER_VISIBLE_ITEM_11_3                                UNIT_END + 0x0127     
#define PLAYER_VISIBLE_ITEM_11_4                                UNIT_END + 0x0128     
#define PLAYER_VISIBLE_ITEM_11_5                                UNIT_END + 0x0129     
#define PLAYER_VISIBLE_ITEM_11_6                                UNIT_END + 0x012A     
#define PLAYER_VISIBLE_ITEM_11_7                                UNIT_END + 0x012B     
#define PLAYER_VISIBLE_ITEM_11_8                                UNIT_END + 0x012C     
#define PLAYER_VISIBLE_ITEM_11_9                                UNIT_END + 0x012D     
#define PLAYER_VISIBLE_ITEM_11_10                               UNIT_END + 0x012E     
#define PLAYER_VISIBLE_ITEM_11_11                               UNIT_END + 0x012F     
#define PLAYER_VISIBLE_ITEM_11_12                               UNIT_END + 0x0130     
#define PLAYER_VISIBLE_ITEM_11_PROPERTIES                       UNIT_END + 0x0131     
#define PLAYER_VISIBLE_ITEM_11_SEED                             UNIT_END + 0x0132     
#define PLAYER_VISIBLE_ITEM_11_PAD                              UNIT_END + 0x0133     
#define PLAYER_VISIBLE_ITEM_12_CREATOR                          UNIT_END + 0x0134     
#define PLAYER_VISIBLE_ITEM_12_CREATOR_X                        UNIT_END + 0x0135     
#define PLAYER_VISIBLE_ITEM_12_0                                UNIT_END + 0x0136     
#define PLAYER_VISIBLE_ITEM_12_1                                UNIT_END + 0x0137     
#define PLAYER_VISIBLE_ITEM_12_2                                UNIT_END + 0x0138     
#define PLAYER_VISIBLE_ITEM_12_3                                UNIT_END + 0x0139     
#define PLAYER_VISIBLE_ITEM_12_4                                UNIT_END + 0x013A     
#define PLAYER_VISIBLE_ITEM_12_5                                UNIT_END + 0x013B     
#define PLAYER_VISIBLE_ITEM_12_6                                UNIT_END + 0x013C     
#define PLAYER_VISIBLE_ITEM_12_7                                UNIT_END + 0x013D     
#define PLAYER_VISIBLE_ITEM_12_8                                UNIT_END + 0x013E     
#define PLAYER_VISIBLE_ITEM_12_9                                UNIT_END + 0x013F     
#define PLAYER_VISIBLE_ITEM_12_10                               UNIT_END + 0x0140     
#define PLAYER_VISIBLE_ITEM_12_11                               UNIT_END + 0x0141     
#define PLAYER_VISIBLE_ITEM_12_12                               UNIT_END + 0x0142     
#define PLAYER_VISIBLE_ITEM_12_PROPERTIES                       UNIT_END + 0x0143     
#define PLAYER_VISIBLE_ITEM_12_SEED                             UNIT_END + 0x0144     
#define PLAYER_VISIBLE_ITEM_12_PAD                              UNIT_END + 0x0145     
#define PLAYER_VISIBLE_ITEM_13_CREATOR                          UNIT_END + 0x0146     
#define PLAYER_VISIBLE_ITEM_13_CREATOR_X                        UNIT_END + 0x0147     
#define PLAYER_VISIBLE_ITEM_13_0                                UNIT_END + 0x0148     
#define PLAYER_VISIBLE_ITEM_13_1                                UNIT_END + 0x0149     
#define PLAYER_VISIBLE_ITEM_13_2                                UNIT_END + 0x014A     
#define PLAYER_VISIBLE_ITEM_13_3                                UNIT_END + 0x014B     
#define PLAYER_VISIBLE_ITEM_13_4                                UNIT_END + 0x014C     
#define PLAYER_VISIBLE_ITEM_13_5                                UNIT_END + 0x014D     
#define PLAYER_VISIBLE_ITEM_13_6                                UNIT_END + 0x014E     
#define PLAYER_VISIBLE_ITEM_13_7                                UNIT_END + 0x014F     
#define PLAYER_VISIBLE_ITEM_13_8                                UNIT_END + 0x0150     
#define PLAYER_VISIBLE_ITEM_13_9                                UNIT_END + 0x0151     
#define PLAYER_VISIBLE_ITEM_13_10                               UNIT_END + 0x0152     
#define PLAYER_VISIBLE_ITEM_13_11                               UNIT_END + 0x0153     
#define PLAYER_VISIBLE_ITEM_13_12                               UNIT_END + 0x0154     
#define PLAYER_VISIBLE_ITEM_13_PROPERTIES                       UNIT_END + 0x0155     
#define PLAYER_VISIBLE_ITEM_13_SEED                             UNIT_END + 0x0156     
#define PLAYER_VISIBLE_ITEM_13_PAD                              UNIT_END + 0x0157     
#define PLAYER_VISIBLE_ITEM_14_CREATOR                          UNIT_END + 0x0158     
#define PLAYER_VISIBLE_ITEM_14_CREATOR_X                        UNIT_END + 0x0159     
#define PLAYER_VISIBLE_ITEM_14_0                                UNIT_END + 0x015A     
#define PLAYER_VISIBLE_ITEM_14_1                                UNIT_END + 0x015B     
#define PLAYER_VISIBLE_ITEM_14_2                                UNIT_END + 0x015C     
#define PLAYER_VISIBLE_ITEM_14_3                                UNIT_END + 0x015D     
#define PLAYER_VISIBLE_ITEM_14_4                                UNIT_END + 0x015E     
#define PLAYER_VISIBLE_ITEM_14_5                                UNIT_END + 0x015F     
#define PLAYER_VISIBLE_ITEM_14_6                                UNIT_END + 0x0160     
#define PLAYER_VISIBLE_ITEM_14_7                                UNIT_END + 0x0161     
#define PLAYER_VISIBLE_ITEM_14_8                                UNIT_END + 0x0162     
#define PLAYER_VISIBLE_ITEM_14_9                                UNIT_END + 0x0163     
#define PLAYER_VISIBLE_ITEM_14_10                               UNIT_END + 0x0164     
#define PLAYER_VISIBLE_ITEM_14_11                               UNIT_END + 0x0165     
#define PLAYER_VISIBLE_ITEM_14_12                               UNIT_END + 0x0166     
#define PLAYER_VISIBLE_ITEM_14_PROPERTIES                       UNIT_END + 0x0167     
#define PLAYER_VISIBLE_ITEM_14_SEED                             UNIT_END + 0x0168     
#define PLAYER_VISIBLE_ITEM_14_PAD                              UNIT_END + 0x0169     
#define PLAYER_VISIBLE_ITEM_15_CREATOR                          UNIT_END + 0x016A     
#define PLAYER_VISIBLE_ITEM_15_CREATOR_X                        UNIT_END + 0x016B     
#define PLAYER_VISIBLE_ITEM_15_0                                UNIT_END + 0x016C     
#define PLAYER_VISIBLE_ITEM_15_1                                UNIT_END + 0x016D     
#define PLAYER_VISIBLE_ITEM_15_2                                UNIT_END + 0x016E     
#define PLAYER_VISIBLE_ITEM_15_3                                UNIT_END + 0x016F     
#define PLAYER_VISIBLE_ITEM_15_4                                UNIT_END + 0x0170     
#define PLAYER_VISIBLE_ITEM_15_5                                UNIT_END + 0x0171     
#define PLAYER_VISIBLE_ITEM_15_6                                UNIT_END + 0x0172     
#define PLAYER_VISIBLE_ITEM_15_7                                UNIT_END + 0x0173     
#define PLAYER_VISIBLE_ITEM_15_8                                UNIT_END + 0x0174     
#define PLAYER_VISIBLE_ITEM_15_9                                UNIT_END + 0x0175     
#define PLAYER_VISIBLE_ITEM_15_10                               UNIT_END + 0x0176     
#define PLAYER_VISIBLE_ITEM_15_11                               UNIT_END + 0x0177     
#define PLAYER_VISIBLE_ITEM_15_12                               UNIT_END + 0x0178     
#define PLAYER_VISIBLE_ITEM_15_PROPERTIES                       UNIT_END + 0x0179     
#define PLAYER_VISIBLE_ITEM_15_SEED                             UNIT_END + 0x017A     
#define PLAYER_VISIBLE_ITEM_15_PAD                              UNIT_END + 0x017B     
#define PLAYER_VISIBLE_ITEM_16_CREATOR                          UNIT_END + 0x017C     
#define PLAYER_VISIBLE_ITEM_16_CREATOR_X                        UNIT_END + 0x017D     
#define PLAYER_VISIBLE_ITEM_16_0                                UNIT_END + 0x017E     
#define PLAYER_VISIBLE_ITEM_16_1                                UNIT_END + 0x017F     
#define PLAYER_VISIBLE_ITEM_16_2                                UNIT_END + 0x0180     
#define PLAYER_VISIBLE_ITEM_16_3                                UNIT_END + 0x0181     
#define PLAYER_VISIBLE_ITEM_16_4                                UNIT_END + 0x0182     
#define PLAYER_VISIBLE_ITEM_16_5                                UNIT_END + 0x0183     
#define PLAYER_VISIBLE_ITEM_16_6                                UNIT_END + 0x0184     
#define PLAYER_VISIBLE_ITEM_16_7                                UNIT_END + 0x0185     
#define PLAYER_VISIBLE_ITEM_16_8                                UNIT_END + 0x0186     
#define PLAYER_VISIBLE_ITEM_16_9                                UNIT_END + 0x0187     
#define PLAYER_VISIBLE_ITEM_16_10                               UNIT_END + 0x0188     
#define PLAYER_VISIBLE_ITEM_16_11                               UNIT_END + 0x0189     
#define PLAYER_VISIBLE_ITEM_16_12                               UNIT_END + 0x018A     
#define PLAYER_VISIBLE_ITEM_16_PROPERTIES                       UNIT_END + 0x018B     
#define PLAYER_VISIBLE_ITEM_16_SEED                             UNIT_END + 0x018C     
#define PLAYER_VISIBLE_ITEM_16_PAD                              UNIT_END + 0x018D     
#define PLAYER_VISIBLE_ITEM_17_CREATOR                          UNIT_END + 0x018E     
#define PLAYER_VISIBLE_ITEM_17_CREATOR_X                        UNIT_END + 0x018F     
#define PLAYER_VISIBLE_ITEM_17_0                                UNIT_END + 0x0190     
#define PLAYER_VISIBLE_ITEM_17_1                                UNIT_END + 0x0191     
#define PLAYER_VISIBLE_ITEM_17_2                                UNIT_END + 0x0192     
#define PLAYER_VISIBLE_ITEM_17_3                                UNIT_END + 0x0193     
#define PLAYER_VISIBLE_ITEM_17_4                                UNIT_END + 0x0194     
#define PLAYER_VISIBLE_ITEM_17_5                                UNIT_END + 0x0195     
#define PLAYER_VISIBLE_ITEM_17_6                                UNIT_END + 0x0196     
#define PLAYER_VISIBLE_ITEM_17_7                                UNIT_END + 0x0197     
#define PLAYER_VISIBLE_ITEM_17_8                                UNIT_END + 0x0198     
#define PLAYER_VISIBLE_ITEM_17_9                                UNIT_END + 0x0199     
#define PLAYER_VISIBLE_ITEM_17_10                               UNIT_END + 0x019A     
#define PLAYER_VISIBLE_ITEM_17_11                               UNIT_END + 0x019B     
#define PLAYER_VISIBLE_ITEM_17_12                               UNIT_END + 0x019C     
#define PLAYER_VISIBLE_ITEM_17_PROPERTIES                       UNIT_END + 0x019D     
#define PLAYER_VISIBLE_ITEM_17_SEED                             UNIT_END + 0x019E     
#define PLAYER_VISIBLE_ITEM_17_PAD                              UNIT_END + 0x019F     
#define PLAYER_VISIBLE_ITEM_18_CREATOR                          UNIT_END + 0x01A0     
#define PLAYER_VISIBLE_ITEM_18_CREATOR_X                        UNIT_END + 0x01A1     
#define PLAYER_VISIBLE_ITEM_18_0                                UNIT_END + 0x01A2     
#define PLAYER_VISIBLE_ITEM_18_1                                UNIT_END + 0x01A3     
#define PLAYER_VISIBLE_ITEM_18_2                                UNIT_END + 0x01A4     
#define PLAYER_VISIBLE_ITEM_18_3                                UNIT_END + 0x01A5     
#define PLAYER_VISIBLE_ITEM_18_4                                UNIT_END + 0x01A6     
#define PLAYER_VISIBLE_ITEM_18_5                                UNIT_END + 0x01A7     
#define PLAYER_VISIBLE_ITEM_18_6                                UNIT_END + 0x01A8     
#define PLAYER_VISIBLE_ITEM_18_7                                UNIT_END + 0x01A9     
#define PLAYER_VISIBLE_ITEM_18_8                                UNIT_END + 0x01AA     
#define PLAYER_VISIBLE_ITEM_18_9                                UNIT_END + 0x01AB     
#define PLAYER_VISIBLE_ITEM_18_10                               UNIT_END + 0x01AC     
#define PLAYER_VISIBLE_ITEM_18_11                               UNIT_END + 0x01AD     
#define PLAYER_VISIBLE_ITEM_18_12                               UNIT_END + 0x01AE     
#define PLAYER_VISIBLE_ITEM_18_PROPERTIES                       UNIT_END + 0x01AF     
#define PLAYER_VISIBLE_ITEM_18_SEED                             UNIT_END + 0x01B0     
#define PLAYER_VISIBLE_ITEM_18_PAD                              UNIT_END + 0x01B1     
#define PLAYER_VISIBLE_ITEM_19_CREATOR                          UNIT_END + 0x01B2     
#define PLAYER_VISIBLE_ITEM_19_CREATOR_X                        UNIT_END + 0x01B3     
#define PLAYER_VISIBLE_ITEM_19_0                                UNIT_END + 0x01B4     
#define PLAYER_VISIBLE_ITEM_19_1                                UNIT_END + 0x01B5     
#define PLAYER_VISIBLE_ITEM_19_2                                UNIT_END + 0x01B6     
#define PLAYER_VISIBLE_ITEM_19_3                                UNIT_END + 0x01B7     
#define PLAYER_VISIBLE_ITEM_19_4                                UNIT_END + 0x01B8     
#define PLAYER_VISIBLE_ITEM_19_5                                UNIT_END + 0x01B9     
#define PLAYER_VISIBLE_ITEM_19_6                                UNIT_END + 0x01BA     
#define PLAYER_VISIBLE_ITEM_19_7                                UNIT_END + 0x01BB     
#define PLAYER_VISIBLE_ITEM_19_8                                UNIT_END + 0x01BC     
#define PLAYER_VISIBLE_ITEM_19_9                                UNIT_END + 0x01BD     
#define PLAYER_VISIBLE_ITEM_19_10                               UNIT_END + 0x01BE     
#define PLAYER_VISIBLE_ITEM_19_11                               UNIT_END + 0x01BF     
#define PLAYER_VISIBLE_ITEM_19_12                               UNIT_END + 0x01C0     
#define PLAYER_VISIBLE_ITEM_19_PROPERTIES                       UNIT_END + 0x01C1     
#define PLAYER_VISIBLE_ITEM_19_SEED                             UNIT_END + 0x01C2     
#define PLAYER_VISIBLE_ITEM_19_PAD                              UNIT_END + 0x01C3     
#define PLAYER_CHOSEN_TITLE                                     UNIT_END + 0x01C4     
#define PLAYER_FIELD_PAD_0                                      UNIT_END + 0x01C5     
#define PLAYER_FIELD_INV_SLOT_HEAD                              UNIT_END + 0x01C6     
#define PLAYER_FIELD_INV_SLOT_HEADX                             UNIT_END + 0x01C7     
#define PLAYER_FIELD_INV_SLOT_NECK                              UNIT_END + 0x01C8     
#define PLAYER_FIELD_INV_SLOT_NECKX                             UNIT_END + 0x01C9     
#define PLAYER_FIELD_INV_SLOT_SHOULDER                          UNIT_END + 0x01CA     
#define PLAYER_FIELD_INV_SLOT_SHOULDERX                         UNIT_END + 0x01CB     
#define PLAYER_FIELD_INV_SLOT_BODY                              UNIT_END + 0x01CC     
#define PLAYER_FIELD_INV_SLOT_BODYX                             UNIT_END + 0x01CD     
#define PLAYER_FIELD_INV_SLOT_CHEST                             UNIT_END + 0x01CE     
#define PLAYER_FIELD_INV_SLOT_CHESTX                            UNIT_END + 0x01CF     
#define PLAYER_FIELD_INV_SLOT_WAIST                             UNIT_END + 0x01D0     
#define PLAYER_FIELD_INV_SLOT_WAISTX                            UNIT_END + 0x01D1     
#define PLAYER_FIELD_INV_SLOT_LEGS                              UNIT_END + 0x01D2     
#define PLAYER_FIELD_INV_SLOT_LEGSX                             UNIT_END + 0x01D3     
#define PLAYER_FIELD_INV_SLOT_FEET                              UNIT_END + 0x01D4     
#define PLAYER_FIELD_INV_SLOT_FEETX                             UNIT_END + 0x01D5     
#define PLAYER_FIELD_INV_SLOT_WRIST                             UNIT_END + 0x01D6     
#define PLAYER_FIELD_INV_SLOT_WRISTX                            UNIT_END + 0x01D7     
#define PLAYER_FIELD_INV_SLOT_HAND                              UNIT_END + 0x01D8     
#define PLAYER_FIELD_INV_SLOT_HANDX                             UNIT_END + 0x01D9     
#define PLAYER_FIELD_INV_SLOT_FINGER1                           UNIT_END + 0x01DA     
#define PLAYER_FIELD_INV_SLOT_FINGER1X                          UNIT_END + 0x01DB     
#define PLAYER_FIELD_INV_SLOT_FINGER2                           UNIT_END + 0x01DC     
#define PLAYER_FIELD_INV_SLOT_FINGER2X                          UNIT_END + 0x01DD     
#define PLAYER_FIELD_INV_SLOT_TRINKET1                          UNIT_END + 0x01DE     
#define PLAYER_FIELD_INV_SLOT_TRINKET1X                         UNIT_END + 0x01DF     
#define PLAYER_FIELD_INV_SLOT_TRINKET2                          UNIT_END + 0x01E0     
#define PLAYER_FIELD_INV_SLOT_TRINKET2X                         UNIT_END + 0x01E1     
#define PLAYER_FIELD_INV_SLOT_BACK                              UNIT_END + 0x01E2     
#define PLAYER_FIELD_INV_SLOT_BACKX                             UNIT_END + 0x01E3     
#define PLAYER_FIELD_INV_SLOT_MAINHAND                          UNIT_END + 0x01E4     
#define PLAYER_FIELD_INV_SLOT_MAINHANDX                         UNIT_END + 0x01E5     
#define PLAYER_FIELD_INV_SLOT_OFFHAND                           UNIT_END + 0x01E6     
#define PLAYER_FIELD_INV_SLOT_OFFHANDX                          UNIT_END + 0x01E7     
#define PLAYER_FIELD_INV_SLOT_RANGED                            UNIT_END + 0x01E8     
#define PLAYER_FIELD_INV_SLOT_RANGEDX                           UNIT_END + 0x01E9     
#define PLAYER_FIELD_INV_SLOT_TABARD                            UNIT_END + 0x01EA     
#define PLAYER_FIELD_INV_SLOT_TABARDX                           UNIT_END + 0x01EB     
#define PLAYER_FIELD_INV_SLOT_BAG0                              UNIT_END + 0x01EC     
#define PLAYER_FIELD_INV_SLOT_BAG0X                             UNIT_END + 0x01ED     
#define PLAYER_FIELD_INV_SLOT_BAG1                              UNIT_END + 0x01EE     
#define PLAYER_FIELD_INV_SLOT_BAG1X                             UNIT_END + 0x01EF     
#define PLAYER_FIELD_INV_SLOT_BAG2                              UNIT_END + 0x01F0     
#define PLAYER_FIELD_INV_SLOT_BAG2X                             UNIT_END + 0x01F1     
#define PLAYER_FIELD_INV_SLOT_BAG3                              UNIT_END + 0x01F2     
#define PLAYER_FIELD_INV_SLOT_BAG3X                             UNIT_END + 0x01F3     
#define PLAYER_FIELD_PACK_SLOT_1                                UNIT_END + 0x01F4     
#define PLAYER_FIELD_PACK_SLOT_1X                               UNIT_END + 0x01F5     
#define PLAYER_FIELD_PACK_SLOT_2                                UNIT_END + 0x01F6     
#define PLAYER_FIELD_PACK_SLOT_2X                               UNIT_END + 0x01F7     
#define PLAYER_FIELD_PACK_SLOT_3                                UNIT_END + 0x01F8     
#define PLAYER_FIELD_PACK_SLOT_3X                               UNIT_END + 0x01F9     
#define PLAYER_FIELD_PACK_SLOT_4                                UNIT_END + 0x01FA     
#define PLAYER_FIELD_PACK_SLOT_4X                               UNIT_END + 0x01FB     
#define PLAYER_FIELD_PACK_SLOT_5                                UNIT_END + 0x01FC     
#define PLAYER_FIELD_PACK_SLOT_5X                               UNIT_END + 0x01FD     
#define PLAYER_FIELD_PACK_SLOT_6                                UNIT_END + 0x01FE     
#define PLAYER_FIELD_PACK_SLOT_6X                               UNIT_END + 0x01FF     
#define PLAYER_FIELD_PACK_SLOT_7                                UNIT_END + 0x0200     
#define PLAYER_FIELD_PACK_SLOT_7X                               UNIT_END + 0x0201     
#define PLAYER_FIELD_PACK_SLOT_8                                UNIT_END + 0x0202     
#define PLAYER_FIELD_PACK_SLOT_8X                               UNIT_END + 0x0203     
#define PLAYER_FIELD_PACK_SLOT_9                                UNIT_END + 0x0204     
#define PLAYER_FIELD_PACK_SLOT_9X                               UNIT_END + 0x0205     
#define PLAYER_FIELD_PACK_SLOT_10                               UNIT_END + 0x0206     
#define PLAYER_FIELD_PACK_SLOT_10X                              UNIT_END + 0x0207     
#define PLAYER_FIELD_PACK_SLOT_11                               UNIT_END + 0x0208     
#define PLAYER_FIELD_PACK_SLOT_11X                              UNIT_END + 0x0209     
#define PLAYER_FIELD_PACK_SLOT_12                               UNIT_END + 0x020A     
#define PLAYER_FIELD_PACK_SLOT_12X                              UNIT_END + 0x020B     
#define PLAYER_FIELD_PACK_SLOT_13                               UNIT_END + 0x020C     
#define PLAYER_FIELD_PACK_SLOT_13X                              UNIT_END + 0x020D     
#define PLAYER_FIELD_PACK_SLOT_14                               UNIT_END + 0x020E     
#define PLAYER_FIELD_PACK_SLOT_14X                              UNIT_END + 0x020F     
#define PLAYER_FIELD_PACK_SLOT_15                               UNIT_END + 0x0210     
#define PLAYER_FIELD_PACK_SLOT_15X                              UNIT_END + 0x0211     
#define PLAYER_FIELD_PACK_SLOT_16                               UNIT_END + 0x0212     
#define PLAYER_FIELD_PACK_SLOT_16X                              UNIT_END + 0x0213     
#define PLAYER_FIELD_BANK_SLOT_1                                UNIT_END + 0x0214     
#define PLAYER_FIELD_BANK_SLOT_1X                               UNIT_END + 0x0215     
#define PLAYER_FIELD_BANK_SLOT_2                                UNIT_END + 0x0216     
#define PLAYER_FIELD_BANK_SLOT_2X                               UNIT_END + 0x0217     
#define PLAYER_FIELD_BANK_SLOT_3                                UNIT_END + 0x0218     
#define PLAYER_FIELD_BANK_SLOT_3X                               UNIT_END + 0x0219     
#define PLAYER_FIELD_BANK_SLOT_4                                UNIT_END + 0x021A     
#define PLAYER_FIELD_BANK_SLOT_4X                               UNIT_END + 0x021B     
#define PLAYER_FIELD_BANK_SLOT_5                                UNIT_END + 0x021C     
#define PLAYER_FIELD_BANK_SLOT_5X                               UNIT_END + 0x021D     
#define PLAYER_FIELD_BANK_SLOT_6                                UNIT_END + 0x021E     
#define PLAYER_FIELD_BANK_SLOT_6X                               UNIT_END + 0x021F     
#define PLAYER_FIELD_BANK_SLOT_7                                UNIT_END + 0x0220     
#define PLAYER_FIELD_BANK_SLOT_7X                               UNIT_END + 0x0221     
#define PLAYER_FIELD_BANK_SLOT_8                                UNIT_END + 0x0222     
#define PLAYER_FIELD_BANK_SLOT_8X                               UNIT_END + 0x0223     
#define PLAYER_FIELD_BANK_SLOT_9                                UNIT_END + 0x0224     
#define PLAYER_FIELD_BANK_SLOT_9X                               UNIT_END + 0x0225     
#define PLAYER_FIELD_BANK_SLOT_10                               UNIT_END + 0x0226     
#define PLAYER_FIELD_BANK_SLOT_10X                              UNIT_END + 0x0227     
#define PLAYER_FIELD_BANK_SLOT_11                               UNIT_END + 0x0228     
#define PLAYER_FIELD_BANK_SLOT_11X                              UNIT_END + 0x0229     
#define PLAYER_FIELD_BANK_SLOT_12                               UNIT_END + 0x022A     
#define PLAYER_FIELD_BANK_SLOT_12X                              UNIT_END + 0x022B     
#define PLAYER_FIELD_BANK_SLOT_13                               UNIT_END + 0x022C     
#define PLAYER_FIELD_BANK_SLOT_13X                              UNIT_END + 0x022D     
#define PLAYER_FIELD_BANK_SLOT_14                               UNIT_END + 0x022E     
#define PLAYER_FIELD_BANK_SLOT_14X                              UNIT_END + 0x022F     
#define PLAYER_FIELD_BANK_SLOT_15                               UNIT_END + 0x0230     
#define PLAYER_FIELD_BANK_SLOT_15X                              UNIT_END + 0x0231     
#define PLAYER_FIELD_BANK_SLOT_16                               UNIT_END + 0x0232     
#define PLAYER_FIELD_BANK_SLOT_16X                              UNIT_END + 0x0233     
#define PLAYER_FIELD_BANK_SLOT_17                               UNIT_END + 0x0234     
#define PLAYER_FIELD_BANK_SLOT_17X                              UNIT_END + 0x0235     
#define PLAYER_FIELD_BANK_SLOT_18                               UNIT_END + 0x0236     
#define PLAYER_FIELD_BANK_SLOT_18X                              UNIT_END + 0x0237     
#define PLAYER_FIELD_BANK_SLOT_19                               UNIT_END + 0x0238     
#define PLAYER_FIELD_BANK_SLOT_19X                              UNIT_END + 0x0239     
#define PLAYER_FIELD_BANK_SLOT_20                               UNIT_END + 0x023A     
#define PLAYER_FIELD_BANK_SLOT_20X                              UNIT_END + 0x023B     
#define PLAYER_FIELD_BANK_SLOT_21                               UNIT_END + 0x023C     
#define PLAYER_FIELD_BANK_SLOT_21X                              UNIT_END + 0x023D     
#define PLAYER_FIELD_BANK_SLOT_22                               UNIT_END + 0x023E     
#define PLAYER_FIELD_BANK_SLOT_22X                              UNIT_END + 0x023F     
#define PLAYER_FIELD_BANK_SLOT_23                               UNIT_END + 0x0240     
#define PLAYER_FIELD_BANK_SLOT_23X                              UNIT_END + 0x0241     
#define PLAYER_FIELD_BANK_SLOT_24                               UNIT_END + 0x0242     
#define PLAYER_FIELD_BANK_SLOT_24X                              UNIT_END + 0x0243     
#define PLAYER_FIELD_BANK_SLOT_25                               UNIT_END + 0x0244     
#define PLAYER_FIELD_BANK_SLOT_25X                              UNIT_END + 0x0245     
#define PLAYER_FIELD_BANK_SLOT_26                               UNIT_END + 0x0246     
#define PLAYER_FIELD_BANK_SLOT_26X                              UNIT_END + 0x0247     
#define PLAYER_FIELD_BANK_SLOT_27                               UNIT_END + 0x0248     
#define PLAYER_FIELD_BANK_SLOT_27X                              UNIT_END + 0x0249     
#define PLAYER_FIELD_BANK_SLOT_28                               UNIT_END + 0x024A     
#define PLAYER_FIELD_BANK_SLOT_28X                              UNIT_END + 0x024B     
#define PLAYER_FIELD_BANKBAG_SLOT_1                             UNIT_END + 0x024C     
#define PLAYER_FIELD_BANKBAG_SLOT_1X                            UNIT_END + 0x024D     
#define PLAYER_FIELD_BANKBAG_SLOT_2                             UNIT_END + 0x024E     
#define PLAYER_FIELD_BANKBAG_SLOT_2X                            UNIT_END + 0x024F     
#define PLAYER_FIELD_BANKBAG_SLOT_3                             UNIT_END + 0x0250     
#define PLAYER_FIELD_BANKBAG_SLOT_3X                            UNIT_END + 0x0251     
#define PLAYER_FIELD_BANKBAG_SLOT_4                             UNIT_END + 0x0252     
#define PLAYER_FIELD_BANKBAG_SLOT_4X                            UNIT_END + 0x0253     
#define PLAYER_FIELD_BANKBAG_SLOT_5                             UNIT_END + 0x0254     
#define PLAYER_FIELD_BANKBAG_SLOT_5X                            UNIT_END + 0x0255     
#define PLAYER_FIELD_BANKBAG_SLOT_6                             UNIT_END + 0x0256     
#define PLAYER_FIELD_BANKBAG_SLOT_6X                            UNIT_END + 0x0257     
#define PLAYER_FIELD_BANKBAG_SLOT_7                             UNIT_END + 0x0258     
#define PLAYER_FIELD_BANKBAG_SLOT_7X                            UNIT_END + 0x0259     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_1                       UNIT_END + 0x025A     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_1X                      UNIT_END + 0x025B     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_2                       UNIT_END + 0x025C     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_X2                      UNIT_END + 0x025D     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_3                       UNIT_END + 0x025E     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_X3                      UNIT_END + 0x025F     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_4                       UNIT_END + 0x0260     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_X4                      UNIT_END + 0x0261     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_5                       UNIT_END + 0x0262     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_X5                      UNIT_END + 0x0263     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_6                       UNIT_END + 0x0264     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_X6                      UNIT_END + 0x0265     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_7                       UNIT_END + 0x0266     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_X7                      UNIT_END + 0x0267     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_8                       UNIT_END + 0x0268     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_X8                      UNIT_END + 0x0269     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_9                       UNIT_END + 0x026A     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_X9                      UNIT_END + 0x026B     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_10                      UNIT_END + 0x026C     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_X10                     UNIT_END + 0x026D     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_11                      UNIT_END + 0x026E     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_X11                     UNIT_END + 0x026F     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_12                      UNIT_END + 0x0270     
#define PLAYER_FIELD_VENDORBUYBACK_SLOT_X12                     UNIT_END + 0x0271     
#define PLAYER_FIELD_KEYRING_SLOT_1                             UNIT_END + 0x0272     
#define PLAYER_FIELD_KEYRING_SLOT_1X                            UNIT_END + 0x0273     
#define PLAYER_FIELD_KEYRING_SLOT_2                             UNIT_END + 0x0274     
#define PLAYER_FIELD_KEYRING_SLOT_X2                            UNIT_END + 0x0275     
#define PLAYER_FIELD_KEYRING_SLOT_3                             UNIT_END + 0x0276     
#define PLAYER_FIELD_KEYRING_SLOT_X3                            UNIT_END + 0x0277     
#define PLAYER_FIELD_KEYRING_SLOT_4                             UNIT_END + 0x0278     
#define PLAYER_FIELD_KEYRING_SLOT_X4                            UNIT_END + 0x0279     
#define PLAYER_FIELD_KEYRING_SLOT_5                             UNIT_END + 0x027A     
#define PLAYER_FIELD_KEYRING_SLOT_X5                            UNIT_END + 0x027B     
#define PLAYER_FIELD_KEYRING_SLOT_6                             UNIT_END + 0x027C     
#define PLAYER_FIELD_KEYRING_SLOT_X6                            UNIT_END + 0x027D     
#define PLAYER_FIELD_KEYRING_SLOT_7                             UNIT_END + 0x027E     
#define PLAYER_FIELD_KEYRING_SLOT_X7                            UNIT_END + 0x027F     
#define PLAYER_FIELD_KEYRING_SLOT_8                             UNIT_END + 0x0280     
#define PLAYER_FIELD_KEYRING_SLOT_X8                            UNIT_END + 0x0281     
#define PLAYER_FIELD_KEYRING_SLOT_9                             UNIT_END + 0x0282     
#define PLAYER_FIELD_KEYRING_SLOT_X9                            UNIT_END + 0x0283     
#define PLAYER_FIELD_KEYRING_SLOT_10                            UNIT_END + 0x0284     
#define PLAYER_FIELD_KEYRING_SLOT_X10                           UNIT_END + 0x0285     
#define PLAYER_FIELD_KEYRING_SLOT_11                            UNIT_END + 0x0286     
#define PLAYER_FIELD_KEYRING_SLOT_X11                           UNIT_END + 0x0287     
#define PLAYER_FIELD_KEYRING_SLOT_12                            UNIT_END + 0x0288     
#define PLAYER_FIELD_KEYRING_SLOT_X12                           UNIT_END + 0x0289     
#define PLAYER_FIELD_KEYRING_SLOT_13                            UNIT_END + 0x028A     
#define PLAYER_FIELD_KEYRING_SLOT_X13                           UNIT_END + 0x028B     
#define PLAYER_FIELD_KEYRING_SLOT_14                            UNIT_END + 0x028C     
#define PLAYER_FIELD_KEYRING_SLOT_X14                           UNIT_END + 0x028D     
#define PLAYER_FIELD_KEYRING_SLOT_15                            UNIT_END + 0x028E     
#define PLAYER_FIELD_KEYRING_SLOT_X15                           UNIT_END + 0x028F     
#define PLAYER_FIELD_KEYRING_SLOT_16                            UNIT_END + 0x0290     
#define PLAYER_FIELD_KEYRING_SLOT_X16                           UNIT_END + 0x0291     
#define PLAYER_FIELD_KEYRING_SLOT_17                            UNIT_END + 0x0292     
#define PLAYER_FIELD_KEYRING_SLOT_X17                           UNIT_END + 0x0293     
#define PLAYER_FIELD_KEYRING_SLOT_18                            UNIT_END + 0x0294     
#define PLAYER_FIELD_KEYRING_SLOT_X18                           UNIT_END + 0x0295     
#define PLAYER_FIELD_KEYRING_SLOT_19                            UNIT_END + 0x0296     
#define PLAYER_FIELD_KEYRING_SLOT_X19                           UNIT_END + 0x0297     
#define PLAYER_FIELD_KEYRING_SLOT_20                            UNIT_END + 0x0298     
#define PLAYER_FIELD_KEYRING_SLOT_X20                           UNIT_END + 0x0299     
#define PLAYER_FIELD_KEYRING_SLOT_21                            UNIT_END + 0x029A     
#define PLAYER_FIELD_KEYRING_SLOT_X21                           UNIT_END + 0x029B     
#define PLAYER_FIELD_KEYRING_SLOT_22                            UNIT_END + 0x029C     
#define PLAYER_FIELD_KEYRING_SLOT_X22                           UNIT_END + 0x029D     
#define PLAYER_FIELD_KEYRING_SLOT_23                            UNIT_END + 0x029E     
#define PLAYER_FIELD_KEYRING_SLOT_X23                           UNIT_END + 0x029F     
#define PLAYER_FIELD_KEYRING_SLOT_24                            UNIT_END + 0x02A0     
#define PLAYER_FIELD_KEYRING_SLOT_X24                           UNIT_END + 0x02A1     
#define PLAYER_FIELD_KEYRING_SLOT_25                            UNIT_END + 0x02A2     
#define PLAYER_FIELD_KEYRING_SLOT_X25                           UNIT_END + 0x02A3     
#define PLAYER_FIELD_KEYRING_SLOT_26                            UNIT_END + 0x02A4     
#define PLAYER_FIELD_KEYRING_SLOT_X26                           UNIT_END + 0x02A5     
#define PLAYER_FIELD_KEYRING_SLOT_27                            UNIT_END + 0x02A6     
#define PLAYER_FIELD_KEYRING_SLOT_X27                           UNIT_END + 0x02A7     
#define PLAYER_FIELD_KEYRING_SLOT_28                            UNIT_END + 0x02A8     
#define PLAYER_FIELD_KEYRING_SLOT_X28                           UNIT_END + 0x02A9     
#define PLAYER_FIELD_KEYRING_SLOT_29                            UNIT_END + 0x02AA     
#define PLAYER_FIELD_KEYRING_SLOT_X29                           UNIT_END + 0x02AB     
#define PLAYER_FIELD_KEYRING_SLOT_30                            UNIT_END + 0x02AC     
#define PLAYER_FIELD_KEYRING_SLOT_X30                           UNIT_END + 0x02AD     
#define PLAYER_FIELD_KEYRING_SLOT_31                            UNIT_END + 0x02AE     
#define PLAYER_FIELD_KEYRING_SLOT_X31                           UNIT_END + 0x02AF     
#define PLAYER_FIELD_KEYRING_SLOT_32                            UNIT_END + 0x02B0     
#define PLAYER_FIELD_KEYRING_SLOT_X32                           UNIT_END + 0x02B1     
#define PLAYER_FIELD_VANITYPET_SLOT_1                           UNIT_END + 0x02B2     
#define PLAYER_FIELD_VANITYPET_SLOT_1X                          UNIT_END + 0x02B3     
#define PLAYER_FIELD_VANITYPET_SLOT_2                           UNIT_END + 0x02B4     
#define PLAYER_FIELD_VANITYPET_SLOT_X2                          UNIT_END + 0x02B5     
#define PLAYER_FIELD_VANITYPET_SLOT_3                           UNIT_END + 0x02B6     
#define PLAYER_FIELD_VANITYPET_SLOT_X3                          UNIT_END + 0x02B7     
#define PLAYER_FIELD_VANITYPET_SLOT_4                           UNIT_END + 0x02B8     
#define PLAYER_FIELD_VANITYPET_SLOT_X4                          UNIT_END + 0x02B9     
#define PLAYER_FIELD_VANITYPET_SLOT_5                           UNIT_END + 0x02BA     
#define PLAYER_FIELD_VANITYPET_SLOT_X5                          UNIT_END + 0x02BB     
#define PLAYER_FIELD_VANITYPET_SLOT_6                           UNIT_END + 0x02BC     
#define PLAYER_FIELD_VANITYPET_SLOT_X6                          UNIT_END + 0x02BD     
#define PLAYER_FIELD_VANITYPET_SLOT_7                           UNIT_END + 0x02BE     
#define PLAYER_FIELD_VANITYPET_SLOT_X7                          UNIT_END + 0x02BF     
#define PLAYER_FIELD_VANITYPET_SLOT_8                           UNIT_END + 0x02C0     
#define PLAYER_FIELD_VANITYPET_SLOT_X8                          UNIT_END + 0x02C1     
#define PLAYER_FIELD_VANITYPET_SLOT_9                           UNIT_END + 0x02C2     
#define PLAYER_FIELD_VANITYPET_SLOT_X9                          UNIT_END + 0x02C3     
#define PLAYER_FIELD_VANITYPET_SLOT_10                          UNIT_END + 0x02C4     
#define PLAYER_FIELD_VANITYPET_SLOT_X10                         UNIT_END + 0x02C5     
#define PLAYER_FIELD_VANITYPET_SLOT_11                          UNIT_END + 0x02C6     
#define PLAYER_FIELD_VANITYPET_SLOT_X11                         UNIT_END + 0x02C7     
#define PLAYER_FIELD_VANITYPET_SLOT_12                          UNIT_END + 0x02C8     
#define PLAYER_FIELD_VANITYPET_SLOT_X12                         UNIT_END + 0x02C9     
#define PLAYER_FIELD_VANITYPET_SLOT_13                          UNIT_END + 0x02CA     
#define PLAYER_FIELD_VANITYPET_SLOT_X13                         UNIT_END + 0x02CB     
#define PLAYER_FIELD_VANITYPET_SLOT_14                          UNIT_END + 0x02CC     
#define PLAYER_FIELD_VANITYPET_SLOT_X14                         UNIT_END + 0x02CD     
#define PLAYER_FIELD_VANITYPET_SLOT_15                          UNIT_END + 0x02CE     
#define PLAYER_FIELD_VANITYPET_SLOT_X15                         UNIT_END + 0x02CF     
#define PLAYER_FIELD_VANITYPET_SLOT_16                          UNIT_END + 0x02D0     
#define PLAYER_FIELD_VANITYPET_SLOT_X16                         UNIT_END + 0x02D1     
#define PLAYER_FIELD_VANITYPET_SLOT_17                          UNIT_END + 0x02D2     
#define PLAYER_FIELD_VANITYPET_SLOT_X17                         UNIT_END + 0x02D3     
#define PLAYER_FIELD_VANITYPET_SLOT_18                          UNIT_END + 0x02D4     
#define PLAYER_FIELD_VANITYPET_SLOT_X18                         UNIT_END + 0x02D5     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_1                       UNIT_END + 0x02D6     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_1X                      UNIT_END + 0x02D7     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_2                       UNIT_END + 0x02D8     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X2                      UNIT_END + 0x02D9     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_3                       UNIT_END + 0x02DA     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X3                      UNIT_END + 0x02DB     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_4                       UNIT_END + 0x02DC     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X4                      UNIT_END + 0x02DD     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_5                       UNIT_END + 0x02DE     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X5                      UNIT_END + 0x02DF     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_6                       UNIT_END + 0x02E0     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X6                      UNIT_END + 0x02E1     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_7                       UNIT_END + 0x02E2     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X7                      UNIT_END + 0x02E3     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_8                       UNIT_END + 0x02E4     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X8                      UNIT_END + 0x02E5     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_9                       UNIT_END + 0x02E6     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X9                      UNIT_END + 0x02E7     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_10                      UNIT_END + 0x02E8     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X10                     UNIT_END + 0x02E9     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_11                      UNIT_END + 0x02EA     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X11                     UNIT_END + 0x02EB     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_12                      UNIT_END + 0x02EC     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X12                     UNIT_END + 0x02ED     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_13                      UNIT_END + 0x02EE     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X13                     UNIT_END + 0x02EF     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_14                      UNIT_END + 0x02F0     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X14                     UNIT_END + 0x02F1     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_15                      UNIT_END + 0x02F2     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X15                     UNIT_END + 0x02F3     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_16                      UNIT_END + 0x02F4     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X16                     UNIT_END + 0x02F5     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_17                      UNIT_END + 0x02F6     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X17                     UNIT_END + 0x02F7     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_18                      UNIT_END + 0x02F8     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X18                     UNIT_END + 0x02F9     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_19                      UNIT_END + 0x02FA     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X19                     UNIT_END + 0x02FB     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_20                      UNIT_END + 0x02FC     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X20                     UNIT_END + 0x02FD     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_21                      UNIT_END + 0x02FE     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X21                     UNIT_END + 0x02FF     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_22                      UNIT_END + 0x0300     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X22                     UNIT_END + 0x0301     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_23                      UNIT_END + 0x0302     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X23                     UNIT_END + 0x0303     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_24                      UNIT_END + 0x0304     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X24                     UNIT_END + 0x0305     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_25                      UNIT_END + 0x0306     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X25                     UNIT_END + 0x0307     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_26                      UNIT_END + 0x0308     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X26                     UNIT_END + 0x0309     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_27                      UNIT_END + 0x030A     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X27                     UNIT_END + 0x030B     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_28                      UNIT_END + 0x030C     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X28                     UNIT_END + 0x030D     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_29                      UNIT_END + 0x030E     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X29                     UNIT_END + 0x030F     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_30                      UNIT_END + 0x0310     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X30                     UNIT_END + 0x0311     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_31                      UNIT_END + 0x0312     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X31                     UNIT_END + 0x0313     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_32                      UNIT_END + 0x0314     
#define PLAYER_FIELD_CURRENCYTOKEN_SLOT_X32                     UNIT_END + 0x0315     
#define PLAYER_FARSIGHT                                         UNIT_END + 0x0316     
#define PLAYER_FARSIGHT_X                                       UNIT_END + 0x0317     
#define PLAYER_FIELD_KNOWN_TITLES                              UNIT_END + 0x0318     
#define PLAYER_FIELD_KNOWN_TITLES_X                            UNIT_END + 0x0319     
#define PLAYER_XP                                               UNIT_END + 0x031A     
#define PLAYER_NEXT_LEVEL_XP                                    UNIT_END + 0x031B     
#define PLAYER_SKILL_INFO_1_1                                   UNIT_END + 0x031C     
#define PLAYER_SKILL_INFO_1_2                                   UNIT_END + 0x031D     
#define PLAYER_SKILL_INFO_1_3                                   UNIT_END + 0x031E     
#define PLAYER_SKILL_INFO_2_1                                   UNIT_END + 0x031F     
#define PLAYER_SKILL_INFO_2_2                                   UNIT_END + 0x0320     
#define PLAYER_SKILL_INFO_2_3                                   UNIT_END + 0x0321     
#define PLAYER_SKILL_INFO_3_1                                   UNIT_END + 0x0322     
#define PLAYER_SKILL_INFO_3_2                                   UNIT_END + 0x0323     
#define PLAYER_SKILL_INFO_3_3                                   UNIT_END + 0x0324     
#define PLAYER_SKILL_INFO_4_1                                   UNIT_END + 0x0325     
#define PLAYER_SKILL_INFO_4_2                                   UNIT_END + 0x0326     
#define PLAYER_SKILL_INFO_4_3                                   UNIT_END + 0x0327     
#define PLAYER_SKILL_INFO_5_1                                   UNIT_END + 0x0328     
#define PLAYER_SKILL_INFO_5_2                                   UNIT_END + 0x0329     
#define PLAYER_SKILL_INFO_5_3                                   UNIT_END + 0x032A     
#define PLAYER_SKILL_INFO_6_1                                   UNIT_END + 0x032B     
#define PLAYER_SKILL_INFO_6_2                                   UNIT_END + 0x032C     
#define PLAYER_SKILL_INFO_6_3                                   UNIT_END + 0x032D     
#define PLAYER_SKILL_INFO_7_1                                   UNIT_END + 0x032E     
#define PLAYER_SKILL_INFO_7_2                                   UNIT_END + 0x032F     
#define PLAYER_SKILL_INFO_7_3                                   UNIT_END + 0x0330     
#define PLAYER_SKILL_INFO_8_1                                   UNIT_END + 0x0331     
#define PLAYER_SKILL_INFO_8_2                                   UNIT_END + 0x0332     
#define PLAYER_SKILL_INFO_8_3                                   UNIT_END + 0x0333     
#define PLAYER_SKILL_INFO_9_1                                   UNIT_END + 0x0334     
#define PLAYER_SKILL_INFO_9_2                                   UNIT_END + 0x0335     
#define PLAYER_SKILL_INFO_9_3                                   UNIT_END + 0x0336     
#define PLAYER_SKILL_INFO_10_1                                  UNIT_END + 0x0337     
#define PLAYER_SKILL_INFO_10_2                                  UNIT_END + 0x0338     
#define PLAYER_SKILL_INFO_10_3                                  UNIT_END + 0x0339     
#define PLAYER_SKILL_INFO_11_1                                  UNIT_END + 0x033A     
#define PLAYER_SKILL_INFO_11_2                                  UNIT_END + 0x033B     
#define PLAYER_SKILL_INFO_11_3                                  UNIT_END + 0x033C     
#define PLAYER_SKILL_INFO_12_1                                  UNIT_END + 0x033D     
#define PLAYER_SKILL_INFO_12_2                                  UNIT_END + 0x033E     
#define PLAYER_SKILL_INFO_12_3                                  UNIT_END + 0x033F     
#define PLAYER_SKILL_INFO_13_1                                  UNIT_END + 0x0340     
#define PLAYER_SKILL_INFO_13_2                                  UNIT_END + 0x0341     
#define PLAYER_SKILL_INFO_13_3                                  UNIT_END + 0x0342     
#define PLAYER_SKILL_INFO_14_1                                  UNIT_END + 0x0343     
#define PLAYER_SKILL_INFO_14_2                                  UNIT_END + 0x0344     
#define PLAYER_SKILL_INFO_14_3                                  UNIT_END + 0x0345     
#define PLAYER_SKILL_INFO_15_1                                  UNIT_END + 0x0346     
#define PLAYER_SKILL_INFO_15_2                                  UNIT_END + 0x0347     
#define PLAYER_SKILL_INFO_15_3                                  UNIT_END + 0x0348     
#define PLAYER_SKILL_INFO_16_1                                  UNIT_END + 0x0349     
#define PLAYER_SKILL_INFO_16_2                                  UNIT_END + 0x034A     
#define PLAYER_SKILL_INFO_16_3                                  UNIT_END + 0x034B     
#define PLAYER_SKILL_INFO_17_1                                  UNIT_END + 0x034C     
#define PLAYER_SKILL_INFO_17_2                                  UNIT_END + 0x034D     
#define PLAYER_SKILL_INFO_17_3                                  UNIT_END + 0x034E     
#define PLAYER_SKILL_INFO_18_1                                  UNIT_END + 0x034F     
#define PLAYER_SKILL_INFO_18_2                                  UNIT_END + 0x0350     
#define PLAYER_SKILL_INFO_18_3                                  UNIT_END + 0x0351     
#define PLAYER_SKILL_INFO_19_1                                  UNIT_END + 0x0352     
#define PLAYER_SKILL_INFO_19_2                                  UNIT_END + 0x0353     
#define PLAYER_SKILL_INFO_19_3                                  UNIT_END + 0x0354     
#define PLAYER_SKILL_INFO_20_1                                  UNIT_END + 0x0355     
#define PLAYER_SKILL_INFO_20_2                                  UNIT_END + 0x0356     
#define PLAYER_SKILL_INFO_20_3                                  UNIT_END + 0x0357     
#define PLAYER_SKILL_INFO_21_1                                  UNIT_END + 0x0358     
#define PLAYER_SKILL_INFO_21_2                                  UNIT_END + 0x0359     
#define PLAYER_SKILL_INFO_21_3                                  UNIT_END + 0x035A     
#define PLAYER_SKILL_INFO_22_1                                  UNIT_END + 0x035B     
#define PLAYER_SKILL_INFO_22_2                                  UNIT_END + 0x035C     
#define PLAYER_SKILL_INFO_22_3                                  UNIT_END + 0x035D     
#define PLAYER_SKILL_INFO_23_1                                  UNIT_END + 0x035E     
#define PLAYER_SKILL_INFO_23_2                                  UNIT_END + 0x035F     
#define PLAYER_SKILL_INFO_23_3                                  UNIT_END + 0x0360     
#define PLAYER_SKILL_INFO_24_1                                  UNIT_END + 0x0361     
#define PLAYER_SKILL_INFO_24_2                                  UNIT_END + 0x0362     
#define PLAYER_SKILL_INFO_24_3                                  UNIT_END + 0x0363     
#define PLAYER_SKILL_INFO_25_1                                  UNIT_END + 0x0364     
#define PLAYER_SKILL_INFO_25_2                                  UNIT_END + 0x0365     
#define PLAYER_SKILL_INFO_25_3                                  UNIT_END + 0x0366     
#define PLAYER_SKILL_INFO_26_1                                  UNIT_END + 0x0367     
#define PLAYER_SKILL_INFO_26_2                                  UNIT_END + 0x0368     
#define PLAYER_SKILL_INFO_26_3                                  UNIT_END + 0x0369     
#define PLAYER_SKILL_INFO_27_1                                  UNIT_END + 0x036A     
#define PLAYER_SKILL_INFO_27_2                                  UNIT_END + 0x036B     
#define PLAYER_SKILL_INFO_27_3                                  UNIT_END + 0x036C     
#define PLAYER_SKILL_INFO_28_1                                  UNIT_END + 0x036D     
#define PLAYER_SKILL_INFO_28_2                                  UNIT_END + 0x036E     
#define PLAYER_SKILL_INFO_28_3                                  UNIT_END + 0x036F     
#define PLAYER_SKILL_INFO_29_1                                  UNIT_END + 0x0370     
#define PLAYER_SKILL_INFO_29_2                                  UNIT_END + 0x0371     
#define PLAYER_SKILL_INFO_29_3                                  UNIT_END + 0x0372     
#define PLAYER_SKILL_INFO_30_1                                  UNIT_END + 0x0373     
#define PLAYER_SKILL_INFO_30_2                                  UNIT_END + 0x0374     
#define PLAYER_SKILL_INFO_30_3                                  UNIT_END + 0x0375     
#define PLAYER_SKILL_INFO_31_1                                  UNIT_END + 0x0376     
#define PLAYER_SKILL_INFO_31_2                                  UNIT_END + 0x0377     
#define PLAYER_SKILL_INFO_31_3                                  UNIT_END + 0x0378     
#define PLAYER_SKILL_INFO_32_1                                  UNIT_END + 0x0379     
#define PLAYER_SKILL_INFO_32_2                                  UNIT_END + 0x037A     
#define PLAYER_SKILL_INFO_32_3                                  UNIT_END + 0x037B     
#define PLAYER_SKILL_INFO_33_1                                  UNIT_END + 0x037C     
#define PLAYER_SKILL_INFO_33_2                                  UNIT_END + 0x037D     
#define PLAYER_SKILL_INFO_33_3                                  UNIT_END + 0x037E     
#define PLAYER_SKILL_INFO_34_1                                  UNIT_END + 0x037F     
#define PLAYER_SKILL_INFO_34_2                                  UNIT_END + 0x0380     
#define PLAYER_SKILL_INFO_34_3                                  UNIT_END + 0x0381     
#define PLAYER_SKILL_INFO_35_1                                  UNIT_END + 0x0382     
#define PLAYER_SKILL_INFO_35_2                                  UNIT_END + 0x0383     
#define PLAYER_SKILL_INFO_35_3                                  UNIT_END + 0x0384     
#define PLAYER_SKILL_INFO_36_1                                  UNIT_END + 0x0385     
#define PLAYER_SKILL_INFO_36_2                                  UNIT_END + 0x0386     
#define PLAYER_SKILL_INFO_36_3                                  UNIT_END + 0x0387     
#define PLAYER_SKILL_INFO_37_1                                  UNIT_END + 0x0388     
#define PLAYER_SKILL_INFO_37_2                                  UNIT_END + 0x0389     
#define PLAYER_SKILL_INFO_37_3                                  UNIT_END + 0x038A     
#define PLAYER_SKILL_INFO_38_1                                  UNIT_END + 0x038B     
#define PLAYER_SKILL_INFO_38_2                                  UNIT_END + 0x038C     
#define PLAYER_SKILL_INFO_38_3                                  UNIT_END + 0x038D     
#define PLAYER_SKILL_INFO_39_1                                  UNIT_END + 0x038E     
#define PLAYER_SKILL_INFO_39_2                                  UNIT_END + 0x038F     
#define PLAYER_SKILL_INFO_39_3                                  UNIT_END + 0x0390     
#define PLAYER_SKILL_INFO_40_1                                  UNIT_END + 0x0391     
#define PLAYER_SKILL_INFO_40_2                                  UNIT_END + 0x0392     
#define PLAYER_SKILL_INFO_40_3                                  UNIT_END + 0x0393     
#define PLAYER_SKILL_INFO_41_1                                  UNIT_END + 0x0394     
#define PLAYER_SKILL_INFO_41_2                                  UNIT_END + 0x0395     
#define PLAYER_SKILL_INFO_41_3                                  UNIT_END + 0x0396     
#define PLAYER_SKILL_INFO_42_1                                  UNIT_END + 0x0397     
#define PLAYER_SKILL_INFO_42_2                                  UNIT_END + 0x0398     
#define PLAYER_SKILL_INFO_42_3                                  UNIT_END + 0x0399     
#define PLAYER_SKILL_INFO_43_1                                  UNIT_END + 0x039A     
#define PLAYER_SKILL_INFO_43_2                                  UNIT_END + 0x039B     
#define PLAYER_SKILL_INFO_43_3                                  UNIT_END + 0x039C     
#define PLAYER_SKILL_INFO_44_1                                  UNIT_END + 0x039D     
#define PLAYER_SKILL_INFO_44_2                                  UNIT_END + 0x039E     
#define PLAYER_SKILL_INFO_44_3                                  UNIT_END + 0x039F     
#define PLAYER_SKILL_INFO_45_1                                  UNIT_END + 0x03A0     
#define PLAYER_SKILL_INFO_45_2                                  UNIT_END + 0x03A1     
#define PLAYER_SKILL_INFO_45_3                                  UNIT_END + 0x03A2     
#define PLAYER_SKILL_INFO_46_1                                  UNIT_END + 0x03A3     
#define PLAYER_SKILL_INFO_46_2                                  UNIT_END + 0x03A4     
#define PLAYER_SKILL_INFO_46_3                                  UNIT_END + 0x03A5     
#define PLAYER_SKILL_INFO_47_1                                  UNIT_END + 0x03A6     
#define PLAYER_SKILL_INFO_47_2                                  UNIT_END + 0x03A7     
#define PLAYER_SKILL_INFO_47_3                                  UNIT_END + 0x03A8     
#define PLAYER_SKILL_INFO_48_1                                  UNIT_END + 0x03A9     
#define PLAYER_SKILL_INFO_48_2                                  UNIT_END + 0x03AA     
#define PLAYER_SKILL_INFO_48_3                                  UNIT_END + 0x03AB     
#define PLAYER_SKILL_INFO_49_1                                  UNIT_END + 0x03AC     
#define PLAYER_SKILL_INFO_49_2                                  UNIT_END + 0x03AD     
#define PLAYER_SKILL_INFO_49_3                                  UNIT_END + 0x03AE     
#define PLAYER_SKILL_INFO_50_1                                  UNIT_END + 0x03AF     
#define PLAYER_SKILL_INFO_50_2                                  UNIT_END + 0x03B0     
#define PLAYER_SKILL_INFO_50_3                                  UNIT_END + 0x03B1     
#define PLAYER_SKILL_INFO_51_1                                  UNIT_END + 0x03B2     
#define PLAYER_SKILL_INFO_51_2                                  UNIT_END + 0x03B3     
#define PLAYER_SKILL_INFO_51_3                                  UNIT_END + 0x03B4     
#define PLAYER_SKILL_INFO_52_1                                  UNIT_END + 0x03B5     
#define PLAYER_SKILL_INFO_52_2                                  UNIT_END + 0x03B6     
#define PLAYER_SKILL_INFO_52_3                                  UNIT_END + 0x03B7     
#define PLAYER_SKILL_INFO_53_1                                  UNIT_END + 0x03B8     
#define PLAYER_SKILL_INFO_53_2                                  UNIT_END + 0x03B9     
#define PLAYER_SKILL_INFO_53_3                                  UNIT_END + 0x03BA     
#define PLAYER_SKILL_INFO_54_1                                  UNIT_END + 0x03BB     
#define PLAYER_SKILL_INFO_54_2                                  UNIT_END + 0x03BC     
#define PLAYER_SKILL_INFO_54_3                                  UNIT_END + 0x03BD     
#define PLAYER_SKILL_INFO_55_1                                  UNIT_END + 0x03BE     
#define PLAYER_SKILL_INFO_55_2                                  UNIT_END + 0x03BF     
#define PLAYER_SKILL_INFO_55_3                                  UNIT_END + 0x03C0     
#define PLAYER_SKILL_INFO_56_1                                  UNIT_END + 0x03C1     
#define PLAYER_SKILL_INFO_56_2                                  UNIT_END + 0x03C2     
#define PLAYER_SKILL_INFO_56_3                                  UNIT_END + 0x03C3     
#define PLAYER_SKILL_INFO_57_1                                  UNIT_END + 0x03C4     
#define PLAYER_SKILL_INFO_57_2                                  UNIT_END + 0x03C5     
#define PLAYER_SKILL_INFO_57_3                                  UNIT_END + 0x03C6     
#define PLAYER_SKILL_INFO_58_1                                  UNIT_END + 0x03C7     
#define PLAYER_SKILL_INFO_58_2                                  UNIT_END + 0x03C8     
#define PLAYER_SKILL_INFO_58_3                                  UNIT_END + 0x03C9     
#define PLAYER_SKILL_INFO_59_1                                  UNIT_END + 0x03CA     
#define PLAYER_SKILL_INFO_59_2                                  UNIT_END + 0x03CB     
#define PLAYER_SKILL_INFO_59_3                                  UNIT_END + 0x03CC     
#define PLAYER_SKILL_INFO_60_1                                  UNIT_END + 0x03CD     
#define PLAYER_SKILL_INFO_60_2                                  UNIT_END + 0x03CE     
#define PLAYER_SKILL_INFO_60_3                                  UNIT_END + 0x03CF     
#define PLAYER_SKILL_INFO_61_1                                  UNIT_END + 0x03D0     
#define PLAYER_SKILL_INFO_61_2                                  UNIT_END + 0x03D1     
#define PLAYER_SKILL_INFO_61_3                                  UNIT_END + 0x03D2     
#define PLAYER_SKILL_INFO_62_1                                  UNIT_END + 0x03D3     
#define PLAYER_SKILL_INFO_62_2                                  UNIT_END + 0x03D4     
#define PLAYER_SKILL_INFO_62_3                                  UNIT_END + 0x03D5     
#define PLAYER_SKILL_INFO_63_1                                  UNIT_END + 0x03D6     
#define PLAYER_SKILL_INFO_63_2                                  UNIT_END + 0x03D7     
#define PLAYER_SKILL_INFO_63_3                                  UNIT_END + 0x03D8     
#define PLAYER_SKILL_INFO_64_1                                  UNIT_END + 0x03D9     
#define PLAYER_SKILL_INFO_64_2                                  UNIT_END + 0x03DA     
#define PLAYER_SKILL_INFO_64_3                                  UNIT_END + 0x03DB     
#define PLAYER_SKILL_INFO_65_1                                  UNIT_END + 0x03DC     
#define PLAYER_SKILL_INFO_65_2                                  UNIT_END + 0x03DD     
#define PLAYER_SKILL_INFO_65_3                                  UNIT_END + 0x03DE     
#define PLAYER_SKILL_INFO_66_1                                  UNIT_END + 0x03DF     
#define PLAYER_SKILL_INFO_66_2                                  UNIT_END + 0x03E0     
#define PLAYER_SKILL_INFO_66_3                                  UNIT_END + 0x03E1     
#define PLAYER_SKILL_INFO_67_1                                  UNIT_END + 0x03E2     
#define PLAYER_SKILL_INFO_67_2                                  UNIT_END + 0x03E3     
#define PLAYER_SKILL_INFO_67_3                                  UNIT_END + 0x03E4     
#define PLAYER_SKILL_INFO_68_1                                  UNIT_END + 0x03E5     
#define PLAYER_SKILL_INFO_68_2                                  UNIT_END + 0x03E6     
#define PLAYER_SKILL_INFO_68_3                                  UNIT_END + 0x03E7     
#define PLAYER_SKILL_INFO_69_1                                  UNIT_END + 0x03E8     
#define PLAYER_SKILL_INFO_69_2                                  UNIT_END + 0x03E9     
#define PLAYER_SKILL_INFO_69_3                                  UNIT_END + 0x03EA     
#define PLAYER_SKILL_INFO_70_1                                  UNIT_END + 0x03EB     
#define PLAYER_SKILL_INFO_70_2                                  UNIT_END + 0x03EC     
#define PLAYER_SKILL_INFO_70_3                                  UNIT_END + 0x03ED     
#define PLAYER_SKILL_INFO_71_1                                  UNIT_END + 0x03EE     
#define PLAYER_SKILL_INFO_71_2                                  UNIT_END + 0x03EF     
#define PLAYER_SKILL_INFO_71_3                                  UNIT_END + 0x03F0     
#define PLAYER_SKILL_INFO_72_1                                  UNIT_END + 0x03F1     
#define PLAYER_SKILL_INFO_72_2                                  UNIT_END + 0x03F2     
#define PLAYER_SKILL_INFO_72_3                                  UNIT_END + 0x03F3     
#define PLAYER_SKILL_INFO_73_1                                  UNIT_END + 0x03F4     
#define PLAYER_SKILL_INFO_73_2                                  UNIT_END + 0x03F5     
#define PLAYER_SKILL_INFO_73_3                                  UNIT_END + 0x03F6     
#define PLAYER_SKILL_INFO_74_1                                  UNIT_END + 0x03F7     
#define PLAYER_SKILL_INFO_74_2                                  UNIT_END + 0x03F8     
#define PLAYER_SKILL_INFO_74_3                                  UNIT_END + 0x03F9     
#define PLAYER_SKILL_INFO_75_1                                  UNIT_END + 0x03FA     
#define PLAYER_SKILL_INFO_75_2                                  UNIT_END + 0x03FB     
#define PLAYER_SKILL_INFO_75_3                                  UNIT_END + 0x03FC     
#define PLAYER_SKILL_INFO_76_1                                  UNIT_END + 0x03FD     
#define PLAYER_SKILL_INFO_76_2                                  UNIT_END + 0x03FE     
#define PLAYER_SKILL_INFO_76_3                                  UNIT_END + 0x03FF     
#define PLAYER_SKILL_INFO_77_1                                  UNIT_END + 0x0400     
#define PLAYER_SKILL_INFO_77_2                                  UNIT_END + 0x0401     
#define PLAYER_SKILL_INFO_77_3                                  UNIT_END + 0x0402     
#define PLAYER_SKILL_INFO_78_1                                  UNIT_END + 0x0403     
#define PLAYER_SKILL_INFO_78_2                                  UNIT_END + 0x0404     
#define PLAYER_SKILL_INFO_78_3                                  UNIT_END + 0x0405     
#define PLAYER_SKILL_INFO_79_1                                  UNIT_END + 0x0406     
#define PLAYER_SKILL_INFO_79_2                                  UNIT_END + 0x0407     
#define PLAYER_SKILL_INFO_79_3                                  UNIT_END + 0x0408     
#define PLAYER_SKILL_INFO_80_1                                  UNIT_END + 0x0409     
#define PLAYER_SKILL_INFO_80_2                                  UNIT_END + 0x040A     
#define PLAYER_SKILL_INFO_80_3                                  UNIT_END + 0x040B     
#define PLAYER_SKILL_INFO_81_1                                  UNIT_END + 0x040C     
#define PLAYER_SKILL_INFO_81_2                                  UNIT_END + 0x040D     
#define PLAYER_SKILL_INFO_81_3                                  UNIT_END + 0x040E     
#define PLAYER_SKILL_INFO_82_1                                  UNIT_END + 0x040F     
#define PLAYER_SKILL_INFO_82_2                                  UNIT_END + 0x0410     
#define PLAYER_SKILL_INFO_82_3                                  UNIT_END + 0x0411     
#define PLAYER_SKILL_INFO_83_1                                  UNIT_END + 0x0412     
#define PLAYER_SKILL_INFO_83_2                                  UNIT_END + 0x0413     
#define PLAYER_SKILL_INFO_83_3                                  UNIT_END + 0x0414     
#define PLAYER_SKILL_INFO_84_1                                  UNIT_END + 0x0415     
#define PLAYER_SKILL_INFO_84_2                                  UNIT_END + 0x0416     
#define PLAYER_SKILL_INFO_84_3                                  UNIT_END + 0x0417     
#define PLAYER_SKILL_INFO_85_1                                  UNIT_END + 0x0418     
#define PLAYER_SKILL_INFO_85_2                                  UNIT_END + 0x0419     
#define PLAYER_SKILL_INFO_85_3                                  UNIT_END + 0x041A     
#define PLAYER_SKILL_INFO_86_1                                  UNIT_END + 0x041B     
#define PLAYER_SKILL_INFO_86_2                                  UNIT_END + 0x041C     
#define PLAYER_SKILL_INFO_86_3                                  UNIT_END + 0x041D     
#define PLAYER_SKILL_INFO_87_1                                  UNIT_END + 0x041E     
#define PLAYER_SKILL_INFO_87_2                                  UNIT_END + 0x041F     
#define PLAYER_SKILL_INFO_87_3                                  UNIT_END + 0x0420     
#define PLAYER_SKILL_INFO_88_1                                  UNIT_END + 0x0421     
#define PLAYER_SKILL_INFO_88_2                                  UNIT_END + 0x0422     
#define PLAYER_SKILL_INFO_88_3                                  UNIT_END + 0x0423     
#define PLAYER_SKILL_INFO_89_1                                  UNIT_END + 0x0424     
#define PLAYER_SKILL_INFO_89_2                                  UNIT_END + 0x0425     
#define PLAYER_SKILL_INFO_89_3                                  UNIT_END + 0x0426     
#define PLAYER_SKILL_INFO_90_1                                  UNIT_END + 0x0427     
#define PLAYER_SKILL_INFO_90_2                                  UNIT_END + 0x0428     
#define PLAYER_SKILL_INFO_90_3                                  UNIT_END + 0x0429     
#define PLAYER_SKILL_INFO_91_1                                  UNIT_END + 0x042A     
#define PLAYER_SKILL_INFO_91_2                                  UNIT_END + 0x042B     
#define PLAYER_SKILL_INFO_91_3                                  UNIT_END + 0x042C     
#define PLAYER_SKILL_INFO_92_1                                  UNIT_END + 0x042D     
#define PLAYER_SKILL_INFO_92_2                                  UNIT_END + 0x042E     
#define PLAYER_SKILL_INFO_92_3                                  UNIT_END + 0x042F     
#define PLAYER_SKILL_INFO_93_1                                  UNIT_END + 0x0430     
#define PLAYER_SKILL_INFO_93_2                                  UNIT_END + 0x0431     
#define PLAYER_SKILL_INFO_93_3                                  UNIT_END + 0x0432     
#define PLAYER_SKILL_INFO_94_1                                  UNIT_END + 0x0433     
#define PLAYER_SKILL_INFO_94_2                                  UNIT_END + 0x0434     
#define PLAYER_SKILL_INFO_94_3                                  UNIT_END + 0x0435     
#define PLAYER_SKILL_INFO_95_1                                  UNIT_END + 0x0436     
#define PLAYER_SKILL_INFO_95_2                                  UNIT_END + 0x0437     
#define PLAYER_SKILL_INFO_95_3                                  UNIT_END + 0x0438     
#define PLAYER_SKILL_INFO_96_1                                  UNIT_END + 0x0439     
#define PLAYER_SKILL_INFO_96_2                                  UNIT_END + 0x043A     
#define PLAYER_SKILL_INFO_96_3                                  UNIT_END + 0x043B     
#define PLAYER_SKILL_INFO_97_1                                  UNIT_END + 0x043C     
#define PLAYER_SKILL_INFO_97_2                                  UNIT_END + 0x043D     
#define PLAYER_SKILL_INFO_97_3                                  UNIT_END + 0x043E     
#define PLAYER_SKILL_INFO_98_1                                  UNIT_END + 0x043F     
#define PLAYER_SKILL_INFO_98_2                                  UNIT_END + 0x0440     
#define PLAYER_SKILL_INFO_98_3                                  UNIT_END + 0x0441     
#define PLAYER_SKILL_INFO_99_1                                  UNIT_END + 0x0442     
#define PLAYER_SKILL_INFO_99_2                                  UNIT_END + 0x0443     
#define PLAYER_SKILL_INFO_99_3                                  UNIT_END + 0x0444     
#define PLAYER_SKILL_INFO_100_1                                 UNIT_END + 0x0445     
#define PLAYER_SKILL_INFO_100_2                                 UNIT_END + 0x0446     
#define PLAYER_SKILL_INFO_100_3                                 UNIT_END + 0x0447     
#define PLAYER_SKILL_INFO_101_1                                 UNIT_END + 0x0448     
#define PLAYER_SKILL_INFO_101_2                                 UNIT_END + 0x0449     
#define PLAYER_SKILL_INFO_101_3                                 UNIT_END + 0x044A     
#define PLAYER_SKILL_INFO_102_1                                 UNIT_END + 0x044B     
#define PLAYER_SKILL_INFO_102_2                                 UNIT_END + 0x044C     
#define PLAYER_SKILL_INFO_102_3                                 UNIT_END + 0x044D     
#define PLAYER_SKILL_INFO_103_1                                 UNIT_END + 0x044E     
#define PLAYER_SKILL_INFO_103_2                                 UNIT_END + 0x044F     
#define PLAYER_SKILL_INFO_103_3                                 UNIT_END + 0x0450     
#define PLAYER_SKILL_INFO_104_1                                 UNIT_END + 0x0451     
#define PLAYER_SKILL_INFO_104_2                                 UNIT_END + 0x0452     
#define PLAYER_SKILL_INFO_104_3                                 UNIT_END + 0x0453     
#define PLAYER_SKILL_INFO_105_1                                 UNIT_END + 0x0454     
#define PLAYER_SKILL_INFO_105_2                                 UNIT_END + 0x0455     
#define PLAYER_SKILL_INFO_105_3                                 UNIT_END + 0x0456     
#define PLAYER_SKILL_INFO_106_1                                 UNIT_END + 0x0457     
#define PLAYER_SKILL_INFO_106_2                                 UNIT_END + 0x0458     
#define PLAYER_SKILL_INFO_106_3                                 UNIT_END + 0x0459     
#define PLAYER_SKILL_INFO_107_1                                 UNIT_END + 0x045A     
#define PLAYER_SKILL_INFO_107_2                                 UNIT_END + 0x045B     
#define PLAYER_SKILL_INFO_107_3                                 UNIT_END + 0x045C     
#define PLAYER_SKILL_INFO_108_1                                 UNIT_END + 0x045D     
#define PLAYER_SKILL_INFO_108_2                                 UNIT_END + 0x045E     
#define PLAYER_SKILL_INFO_108_3                                 UNIT_END + 0x045F     
#define PLAYER_SKILL_INFO_109_1                                 UNIT_END + 0x0460     
#define PLAYER_SKILL_INFO_109_2                                 UNIT_END + 0x0461     
#define PLAYER_SKILL_INFO_109_3                                 UNIT_END + 0x0462     
#define PLAYER_SKILL_INFO_110_1                                 UNIT_END + 0x0463     
#define PLAYER_SKILL_INFO_110_2                                 UNIT_END + 0x0464     
#define PLAYER_SKILL_INFO_110_3                                 UNIT_END + 0x0465     
#define PLAYER_SKILL_INFO_111_1                                 UNIT_END + 0x0466     
#define PLAYER_SKILL_INFO_111_2                                 UNIT_END + 0x0467     
#define PLAYER_SKILL_INFO_111_3                                 UNIT_END + 0x0468     
#define PLAYER_SKILL_INFO_112_1                                 UNIT_END + 0x0469     
#define PLAYER_SKILL_INFO_112_2                                 UNIT_END + 0x046A     
#define PLAYER_SKILL_INFO_112_3                                 UNIT_END + 0x046B     
#define PLAYER_SKILL_INFO_113_1                                 UNIT_END + 0x046C     
#define PLAYER_SKILL_INFO_113_2                                 UNIT_END + 0x046D     
#define PLAYER_SKILL_INFO_113_3                                 UNIT_END + 0x046E     
#define PLAYER_SKILL_INFO_114_1                                 UNIT_END + 0x046F     
#define PLAYER_SKILL_INFO_114_2                                 UNIT_END + 0x0470     
#define PLAYER_SKILL_INFO_114_3                                 UNIT_END + 0x0471     
#define PLAYER_SKILL_INFO_115_1                                 UNIT_END + 0x0472     
#define PLAYER_SKILL_INFO_115_2                                 UNIT_END + 0x0473     
#define PLAYER_SKILL_INFO_115_3                                 UNIT_END + 0x0474     
#define PLAYER_SKILL_INFO_116_1                                 UNIT_END + 0x0475     
#define PLAYER_SKILL_INFO_116_2                                 UNIT_END + 0x0476     
#define PLAYER_SKILL_INFO_116_3                                 UNIT_END + 0x0477     
#define PLAYER_SKILL_INFO_117_1                                 UNIT_END + 0x0478     
#define PLAYER_SKILL_INFO_117_2                                 UNIT_END + 0x0479     
#define PLAYER_SKILL_INFO_117_3                                 UNIT_END + 0x047A     
#define PLAYER_SKILL_INFO_118_1                                 UNIT_END + 0x047B     
#define PLAYER_SKILL_INFO_118_2                                 UNIT_END + 0x047C     
#define PLAYER_SKILL_INFO_118_3                                 UNIT_END + 0x047D     
#define PLAYER_SKILL_INFO_119_1                                 UNIT_END + 0x047E     
#define PLAYER_SKILL_INFO_119_2                                 UNIT_END + 0x047F     
#define PLAYER_SKILL_INFO_119_3                                 UNIT_END + 0x0480     
#define PLAYER_SKILL_INFO_120_1                                 UNIT_END + 0x0481     
#define PLAYER_SKILL_INFO_120_2                                 UNIT_END + 0x0482     
#define PLAYER_SKILL_INFO_120_3                                 UNIT_END + 0x0483     
#define PLAYER_SKILL_INFO_121_1                                 UNIT_END + 0x0484     
#define PLAYER_SKILL_INFO_121_2                                 UNIT_END + 0x0485     
#define PLAYER_SKILL_INFO_121_3                                 UNIT_END + 0x0486     
#define PLAYER_SKILL_INFO_122_1                                 UNIT_END + 0x0487     
#define PLAYER_SKILL_INFO_122_2                                 UNIT_END + 0x0488     
#define PLAYER_SKILL_INFO_122_3                                 UNIT_END + 0x0489     
#define PLAYER_SKILL_INFO_123_1                                 UNIT_END + 0x048A     
#define PLAYER_SKILL_INFO_123_2                                 UNIT_END + 0x048B     
#define PLAYER_SKILL_INFO_123_3                                 UNIT_END + 0x048C     
#define PLAYER_SKILL_INFO_124_1                                 UNIT_END + 0x048D     
#define PLAYER_SKILL_INFO_124_2                                 UNIT_END + 0x048E     
#define PLAYER_SKILL_INFO_124_3                                 UNIT_END + 0x048F     
#define PLAYER_SKILL_INFO_125_1                                 UNIT_END + 0x0490     
#define PLAYER_SKILL_INFO_125_2                                 UNIT_END + 0x0491     
#define PLAYER_SKILL_INFO_125_3                                 UNIT_END + 0x0492     
#define PLAYER_SKILL_INFO_126_1                                 UNIT_END + 0x0493     
#define PLAYER_SKILL_INFO_126_2                                 UNIT_END + 0x0494     
#define PLAYER_SKILL_INFO_126_3                                 UNIT_END + 0x0495     
#define PLAYER_SKILL_INFO_127_1                                 UNIT_END + 0x0496     
#define PLAYER_SKILL_INFO_127_2                                 UNIT_END + 0x0497     
#define PLAYER_SKILL_INFO_127_3                                 UNIT_END + 0x0498     
#define PLAYER_SKILL_INFO_128_1                                 UNIT_END + 0x0499     
#define PLAYER_SKILL_INFO_128_2                                 UNIT_END + 0x049A     
#define PLAYER_SKILL_INFO_128_3                                 UNIT_END + 0x049B     
#define PLAYER_CHARACTER_POINTS1                                UNIT_END + 0x049C     
#define PLAYER_CHARACTER_POINTS2                                UNIT_END + 0x049D     
#define PLAYER_TRACK_CREATURES                                  UNIT_END + 0x049E     
#define PLAYER_TRACK_RESOURCES                                  UNIT_END + 0x049F     
#define PLAYER_BLOCK_PERCENTAGE                                 UNIT_END + 0x04A0     
#define PLAYER_DODGE_PERCENTAGE                                 UNIT_END + 0x04A1     
#define PLAYER_PARRY_PERCENTAGE                                 UNIT_END + 0x04A2     
#define PLAYER_EXPERTISE                                        UNIT_END + 0x04A3     
#define PLAYER_OFFHAND_EXPERTISE                                UNIT_END + 0x04A4     
#define PLAYER_CRIT_PERCENTAGE                                  UNIT_END + 0x04A5     
#define PLAYER_RANGED_CRIT_PERCENTAGE                           UNIT_END + 0x04A6     
#define PLAYER_OFFHAND_CRIT_PERCENTAGE                          UNIT_END + 0x04A7     
#define PLAYER_SPELL_CRIT_PERCENTAGE1                           UNIT_END + 0x04A8     
#define PLAYER_SPELL_CRIT_PERCENTAGE2                           UNIT_END + 0x04A9     
#define PLAYER_SPELL_CRIT_PERCENTAGE3                           UNIT_END + 0x04AA     
#define PLAYER_SPELL_CRIT_PERCENTAGE4                           UNIT_END + 0x04AB     
#define PLAYER_SPELL_CRIT_PERCENTAGE5                           UNIT_END + 0x04AC     
#define PLAYER_SPELL_CRIT_PERCENTAGE6                           UNIT_END + 0x04AD     
#define PLAYER_SPELL_CRIT_PERCENTAGE7                           UNIT_END + 0x04AE     
#define PLAYER_SHIELD_BLOCK                                     UNIT_END + 0x04AF     
#define PLAYER_SHIELD_BLOCK_CRIT_PERCENTAGE                     UNIT_END + 0x04B0     
#define PLAYER_EXPLORED_ZONES_1                                 UNIT_END + 0x04B1     
#define PLAYER_EXPLORED_ZONES_2                                 UNIT_END + 0x04B2     
#define PLAYER_EXPLORED_ZONES_3                                 UNIT_END + 0x04B3     
#define PLAYER_EXPLORED_ZONES_4                                 UNIT_END + 0x04B4     
#define PLAYER_EXPLORED_ZONES_5                                 UNIT_END + 0x04B5     
#define PLAYER_EXPLORED_ZONES_6                                 UNIT_END + 0x04B6     
#define PLAYER_EXPLORED_ZONES_7                                 UNIT_END + 0x04B7     
#define PLAYER_EXPLORED_ZONES_8                                 UNIT_END + 0x04B8     
#define PLAYER_EXPLORED_ZONES_9                                 UNIT_END + 0x04B9     
#define PLAYER_EXPLORED_ZONES_10                                UNIT_END + 0x04BA     
#define PLAYER_EXPLORED_ZONES_11                                UNIT_END + 0x04BB     
#define PLAYER_EXPLORED_ZONES_12                                UNIT_END + 0x04BC     
#define PLAYER_EXPLORED_ZONES_13                                UNIT_END + 0x04BD     
#define PLAYER_EXPLORED_ZONES_14                                UNIT_END + 0x04BE     
#define PLAYER_EXPLORED_ZONES_15                                UNIT_END + 0x04BF     
#define PLAYER_EXPLORED_ZONES_16                                UNIT_END + 0x04C0     
#define PLAYER_EXPLORED_ZONES_17                                UNIT_END + 0x04C1     
#define PLAYER_EXPLORED_ZONES_18                                UNIT_END + 0x04C2     
#define PLAYER_EXPLORED_ZONES_19                                UNIT_END + 0x04C3     
#define PLAYER_EXPLORED_ZONES_20                                UNIT_END + 0x04C4     
#define PLAYER_EXPLORED_ZONES_21                                UNIT_END + 0x04C5     
#define PLAYER_EXPLORED_ZONES_22                                UNIT_END + 0x04C6     
#define PLAYER_EXPLORED_ZONES_23                                UNIT_END + 0x04C7     
#define PLAYER_EXPLORED_ZONES_24                                UNIT_END + 0x04C8     
#define PLAYER_EXPLORED_ZONES_25                                UNIT_END + 0x04C9     
#define PLAYER_EXPLORED_ZONES_26                                UNIT_END + 0x04CA     
#define PLAYER_EXPLORED_ZONES_27                                UNIT_END + 0x04CB     
#define PLAYER_EXPLORED_ZONES_28                                UNIT_END + 0x04CC     
#define PLAYER_EXPLORED_ZONES_29                                UNIT_END + 0x04CD     
#define PLAYER_EXPLORED_ZONES_30                                UNIT_END + 0x04CE     
#define PLAYER_EXPLORED_ZONES_31                                UNIT_END + 0x04CF     
#define PLAYER_EXPLORED_ZONES_32                                UNIT_END + 0x04D0     
#define PLAYER_EXPLORED_ZONES_33                                UNIT_END + 0x04D1     
#define PLAYER_EXPLORED_ZONES_34                                UNIT_END + 0x04D2     
#define PLAYER_EXPLORED_ZONES_35                                UNIT_END + 0x04D3     
#define PLAYER_EXPLORED_ZONES_36                                UNIT_END + 0x04D4     
#define PLAYER_EXPLORED_ZONES_37                                UNIT_END + 0x04D5     
#define PLAYER_EXPLORED_ZONES_38                                UNIT_END + 0x04D6     
#define PLAYER_EXPLORED_ZONES_39                                UNIT_END + 0x04D7     
#define PLAYER_EXPLORED_ZONES_40                                UNIT_END + 0x04D8     
#define PLAYER_EXPLORED_ZONES_41                                UNIT_END + 0x04D9     
#define PLAYER_EXPLORED_ZONES_42                                UNIT_END + 0x04DA     
#define PLAYER_EXPLORED_ZONES_43                                UNIT_END + 0x04DB     
#define PLAYER_EXPLORED_ZONES_44                                UNIT_END + 0x04DC     
#define PLAYER_EXPLORED_ZONES_45                                UNIT_END + 0x04DD     
#define PLAYER_EXPLORED_ZONES_46                                UNIT_END + 0x04DE     
#define PLAYER_EXPLORED_ZONES_47                                UNIT_END + 0x04DF     
#define PLAYER_EXPLORED_ZONES_48                                UNIT_END + 0x04E0     
#define PLAYER_EXPLORED_ZONES_49                                UNIT_END + 0x04E1     
#define PLAYER_EXPLORED_ZONES_50                                UNIT_END + 0x04E2     
#define PLAYER_EXPLORED_ZONES_51                                UNIT_END + 0x04E3     
#define PLAYER_EXPLORED_ZONES_52                                UNIT_END + 0x04E4     
#define PLAYER_EXPLORED_ZONES_53                                UNIT_END + 0x04E5     
#define PLAYER_EXPLORED_ZONES_54                                UNIT_END + 0x04E6     
#define PLAYER_EXPLORED_ZONES_55                                UNIT_END + 0x04E7     
#define PLAYER_EXPLORED_ZONES_56                                UNIT_END + 0x04E8     
#define PLAYER_EXPLORED_ZONES_57                                UNIT_END + 0x04E9     
#define PLAYER_EXPLORED_ZONES_58                                UNIT_END + 0x04EA     
#define PLAYER_EXPLORED_ZONES_59                                UNIT_END + 0x04EB     
#define PLAYER_EXPLORED_ZONES_60                                UNIT_END + 0x04EC     
#define PLAYER_EXPLORED_ZONES_61                                UNIT_END + 0x04ED     
#define PLAYER_EXPLORED_ZONES_62                                UNIT_END + 0x04EE     
#define PLAYER_EXPLORED_ZONES_63                                UNIT_END + 0x04EF     
#define PLAYER_EXPLORED_ZONES_64                                UNIT_END + 0x04F0     
#define PLAYER_REST_STATE_EXPERIENCE                            UNIT_END + 0x04F1     
#define PLAYER_FIELD_COINAGE                                    UNIT_END + 0x04F2     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_POS                        UNIT_END + 0x04F3     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_POS_2                      UNIT_END + 0x04F4     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_POS_3                      UNIT_END + 0x04F5     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_POS_4                      UNIT_END + 0x04F6     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_POS_5                      UNIT_END + 0x04F7     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_POS_6                      UNIT_END + 0x04F8     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_POS_7                      UNIT_END + 0x04F9     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_NEG                        UNIT_END + 0x04FA     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_NEG_2                      UNIT_END + 0x04FB     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_NEG_3                      UNIT_END + 0x04FC     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_NEG_4                      UNIT_END + 0x04FD     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_NEG_5                      UNIT_END + 0x04FE     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_NEG_6                      UNIT_END + 0x04FF     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_NEG_7                      UNIT_END + 0x0500     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_PCT                        UNIT_END + 0x0501     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_PCT_2                      UNIT_END + 0x0502     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_PCT_3                      UNIT_END + 0x0503     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_PCT_4                      UNIT_END + 0x0504     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_PCT_5                      UNIT_END + 0x0505     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_PCT_6                      UNIT_END + 0x0506     
#define PLAYER_FIELD_MOD_DAMAGE_DONE_PCT_7                      UNIT_END + 0x0507     
#define PLAYER_FIELD_MOD_HEALING_DONE_POS                       UNIT_END + 0x0508     
#define PLAYER_FIELD_MOD_TARGET_RESISTANCE                      UNIT_END + 0x0509     
#define PLAYER_FIELD_MOD_TARGET_PHYSICAL_RESISTANCE             UNIT_END + 0x050A     
#define PLAYER_FIELD_BYTES                                      UNIT_END + 0x050B     
#define PLAYER_AMMO_ID                                          UNIT_END + 0x050C     
#define PLAYER_SELF_RES_SPELL                                   UNIT_END + 0x050D     
#define PLAYER_FIELD_PVP_MEDALS                                 UNIT_END + 0x050E     
#define PLAYER_FIELD_BUYBACK_PRICE_1                            UNIT_END + 0x050F     
#define PLAYER_FIELD_BUYBACK_PRICE_2                            UNIT_END + 0x0510     
#define PLAYER_FIELD_BUYBACK_PRICE_3                            UNIT_END + 0x0511     
#define PLAYER_FIELD_BUYBACK_PRICE_4                            UNIT_END + 0x0512     
#define PLAYER_FIELD_BUYBACK_PRICE_5                            UNIT_END + 0x0513     
#define PLAYER_FIELD_BUYBACK_PRICE_6                            UNIT_END + 0x0514     
#define PLAYER_FIELD_BUYBACK_PRICE_7                            UNIT_END + 0x0515     
#define PLAYER_FIELD_BUYBACK_PRICE_8                            UNIT_END + 0x0516     
#define PLAYER_FIELD_BUYBACK_PRICE_9                            UNIT_END + 0x0517     
#define PLAYER_FIELD_BUYBACK_PRICE_10                           UNIT_END + 0x0518     
#define PLAYER_FIELD_BUYBACK_PRICE_11                           UNIT_END + 0x0519     
#define PLAYER_FIELD_BUYBACK_PRICE_12                           UNIT_END + 0x051A     
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_1                        UNIT_END + 0x051B     
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_2                        UNIT_END + 0x051C     
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_3                        UNIT_END + 0x051D     
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_4                        UNIT_END + 0x051E     
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_5                        UNIT_END + 0x051F     
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_6                        UNIT_END + 0x0520     
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_7                        UNIT_END + 0x0521     
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_8                        UNIT_END + 0x0522     
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_9                        UNIT_END + 0x0523     
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_10                       UNIT_END + 0x0524     
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_11                       UNIT_END + 0x0525     
#define PLAYER_FIELD_BUYBACK_TIMESTAMP_12                       UNIT_END + 0x0526     
#define PLAYER_FIELD_KILLS                                      UNIT_END + 0x0527     
#define PLAYER_FIELD_TODAY_CONTRIBUTION                         UNIT_END + 0x0528     
#define PLAYER_FIELD_YESTERDAY_CONTRIBUTION                     UNIT_END + 0x0529     
#define PLAYER_FIELD_LIFETIME_HONORBALE_KILLS                   UNIT_END + 0x052A     
#define PLAYER_FIELD_BYTES2                                     UNIT_END + 0x052B     
#define PLAYER_FIELD_WATCHED_FACTION_INDEX                      UNIT_END + 0x052C     
#define PLAYER_FIELD_COMBAT_RATING_1                            UNIT_END + 0x052D     
#define PLAYER_FIELD_COMBAT_RATING_2                            UNIT_END + 0x052E     
#define PLAYER_FIELD_COMBAT_RATING_3                            UNIT_END + 0x052F     
#define PLAYER_FIELD_COMBAT_RATING_4                            UNIT_END + 0x0530     
#define PLAYER_FIELD_COMBAT_RATING_5                            UNIT_END + 0x0531     
#define PLAYER_FIELD_COMBAT_RATING_6                            UNIT_END + 0x0532     
#define PLAYER_FIELD_COMBAT_RATING_7                            UNIT_END + 0x0533     
#define PLAYER_FIELD_COMBAT_RATING_8                            UNIT_END + 0x0534     
#define PLAYER_FIELD_COMBAT_RATING_9                            UNIT_END + 0x0535     
#define PLAYER_FIELD_COMBAT_RATING_10                           UNIT_END + 0x0536     
#define PLAYER_FIELD_COMBAT_RATING_11                           UNIT_END + 0x0537     
#define PLAYER_FIELD_COMBAT_RATING_12                           UNIT_END + 0x0538     
#define PLAYER_FIELD_COMBAT_RATING_13                           UNIT_END + 0x0539     
#define PLAYER_FIELD_COMBAT_RATING_14                           UNIT_END + 0x053A     
#define PLAYER_FIELD_COMBAT_RATING_15                           UNIT_END + 0x053B     
#define PLAYER_FIELD_COMBAT_RATING_16                           UNIT_END + 0x053C     
#define PLAYER_FIELD_COMBAT_RATING_17                           UNIT_END + 0x053D     
#define PLAYER_FIELD_COMBAT_RATING_18                           UNIT_END + 0x053E     
#define PLAYER_FIELD_COMBAT_RATING_19                           UNIT_END + 0x053F     
#define PLAYER_FIELD_COMBAT_RATING_20                           UNIT_END + 0x0540     
#define PLAYER_FIELD_COMBAT_RATING_21                           UNIT_END + 0x0541     
#define PLAYER_FIELD_COMBAT_RATING_22                           UNIT_END + 0x0542     
#define PLAYER_FIELD_COMBAT_RATING_23                           UNIT_END + 0x0543     
#define PLAYER_FIELD_COMBAT_RATING_24                           UNIT_END + 0x0544     
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_1                        UNIT_END + 0x0545     
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_2                        UNIT_END + 0x0546     
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_3                        UNIT_END + 0x0547     
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_5                        UNIT_END + 0x0548     
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_6                        UNIT_END + 0x0549     
#define PLAYER_FIELD_ARENA_TEAM_INFO_1_7                        UNIT_END + 0x054A     
#define PLAYER_FIELD_ARENA_TEAM_INFO_2_1                        UNIT_END + 0x054B     
#define PLAYER_FIELD_ARENA_TEAM_INFO_2_2                        UNIT_END + 0x054C     
#define PLAYER_FIELD_ARENA_TEAM_INFO_2_3                        UNIT_END + 0x054D     
#define PLAYER_FIELD_ARENA_TEAM_INFO_2_5                        UNIT_END + 0x054E     
#define PLAYER_FIELD_ARENA_TEAM_INFO_2_6                        UNIT_END + 0x054F     
#define PLAYER_FIELD_ARENA_TEAM_INFO_2_7                        UNIT_END + 0x0550     
#define PLAYER_FIELD_ARENA_TEAM_INFO_3_1                        UNIT_END + 0x0551     
#define PLAYER_FIELD_ARENA_TEAM_INFO_3_2                        UNIT_END + 0x0552     
#define PLAYER_FIELD_ARENA_TEAM_INFO_3_3                        UNIT_END + 0x0553     
#define PLAYER_FIELD_ARENA_TEAM_INFO_3_5                        UNIT_END + 0x0554     
#define PLAYER_FIELD_ARENA_TEAM_INFO_3_6                        UNIT_END + 0x0555     
#define PLAYER_FIELD_ARENA_TEAM_INFO_3_7                        UNIT_END + 0x0556     
#define PLAYER_FIELD_HONOR_CURRENCY                             UNIT_END + 0x0557     
#define PLAYER_FIELD_ARENA_CURRENCY                             UNIT_END + 0x0558     
#define PLAYER_FIELD_MOD_MANA_REGEN                             UNIT_END + 0x0559     
#define PLAYER_FIELD_MOD_MANA_REGEN_INTERRUPT                   UNIT_END + 0x055A     
#define PLAYER_FIELD_MAX_LEVEL                                  UNIT_END + 0x055B     
#define PLAYER_FIELD_DAILY_QUESTS_1                             UNIT_END + 0x055C     
#define PLAYER_FIELD_DAILY_QUESTS_2                             UNIT_END + 0x055D     
#define PLAYER_FIELD_DAILY_QUESTS_3                             UNIT_END + 0x055E     
#define PLAYER_FIELD_DAILY_QUESTS_4                             UNIT_END + 0x055F     
#define PLAYER_FIELD_DAILY_QUESTS_5                             UNIT_END + 0x0560     
#define PLAYER_FIELD_DAILY_QUESTS_6                             UNIT_END + 0x0561     
#define PLAYER_FIELD_DAILY_QUESTS_7                             UNIT_END + 0x0562     
#define PLAYER_FIELD_DAILY_QUESTS_8                             UNIT_END + 0x0563     
#define PLAYER_FIELD_DAILY_QUESTS_9                             UNIT_END + 0x0564     
#define PLAYER_FIELD_DAILY_QUESTS_10                            UNIT_END + 0x0565     
#define PLAYER_FIELD_DAILY_QUESTS_11                            UNIT_END + 0x0566     
#define PLAYER_FIELD_DAILY_QUESTS_12                            UNIT_END + 0x0567     
#define PLAYER_FIELD_DAILY_QUESTS_13                            UNIT_END + 0x0568     
#define PLAYER_FIELD_DAILY_QUESTS_14                            UNIT_END + 0x0569     
#define PLAYER_FIELD_DAILY_QUESTS_15                            UNIT_END + 0x056A     
#define PLAYER_FIELD_DAILY_QUESTS_16                            UNIT_END + 0x056B     
#define PLAYER_FIELD_DAILY_QUESTS_17                            UNIT_END + 0x056C     
#define PLAYER_FIELD_DAILY_QUESTS_18                            UNIT_END + 0x056D     
#define PLAYER_FIELD_DAILY_QUESTS_19                            UNIT_END + 0x056E     
#define PLAYER_FIELD_DAILY_QUESTS_20                            UNIT_END + 0x056F     
#define PLAYER_FIELD_DAILY_QUESTS_21                            UNIT_END + 0x0570     
#define PLAYER_FIELD_DAILY_QUESTS_22                            UNIT_END + 0x0571     
#define PLAYER_FIELD_DAILY_QUESTS_23                            UNIT_END + 0x0572     
#define PLAYER_FIELD_DAILY_QUESTS_24                            UNIT_END + 0x0573     
#define PLAYER_FIELD_DAILY_QUESTS_25                            UNIT_END + 0x0574     
#define PLAYER_FIELD_NUM_RUNES                                  UNIT_END + 0x0575     
#define PLAYER_FIELD_BASE_RUNES_1                               UNIT_END + 0x0576     
#define PLAYER_FIELD_BASE_RUNES_2                               UNIT_END + 0x0577     
#define PLAYER_FIELD_BASE_RUNES_3                               UNIT_END + 0x0578     
#define PLAYER_FIELD_BASE_RUNES_4                               UNIT_END + 0x0579     
#define PLAYER_FIELD_BASE_RUNES_5                               UNIT_END + 0x057A     
#define PLAYER_FIELD_BASE_RUNES_6                               UNIT_END + 0x057B     
#define PLAYER_FIELD_BASE_RUNES_7                               UNIT_END + 0x057C     
#define PLAYER_FIELD_BASE_RUNES_8                               UNIT_END + 0x057D     
#define PLAYER_FIELD_RUNES_1                                    UNIT_END + 0x057E     
#define PLAYER_FIELD_RUNES_2                                    UNIT_END + 0x057F     
#define PLAYER_FIELD_RUNES_3                                    UNIT_END + 0x0580     
#define PLAYER_FIELD_RUNES_4                                    UNIT_END + 0x0581     
#define PLAYER_FIELD_RUNES_5                                    UNIT_END + 0x0582     
#define PLAYER_FIELD_RUNES_6                                    UNIT_END + 0x0583     
#define PLAYER_FIELD_RUNES_7                                    UNIT_END + 0x0584     
#define PLAYER_FIELD_RUNES_8                                    UNIT_END + 0x0585     
#define PLAYER_RUNE_REGEN_1                                     UNIT_END + 0x0586     
#define PLAYER_RUNE_REGEN_2                                     UNIT_END + 0x0587     
#define PLAYER_RUNE_REGEN_3                                     UNIT_END + 0x0588     
#define PLAYER_RUNE_REGEN_4                                     UNIT_END + 0x0589     
#define PLAYER_RUNE_POWER                                       UNIT_END + 0x058A     
#define PLAYER_RUNE_REGEN_ENABLED                               UNIT_END + 0x058B     
#define PLAYER_NO_REAGENT_COST_1                                UNIT_END + 0x058C     
#define PLAYER_NO_REAGENT_COST_2                                UNIT_END + 0x058D     
#define PLAYER_FIELD_GLYPH_SLOTS_1                              UNIT_END + 0x058E     
#define PLAYER_FIELD_GLYPH_SLOTS_2                              UNIT_END + 0x058F     
#define PLAYER_FIELD_GLYPH_SLOTS_3                              UNIT_END + 0x0590     
#define PLAYER_FIELD_GLYPH_SLOTS_4                              UNIT_END + 0x0591     
#define PLAYER_FIELD_GLYPH_SLOTS_5                              UNIT_END + 0x0592     
#define PLAYER_FIELD_GLYPH_SLOTS_6                              UNIT_END + 0x0593     
#define PLAYER_FIELD_GLYPH_SLOTS_7                              UNIT_END + 0x0594     
#define PLAYER_FIELD_GLYPH_SLOTS_8                              UNIT_END + 0x0595     
#define PLAYER_FIELD_GLYPHS_1                                   UNIT_END + 0x0596     
#define PLAYER_FIELD_GLYPHS_2                                   UNIT_END + 0x0597     
#define PLAYER_FIELD_GLYPHS_3                                   UNIT_END + 0x0598     
#define PLAYER_FIELD_GLYPHS_4                                   UNIT_END + 0x0599     
#define PLAYER_FIELD_GLYPHS_5                                   UNIT_END + 0x059A     
#define PLAYER_FIELD_GLYPHS_6                                   UNIT_END + 0x059B     
#define PLAYER_FIELD_GLYPHS_7                                   UNIT_END + 0x059C     
#define PLAYER_FIELD_GLYPHS_8                                   UNIT_END + 0x059D     
#define PLAYER_FIELD_END                                        UNIT_END + 0x059E     

#define OBJECT_FIELD_CREATED_BY                                 OBJECT_END + 0x000

//GAMEOBJECT_:
#define GAMEOBJECT_DISPLAYID                                    OBJECT_END + 0x0002     
#define GAMEOBJECT_FLAGS                                        OBJECT_END + 0x0003     
#define GAMEOBJECT_ROTATION                                     OBJECT_END + 0x0004     
#define GAMEOBJECT_ROTATION_X2                                  OBJECT_END + 0x0005     
#define GAMEOBJECT_ROTATION_X3                                  OBJECT_END + 0x0006     
#define GAMEOBJECT_ROTATION_X4                                  OBJECT_END + 0x0007     
#define GAMEOBJECT_STATE                                        OBJECT_END + 0x0008     
#define GAMEOBJECT_POS_X                                        OBJECT_END + 0x0009     
#define GAMEOBJECT_POS_Y                                        OBJECT_END + 0x000A     
#define GAMEOBJECT_POS_Z                                        OBJECT_END + 0x000B     
#define GAMEOBJECT_FACING                                       OBJECT_END + 0x000C     
#define GAMEOBJECT_DYN_FLAGS                                    OBJECT_END + 0x000D     
#define GAMEOBJECT_FACTION                                      OBJECT_END + 0x000E     
#define GAMEOBJECT_TYPE_ID                                      OBJECT_END + 0x000F     
#define GAMEOBJECT_LEVEL                                        OBJECT_END + 0x0010     
#define GAMEOBJECT_ARTKIT                                       OBJECT_END + 0x0011     
#define GAMEOBJECT_ANIMPROGRESS                                 OBJECT_END + 0x0012     
#define GAMEOBJECT_PADDING                                      OBJECT_END + 0x0013     
#define GAMEOBJECT_END                                          OBJECT_END + 0x0014     

//DYNAMICOBJECT_:
#define DYNAMICOBJECT_CASTER                                    OBJECT_END + 0x0000     
#define DYNAMICOBJECT_CASTER_X                                  OBJECT_END + 0x0001     
#define DYNAMICOBJECT_BYTES                                     OBJECT_END + 0x0002     
#define DYNAMICOBJECT_SPELLID                                   OBJECT_END + 0x0003     
#define DYNAMICOBJECT_RADIUS                                    OBJECT_END + 0x0004     
#define DYNAMICOBJECT_POS_X                                     OBJECT_END + 0x0005     
#define DYNAMICOBJECT_POS_Y                                     OBJECT_END + 0x0006     
#define DYNAMICOBJECT_POS_Z                                     OBJECT_END + 0x0007     
#define DYNAMICOBJECT_FACING                                    OBJECT_END + 0x0008     
#define DYNAMICOBJECT_CASTTIME                                  OBJECT_END + 0x0009     
#define DYNAMICOBJECT_END                                       OBJECT_END + 0x000A     

//CORPSE_FIELD_:
#define CORPSE_FIELD_OWNER                                      OBJECT_END + 0x0000     
#define CORPSE_FIELD_OWNER_EXTRA                                OBJECT_END + 0x0001     
#define CORPSE_FIELD_FACING                                     OBJECT_END + 0x0002     
#define CORPSE_FIELD_POS_X                                      OBJECT_END + 0x0003     
#define CORPSE_FIELD_POS_Y                                      OBJECT_END + 0x0004     
#define CORPSE_FIELD_POS_Z                                      OBJECT_END + 0x0005     
#define CORPSE_FIELD_DISPLAY_ID                                 OBJECT_END + 0x0006     
#define CORPSE_FIELD_ITEM                                       OBJECT_END + 0x0007     
#define CORPSE_FIELD_ITEM_2                                     OBJECT_END + 0x0008     
#define CORPSE_FIELD_ITEM_3                                     OBJECT_END + 0x0009     
#define CORPSE_FIELD_ITEM_4                                     OBJECT_END + 0x000A     
#define CORPSE_FIELD_ITEM_5                                     OBJECT_END + 0x000B     
#define CORPSE_FIELD_ITEM_6                                     OBJECT_END + 0x000C     
#define CORPSE_FIELD_ITEM_7                                     OBJECT_END + 0x000D     
#define CORPSE_FIELD_ITEM_8                                     OBJECT_END + 0x000E     
#define CORPSE_FIELD_ITEM_9                                     OBJECT_END + 0x000F     
#define CORPSE_FIELD_ITEM_10                                    OBJECT_END + 0x0010     
#define CORPSE_FIELD_ITEM_11                                    OBJECT_END + 0x0011     
#define CORPSE_FIELD_ITEM_12                                    OBJECT_END + 0x0012     
#define CORPSE_FIELD_ITEM_13                                    OBJECT_END + 0x0013     
#define CORPSE_FIELD_ITEM_14                                    OBJECT_END + 0x0014     
#define CORPSE_FIELD_ITEM_15                                    OBJECT_END + 0x0015     
#define CORPSE_FIELD_ITEM_16                                    OBJECT_END + 0x0016     
#define CORPSE_FIELD_ITEM_17                                    OBJECT_END + 0x0017     
#define CORPSE_FIELD_ITEM_18                                    OBJECT_END + 0x0018     
#define CORPSE_FIELD_ITEM_19                                    OBJECT_END + 0x0019     
#define CORPSE_FIELD_BYTES_1                                    OBJECT_END + 0x001A     
#define CORPSE_FIELD_BYTES_2                                    OBJECT_END + 0x001B     
#define CORPSE_FIELD_GUILD                                      OBJECT_END + 0x001C     
#define CORPSE_FIELD_FLAGS                                      OBJECT_END + 0x001D     
#define CORPSE_FIELD_DYNAMIC_FLAGS                              OBJECT_END + 0x001E     
#define CORPSE_FIELD_PAD                                        OBJECT_END + 0x001F     
#define CORPSE_FIELD_END                                        OBJECT_END + 0x0020     

#endif // __arcemu_UPDATEFIELDS_H
*/
