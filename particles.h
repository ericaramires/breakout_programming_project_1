#ifndef PARTICLES_H
#define PARTICLES_H

#include <allegro5/allegro.h>
#include <stdbool.h>

// ===== PARTICLE STRUCTURE =====
typedef struct Particle {
    float x;
    float y;
    float velocity_x;
    float velocity_y;
    float life_time;
    float max_life_time;
    float size;
    bool is_active;
    ALLEGRO_COLOR color;
    
    // Physics
    float gravity;
    float friction;
} Particle;

// ===== FUNCTION DECLARATIONS =====

// Core particle system functions
void particles_init(Particle particles[]);
void particles_update(Particle particles[], double delta_time);
void particles_render(Particle particles[]);

// Particle creation
void particles_create_explosion(Particle particles[], float x, float y, ALLEGRO_COLOR color, int count);
void particles_create_trail(Particle particles[], float x, float y, ALLEGRO_COLOR color);

// Individual particle functions
void particle_init(Particle *particle, float x, float y, float vx, float vy, float life, ALLEGRO_COLOR color);
void particle_update(Particle *particle, double delta_time);
void particle_render(Particle *particle);

// Utility functions
int particles_count_active(Particle particles[]);
void particles_clear_all(Particle particles[]);

#endif // PARTICLES_H 