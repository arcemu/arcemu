/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

#ifndef OBJECT_STORAGE_H
#define OBJECT_STORAGE_H

extern SERVER_DECL SQLStorage<ItemPrototype, ArrayStorageContainer<ItemPrototype> >				ItemPrototypeStorage;
extern SERVER_DECL SQLStorage<CreatureInfo, HashMapStorageContainer<CreatureInfo> >				CreatureNameStorage;
extern SERVER_DECL SQLStorage<GameObjectInfo, HashMapStorageContainer<GameObjectInfo> >			GameObjectNameStorage;
extern SERVER_DECL SQLStorage<CreatureProto, HashMapStorageContainer<CreatureProto> >			CreatureProtoStorage;
extern SERVER_DECL SQLStorage<AreaTrigger, HashMapStorageContainer<AreaTrigger> >				AreaTriggerStorage;
extern SERVER_DECL SQLStorage<ItemPage, HashMapStorageContainer<ItemPage> >						ItemPageStorage;
extern SERVER_DECL SQLStorage<Quest, HashMapStorageContainer<Quest> >							QuestStorage;
extern SERVER_DECL SQLStorage<GossipText, HashMapStorageContainer<GossipText> >					NpcTextStorage;
extern SERVER_DECL SQLStorage<GraveyardTeleport, HashMapStorageContainer<GraveyardTeleport> >	GraveyardStorage;
extern SERVER_DECL SQLStorage<TeleportCoords, HashMapStorageContainer<TeleportCoords> >			TeleportCoordStorage;
extern SERVER_DECL SQLStorage<FishingZoneEntry, HashMapStorageContainer<FishingZoneEntry> >		FishingZoneStorage;
extern SERVER_DECL SQLStorage<MapInfo, ArrayStorageContainer<MapInfo> >							WorldMapInfoStorage;
extern SERVER_DECL SQLStorage<ZoneGuardEntry, HashMapStorageContainer<ZoneGuardEntry> >			ZoneGuardStorage;
extern SERVER_DECL SQLStorage<UnitModelSizeEntry, HashMapStorageContainer<UnitModelSizeEntry> >	UnitModelSizeStorage;

void Storage_FillTaskList(TaskList & tl);
void Storage_Cleanup();
bool Storage_ReloadTable(const char * TableName);
void Storage_LoadAdditionalTables();

extern SERVER_DECL set<string> ExtraMapCreatureTables;
extern SERVER_DECL set<string> ExtraMapGameObjectTables;

#endif


