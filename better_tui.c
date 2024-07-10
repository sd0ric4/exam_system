#include "student.h"
#include <cjson/cJSON.h>
#include <stdbool.h>
#include <dirent.h>
#include <locale.h>
#include <time.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DISPLAY 5 // 最多显示5个文件名

/**
 * 结构体
 * 用于存储题目的信息
 * 包括题目、选项、正确答案、题目类型、选项数量
 * @param title 题目
 * @param options 选项
 * @param correctAnswer 正确答案
 * @param type 题目类型
 * @param n_options 选项数量
 */
typedef struct
{
    char *title;
    char *options[4];
    char *correctAnswer;
    char *type;
    int n_options;
} Question;

void shuffle_array(cJSON **array, int n)
{
    //异常处理
    if (array == NULL || n <= 0)
    {
        fprintf(stderr, "打乱数组时出错\n");
    }

    if (n > 1)
    {
        srand(time(NULL)); // 初始化随机种子
        for (int i = 0; i < n - 1; i++)
        {
            int j = i + rand() / (RAND_MAX / (n - i) + 1);
            cJSON *temp = array[j];
            array[j] = array[i];
            array[i] = temp;
        }
    }
}

/**
 * 从文件中加载题库
 * @param filename 文件名
 * @return 题库
 */
cJSON *select_questions_randomly(int count, cJSON *questions)
{
    cJSON *selected_questions = cJSON_CreateArray();
    int n_questions = cJSON_GetArraySize(questions);
    cJSON **questions_array = (cJSON **)malloc(n_questions * sizeof(cJSON *)); // 创建问题指针数组

    // 异常处理
    if (questions_array == NULL)
    {
        fprintf(stderr, "分配内存时出错\n");
        exit(EXIT_FAILURE);
    }
    if(count > n_questions){
        count = n_questions;
    }
    // 填充问题指针数组
    for (int i = 0; i < n_questions; ++i)
    {
        questions_array[i] = cJSON_GetArrayItem(questions, i);
    }

    shuffle_array(questions_array, n_questions); // 打乱问题数组

    // 选择前 count 个问题
    for (int i = 0; i < count; ++i)
    {
        cJSON_AddItemToArray(selected_questions, cJSON_Duplicate(questions_array[i], 1));
    }

    free(questions_array); // 释放问题指针数组内存
    return selected_questions;
}

/**
 * 输入题量页面
 * @return 题量
 */
