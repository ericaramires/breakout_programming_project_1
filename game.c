/******************************************************************************
 * BREAKOUT GAME - GAME CORE MODULE
 * Enhanced Professional Version
 ******************************************************************************/

#include "game.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

// ===== CORE GAME FUNCTIONS =====

bool game_init(GameState *state) {
    // Initialize Allegro
    if (!al_init()) {
        fprintf(stderr, "Failed to initialize Allegro\n");
        return false;
    }
    
    // Initialize addons
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Failed to initialize primitives addon\n");
        return false;
    }
    
    if (!al_init_font_addon()) {
        fprintf(stderr, "Failed to initialize font addon\n");
        return false;
    }
    
    if (!al_init_ttf_addon()) {
        fprintf(stderr, "Failed to initialize TTF addon\n");
        return false;
    }
    
    if (!al_install_keyboard()) {
        fprintf(stderr, "Failed to install keyboard\n");
        return false;
    }
    
    if (!al_install_audio()) {
        fprintf(stderr, "Failed to install audio\n");
        return false;
    }
    
    if (!al_init_acodec_addon()) {
        fprintf(stderr, "Failed to initialize audio codec addon\n");
        return false;
    }
    
    // Create display
    state->display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!state->display) {
        fprintf(stderr, "Failed to create display\n");
        return false;
    }
    
    al_set_window_title(state->display, "Breakout - Enhanced Professional Version");
    
    // Create event queue and timer
    state->event_queue = al_create_event_queue();
    state->timer = al_create_timer(1.0 / FPS);
    
    if (!state->event_queue || !state->timer) {
        fprintf(stderr, "Failed to create event queue or timer\n");
        return false;
    }
    
    // Register event sources
    al_register_event_source(state->event_queue, al_get_display_event_source(state->display));
    al_register_event_source(state->event_queue, al_get_timer_event_source(state->timer));
    al_register_event_source(state->event_queue, al_get_keyboard_event_source());
    
    // Create fonts
    state->font_large = al_create_builtin_font();
    state->font_medium = al_create_builtin_font();
    state->font_small = al_create_builtin_font();
    
    if (!state->font_large || !state->font_medium || !state->font_small) {
        fprintf(stderr, "Failed to create fonts\n");
        return false;
    }
    
    // Initialize game components
    player_init(&state->player);
    ball_init(&state->ball);
    blocks_init(state->blocks, 1);
    particles_init(state->particles);
    
    // Initialize game state
    state->current_state = GAME_STATE_MENU;
    state->previous_state = GAME_STATE_MENU;
    state->score = 0;
    state->lives = MAX_LIVES;
    state->level = 1;
    state->blocks_remaining = blocks_count_active(state->blocks);
    state->game_time = 0.0;
    state->last_update_time = 0.0;
    state->state_transition_time = 0.0;
    state->screen_shake = 0.0f;
    state->flash_intensity = 0.0f;
    
    // Initialize input arrays
    for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
        state->keys[i] = false;
        state->key_pressed[i] = false;
    }
    
    // Start timer
    al_start_timer(state->timer);
    
    srand(time(NULL)); // Seed random number generator
    
    printf("Game initialized successfully!\n");
    return true;
}

void game_cleanup(GameState *state) {
    // Cleanup fonts
    if (state->font_large) al_destroy_font(state->font_large);
    if (state->font_medium) al_destroy_font(state->font_medium);
    if (state->font_small) al_destroy_font(state->font_small);
    
    // Cleanup Allegro components
    if (state->timer) al_destroy_timer(state->timer);
    if (state->event_queue) al_destroy_event_queue(state->event_queue);
    if (state->display) al_destroy_display(state->display);
    
    printf("Game cleanup complete\n");
}

void game_run(GameState *state) {
    bool running = true;
    bool redraw = true;
    
    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(state->event_queue, &event);
        
        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                game_update_input(state);
                game_update(state, 1.0 / FPS);
                redraw = true;
                break;
                
            case ALLEGRO_EVENT_KEY_DOWN:
                state->keys[event.keyboard.keycode] = true;
                state->key_pressed[event.keyboard.keycode] = true;
                
                // Handle global keys
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    if (state->current_state == GAME_STATE_PLAYING) {
                        game_change_state(state, GAME_STATE_PAUSED);
                    } else if (state->current_state == GAME_STATE_PAUSED) {
                        game_change_state(state, GAME_STATE_MENU);
                    } else {
                        running = false;
                    }
                }
                break;
                
            case ALLEGRO_EVENT_KEY_UP:
                state->keys[event.keyboard.keycode] = false;
                break;
                
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;
        }
        
        if (redraw && al_is_event_queue_empty(state->event_queue)) {
            game_render(state);
            redraw = false;
        }
        
        // Check if we should exit
        if (state->current_state == GAME_STATE_EXIT) {
            running = false;
        }
    }
}

