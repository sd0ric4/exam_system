#ifndef QUESTION_H
#define QUESTION_H

#include <cjson/cJSON.h>

#define MAX_DISPLAY 5 // 最多显示5个文件名

typedef struct {
    char *title;
    char *options[4];
    char *correctAnswer;
    char *type;
    int n_options;
} Question;

void shuffle_array(cJSON **array, int n);
cJSON *select_questions_randomly(int count, cJSON *questions);
Question parse_question_from_cjson(const cJSON *questionItem);
void free_question(Question *question);

#endif // QUESTION_H
