#ifndef STUDENT_H
#define STUDENT_H

#include <cjson/cJSON.h>

// 函数声明
void compare_answers_and_score(cJSON *question, const char *user_answer);
void display_question_and_get_answer(cJSON *question);
void select_random_questions(cJSON *questions, int count);
cJSON *load_question_bank(const char *filename);

#endif // STUDENT_H