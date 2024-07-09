#include "student.h"
#include <cjson/cJSON.h>
#include <stdbool.h>
#include <dirent.h>
#include <locale.h> // 包含locale.h头文件
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * 输入题量随机选择题目
 * @param conunt 题量
 * @param 导入的题库
 * @return 返回的题，cJSON对象
 */
cJSON *select_questions_randomly(int count, cJSON *questions)
{
    cJSON *selected_questions = cJSON_CreateArray();
    int n_questions = cJSON_GetArraySize(questions);
    for (int i = 0; i < count; ++i)
    {
        int index = rand() % n_questions;
        cJSON *question = cJSON_GetArrayItem(questions, index);
        cJSON_AddItemToArray(selected_questions, cJSON_Duplicate(question, 1));
    }
    return selected_questions;
}

/**
 * 输入数量的页面
 * @return 返回输入的数量
 */
int input_count_page()
{
    initscr(); // 初始化屏幕
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();                         // 初始化颜色系统
    init_pair(1, COLOR_BLUE, COLOR_BLACK); // 初始化颜色对

    mvprintw(1, 5, "输入题量:");
    char input[256];
    int ch;
    int i = 0;
    while (1)
    {
        ch = getch();
        if (ch >= '0' && ch <= '9')
        {
            input[i++] = ch;
            input[i] = '\0';
            mvprintw(2, 5, "%s", input);
        }
        else if (ch == 127) // Backspace key
        {
            if (i > 0)
            {
                input[--i] = '\0';
                mvprintw(2, 5, "%s", input);
            }
        }
        else if (ch == 10) // Enter key
        {
            break;
        }
    }
    int count = atoi(input);
    clear();
    return count;
}

/**
 * 展示选择题库文件的导航页
 * @param path 题库文件夹路径
 * @return 返回所选题库文件名
 */
char *navigate_question_bank(const char *path)
{
    DIR *dir;
    struct dirent *ptr;
    char *files[256];
    int count = 0;
    int highlight = 0;
    int ch;
    static char selected_file[256];

    initscr(); // 初始化屏幕
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();                         // 初始化颜色系统
    init_pair(1, COLOR_BLUE, COLOR_BLACK); // 初始化颜色对

    dir = opendir(path);
    if (!dir)
    {
        mvprintw(1, 5, "无法打开题库目录: %s", path);
        getch();  // 等待用户按键
        endwin(); // 恢复终端设置
        return NULL;
    }

    mvprintw(1, 5, "选择题库文件:");
    while ((ptr = readdir(dir)) != NULL)
    {
        if (ptr->d_type == 8)
        {
            files[count] = strdup(ptr->d_name);
            mvprintw(count + 2, 5, "%s", files[count]);
            count++;
        }
    }
    closedir(dir);

    while (1)
    {
        for (int i = 0; i < count; ++i)
        {
            if (i == highlight)
                attron(A_REVERSE);
            mvprintw(i + 2, 5, "%s", files[i]);
            if (i == highlight)
                attroff(A_REVERSE);
        }

        ch = getch();
        switch (ch)
        {
        case KEY_UP:
            highlight = (highlight - 1 + count) % count;
            break;
        case KEY_DOWN:
            highlight = (highlight + 1) % count;
            break;
        case 10: // Enter key
            strcpy(selected_file, files[highlight]);
            for (int i = 0; i < count; ++i)
            {
                free(files[i]);
            }
            clear();
            return selected_file;
        }
    }
}
/**
 * 展示问题
 * @param question 问题
 */
void display_question(const char *question)
{
    mvprintw(2, 5, "%s", question);
}

/**
 * 展示选项
 * @param options 选项
 * @param n_options 选项数量
 * @param highlight 高亮
 * @param selected_flags 选中的选项标记
 * @param is_multiple_choice 是否为多选题
 */
void display_options(const char **options, int n_options, int highlight,
                     int *selected_flags, int is_multiple_choice)
{
    const char labels[] = {'A', 'B', 'C', 'D'}; // 定义ABCD标签
    for (int i = 0; i < n_options; ++i)
    {
        if (i == highlight)
            attron(A_REVERSE);
        if (is_multiple_choice)
        {
            mvprintw(i + 5, 5, "%s%c. %s", selected_flags[i] ? "[*] " : "[ ] ",
                     labels[i],
                     options[i]); // 多选题显示选中标记
        }
        else
        {
            mvprintw(i + 5, 5, "%c. %s", labels[i],
                     options[i]); // 单选题不显示选中标记
        }
        attroff(A_REVERSE);
    }
}

/**
 * 获取用户选择
 * @param options 选项
 * @param n_options 选项数量
 * @param highlight 高亮
 * @param selected_flags 选中的选项标记
 * @param is_multiple_choice 是否为多选题
 * @return 返回用户选择的选项编号字符串
 */
