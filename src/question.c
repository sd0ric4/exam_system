#include "question.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void shuffle_array(cJSON **array, int n) {
    if (array == NULL || n <= 0) {
        fprintf(stderr, "打乱数组时出错\n");
        return;
    }

    if (n > 1) {
        srand(time(NULL));
        for (int i = 0; i < n - 1; i++) {
            int j = i + rand() / (RAND_MAX / (n - i) + 1);
            cJSON *temp = array[j];
            array[j] = array[i];
            array[i] = temp;
        }
    }
}

cJSON *select_questions_randomly(int count, cJSON *questions) {
    cJSON *selected_questions = cJSON_CreateArray();
    int n_questions = cJSON_GetArraySize(questions);
    cJSON **questions_array = (cJSON **)malloc(n_questions * sizeof(cJSON *));

    if (questions_array == NULL) {
        fprintf(stderr, "分配内存时出错\n");
        exit(EXIT_FAILURE);
    }
    if (count > n_questions) {
        count = n_questions;
    }

    for (int i = 0; i < n_questions; ++i) {
        questions_array[i] = cJSON_GetArrayItem(questions, i);
    }

    shuffle_array(questions_array, n_questions);

    for (int i = 0; i < count; ++i) {
        cJSON_AddItemToArray(selected_questions, cJSON_Duplicate(questions_array[i], 1));
    }

    free(questions_array);
    return selected_questions;
}

Question parse_question_from_cjson(const cJSON *questionItem) {
    Question question;
    const cJSON *title = cJSON_GetObjectItem(questionItem, "title");
    const cJSON *options = cJSON_GetObjectItem(questionItem, "options");
    const cJSON *answer = cJSON_GetObjectItem(questionItem, "correctAnswer");
    const cJSON *type = cJSON_GetObjectItem(questionItem, "type");
    const cJSON *optionItem;

    const char *optionsArray[4];
    int i = 0;

    if (!cJSON_IsString(title) || !cJSON_IsArray(options) || !cJSON_IsString(answer) || !cJSON_IsString(type)) {
        fprintf(stderr, "解析题目时出错\n");
        exit(EXIT_FAILURE);
    }

    question.title = strdup(title->valuestring);
    question.correctAnswer = strdup(answer->valuestring);
    question.type = strdup(type->valuestring);
    question.n_options = cJSON_GetArraySize(options);

    cJSON_ArrayForEach(optionItem, options) {
        cJSON *optionValue = optionItem->child;
        optionsArray[i++] = optionValue->valuestring;
        question.options[i - 1] = strdup(optionValue->valuestring);
    }

    return question;
}

void free_question(Question *question) {
    free(question->title);
    free(question->correctAnswer);
    free(question->type);
    for (int i = 0; i < question->n_options; ++i) {
        free(question->options[i]);
    }
}
