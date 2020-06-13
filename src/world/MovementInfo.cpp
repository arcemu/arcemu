/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2020 <http://www.ArcEmu.org/>
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

#include "StdAfx.h"

MovementInfo::MovementInfo(){
	time = 0;
	pitch = 0.0f;
	redirectSin = 0.0f;
	redirectCos = 0.0f;
	redirect2DSpeed = 0.0f;
	unk11 = 0;
	unk12 = 0;
	unk13 = 0;
	unklast = 0;
	unk_230 = 0;
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	orientation = 0.0f;
	flags = 0;
	redirectVelocity = 0.0f;
	transGuid = 0;
	transX = 0.0f;
	transY = 0.0f;
	transZ = 0.0f;
	transO = 0.0f;
	transUnk = 0.0f;
	transUnk_2 = 0;
}

void MovementInfo::init(WorldPacket & data)
{
	transGuid = 0;
	unk13 = 0;
	data >> flags >> unk_230 >> time;
	data >> x >> y >> z >> orientation;

	if(flags & MOVEFLAG_TRANSPORT)
	{
		data >> transGuid >> transX >> transY >> transZ >> transO >> transUnk >> transUnk_2;
	}
	if(flags & (MOVEFLAG_SWIMMING | MOVEFLAG_AIR_SWIMMING) || unk_230 & 0x20)
	{
		data >> pitch;
	}
	if(flags & MOVEFLAG_REDIRECTED)
	{
		data >> redirectVelocity >> redirectSin >> redirectCos >> redirect2DSpeed;
	}
	if(flags & MOVEFLAG_SPLINE_MOVER)
	{
		data >> unk12;
	}

	data >> unklast;
	if(data.rpos() != data.wpos())
	{
		if(data.rpos() + 4 == data.wpos())
			data >> unk13;
		else
			LOG_DEBUG("Extra bits of movement packet left");
	}
}

void MovementInfo::write(WorldPacket & data)
{
	data << flags << unk_230 << Arcemu::Shared::Util::getMSTime();

	data << x << y << z << orientation;

	if(flags & MOVEFLAG_TRANSPORT)
	{
		data << transGuid << transX << transY << transZ << transO << transUnk << transUnk_2;
	}
	if(flags & MOVEFLAG_SWIMMING)
	{
		data << pitch;
	}
	if(flags & MOVEFLAG_FALLING)
	{
		data << redirectVelocity << redirectSin << redirectCos << redirect2DSpeed;
	}
	if(flags & MOVEFLAG_SPLINE_MOVER)
	{
		data << unk12;
	}
	data << unklast;
	if(unk13)
		data << unk13;
}
