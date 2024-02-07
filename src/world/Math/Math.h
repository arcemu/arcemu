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

#ifndef ARCEMU_MATH_H
#define ARCEMU_MATH_H

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

/* platforms that already define M_PI in math.h */
#ifdef M_PI
#undef M_PI
#endif

#define M_PI	   3.14159265358979323846
#define M_H_PI     1.57079632679489661923
#define M_Q_PI     0.785398163397448309615
#define M_PI_FLOAT 3.14159f

class SERVER_DECL Math
{
public:
	static float calcAngle(float Position1X, float Position1Y, float Position2X, float Position2Y);
	static float calcRadAngle(float Position1X, float Position1Y, float Position2X, float Position2Y);
	static float getEasyAngle(float angle);
	static bool inRangeYZX(const float* v1, const float* v2, const float r, const float h);
	static float CalcDistance(float OaX, float OaY, float OaZ, float ObX, float ObY, float ObZ);
	static bool inArc(float Position1X, float Position1Y, float FOV, float Orientation, float Position2X, float Position2Y);
	static bool isInFront( float ax, float ay, float ao, float bx, float by );

	/// Return the difference between two values
	template< typename T > static T diff( const T a, const T b )
	{
		if( a > b )
		{
			return a - b;
		}
		else
		{
			return b - a;
		}
	}
	
	/// Based on the input value returns a value that is between min and max
	template< typename T > static T clamp( const T value, const T min, const T max )
	{
		if( value < min )
		{
			return min;
		}

		if( value > max )
		{
			return max;
		}

		return value;
	}
};

#endif
