/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#ifndef _TERRAINMGR_H
#define _TERRAINMGR_H

#define MAP_RESOLUTION 256
#define CellsPerTile 8

typedef struct
{
    uint16    AreaID[16/CellsPerTile][16/CellsPerTile];
    uint8    LiquidType[16/CellsPerTile][16/CellsPerTile];
    float    LiquidLevel[16/CellsPerTile][16/CellsPerTile];
    float    Z[MAP_RESOLUTION/CellsPerTile][MAP_RESOLUTION/CellsPerTile];
}CellTerrainInformation; 

#define FL2UINT (uint32)
#define TERRAIN_HEADER_SIZE 1048576	 // size of [512][512] array.

/* @class TerrainMgr
   TerrainMgr maintains the MapCellInfo information for accessing water levels,
   water types, Z levels, area id's, and walkable graph information.

   TerrainMgr can dynamically allocate and un-allocate cell information for main
   continents as their information is *quite* large and not needed at all times
   to be loaded. Unloading this in idle times is a nice way to save memory.

   However, on instanced maps, we would want to keep the cell's information
   loaded at all times as it is a lot smaller and we can have multiple instances
   wanting to access this information at once.
  */

//#include <string>

class TerrainMgr
{
public:
	/* Initializes the terrain interface, allocates all required arrays, and sets 
	   all variables.
	   Parameter 1: The path to the packed map files.
	   Parameter 2: The map that we'll be retrieving information from.
	   Parameter 3: Controls whether the map will unload information when it's not
					in use.
	   No return value.
	  */
	TerrainMgr(std::string MapPath, uint32 MapId, bool Instanced);

	/* Cleans up all arrays, and unloads any pending cell information.
	   No parameters.
	   No return value.
	  */
	~TerrainMgr();

	/* If we're a non-instanced map, we'll unload the cell information as it's
	   not needed.
	   Parameter 1: The x co-ordinate of the cell that's gone idle.
	   Parameter 2: The y co-ordinate of the cell that's gone idle.
	   No return value.
	  */
	void CellGoneIdle(uint32 x, uint32 y);

	/* Loads the cell information if it has not already been loaded.
	   Parameter 1: The x co-ordinate of the cell that's gone active.
	   Parameter 2: The y co-ordinate of the cell that's gone active.
	   No return value.
	  */
	void CellGoneActive(uint32 x, uint32 y);

	/* Information retrieval functions
	   These functions all take the same input values, an x and y global co-ordinate.
	   They will all return 0 if the cell information is not loaded or does not exist,
	   apart from the water function which will return '-999999.0'.
	  */
	float  GetLandHeight(float x, float y);
	float  GetWaterHeight(float x, float y);
	uint8  GetWaterType(float x, float y);
	uint8  GetWalkableState(float x, float y);
	uint16 GetAreaID(float x, float y);

private:

	/// MapPath contains the location of all mapfiles.
	std::string mapPath;

	/// Map ID
	uint32 mapId;

	/// Are we an instance?
	bool Instance;

#ifndef USE_MEMORY_MAPPING_FOR_MAPS
	
	/// Our main file descriptor for accessing the binary terrain file.
	FILE * FileDescriptor;

	/// This holds the offsets of the cell information for each cell.
	uint32 CellOffsets[_sizeX][_sizeY];

#else

	/// Mapped file handle
	HANDLE hMappedFile;
	HANDLE hMap;
	uint32 mFileSize;

	/// This holds the offsets of the cell information for each cell.
	uint32 CellOffsets[_sizeX][_sizeY];
	uint8 * m_Memory;

#endif

	/// Our storage array. This contains pointers to all allocated CellInfo's.
	CellTerrainInformation *** CellInformation;
	
public:
	/* Initializes the file descriptor and readys it for data retreival.
	   No parameters taken.
	   Returns true if the index was read successfully, false if not.
	  */
	bool LoadTerrainHeader();

protected:
	/* Retrieves the cell data for the specified co-ordinates from the file and sets it in
	   the CellInformation array.
	   Parameter 1: x co-ordinate of the cell information to load.
	   Parameter 2: y co-ordinate of the cell information to load.
	   Returns true if the cell information exists and was loaded, false if not.
	  */
	bool LoadCellInformation(uint32 x, uint32 y);

	/* Unloads the cell data at the specified co-ordinates and frees the memory.
	   Parameter 1: x co-ordinate of the cell information to free.
	   Parameter 2: y co-ordinate of the cell information to free.
	   Returns true if the free was successful, otherwise false.
	  */
	bool UnloadCellInformation(uint32 x, uint32 y);

	/* Gets the offset for the specified cell from the cached offset index.
	   Parameter 1: cell x co-ordinate.
	   Parameter 2: cell y co-ordinate.
	   Returns the offset in bytes of that cell's information, or 0 if it doesn't exist.
	  */
	uint32 GetCellInformationOffset(uint32 x, uint32 y)
	{
		return CellOffsets[x][y];
	}

	/* Gets a cell information pointer so that another function can access its data.
	   Parameter 1: cell x co-ordinate.
	   Parameter 2: cell y co-ordinate.
	   Returns the memory address of the information for that cell.
	  */
	CellTerrainInformation* GetCellInformation(uint32 x, uint32 y)
	{
		return CellInformation[x][y];
	}

	/* Converts a global x co-ordinate into a cell x co-ordinate.
	   Parameter 1: global x co-ordinate.
	   Returns the cell x co-ordinate.
	  */
	uint32 ConvertGlobalXCoordinate(float x)
	{
		return FL2UINT((_maxX-x)/_cellSize);
	}

	/* Converts a global y co-ordinate into a cell y co-ordinate.
	   Parameter 1: global y co-ordinate.
	   Returns the cell y co-ordinate.
	*/
	uint32 ConvertGlobalYCoordinate(float y)
	{
		return FL2UINT((_maxY-y)/_cellSize);
	}

	/* Converts a global x co-ordinate into a INTERNAL cell x co-ordinate.
	   Parameter 1: global x co-ordinate.
	   Parameter 2: the cell x co-ordinate.
	   Returns the internal x co-ordinate.
	*/
	float ConvertInternalXCoordinate(float x, uint32 cellx)
	{
		float X = (_maxX - x);
		X -= (cellx * _cellSize);
		return X;
	}

	/* Converts a global y co-ordinate into a INTERNAL cell y co-ordinate.
	   Parameter 1: global y co-ordinate.
	   Parameter 2: the cell y co-ordinate.
	   Returns the internal y co-ordinate.
	*/
	float ConvertInternalYCoordinate(float y, uint32 celly)
	{
		float Y = (_maxY - y);
		Y -= (celly * _cellSize);
		return Y;
	}

	/* Checks whether a cell information is loaded or not.
	  */
	bool CellInformationLoaded(uint32 x, uint32 y)
	{
		if(CellInformation[x][y] != 0)
			return true;
		else
			return false;
	}

	/* Converts the internal co-ordinate to an index in the 
	   2 dimension areaid, or liquid type arrays.
	  */
	uint32 ConvertTo2dArray(float c)
	{
		return FL2UINT(c*(16/CellsPerTile/_cellSize));
	}

	/* Checks that the co-ordinates are within range.
	  */
	bool AreCoordinatesValid(float x, float y)
	{
		if(x > _maxX || x < _minX)
			return false;
		if(y > _maxY || y < _minY)
			return false;
		return true;
	}

public:
	//this is in memory only
	bool WriteNewHeightToPos(float x, float y, float new_z);
	//write practicall results
	bool Save_map_to_file(char *new_filename);
};

#endif
