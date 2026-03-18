#include "Algorithms.h"
#include <queue>
#include <cmath>
#include <algorithm>
#include <set>
#include <map>
using namespace std;
// Feel free to include any other standard library headers you need (like <unordered_map> or <set>)!

Position findNearestDirtyCell(const Environment& env, Position startPos) {
    if (env.getCellType(startPos) == CellType::DIRTY) {
        return startPos;
    }

    int directions[4][2] = {{0,1},{1,0},{-1,0},{0,-1}};
    queue<pair<int,int>> q;
    set<pair<int,int>> visited;
    
    q.push({startPos.x, startPos.y});
    visited.insert({startPos.x, startPos.y});
    
    while(!q.empty()){
        int x = q.front().first; 
        int y = q.front().second;
        q.pop();
        
        for(auto di : directions){
            int nx = di[0] + x;
            int ny = di[1] + y;
            Position p1;
            p1.x = nx; 
            p1.y = ny;
            
            // Check if valid move AND not visited yet
            if(env.isValidMove(p1) && visited.find({nx, ny}) == visited.end()){
                if(env.getCellType(p1) == CellType::DIRTY){
                    return p1;
                }
                visited.insert({nx, ny});
                q.push({nx,ny});
            }
        }
    }
    
    return {-1, -1}; // Return dummy coordinate if no dirt is reachable
}

// Helper struct for Priority Queue in A*
struct Node {
    Position pos;
    int f_score;
    
    bool operator>(const Node& other) const {
        return f_score > other.f_score; // Min-heap (lowest f_score at top)
    }
};

// Calculate Manhattan distance heuristic
int heuristic(Position a, Position b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

Path aStarSearch(const Environment& env, Position start, Position target) {
    if (start.x == target.x && start.y == target.y) {
        return {start};
    }

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
    openSet.push({start, heuristic(start, target)});

    // To reconstruct the path, we map a position to its "parent" position
    std::map<pair<int,int>, Position> cameFrom;

    // gScore: cost from start along best known path
    std::map<pair<int,int>, int> gScore;
    gScore[{start.x, start.y}] = 0;

    int directions[4][2] = {{0,1}, {1,0}, {-1,0}, {0,-1}};

    while (!openSet.empty()) {
        Position current = openSet.top().pos;
        openSet.pop();

        if (current.x == target.x && current.y == target.y) {
            // Reconstruct path
            Path path;
            Position temp = current;
            while (!(temp.x == start.x && temp.y == start.y)) {
                path.push_back(temp);
                temp = cameFrom[{temp.x, temp.y}];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (auto di : directions) {
            Position neighbor = {current.x + di[0], current.y + di[1]};

            if (env.isValidMove(neighbor)) {
                // The distance from current to a neighbor is 1 step
                int tentative_gScore = gScore[{current.x, current.y}] + 1;

                // If neighbor not in gScore yet, it defaults to 0, so we check if it exists 
                // OR if the new score is better than the known score
                if (gScore.find({neighbor.x, neighbor.y}) == gScore.end() || tentative_gScore < gScore[{neighbor.x, neighbor.y}]) {
                    cameFrom[{neighbor.x, neighbor.y}] = current;
                    gScore[{neighbor.x, neighbor.y}] = tentative_gScore;
                    
                    int f_score = tentative_gScore + heuristic(neighbor, target);
                    openSet.push({neighbor, f_score});
                }
            }
        }
    }

    return {}; // Return empty path if target is unreachable
}
