#include "stdafx.h"

//this program tends to solve Vmap height acuracy issues. 
//simply borrow a few info from collision maps 

//laugh you ass off but we are going to regnerate the maps by simply walking on all cells and checking if we are 
//very wrong about our Z possition

//about acuracy of Vmaps:
//atm : map has 64 tiles, a tile size is 533,tile has 8 cells, cell has 32 Z pos 
//64 tiles mean that minx => -17.000 and maxx = 17000
//if map is 20.000 yards then => 20k/533/8/32 => 0.14 yards acuracy ? (pretty good)

#define values_in_cell 32.0f //this is given by cell struct having a 32x32 struct
#define major_height_diff 5.0f

bool FExists(const char * filename)
{
	return GetFileAttributes(filename) != 0xFFFFFFFF;
}


void main()
{
	TerrainMgr      *_terrain;
//	unsigned int rescan_maps[]={ 0,1,30,33,36,37,47,129,169,189,209,269,289,309,329,451,469,489,509,529,530,531,532,533,534,543,559,560,562,564,566,568,572,580,585, MAXINT };
	unsigned int rescan_maps[]={ 30,33,36,37,47,129,169,189,209,269,289,309,329,451,469,489,509,529,531,532,533,534,543,559,560,562,564,566,568,572,580,585, MAXINT };
//	unsigned int rescan_maps[]={ 0, MAXINT };
	unsigned int ind=0;

	printf("This program tries to increase Vmap acuracy by borrowing info from collision maps\n");
	printf("This program does not solve multi level issues.\n");
	printf("Map list is predifined in source. You need to edit it to change list\n");

	CollideInterface.Init();

	while( rescan_maps[ind] != MAXINT )
	{
		uint32 total_valid_checks=0;
		uint32 total_checks_with_dif=0;
		uint32 total_major_height_difs=0;
		uint32 total_underdrops=0;
		float total_underdrops_diff=0;
		float total_height_diff=0;
		int cur_tile_x=MAXINT,cur_tile_y=0;

		_terrain = new TerrainMgr( vMapPath,  rescan_maps[ind], 0);

		if( !_terrain )
		{
			printf("Could not open map at %s with id %d\n",vMapPath,rescan_maps[ind]);
			ind++;
			continue; 
		}

		if( !_terrain->LoadTerrainHeader() )
		{
			printf("Could not load header for map at %s with id %d\n",vMapPath,rescan_maps[ind]);
			ind++;
			delete _terrain;
			continue;
		}

		printf("======================================================== \n" );
		printf("Processing map %d\n",rescan_maps[ind] ); 

		for(int ty=1; ty < TilesCount-1; ty++ )
		{
			for(int tx=1; tx < TilesCount-1; tx++ )
			{
				//see if we have this tile at all
				if( !CollideInterface.ActivateTile(rescan_maps[ind],tx,ty) )
					continue;

				for(float py = (ty-TilesCount/2)*TileSize; py < (ty-TilesCount/2)*TileSize + TileSize; py += _cellSize/values_in_cell )
					for(float px = (tx-TilesCount/2)*TileSize; px < (tx-TilesCount/2)*TileSize + TileSize; px += _cellSize/values_in_cell )
					{
						//get our current height from vmap
						float vmap_z = _terrain->GetLandHeight(px,py);

						//no need for further actions if this is not considered a valid position on map
						if( vmap_z == 0 || vmap_z == NO_WMO_HEIGHT )
							continue;

						//get map height while we have objects spawned on it (theoretically)
						float colide_z;
						colide_z = CollideInterface.GetHeight(rescan_maps[ind],px,py,vmap_z);
						if( colide_z == NO_WMO_HEIGHT )
							colide_z = CollideInterface.GetHeight(rescan_maps[ind],px,py,vmap_z+10);
						if( colide_z == NO_WMO_HEIGHT )
							colide_z = CollideInterface.GetHeight(rescan_maps[ind],px,py,vmap_z+20);

						if( colide_z == NO_WMO_HEIGHT )
							continue;

						//for stats
						total_valid_checks++;
						if( colide_z != vmap_z )
						{
							total_checks_with_dif++;
							total_height_diff += abs( colide_z - vmap_z );
							if(abs( colide_z - vmap_z ) > major_height_diff )
							{
//								printf("\r major diff detected with value %f at x=%f and y=%f\n", colide_z - vmap_z,px,py);
								total_major_height_difs++;
							}
							if( colide_z > vmap_z )
							{
								total_underdrops++;
								total_underdrops_diff += colide_z - vmap_z;

								//we try to change current vmap mapping to memory
								if( !_terrain->WriteNewHeightToPos( px, py, colide_z ) )
									printf("omg cannot write back value to pos x=%f and y=%f\n",px,py);
							}
						}
					}

				//we are finished with this tile so we can unload it
				CollideInterface.DeactivateTile(rescan_maps[ind],tx,ty);
				printf("\r [%d][%d] -> pct done : %d ",(int)ty,(int)tx,(uint32)(((TilesCount*ty+tx)*100.0f)/(TilesCount*TilesCount)));
			}
			printf("\r");
		}

		if( total_checks_with_dif )
		{
			printf("Map finetuning report for map : %u\n",rescan_maps[ind]);
//			printf("\t valid checks made : %u\n",total_valid_checks);
//			printf("\t differences detected in vmap : %u\n",total_checks_with_dif);
			printf("\t fall underground fixes: %u\n",total_underdrops);
//			printf("\t hight difference sum : %f \n",total_height_diff);
//			printf("\t major hight differences detected : %d\n",total_major_height_difs);
			printf("\t avg height dif change : %f\n",total_underdrops_diff/total_underdrops);

			char newfilename[500];
			sprintf(newfilename,"%s/map_%d",vMapOutPath,rescan_maps[ind]);
			_terrain->Save_map_to_file(newfilename);
		}

		//we are finished with this map
		delete _terrain;

		//next map to be processed please
		ind++;
	}
	printf("Finished all maps\n");
	getch();
}