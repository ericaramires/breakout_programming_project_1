#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FPS 60

// Estruturas do jogo
struct jogador {
    int mov;
    int pontos;
    int vidas;
    int con_colisao;
} player;

struct bola {
    int con_Bola;
    int posx;
    int posy;
    int coeAy;
    int coeAx;
} bol;

struct objeto {
    int cor_r, cor_g, cor_b;
    int xc, yc;
    int pontos;
    int ativo;
} bloco[75];

struct recordes {
    int pontos;
};

// Variáveis globais
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_FONT *font = NULL;
ALLEGRO_EVENT event;

// Funções de recordes
void gravarRecordes(int pontos) {
    FILE *arq = fopen("recordes.txt", "a");
    if (arq != NULL) {
        fprintf(arq, "%d\n", pontos);
        fclose(arq);
    }
}

int numeroRecordes() {
    FILE *arq;
    int count = 0;
    int temp;
    
    arq = fopen("recordes.txt", "r");
    if (arq != NULL) {
        while (fscanf(arq, "%d", &temp) == 1) {
            count++;
        }
        fclose(arq);
    }
    return count;
}

void mostrarRecordes() {
    FILE *arq;
    int pontos[100];
    int count = 0;
    char texto[100];
    
    arq = fopen("recordes.txt", "r");
    if (arq != NULL) {
        while (fscanf(arq, "%d", &pontos[count]) == 1 && count < 100) {
            count++;
        }
        fclose(arq);
    }
    
    // Ordenar recordes (maior para menor)
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (pontos[i] < pontos[j]) {
                int temp = pontos[i];
                pontos[i] = pontos[j];
                pontos[j] = temp;
            }
        }
    }
    
    bool showing = true;
    while (showing) {
        while (al_get_next_event(event_queue, &event)) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                showing = false;
            }
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE || 
                    event.keyboard.keycode == ALLEGRO_KEY_ENTER ||
                    event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    showing = false;
                }
            }
        }
        
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 50, 
                    ALLEGRO_ALIGN_CENTER, "=== RECORDES ===");
        
        if (count == 0) {
            al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 150, 
                        ALLEGRO_ALIGN_CENTER, "Nenhum recorde encontrado!");
        } else {
            int maxRecordes = count < 10 ? count : 10; // Mostrar no máximo 10
            for (int i = 0; i < maxRecordes; i++) {
                sprintf(texto, "%d. %d pontos", i + 1, pontos[i]);
                al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 120 + i * 25, 
                            ALLEGRO_ALIGN_CENTER, texto);
            }
            
            if (count > 10) {
                sprintf(texto, "... e mais %d recordes", count - 10);
                al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH/2, 370, 
                            ALLEGRO_ALIGN_CENTER, texto);
            }
        }
        
        al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH/2, 420, 
                    ALLEGRO_ALIGN_CENTER, "Pressione ESC, ENTER ou ESPAÇO para voltar");
        
        al_flip_display();
    }
}

