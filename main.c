#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "acid_rain.h"

int main(void) {
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

    char pool[MAX_WORDS][MAX_LEN];
    char history[MAX_HISTORY][MAX_LEN];
    char input[MAX_LEN];
    int level, pool_size, score = 0, stage = 0;

    srand((unsigned int)time(NULL));
    print_title();
    level = get_level();
    pool_size = make_pool(level, dict, MAX_WORDS, pool);

    while (1) {
        int idx = rand() % pool_size;
        print_word(pool[idx], stage, score, level);
        read_line(input, MAX_LEN);

        if (strcmp(input, pool[idx]) == 0) {
            if (score < MAX_HISTORY) {
                strcpy(history[score], pool[idx]);
            }
            score++;
            stage++;
            printf("정답! (연속 %d개)\n", score);
        } else {
            printf("목표: %s / 입력: %s\n", pool[idx], input);
            print_result(level, score, history);
            break;
        }
    }

    return 0;
}