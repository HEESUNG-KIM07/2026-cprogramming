# C프로그래밍 기말 프로젝트 - 산성비 타자 연습
## 핵심 코드 구조는 난이도(1~10)에 따른 단어 조절을 하였고, 시각적으로 약간의 압박을 주도록 설계 하였으며, 오타 시 즉시 종료와 연속 정답과 사용자가 친 단어 기록을 보여주는 형태로 요약된다. 이 코드만을 다룬 논문이 있는 것은 아니지만, 관련된 코드와 이미 여러 사람들이 만들고, 짜본 코드를 보아 작성했다.
### 전체적인 구조로는 1.속도, 2. 인지, 3 피드백 이다. 첫 번째는 쉬운 단어이거나 혹은 빠르게 누를 수 있는 단어가 나오고 입력하는 순간 속도를 높여 빠를수록 오타가 늘기 쉬운 경향을 다루었으며, 입력과 길이가 커지거나 반복되면 정확도 유지가 어려워진다는 점과 연결된다. 두 번째는 첫번째와 말이 이어지는데 단어가 길어진다는 것은 단계적으로 키 를 입력할때 처리해야할 정보량이 늘어나 반응 속도나 정확도에 영향을 주는 원리 이다. 세 번째 로는 즉시 종료와 연속 정답 그리고 결과를 요약해 반복 훈련의 몰입도와 자기 점검을 높이는 요소이다.
#### 강조할 단어(make_pool) : 1~10단계에 맞춰 단어 길이와 난이도를 나눔
#### 강조할 단어(score) : 오타 허용 없이 누적 기록을 쌓다가 틀리면 바로 끝나는 게임 규칙을 보여줌
#### 강조할 단어(print_result) : % 5 == 0을 이용해 맞힌 단어를 한 줄에 5개씩 깔끔하게 정리한 출력
#### 강조할 단어(clear_buffer) : 뒤 남은 개행 문자를 비워 다음 입력이 꼬이지 않게 막는 기초 안전장치

##### 난이도 설정 및 필터링 (level, make_pool, pool): get_level로 1~10 사이의 level을 받은 뒤, make_pool이 조건에 맞는 단어를 골라 pool 배열에 담음

단어 출력과 입력 판정 (stage, print_word, read_line, strcmp): stage에 맞춰 print_word로 단어를 띄우고, read_line으로 입력을 받은 다음 strcmp로 정답 여부를 비교

연속 정답 기록 (score, history): 정답일 때마다 score를 늘리고 history 배열에 맞힌 단어를 저장하며, 오타가 나면 루프를 멈춤

5개씩 결과 정리 (print_result): 게임이 끝나면 print_result에서 score만큼 반복하며 history를 5개씩 묶어 출력

입력 버퍼 비우기 (clear_buffer): scanf 뒤에 남은 문자를 clear_buffer로 지워 입력 꼬임을 막음

##### #include <stdio.h>   /* printf, scanf, getchar, fgets 등 입출력 함수 사용 */
#include <stdlib.h>  /* rand, srand 등 난수 생성 함수 사용 */
#include <string.h>  /* strcpy, strcmp, strlen 등 문자열 처리 함수 사용 */
#include <time.h>    /* time() 함수로 실행할 때마다 다른 난수 시드 생성 */

#define MAX_WORDS 45     /* 사전에 들어갈 전체 단어 개수 */
#define MAX_LEN 100      /* 한 단어(또는 구절)와 사용자 입력의 최대 길이 */
#define MAX_HISTORY 500  /* 오타 없이 맞힌 단어를 기록할 최대 개수 */

/* 단어 하나와 그 단어의 난이도를 묶어 저장하는 구조체 */
typedef struct {
    char word[MAX_LEN];  /* 출제할 한국어 단어/문장 */
    int level;           /* 해당 단어의 난이도 (1~10) */
} WordItem;

/* 함수 전방 선언: main 함수보다 아래에 정의된 함수들을 미리 알려줌 */
void clear_buffer(void);                                              /* 입력 버퍼에 남은 찌꺼기 문자 비우기 */
void print_title(void);                                               /* 게임 제목과 규칙 안내 출력 */
int get_level(void);                                                  /* 1~10 사이의 난이도 입력받기 */
int make_pool(int level, WordItem dict[], int dict_size, char pool[][MAX_LEN]); /* 난이도에 맞는 단어 목록 만들기 */
void print_word(const char *word, int stage, int score, int level);   /* 현재 상태와 단어를 화면에 출력 */
void read_line(char *str, int size);                                  /* 공백 포함 한 줄 입력받고 끝 개행 제거 */
void print_result(int level, int score, char history[][MAX_LEN]);     /* 게임 종료 후 결과 및 맞힌 단어 5개씩 출력 */

