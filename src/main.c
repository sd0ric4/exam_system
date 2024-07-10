#include "question.h"
#include "ui.h"
#include "file_utils.h"
#include "student.h"
#include "mode.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <cjson/cJSON.h>

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    srand(time(NULL));

    int backend_mode = 0; // 0 for TUI, 1 for backend, 2 for better_tui

    // 加载默认题库
    question_bank = load_question_bank("./QuestionBank/ch16.json");
    if (question_bank == NULL)
    {
        printf("题库加载失败。\n");
        return 1;
    }

    // 解析命令行参数
    int opt;
    while ((opt = getopt(argc, argv, "btgmeh")) != -1)
    {
        switch (opt)
        {
        case 'b':
            backend_mode = 1;
            break;
        case 't':
            backend_mode = 0;
            break;
        case 'g':
            backend_mode = 2;
            break;
        case 'm':
            backend_mode = 3;
            break;
        case 'e':
            backend_mode = 4;
            break;
        case 'h':
            fprintf(stderr, "Usage: %s [-b for backend mode] [-t for TUI mode] [-g for better TUI mode] [-m for manage mode] [-e for exercise management mode]\n", argv[0]);
            return 1;
        default:
            fprintf(stderr, "Usage: %s [-b for backend mode] [-t for TUI mode] [-g for better TUI mode] [-m for manage mode] [-e for exercise management mode]\n", argv[0]);
            return 1;
        }
    }

    if (backend_mode == 1)
    {
        run_backend_mode();
    }
    else if (backend_mode == 2)
    {
        run_better_tui_mode();
    }
    else if (backend_mode == 3)
    {
        run_manage_mode();
    }
    else if (backend_mode == 4)
    {
        run_exercise_management_mode();
    }
    else if (backend_mode == 0)
    {
        run_tui_mode();
    }

    return 0;
}
