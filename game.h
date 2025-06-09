#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdbool.h>

// ===== CONSTANTS =====
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 60

// Paddle constants
#define PADDLE_WIDTH 120
#define PADDLE_HEIGHT 15
#define PADDLE_Y_POSITION (SCREEN_HEIGHT - 50)
#define PADDLE_SPEED 8

// Ball constants
#define BALL_RADIUS 8
#define BALL_INITIAL_SPEED 5
#define BALL_MAX_SPEED 12

// Blocks constants
#define BLOCK_WIDTH 75
#define BLOCK_HEIGHT 25
#define BLOCK_ROWS 8
#define BLOCK_COLS 10
#define BLOCK_SPACING 5
#define BLOCK_OFFSET_X 50
#define BLOCK_OFFSET_Y 80
#define MAX_BLOCKS (BLOCK_ROWS * BLOCK_COLS)

// Game constants
#define MAX_LIVES 3
#define MAX_RECORDS 10
#define RECORD_FILENAME "records.txt"

// Colors
#define COLOR_BLACK al_map_rgb(0, 0, 0)
#define COLOR_WHITE al_map_rgb(255, 255, 255)
#define COLOR_RED al_map_rgb(255, 100, 100)
#define COLOR_GREEN al_map_rgb(100, 255, 100)
#define COLOR_BLUE al_map_rgb(100, 100, 255)
#define COLOR_YELLOW al_map_rgb(255, 255, 100)
#define COLOR_PURPLE al_map_rgb(255, 100, 255)
#define COLOR_CYAN al_map_rgb(100, 255, 255)
#define COLOR_ORANGE al_map_rgb(255, 200, 100)

// ===== ENUMS =====
typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_GAME_OVER,
    GAME_STATE_RECORDS,
    GAME_STATE_INSTRUCTIONS,
    GAME_STATE_EXIT
} GameStateType;

typedef enum {
    BLOCK_TYPE_NORMAL,
    BLOCK_TYPE_STRONG,
    BLOCK_TYPE_BONUS
} BlockType;

// ===== FORWARD DECLARATIONS =====
typedef struct Player Player;
typedef struct Ball Ball;
typedef struct Block Block;
typedef struct Particle Particle;
typedef struct GameState GameState;

// ===== INCLUDES =====
#include "player.h"
#include "ball.h"
#include "blocks.h"
#include "particles.h"
#include "io.h"
#include "ui.h"

// ===== MAIN GAME STATE =====
struct GameState {
    // Core components
    Player player;
    Ball ball;
    Block blocks[MAX_BLOCKS];
    Particle particles[100];
    
    // Game state
    GameStateType current_state;
    GameStateType previous_state;
    
    // Game stats
    int score;
    int lives;
    int level;
    int blocks_remaining;
    
    // Time tracking
    double game_time;
    double last_update_time;
    double state_transition_time;
    
    // Visual effects
    float screen_shake;
    float flash_intensity;
    
    // Input handling
    bool keys[ALLEGRO_KEY_MAX];
    bool key_pressed[ALLEGRO_KEY_MAX];
    
    // Allegro components
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_TIMER *timer;
    ALLEGRO_FONT *font_large;
    ALLEGRO_FONT *font_medium;
    ALLEGRO_FONT *font_small;
    
    // Audio
    ALLEGRO_SAMPLE *sound_paddle_hit;
    ALLEGRO_SAMPLE *sound_block_break;
    ALLEGRO_SAMPLE *sound_wall_hit;
    ALLEGRO_SAMPLE *sound_life_lost;
    ALLEGRO_SAMPLE *sound_game_over;
    ALLEGRO_SAMPLE *background_music;
    ALLEGRO_SAMPLE_INSTANCE *music_instance;
};

// ===== FUNCTION DECLARATIONS =====

// Core game functions
bool game_init(GameState *state);
void game_cleanup(GameState *state);
void game_run(GameState *state);
void game_update(GameState *state, double delta_time);
void game_render(GameState *state);

// State management
void game_change_state(GameState *state, GameStateType new_state);
void game_update_input(GameState *state);

// Game logic
void game_reset(GameState *state);
void game_start_level(GameState *state);
void game_check_win_condition(GameState *state);
void game_lose_life(GameState *state);

// Utility functions
double game_get_time(void);
void game_add_screen_shake(GameState *state, float intensity);
void game_add_flash(GameState *state, float intensity);

#endif // GAME_H 