#ifndef STUDENT_H
#define STUDENT_H

#include <microhttpd.h>
#include <cjson/cJSON.h>
#include <stdbool.h>
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

#endif // STUDENT_H
