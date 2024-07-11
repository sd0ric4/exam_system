# manage

## 头文件

```c
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
void ListModify(STU *L);
void changepassword(STU *p);
void add_student_to_json(cJSON *root, STU *stu);
void read_student_from_json(cJSON *root, int rootNum, STU *L);
cJSON *saveRoot(cJSON *root, STU *L);
void save_students_to_file(cJSON *root);
cJSON *read_students_from_file(char *filename);
bool stuExist(STU *L, char *sch_num);

#endif
```

#### 头文件说明

头文件 `manage.h` 定义了一些函数,结构体，用于学生管理端的功能实现。

#### 结构体说明

`STU` 结构体:
| 成员 | 类型 | 说明 |
| --- | --- | --- |
| name | char[40] | 学生姓名 |
| no | char[18] | 学号 |
| xb | int | 性别 |
| age | int | 年龄 |
| sch_num | char[20] | 学校编号 |
| password | char[20] | 密码 |
| next | struct student * | 下一个学生 |
| countStuents | int | 学生数量 |

## 源文件
[`manage.c`](../../src/manage.c)

### 函数定义

#### ListInit
```c
/**
 * @brief 创建表头，初始化链表
 * @param L 链表头指针
 * @return STU* 返回链表头指针
 */
STU *ListInit(STU *L) {
    STU *head = (STU *)malloc(sizeof(STU));
    head->next = NULL;
    L = head;
    return L;
}
```

#### 函数说明

函数 `ListInit` 用于创建链表的表头并初始化链表。

#### 函数算法实现解析

1. 分配一个 `STU` 结构体作为链表头节点。
2. 初始化头节点的 `next` 指针为 `NULL`。
3. 返回初始化后的链表头指针。

---

#### add_student_to_json
```c
/**
 * @brief 添加学生信息到JSON对象
 * @param root JSON根节点
 * @param stu 学生信息
 */
void add_student_to_json(cJSON *root, STU *stu) {
    cJSON *student = cJSON_CreateObject();
    cJSON_AddStringToObject(student, "name", stu->name);
    cJSON_AddStringToObject(student, "no", stu->no);
    cJSON_AddNumberToObject(student, "xb", stu->xb);
    cJSON_AddNumberToObject(student, "age", stu->age);
    cJSON_AddStringToObject(student, "sch_num", stu->sch_num);
    cJSON_AddStringToObject(student, "password", stu->password);
    cJSON_AddNumberToObject(student, "countStuents", stu->countStuents);
    cJSON_AddItemToArray(root, student);
}
```

#### 函数说明

函数 `add_student_to_json` 用于将学生信息添加到JSON对象中。

#### 函数算法实现解析

1. 创建一个新的JSON对象 `student`。
2. 将学生信息添加到 `student` 对象中。
3. 将 `student` 对象添加到 `root` JSON数组中。

---

#### read_student_from_json
```c
/**
 * @brief 从JSON对象中读取学生信息
 * @param root JSON根节点
 * @param rootNum JSON对象数量
 * @param L 链表头指针
 */
void read_student_from_json(cJSON *root, int rootNum, STU *L) {
    for (int i = 0; i < rootNum; i++) {
        STU *stu = (STU *)malloc(sizeof(STU));
        cJSON *item = cJSON_GetArrayItem(root, i);
        strcpy(stu->name, cJSON_GetObjectItem(item, "name")->valuestring);
        strcpy(stu->no, cJSON_GetObjectItem(item, "no")->valuestring);
        stu->xb = cJSON_GetObjectItem(item, "xb")->valueint;
        stu->age = cJSON_GetObjectItem(item, "age")->valueint;
        strcpy(stu->sch_num, cJSON_GetObjectItem(item, "sch_num")->valuestring);
        strcpy(stu->password, cJSON_GetObjectItem(item, "password")->valuestring);
        stu->countStuents = cJSON_GetObjectItem(item, "countStuents")->valueint;
        stu->next = L->next;
        L->next = stu;
    }
}
```

#### 函数说明

函数 `read_student_from_json` 用于从JSON对象中读取学生信息并插入到链表中。

#### 函数算法实现解析

1. 遍历JSON数组，逐个读取学生信息。
2. 为每个学生分配内存并复制信息。
3. 将学生节点插入到链表头部。

---

