#ifndef STUDENT_H
#define STUDENT_H

#include <microhttpd.h>
#include <cjson/cJSON.h>

// 函数声明
void read_question_bank(char *path);
void choose_question_bank(char *filename);
bool compare_answers_and_score(cJSON *question, const char *user_answer);
bool display_question_and_get_answer(cJSON *question);
void select_random_questions(cJSON *questions, int count, cJSON *selected_questions);
cJSON *load_question_bank(const char *filename);
static enum MHD_Result handle_request(void *cls, struct MHD_Connection *connection,
                                      const char *url, const char *method,
                                      const char *version, const char *upload_data,
                                      size_t *upload_data_size, void **con_cls);
static enum MHD_Result answer_question(struct MHD_Connection *connection, const char *question_id, const char *answer);
static enum MHD_Result select_questions(struct MHD_Connection *connection, int count);

#endif // STUDENT_H