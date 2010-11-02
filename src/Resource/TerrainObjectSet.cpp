/*
 * Copyright 2010 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "TerrainObjectSet.h"
#include "TerrainObject.h"
#include "../Engine/SurfaceSet.h"
#include <fstream>

/**
*
*/
TerrainObjectSet::TerrainObjectSet(const std::string &name) : _name(name)
{
}

/**
*
*/
TerrainObjectSet::~TerrainObjectSet()
{
	for (std::vector<TerrainObject*>::iterator i = _terrainObjects.begin(); i != _terrainObjects.end(); i++)
	{
		delete *i;
	}

}

/**
* @return pointer to an array of MCD objects
*/
std::vector<TerrainObject*> *TerrainObjectSet::getTerrainObjects()
{
	return &_terrainObjects;
}

/**
 * Loads terraindata in X-Com format directly into a
 * @param filename Filename of the MAP file.
 * @param tiles Pointer to the tile set.
 * @param xoff mapblock offset in X direction
 * @param yoff mapblock offset in Y direction
 * @sa http://www.ufopaedia.org/index.php?title=MCD
 */
void TerrainObjectSet::load(const std::string &filename)
{
	MCD* mcd;

	// Load file
	std::ifstream mapFile (filename.c_str(), std::ios::in | std::ios::binary);
	if (!mapFile)
	{
		throw "Failed to load MCD";
	}
	
	mcd = new MCD();
	while (mapFile.read((char*)mcd, sizeof(MCD)))
	{
		_terrainObjects.push_back(new TerrainObject(mcd));
		mcd = new MCD();
	}
	delete mcd;

	if (!mapFile.eof())
	{
		throw "Invalid data from file";
	}

	mapFile.close();
}

/**
 * Links the terrain objects with corresponding sprites
 * @param sprites pointer to the surfaceset to link with
 */
void TerrainObjectSet::linkSprites(SurfaceSet *sprites)
{
	for (std::vector<TerrainObject*>::iterator i = _terrainObjects.begin(); i != _terrainObjects.end(); i++)
	{
		TerrainObject *tob = (TerrainObject*)*i;
		for (int frame = 0; frame < 8; frame++)
		{
			tob->setSprite(sprites->getFrame(tob->getSpriteIndex(frame)),frame);
		}
	}
}