#ifndef TERRAINTILE_HPP
#define TERRAINTILE_HPP

#include <vector>

#include "gdal_priv.h"

#include "TileCoordinate.hpp"

#define TILE_SIZE 65 * 65
#define MASK_SIZE 256 * 256

// The maximum byte size of an uncompressed terrain tile (heights + child flags
// + water mask)
#define MAX_TERRAIN_SIZE ( TILE_SIZE * 2 ) + 1 + MASK_SIZE

class Terrain {
public:
  Terrain();
  Terrain(const char *fileName);
  Terrain(FILE *fp);

  void readFile(const char *fileName);
  
  void writeFile(FILE *fp) const;
  void writeFile(const char *fileName) const;
  std::vector<bool> mask();

  bool hasChildren() const;
  bool hasChildSW() const;
  bool hasChildSE() const;
  bool hasChildNW() const;
  bool hasChildNE() const;

  void setChildSW(bool on = true);
  void setChildSE(bool on = true);
  void setChildNW(bool on = true);
  void setChildNE(bool on = true);

  void setAllChildren(bool on = true);

  void setIsWater();
  bool isWater() const;

  void setIsLand();
  bool isLand() const;
  bool hasWaterMask() const;

  std::vector<i_terrain_height> mHeights;

private:
  char mChildren;
  char mMask[MASK_SIZE];
  size_t mMaskLength;

  enum Children {
    TERRAIN_CHILD_SW = 1,       // 2^0, bit 0
    TERRAIN_CHILD_SE = 2,       // 2^1, bit 1
    TERRAIN_CHILD_NW = 4,       // 2^2, bit 2
    TERRAIN_CHILD_NE = 8        // 2^3, bit 3
  };
};

class TerrainTile :
  public Terrain
{
public:
  TerrainTile(TileCoordinate coord);
  TerrainTile(const char *fileName, TileCoordinate coord);
  TerrainTile(const Terrain &terrain, TileCoordinate coord);

  GDALDatasetH heightsToRaster() const;
  
  TileCoordinate & getCoordinate() {
    return coord;
  }

  const TileCoordinate & getCoordinate() const {
    return const_cast<const TileCoordinate &>(coord);
  }


private:
  TileCoordinate coord;
};

#endif /* TERRAINTILE_HPP */
