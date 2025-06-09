/******************************************************************************
 * BREAKOUT GAME - PARTICLES MODULE
 * Enhanced Professional Version
 ******************************************************************************/

#include "particles.h"
#include "game.h"
#include <math.h>
#include <stdlib.h>

// ===== CORE PARTICLE FUNCTIONS =====

void particles_init(Particle particles[]) {
    for (int i = 0; i < 100; i++) {
        particles[i].is_active = false;
        particles[i].life_time = 0.0f;
        particles[i].max_life = 0.0f;
    }
}

void particles_update(Particle particles[], double delta_time) {
    for (int i = 0; i < 100; i++) {
        if (particles[i].is_active) {
            // Update position
            particles[i].x += particles[i].velocity_x * delta_time * 60.0f;
            particles[i].y += particles[i].velocity_y * delta_time * 60.0f;
            
            // Apply gravity
            particles[i].velocity_y += particles[i].gravity * delta_time * 60.0f;
            
            // Update life
            particles[i].life_time -= delta_time;
            if (particles[i].life_time <= 0) {
                particles[i].is_active = false;
            }
            
            // Update size and alpha based on life
            float life_ratio = particles[i].life_time / particles[i].max_life;
            particles[i].size = particles[i].initial_size * life_ratio;
            particles[i].alpha = life_ratio;
        }
    }
}

void particles_render(Particle particles[]) {
    for (int i = 0; i < 100; i++) {
        if (particles[i].is_active) {
            ALLEGRO_COLOR color = al_map_rgba_f(
                al_get_float_color_component(particles[i].color, ALLEGRO_RED),
                al_get_float_color_component(particles[i].color, ALLEGRO_GREEN),
                al_get_float_color_component(particles[i].color, ALLEGRO_BLUE),
                particles[i].alpha
            );
            
            al_draw_filled_circle(particles[i].x, particles[i].y, particles[i].size, color);
        }
    }
}

// ===== PARTICLE CREATION =====

void particles_create_explosion(Particle particles[], float x, float y, ALLEGRO_COLOR color, int count) {
    for (int created = 0; created < count; created++) {
        int index = particles_find_free_slot(particles);
        if (index == -1) break; // No free slots
        
        Particle *p = &particles[index];
        p->x = x;
        p->y = y;
        
        // Random velocity in all directions
        float angle = (rand() % 360) * M_PI / 180.0f;
        float speed = (rand() % 100 + 50) / 10.0f; // 5-15 speed
        p->velocity_x = cos(angle) * speed;
        p->velocity_y = sin(angle) * speed;
        
        p->gravity = 200.0f; // Downward gravity
        p->color = color;
        p->initial_size = (rand() % 5 + 2); // 2-7 size
        p->size = p->initial_size;
        p->max_life = (rand() % 100 + 50) / 100.0f; // 0.5-1.5 seconds
        p->life_time = p->max_life;
        p->alpha = 1.0f;
        p->is_active = true;
    }
}

void particles_create_sparkle(Particle particles[], float x, float y, ALLEGRO_COLOR color) {
    for (int created = 0; created < 5; created++) {
        int index = particles_find_free_slot(particles);
        if (index == -1) break;
        
        Particle *p = &particles[index];
        p->x = x + (rand() % 20 - 10); // Spread around position
        p->y = y + (rand() % 20 - 10);
        
        // Small random velocity
        p->velocity_x = (rand() % 40 - 20) / 10.0f;
        p->velocity_y = (rand() % 40 - 20) / 10.0f;
        
        p->gravity = 0.0f; // No gravity for sparkles
        p->color = color;
        p->initial_size = (rand() % 3 + 1); // 1-4 size
        p->size = p->initial_size;
        p->max_life = 0.3f; // Short lived
        p->life_time = p->max_life;
        p->alpha = 1.0f;
        p->is_active = true;
    }
}

void particles_create_trail(Particle particles[], float x, float y, ALLEGRO_COLOR color) {
    int index = particles_find_free_slot(particles);
    if (index == -1) return;
    
    Particle *p = &particles[index];
    p->x = x + (rand() % 10 - 5);
    p->y = y + (rand() % 10 - 5);
    p->velocity_x = (rand() % 20 - 10) / 20.0f;
    p->velocity_y = (rand() % 20 - 10) / 20.0f;
    p->gravity = 50.0f;
    p->color = color;
    p->initial_size = 2.0f;
    p->size = p->initial_size;
    p->max_life = 0.2f;
    p->life_time = p->max_life;
    p->alpha = 0.7f;
    p->is_active = true;
}

// ===== UTILITY FUNCTIONS =====

int particles_find_free_slot(Particle particles[]) {
    for (int i = 0; i < 100; i++) {
        if (!particles[i].is_active) {
            return i;
        }
    }
    return -1; // No free slot found
}

void particles_clear_all(Particle particles[]) {
    for (int i = 0; i < 100; i++) {
        particles[i].is_active = false;
    }
} 