int input_count_page()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);

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
        else if (ch == 127 || ch == KEY_BACKSPACE) // 处理 ASCII 码 127 (DEL) 和键值 KEY_BACKSPACE
        {
            if (i > 0)
            {
                input[--i] = '\0';
                mvprintw(2, 5, "          "); // 清除之前的输出
                mvprintw(2, 5, "%s", input);  // 重新打印更新后的输入
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
 * 导航题库页面
 * @param path 题库路径
 * @return 选择的题库文件
 */
char *navigate_question_bank(const char *path)
{
    DIR *dir;
    struct dirent *ptr;
    char *files[256];
    int count = 0;
    int highlight = 0;
    int page = 0;
    int ch;
    static char selected_file[256];

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);

    dir = opendir(path);
    if (!dir)
    {
        mvprintw(1, 5, "无法打开题库目录: %s", path);
        getch();
        endwin();
        return NULL;
    }

    mvprintw(1, 5, "选择题库文件:");
    while ((ptr = readdir(dir)) != NULL)
    {
        if (ptr->d_type == 8) // 只处理普通文件
        {
            files[count] = strdup(ptr->d_name);
            count++;
        }
    }
    closedir(dir);

    while (1)
    {
        clear();
        mvprintw(1, 5, "选择题库文件:");
        int start = page * MAX_DISPLAY;
        int end = start + MAX_DISPLAY < count ? start + MAX_DISPLAY : count;
        for (int i = start; i < end; ++i)
        {
            if (i == highlight)
                attron(A_REVERSE);
            mvprintw(i - start + 2, 5, "%s", files[i]);
            if (i == highlight)
                attroff(A_REVERSE);
        }

        ch = getch();
        switch (ch)
        {
        case KEY_UP:
            if (highlight == start && page > 0)
            {
                page--;
                highlight -= MAX_DISPLAY;
            }
            else if (highlight > 0)
            {
                highlight--;
            }
            break;
        case KEY_DOWN:
            if (highlight == end - 1 && end < count)
            {
                page++;
                highlight += MAX_DISPLAY;
            }
            else if (highlight < count - 1)
            {
                highlight++;
            }
            break;
        case KEY_LEFT:
            if (page > 0)
            {
                page--;
                highlight -= MAX_DISPLAY;
            }
            break;
        case KEY_RIGHT:
            if ((page + 1) * MAX_DISPLAY < count)
            {
                page++;
                highlight += MAX_DISPLAY;
            }
            break;
        case 10: // Enter键
            strcpy(selected_file, files[highlight]);
            for (int i = 0; i < count; ++i)
            {
                free(files[i]);
            }
            clear();
            endwin();
            return selected_file;
        }
    }
}

/**
 * 从文件中加载题库
 * @param filename 文件名
 * @return 题库
 */
void display_question(const char *question)
{
    // 启用加粗属性
    attron(A_BOLD);
    mvprintw(2, 5, "%s", question);
    // 禁用加粗属性
    attroff(A_BOLD);
}

void display_options(const char **options, int n_options, int highlight, int *selected_flags, int is_multiple_choice)
{
    const char labels[] = {'A', 'B', 'C', 'D'};
    for (int i = 0; i < n_options; ++i)
    {
        if (i == highlight)
            attron(A_REVERSE);
        if (is_multiple_choice)
        {
            mvprintw(i + 5, 5, "%s%c. %s", selected_flags[i] ? "[*] " : "[ ] ", labels[i], options[i]);
        }
        else
        {
            mvprintw(i + 5, 5, "%c. %s", labels[i], options[i]);
        }
        attroff(A_REVERSE);
    }
}

/**
 * 获取用户选择
 * @param options 选项
 * @param n_options 选项数量
 * @param highlight 当前高亮选项
 * @param selected_flags 选中标志
 * @param is_multiple_choice 是否多选
 * @return 用户选择
 */
char *get_user_choice(const char **options, int n_options, int highlight, int *selected_flags, int is_multiple_choice)
{
    int choice;
    static char user_choice[256] = "";
    char labels[] = {'A', 'B', 'C', 'D'};
    user_choice[0] = '\0';
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
                selected_flags[highlight] = !selected_flags[highlight];
            }
            break;
        case 10:
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
            return user_choice;
        }
        display_options(options, n_options, highlight, selected_flags, is_multiple_choice);
    }
}

/**
 * 显示用户选择
 * @param user_choice 用户选择
 */
void display_user_choice(const char *user_choice,bool is_correct)
{
    start_color(); // 确保已经初始化颜色
    init_pair(1, COLOR_RED, COLOR_BLACK); // 初始化颜色对，红色文字，黑色背景
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // 初始化颜色对，绿色文字，黑色背景

    attron(A_BOLD); // 启用加粗属性
    mvprintw(10, 5, "你的答案是: ");
    attroff(A_BOLD); // 禁用加粗属性

    if(is_correct){
        attron(COLOR_PAIR(2)); // 启用绿色文字
        printw("%s", user_choice); // 继续在同一行打印 user_choice
        attroff(COLOR_PAIR(2)); // 禁用绿色文字
    }else{
        attron(COLOR_PAIR(1)); // 启用红色文字
        printw("%s", user_choice); // 继续在同一行打印 user_choice
        attroff(COLOR_PAIR(1)); // 禁用红色文字
    }
}

/**
 * 显示正确答案
 * @param answer 正确答案
 */
void display_answer(const char *answer)
{
    start_color(); // 初始化颜色
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // 初始化颜色对，绿色文字，黑色背景
    if (strcmp(answer, "对") == 0)
    {
        answer = "A";
    }
    else if (strcmp(answer, "错") == 0)
    {
        answer = "B";
    }
    attron(A_BOLD); // 启用加粗属性
    mvprintw(11, 5, "正确答案是: ");
    attroff(A_BOLD); // 禁用加粗属性
    attron(COLOR_PAIR(2)); // 启用绿色文字
    printw("%s", answer); // 继续在同一行打印 answer
    attroff(COLOR_PAIR(2)); // 禁用绿色文字
}

/**
 * 比较用户答案和正确答案
 * @param user_choice 用户选择
 * @param answer 正确答案
 * @return 是否正确
 */