#### saveRoot
```c
/**
 * @brief 保存链表到JSON对象
 * @param root JSON根节点
 * @param L 链表头指针
 * @return cJSON* 返回更新后的JSON根节点
 */
cJSON *saveRoot(cJSON *root, STU *L) {
    STU *p = L->next;
    cJSON_Delete(root);
    root = cJSON_CreateArray();
    while (p != NULL) {
        add_student_to_json(root, p);
        p = p->next;
    }
    return root;
}
```

#### 函数说明

函数 `saveRoot` 用于将链表中的学生信息保存到JSON对象中。

#### 函数算法实现解析

1. 删除原有的JSON根节点。
2. 创建新的JSON数组。
3. 遍历链表，将每个学生信息添加到JSON数组中。
4. 返回更新后的JSON根节点。

---

#### save_students_to_file
```c
/**
 * @brief 保存学生信息到文件
 * @param root JSON根节点
 */
void save_students_to_file(cJSON *root) {
    FILE *file = fopen("student.json", "w");
    if (!file) {
        perror("Error opening file");
        return;
    }
    char *json_string = cJSON_Print(root);
    fprintf(file, "%s", json_string);
    free(json_string);
    fclose(file);
}
```

#### 函数说明

函数 `save_students_to_file` 用于将JSON对象中的学生信息保存到文件中。

#### 函数算法实现解析

1. 打开文件，如果打开失败则输出错误信息并返回。
2. 将JSON对象转换为字符串。
3. 将字符串写入文件。
4. 关闭文件。

---

#### read_students_from_file
```c
/**
 * @brief 从文件中读取学生信息
 * @param filename 文件名
 * @return cJSON* 返回JSON根节点
 */
cJSON *read_students_from_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(size + 1);
    fread(buffer, size, 1, file);
    buffer[size] = '\0';

    cJSON *root = cJSON_Parse(buffer);
    free(buffer);
    fclose(file);
    return root;
}
```

#### 函数说明

函数 `read_students_from_file` 用于从文件中读取学生信息并返回JSON对象。

#### 函数算法实现解析

1. 打开文件，如果打开失败则返回 `NULL`。
2. 读取文件内容到缓冲区。
3. 解析缓冲区内容为JSON对象。
4. 释放缓冲区并关闭文件。
5. 返回解析后的JSON对象。

---

#### stuExist
```c
/**
 * @brief 判断学生是否存在
 * @param L 链表头指针
 * @param sch_num 学号
 * @return bool 学生是否存在
 */
bool stuExist(STU *L, char *sch_num) {
    STU *p = L->next;
    while (p != NULL) {
        if (strcmp(p->sch_num, sch_num) == 0) {
            return true;
        }
        p = p->next;
    }
    return false;
}
```

#### 函数说明

函数 `stuExist` 用于判断链表中是否存在指定学号的学生。

#### 函数算法实现解析

1. 遍历链表。
2. 比较每个节点的学号与指定学号，如果相同则返回 `true`。
3. 如果遍历结束仍未找到，则返回 `false`。

---

#### ListCreate
```c
/**
 * @brief 创建链表，将新生成的节点插入到链表的表头
 * @param L 链表头指针
 * @param n 学生数量
 * @return STU* 返回链表头指针
 */
STU *ListCreate(STU *L, int n) {
    int i;
    for (i = 0; i < n; i++) {
        STU *p = (STU *)malloc(sizeof(STU));
        if (Input(p, i, L) == false) {
            i--;
        } else {
            p->next = L->next;
            L->next = p;
        }
    }
    return L;
}
```

#### 函数说明

函数 `ListCreate` 用于创建链表并将新生成的节点插入到链表头部。

#### 函数算法实现解析

1. 遍历指定数量的学生。
2. 为每个学生分配内存并调用 `Input` 函数输入学生信息。
3. 将新节点插入到链表头部。
4. 返回链表头指针。

---

#### ListInsert
```c
/**
 * @brief 对链表进行节点的插入操作
 * @param L 链表头指针
 */
void ListInsert(STU *L) {
    STU *s = (STU *)malloc(sizeof(STU));
    printf("请输入您要插入的考生的学号:");
    scanf("%s", s->sch_num);
    if (stuExist(L, s->sch_num)) {
        printf("该考生已存在，无法插入！\n");
        free(s);
        return;
    }
    printf("请输入您要插入的考生的姓名:");
    scanf("%s", s->name);
    printf("请输入您要插入的考生的身份证号:");
    scanf("%s", s->no);


    printf("请输入您要插入的考生的性别（女性为0，男性为1）:");
    scanf("%d", &s->xb);
    printf("请输入您要插入的考生的年龄:");
    scanf("%d", &s->age);
    s->countStuents = 1;
    s->password[0] = '\0';
    s->next = L->next;
    L->next = s;
}
```

