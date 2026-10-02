//
// Created by danie on 3/19/2025.
//

#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
#include "Reader.h"
#include "Graph.h"

/**
 * @brief The menu class provides a user interface for interacting with the route planning system.
 */
class menu {

private:
public:
    /**
     * @brief Constructs the menu and displays a welcome message.
     */
    menu();

    /**
     * @brief Displays the main menu and handles user input for different modes.
     *
     * Options:
     * - Normal Mode: Display basic graph information.
     * - Batch Mode: Process input and generate output in batch mode.
     * - Shortest Path: Calculate and display the shortest path between two nodes.
     */
    void mainMenu();

    /**
     * @brief Returns to the previous menu or repeats commands.
     */
    void backToMenu(menu& m);
};

#endif // MENU_H