void mostrarManual() {
    bool showing = true;
    
    while (showing) {
        while (al_get_next_event(event_queue, &event)) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                showing = false;
            }
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE || 
                    event.keyboard.keycode == ALLEGRO_KEY_ENTER ||
                    event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    showing = false;
                }
            }
        }
        
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 50, 
                    ALLEGRO_ALIGN_CENTER, "=== MANUAL DE INSTRUÇÕES ===");
        
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 100, 
                    ALLEGRO_ALIGN_CENTER, "OBJETIVO:");
        al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH/2, 120, 
                    ALLEGRO_ALIGN_CENTER, "Destrua todos os blocos sem deixar a bola cair!");
        
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 160, 
                    ALLEGRO_ALIGN_CENTER, "CONTROLES NO MENU:");
        al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH/2, 180, 
                    ALLEGRO_ALIGN_CENTER, "(I) - Iniciar Jogo");
        al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH/2, 200, 
                    ALLEGRO_ALIGN_CENTER, "(R) - Ver Recordes");
        al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH/2, 220, 
                    ALLEGRO_ALIGN_CENTER, "(M) - Manual (esta tela)");
        al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH/2, 240, 
                    ALLEGRO_ALIGN_CENTER, "(S) - Sair");
        
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 280, 
                    ALLEGRO_ALIGN_CENTER, "CONTROLES NO JOGO:");
        al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH/2, 300, 
                    ALLEGRO_ALIGN_CENTER, "Setas ← → - Mover paddle");
        al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH/2, 320, 
                    ALLEGRO_ALIGN_CENTER, "ESPAÇO - Lançar bola");
        al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH/2, 340, 
                    ALLEGRO_ALIGN_CENTER, "ESC - Sair do jogo");
        
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 380, 
                    ALLEGRO_ALIGN_CENTER, "PONTUAÇÃO:");
        al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH/2, 400, 
                    ALLEGRO_ALIGN_CENTER, "Cada bloco vale pontos + bônus por vida!");
        
        al_draw_text(font, al_map_rgb(150, 150, 150), SCREEN_WIDTH/2, 440, 
                    ALLEGRO_ALIGN_CENTER, "Pressione ESC, ENTER ou ESPAÇO para voltar");
        
        al_flip_display();
    }
}

// Funções de colisão
void colisaoBarra(int bolposy, int bolposx, int *playermov, int *bolcoeAy, int *bolcoeAx) {
    if (bolposy + 460 >= 470 && (bolposx >= 400 + *playermov && bolposx <= 530 + *playermov)) {
        if (bolposx >= 400 + *playermov && bolposx <= 450 + *playermov) {
            *bolcoeAy = -1;
            if (*bolcoeAx == 0) {
                *bolcoeAx = -1;
            }
        }
        if (bolposx > 450 + *playermov && bolposx <= 470) {
            *bolcoeAy = -1;
            *bolcoeAx = 0;
        }
        if (bolposx > 470 + *playermov && bolposx <= 500) {
            *bolcoeAy = -1;
            if (*bolcoeAx == 0) {
                *bolcoeAx = 1;
            }
        }
    }
}

void colisaoParede(int bolposy, int bolposx, int *bolcoeAx, int *bolcoeAy) {
    if (bolposy + 455 <= 20) {
        *bolcoeAy *= -1;
    }
    if (bolposx <= 20) {
        *bolcoeAx *= -1;
    }
    if (bolposx >= 620) {
        *bolcoeAx *= -1;
    }
}

// Função de teclado
void teclado(ALLEGRO_EVENT *ev, int *playermov, int *bolcon_Bola, int *playerpontos, int *playervidas) {
    if (ev->type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (ev->keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:
                if (400 + *playermov >= 20) {
                    *playermov -= 5;
                }
                break;
            case ALLEGRO_KEY_RIGHT:
                if (500 + *playermov <= 620) {
                    *playermov += 5;
                }
                break;
            case ALLEGRO_KEY_SPACE:
                *bolcon_Bola = 1;
                break;
            case ALLEGRO_KEY_P:
                *playerpontos += 20;
                break;
        }
    }
}

void iniciarBloco() {
    int i = 0, j = 0, k = 0;

    while (i < 75) {
        bloco[i].ativo = 1;
        bloco[i].pontos = 20;
        
        if (i < 25) {
            bloco[i].xc = 100 + 10 * (2 * i - 1);
            bloco[i].yc = 220;
        }
        if (i >= 25 && i < 50) {
            bloco[i].xc = 100 + 10 * (2 * j - 1);
            bloco[i].yc = 240;
            j++;
        }
        if (i >= 50 && i < 75) {
            bloco[i].xc = 100 + 10 * (2 * k - 1);
            bloco[i].yc = 260;
            k++;
        }
        
        if (i % 2 == 0) {
            bloco[i].cor_r = 35; bloco[i].cor_g = 107; bloco[i].cor_b = 142;
        } else if (i % 3 == 0) {
            bloco[i].cor_r = 165; bloco[i].cor_g = 42; bloco[i].cor_b = 42;
        } else {
            bloco[i].cor_r = 217; bloco[i].cor_g = 217; bloco[i].cor_b = 25;
        }
        i++;
    }
}

