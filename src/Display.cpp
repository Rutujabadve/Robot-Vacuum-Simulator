#include "Display.h"

// Convert simulation reference from const to mutable so we can call sim.update()
Display::Display(Simulation& simulation, int cSize) 
    : window(sf::VideoMode({(unsigned int)(simulation.getEnvironment().getWidth() * cSize), (unsigned int)(simulation.getEnvironment().getHeight() * cSize + 80)}), "Robot Vacuum Simulator"),
      sim(simulation), 
      cellSize(cSize) {
    
    window.setFramerateLimit(60);
}

void Display::run() {
    sf::Clock updateClock;
    float timePerTick = 0.5f; // Run the simulation exactly one step every half second
    bool isRunning = false; // Starts paused so the user has to click START

    // Load Font for HUD
    sf::Font font;
    bool hasFont = font.openFromFile("assets/Roboto-Regular.ttf");

    // HUD Text shapes setup
    sf::Text statusText(font);
    statusText.setCharacterSize(20);
    statusText.setFillColor(sf::Color::White);
    statusText.setPosition({10.f, (float)(sim.getEnvironment().getHeight() * cellSize) + 10.f});

    sf::Text statsText(font);
    statsText.setCharacterSize(20);
    statsText.setFillColor(sf::Color::White);
    statsText.setPosition({10.f, (float)(sim.getEnvironment().getHeight() * cellSize) + 40.f});

    // Basic SFML loop
    while (window.isOpen()) {
        const Environment& env = sim.getEnvironment();

        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            
            // Check for Mouse Clicks
            if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButton->button == sf::Mouse::Button::Left) {
                    float btnX = (float)(env.getWidth() * cellSize) - 140.f;
                    float btnY = (float)(env.getHeight() * cellSize) + 20.f;
                    
                    if (mouseButton->position.x >= btnX && mouseButton->position.x <= btnX + 120.f &&
                        mouseButton->position.y >= btnY && mouseButton->position.y <= btnY + 40.f) {
                        isRunning = !isRunning; // Toggle play/pause
                    }
                }
            }
        }

        // 1. Simulation Logic Update!
        // This fires consistently every time `timePerTick` passes
        if (isRunning && updateClock.getElapsedTime().asSeconds() > timePerTick) {
            sim.update();
            updateClock.restart();
        } else if (!isRunning) {
            updateClock.restart(); // Don't build up time while paused
        }

        // 2. Clear the frame
        window.clear(sf::Color(200, 200, 200)); // Light grey grid lines

        // 3. Draw the environment Environment
        for (int x = 0; x < env.getWidth(); ++x) {
            for (int y = 0; y < env.getHeight(); ++y) {
                // Background floor tile
                sf::RectangleShape cell(sf::Vector2f((float)(cellSize - 2), (float)(cellSize - 2)));
                cell.setPosition({(float)(x * cellSize + 1), (float)(y * cellSize + 1)});
                
                CellType type = env.getCellType({x, y});
                
                // Base floor color (light wood / beige)
                cell.setFillColor(sf::Color(245, 222, 179)); 
                
                if (type == CellType::OBSTACLE) {
                    // Outer crate
                    cell.setFillColor(sf::Color(80, 80, 80)); // Dark Gray Block
                    window.draw(cell);

                    // Inner crate bevel for 3D look
                    sf::RectangleShape innerCell(sf::Vector2f((float)(cellSize - 10), (float)(cellSize - 10)));
                    innerCell.setPosition({(float)(x * cellSize + 5), (float)(y * cellSize + 5)});
                    innerCell.setFillColor(sf::Color(110, 110, 110));
                    window.draw(innerCell);
                    // Skip redrawing the generic cell below
                    continue;

                } else if (type == CellType::DOCK) {
                    // Draw base charging pad
                    cell.setFillColor(sf::Color(40, 40, 40)); 
                    window.draw(cell);

                    // Draw glowing green charge strip
                    sf::RectangleShape chargeStrip(sf::Vector2f((float)(cellSize - 20), (float)8));
                    chargeStrip.setPosition({(float)(x * cellSize + 10), (float)(y * cellSize + cellSize / 2.0f - 4)});
                    chargeStrip.setFillColor(sf::Color(0, 255, 0));
                    window.draw(chargeStrip);
                    continue;
                }
                
                window.draw(cell);

                // If it's dirty, draw specks of dirt instead of a solid colored block
                if (type == CellType::DIRTY) {
                    // Draw 5 little dirt specks pseudo-randomly placed
                    for (int i = 0; i < 5; ++i) {
                        sf::CircleShape speck(3.0f);
                        speck.setFillColor(sf::Color(101, 67, 33)); // Dark Brown
                        
                        // Use the coordinates to generate a pseudo-random offset so they stay fixed
                        float offsetX = ((x * 13 + y * 7 + i * 11) % 100) / 100.0f * (cellSize - 10) + 5;
                        float offsetY = ((x * 17 + y * 23 + i * 5) % 100) / 100.0f * (cellSize - 10) + 5;
                        
                        speck.setPosition({(float)(x * cellSize) + offsetX, (float)(y * cellSize) + offsetY});
                        window.draw(speck);
                    }
                }
            }
        }

        // 4. (Optional) Draw the path the A* algorithm planned so we can see its brain!
        const Path& path = sim.getCurrentPath();
        for (const Position& p : path) {
            sf::CircleShape pathMarker(4.0f);
            pathMarker.setFillColor(sf::Color(255, 200, 0, 180)); // Orange/Yellow dots
            pathMarker.setPosition({(float)(p.x * cellSize + cellSize * 0.5f - 4.0f), (float)(p.y * cellSize + cellSize * 0.5f - 4.0f)});
            window.draw(pathMarker);
        }

        // 5. Draw the Robot Vacuum (Roomba style)
        float rx = (float)(sim.getRobot().getPosition().x * cellSize);
        float ry = (float)(sim.getRobot().getPosition().y * cellSize);

        // Main Body (Dark Grey Circle)
        sf::CircleShape robotBase((float)cellSize / 2.0f - 4.0f);
        robotBase.setFillColor(sf::Color(40, 40, 40));
        robotBase.setOutlineThickness(2.0f);
        robotBase.setOutlineColor(sf::Color(20, 20, 20));
        robotBase.setPosition({rx + 4.0f, ry + 4.0f});
        window.draw(robotBase);

        // Inner raised section (Lighter Grey)
        sf::CircleShape robotInner((float)cellSize / 3.0f - 2.0f);
        robotInner.setFillColor(sf::Color(70, 70, 70));
        robotInner.setPosition({rx + (cellSize / 2.0f) - robotInner.getRadius(), ry + (cellSize / 2.0f) - robotInner.getRadius()});
        window.draw(robotInner);
        
        // Status LED indicating its State
        sf::CircleShape robotLED(5.0f);
        RobotState state = sim.getState();
        if (state == RobotState::IDLE) {
            robotLED.setFillColor(sf::Color::Green); // 🟢 
        } else if (state == RobotState::SEEKING_DIRT) {
            robotLED.setFillColor(sf::Color::Cyan);  // 🔵
        } else if (state == RobotState::CLEANING) {
            robotLED.setFillColor(sf::Color::Magenta); // 🟣
        } else if (state == RobotState::RETURNING_TO_DOCK) {
            robotLED.setFillColor(sf::Color::Red); // 🔴
        } else if (state == RobotState::CHARGING) {
            robotLED.setFillColor(sf::Color::Yellow); // 🟡
        }
        
        // Put the LED in the center
        robotLED.setPosition({rx + (cellSize / 2.0f) - 5.0f, ry + (cellSize / 2.0f) - 5.0f});

        window.draw(robotLED);

        // 6. Draw the HUD Background
        sf::RectangleShape hudBackground(sf::Vector2f((float)(env.getWidth() * cellSize), 80.f));
        hudBackground.setPosition({0.f, (float)(env.getHeight() * cellSize)});
        hudBackground.setFillColor(sf::Color(50, 50, 50));
        window.draw(hudBackground);

        // 7. Update and Draw HUD Text & Button
        if (hasFont) {
            std::string stateStr = "IDLE";
            if (state == RobotState::SEEKING_DIRT) stateStr = "SEEKING DIRT";
            else if (state == RobotState::CLEANING) stateStr = "CLEANING...";
            else if (state == RobotState::RETURNING_TO_DOCK) stateStr = "RETURNING TO DOCK";
            else if (state == RobotState::CHARGING) stateStr = "CHARGING...";
            
            if (!isRunning) stateStr = "PAUSED"; // Override status display if paused

            statusText.setString("Status: " + stateStr);
            statsText.setString("Battery: " + std::to_string(sim.getRobot().getBatteryLevel()) + "%   |   Cleaned: " + std::to_string(sim.getRobot().getCellsCleaned()));

            window.draw(statusText);
            window.draw(statsText);
            
            // Draw Start/Pause Button
            sf::RectangleShape button(sf::Vector2f(120.f, 40.f));
            float btnX = (float)(env.getWidth() * cellSize) - 140.f;
            float btnY = (float)(env.getHeight() * cellSize) + 20.f;
            button.setPosition({btnX, btnY});
            button.setFillColor(isRunning ? sf::Color(200, 50, 50) : sf::Color(50, 200, 50));
            window.draw(button);

            // Draw Button Text
            sf::Text btnText(font);
            btnText.setString(isRunning ? "PAUSE" : "START");
            btnText.setCharacterSize(20);
            btnText.setFillColor(sf::Color::White);
            
            // Center the text in the button manually
            float textOffset = isRunning ? 25.f : 28.f;
            btnText.setPosition({btnX + textOffset, btnY + 8.f});
            window.draw(btnText);
        }

        // 8. Push frame to screen
        window.display();
    }
}
