#ifndef BALL_H
#define BALL_H

#include <allegro5/allegro.h>

// ===== BALL STRUCTURE =====
typedef struct Ball {
    float x;
    float y;
    float velocity_x;
    float velocity_y;
    float radius;
    float speed;
    
    // State
    bool is_launched;
    bool is_stuck_to_paddle;
    
    // Visual effects
    float trail_positions[10][2]; // Store last 10 positions for trail effect
    int trail_index;
    ALLEGRO_COLOR color;
    
    // Animation
    float pulse_timer;
    float pulse_scale;
} Ball;

// ===== FUNCTION DECLARATIONS =====

// Core ball functions
void ball_init(Ball *ball);
void ball_reset(Ball *ball, float paddle_x);
void ball_update(Ball *ball, double delta_time);
void ball_render(Ball *ball);

// Movement and physics
void ball_launch(Ball *ball);
void ball_set_velocity(Ball *ball, float vx, float vy);
void ball_normalize_velocity(Ball *ball);
void ball_increase_speed(Ball *ball, float factor);

// Collision handling
void ball_bounce_horizontal(Ball *ball);
void ball_bounce_vertical(Ball *ball);
void ball_handle_wall_collision(Ball *ball);

// Utility functions
float ball_get_left(Ball *ball);
float ball_get_right(Ball *ball);
float ball_get_top(Ball *ball);
float ball_get_bottom(Ball *ball);
bool ball_is_out_of_bounds(Ball *ball);

#endif // BALL_H 