#### 函数说明

函数 `ListInsert` 用于在链表中插入新节点。

#### 函数算法实现解析

1. 为新节点分配内存并输入信息。
2. 判断学号是否已存在，如果存在则输出提示信息并释放内存。
3. 如果不存在，则将新节点插入到链表头部。

---

#### ListDelete
```c
/**
 * @brief 对链表进行节点的删除操作
 * @param L 链表头指针
 */
void ListDelete(STU *L) {
    char n[40];
    char choose;
    STU *p = L->next, *pre = L;
    if (p == NULL)
        printf("数据为空，无法删除！");
    else {
        printf("请输入您要删除的考生的学号:");
        scanf("%s", n);
        while (strcmp(p->sch_num, n) != 0) {
            pre = p;
            p = pre->next;
            if (p == NULL) {
                printf("没有找到相关信息，无法删除\n");
                return;
            }
        }
        printf("您要删除的考生信息如下：\n");
        printstu(p);
        printf("\n是否确认删除该考生信息？(Y/N)\n");
        getchar();
        scanf("%c", &choose);
        if (choose == 'Y' || choose == 'y') {
            pre->next = p->next;
            free(p);
            printf("删除成功!");
        } else {
            printf("已取消删除");
        }
    }
}
```

#### 函数说明

函数 `ListDelete` 用于删除链表中的指定节点。

#### 函数算法实现解析

1. 输入要删除的学生学号。
2. 遍历链表查找对应节点。
3. 如果找到，输出学生信息并确认删除。
4. 如果确认删除，则将节点从链表中移除并释放内存。

---

#### printHeader
```c
/**
 * @brief 打印表头
 */
void printHeader() {
    printf("| %-39s | %-17s | %-6s | %-4s | %-19s | %-12s | %-19s |\n", 
           "name", "identityCard", "gender", "age", "studentNumber", "answerTimes", "password");
}
```

#### 函数说明

函数 `printHeader` 用于打印表头信息。

#### 函数算法实现解析

1. 打印表头信息，包括姓名、身份证号、性别、年龄、学号、答题次数和密码。

---

#### printstu
```c
/**
 * @brief 打印学生信息
 * @param p 学生信息
 */
void printstu(STU *p) {
    printf("| %-39s | %-17s | %-6s | %-4d | %-19s | %-12d | %-19s |\n",
           p->name,
           p->no,
           p->xb == 0 ? "female" : "male",
           p->age,
           p->sch_num,
           p->countStuents,
           p->password);
}
```

#### 函数说明

函数 `printstu` 用于打印学生信息。

#### 函数算法实现解析

1. 打印学生的姓名、身份证号、性别、年龄、学号、答题次数和密码。

---

#### ListSearch
```c
/**
 * @brief 对链表进行查找操作
 * @param L 链表头指针
 */
void ListSearch(STU *L) {
    char n[40];
    STU *p = L->next;
    if (p == NULL)
        printf("数据为空，无法查找！");
    else {
        printf("请输入您要查找的考生的学号:");
        scanf("%s", n);
        while (strcmp(p->sch_num, n) != 0) {
            p = p->next;
            if (p == NULL) {
                printf("没有找到相关信息\n");
                return;
            }
        }
        printf("您要查找的考生信息如下：\n");
        printHeader();
        printstu(p);
    }
}
```

#### 函数说明

函数 `ListSearch` 用于查找链表中的指定节点并输出其信息。

#### 函数算法实现解析

1. 输入要查找的学生学号。
2. 遍历链表查找对应节点。
3. 如果找到，输出学生信息。

---

