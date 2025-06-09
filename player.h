#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>

// Forward declaration
struct Ball;

// ===== PLAYER STRUCTURE =====
typedef struct Player {
    float x;
    float y;
    float width;
    float height;
    float velocity_x;
    float max_speed;
    
    // Visual effects
    float hit_flash_timer;
    ALLEGRO_COLOR color;
    
    // Animation
    float hover_offset;
    float hover_time;
} Player;

// ===== FUNCTION DECLARATIONS =====

// Core player functions
void player_init(Player *player);
void player_update(Player *player, double delta_time);
void player_render(Player *player);

// Input handling
void player_handle_input(Player *player, bool key_left, bool key_right);

// Collision and interaction
bool player_check_ball_collision(Player *player, struct Ball *ball);
void player_on_ball_hit(Player *player);

// Utility functions
float player_get_left(Player *player);
float player_get_right(Player *player);
float player_get_top(Player *player);
float player_get_bottom(Player *player);
float player_get_center_x(Player *player);

#endif // PLAYER_H 