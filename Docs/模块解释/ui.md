# ui

## 头文件

```c
#ifndef UI_H
#define UI_H

#include "ncurses.h"
#include <stdbool.h>
#include "question.h"

int input_count_page();
char *navigate_question_bank(const char *path);
void display_score(double score);
void display_question_count(int question_count);
void display_correct_count(int correct_count);
void display_question(const char *question);
void display_final_result(double score, int question_count, int correct_count);
void display_options(const char **options, int n_options, int highlight, int *selected_flags, int is_multiple_choice);
char *get_user_choice(const char **options, int n_options, int highlight, int *selected_flags, int is_multiple_choice);
void display_user_choice(const char *user_choice, bool is_correct);
void display_answer(const char *answer);
bool compare_answers_tui(const char *user_choice, const char *answer);
void display_result(bool is_correct);
void process_question(Question question);
void studentMenu();
void teacherMenu();
void mainMenu();
void exerciseMenu();

#endif // UI_H
```

#### 头文件说明

头文件 `ui.h` 定义了一些函数，用于实现用户界面。

## 源文件

[`ui.c`](../../src/ui.c)

### 函数定义

#### input_count_page
```c
/**
 * @brief 输入题量
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
        else if (ch == 127 || ch == KEY_BACKSPACE)
        {
            if (i > 0)
            {
                input[--i] = '\0';
                mvprintw(2, 5, "          ");
                mvprintw(2, 5, "%s", input);
            }
        }
        else if (ch == 10)
        {
            break;
        }
    }
    int count = atoi(input);
    question_count = count;
    clear();
    return count;
}
```

#### 函数说明

函数 `input_count_page` 用于输入题量并返回输入的题量。

#### 函数算法实现解析

1. 初始化 ncurses 界面。
2. 提示用户输入题量。
3. 获取用户输入的数字字符并显示在屏幕上。
4. 按回车键结束输入并返回题量。

---

#### navigate_question_bank
```c
/**
 * @brief 浏览题库
 * @param path 题库路径
 * @return 选择的题库文件名
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
        if (ptr->d_type == 8)
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
        case 10:
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
```

#### 函数说明

函数 `navigate_question_bank` 用于浏览题库并选择一个题库文件。

#### 函数算法实现解析

1. 打开指定路径的目录，读取目录中的文件名。
2. 使用 ncurses 显示文件列表，允许用户使用箭头键导航。
3. 用户选择文件后，返回所选文件名。

---

#### display_question
```c
/**
 * @brief 显示题目
 * @param question 题目
 */
void display_question(const char *question)
{
    attron(A_BOLD);
    mvprintw(2, 5, "%s", question);
    attroff(A_BOLD);
}
```

#### 函数说明

函数 `display_question` 用于显示题目。

#### 函数算法实现解析

1. 使用 ncurses 在屏幕上显示题目，带有加粗效果。

---

#### display_score
```c
/**
 * @brief 显示得分
 * @param score 得分
 */
void display_score(double score)
{
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    attron(COLOR_PAIR(4));
    mvprintw(13, 5, "你的得分是: %.2f", score);
    attroff(COLOR_PAIR(4));
}
```

#### 函数说明

函数 `display_score` 用于显示得分。

#### 函数算法实现解析

1. 使用 ncurses 显示得分，带有黄色字体效果。

---

#### display_question_count
```c
/**
 * @brief 显示题目总数
 * @param question_count 题目总数
 */
void display_question_count(int question_count)
{
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    attron(COLOR_PAIR(5));
    mvprintw(14, 5, "题目总数: %d", question_count);
    attroff(COLOR_PAIR(5));
}
```

#### 函数说明

函数 `display_question_count` 用于显示题目总数。

#### 函数算法实现解析

1. 使用 ncurses 显示题目总数，带有品红色字体效果。

---

#### display_correct_count
```c
/**
 * @brief 显示正确答题数
 * @param correct_count 正确答题数
 */
void display_correct_count(int correct_count)
{
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(2));
    mvprintw(15, 5, "正确答题数: %d", correct_count);
    attroff(COLOR_PAIR(2));
}
```

#### 函数说明

函数 `display_correct_count` 用于显示正确答题数。

#### 函数算法实现解析

1. 使用 ncurses 显示正确答题数，带有绿色字体效果。

---

#### display_final_result
```c
/**
 * @brief 显示最终结果
 * @param score 得分
 * @param question_count 题目总数
 * @param correct_count 正确答题数
 */
void display_final_result(double score, int question_count, int correct_count)
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);

    display_score(score);
    display_question_count(question_count);
    display_correct_count(correct_count);

    getch();
    clear();
}
```

