/******************************************************************************
 * BREAKOUT GAME - USER INTERFACE MODULE
 * Enhanced Professional Version
 ******************************************************************************/

#include "ui.h"
#include "game.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include <math.h>

// ===== MENU FUNCTIONS =====

void ui_render_main_menu(GameState *state) {
    // Clear screen with gradient background
    ui_draw_gradient_background();
    
    // Animated title
    float title_pulse = 1.0f + 0.1f * sin(state->game_time * 2.0f);
    ui_draw_text_centered(state->font_large, "BREAKOUT", SCREEN_WIDTH/2, 100, COLOR_WHITE, title_pulse);
    ui_draw_text_centered(state->font_medium, "Enhanced Edition", SCREEN_WIDTH/2, 140, COLOR_CYAN, 1.0f);
    
    // Menu options with hover effects
    int menu_y = 250;
    int spacing = 50;
    
    ui_draw_menu_option(state, "(I) Iniciar Jogo", SCREEN_WIDTH/2, menu_y, 0);
    ui_draw_menu_option(state, "(R) Recordes", SCREEN_WIDTH/2, menu_y + spacing, 1);
    ui_draw_menu_option(state, "(M) Manual", SCREEN_WIDTH/2, menu_y + spacing * 2, 2);
    ui_draw_menu_option(state, "(S) Sair", SCREEN_WIDTH/2, menu_y + spacing * 3, 3);
    
    // Footer info
    ui_draw_text_centered(state->font_small, "Projeto Final - Programação I", SCREEN_WIDTH/2, SCREEN_HEIGHT - 60, COLOR_WHITE, 0.7f);
    ui_draw_text_centered(state->font_small, "Enhanced Professional Version", SCREEN_WIDTH/2, SCREEN_HEIGHT - 40, COLOR_GREEN, 0.8f);
}

void ui_render_game_hud(GameState *state) {
    // Score display with glow effect
    char score_text[64];
    snprintf(score_text, sizeof(score_text), "Pontos: %d", state->score);
    ui_draw_text_with_shadow(state->font_medium, score_text, 20, 20, COLOR_YELLOW);
    
    // Lives display with heart icons
    ui_draw_lives_display(state, SCREEN_WIDTH - 150, 20);
    
    // Level display
    char level_text[32];
    snprintf(level_text, sizeof(level_text), "Nível %d", state->level);
    ui_draw_text_centered(state->font_medium, level_text, SCREEN_WIDTH/2, 20, COLOR_WHITE, 1.0f);
    
    // High score in corner
    int high_score = io_get_highest_record();
    if (high_score > 0) {
        char high_score_text[64];
        snprintf(high_score_text, sizeof(high_score_text), "Recorde: %d", high_score);
        ui_draw_text_with_shadow(state->font_small, high_score_text, 20, SCREEN_HEIGHT - 40, COLOR_CYAN);
    }
}

void ui_render_game_over(GameState *state) {
    // Semi-transparent overlay
    ALLEGRO_COLOR overlay = al_map_rgba(0, 0, 0, 180);
    al_draw_filled_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, overlay);
    
    // Animated "GAME OVER" text
    float shake_x = sin(state->game_time * 20.0f) * 3.0f;
    float shake_y = cos(state->game_time * 15.0f) * 2.0f;
    
    ui_draw_text_centered(state->font_large, "GAME OVER", 
                         SCREEN_WIDTH/2 + shake_x, SCREEN_HEIGHT/2 - 100 + shake_y, 
                         COLOR_RED, 1.2f);
    
    // Final score
    char final_score[64];
    snprintf(final_score, sizeof(final_score), "Pontuação Final: %d", state->score);
    ui_draw_text_centered(state->font_medium, final_score, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 30, COLOR_WHITE, 1.0f);
    
    // Check if new record
    if (io_is_new_record(state->score)) {
        float glow = sin(state->game_time * 5.0f) * 0.5f + 0.5f;
        ALLEGRO_COLOR glow_color = al_map_rgba_f(1.0f, 1.0f, 0.0f, glow);
        ui_draw_text_centered(state->font_medium, "*** NOVO RECORDE! ***", 
                             SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 20, glow_color, 1.1f);
    }
    
    // Instructions
    ui_draw_text_centered(state->font_small, "Pressione ESC para voltar ao menu", 
                         SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 80, COLOR_WHITE, 0.8f);
}

void ui_render_pause_menu(GameState *state) {
    // Semi-transparent overlay
    ALLEGRO_COLOR overlay = al_map_rgba(0, 0, 0, 150);
    al_draw_filled_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, overlay);
    
    ui_draw_text_centered(state->font_large, "PAUSADO", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 50, COLOR_YELLOW, 1.0f);
    ui_draw_text_centered(state->font_medium, "Pressione P para continuar", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 20, COLOR_WHITE, 1.0f);
    ui_draw_text_centered(state->font_small, "ESC - Voltar ao menu", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 60, COLOR_WHITE, 0.8f);
}

