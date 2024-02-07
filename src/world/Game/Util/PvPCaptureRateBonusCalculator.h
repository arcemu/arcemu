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

#ifndef PCRBC_H
#define PCRBC_H

/// Calculates the bonus factor when capturing a world PvP objective like EPL towers
class PvPCaptureRateBonusFactorCalculator
{
private:
	unsigned int maxAdvantage;

public:

	/// Constructs the object
	///
	/// maxAdvantage - The maximum number of players to give bonus for
	PvPCaptureRateBonusFactorCalculator( unsigned int maxAdvantage )
	{
		this->maxAdvantage = maxAdvantage;
	}

	/// Calculates the bonus factor.
	///
	/// Input is the advantage (player of larger team - players of smaller team) of the superior team
	/// Returns a float between 1.0f and 2.0f
	float calculateBonusFactor( unsigned int advantage )
	{
		if( advantage < 1 )
		{
			return 0.0f;
		}

		if( advantage == 1 )
		{
			return 1.0f;
		}

		if( advantage > maxAdvantage )
		{
			advantage = maxAdvantage;
		}

		float bonusPerPlayer = 1.0f / ( maxAdvantage - 1 );
		float bonus = ( advantage - 1 ) * bonusPerPlayer;

		return 1.0f + bonus;
	}
};

#endif
