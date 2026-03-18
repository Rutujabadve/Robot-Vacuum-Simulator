#include <iostream>
#include "Simulation.h"
#include "Display.h"

int main() {
    std::cout << "Robot Vacuum Simulator Starting..." << std::endl;
    
    // Create a 10x10 environment, robot starts at (0,0) with 25 battery to test docking
    Simulation sim(10, 10, {0, 0}, 80);
    
    // Create the display with a 60-pixel cell size
    Display display(sim, 60);
    
    // Start the visualizer and simulation loop!
    display.run();

    return 0;
}