void game_update(GameState *state, double delta_time) {
    state->game_time += delta_time;
    
    // Update screen shake
    if (state->screen_shake > 0) {
        state->screen_shake -= delta_time * 5.0f;
        if (state->screen_shake < 0) state->screen_shake = 0;
    }
    
    // Update flash effect
    if (state->flash_intensity > 0) {
        state->flash_intensity -= delta_time * 3.0f;
        if (state->flash_intensity < 0) state->flash_intensity = 0;
    }
    
    switch (state->current_state) {
        case GAME_STATE_MENU:
            // Handle menu input
            if (state->key_pressed[ALLEGRO_KEY_I]) {
                game_change_state(state, GAME_STATE_PLAYING);
                game_reset(state);
            } else if (state->key_pressed[ALLEGRO_KEY_R]) {
                game_change_state(state, GAME_STATE_RECORDS);
            } else if (state->key_pressed[ALLEGRO_KEY_S]) {
                game_change_state(state, GAME_STATE_EXIT);
            }
            break;
            
        case GAME_STATE_PLAYING:
            // Update game components
            player_update(&state->player, delta_time);
            ball_update(&state->ball, delta_time);
            blocks_update(state->blocks, delta_time);
            particles_update(state->particles, delta_time);
            
            // Handle ball launch
            if (state->ball.is_stuck_to_paddle && state->key_pressed[ALLEGRO_KEY_SPACE]) {
                ball_launch(&state->ball);
            }
            
            // Check collisions
            if (player_check_ball_collision(&state->player, &state->ball)) {
                // Calculate bounce angle based on hit position
                float hit_pos = (state->ball.x - player_get_center_x(&state->player)) / (PADDLE_WIDTH / 2.0f);
                hit_pos = fmax(-1.0f, fmin(1.0f, hit_pos)); // Clamp between -1 and 1
                
                float angle = hit_pos * 3.14159f / 3.0f; // Max 60 degrees
                float speed = state->ball.speed;
                
                state->ball.velocity_x = sin(angle) * speed;
                state->ball.velocity_y = -fabs(cos(angle) * speed); // Always bounce up
                
                player_on_ball_hit(&state->player);
                game_add_screen_shake(state, 0.5f);
            }
            
            // Check block collisions
            for (int i = 0; i < MAX_BLOCKS; i++) {
                if (state->blocks[i].is_active && block_check_collision(&state->blocks[i], &state->ball)) {
                    block_hit(&state->blocks[i]);
                    
                    // Simple collision response - reverse ball direction
                    ball_bounce_vertical(&state->ball);
                    
                    // Add score
                    state->score += state->blocks[i].points_value;
                    
                    // Create particle explosion
                    particles_create_explosion(state->particles, state->blocks[i].x + BLOCK_WIDTH/2, 
                                             state->blocks[i].y + BLOCK_HEIGHT/2, 
                                             state->blocks[i].color, 8);
                    
                    game_add_screen_shake(state, 0.3f);
                    break; // Only hit one block per frame
                }
            }
            
            // Check if ball is out of bounds
            if (ball_is_out_of_bounds(&state->ball)) {
                game_lose_life(state);
            }
            
            // Check win condition
            game_check_win_condition(state);
            
            // Pause with P key
            if (state->key_pressed[ALLEGRO_KEY_P]) {
                game_change_state(state, GAME_STATE_PAUSED);
            }
            break;
            
        case GAME_STATE_PAUSED:
            if (state->key_pressed[ALLEGRO_KEY_P]) {
                game_change_state(state, GAME_STATE_PLAYING);
            }
            break;
            
        case GAME_STATE_GAME_OVER:
            if (state->key_pressed[ALLEGRO_KEY_ESCAPE] || state->key_pressed[ALLEGRO_KEY_ENTER]) {
                // Save record if it's a new one
                if (io_is_new_record(state->score)) {
                    io_save_record(state->score);
                }
                game_change_state(state, GAME_STATE_MENU);
            }
            break;
            
        case GAME_STATE_RECORDS:
            if (state->key_pressed[ALLEGRO_KEY_ESCAPE]) {
                game_change_state(state, GAME_STATE_MENU);
            }
            break;
            
        default:
            break;
    }
    
    // Reset key pressed flags
    for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
        state->key_pressed[i] = false;
    }
}

