#ifndef UI_H
#define UI_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stdbool.h>

// Forward declaration
typedef struct GameState GameState;

// ===== UI CONSTANTS =====
#define UI_BUTTON_HEIGHT 50
#define UI_BUTTON_WIDTH 200
#define UI_MENU_SPACING 20

// ===== UI STRUCTURES =====
typedef struct Button {
    float x;
    float y;
    float width;
    float height;
    char text[64];
    bool is_hovered;
    bool is_pressed;
    ALLEGRO_COLOR color_normal;
    ALLEGRO_COLOR color_hover;
    ALLEGRO_COLOR color_pressed;
} Button;

typedef struct MenuState {
    Button buttons[10];
    int button_count;
    int selected_button;
    float transition_timer;
    float background_scroll;
} MenuState;

// ===== FUNCTION DECLARATIONS =====

// Core UI functions
void ui_init(void);
void ui_cleanup(void);

// Menu functions
void ui_render_main_menu(GameState *state);
void ui_render_game_over(GameState *state);
void ui_render_records(GameState *state);
void ui_render_instructions(GameState *state);
void ui_render_pause_menu(GameState *state);

// Game UI functions
void ui_render_hud(GameState *state);
void ui_render_score(GameState *state, int x, int y);
void ui_render_lives(GameState *state, int x, int y);
void ui_render_level(GameState *state, int x, int y);

// Button functions
void button_init(Button *button, float x, float y, const char *text);
void button_update(Button *button, float mouse_x, float mouse_y, bool mouse_pressed);
void button_render(Button *button, ALLEGRO_FONT *font);
bool button_is_clicked(Button *button);

// Visual effects
void ui_render_background_gradient(void);
void ui_render_animated_title(const char *title, float y, double time);
void ui_render_particle_background(GameState *state);

// Utility functions
void ui_draw_centered_text(ALLEGRO_FONT *font, ALLEGRO_COLOR color, float y, const char *text);
void ui_draw_text_with_shadow(ALLEGRO_FONT *font, ALLEGRO_COLOR text_color, ALLEGRO_COLOR shadow_color, 
                              float x, float y, int flags, const char *text);
ALLEGRO_COLOR ui_lerp_color(ALLEGRO_COLOR a, ALLEGRO_COLOR b, float t);

#endif // UI_H 