#### ListModify
```c
/**
 * @brief 对链表进行修改操作
 * @param L 链表头指针
 */
void ListModify(STU *L) {
    int x, a, item;
    int flag = 1;
    char nam[40], no[18], number[20], n[40];
    STU *p = L->next;
    if (p == NULL)
        printf("数据为空，无法修改！");
    else {
        printf("请输入您要修改的考生的学号:");
        scanf("%s", n);
        while (strcmp(p->sch_num, n) != 0) {
            p = p->next;
            if (p == NULL) {
                printf("没有找到相关信息\n");
                return;
            }
        }
        printf("请选择您要修改的考生信息类型:\n");
        printf("1、姓名\n2、身份证号\n3、性别\n4、年龄\n5、学号\n6、密码\n");
        scanf("%d", &item);
        while (flag == 1) {
            switch (item) {
            case 1:
                printf("请输入您修改后的姓名:");
                scanf("%s", nam);
                strcpy(p->name, nam);
                flag = 0;
                break;
            case 2:
                printf("请输入您修改后的身份证号:");
                scanf("%s", no);
                strcpy(p->no, no);
                flag = 0;
                break;
            case 3:
                printf("请输入您修改后的性别:");
                scanf("%d", &x);
                p->xb = x;
                flag = 0;
                break;
            case 4:
                printf("请输入您修改后的年龄:");
                scanf("%d", &a);
                p->age = a;
                flag = 0;
                break;
            case 5:
                printf("请输入您修改后的学号:");
                scanf("%s", number);
                strcpy(p->sch_num, number);
                flag = 0;
                break;
            case 6:
                printf("请输入您修改后的密码:");
                scanf("%s", number);
                strcpy(p->password, number);
                flag = 0;
                break;
            default:
                printf("输入错误，请重新输入！\n");
                scanf("%d", &item);
                flag = 1;
                break;
            }
        }
        printf("修改成功!");
    }
}
```

#### 函数说明

函数 `ListModify` 用于修改链表中的指定节点信息。

#### 函数算法实现解析

1. 输入要修改的学生学号。
2. 遍历链表查找对应节点。
3. 显示可修改的信息类型菜单。
4. 根据用户选择修改对应信息。

---

#### Input
```c
/**
 * @brief 输入学生信息
 * @param p 学生信息
 * @param i 序号
 * @param L 链表头指针
 * @return true/false 输入是否成功
 */
bool Input(STU *p, int i, STU *L) {
    printf("请输入第%d名考生的学号:", i + 1);
    scanf("%s", p->sch_num);
    if (stuExist(L, p->sch_num)) {
        printf("该考生已存在，请重新输入！\n");
        return false;
    }
    printf("请输入第%d名考生的姓名:", i + 1);
    scanf("%s", p->name);
    printf("请输入第%d名考生的身份证号:", i + 1);
    scanf("%s", p->no);
    printf("请输入第%d名考生的性别（女性为0，男性为1）:", i + 1);
    scanf("%d", &p->xb);
    if (p->xb != 0 && p->xb != 1) {
        printf("性别输入错误，请重新输入！\n");
        return false;
    }
    printf("请输入第%d名考生的年龄:", i + 1);
    scanf("%d", &p->age);
    p->countStuents = 0;
    p->password[0] = '\0';
    return true;
}
```

#### 函数说明

函数 `Input` 用于输入学生信息并进行有效性检查。

#### 函数算法实现解析

1. 输入学生学号并检查是否已存在。
2. 输入学生的姓名、身份证号、性别和年龄，并进行有效性检查。
3. 返回输入是否成功的状态。

---

#### Output
```c


/**
 * @brief 输出学生信息
 * @param L 链表头指针
 */
void Output(STU *L) {
    STU *p = L->next;
    while (p != NULL) {
        printstu(p);
        p = p->next;
    }
}
```

#### 函数说明

函数 `Output` 用于输出链表中的所有学生信息。

#### 函数算法实现解析

1. 遍历链表并打印每个节点的学生信息。

---

#### changepassword
```c
/**
 * @brief 修改学生密码
 * @param p 学生信息指针
 */
void changepassword(STU *p) {
    char password[20];
    printf("请输入旧密码:");
    scanf("%s", password);
    if (strcmp(p->password, password) != 0) {
        printf("密码错误！\n");
    } else {
        printf("请输入新密码:");
        scanf("%s", password);
        strcpy(p->password, password);
        printf("密码修改成功！\n");
    }
}
```

#### 函数说明

函数 `changepassword` 用于修改学生密码。

#### 函数算法实现解析

1. 输入旧密码并进行验证。
2. 输入新密码并更新学生信息。

---

