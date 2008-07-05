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

extern SQLStorage<ItemPrototype, ArrayStorageContainer<ItemPrototype> >				ItemPrototypeStorage;
extern SQLStorage<CreatureInfo, HashMapStorageContainer<CreatureInfo> >				CreatureNameStorage;
extern SQLStorage<GameObjectInfo, HashMapStorageContainer<GameObjectInfo> >			GameObjectNameStorage;
//extern SQLStorage<CreatureProto, HashMapStorageContainer<CreatureProto> >			CreatureProtoStorage;
//extern SQLStorage<AreaTrigger, HashMapStorageContainer<AreaTrigger> >				AreaTriggerStorage;
extern SQLStorage<ItemPage, HashMapStorageContainer<ItemPage> >						ItemPageStorage;
extern SQLStorage<Quest, HashMapStorageContainer<Quest> >							QuestStorage;
extern SQLStorage<GossipText, HashMapStorageContainer<GossipText> >					NpcTextStorage;
//extern SQLStorage<SpellExtraInfo, HashMapStorageContainer<SpellExtraInfo> >			SpellExtraStorage;
//extern SQLStorage<GraveyardTeleport, HashMapStorageContainer<GraveyardTeleport> >	GraveyardStorage;
//extern SQLStorage<TeleportCoords, HashMapStorageContainer<TeleportCoords> >			TeleportCoordStorage;
//extern SQLStorage<FishingZoneEntry, HashMapStorageContainer<FishingZoneEntry> >		FishingZoneStorage;
extern SQLStorage<MapInfo, HashMapStorageContainer<MapInfo> >						WorldMapInfoStorage;

/*void Storage_FillTaskList(TaskList & tl);*/
void Storage_Cleanup();
void Storage_Load();
bool Storage_ReloadTable(const char * TableName);

#endif