void colisaoBloco() {
    int i;
    for (i = 0; i < 75; i++) {
        if (bloco[i].ativo == 1) {
            if ((bol.posy + 460 == bloco[i].yc - 10 || bol.posy + 460 == bloco[i].yc + 10) && 
                (bol.posx >= bloco[i].xc - 10 && bol.posx <= bloco[i].xc + 10)) {
                bol.coeAy *= -1;
                bloco[i].ativo = 0;
                player.pontos += bloco[i].pontos + 10 * player.vidas;
            }
            if ((bol.posx == bloco[i].xc - 10 || bol.posx == bloco[i].xc + 10) && 
                (bol.posy + 460 >= bloco[i].yc - 10 && bol.posy + 460 <= bloco[i].yc + 10)) {
                bol.coeAx *= -1;
                bloco[i].ativo = 0;
                player.pontos += bloco[i].pontos - 10 * player.vidas;
            }
        }
    }
}

void sairJogo() {
    bol.posy = 0;
    bol.con_Bola = 0;
    bol.coeAy = -1;
    bol.coeAx = 0;
    player.con_colisao = 0;
}

int jogo() {
    player.vidas = 3;
    player.pontos = 0;
    player.con_colisao = 0;
    bol.coeAx = 0;
    bol.coeAy = -1;
    player.mov = 0;
    bol.con_Bola = 0;
    
    iniciarBloco();
    
    bool running = true;
    bool game_over = false;
    char interface[100];
    
    while (running) {
        while (al_get_next_event(event_queue, &event)) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                running = false;
            }
            
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    running = false;
                }
                if (!game_over) {
                    teclado(&event, &player.mov, &bol.con_Bola, &player.pontos, &player.vidas);
                }
            }
            
            if (event.type == ALLEGRO_EVENT_TIMER) {
                if (!game_over) {
                    colisaoParede(bol.posy, bol.posx, &bol.coeAx, &bol.coeAy);
                    colisaoBarra(bol.posy, bol.posx, &player.mov, &bol.coeAy, &bol.coeAx);
                    colisaoBloco();
                    
                    if (bol.con_Bola == 1) {
                        bol.posy = bol.posy + (1 * bol.coeAy);
                        bol.posx = bol.posx + (1 * bol.coeAx);
                    }
                    
                    if (460 + bol.posy >= 500) {
                        player.vidas--;
                        bol.con_Bola = 0;
                        bol.posy = 0;
                        bol.coeAy = -1;
                        bol.coeAx = 0;
                        player.con_colisao = 0;
                    }
                    
                    if (player.vidas <= 0) {
                        game_over = true;
                    }
                }
            }
        }
        
        // Desenhar
        al_clear_to_color(al_map_rgb(0, 0, 0));
        
        if (!game_over) {
            // Desenhar blocos
            int i;
            for (i = 0; i < 75; i++) {
                if (bloco[i].ativo == 1) {
                    al_draw_filled_rectangle(bloco[i].xc - 10, bloco[i].yc - 10, 
                                           bloco[i].xc + 10, bloco[i].yc + 10, 
                                           al_map_rgb(bloco[i].cor_r, bloco[i].cor_g, bloco[i].cor_b));
                }
            }
            
            // Desenhar bola
            if (bol.con_Bola == 0) {
                al_draw_filled_circle(450 + player.mov, 460, 5, al_map_rgb(255, 0, 0));
                bol.posx = 450 + player.mov;
            } else {
                al_draw_filled_circle(bol.posx, 460 + bol.posy, 5, al_map_rgb(255, 0, 0));
            }
            
            // Desenhar interface
            sprintf(interface, "Pontos: %d  Vidas: %d", player.pontos, player.vidas);
            al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, interface);
            
            // Desenhar paredes
            al_draw_filled_rectangle(0, 0, 20, 480, al_map_rgb(205, 205, 205));
            al_draw_filled_rectangle(0, 0, 640, 20, al_map_rgb(205, 205, 205));
            al_draw_filled_rectangle(620, 0, 640, 480, al_map_rgb(205, 205, 205));
            
            // Desenhar barra
            al_draw_filled_rectangle(400 + player.mov, 470, 500 + player.mov, 480, 
                                   al_map_rgb(153, 204, 50));
        } else {
            // Tela de Game Over
            sprintf(interface, "GAME OVER - Pontos: %d", player.pontos);
            al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 
                        ALLEGRO_ALIGN_CENTER, interface);
            al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 30, 
                        ALLEGRO_ALIGN_CENTER, "Pressione ESC para sair");
        }
        
        al_flip_display();
    }
    
    sairJogo();
    return player.pontos;
}

