/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
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

#ifndef OBJECT_STORAGE_H
#define OBJECT_STORAGE_H

extern SERVER_DECL SQLStorage<ItemPrototype, ArrayStorageContainer<ItemPrototype> >				ItemPrototypeStorage;
extern SERVER_DECL SQLStorage<ItemName, ArrayStorageContainer<ItemName> > ItemNameStorage;
extern SERVER_DECL SQLStorage<CreatureInfo, HashMapStorageContainer<CreatureInfo> >				CreatureNameStorage;
extern SERVER_DECL SQLStorage<GameObjectInfo, HashMapStorageContainer<GameObjectInfo> >			GameObjectNameStorage;
extern SERVER_DECL SQLStorage<CreatureProto, HashMapStorageContainer<CreatureProto> >			CreatureProtoStorage;
extern SERVER_DECL SQLStorage<DisplayBounding, HashMapStorageContainer<DisplayBounding> >		DisplayBoundingStorage;
extern SERVER_DECL SQLStorage<VendorRestrictionEntry, ArrayStorageContainer<VendorRestrictionEntry> > VendorRestrictionEntryStorage;
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
extern SERVER_DECL SQLStorage<WorldStringTable, HashMapStorageContainer<WorldStringTable> >	WorldStringTableStorage;
extern SERVER_DECL SQLStorage<WorldBroadCast, HashMapStorageContainer<WorldBroadCast> >	WorldBroadCastStorage;
extern SERVER_DECL SQLStorage<BGMaster, HashMapStorageContainer<BGMaster> >						BGMasterStorage;
extern SERVER_DECL SQLStorage< SpellClickSpell, HashMapStorageContainer< SpellClickSpell > >		SpellClickSpellStorage;
extern SERVER_DECL SQLStorage< TotemDisplayIdEntry, HashMapStorageContainer< TotemDisplayIdEntry > > TotemDisplayIdStorage;
extern SERVER_DECL SQLStorage<PointOfInterest, HashMapStorageContainer<PointOfInterest> >		PointOfInterestStorage;

void Storage_FillTaskList(TaskList & tl);
void Storage_Cleanup();
bool Storage_ReloadTable(const char* TableName);
void Storage_LoadAdditionalTables();

extern SERVER_DECL set<string> ExtraMapCreatureTables;
extern SERVER_DECL set<string> ExtraMapGameObjectTables;

#endif


