#include <HexGrid.hpp>

template <typename... Args>
std::string string_format(const std::string& format, Args... args) {
  // Formats given string based on printf directives.
  int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
  if (size_s <= 0) {
    throw std::runtime_error("Error during formatting.");
  }
  auto size = static_cast<size_t>(size_s);
  auto buf = std::make_unique<char[]>(size);
  std::snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(), buf.get() + size - 1);
}
namespace HexGrid {
std::string location(hexgridType type, int coordinate) {
  // Returns a formatted string representing the coordinate. The format depends
  // on the coordinate type.

  //   Tiles look like: 1, 12
  //   Nodes look like: (1 NW), (12 S)
  //   Edges look like: (1 NW), (12 SE)

  if (type == hexgridType::Tile) {
    return std::to_string(coordinate);
  } else if (type == hexgridType::Node) {
    int tileID = nearestTileToNode(coordinate);
    std::string direction =
        tileTileOffsetToDirection(coordinate - tileIdToCoordinate(tileID));
    return string_format("(%d %s)", tileID, direction);
  } else if (type == hexgridType::Edge) {
    int tileID = nearestTileToEdge(coordinate);
    std::string direction =
        tileEdgeOffsetToDirection(coordinate - tileIdToCoordinate(tileID));
    return string_format("(%d %s)", tileID, direction);
  }
  return "";
}
int fromLocation(hexgridType type, int tileID, std::string direction = "") {
  // Returns an integer coordinate in this module's hexadecimal coordinate
  // system.

  if (type == hexgridType::Tile) {
    if (direction != "") {
      return -1;
    }
    return tileIdToCoordinate(tileID);
  } else if (type == hexgridType::Node) {
    return nodeCoordinateInDirection(tileID, direction);
  } else if (type == hexgridType::Edge) {
    return edgeCoordinateInDirection(tileID, direction);
  }
  return -1;
}
std::vector<int> coastalTileIDs(void) {
  // Returns a list of tile identifiers which lie on the border of the grid.
  std::vector<int> coastalTiles;
  std::vector<int> tiles = legalTileIDs();
  for (int i = 0; i < tiles.size(); i++) {
    if (coastalEdges(tiles[i]).size() > 0) {
      coastalTiles.push_back(tiles[i]);
    }
  }
  return coastalTiles;
}
std::vector<std::pair<int, std::string>> coastalCoordinates(void) {
  // A coastal coord is a pair: (tile id, direction).
  std::vector<std::pair<int, std::string>> coasts;
  std::vector<int> IDs = coastalTileIDs();
  for (int i = 0; i < IDs.size(); i++) {
    int tileCoordinate = tileIdToCoordinate(IDs[i]);
    std::vector<int> edgeCoordinates = coastalEdges(IDs[i]);
    for (int j = 0; j < edgeCoordinates.size(); i++) {
      std::string dirction =
          tileEdgeOffsetToDirection(edgeCoordinates[j] - tileCoordinate);
      if (tileIdInDirection(IDs[i], dirction) == -1) {
        std::pair<int, std::string> coast{IDs[i], dirction};
        coasts.push_back(coast);
      }
    }
    return coasts;
  }
}
std::vector<int> coastalEdges(int tileID) {
  // Returns a list of coastal edge coordinate.
  std::vector<int> edges;
  int tileCoordinate = tileIdToCoordinate(tileID);
  std::vector<int> touchingEdges = edgesTouchingTile(tileID);
  for (int i = 0; i < touchingEdges.size(); i++) {
    std::string direction =
        tileEdgeOffsetToDirection(touchingEdges[i] - tileCoordinate);
    if (tileIdInDirection(tileID, direction) == -1) {
      edges.push_back(touchingEdges[i]);
    }
  }
  return edges;
}
int tileIdInDirection(int originTile, std::string direction) {
  // Finds neighbouring tile in a given direction.
  int originCoordinate = tileIdToCoordinate(originTile);
  std::map<int, std::string>::const_iterator iter = tileTileOffsets.cbegin();
  for (; iter != tileTileOffsets.cend(); iter++) {
    if (direction == iter->second) {
      int destinstionCoordinate = originCoordinate + iter->first;
      if (isLegalTile(destinstionCoordinate)) {
        return tileIdFromCoordinate(destinstionCoordinate);
      }
    }
  }
  return -1;
}
std::string directionFromTile(int originTile, int destinationTile) {
  // Finds the direction in with a neighbouring tile lies.
  int originCoordinate = tileIdToCoordinate(originTile);
  int destinationCoordinate = tileIdToCoordinate(destinationTile);
  std::string direction =
      tileTileOffsetToDirection(destinationCoordinate - originCoordinate);
  return direction;
}
std::string tileTileOffsetToDirection(int offset) {
  // Get the cardinal direction of a tile-tile offset.
  if (tileTileOffsets.find(offset) != tileTileOffsets.cend()) {
    return tileTileOffsets[offset];
  }
  return "";
}
std::string tileNodeOffsetToDirection(int offset) {
  // Get the cardinal direction of a tile-node offset.
  if (tileNodeOffsets.find(offset) != tileNodeOffsets.cend()) {
    return tileNodeOffsets[offset];
  }
  return "";
}
std::string tileEdgeOffsetToDirection(int offset) {
  // Get the cardinal direction of a tile-edge offset.
  if (tileEdgeOffsets.find(offset) != tileEdgeOffsets.cend()) {
    return tileEdgeOffsets[offset];
  }
  return "";
}
int edgeCoordinateInDirection(int tileID, std::string direction) {
  // Returns the edge coordinate in the given direction.
  int tileCoordinate = tileIdToCoordinate(tileID);
  std::vector<int> edges = edgesTouchingTile(tileID);
  for (int i = 0; i < edges.size(); i++) {
    if (tileEdgeOffsetToDirection(edges[i] - tileCoordinate) == direction) {
      return edges[i];
    }
  }
  return -1;
}
int nodeCoordinateInDirection(int tileID, std::string direction) {
  // Returns the node coordinate in the given direction.
  int tileCoordinate = tileIdToCoordinate(tileID);
  std::vector<int> nodes = nodesTouchingTile(tileID);
  for (int i = 0; i < nodes.size(); i++) {
    if (tileEdgeOffsetToDirection(nodes[i] - tileCoordinate) == direction) {
      return nodes[i];
    }
  }
  return -1;
}
int tileIdToCoordinate(int offset) {
  // Convert a tile ID to its corresponding grid coordinate.
  if (tileIdCoordinates.find(offset) != tileIdCoordinates.cend()) {
    return tileIdCoordinates[offset];
  }
  return -1;
}
int tileIdFromCoordinate(int coordinate) {
  // Convert a tile coordinate to its corresponding tile ID.
  std::map<int, int>::const_iterator iter = tileIdCoordinates.cbegin();
  for (; iter != tileIdCoordinates.cend(); iter++) {
    if (iter->second == coordinate) {
      return iter->first;
    }
  }
  return -1;
}
int nearestTileToEdge(int edgeCoordinate) {
  // Get the first tile found adjacent to the given edge.
  std::vector<int> tiles = legalTileIDs();
  for (int i = 0; i < tiles.size(); i++) {
    if (tileEdgeOffsets.find(edgeCoordinate - tileIdToCoordinate(tiles[i])) !=
        tileEdgeOffsets.cend()) {
      return tiles[i];
    }
  }
  return -1;
}
int nearestTileToNode(int nodeCoordinate) {
  // Get the first tile found adjacent to the given node.
  std::vector<int> tiles = legalTileIDs();
  for (int i = 0; i < tiles.size(); i++) {
    if (tileNodeOffsets.find(nodeCoordinate - tileIdToCoordinate(tiles[i])) !=
        tileNodeOffsets.cend()) {
      return tiles[i];
    }
  }
  return -1;
}
std::vector<int> edgesTouchingTile(int tileID) {
  // Get a list of edge coordinates touching the given tile.
  int tileCoordinate = tileIdToCoordinate(tileID);
  std::vector<int> edges;
  for (auto& element : tileEdgeOffsets) {
    edges.push_back(tileCoordinate + element.first);
  }
  return edges;
}
std::vector<int> nodesTouchingTile(int tileID) {
  // Get a list of node coordinates touching the given tile.
  int tileCoordinate = tileIdToCoordinate(tileID);
  std::vector<int> nodes;
  for (auto& element : tileNodeOffsets) {
    nodes.push_back(tileCoordinate + element.first);
  }
  return nodes;
}
std::vector<int> nodesTouchingEdge(int edgeCoordinate) {
  // Returns the two node coordinates which are on the given edge coordinate.
  int a = hexDigit(edgeCoordinate, 1);
  int b = hexDigit(edgeCoordinate, 2);
  std::vector<int> nodes;
  if (a % 2 == 0 && b % 2 == 0) {
    nodes.push_back(coordinateFromHexDigit(a, b + 1));
    nodes.push_back(coordinateFromHexDigit(a + 1, b));
  } else {
    nodes.push_back(coordinateFromHexDigit(a, b));
    nodes.push_back(coordinateFromHexDigit(a + 1, b + 1));
  }
  return nodes;
}
std::set<int> legalEdgeCoordinates(void) {
  // Return all legal edge coordinates on the grid.
  std::set<int> edges;
  std::vector<int> tiles = legalTileIDs();
  for (int i = 0; i < tiles.size(); i++) {
    std::vector<int> touchingEdges = edgesTouchingTile(tiles[i]);
    for (int j = 0; j < touchingEdges.size(); j++) {
      edges.insert(touchingEdges[j]);
    }
  }
  return edges;
}
std::set<int> legalNodeCoordinates(void) {
  // Return all legal node coordinates on the grid.
  std::set<int> nodes;
  std::vector<int> tiles = legalTileIDs();
  for (int i = 0; i < tiles.size(); i++) {
    std::vector<int> touchingNodes = nodesTouchingTile(tiles[i]);
    for (int j = 0; j < touchingNodes.size(); j++) {
      nodes.insert(touchingNodes[j]);
    }
  }
  return nodes;
}
std::vector<int> legalTileCoordinates(void) {
  // Return all legal tile coordinates on the grid.
  std::vector<int> coordinates;
  std::map<int, int>::const_iterator iter = tileIdCoordinates.cbegin();
  for (; iter != tileIdCoordinates.cend(); iter++) {
    coordinates.push_back(iter->second);
  }
  return coordinates;
}
std::vector<int> legalTileIDs(void) {
  // Return all legal tile IDs on the grid.
  std::vector<int> IDs;
  std::map<int, int>::const_iterator iter = tileIdCoordinates.cbegin();
  for (; iter != tileIdCoordinates.cend(); iter++) {
    IDs.push_back(iter->first);
  }
  return IDs;
}
bool isLegalTile(int coordinate) {
  // Checks if a tile exists in the given coordinate.
  std::vector<int> coordinates = legalTileCoordinates();
  return std::find(coordinates.begin(), coordinates.end(), coordinate) !=
         coordinates.end();
}
bool isLegalEdge(int coordinate) {
  // Checks if a tile exists in the given coordinate.
  std::set<int> coordinates = legalNodeCoordinates();
  return std::find(coordinates.begin(), coordinates.end(), coordinate) !=
         coordinates.end();
}
bool isLegalNode(int coordinate) {
  // Checks if a tile exists in the given coordinate.
  std::set<int> coordinates = legalNodeCoordinates();
  return std::find(coordinates.begin(), coordinates.end(), coordinate) !=
         coordinates.end();
}
int hexDigit(int coordinate, int digit = 1) {
  // Returns either the first or second digit of the hexadecimal representation
  // of the given coordinate.
  if (digit != 1 && digit != 2) {
    return -1;
  }
  std::stringstream stream;
  stream << std::hex << coordinate;
  std::string hex = stream.str();
  return std::stoul(std::string(1, hex[digit - 1]), nullptr, 16);
}
int coordinateFromHexDigit(int firstDigit, int secondDigit) {
  // Returns an integer representing the hexadecimal coordinate with the two
  // given hexadecimal digits.
  return firstDigit * 16 + secondDigit;
}
std::string rotateDirection(hexgridType type, std::string direction,
                            bool ccw = true) {
  // Takes a direction string associated with a type of hexgrid element, and
  // rotates it one tick in the given direction.
  std::vector<std::string> directions;
  if (type == hexgridType::Node || type == hexgridType::Edge) {
    if (ccw) {
      directions = {"NW", "W", "SW", "SE", "E", "NE", "NW"};
    } else {
      directions = {"NW", "NE", "E", "SE", "SW", "W", "NW"};
    }
  } else {
    if (ccw) {
      directions = {"N", "NW", "SW", "S", "SE", "NE", "N"};
    } else {
      directions = {"N", "NE", "SE", "S", "SW", "NW", "N"};
    }
  }
  std::vector<std::string>::iterator i =
      std::find(directions.begin(), directions.end(), direction);
  if (i != directions.end()) {
    return *++i;
  }
  return "";
}
}  // namespace HexGrid