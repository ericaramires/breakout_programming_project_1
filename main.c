/******************************************************************************
 * BREAKOUT GAME - PROGRAMMING I PROJECT (2015)
 * Enhanced Professional Version
 * 
 * Original project: Academic assignment for Programming I course
 * Enhancement: Modern modular architecture with professional standards
 * 
 * File: main.c
 * Purpose: Application entry point
 ******************************************************************************/

#include "game.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Main entry point of the application
 */
int main(int argc, char *argv[]) {
    // Suppress unused parameter warnings
    (void)argc;
    (void)argv;
    
    // Create and initialize game state
    GameState game_state = {0};
    
    // Initialize the game
    if (!game_init(&game_state)) {
        fprintf(stderr, "Failed to initialize game\n");
        return EXIT_FAILURE;
    }
    
    printf("=== BREAKOUT GAME ===\n");
    printf("Enhanced Professional Version\n");
    printf("Original Project: Programming I (2015)\n");
    printf("Press F11 for fullscreen, ESC to exit\n\n");
    
    // Run the main game loop
    game_run(&game_state);
    
    // Cleanup and exit
    game_cleanup(&game_state);
    
    printf("Thank you for playing!\n");
    return EXIT_SUCCESS;
} 