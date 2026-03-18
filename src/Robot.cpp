#include "Robot.h"

// TODO: Implement the methods declared in Robot.h here!

Robot::Robot(Position startPos, int maxBat) {
    position = startPos;
    maxBattery = maxBat;
    batteryLevel = maxBat;
    cellsCleaned = 0;
}

Position Robot::getPosition() const {
    return position;
}

int Robot::getBatteryLevel() const {
    return batteryLevel;
}

int Robot::getCellsCleaned() const {
    return cellsCleaned;
}

void Robot::move(Position newPos){
    position = newPos;
    batteryLevel--;
}

void Robot::clean(Environment& env){
    env.cleanCell(position);
    batteryLevel--;
    cellsCleaned++;
}

void Robot::recharge(){
    batteryLevel = maxBattery;
}
