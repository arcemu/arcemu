/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2023 <http://www.ArcEmu.org/>
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

#include <StdAfx.h>

float Math::calcAngle(float Position1X, float Position1Y, float Position2X, float Position2Y)
{
	float dx = Position2X - Position1X;
	float dy = Position2Y - Position1Y;
	double angle = 0.0f;

	// Calculate angle
	if(dx == 0.0)
	{
		if(dy == 0.0)
			angle = 0.0;
		else if(dy > 0.0)
			angle = M_PI * 0.5 /* / 2 */;
		else
			angle = M_PI * 3.0 * 0.5/* / 2 */;
	}
	else if(dy == 0.0)
	{
		if(dx > 0.0)
			angle = 0.0;
		else
			angle = M_PI;
	}
	else
	{
		if(dx < 0.0)
			angle = atanf(dy / dx) + M_PI;
		else if(dy < 0.0)
			angle = atanf(dy / dx) + (2 * M_PI);
		else
			angle = atanf(dy / dx);
	}

	// Convert to degrees
	angle = angle * float(180 / M_PI);

	// Return
	return float(angle);
}

float Math::calcRadAngle(float Position1X, float Position1Y, float Position2X, float Position2Y)
{
	double dx = double(Position2X - Position1X);
	double dy = double(Position2Y - Position1Y);
	double angle = 0.0;

	// Calculate angle
	if(dx == 0.0)
	{
		if(dy == 0.0)
			angle = 0.0;
		else if(dy > 0.0)
			angle = M_PI * 0.5/*/ 2.0*/;
		else
			angle = M_PI * 3.0 * 0.5/*/ 2.0*/;
	}
	else if(dy == 0.0)
	{
		if(dx > 0.0)
			angle = 0.0;
		else
			angle = M_PI;
	}
	else
	{
		if(dx < 0.0)
			angle = atan(dy / dx) + M_PI;
		else if(dy < 0.0)
			angle = atan(dy / dx) + (2 * M_PI);
		else
			angle = atan(dy / dx);
	}

	// Return
	return float(angle);
}

float Math::getEasyAngle(float angle)
{
	while(angle < 0)
	{
		angle = angle + 360;
	}
	while(angle >= 360)
	{
		angle = angle - 360;
	}
	return angle;
}

bool Math::inRangeYZX(const float* v1, const float* v2, const float r, const float h)
{
	const float dx = v2[0] - v1[0];
	const float dy = v2[1] - v1[1]; // elevation
	const float dz = v2[2] - v1[2];
	return (dx * dx + dz * dz) < r * r && fabsf(dy) < h;
}

float Math::CalcDistance(float OaX, float OaY, float OaZ, float ObX, float ObY, float ObZ)
{
	float xdest = OaX - ObX;
	float ydest = OaY - ObY;
	float zdest = OaZ - ObZ;
	return sqrtf(zdest * zdest + ydest * ydest + xdest * xdest);
}

bool Math::inArc(float Position1X, float Position1Y, float FOV, float Orientation, float Position2X, float Position2Y)
{
	double x = Position2X - Position1X;
	double y = Position2Y - Position1Y;

	double angle = atan2(y, x);
	if( angle < 0.0f )
		angle = 2.0 * M_PI + angle;

	angle -= Orientation;

	while( angle > M_PI )
		angle -= 2.0 * M_PI;

	while(angle < -M_PI)
		angle += 2.0 * M_PI;

	float fovRad = FOV * ( M_PI / 180.0 );

	double left = -1.0 * ( fovRad / 2.0 );
	double right = fovRad / 2.0;

	return((angle >= left) && (angle <= right));
}

bool Math::isInFront( float ax, float ay, float ao, float bx, float by )
{
	return inArc( ax, ay, 180.0f, ao, bx, by );
}
