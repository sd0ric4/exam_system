#ifndef STUDENT_H
#define STUDENT_H

#include "microhttpd.h"
#include "cJSON.h"
#include <stdbool.h>

// 定义结构体，用于存储答案
typedef struct
{
    const char *user_answer;
    const char *correct_answer;
} Answer;

// 函数声明
void read_question_bank(char *path);
void choose_question_bank(char *filename);
bool compare_answers_and_score(cJSON *question, const char *user_answer);
bool display_question_and_get_answer(cJSON *question);
void select_random_questions(cJSON *questions, int count, cJSON *selected_questions);
enum MHD_Result handle_request(void *cls, struct MHD_Connection *connection,
                               const char *url, const char *method,
                               const char *version, const char *upload_data,
                               size_t *upload_data_size, void **con_cls);
enum MHD_Result answer_question(struct MHD_Connection *connection, const char *question_id, const char *answer);
enum MHD_Result select_questions(struct MHD_Connection *connection, int count);

// 定义题库路径
#define QUESTION_BANK_PATH "./QuestionBank"
// 定义初始分数
#define INIT_SCORE 100
// 定义端口
#define PORT 8888
// 题库
extern cJSON *question_bank;
// 全局变量，用于存储分数
extern double score;
// 全局变量，用于存储题目总数
extern int question_count;
// 全局变量，用于存储正确答题数
extern int correct_count;

#endif // STUDENT_H
