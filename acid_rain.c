#include <stdio.h>
#include <string.h>
#include "acid_rain.h"

void clear_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void print_title(void) {
    printf("=== 한국어 산성비 타자 연습 ===\n");
    printf("난이도(1~10)를 고르고 단어를 입력하세요.\n");
    printf("오타가 나면 게임이 끝나고 맞힌 단어를 5개씩 보여줍니다.\n");
}

int get_level(void) {
    int level = 0;
    while (1) {
        printf("\n난이도 입력 (1~10): ");
        if (scanf("%d", &level) == 1) {
            clear_buffer();
            if (level >= 1 && level <= 10) {
                return level;
            }
        } else {
            clear_buffer();
        }
        printf("1부터 10 사이의 숫자만 입력하세요.\n");
    }
}

int make_pool(int level, WordItem dict[], int dict_size, char pool[][MAX_LEN]) {
    int count = 0;
    for (int i = 0; i < dict_size; i++) {
        if (level >= 7) {
            if (dict[i].level >= 6 && dict[i].level <= level) {
                strcpy(pool[count], dict[i].word);
                count++;
            }
        } else {
            if (dict[i].level <= level) {
                strcpy(pool[count], dict[i].word);
                count++;
            }
        }
    }
    return count;
}

void print_word(const char *word, int stage, int score, int level) {
    int row = stage % 5;
    printf("\n[난이도 %d | 성공 %d개]\n", level, score);
    for (int i = 0; i < 5; i++) {
        if (i == row) {
            printf("하강 %d: >>> %s <<<\n", i + 1, word);
        } else {
            printf("하강 %d: .\n", i + 1);
        }
    }
    printf("입력: ");
}

void read_line(char *str, int size) {
    int len;
    if (fgets(str, size, stdin) != NULL) {
        len = (int)strlen(str);
        while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
            str[len - 1] = '\0';
            len--;
        }
    } else {
        str[0] = '\0';
    }
}

void print_result(int level, int score, char history[][MAX_LEN]) {
    printf("\n=== 오타 발생! 게임 종료 ===\n");
    printf("난이도: %d | 오타 없이 성공한 단어 수: %d개\n", level, score);
    printf("[맞힌 단어 목록]\n");

    if (score == 0) {
        printf("없음\n");
        return;
    }

    for (int i = 0; i < score; i++) {
        if (i % 5 == 0) {
            printf("%d번~: ", i + 1);
        }
        printf("%s", history[i]);
        if ((i + 1) % 5 == 0 || i == score - 1) {
            printf("\n");
        } else {
            printf(", ");
        }
    }
}