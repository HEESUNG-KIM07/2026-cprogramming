#ifndef ACID_RAIN_H
#define ACID_RAIN_H

#define MAX_WORDS 45
#define MAX_LEN 100
#define MAX_HISTORY 500

typedef struct {
    char word[MAX_LEN];
    int level;
} WordItem;

void clear_buffer(void);
void print_title(void);
int get_level(void);
int make_pool(int level, WordItem dict[], int dict_size, char pool[][MAX_LEN]);
void print_word(const char *word, int stage, int score, int level);
void read_line(char *str, int size);
void print_result(int level, int score, char history[][MAX_LEN]);

#endif