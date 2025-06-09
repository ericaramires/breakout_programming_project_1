/******************************************************************************
 * BREAKOUT GAME - BLOCKS MODULE
 * Enhanced Professional Version
 ******************************************************************************/

#include "blocks.h"
#include "game.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdlib.h>

// ===== CORE BLOCK FUNCTIONS =====

void blocks_init(Block blocks[], int level) {
    blocks_clear_all(blocks);
    blocks_generate_level(blocks, level);
}

void blocks_update(Block blocks[], double delta_time) {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blocks[i].is_active) {
            // Update destruction animation
            if (blocks[i].destruction_timer > 0) {
                blocks[i].destruction_timer -= delta_time;
                if (blocks[i].destruction_timer <= 0) {
                    blocks[i].is_active = false;
                }
            }
            
            // Update pulse animation
            blocks[i].pulse_timer += delta_time * 2.0f;
            
            // Update shake effect (when hit)
            if (blocks[i].shake_offset_x != 0 || blocks[i].shake_offset_y != 0) {
                blocks[i].shake_offset_x *= 0.8f;
                blocks[i].shake_offset_y *= 0.8f;
                if (fabs(blocks[i].shake_offset_x) < 0.1f && fabs(blocks[i].shake_offset_y) < 0.1f) {
                    blocks[i].shake_offset_x = 0;
                    blocks[i].shake_offset_y = 0;
                }
            }
        }
    }
}

void blocks_render(Block blocks[]) {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blocks[i].is_active) {
            float x = blocks[i].x + blocks[i].shake_offset_x;
            float y = blocks[i].y + blocks[i].shake_offset_y;
            
            // Apply destruction fade effect
            float alpha = 1.0f;
            if (blocks[i].destruction_timer > 0) {
                alpha = blocks[i].destruction_timer / 0.5f; // 0.5s destruction animation
            }
            
            // Apply pulse effect based on hits
            float pulse = 1.0f;
            if (blocks[i].current_hits > 0) {
                pulse = 1.0f + 0.05f * sin(blocks[i].pulse_timer * 5.0f);
            }
            
            // Adjust color based on hits taken
            ALLEGRO_COLOR color = blocks[i].color;
            if (blocks[i].current_hits > 0) {
                // Darken the block as it takes damage
                float damage_factor = 1.0f - (blocks[i].current_hits / (float)blocks[i].hits_required) * 0.3f;
                color = al_map_rgba_f(
                    al_get_float_color_component(color, ALLEGRO_RED) * damage_factor * alpha,
                    al_get_float_color_component(color, ALLEGRO_GREEN) * damage_factor * alpha,
                    al_get_float_color_component(color, ALLEGRO_BLUE) * damage_factor * alpha,
                    alpha
                );
            }
            
            // Draw block with effects
            float width = blocks[i].width * pulse;
            float height = blocks[i].height * pulse;
            al_draw_filled_rectangle(x, y, x + width, y + height, color);
            
            // Draw border
            ALLEGRO_COLOR border_color = al_map_rgba_f(1.0f, 1.0f, 1.0f, alpha * 0.5f);
            al_draw_rectangle(x, y, x + width, y + height, border_color, 1.0f);
        }
    }
}

// ===== INDIVIDUAL BLOCK FUNCTIONS =====

void block_init(Block *block, float x, float y, int type) {
    block->x = x;
    block->y = y;
    block->width = BLOCK_WIDTH;
    block->height = BLOCK_HEIGHT;
    block->block_type = type;
    block->is_active = true;
    block->destruction_timer = 0;
    block->pulse_timer = 0;
    block->shake_offset_x = 0;
    block->shake_offset_y = 0;
    
    switch (type) {
        case BLOCK_TYPE_NORMAL:
            block->hits_required = 1;
            block->points_value = 10;
            break;
        case BLOCK_TYPE_STRONG:
            block->hits_required = 2;
            block->points_value = 25;
            break;
        case BLOCK_TYPE_BONUS:
            block->hits_required = 1;
            block->points_value = 50;
            break;
        default:
            block->hits_required = 1;
            block->points_value = 10;
            break;
    }
    
    block->current_hits = 0;
    block->color = blocks_get_color_for_type(type);
}

void block_hit(Block *block) {
    if (!block->is_active) return;
    
    block->current_hits++;
    
    // Add shake effect
    block->shake_offset_x = (rand() % 10 - 5) * 0.5f;
    block->shake_offset_y = (rand() % 10 - 5) * 0.5f;
    
    if (block->current_hits >= block->hits_required) {
        block_destroy(block);
    }
}

void block_destroy(Block *block) {
    if (!block->is_active) return;
    
    // Start destruction animation
    block->destruction_timer = 0.5f; // 0.5 seconds to fade out
}

bool block_check_collision(Block *block, struct Ball *ball) {
    if (!block->is_active || block->destruction_timer > 0) {
        return false;
    }
    
    // Simple AABB collision detection
    float ball_left = ball->x - ball->radius;
    float ball_right = ball->x + ball->radius;
    float ball_top = ball->y - ball->radius;
    float ball_bottom = ball->y + ball->radius;
    
    float block_left = block->x;
    float block_right = block->x + block->width;
    float block_top = block->y;
    float block_bottom = block->y + block->height;
    
    return (ball_right >= block_left && ball_left <= block_right &&
            ball_bottom >= block_top && ball_top <= block_bottom);
}

// ===== UTILITY FUNCTIONS =====

int blocks_count_active(Block blocks[]) {
    int count = 0;
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blocks[i].is_active && blocks[i].destruction_timer == 0) {
            count++;
        }
    }
    return count;
}

int blocks_get_total_points(Block blocks[]) {
    int total = 0;
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blocks[i].is_active) {
            total += blocks[i].points_value;
        }
    }
    return total;
}

void blocks_clear_all(Block blocks[]) {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        blocks[i].is_active = false;
        blocks[i].destruction_timer = 0;
        blocks[i].current_hits = 0;
    }
}

// ===== LEVEL GENERATION =====

void blocks_generate_level(Block blocks[], int level) {
    int block_index = 0;
    
    // Generate pattern based on level
    for (int row = 0; row < BLOCK_ROWS && block_index < MAX_BLOCKS; row++) {
        for (int col = 0; col < BLOCK_COLS && block_index < MAX_BLOCKS; col++) {
            float x = BLOCK_OFFSET_X + col * (BLOCK_WIDTH + BLOCK_SPACING);
            float y = BLOCK_OFFSET_Y + row * (BLOCK_HEIGHT + BLOCK_SPACING);
            
            // Determine block type based on row and level
            int type = BLOCK_TYPE_NORMAL;
            if (row < 2) {
                type = BLOCK_TYPE_BONUS; // Top rows are bonus blocks
            } else if (row < 4 && level > 1) {
                type = BLOCK_TYPE_STRONG; // Middle rows are strong blocks in higher levels
            }
            
            // Skip some blocks in higher levels for interesting patterns
            if (level > 2 && (row + col) % 3 == 0) {
                continue; // Skip this block
            }
            
            block_init(&blocks[block_index], x, y, type);
            block_index++;
        }
    }
}

ALLEGRO_COLOR blocks_get_color_for_type(int type) {
    switch (type) {
        case BLOCK_TYPE_NORMAL:
            return COLOR_BLUE;
        case BLOCK_TYPE_STRONG:
            return COLOR_RED;
        case BLOCK_TYPE_BONUS:
            return COLOR_YELLOW;
        default:
            return COLOR_WHITE;
    }
} 