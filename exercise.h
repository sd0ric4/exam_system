#ifndef EXERCISE_H
#define EXERCISE_H

#include <cjson/cJSON.h>

#define UpperLimit 100 //试题题库默认题目上限

typedef struct 试题 {
    char chapter[32]; // 章节
    char question[256]; // 题干
    char choices[4][128]; // 四个选项
    char answer; // 标准答案，A/B/C/D
    int AMOUNT; // 每个单选题五分
    char type[10]; // 题型
} Exercise;

typedef struct 试题题库 {
    char ExerciseName[32];
    char CreatorName[16];
    Exercise SetOfExercise[UpperLimit];
    int ExerciseSum; // 当前题目总数
} ExerciseDataBase;

// 函数声明
void initialDB(ExerciseDataBase db);
void addExercise(ExerciseDataBase *db, Exercise exercise);
void deleteExercise(ExerciseDataBase *db, int index);
void modifyExercise(ExerciseDataBase *db, int index, Exercise exercise);
void saveExercises(ExerciseDataBase db);
void queryExercise(const ExerciseDataBase *db, int index);
void browseExercises(const ExerciseDataBase *db);
void generateExercise(ExerciseDataBase *db, int numQuestions);
void generateJsonFile(const ExerciseDataBase *db, const char *filename);

#endif // EXERCISE_H
