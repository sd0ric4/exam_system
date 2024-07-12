#ifndef MANAGE_H
#define MANAGE_H

#include "cJSON.h"
#include <stdbool.h>

// 定义一个考生结构体
typedef struct student {
    char name[40];
    char no[18];
    int xb;
    int age;
    char sch_num[20];
    char password[20];
    struct student *next;
    int countStuents;
    int countAnswerTimes;
    int countRightTimes;
} STU;

// 函数声明
STU *ListInit(STU *L);
STU *ListCreate(STU *L, int n);
void ListInsert(STU *L);
void ListDelete(STU *L);
void ListSearch(STU *L);
bool Input(STU *p, int i, STU *L);
void Output(STU *L);
void studentmodel(cJSON *root, STU *L);
void teachermodel(cJSON *root, STU *L);
void printstu(STU *p);
void printHeader();
void ListModify(STU *L);
void changepassword(STU *p);
void add_student_to_json(cJSON *root, STU *stu);
void read_student_from_json(cJSON *root, int rootNum, STU *L);
cJSON *saveRoot(cJSON *root, STU *L);
void save_students_to_file(cJSON *root);
cJSON *read_students_from_file(char *filename);
bool stuExist(STU *L, char *sch_num);
void disableEcho(char *prompt, char *password);

// 定义宏
#define ADMIN_NAME "admin"
#define ADMIN_PASSWORD "123456"
#endif
