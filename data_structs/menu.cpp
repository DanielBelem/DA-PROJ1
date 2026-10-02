//
// Created by danie on 3/19/2025.
//

#include "menu.h"


#define BATCH_MODE 1
#define SHORTEST_PATH 2
#define RESTRICTED_ROUTE 3
#define ENVIRONMENTALLY_FRIENDLY 4
#define ENVIRONMENTAL_ALTERNATIVE 5
#define QUIT 0

#include "batcher.h"
#include "exercises.h"


/**
 * @brief Constructs the menu and displays a welcome message.
 */
menu::menu() {
    std::cout << "Welcome to the menu\n";
}

/**
 * @brief Displays the main menu and handles user input for different modes.
 *
 * Modes:
 * 1 - Normal Mode: Displays the number of vertices in the driving graph.
 * 2 - Batch Mode: Processes batch files and generates an output file.
 * 3 - Shortest Path: Finds the shortest path between two locations using Dijkstra's algorithm.
 */

void menu::mainMenu() {
    std::cout << "=====================================\n";
    std::cout << "          Route Planner Menu         \n";
    std::cout << "=====================================\n";
    std::cout << " 1. Batch Mode\n";
    std::cout << " 2. Shortest Path\n";
    std::cout << " 3. Restricted Route\n";
    std::cout << " 4. Environmentally Friendly Route\n";
    std::cout << " 5. Environmental Alternative\n";
    std::cout << " 0. Quit\n";
    std::cout << "-------------------------------------\n";
    std::cout << " Choose an option: ";

    int mode;
    std::cin >> mode;

    Reader reader;
    reader.readNodes("../test_data/small_data/Locations.csv");
    reader.readEdges("../test_data/small_data/Distances.csv");
    Exerciser exerciser;

    if (mode == BATCH_MODE) {

        std::cout << "Starting to read batch files...\n";

        Batcher batcher;
        batcher.readInput("../test_data/batch/input.txt");

        // Generate output file
        batcher.generateOutput("../test_data/batch/output.txt");
        std::cout << "Output generated: " << "output.txt" << "\n";
        std::cout << "Batch processing completed.\n";
        backToMenu(*this);
    }
    else if (mode == SHORTEST_PATH) {
        exerciser.ex_2_1(reader);
        backToMenu(*this);
    }
    else if (mode == RESTRICTED_ROUTE) {
        exerciser.ex_2_2(reader);
        backToMenu(*this);
    }
    else if (mode == ENVIRONMENTALLY_FRIENDLY) {
        exerciser.ex_3_1(reader);
        backToMenu(*this);
    }
    else if (mode == ENVIRONMENTAL_ALTERNATIVE) {
        exerciser.ex_3_2(reader);
        backToMenu(*this);
    }
    else if (mode == QUIT) {
        std::cout << "Exiting program...\n";
        exit(0);
    }
    else {
        std::cout << "Invalid option. Please try again.\n";

        mainMenu();
    }
}


void menu::backToMenu(menu& m) {
    std::cout << "\n-------------------------------------\n";
    std::cout << "Choose 0 to return to the menu or press any other key to exit";

    char choice;
    std::cin >> choice;

    if (choice == '0') {
        m.mainMenu();
    } else {
        std::cout << "Exiting program...\n";
        exit(0);
    }
}