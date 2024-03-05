/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2024 <http://www.ArcEmu.org/>
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

#ifndef A_PIOOS_H_
#define A_PIOOS_H_

class PythonInstanceOOScript : public InstanceScript
{
private:
	PyObject *pyObject;

public:
	PythonInstanceOOScript( MapMgr *mgr, PyObject *pyObject );
	~PythonInstanceOOScript();

	void setObject( PyObject *pyObject );

	void OnPlayerDeath( Player* victim, Unit* killer );
	void OnPlayerEnter( Player* player );
	void OnAreaTrigger( Player* player, uint32 areaId );
	void OnZoneChange( Player* player, uint32 newZone, uint32 oldZone );
	void OnCreatureDeath( Creature* victim, Unit* killer );
	void OnCreaturePushToWorld( Creature* creature );
	void OnGameObjectActivate( GameObject* gameObject, Player* player );
	void OnGameObjectPushToWorld( GameObject* gameObject );
	void OnLoad();
	void Destroy();
};

#endif