#### studentmodel
```c
/**
 * @brief 学生模式
 * @param root JSON根节点
 * @param L 链表头指针
 */
void studentmodel(cJSON *root, STU *L) {
    int item;
    char num[20];
    char password[20];
    STU *p = L->next;
    printf("请输入学号:");
    scanf("%s", num);
    while (strcmp(p->sch_num, num) != 0) {
        p = p->next;
        if (p == NULL) {
            printf("没有找到相关信息！\n");
            getchar();
            printf("\n请按任意键返回主菜单\n");
            getchar();
            system("clear");
            return;
        }
    }
    if (p->password[0] == '\0') {
        printf("请设置密码:");
        scanf("%s", password);
        strcpy(p->password, password);
    } else {
        printf("请输入密码:");
        scanf("%s", password);
        if (strcmp(p->password, password) != 0) {
            printf("密码错误！\n");
            getchar();
            printf("\n请按任意键返回主菜单\n");
            getchar();
            system("clear");
            return;
        }
    }
    printf("欢迎您，%s同学!\n", p->name);

    do {
        studentMenu();
        printf("请输入相应的数字，进行相应的操作:\n");
        scanf("%d", &item);
        system("clear");
        switch (item) {
        case 1:
            // 添加答题功能代码
            run_better_tui_mode();
            break;
        case 2:
            changepassword(p);
            getchar();
            printf("\n请按任意键返回主菜单\n");
            getchar();
            system("clear");
            break;
        case 0:
            printf("即将退出考生信息管理系统.....\n");
            getchar();
            save_students_to_file(saveRoot(root, L));
            printf("\n请按任意键继续\n");
            getchar();
            system("clear");
            return;
        default:
            printf("您输入的命令有误，请重新输入\n");
            getchar();
            printf("\n请按任意键返回主菜单\n");
            getchar();
            system("clear");
        }
    } while (item);
}
```

#### 函数说明

函数 `studentmodel` 用于学生模式下进行操作，包括答题和修改密码。

#### 函数算法实现解析

1. 输入学号并验证。
2. 如果密码未设置，要求设置密码，否则要求输入密码进行验证。
3. 欢迎学生并显示操作菜单。
4. 根据用户选择执行相应操作。

---

#### teachermodel
```c
/**
 * @brief 管理员模式
 * @param root JSON根节点
 * @param L 链表头指针
 */
void teachermodel(cJSON *root, STU *L) {
    int item, n;
    char nam[30];
    char password[30];
    printf("请输入管理员账号:");
    scanf("%s", nam);
    if (strcmp(nam, "admin") != 0) {
        printf("账号错误，请重新输入！\n");
        getchar();
        printf("\n请按任意键返回主菜单\n");
        getchar();
        system("clear");
        return;
    }
    printf("请输入密码:");
    scanf("%s", password);
    if (strcmp(password, "123456") != 0) {
        printf("密码错误，请重新输入！\n");
        getchar();
        printf("\n请按任意键返回主菜单\n");
        getchar();
        system("clear");
        return;
    }
    system("clear");
    do {
        teacherMenu();
        printf("请输入相应的数字，进行相应的操作:\n");
        scanf("%d", &item);
        system("clear");
        switch (item) {
        case 1:
            printf("请输入您要录入的考生人数:");
            scanf("%d", &n);
            L = ListCreate(L, n);
            getchar();
            printf("\n请按任意键返回主菜单\n");
            getchar();
            system("clear");
            break;
        case 2:
            printf("全部考生信息如下:\n");
            printHeader();
            Output(L);
            getchar();
            printf("\n请按任意键返回主菜单\n");
            getchar();
            system("clear");
            break;
        case 3:
            ListSearch(L);
            getchar();
            printf("\n请按任意键返回主菜单\n");
            getchar();
            system("clear");
            break;
        case 4:
            ListModify(L);
            getchar();
            printf("\n请按任意键返回主菜单\n");
            getchar();
            system("clear");
            break;
        case 5:
            ListDelete(L);
            getchar();
            printf("\n请按任意键返回主菜单\n");
            getchar();
            system("clear");
            break;
        case 6:
            ListInsert(L);
            getchar();
            printf("\n请按任意键返回主菜单\n");
            getchar();
            system("clear");
            break;
        case 7:
            save_students_to_file(saveRoot(root, L));
            getchar();
            printf("\n保存成功！");
            printf("\n请按任意键返回主菜单\n");
            getchar();
            system("clear");
            break;
        case 0:
            getchar();
            printf("即将退出考生信息管理系统.....\n");
            printf("\n请按任意键继续\n");
            getchar();
            system("clear");
            save_students_to_file(saveRoot(root, L));
            return;
        default:
            printf("您输入的命令有误，请重新输入");
        }
        printf("\n\n\n\n");
    } while (item);
}
```

#### 函数说明

函数 `teachermodel` 用于管理员模式下进行操作，包括录入、查询、修改、删除和插入学生信息。

#### 函数算法实现解析

1. 输入管理员账号和密码进行验证。
2. 显示操作菜单。
3. 根据用户选择执行相应操作，包括录入、查询、修改、删除和插入学生信息。
4. 提供保存功能，将学生信息保存到文件。