#### 函数说明

函数 `display_final_result` 用于显示最终结果，包括得分、题目总数和正确答题数。

#### 函数算法实现解析

1. 初始化 ncurses 界面。
2. 显示得分、题目总数和正确答题数。
3. 等待用户按键后清屏。

---

#### display_options
```c
/**
 * @brief 显示选项
 * @param options 选项
 * @param n_options 选项数量
 * @param highlight 当前高亮选项
 * @param selected_flags 选中标志
 * @param is_multiple_choice 是否多选
 */
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
            mvprintw(i + 5, 5, "%c.

 %s", labels[i], options[i]);
        }
        attroff(A_REVERSE);
    }
}
```

#### 函数说明

函数 `display_options` 用于显示选项，支持高亮显示和多选。

#### 函数算法实现解析

1. 遍历选项，根据当前高亮选项和选中标志显示选项。
2. 支持单选和多选题型。

---

#### get_user_choice
```c
/**
 * @brief 获取用户选择
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
```

#### 函数说明

函数 `get_user_choice` 用于获取用户选择的答案，支持高亮显示和多选。

#### 函数算法实现解析

1. 根据用户按键更新高亮选项和选中标志。
2. 用户按回车键后返回选择的答案。

---

#### display_user_choice
```c
/**
 * @brief 显示用户选择
 * @param user_choice 用户选择
 * @param is_correct 是否正确
 */
void display_user_choice(const char *user_choice, bool is_correct)
{
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    attron(A_BOLD);
    mvprintw(10, 5, "你的答案是: ");
    attroff(A_BOLD);

    if (is_correct)
    {
        attron(COLOR_PAIR(2));
        printw("%s", user_choice);
        attroff(COLOR_PAIR(2));
    }
    else
    {
        attron(COLOR_PAIR(1));
        printw("%s", user_choice);
        attroff(COLOR_PAIR(1));
    }
}
```

#### 函数说明

函数 `display_user_choice` 用于显示用户选择的答案，并指示答案是否正确。

#### 函数算法实现解析

1. 使用 ncurses 显示用户选择的答案。
2. 根据答案是否正确，使用不同颜色显示。

---

#### display_answer
```c
/**
 * @brief 显示答案
 * @param answer 正确答案
 */
void display_answer(const char *answer)
{
    start_color();
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    if (strcmp(answer, "对") == 0)
    {
        answer = "A";
    }
    else if (strcmp(answer, "错") == 0)
    {
        answer = "B";
    }
    attron(A_BOLD);
    mvprintw(11, 5, "正确答案是: ");
    attroff(A_BOLD);
    attron(COLOR_PAIR(2));
    printw("%s", answer);
    attroff(COLOR_PAIR(2));
}
```

#### 函数说明

函数 `display_answer` 用于显示正确答案。

#### 函数算法实现解析

1. 使用 ncurses 显示正确答案。
2. 判断答案为 "对" 或 "错" 时，将其转换为对应的字母 "A" 或 "B"。

---

#### compare_answers_tui
```c
/**
 * @brief 比较答案
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
```

#### 函数说明

函数 `compare_answers_tui` 用于比较用户选择的答案和正确答案，判断是否正确。

#### 函数算法实现解析

1. 判断答案为 "对" 或 "错" 时，将其转换为对应的字母 "A" 或 "B"。
2. 比较用户选择的答案和正确答案，返回比较结果。

---

#### display_result
```c
/**
 * @brief 显示结果
 * @param is_correct 是否正确
 */
void display_result(bool is_correct)
{
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    if (is_correct)
    {
        attron(COLOR_PAIR(2));
        mvprintw(12, 5, "回答正确!");
        attroff(COLOR_PAIR(2));
    }
    else
    {
        attron(COLOR_PAIR(1));
        mvprintw(12, 5, "回答错误!");
        attroff(COLOR_PAIR(1));
    }
}
```

#### 函数说明

函数 `display_result` 用于显示回答结果。

#### 函数算法实现解析

1. 使用 ncurses 显示回答结果。
2. 根据答案是否正确，使用不同颜色显示。

---

#### process_question
```c
/**
 * @brief 处理问题
 * @param question 问题
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
    if (is_correct)
    {
        correct_count++;
    }
    display_user_choice(user_choice, is_correct);
    display_answer(question.correctAnswer);
    display_result(is_correct);
    getch();
    clear();
}
```