bool init() {
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro\n");
        return false;
    }
    
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Falha ao inicializar primitives addon\n");
        return false;
    }
    
    if (!al_init_font_addon()) {
        fprintf(stderr, "Falha ao inicializar font addon\n");
        return false;
    }
    
    if (!al_install_keyboard()) {
        fprintf(stderr, "Falha ao instalar teclado\n");
        return false;
    }
    
    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        fprintf(stderr, "Falha ao criar display\n");
        return false;
    }
    
    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "Falha ao criar event queue\n");
        return false;
    }
    
    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "Falha ao criar timer\n");
        return false;
    }
    
    font = al_create_builtin_font();
    if (!font) {
        fprintf(stderr, "Falha ao criar fonte\n");
        return false;
    }
    
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    
    al_start_timer(timer);
    
    return true;
}

void cleanup() {
    if (timer) al_destroy_timer(timer);
    if (font) al_destroy_font(font);
    if (event_queue) al_destroy_event_queue(event_queue);
    if (display) al_destroy_display(display);
}

int main() {
    if (!init()) {
        return -1;
    }
    
    al_set_window_title(display, "Breakout Game");
    
    bool running = true;
    int menu_option = 0;
    
    while (running) {
        while (al_get_next_event(event_queue, &event)) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                running = false;
            }
            
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_ESCAPE:
                        running = false;
                        break;
                    case ALLEGRO_KEY_I:
                        menu_option = 1;
                        break;
                    case ALLEGRO_KEY_R:
                        menu_option = 2;
                        break;
                    case ALLEGRO_KEY_M:
                        menu_option = 3;
                        break;
                    case ALLEGRO_KEY_S:
                        running = false;
                        break;
                }
            }
        }
        
        if (menu_option == 1) {
            int pontos = jogo();
            gravarRecordes(pontos);
            menu_option = 0;
        }
        
        if (menu_option == 2) {
            mostrarRecordes();
            menu_option = 0;
        }
        
        if (menu_option == 3) {
            mostrarManual();
            menu_option = 0;
        }
        
        // Desenhar menu
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 150, 
                    ALLEGRO_ALIGN_CENTER, "BREAKOUT GAME");
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 200, 
                    ALLEGRO_ALIGN_CENTER, "(I) Iniciar Jogo");
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 230, 
                    ALLEGRO_ALIGN_CENTER, "(R) Ver Recordes");
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 260, 
                    ALLEGRO_ALIGN_CENTER, "(M) Manual de Instruções");
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH/2, 290, 
                    ALLEGRO_ALIGN_CENTER, "(S) Sair");
        al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH/2, 350, 
                    ALLEGRO_ALIGN_CENTER, "Use as setas para mover, ESPAÇO para lançar");
        
        al_flip_display();
    }
    
    cleanup();
    return 0;
} 