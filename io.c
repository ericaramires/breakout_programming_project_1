/******************************************************************************
 * BREAKOUT GAME - INPUT/OUTPUT MODULE
 * Enhanced Professional Version
 ******************************************************************************/

#include "io.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===== RECORD MANAGEMENT =====

bool io_save_record(int score) {
    return true; // Stub for now
}

int io_load_records(int records[], int max_count) {
    FILE *file = fopen(RECORD_FILENAME, "r");
    if (!file) {
        return 0; // No records file exists yet
    }
    
    int count = 0;
    int score;
    
    while (fscanf(file, "%d", &score) == 1 && count < max_count) {
        records[count] = score;
        count++;
    }
    
    fclose(file);
    
    // Sort records in descending order (highest first)
    io_sort_records(records, count);
    
    return count;
}

void io_sort_records(int records[], int count) {
    // Simple bubble sort - good enough for small arrays
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (records[j] < records[j + 1]) {
                int temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }
}

int io_get_record_count(void) {
    FILE *file = fopen(RECORD_FILENAME, "r");
    if (!file) {
        return 0;
    }
    
    int count = 0;
    int temp;
    
    while (fscanf(file, "%d", &temp) == 1) {
        count++;
    }
    
    fclose(file);
    return count;
}

int io_get_highest_record(void) {
    int records[MAX_RECORDS];
    int count = io_load_records(records, MAX_RECORDS);
    
    if (count > 0) {
        return records[0]; // First record is highest after sorting
    }
    
    return 0;
}

bool io_is_new_record(int score) {
    return true; // Stub for now
}

// ===== GAME SETTINGS =====

bool io_save_settings(const GameSettings *settings) {
    FILE *file = fopen("settings.cfg", "w");
    if (!file) {
        return false;
    }
    
    fprintf(file, "volume=%d\n", settings->volume);
    fprintf(file, "fullscreen=%d\n", settings->fullscreen ? 1 : 0);
    fprintf(file, "difficulty=%d\n", settings->difficulty);
    
    fclose(file);
    return true;
}

bool io_load_settings(GameSettings *settings) {
    FILE *file = fopen("settings.cfg", "r");
    if (!file) {
        // Use default settings
        settings->volume = 100;
        settings->fullscreen = false;
        settings->difficulty = 1;
        return false;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "volume=", 7) == 0) {
            settings->volume = atoi(line + 7);
        } else if (strncmp(line, "fullscreen=", 11) == 0) {
            settings->fullscreen = (atoi(line + 11) == 1);
        } else if (strncmp(line, "difficulty=", 11) == 0) {
            settings->difficulty = atoi(line + 11);
        }
    }
    
    fclose(file);
    return true;
}

// ===== FILE UTILITIES =====

bool io_file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

bool io_create_backup(const char *filename) {
    if (!io_file_exists(filename)) {
        return false;
    }
    
    char backup_name[512];
    snprintf(backup_name, sizeof(backup_name), "%s.backup", filename);
    
    FILE *original = fopen(filename, "r");
    FILE *backup = fopen(backup_name, "w");
    
    if (!original || !backup) {
        if (original) fclose(original);
        if (backup) fclose(backup);
        return false;
    }
    
    int ch;
    while ((ch = fgetc(original)) != EOF) {
        fputc(ch, backup);
    }
    
    fclose(original);
    fclose(backup);
    return true;
} 