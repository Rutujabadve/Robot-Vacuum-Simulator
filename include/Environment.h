#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
using namespace std;
// Enumeration to represent what is on each cell of the grid
enum class CellType { EMPTY, OBSTACLE, DIRTY, DOCK };

// Simple struct to represent 2D coordinates
struct Position {
  int x;
  int y;

  bool operator==(const Position& other) const {
    return x == other.x && y == other.y;
  }
};

class Environment {
private:
  int width;
  int height;
  std::vector<std::vector<CellType>> grid;
  Position dockPosition;

public:

  Environment(int w, int h){
    width = w;
    height = h;
    grid.resize(height,vector<CellType>(width,CellType::EMPTY));
    dockPosition = {0,0};
  };

  // TODO: Create methods to place specific items at a position
  void setCellType(Position p, CellType type){
    grid[p.x][p.y] = type;
  }
  void setDockPosition(Position p){
    dockPosition = p;
  }

  // TODO: Create getter methods
  int getWidth() const{
    return width;
  }
  int getHeight() const{
    return height;
  }
  CellType getCellType(Position p) const{
    return grid[p.x][p.y];
  }
  Position getDockPosition() const{
    return dockPosition;
  }

  // TODO: Method to clean a cell (change DIRTY to EMPTY)
  void cleanCell(Position p){
    grid[p.x][p.y] = CellType::EMPTY;;
  }

  // TODO: Method to check if a position is valid for the robot to move to
  // (It must be within the bounds of the grid, and MUST NOT be an OBSTACLE)
  bool isValidMove(Position p) const{
    return p.x >=0 && p.x < width &&p.y>=0 && p.y < height && grid[p.x][p.y] != CellType::OBSTACLE;
  }
};

#endif // ENVIRONMENT_H
