#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <getopt.h>
#include <microhttpd.h>
#include "student.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");

    srand(time(NULL));

    // 加载默认题库
    question_bank = load_question_bank("./QuestionBank/ch16.json");
    if (question_bank == NULL)
    {
        printf("题库加载失败。\n");
        return 1;
    }

    int backend_mode = 0; // 0 for TUI, 1 for backend

    // 解析命令行参数
    int opt;
    while ((opt = getopt(argc, argv, "bt")) != -1)
    {
        switch (opt)
        {
        case 'b':
            backend_mode = 1;
            break;
        case 't':
            backend_mode = 0;
            break;
        default:
            fprintf(stderr, "Usage: %s [-b for backend mode] [-t for TUI mode]\n", argv[0]);
            return 1;
        }
    }

    if (backend_mode)
    {
        struct MHD_Daemon *daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL, &handle_request, NULL, MHD_OPTION_END);
        if (NULL == daemon)
        {
            printf("Failed to start HTTP server.\n");
            return 1;
        }

        printf("HTTP server is running on port %d\n", PORT);

        getchar();

        MHD_stop_daemon(daemon);
    }
    else
    {
        read_question_bank(QUESTION_BANK_PATH);
        printf("\033[1m请输入题库名:\033[0m\n");
        char full_path[512]; // 假设这个大小足够存储 QUESTION_BANK_PATH 和 filename 的连接
        char filename[256];
        scanf("%s", filename);
        strcpy(full_path, QUESTION_BANK_PATH); // 首先复制 QUESTION_BANK_PATH 到缓冲区
        strcat(full_path, "/");                // 然后连接 /
        strcat(full_path, filename);           // 然后连接 filename
        printf("您选择的题库路径是:\033[34m%s\033[0m\n", full_path);
        choose_question_bank(full_path);
        printf("\033[32m题库加载成功\033[0m\n");
        double score = INIT_SCORE;
        printf("\033[1m请选择你要多少道题目:\033[0m");
        int count;
        scanf("%d", &count);

        int questions_to_display = count;
        int question_bank_size = cJSON_GetArraySize(question_bank);
        int correct_count = 0;
        for (int i = 0; i < questions_to_display; i++)
        {
            int index = rand() % question_bank_size;
            cJSON *question = cJSON_GetArrayItem(question_bank, index);
            if (display_question_and_get_answer(question))
            {
                correct_count++;
            }
        }
        printf("\033[32m正确答题数：%d\n\033[0m", correct_count); // 绿色
        printf("\033[34m总题数：%d\n\033[0m", count);             // 蓝色
        double final_score = score * correct_count / count;
        printf("\033[35m最终得分：%.2f\n\033[0m", final_score); // 紫色
    }

    cJSON_Delete(question_bank);

    return 0;
}