/* stdin 입력 버퍼에 남은 '\n'(엔터)이나 불필요한 문자를 끝까지 비우는 함수 */
void clear_buffer(void) {
    int ch;  /* getchar() 반환값(문자 또는 EOF)을 담을 변수 */
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* 줄바꿈('\n')이나 입력 끝(EOF)을 만날 때까지 한 글자씩 읽어서 버림 */
    }
}

/* 게임 시작 시 제목과 간단한 규칙을 출력하는 함수 */
void print_title(void) {
    printf("=== 한국어 산성비 타자 연습 ===\n");
    printf("난이도(1~10)를 고르고 단어를 입력하세요.\n");
    printf("오타가 나면 게임이 끝나고 맞힌 단어를 5개씩 보여줍니다.\n");
}

/* 1부터 10 사이의 올바른 정수가 입력될 때까지 반복해서 입력받는 함수 */
int get_level(void) {
    int level = 0;  /* 사용자가 입력할 난이도 값 */

    while (1) {  /* 올바른 값이 들어올 때까지 무한 반복 */
        printf("\n난이도 입력 (1~10): ");

        if (scanf("%d", &level) == 1) {  /* 정수를 1개 제대로 읽었는지 확인 */
            clear_buffer();              /* 숫자 뒤에 남은 '\n'을 비워 다음 입력 꼬임 방지 */
            if (level >= 1 && level <= 10) {  /* 범위가 1 이상 10 이하인지 검사 */
                return level;            /* 유효한 난이도를 반환하고 함수 종료 */
            }
        } else {
            clear_buffer();              /* 문자를 입력했을 때 버퍼에 남은 잘못된 입력 제거 */
        }

        printf("1부터 10 사이의 숫자만 입력하세요.\n");  /* 잘못된 입력 안내 */
    }
}

/* 선택한 level에 맞춰 dict(사전)에서 단어들을 골라 pool 배열에 담는 함수 */
int make_pool(int level, WordItem dict[], int dict_size, char pool[][MAX_LEN]) {
    int count = 0;  /* pool에 담긴 단어 개수를 세는 변수 */

    for (int i = 0; i < dict_size; i++) {  /* 전체 사전을 처음부터 끝까지 순회 */
        if (level >= 7) {  /* 고난이도(7~10)를 선택한 경우 */
            if (dict[i].level >= 6 && dict[i].level <= level) {  /* 난이도 6 이상~선택 난이도 이하 단어 선별 */
                strcpy(pool[count], dict[i].word);  /* 조건에 맞으면 단어를 pool에 복사 */
                count++;                            /* 담은 단어 개수 1 증가 */
            }
        } else {  /* 1~6 난이도를 선택한 경우 */
            if (dict[i].level <= level) {  /* 선택한 난이도 이하의 단어 선별 */
                strcpy(pool[count], dict[i].word);  /* 조건에 맞으면 단어를 pool에 복사 */
                count++;                            /* 담은 단어 개수 1 증가 */
            }
        }
    }

    return count;  /* 선택된 단어의 총 개수를 반환 */
}

/* 현재 난이도, 연속 정답 수(score), 하강 단계(stage)와 함께 목표 단어를 출력하는 함수 */
void print_word(const char *word, int stage, int score, int level) {
    int row = stage % 5;  /* 0~4 사이의 값으로, 단어가 몇 번째 줄에 표시될지 결정 */

    printf("\n[난이도 %d | 성공 %d개]\n", level, score);  /* 상단 상태 표시 */

    for (int i = 0; i < 5; i++) {  /* 5줄의 낙하 칸을 위에서 아래로 출력 */
        if (i == row) {  /* 현재 단어가 위치한 줄이면 단어를 강조해서 출력 */
            printf("하강 %d: >>> %s <<<\n", i + 1, word);
        } else {         /* 그 외 줄은 빈 낙하 칸 표시 */
            printf("하강 %d: .\n", i + 1);
        }
    }

    printf("입력: ");  /* 사용자 입력 프롬프트 */
}

/* 한 줄 전체(띄어쓰기 포함)를 안전하게 읽고 문자열 끝의 개행 문자를 지우는 함수 */
void read_line(char *str, int size) {
    int len;  /* 문자열 길이를 저장할 변수 */

    if (fgets(str, size, stdin) != NULL) {  /* 최대 size-1 길이만큼 한 줄 읽기 */
        len = (int)strlen(str);             /* 읽은 문자열의 길이 계산 */

        /* 문자열 끝에 붙은 '\n'(LF)이나 '\r'(CR)을 모두 '\0'(문자열 끝)으로 치환 */
        while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
            str[len - 1] = '\0';
            len--;
        }
    } else {
        str[0] = '\0';  /* 입력 실패 시 빈 문자열로 처리 */
    }
}

