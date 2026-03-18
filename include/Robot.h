#ifndef ROBOT_H
#define ROBOT_H

#include "Environment.h"

class Robot {
private:
    Position position;
    int batteryLevel;
    int maxBattery;
    int cellsCleaned;

public:
    // TODO: Constructor to initialize robot at a starting position with max battery
    Robot(Position startPos, int maxBat);

    // TODO: Getters
    Position getPosition() const;
    int getBatteryLevel() const;
    int getCellsCleaned() const;

    // TODO: Action - move to a new position (decrements battery by 1)
    void move(Position newPos);

    // TODO: Action - clean the current cell in the environment 
    // (decrements battery by 1, increments cellsCleaned, and calls env.cleanCell)
    void clean(Environment& env);

    // TODO: Action - recharge the battery to maxBattery
    void recharge();
};

#endif // ROBOT_H