// ===== RECORD DISPLAY =====

void ui_render_records_screen(GameState *state) {
    ui_draw_gradient_background();
    
    ui_draw_text_centered(state->font_large, "=== RECORDES ===", SCREEN_WIDTH/2, 80, COLOR_WHITE, 1.0f);
    
    int records[MAX_RECORDS];
    int count = io_load_records(records, MAX_RECORDS);
    
    if (count == 0) {
        ui_draw_text_centered(state->font_medium, "Nenhum recorde encontrado!", 
                             SCREEN_WIDTH/2, SCREEN_HEIGHT/2, COLOR_WHITE, 1.0f);
    } else {
        int start_y = 150;
        int max_display = count < 10 ? count : 10;
        
        for (int i = 0; i < max_display; i++) {
            char record_text[64];
            snprintf(record_text, sizeof(record_text), "%d. %d pontos", i + 1, records[i]);
            
            ALLEGRO_COLOR color = (i == 0) ? COLOR_YELLOW : COLOR_WHITE;
            float scale = (i == 0) ? 1.1f : 1.0f;
            
            ui_draw_text_centered(state->font_medium, record_text, 
                                 SCREEN_WIDTH/2, start_y + i * 35, color, scale);
        }
        
        if (count > 10) {
            char more_text[64];
            snprintf(more_text, sizeof(more_text), "... e mais %d recordes", count - 10);
            ui_draw_text_centered(state->font_small, more_text, 
                                 SCREEN_WIDTH/2, start_y + 10 * 35 + 20, COLOR_WHITE, 0.8f);
        }
    }
    
    ui_draw_text_centered(state->font_small, "Pressione ESC para voltar", 
                         SCREEN_WIDTH/2, SCREEN_HEIGHT - 60, COLOR_WHITE, 0.8f);
}

// ===== UTILITY FUNCTIONS =====

void ui_draw_gradient_background(void) {
    // Simple vertical gradient from dark blue to black
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        float ratio = (float)y / SCREEN_HEIGHT;
        ALLEGRO_COLOR color = al_map_rgba_f(0.0f, 0.0f, 0.2f * (1.0f - ratio), 1.0f);
        al_draw_line(0, y, SCREEN_WIDTH, y, color, 1.0f);
    }
}

void ui_draw_text_centered(ALLEGRO_FONT *font, const char *text, float x, float y, ALLEGRO_COLOR color, float scale) {
    if (!font || !text) return;
    
    // Calculate text position for centering
    int text_width = al_get_text_width(font, text);
    int font_height = al_get_font_line_height(font);
    
    float draw_x = x - (text_width * scale) / 2.0f;
    float draw_y = y - (font_height * scale) / 2.0f;
    
    if (scale != 1.0f) {
        ALLEGRO_TRANSFORM transform;
        al_identity_transform(&transform);
        al_scale_transform(&transform, scale, scale);
        al_translate_transform(&transform, draw_x, draw_y);
        al_use_transform(&transform);
        
        al_draw_text(font, color, 0, 0, 0, text);
        
        // Reset transform
        al_identity_transform(&transform);
        al_use_transform(&transform);
    } else {
        al_draw_text(font, color, draw_x, draw_y, 0, text);
    }
}

void ui_draw_text_with_shadow(ALLEGRO_FONT *font, const char *text, float x, float y, ALLEGRO_COLOR color) {
    if (!font || !text) return;
    
    // Draw shadow
    ALLEGRO_COLOR shadow = al_map_rgba(0, 0, 0, 100);
    al_draw_text(font, shadow, x + 2, y + 2, 0, text);
    
    // Draw main text
    al_draw_text(font, color, x, y, 0, text);
}

void ui_draw_menu_option(GameState *state, const char *text, float x, float y, int option_index) {
    // Simple highlight effect - in a real implementation you'd track which option is selected
    ALLEGRO_COLOR color = COLOR_WHITE;
    float scale = 1.0f;
    
    ui_draw_text_centered(state->font_medium, text, x, y, color, scale);
}

void ui_draw_lives_display(GameState *state, float x, float y) {
    char lives_text[32];
    snprintf(lives_text, sizeof(lives_text), "Vidas: ");
    al_draw_text(state->font_medium, COLOR_WHITE, x, y, 0, lives_text);
    
    // Draw heart symbols for lives
    float heart_x = x + al_get_text_width(state->font_medium, lives_text) + 10;
    for (int i = 0; i < state->lives; i++) {
        // Simple heart using a circle (could be replaced with actual heart sprite)
        al_draw_filled_circle(heart_x + i * 25, y + 10, 8, COLOR_RED);
        al_draw_filled_circle(heart_x + i * 25 - 4, y + 6, 5, COLOR_RED);
        al_draw_filled_circle(heart_x + i * 25 + 4, y + 6, 5, COLOR_RED);
    }
} 