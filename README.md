# Robot Vacuum Simulator

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![SFML](https://img.shields.io/badge/SFML-3.0-green.svg)

An interactive, grid-based Robot Vacuum Simulator built in C++ to demonstrate foundational concepts in robotics, pathfinding, and autonomous decision-making. 

Watch the vacuum navigate around obstacles, hunt for dirt, manage its battery, and autonomously return to its charging dock!

## 🎥 Demo
[Watch the Visualization Video]

## 🧠 How it Works

The Robot Vacuum is powered by a **Finite State Machine** (FSM) representing its "brain", which ticks continually to sense its environment, decide on a goal, and act on it. 

The primary decision tree relies on two core pathfinding algorithms:
1. **Breadth-First Search (BFS)**: Used during the `IDLE` state. The vacuum scans the entire grid evenly, radiating outward from its current position to locate the absolute *nearest* reachable tile of dirt.
2. **A* (A-Star) Pathfinding**: Once a target is found (either the nearest dirt, or the charging dock), the vacuum uses A* with a Manhattan distance heuristic to calculate the shortest, most optimal path to its goal while maneuvering seamlessly around obstacles.

### State Machine 
- 🟢 **IDLE**: Scanning the environment for dirt using BFS.
- 🔵 **SEEKING DIRT**: Following the A* path to the targeted dirt tile.
- 🟣 **CLEANING**: Removing the dirt and updating internal statistics.
- 🔴 **RETURNING TO DOCK**: Battery is critically low (< 15%). The robot drops what it is doing and uses A* to navigate straight to the charging dock.
- 🟡 **CHARGING**: Recharging battery to full capacity.

## 🚀 Running the Simulator Locally

### Requirements
- A modern C++ compiler (supports C++17)
- CMake
- [SFML 3.x](https://www.sfml-dev.org/) (`brew install sfml` on macOS)

### Build and Execute
```bash
git clone https://github.com/yourusername/Robot-Vacuum-Simulator.git
cd Robot-Vacuum-Simulator

# Build the project using CMake
cmake .
make

# Run the Simulator
./VacuumSim
```

## 🛠️ Project Architecture
- `Environment`: Maintains the physical grid, obstacles, dock, and dirt spawn locations.
- `Robot`: Maintains the physical location of the vacuum, battery life, and statistics.
- `Algorithms`: Pure functional implementations of `findNearestDirtyCell` (BFS) and `aStarSearch`.
- `Simulation`: The coordinator acting as the ticking loop, updating the FSM every cycle.
- `Display`: The visual layer abstracting away SFML boilerplate to render the live grid, A* path visualizations, and HUD.
