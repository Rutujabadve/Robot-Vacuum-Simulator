#ifndef SIMULATION_H
#define SIMULATION_H

#include "Environment.h"
#include "Robot.h"
#include "Algorithms.h"

// Define the different states the robot can be in
enum class RobotState {
    IDLE,
    SEEKING_DIRT,
    CLEANING,
    RETURNING_TO_DOCK,
    CHARGING
};

class Simulation {
private:
    Environment env;
    Robot robot;
    RobotState currentState;
    Path currentPath; // The route the robot is currently following

public:
    // TODO: Constructor that takes width, height for environment, 
    // starting position for robot, and max battery level.
    Simulation(int envWidth, int envHeight, Position robotStart, int maxBattery);

    // TODO: Method to run exactly ONE "tick" or "step" of the simulation.
    // In one tick, the robot should:
    // 1. Check if battery is low and change state to RETURNING_TO_DOCK if so.
    // 2. Take ONE action based on its currentState.
    void update();

    // Getters for the visualization
    const Environment& getEnvironment() const;
    const Robot& getRobot() const;
    RobotState getState() const;
    const Path& getCurrentPath() const;
};

#endif // SIMULATION_H
