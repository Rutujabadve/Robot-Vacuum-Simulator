#include "Simulation.h"
using namespace std;
// TODO: Implement the methods declared in Simulation.h here!

// Constructor
Simulation::Simulation(int envWidth, int envHeight, Position robotStart, int maxBattery) 
    : env(envWidth, envHeight), robot(robotStart, maxBattery), currentState(RobotState::IDLE) {
    
    // You might want to generate some random dirt and obstacles here!
    // Example:
    // Generate walls and corridors
    env.setCellType({2, 1}, CellType::OBSTACLE);
    env.setCellType({2, 2}, CellType::OBSTACLE);
    env.setCellType({2, 3}, CellType::OBSTACLE);
    env.setCellType({2, 4}, CellType::OBSTACLE);
    env.setCellType({2, 7}, CellType::OBSTACLE);
    env.setCellType({2, 8}, CellType::OBSTACLE);

    env.setCellType({5, 5}, CellType::OBSTACLE);
    env.setCellType({6, 5}, CellType::OBSTACLE);
    env.setCellType({7, 5}, CellType::OBSTACLE);
    env.setCellType({5, 6}, CellType::OBSTACLE);
    env.setCellType({5, 7}, CellType::OBSTACLE);

    env.setCellType({8, 2}, CellType::OBSTACLE);
    env.setCellType({9, 2}, CellType::OBSTACLE);
    env.setCellType({8, 8}, CellType::OBSTACLE);

    // Scatter plenty of dirt!
    env.setCellType({1, 1}, CellType::DIRTY);
    env.setCellType({1, 8}, CellType::DIRTY);
    env.setCellType({3, 2}, CellType::DIRTY);
    env.setCellType({4, 7}, CellType::DIRTY);
    env.setCellType({6, 2}, CellType::DIRTY);
    env.setCellType({6, 8}, CellType::DIRTY);
    env.setCellType({9, 0}, CellType::DIRTY);
    env.setCellType({9, 5}, CellType::DIRTY);
    env.setCellType({9, 9}, CellType::DIRTY);
    env.setCellType({2, 6}, CellType::DIRTY);
    env.setCellType({7, 7}, CellType::DIRTY);
    env.setCellType({0, 4}, CellType::DIRTY);
}

const Environment& Simulation::getEnvironment() const{
    return env;
}

const Robot& Simulation::getRobot() const{
    return robot;
}

RobotState Simulation::getState() const{
    return currentState;
}

const Path& Simulation::getCurrentPath() const{
    return currentPath;
}

void Simulation::update() {
    // 1. Sense: Check battery. If battery is too low (e.g., < 20) and we are not already returning or charging, change state.
    if (getRobot().getBatteryLevel() < 15 && currentState != RobotState::CHARGING && currentState != RobotState::RETURNING_TO_DOCK) {
        currentState = RobotState::RETURNING_TO_DOCK;
        currentPath.clear(); // Clear any existing paths
    }

    // 2. Decide & Act based on State:
    if (currentState == RobotState::IDLE) {
        // Find nearest dirt using BFS
        Position dirty_c = findNearestDirtyCell(env, robot.getPosition());
        
        if (dirty_c.x != -1 && dirty_c.y != -1) {
            // Dirt found, use A* to plan a path to it
            currentPath = aStarSearch(env, robot.getPosition(), dirty_c);
            if (!currentPath.empty()) {
                // The first node in currentPath is the start node itself (which the robot is already on), 
                // but let's assume A* returns the path EXCLUDING the start node, OR we handle it by not moving if it's the current position. 
                // Our A* implementation includes the start node if start == target, but otherwise we need to check.
                // Wait, our A* starts from start node, but openSet explores it. The reconstructed path INCLUDES the start node.
                // So let's pop the first node if it is the current position!
                if (currentPath.front() == robot.getPosition()) {
                    currentPath.erase(currentPath.begin());
                }

                currentState = RobotState::SEEKING_DIRT;
            }
        }
    } 
    else if (currentState == RobotState::SEEKING_DIRT) {
        // Follow the path to the dirt
        if (!currentPath.empty()) {
            Position nextStep = currentPath.front();
            currentPath.erase(currentPath.begin()); // Remove the step we are about to take
            robot.move(nextStep);
        }
        
        // If path is empty after moving, we should be at the dirt.
        if (currentPath.empty()) {
            currentState = RobotState::CLEANING;
        }
    } 
    else if (currentState == RobotState::CLEANING) {
        // Clean the dirt
        robot.clean(env);
        currentState = RobotState::IDLE;
    } 
    else if (currentState == RobotState::RETURNING_TO_DOCK) {
        // Check if we need to plan a path back to the dock
        if (currentPath.empty() && !(robot.getPosition() == env.getDockPosition())) {
            currentPath = aStarSearch(env, robot.getPosition(), env.getDockPosition());
            
            // Remove the start node if it's the current position
            if (!currentPath.empty() && currentPath.front() == robot.getPosition()) {
                currentPath.erase(currentPath.begin());
            }
        }
        
        // Follow the path to the dock
        if (!currentPath.empty()) {
            Position nextStep = currentPath.front();
            currentPath.erase(currentPath.begin());
            robot.move(nextStep);
        }
        
        // If at the dock, switch to charging
        if (robot.getPosition() == env.getDockPosition()) {
            currentState = RobotState::CHARGING;
            currentPath.clear();
        }
    } 
    else if (currentState == RobotState::CHARGING) {
        // Recharge the battery
        robot.recharge();
        currentState = RobotState::IDLE;
    }
}
