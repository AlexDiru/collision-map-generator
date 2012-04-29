#include <iostream>
#include <string>
#include <vector>
#include <cstdarg>

//Quite procedural, but don't see benefit in spending time making OOP

// entityLayer (contains entities and items)
//    ^
// mapLayer (contains walls and cover)
//    ^
// groundLayer (contains fllor textures)

struct tilemap //all the data for a tilemap
{
	std::vector<std::string> tiles; //map data
	std::string solidTiles; //string of all the tiles the player can't walk into
	std::string coverTiles; //tiles that represent cover
};

//gets the collision map of an input map
std::vector<std::string> getCollisionMap( const tilemap& tm )
{
	std::vector<std::string> rv;

	for (int y = 0; y < tm.tiles.size(); y++)
	{
		std::string row; //to push back into vec
		for (int x = 0; x < tm.tiles[y].length(); x++)
		{
			bool solidFound = false;
			bool coverFound = false;
			for (int s = 0; s < tm.solidTiles.length(); s++)
			{
				if (tm.tiles[y][x] == tm.solidTiles[s])
				{
					//solid detected
					solidFound = true;
					row.push_back('#');
					break;
				}
			}
			for (int s = 0; s < tm.coverTiles.length(); s++)
			{
				if (tm.tiles[y][x] == tm.coverTiles[s])
				{
					coverFound = true;
					row.push_back('C');
					break;
				}
			}
			if (!solidFound && !coverFound)
				row.push_back(' ');
		}
		rv.push_back(row);
	}

	return rv;
}

//appends one collision map to another
void appendCollisionMap(tilemap& target, const std::vector<std::string>& cm)
{
	if (cm.size() != target.tiles.size())
	{
		std::cout << "Incompatible size" << std::endl;
		return;
	}
	
	for (int y = 0; y < cm.size(); y++)
		for (int x = 0; x < cm[y].size(); x++)
			if (cm[y][x] == '#') //solid
			{
				if (target.tiles[y][x] != '#') 
					target.tiles[y][x] = '#'; 
			}
			else if (cm[y][x] == 'C') //cover
				if (target.tiles[y][x] != 'C')
					target.tiles[y][x] = 'C';
	
}

std::vector<std::string> generateCollisionMap(int numMaps, ... )
{
	tilemap rv;

	va_list vl;
	va_start(vl, numMaps);

	for (int i = 0; i < numMaps; i++) //iterate maps
	{
		if (!i)
			rv.tiles = getCollisionMap(va_arg( vl, tilemap));
		else
			appendCollisionMap(rv,getCollisionMap(va_arg( vl, tilemap)));
	}

	return rv.tiles;
}

int main()
{
	tilemap groundLayer, mapLayer;

	//groundLayer
	//0 = grass
	//1 = water
	groundLayer.solidTiles = "1";
	groundLayer.tiles.push_back("1100");
	groundLayer.tiles.push_back("1000");
	groundLayer.tiles.push_back("1000");
	groundLayer.tiles.push_back("0000");

	//mapLayer
	//0 = open
	//1 = wall
	//2 = cover
	mapLayer.solidTiles = "1";
	mapLayer.coverTiles = "2";
	mapLayer.tiles.push_back("0012");
	mapLayer.tiles.push_back("0020");
	mapLayer.tiles.push_back("0000");
	mapLayer.tiles.push_back("0001");

	//entityLayer
	//nothing on this is a collision map...

	std::vector<std::string> collisionMap = generateCollisionMap(2, groundLayer, mapLayer);

	//print collisionmap

	for (int y = 0; y < collisionMap.size(); y++)
	{
		for (int x = 0; x < collisionMap[y].size(); x++)
			std::cout << collisionMap[y][x];
		std::cout << std::endl;
	}

	return 0;
}