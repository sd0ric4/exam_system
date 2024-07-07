#ifndef STUDENT_H
#define STUDENT_H

#include <microhttpd.h>
#include <cjson/cJSON.h>

// 函数声明
void read_question_bank(char *path);
void choose_question_bank(char *filename);
void compare_answers_and_score(cJSON *question, const char *user_answer);
void display_question_and_get_answer(cJSON *question);
void select_random_questions(cJSON *questions, int count, cJSON *selected_questions);
cJSON *load_question_bank(const char *filename);

#endif // STUDENT_H