char *get_user_choice(const char **options, int n_options, int highlight,
                      int *selected_flags, int is_multiple_choice)
{
    int choice;
    static char user_choice[256] = "";
    char labels[] = {'A', 'B', 'C', 'D'};
    user_choice[0] = '\0'; // 清空之前的选择
    while (1)
    {
        choice = getch();
        switch (choice)
        {
        case KEY_UP:
            highlight = (highlight - 1 + n_options) % n_options;
            break;
        case KEY_DOWN:
            highlight = (highlight + 1) % n_options;
            break;
        case ' ':
            if (is_multiple_choice)
            {
                selected_flags[highlight] =
                    !selected_flags[highlight]; // 切换选项标记
            }
            break;
        case 10: // Enter key
            if (is_multiple_choice)
            {
                for (int i = 0; i < n_options; ++i)
                {
                    if (selected_flags[i])
                    {
                        char temp[4];
                        sprintf(temp, "%c", labels[i]);
                        strcat(user_choice, temp);
                    }
                }
            }
            else
            {
                sprintf(user_choice, "%c", labels[highlight]);
            }
            return user_choice; // 返回用户选择的选项编号字符串
        }
        display_options(options, n_options, highlight, selected_flags,
                        is_multiple_choice);
    }
}

/**
 * 展示用户选择的选项
 * @param user_choice 用户选择的选项
 */
void display_user_choice(const char *user_choice)
{
    mvprintw(10, 5, "你的答案是: %s", user_choice);
}
/**
 * 展示答案
 * @param answer 答案
 */
void display_answer(const char *answer)
{
    mvprintw(11, 5, "正确答案是: %s", answer);
}

/**
 * 比较答案并计分
 * @param user_choice 用户答案
 * @param answer 正确答案
 */
bool compare_answers_and_score_tui(const char *user_choice, const char *answer)
{
    if(strcmp(answer, "对") == 0)
    {
        answer = "A";
    }else if(strcmp(answer, "错") == 0)
    {
        answer = "B";
    }
    return strcmp(user_choice, answer) == 0;
}
/**
 * 展示答题结果是否正确
 * @param is_correct 是否正确
 */
void display_result(bool is_correct)
{
    if (is_correct)
    {
        mvprintw(12, 5, "回答正确!");
    }
    else
    {
        mvprintw(12, 5, "回答错误!");
    }
}
/**
 * 处理问题
 * @param title 标题
 * @param options 选项
 * @param answer 答案
 * @param type 题目类型
 */
void process_question(const char *title, const char **options, int n_options,
                      const char *answer, const char *type)
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    display_question(title);

    char *user_choice;
    int selected_flags[4] = {0, 0, 0, 0}; // 初始化选项标记
    int is_multiple_choice = (strcmp(type, "多选题") == 0);

    // 根据题目类型调整显示
    if (strcmp(type, "判断题") == 0)
    {
        const char *judgment_options[2] = {"对", "错"};
        display_options(judgment_options, 2, 0, selected_flags,
                        is_multiple_choice);
        user_choice = get_user_choice(judgment_options, 2, 0, selected_flags,
                                      is_multiple_choice); // 获取用户选择
    }
    else
    {
        display_options(options, n_options, 0, selected_flags,
                        is_multiple_choice);
        user_choice = get_user_choice(options, n_options, 0, selected_flags,
                                      is_multiple_choice); // 获取用户选择
    }

    attron(COLOR_PAIR(2));
    display_user_choice(user_choice);
    // 展示答案
    display_answer(answer);
    // 比较答案并计分
    bool is_correct = compare_answers_and_score_tui(user_choice, answer);
    display_result(is_correct);
    attroff(COLOR_PAIR(2));
    getch(); // 等待另一个按键以退出
    clear(); // 清屏
}

int main()
{
    setlocale(LC_ALL, ""); // 设置locale

    // 选择题库
    char *selected_file = navigate_question_bank(QUESTION_BANK_PATH);
    if (selected_file == NULL)
    {
        fprintf(stderr, "题库选择失败\n");
        return 1;
    }

    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s/%s", QUESTION_BANK_PATH,
             selected_file);
    cJSON *json = load_question_bank(filepath);
    int count = input_count_page();
    cJSON *selected_questions = select_questions_randomly(count, json);
    const cJSON *questionItem = NULL;
    cJSON_ArrayForEach(questionItem, selected_questions)
    {
        const cJSON *title =
            cJSON_GetObjectItemCaseSensitive(questionItem, "title");
        const cJSON *options =
            cJSON_GetObjectItemCaseSensitive(questionItem, "options");
        const cJSON *answer =
            cJSON_GetObjectItemCaseSensitive(questionItem, "correctAnswer");
        const cJSON *type =
            cJSON_GetObjectItemCaseSensitive(questionItem, "type");
        const cJSON *optionItem;
        const char *optionsArray[4];
        int i = 0;

        cJSON_ArrayForEach(optionItem, options)
        {
            cJSON *optionValue = optionItem->child;
            optionsArray[i++] = optionValue->valuestring;
        }

        process_question(title->valuestring, optionsArray, i,
                         answer->valuestring, type->valuestring);
    }

    cJSON_Delete(json);
    return 0;
}
