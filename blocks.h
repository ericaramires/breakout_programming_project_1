#ifndef BLOCKS_H
#define BLOCKS_H

#include <allegro5/allegro.h>
#include <stdbool.h>

// ===== BLOCK STRUCTURE =====
typedef struct Block {
    float x;
    float y;
    float width;
    float height;
    int hits_required;
    int current_hits;
    bool is_active;
    int points_value;
    
    // Type and visual
    int block_type; // BlockType enum
    ALLEGRO_COLOR color;
    
    // Animation and effects
    float destruction_timer;
    float pulse_timer;
    float shake_offset_x;
    float shake_offset_y;
} Block;

// ===== FUNCTION DECLARATIONS =====

// Core block functions
void blocks_init(Block blocks[], int level);
void blocks_update(Block blocks[], double delta_time);
void blocks_render(Block blocks[]);

// Individual block functions
void block_init(Block *block, float x, float y, int type);
void block_hit(Block *block);
void block_destroy(Block *block);
bool block_check_collision(Block *block, struct Ball *ball);

// Utility functions
int blocks_count_active(Block blocks[]);
int blocks_get_total_points(Block blocks[]);
void blocks_clear_all(Block blocks[]);

// Level generation
void blocks_generate_level(Block blocks[], int level);
ALLEGRO_COLOR blocks_get_color_for_type(int type);

#endif // BLOCKS_H 