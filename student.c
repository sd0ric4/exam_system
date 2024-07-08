#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <microhttpd.h>
#include <cjson/cJSON.h>
#include <locale.h>
#include <dirent.h>
#include "student.h"

cJSON *question_bank = NULL;

void read_question_bank(char *path)
{
    DIR *dir;
    struct dirent *ptr;
    dir = opendir(path);
    printf("正在读取题库...\n");
    if (readdir(dir) != NULL)
    {
        printf("题库列表：\n");
    }

    while ((ptr = readdir(dir)) != NULL)
    {
        if (ptr->d_type == 8)
        {
            char *filename = ptr->d_name;
            char *name = filename;
            // 添加颜色
            printf("\033[34m%s\033[0m\n", name);
        }
    }
    closedir(dir);
}

void choose_question_bank(char *filename)
{
    question_bank = load_question_bank(filename);
    if (question_bank == NULL)
    {
        printf("题库加载失败。\n");
        exit(1);
    }
}

cJSON *load_question_bank(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("fopen");
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *file_content = (char *)malloc(file_size + 1);
    fread(file_content, 1, file_size, fp);
    file_content[file_size] = '\0';

    cJSON *question_bank = cJSON_Parse(file_content);

    free(file_content);
    fclose(fp);

    return question_bank;
}

void select_random_questions(cJSON *questions, int count, cJSON *selected_questions)
{
    for (int i = 0; i < count; i++)
    {
        int index = rand() % cJSON_GetArraySize(questions);
        cJSON *question = cJSON_GetArrayItem(questions, index);
        cJSON_AddItemToArray(selected_questions, cJSON_Duplicate(question, 1));
    }
}

bool display_question_and_get_answer(cJSON *question)
{
    printf("\033[1m%s\033[0m \n\033[31m%s\033[0m\n", cJSON_GetObjectItem(question, "type")->valuestring, cJSON_GetObjectItem(question, "title")->valuestring);
    cJSON *options = cJSON_GetObjectItem(question, "options");
    if (options != NULL)
    {
        for (int i = 0; i < cJSON_GetArraySize(options); i++)
        {
            cJSON *option = cJSON_GetArrayItem(options, i);
            if (option != NULL)
            {
                cJSON *key = option->child;
                if (key != NULL)
                {
                    printf("%s: %s\n", key->string, key->valuestring);
                }
            }
        }
    }
    char user_answer[16];
    printf("请输入答案：");
    scanf("%s", user_answer);
    return compare_answers_and_score(question, user_answer);
}

bool compare_answers_and_score(cJSON *question, const char *user_answer)
{
    bool is_correct = false;
    const char *correct_answer = cJSON_GetObjectItem(question, "correctAnswer")->valuestring;
    char upper_case_answer[strlen(user_answer) + 1];

    for (int i = 0; i < strlen(user_answer); i++)
    {
        if (user_answer[i] >= 'a' && user_answer[i] <= 'z')
        {
            upper_case_answer[i] = user_answer[i] - 32;
        }
        else
        {
            upper_case_answer[i] = user_answer[i];
        }
    }
    upper_case_answer[strlen(user_answer)] = '\0';

    if (strcmp(correct_answer, upper_case_answer) == 0)
    {
        printf("\033[32m回答正确!\033[0m\n\n");
        is_correct = true;
    }
    else
    {
        printf("\033[31m回答错误!\033[0m 正确答案是：\033[32m%s\033[0m\n\n", correct_answer);
    }
    return is_correct;
}

enum MHD_Result answer_question(struct MHD_Connection *connection, const char *question_id, const char *answer)
{
    cJSON *question = cJSON_GetArrayItem(question_bank, atoi(question_id));
    if (!question)
    {
        const char *error = "Question not found";
        return MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, MHD_create_response_from_buffer(strlen(error), (void *)error, MHD_RESPMEM_PERSISTENT));
    }

    const char *correct_answer = cJSON_GetObjectItem(question, "correctAnswer")->valuestring;
    char response[256];
    if (strcmp(correct_answer, answer) == 0)
    {
        sprintf(response, "回答正确!");
    }
    else
    {
        sprintf(response, "回答错误! 正确答案是：%s", correct_answer);
    }

    return MHD_queue_response(connection, MHD_HTTP_OK, MHD_create_response_from_buffer(strlen(response), (void *)response, MHD_RESPMEM_MUST_COPY));
}

enum MHD_Result select_questions(struct MHD_Connection *connection, int count)
{
    cJSON *selected_questions = cJSON_CreateArray();
    select_random_questions(question_bank, count, selected_questions);
    char *response_str = cJSON_Print(selected_questions);
    int response_len = strlen(response_str);

    int ret = MHD_queue_response(connection, MHD_HTTP_OK, MHD_create_response_from_buffer(response_len, (void *)response_str, MHD_RESPMEM_MUST_COPY));

    free(response_str);
    cJSON_Delete(selected_questions);

    return ret;
}

enum MHD_Result handle_request(void *cls, struct MHD_Connection *connection,
                               const char *url, const char *method,
                               const char *version, const char *upload_data,
                               size_t *upload_data_size, void **con_cls)
{
    if (strcmp(method, "GET") == 0)
    {
        if (strncmp(url, "/questions", 10) == 0)
        {
            const char *count_str = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "count");
            int count = count_str ? atoi(count_str) : 1;
            return select_questions(connection, count);
        }
        else if (strncmp(url, "/answer", 7) == 0)
        {
            const char *question_id = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "id");
            const char *answer = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "answer");
            if (question_id && answer)
            {
                return answer_question(connection, question_id, answer);
            }
        }
    }

    const char *error = "Not found";
    return MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, MHD_create_response_from_buffer(strlen(error), (void *)error, MHD_RESPMEM_PERSISTENT));
}
