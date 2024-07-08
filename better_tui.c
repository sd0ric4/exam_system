#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h> // 包含locale.h头文件
#include "student.h"

/**
 * 展示问题
 * @param question 问题
 */
void display_question(const char *question) {
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
void display_options(const char **options, int n_options, int highlight, int *selected_flags, int is_multiple_choice) {
    const char labels[] = {'A', 'B', 'C', 'D'}; // 定义ABCD标签
    for (int i = 0; i < n_options; ++i) {
        if (i == highlight)
            attron(A_REVERSE);
        if (is_multiple_choice) {
            mvprintw(i + 5, 5, "%c. %s%s", labels[i], selected_flags[i] ? "[*] " : "[ ] ", options[i]); // 多选题显示选中标记
        } else {
            mvprintw(i + 5, 5, "%c. %s", labels[i], options[i]); // 单选题不显示选中标记
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
 */
void get_user_choice(const char **options, int n_options, int highlight, int *selected_flags, int is_multiple_choice) {
    int choice;
    while (1) {
        choice = getch();
        switch (choice) {
            case KEY_UP:
                highlight = (highlight - 1 + n_options) % n_options;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % n_options;
                break;
            case ' ':
                if (is_multiple_choice) {
                    selected_flags[highlight] = !selected_flags[highlight]; // 切换选项标记
                }
                break;
            case 10: // Enter key
                return; // 用户按下Enter键结束选择
        }
        display_options(options, n_options, highlight, selected_flags, is_multiple_choice);
    }
}

/**
 * 展示答案
 * @param answer 答案
 */
void display_answer(const char *answer) {
    mvprintw(10, 5, "The correct answer is: %s", answer);
}

/**
 * 处理问题
 * @param title 标题
 * @param options 选项
 * @param answer 答案
 * @param type 题目类型
 */
void process_question(const char *title, const char **options, int n_options, const char *answer, const char *type) {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    display_question(title);
    
    int selected_flags[4] = {0, 0, 0, 0}; // 初始化选项标记
    int is_multiple_choice = (strcmp(type, "多选题") == 0);

    // 根据题目类型调整显示
    if (strcmp(type, "判断题") == 0) {
        const char *judgment_options[2] = {"对", "错"};
        display_options(judgment_options, 2, 0, selected_flags, is_multiple_choice);
        get_user_choice(judgment_options, 2, 0, selected_flags, is_multiple_choice); // 获取用户选择
    } else {
        display_options(options, n_options, 0, selected_flags, is_multiple_choice);
        get_user_choice(options, n_options, 0, selected_flags, is_multiple_choice); // 获取用户选择
    }

    attron(COLOR_PAIR(2));
    // 展示答案
    display_answer(answer);
    attroff(COLOR_PAIR(2));
    getch(); // 等待另一个按键以退出
    clear(); // 清屏

    endwin();
}

int main() {
    setlocale(LC_ALL, ""); // 设置locale

    cJSON *json = load_question_bank("./QuestionBank/ch7.json");

    const cJSON *questionItem = NULL;
    cJSON_ArrayForEach(questionItem, json) {
        const cJSON *title = cJSON_GetObjectItemCaseSensitive(questionItem, "title");
        const cJSON *options = cJSON_GetObjectItemCaseSensitive(questionItem, "options");
        const cJSON *answer = cJSON_GetObjectItemCaseSensitive(questionItem, "correctAnswer");
        const cJSON *type = cJSON_GetObjectItemCaseSensitive(questionItem, "type");
        const cJSON *optionItem;
        const char *optionsArray[4];
        int i = 0;

        cJSON_ArrayForEach(optionItem, options) {
            cJSON *optionValue = optionItem->child;
            optionsArray[i++] = optionValue->valuestring;
        }

        process_question(title->valuestring, optionsArray, i, answer->valuestring, type->valuestring);
    }

    cJSON_Delete(json);
    return 0;
}