#### 函数说明

函数 `process_question` 用于处理一个问题的显示和用户交互。

#### 函数算法实现解析

1. 初始化 ncurses 界面。
2. 显示问题标题和选项。
3. 获取用户选择的答案。
4. 判断答案是否正确，并更新正确答题数。
5. 显示用户选择的答案、正确答案和回答结果。
6. 等待用户按键后清屏。

---

#### studentMenu
```c
/**
 * @brief 学生菜单
 */
void studentMenu()
{
    printf("\n\n");
    printf("\t\t\t=======================考试系统=======================\n");
    printf("\t\t\t*                                                    *\n");
    printf("\t\t\t*                  1>. 答题                          *\n");
    printf("\t\t\t*                  2>. 修改密码                      *\n");
    printf("\t\t\t*                  0>. 退出                         *\n");
    printf("\t\t\t*                                     欢迎使用本系统!*\n");
    printf("\t\t\t=====================================================\n");
    printf("\t\t\t输入选项，按回车进入选项:                              \n");
}
```

#### 函数说明

函数 `studentMenu` 用于显示学生菜单。

#### 函数算法实现解析

1. 显示学生菜单，包括答题、修改密码和退出选项。

---

#### teacherMenu
```c
/**
 * @brief 教师菜单
 */
void teacherMenu()


{
    printf("\n\n");
    printf("\t\t\t=======================考试系统======================\n");
    printf("\t\t\t*                                                    *\n");
    printf("\t\t\t*                 1>. 录入考生信息                   *\n");
    printf("\t\t\t*                 2>. 全部考生信息                   *\n");
    printf("\t\t\t*                 3>. 查看考生信息                   *\n");
    printf("\t\t\t*                 4>. 修改考生信息                   *\n");
    printf("\t\t\t*                 5>. 删除考生信息                   *\n");
    printf("\t\t\t*                 6>. 插入考生信息                   *\n");
    printf("\t\t\t*                 7>. 保存考生信息                   *\n");
    printf("\t\t\t*                 0>. 退出管理系统                   *\n");
    printf("\t\t\t*                                     欢迎使用本系统!*\n");
    printf("\t\t\t=====================================================\n");
    printf("\t\t\t输入选项，按回车进入选项:                             \n");
}
```

#### 函数说明

函数 `teacherMenu` 用于显示教师菜单。

#### 函数算法实现解析

1. 显示教师菜单，包括录入、查看、修改、删除、插入和保存考生信息选项。

---

#### mainMenu
```c
/**
 * @brief 主菜单
 */
void mainMenu()
{
    printf("\n\n");
    printf("\t\t\t=======================考试系统======================\n");
    printf("\t\t\t*                                                   *\n");
    printf("\t\t\t*                  1>. 管理员登陆                    *\n");
    printf("\t\t\t*                  2>. 学生登陆                      *\n");
    printf("\t\t\t*                  0>. 退出                         *\n");
    printf("\t\t\t*                                     欢迎使用本系统!*\n");
    printf("\t\t\t=====================================================\n");
    printf("\t\t\t输入选项，按回车进入选项:                             \n");
}
```

#### 函数说明

函数 `mainMenu` 用于显示主菜单。

#### 函数算法实现解析

1. 显示主菜单，包括管理员登录、学生登录和退出选项。

---

#### exerciseMenu
```c
/**
 * @brief 试题菜单
 */
void exerciseMenu()
{
    printf("\n\n");
    printf("\t\t\t=======================题库管理系统======================\n");
    printf("\t\t\t*                                                      *\n");
    printf("\t\t\t*                  1>. 添加试题                         *\n");
    printf("\t\t\t*                  2>. 删除试题                         *\n");
    printf("\t\t\t*                  3>. 修改试题                         *\n");
    printf("\t\t\t*                  4>. 查询试题                         *\n");
    printf("\t\t\t*                  5>. 浏览所有试题                     *\n");
    printf("\t\t\t*                  6>. 生成试卷                         *\n");
    printf("\t\t\t*                  7>. 退出                             *\n");
    printf("\t\t\t*                                     欢迎使用本系统!  *\n");
    printf("\t\t\t========================================================\n");
    printf("\t\t\t输入选项，按回车进入选项:                                \n");
}
```

#### 函数说明

函数 `exerciseMenu` 用于显示试题菜单。

#### 函数算法实现解析

1. 显示试题菜单，包括添加、删除、修改、查询、浏览和生成试卷选项。