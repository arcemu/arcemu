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

#ifndef PROGRESS_H
#define PROGRESS_H

namespace Arcemu
{
	namespace Shared
	{

		class Progress
		{
		private:
			unsigned int current;
			unsigned int max;

		public:
			Progress( unsigned int max = 100, unsigned int initial = 0 )
			{
				this->max = max;
				this->current = initial;
			}

			unsigned int getCurrent() const
			{
				return current;
			}

			unsigned int getMax() const
			{
				return max;
			}

			void setCurrent( unsigned int current )
			{
				this->current = current;
			}

			void setMax( unsigned int max )
			{
				this->max = max;
			}

			unsigned int getPercent() const
			{
				return ( current * 100 ) / max;
			}

			void setPercent( unsigned int percent )
			{
				current = ( max * percent ) / 100;
			}

			void advanceBy( int amount )
			{
				if( ( ( amount > 0 ) && ( current < max ) ) ||
					( ( amount < 0 ) && ( current > 0 ) )
				)
				{
					int temp = current;

					temp += amount;

					if( temp < 0 )
					{
						temp = 0;
					}
					else
					if( static_cast< unsigned int >( temp ) > max )
					{
						temp = max;
					}

					current = temp;
				}
			}
		};

	}
}

#endif
