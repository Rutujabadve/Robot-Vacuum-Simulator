#ifndef DISPLAY_H
#define DISPLAY_H

#include <SFML/Graphics.hpp>
#include "Simulation.h"

class Display {
private:
    sf::RenderWindow window;
    Simulation& sim;
    int cellSize; // Pixel size of one grid cell

public:
    // TODO: Constructor to initialize the SFML window based on the environment size 
    // and store a reference to the simulation.
    Display(Simulation& simulation, int cellSize);

    // TODO: Main rendering loop method.
    // 1. Clear the window (e.g., sf::Color::White).
    // 2. Draw the grid background.
    // 3. Draw obstacles, dirt, and dock.
    // 4. Draw the robot.
    // 5. Run sim.update() every X frames (or use a timer/clock to control speed).
    // 6. Display everything on screen.
    void run();
};

#endif // DISPLAY_H