bool compare_answers_tui(const char *user_choice, const char *answer)
{
    if (strcmp(answer, "对") == 0)
    {
        answer = "A";
    }
    else if (strcmp(answer, "错") == 0)
    {
        answer = "B";
    }
    return strcmp(user_choice, answer) == 0;
}

/**
 * 显示结果
 * @param is_correct 是否正确
 */
void display_result(bool is_correct)
{
    start_color(); // 初始化颜色
    init_pair(1, COLOR_RED, COLOR_BLACK); // 初始化颜色对，红色文字，黑色背景
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // 初始化颜色对，绿色文字，黑色背景
    if (is_correct)
    {
        attron(COLOR_PAIR(2)); // 启用绿色文字
        mvprintw(12, 5, "回答正确!");
        attroff(COLOR_PAIR(2)); // 禁用绿色文字，恢复默认颜色
    }
    else
    {
        attron(COLOR_PAIR(1)); // 启用红色文字
        mvprintw(12, 5, "回答错误!");
        attroff(COLOR_PAIR(1)); // 禁用红色文字，恢复默认颜色
    }
}

/**
 * 处理题目
 * @param question 题目
 */
void process_question(Question question)
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);

    display_question(question.title);

    char *user_choice;
    int selected_flags[4] = {0, 0, 0, 0};
    int is_multiple_choice = (strcmp(question.type, "多选题") == 0);

    if (strcmp(question.type, "判断题") == 0)
    {
        const char *judgment_options[2] = {"对", "错"};
        display_options(judgment_options, 2, 0, selected_flags, is_multiple_choice);
        user_choice = get_user_choice(judgment_options, 2, 0, selected_flags, is_multiple_choice);
    }
    else
    {
        display_options((const char **)question.options, question.n_options, 0, selected_flags, is_multiple_choice);
        user_choice = get_user_choice((const char **)question.options, question.n_options, 0, selected_flags, is_multiple_choice);
    }

    bool is_correct = compare_answers_tui(user_choice, question.correctAnswer);
    display_user_choice(user_choice,is_correct);
    display_answer(question.correctAnswer);
    display_result(is_correct);
    getch();
    clear();
}

/**
 * 从文件中加载题库
 * @param filename 文件名
 * @return 题库
 */
Question parse_question_from_cjson(const cJSON *questionItem)
{
    Question question;
    const cJSON *title = cJSON_GetObjectItem(questionItem, "title");
    const cJSON *options = cJSON_GetObjectItem(questionItem, "options");
    const cJSON *answer = cJSON_GetObjectItem(questionItem, "correctAnswer");
    const cJSON *type = cJSON_GetObjectItem(questionItem, "type");
    const cJSON *optionItem;

    const char *optionsArray[4];
    int i = 0;

    // 确保每个字段都被正确解析
    if (!cJSON_IsString(title) || !cJSON_IsArray(options) || !cJSON_IsString(answer) || !cJSON_IsString(type))
    {
        fprintf(stderr, "解析题目时出错\n");
        exit(EXIT_FAILURE);
    }

    question.title = strdup(title->valuestring);
    question.correctAnswer = strdup(answer->valuestring);
    question.type = strdup(type->valuestring);
    question.n_options = cJSON_GetArraySize(options);

    cJSON_ArrayForEach(optionItem, options)
    {
        cJSON *optionValue = optionItem->child;
        optionsArray[i++] = optionValue->valuestring;
        question.options[i - 1] = strdup(optionValue->valuestring);
    }

    return question;
}

void free_question(Question *question)
{
    free(question->title);
    free(question->correctAnswer);
    free(question->type);
    for (int i = 0; i < question->n_options; ++i)
    {
        free(question->options[i]);
    }
}

int main()
{
    setlocale(LC_ALL, "");

    char *selected_file = navigate_question_bank(QUESTION_BANK_PATH);
    if (selected_file == NULL)
    {
        fprintf(stderr, "题库选择失败\n");
        return 1;
    }

    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s/%s", QUESTION_BANK_PATH, selected_file);
    cJSON *json = load_question_bank(filepath);
    int count = input_count_page();
    cJSON *selected_questions = select_questions_randomly(count, json);
    const cJSON *questionItem = NULL;

    cJSON_ArrayForEach(questionItem, selected_questions)
    {
        Question question = parse_question_from_cjson(questionItem);
        process_question(question);
        free_question(&question);
    }

    cJSON_Delete(json);
    return 0;
}
