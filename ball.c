/******************************************************************************
 * BREAKOUT GAME - BALL MODULE
 * Enhanced Professional Version
 ******************************************************************************/

#include "ball.h"
#include "game.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdlib.h>

// ===== CORE BALL FUNCTIONS =====

void ball_init(Ball *ball) {
    ball->x = SCREEN_WIDTH / 2.0f;
    ball->y = PADDLE_Y_POSITION - BALL_RADIUS - 5;
    ball->velocity_x = 0.0f;
    ball->velocity_y = 0.0f;
    ball->radius = BALL_RADIUS;
    ball->speed = BALL_INITIAL_SPEED;
    ball->is_launched = false;
    ball->is_stuck_to_paddle = true;
    ball->color = COLOR_WHITE;
    ball->pulse_timer = 0.0f;
    ball->pulse_scale = 1.0f;
    ball->trail_index = 0;
    
    // Initialize trail positions
    for (int i = 0; i < 10; i++) {
        ball->trail_positions[i][0] = ball->x;
        ball->trail_positions[i][1] = ball->y;
    }
}

void ball_reset(Ball *ball, float paddle_x) {
    ball->x = paddle_x + PADDLE_WIDTH / 2.0f;
    ball->y = PADDLE_Y_POSITION - BALL_RADIUS - 5;
    ball->velocity_x = 0.0f;
    ball->velocity_y = 0.0f;
    ball->is_launched = false;
    ball->is_stuck_to_paddle = true;
    ball->pulse_timer = 0.0f;
    ball->pulse_scale = 1.0f;
}

void ball_update(Ball *ball, double delta_time) {
    // Update animation timers
    ball->pulse_timer += delta_time * 3.0f;
    ball->pulse_scale = 1.0f + 0.1f * sin(ball->pulse_timer);
    
    if (ball->is_launched && !ball->is_stuck_to_paddle) {
        // Store current position in trail
        ball->trail_positions[ball->trail_index][0] = ball->x;
        ball->trail_positions[ball->trail_index][1] = ball->y;
        ball->trail_index = (ball->trail_index + 1) % 10;
        
        // Update position
        ball->x += ball->velocity_x * delta_time * 60.0f;
        ball->y += ball->velocity_y * delta_time * 60.0f;
        
        // Handle wall collisions
        ball_handle_wall_collision(ball);
    }
}

void ball_render(Ball *ball) {
    // Draw trail effect
    for (int i = 0; i < 10; i++) {
        int trail_index = (ball->trail_index - i - 1 + 10) % 10;
        float alpha = (10 - i) / 10.0f * 0.3f;
        ALLEGRO_COLOR trail_color = al_map_rgba_f(1.0f, 1.0f, 1.0f, alpha);
        al_draw_filled_circle(ball->trail_positions[trail_index][0], 
                             ball->trail_positions[trail_index][1], 
                             ball->radius * 0.5f, trail_color);
    }
    
    // Draw main ball with pulse effect
    float size = ball->radius * ball->pulse_scale;
    al_draw_filled_circle(ball->x, ball->y, size, ball->color);
    
    // Draw glow effect
    ALLEGRO_COLOR glow_color = al_map_rgba(255, 255, 255, 50);
    al_draw_filled_circle(ball->x, ball->y, size + 3, glow_color);
}

// ===== MOVEMENT AND PHYSICS =====

void ball_launch(Ball *ball) {
    if (!ball->is_launched) {
        // Launch ball at an angle
        float angle = (rand() % 120 - 60) * M_PI / 180.0f; // -60 to +60 degrees
        ball->velocity_x = sin(angle) * ball->speed;
        ball->velocity_y = -cos(angle) * ball->speed;
        ball->is_launched = true;
        ball->is_stuck_to_paddle = false;
    }
}

void ball_set_velocity(Ball *ball, float vx, float vy) {
    ball->velocity_x = vx;
    ball->velocity_y = vy;
}

void ball_normalize_velocity(Ball *ball) {
    float magnitude = sqrt(ball->velocity_x * ball->velocity_x + ball->velocity_y * ball->velocity_y);
    if (magnitude > 0) {
        ball->velocity_x = (ball->velocity_x / magnitude) * ball->speed;
        ball->velocity_y = (ball->velocity_y / magnitude) * ball->speed;
    }
}

void ball_increase_speed(Ball *ball, float factor) {
    ball->speed *= factor;
    if (ball->speed > BALL_MAX_SPEED) {
        ball->speed = BALL_MAX_SPEED;
    }
    ball_normalize_velocity(ball);
}

// ===== COLLISION HANDLING =====

void ball_bounce_horizontal(Ball *ball) {
    ball->velocity_x = -ball->velocity_x;
}

void ball_bounce_vertical(Ball *ball) {
    ball->velocity_y = -ball->velocity_y;
}

void ball_handle_wall_collision(Ball *ball) {
    // Left wall
    if (ball_get_left(ball) <= 0) {
        ball->x = ball->radius;
        ball_bounce_horizontal(ball);
    }
    
    // Right wall
    if (ball_get_right(ball) >= SCREEN_WIDTH) {
        ball->x = SCREEN_WIDTH - ball->radius;
        ball_bounce_horizontal(ball);
    }
    
    // Top wall
    if (ball_get_top(ball) <= 0) {
        ball->y = ball->radius;
        ball_bounce_vertical(ball);
    }
}

// ===== UTILITY FUNCTIONS =====

float ball_get_left(Ball *ball) {
    return ball->x - ball->radius;
}

float ball_get_right(Ball *ball) {
    return ball->x + ball->radius;
}

float ball_get_top(Ball *ball) {
    return ball->y - ball->radius;
}

float ball_get_bottom(Ball *ball) {
    return ball->y + ball->radius;
}

bool ball_is_out_of_bounds(Ball *ball) {
    return ball->y > SCREEN_HEIGHT + ball->radius;
} 