void game_render(GameState *state) {
    al_clear_to_color(COLOR_BLACK);
    
    switch (state->current_state) {
        case GAME_STATE_MENU:
            ui_render_main_menu(state);
            break;
            
        case GAME_STATE_PLAYING:
        case GAME_STATE_PAUSED:
            // Render game objects
            blocks_render(state->blocks);
            player_render(&state->player);
            ball_render(&state->ball);
            particles_render(state->particles);
            
            // Render HUD
            ui_render_game_hud(state);
            
            // Render pause overlay if paused
            if (state->current_state == GAME_STATE_PAUSED) {
                ui_render_pause_menu(state);
            }
            break;
            
        case GAME_STATE_GAME_OVER:
            // Render game objects (faded)
            blocks_render(state->blocks);
            player_render(&state->player);
            ball_render(&state->ball);
            particles_render(state->particles);
            ui_render_game_hud(state);
            
            // Render game over screen
            ui_render_game_over(state);
            break;
            
        case GAME_STATE_RECORDS:
            ui_render_records_screen(state);
            break;
            
        default:
            break;
    }
    
    // Apply screen effects
    if (state->flash_intensity > 0) {
        ALLEGRO_COLOR flash = al_map_rgba_f(1.0f, 1.0f, 1.0f, state->flash_intensity);
        al_draw_filled_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, flash);
    }
    
    al_flip_display();
}

// ===== STATE MANAGEMENT =====

void game_change_state(GameState *state, GameStateType new_state) {
    state->previous_state = state->current_state;
    state->current_state = new_state;
    state->state_transition_time = state->game_time;
    
    printf("State changed from %d to %d\n", state->previous_state, new_state);
}

void game_update_input(GameState *state) {
    // Update player input
    if (state->current_state == GAME_STATE_PLAYING) {
        player_handle_input(&state->player, 
                           state->keys[ALLEGRO_KEY_LEFT], 
                           state->keys[ALLEGRO_KEY_RIGHT]);
    }
}

// ===== UTILITY FUNCTIONS =====

double game_get_time(void) {
    return al_get_time();
}

void game_add_screen_shake(GameState *state, float intensity) {
    state->screen_shake = intensity;
}

void game_add_flash(GameState *state, float intensity) {
    state->flash_intensity = intensity;
}

void game_reset(GameState *state) {
    state->score = 0;
    state->lives = MAX_LIVES;
    state->level = 1;
    
    // Reset game components
    player_init(&state->player);
    ball_reset(&state->ball, state->player.x);
    blocks_init(state->blocks, state->level);
    particles_clear_all(state->particles);
    
    state->blocks_remaining = blocks_count_active(state->blocks);
    
    printf("Game reset for new game\n");
}

void game_start_level(GameState *state) {
    state->level++;
    blocks_init(state->blocks, state->level);
    ball_reset(&state->ball, state->player.x);
    state->blocks_remaining = blocks_count_active(state->blocks);
    
    printf("Started level %d\n", state->level);
}

void game_check_win_condition(GameState *state) {
    int active_blocks = blocks_count_active(state->blocks);
    if (active_blocks == 0) {
        // Level completed!
        state->score += state->lives * 100; // Bonus for remaining lives
        game_start_level(state);
        game_add_flash(state, 0.5f);
    }
}

void game_lose_life(GameState *state) {
    state->lives--;
    
    if (state->lives <= 0) {
        // Game over
        game_change_state(state, GAME_STATE_GAME_OVER);
        game_add_flash(state, 1.0f);
    } else {
        // Reset ball position
        ball_reset(&state->ball, state->player.x);
        game_add_screen_shake(state, 1.0f);
    }
    
    printf("Life lost! Lives remaining: %d\n", state->lives);
} 