#ifndef UI_H
#define UI_H

#include <ncurses.h>
#include <stdbool.h>
#include "question.h"

int input_count_page();
char *navigate_question_bank(const char *path);
void display_question(const char *question);
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
