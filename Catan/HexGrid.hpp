#ifndef HEX_GRID_HPP_
#define HEX_GRID_HPP_
#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace HexGrid {
enum hexgridType { Edge, Tile, Node };
std::map<int, int> tileIdCoordinates;  // Add tile ID and coordinates
std::map<int, std::string> tileTileOffsets{{-0x20, "NW"}, {-0x22, "W"},
                                           {-0x02, "SW"}, {0x20, "SE"},
                                           {0x22, "E"},   {0x02, "NE"}};
std::map<int, std::string> tileNodeOffsets{{0x01, "N"},   {-0x10, "NW"},
                                           {-0x01, "SW"}, {0x10, "S"},
                                           {0x21, "SE"},  {0x12, "NE"}};
std::map<int, std::string> tileEdgeOffsets{{-0x10, "NW"}, {-0x11, "W"},
                                           {-0x01, "SW"}, {0x10, "SE"},
                                           {0x11, "E"},   {0x01, "NE"}};
std::string location(hexgridType type, int coordinate);
int fromLocation(hexgridType type, int tileID, std::string direction = NULL);
std::vector<int> coastalTileIDs(void);
std::vector<std::pair<int, std::string>> coastalCoordinates(void);
std::vector<int> coastalEdges(int tileID);
int tileIdInDirection(int originTile, std::string direction);
std::string directionFromTile(int originTile, int destinationTile);
std::string tileTileOffsetToDirection(int offset);
std::string tileNodeOffsetToDirection(int offset);
std::string tileEdgeOffsetToDirection(int offset);
int edgeCoordinateInDirection(int tileID, std::string direction);
int nodeCoordinateInDirection(int tileID, std::string direction);
int tileIdToCoordinate(int tileID);
int tileIdFromCoordinate(int coordinate);
int nearestTileToEdge(int edgeCoordinate);
int nearestTileToNode(int nodeCoordinate);
std::vector<int> edgesTouchingTile(int tileID);
std::vector<int> nodesTouchingTile(int tileID);
std::vector<int> nodesTouchingEdge(int edgeCoordinate);
std::set<int> legalEdgeCoordinates(void);
std::set<int> legalNodeCoordinates(void);
std::vector<int> legalTileCoordinates(void);
std::vector<int> legalTileIDs(void);
bool isLegalTile(int coordinate);
bool isLegalNode(int coordinate);
bool isLegalEdge(int coordinate);
int hexDigit(int coordinate, int digit = 1);
int coordinateFromHexDigit(int firstDigit, int secondDigit);
std::string rotateDirection(hexgridType type, std::string direction,
                            bool ccw = true);
}  // namespace HexGrid
#endif