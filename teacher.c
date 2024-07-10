#include "exercise.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <locale.h>

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");

    ExerciseDataBase db = {0}; // 初始化数据库
    strcpy(db.ExerciseName, "ExampleDB");
    strcpy(db.CreatorName, "Creator");

    int opt;
    int backend_mode = 0; // 0 for TUI, 1 for backend

    while ((opt = getopt(argc, argv, "bt")) != -1) {
        switch (opt) {
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

    if (backend_mode) {
        printf("Backend mode not implemented in this version.\n");
    } else {
        exerciseMenu();
        int choice;
        while (1) {
            printf("请选择操作：");
            scanf("%d", &choice);
            if (choice == 7) break;

            int index;
            Exercise exercise;
            switch (choice) {
                case 1:
                    printf("输入章节：");
                    scanf(" %[^\n]", exercise.chapter);
                    printf("输入题型（单选题/多选题/判断题）：");
                    scanf(" %[^\n]", exercise.type);
                    printf("输入题干：");
                    scanf(" %[^\n]", exercise.question);
                    for (int i = 0; i < 4; i++) {
                        printf("输入选项 %c：", 'A' + i);
                        scanf(" %[^\n]", exercise.choices[i]);
                    }
                    printf("输入标准答案（A/B/C/D）：");
                    scanf(" %c", &exercise.answer);
                    addExercise(&db, exercise);
                    break;
                case 2:
                    printf("输入要删除的题号：");
                    scanf("%d", &index);
                    deleteExercise(&db, index - 1);
                    break;
                case 3:
                    printf("输入要修改的题号：");
                    scanf("%d", &index);
                    printf("输入章节：");
                    scanf(" %[^\n]", exercise.chapter);
                    printf("输入题干：");
                    scanf(" %[^\n]", exercise.question);
                    for (int i = 0; i < 4; i++) {
                        printf("输入选项 %c：", 'A' + i);
                        scanf(" %[^\n]", exercise.choices[i]);
                    }
                    printf("输入标准答案（A/B/C/D）：");
                    scanf(" %c", &exercise.answer);
                    modifyExercise(&db, index - 1, exercise);
                    break;
                case 4:
                    printf("输入要查询的题号：");
                    scanf("%d", &index);
                    queryExercise(&db, index - 1);
                    break;
                case 5:
                    browseExercises(&db);
                    break;
                case 6:
                    printf("输入生成试卷的题目数：");
                    int numQuestions;
                    scanf("%d", &numQuestions);
                    generateExercise(&db, numQuestions);
                    break;
                default:
                    printf("无效选择\n");
            }

            saveExercises(db);
        }
    }

    return 0;
}
