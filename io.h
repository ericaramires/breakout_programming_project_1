#ifndef IO_H
#define IO_H

#include <stdbool.h>

// ===== RECORD STRUCTURE =====
typedef struct Record {
    int score;
    char player_name[32];
    char date[32];
    int level_reached;
} Record;

// ===== FUNCTION DECLARATIONS =====

// Record management
bool records_load(Record records[], int max_records);
bool records_save(Record records[], int count);
void records_add(Record records[], int *count, int score, const char *name, int level);
void records_sort(Record records[], int count);
bool records_is_high_score(Record records[], int count, int score);

// File operations
bool file_exists(const char *filename);
void create_default_records_file(void);

// Input utilities
void get_player_name(char *name, int max_length);

#endif // IO_H 