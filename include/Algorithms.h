#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "Environment.h"
#include <vector>

// We represent a path as a sequence of positions to step onto
using Path = std::vector<Position>;

// TODO: BFS to find the nearest reachable DIRTY cell from a given start position.
// If no dirty cell is reachable, return a Position with coordinates {-1, -1}.
Position findNearestDirtyCell(const Environment& env, Position startPos);

// TODO: A* Pathfinding to calculate the shortest path from start to target.
// It should return a sequence of Positions to move to. If unreachable, return an empty path.
Path aStarSearch(const Environment& env, Position start, Position target);

#endif // ALGORITHMS_H
