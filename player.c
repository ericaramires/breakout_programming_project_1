/******************************************************************************
 * BREAKOUT GAME - PLAYER MODULE
 * Enhanced Professional Version
 ******************************************************************************/

#include "player.h"
#include "game.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdbool.h>

// ===== CORE PLAYER FUNCTIONS =====

void player_init(Player *player) {
    player->x = (SCREEN_WIDTH - PADDLE_WIDTH) / 2.0f;
    player->y = PADDLE_Y_POSITION;
    player->width = PADDLE_WIDTH;
    player->height = PADDLE_HEIGHT;
    player->velocity_x = 0.0f;
    player->max_speed = PADDLE_SPEED;
    player->hit_flash_timer = 0.0f;
    player->color = COLOR_WHITE;
    player->hover_offset = 0.0f;
    player->hover_time = 0.0f;
}

void player_update(Player *player, double delta_time) {
    // Update position based on velocity
    player->x += player->velocity_x * delta_time * 60.0f;
    
    // Keep player within screen bounds
    if (player->x < 0) {
        player->x = 0;
        player->velocity_x = 0;
    }
    if (player->x + player->width > SCREEN_WIDTH) {
        player->x = SCREEN_WIDTH - player->width;
        player->velocity_x = 0;
    }
    
    // Update hit flash effect
    if (player->hit_flash_timer > 0) {
        player->hit_flash_timer -= delta_time;
    }
    
    // Update hover animation
    player->hover_time += delta_time * 4.0f;
    player->hover_offset = sin(player->hover_time) * 2.0f;
    
    // Apply friction to velocity
    player->velocity_x *= 0.85f;
}

void player_render(Player *player) {
    float render_y = player->y + player->hover_offset;
    
    // Calculate color with flash effect
    ALLEGRO_COLOR render_color = player->color;
    if (player->hit_flash_timer > 0) {
        float flash_intensity = player->hit_flash_timer / 0.2f; // 0.2s flash duration
        render_color = al_map_rgba_f(1.0f, 1.0f, 1.0f, flash_intensity);
    }
    
    // Draw paddle with rounded corners effect
    al_draw_filled_rectangle(player->x, render_y, 
                            player->x + player->width, render_y + player->height, 
                            render_color);
    
    // Draw border
    ALLEGRO_COLOR border_color = al_map_rgba(200, 200, 200, 255);
    al_draw_rectangle(player->x, render_y, 
                     player->x + player->width, render_y + player->height, 
                     border_color, 2.0f);
    
    // Draw glow effect
    ALLEGRO_COLOR glow_color = al_map_rgba(255, 255, 255, 30);
    al_draw_filled_rectangle(player->x - 2, render_y - 2, 
                            player->x + player->width + 2, render_y + player->height + 2, 
                            glow_color);
}

// ===== INPUT HANDLING =====

void player_handle_input(Player *player, bool key_left, bool key_right) {
    if (key_left && !key_right) {
        player->velocity_x = -player->max_speed;
    } else if (key_right && !key_left) {
        player->velocity_x = player->max_speed;
    } else {
        // No input or both pressed - apply stronger friction
        player->velocity_x *= 0.7f;
    }
}

// ===== COLLISION AND INTERACTION =====

bool player_check_ball_collision(Player *player, struct Ball *ball) {
    float ball_left = ball->x - ball->radius;
    float ball_right = ball->x + ball->radius;
    float ball_top = ball->y - ball->radius;
    float ball_bottom = ball->y + ball->radius;
    
    float paddle_left = player->x;
    float paddle_right = player->x + player->width;
    float paddle_top = player->y + player->hover_offset;
    float paddle_bottom = paddle_top + player->height;
    
    return (ball_right >= paddle_left && ball_left <= paddle_right &&
            ball_bottom >= paddle_top && ball_top <= paddle_bottom);
}

void player_on_ball_hit(Player *player) {
    // Start flash effect
    player->hit_flash_timer = 0.2f;
}

// ===== UTILITY FUNCTIONS =====

float player_get_left(Player *player) {
    return player->x;
}

float player_get_right(Player *player) {
    return player->x + player->width;
}

float player_get_top(Player *player) {
    return player->y + player->hover_offset;
}

float player_get_bottom(Player *player) {
    return player->y + player->height + player->hover_offset;
}

float player_get_center_x(Player *player) {
    return player->x + player->width / 2.0f;
} 