/* 오타로 게임이 끝났을 때 최종 점수와 맞힌 단어들을 한 줄에 5개씩 출력하는 함수 */
void print_result(int level, int score, char history[][MAX_LEN]) {
    printf("\n=== 오타 발생! 게임 종료 ===\n");
    printf("난이도: %d | 오타 없이 성공한 단어 수: %d개\n", level, score);
    printf("[맞힌 단어 목록]\n");

    if (score == 0) {  /* 첫 문제부터 틀려서 맞힌 단어가 없는 경우 */
        printf("없음\n");
        return;
    }

    for (int i = 0; i < score; i++) {  /* 맞힌 단어 개수(score)만큼 반복 */
        if (i % 5 == 0) {  /* 0, 5, 10... 번째(각 줄의 첫 단어 앞)에 시작 번호 출력 */
            printf("%d번~: ", i + 1);
        }

        printf("%s", history[i]);  /* 저장해 둔 단어 출력 */

        if ((i + 1) % 5 == 0 || i == score - 1) {  /* 5개를 채웠거나 마지막 단어면 줄바꿈 */
            printf("\n");
        } else {                                    /* 같은 줄에 단어가 더 이어지면 쉼표로 구분 */
            printf(", ");
        }
    }
}

/* 프로그램의 시작점인 main 함수 */
int main(void) {
    /* 전체 단어 사전 배열 초기화 (단어, 최소 난이도) */
    WordItem dict[MAX_WORDS] = {
        {"하늘", 1}, {"바람", 1}, {"구름", 1}, {"바다", 1}, {"나무", 1},
        {"꽃잎", 1}, {"별빛", 2}, {"노을", 2}, {"새벽", 2}, {"모래", 2},
        {"초원", 2}, {"계절", 3}, {"여행", 3}, {"그림자", 3}, {"파도", 3},
        {"무지개", 4}, {"소나기", 4}, {"산들바람", 4}, {"책갈피", 4}, {"정거장", 5},
        {"은하수", 5}, {"돌멩이", 5}, {"나침반", 5}, {"발자국", 6}, {"단풍잎", 6},
        {"속삭임", 6}, {"황금빛", 6}, {"백일몽", 7}, {"신기루", 7}, {"어둠속의빛", 7},
        {"겨울왕국", 7}, {"시간여행자", 7}, {"유리구두", 7}, {"천둥번개", 7}, {"미로찾기", 8},
        {"심연의바다", 8}, {"불꽃놀이", 8}, {"황혼의빛", 8}, {"기억의조각", 8},
        {"차가운바람이분다", 9}, {"빗방울이창문을두드린다", 9}, {"끝없는밤하늘의별", 9},
        {"고요한새벽의안개", 9}, {"거센폭풍우가몰아친다", 10}, {"빛과어둠의경계선", 10}
    };

    char pool[MAX_WORDS][MAX_LEN];     /* 선택된 난이도에 맞는 단어들을 저장할 배열 */
    char history[MAX_HISTORY][MAX_LEN];  /* 오타 없이 맞힌 단어들을 순서대로 저장할 배열 */
    char input[MAX_LEN];                 /* 사용자가 키보드로 입력한 문자열을 담을 배열 */
    int level;                           /* 사용자가 고른 난이도 (1~10) */
    int pool_size;                       /* pool 배열에 들어간 단어의 개수 */
    int score = 0;                       /* 오타 없이 맞힌 연속 단어 수 */
    int stage = 0;                       /* 단어가 내려온 단계(줄 위치 계산용) */

    srand((unsigned int)time(NULL));  /* 현재 시각을 기준으로 난수 초기값 설정 */
    print_title();                     /* 시작 화면 출력 */
    level = get_level();               /* 난이도 입력받기 */
    pool_size = make_pool(level, dict, MAX_WORDS, pool);  /* 난이도별 단어 풀 생성 */

    while (1) {  /* 오타가 날 때까지 문제 출제 및 채점 반복 */
        int idx = rand() % pool_size;  /* pool 배열 범위(0 ~ pool_size-1) 안에서 무작위 인덱스 선택 */

        print_word(pool[idx], stage, score, level);  /* 선택된 단어와 현재 상태 출력 */
        read_line(input, MAX_LEN);                   /* 사용자 입력 읽기 */

        if (strcmp(input, pool[idx]) == 0) {  /* 입력 문자열과 출제 단어가 완전히 같으면(0 반환) */
            if (score < MAX_HISTORY) {        /* 기록 배열 한도를 넘지 않았는지 확인 */
                strcpy(history[score], pool[idx]);  /* 맞힌 단어를 history에 저장 */
            }
            score++;   /* 연속 정답 수 1 증가 */
            stage++;   /* 다음 출제 시 하강 위치 1칸 이동 */
            printf("정답! (연속 %d개)\n", score);
        } else {  /* 한 글자라도 다르면 오타 처리 */
            printf("목표: %s / 입력: %s\n", pool[idx], input);  /* 정답과 내 입력 비교 출력 */
            print_result(level, score, history);              /* 최종 결과 및 맞힌 단어 5개씩 출력 */
            break;                                            /* while 루프 탈출 후 게임 종료 */
        }
    }

    return 0;  /* 프로그램 정상 종료 */
}
