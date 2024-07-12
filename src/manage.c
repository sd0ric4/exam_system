#include "manage.h"
#include "mode.h"
#include "student.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

/**
 * @brief 取消回显
 * @param prompt 提示信息
 * @param password 密码
 */
void disableEcho(char *prompt, char *password) {
    struct termios oldt, newt;

    // 显示提示信息
    printf("%s", prompt);

    // 关闭回显
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // 读取密码
    scanf("%s", password);

    // 恢复回显
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n"); // 为了美观，输入密码后换行
}

/**
 * @brief 创建表头，初始化链表
 * @param L 链表头指针
 * @return STU* 返回链表头指针
 */
STU *ListInit(STU *L)
{
    STU *head = NULL;
    head = (STU *)malloc(sizeof(STU));
    head->next = NULL;
    L = head;
    return L;
}
/**
 * @brief 添加学生信息到json
 * @param root json根节点
 * @param stu 学生信息
 */
void add_student_to_json(cJSON *root, STU *stu)
{
    cJSON *student = cJSON_CreateObject();
    cJSON_AddStringToObject(student, "name", stu->name);
    cJSON_AddStringToObject(student, "no", stu->no);
    cJSON_AddNumberToObject(student, "xb", stu->xb);
    cJSON_AddNumberToObject(student, "age", stu->age);
    cJSON_AddStringToObject(student, "sch_num", stu->sch_num);
    cJSON_AddStringToObject(student, "password", stu->password);
    cJSON_AddNumberToObject(student, "countStuents", stu->countStuents);
    cJSON_AddNumberToObject(student, "countAnswerTimes", stu->countAnswerTimes);
    cJSON_AddNumberToObject(student, "countRightTimes", stu->countRightTimes);
    cJSON_AddItemToArray(root, student);
}

/**
 * @brief 从json中读取学生信息
 * @param root json根节点
 * @param rootNum json根节点的数量
 * @param L 链表头指针
 */
void read_student_from_json(cJSON *root, int rootNum, STU *L)
{
    for (int i = 0; i < rootNum; i++)
    {
        STU *stu = (STU *)malloc(sizeof(STU));
        cJSON *item = cJSON_GetArrayItem(root, i);
        strcpy(stu->name, cJSON_GetObjectItem(item, "name")->valuestring);
        strcpy(stu->no, cJSON_GetObjectItem(item, "no")->valuestring);
        stu->xb = cJSON_GetObjectItem(item, "xb")->valueint;
        stu->age = cJSON_GetObjectItem(item, "age")->valueint;
        strcpy(stu->sch_num, cJSON_GetObjectItem(item, "sch_num")->valuestring);
        strcpy(stu->password, cJSON_GetObjectItem(item, "password")->valuestring);
        stu->countStuents = cJSON_GetObjectItem(item, "countStuents")->valueint;
        stu->countAnswerTimes = cJSON_GetObjectItem(item, "countAnswerTimes")->valueint;
        stu->countRightTimes = cJSON_GetObjectItem(item, "countRightTimes")->valueint;
        stu->next = L->next;
        L->next = stu;
    }
}

/**
 * @brief 保存链表到json
 * @param root json根节点
 * @param L 链表头指针
 * @return cJSON* 返回json根节点
 */
cJSON *saveRoot(cJSON *root, STU *L)
{
    STU *p = L->next;
    cJSON_Delete(root);
    root = cJSON_CreateArray();
    while (p != NULL)
    {
        add_student_to_json(root, p);
        p = p->next;
    }
    return root;
}

/**
 * @brief 保存学生信息到文件
 * @param root json根节点
 */
void save_students_to_file(cJSON *root)
{
    FILE *file = fopen("student.json", "w");
    if (!file)
    {
        perror("Error opening file");
        return;
    }
    char *json_string = cJSON_Print(root);
    fprintf(file, "%s", json_string);
    free(json_string);
    fclose(file);
}

/**
 * @brief 从文件中读取学生信息
 * @param filename 文件名
 * @return cJSON* 返回json根节点
 */
cJSON *read_students_from_file(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
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

/**
 * @brief 判断学生是否存在
 * @param L 链表头指针
 * @param sch_num 学号
 * @return true/false
 */
bool stuExist(STU *L, char *sch_num)
{
    STU *p = L->next;
    while (p != NULL)
    {
        if (strcmp(p->sch_num, sch_num) == 0)
        {
            return true;
        }
        p = p->next;
    }
    return false;
}

/**
 * @brief 创建链表，将新生成的节点插入到链表的表头
 * @param L 链表头指针
 * @param n 学生数量
 * @return STU* 返回链表头指针
 */
STU *ListCreate(STU *L, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        STU *p;
        // 将新生成的节点插入到链表中
        p = NULL;
        p = (STU *)malloc(sizeof(STU));
        if (Input(p, i, L) == false)
        {
            i--;
        }
        else
        {
            p->next = L->next;
            L->next = p;
        }
    }
    return L;
}

/**
 * @brief 对链表进行节点的插入操作
 * @param L 链表头指针
 */
void ListInsert(STU *L)
{
    STU *s = NULL;
    STU *p = L->next;
    // 生成一个新节点s
    s = (STU *)malloc(sizeof(STU));
    printf("请输入您要插入的考生的学号:");
    scanf("%s", s->sch_num);
    if (stuExist(L, s->sch_num))
    {
        printf("该考生已存在，无法插入！\n");
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
    s->countStuents = 0;
    s->countAnswerTimes = 0;
    s->countRightTimes = 0;
    s->password[0] = '\0';
    s->next = L->next;
    L->next = s;
}

/**
 * @brief 对链表的进行节点的删除操作
 * @param L 链表头指针
 */
void ListDelete(STU *L)
{
    char n[40];
    char choose;
    STU *p = L->next, *pre = L; // 定义p指针指向头节点的指向，定义pre指向头节点，pre始终指向p的前驱节点
    if (p == NULL)
        printf("数据为空，无法删除！");
    else
    {
        printf("请输入您要删除的考生的学号:");
        scanf("%s", n);
        while (strcmp(p->sch_num, n) != 0)
        {
            pre = p;
            p = pre->next;
            if (p == NULL)
            {
                printf("没有找到相关信息，无法删除\n");
                return;
            }
        }
        printf("您要删除的考生信息如下：\n");
        printHeader();
        printstu(p);
        printf("\n是否确认删除该考生信息？(Y/N)\n");
        getchar();
        scanf("%c", &choose);
        if (choose == 'Y' || choose == 'y')
        {
            pre->next = p->next;
            free(p);
            printf("删除成功!");
            return;
        }
        else if (choose == 'N' || choose == 'n')
        {
            printf("已取消删除");
            return;
        }
        else
        {
            printf("输入有误！");
            return;
        }
    }
}
/**
 * @brief 打印表头
 */
void printHeader()
{
    printf("| %-39s | %-17s | %-6s | %-4s | %-19s | %-12s | %-19s | %-8s |\n",
           "name", "identityCard", "gender", "age", "studentNumber", "answerTimes", "password", "accuracy");
}

/**
 * @brief 打印学生信息
 * @param p 学生信息
 */
void printstu(STU *p)
{
    printf("| %-39s | %-17s | %-6s | %-4d | %-19s | %-12d | %-19s | %-8.2f |\n",
           p->name,
           p->no,
           p->xb == 0 ? "female" : "male",
           p->age,
           p->sch_num,
           p->countStuents,
           p->password,
           p->countStuents == 0 ? 0 : (double)p->countRightTimes / p->countStuents * 100);
}

/**
 * @brief 对链表进行查找操作
 * @param L 链表头指针
 */
void ListSearch(STU *L)
{
    char n[40];
    STU *p = L->next;
    if (p == NULL)
        printf("数据为空，无法查找！");
    else
    {
        printf("请输入您要查找的考生的学号:");
        scanf("%s", n);
        while (strcmp(p->sch_num, n) != 0)
        {
            p = p->next;
            if (p == NULL)
            {
                printf("没有找到相关信息\n");
                return;
            }
        }
        printf("您要查找的考生信息如下：\n");
        printHeader();
        printstu(p);
    }
}

/**
 * @brief 对链表进行修改操作
 * @param L 链表头指针
 */
void ListModify(STU *L)
{
    int x, a, item;
    int flag = 1;
    char nam[40], no[18], number[20], n[40];
    STU *p = L->next;
    if (p == NULL)
        printf("数据为空，无法修改！");
    else
    {
        printf("请输入您要修改的考生的学号:");
        scanf("%s", n);
        while (strcmp(p->sch_num, n) != 0)
        {
            p = p->next;
            if (p == NULL)
            {
                printf("没有找到相关信息\n");
                return;
            }
        }
        printf("请选择您要修改的考生信息类型:\n");
        printf("1、姓名\n2、身份证号\n3、性别\n4、年龄\n5、学号\n6、密码\n");
        scanf("%d", &item);
        while (flag == 1)
        {
            switch (item)
            {
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
                disableEcho("请输入您修改后的密码:", number);
                char password2[20];
                disableEcho("请再次输入密码:", password2);
                if (strcmp(number, password2) != 0)
                {
                    printf("两次密码输入不一致！\n");
                    return;
                }
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

/**
 * @brief 输入学生信息
 * @param p 学生信息
 * @param i 序号
 * @param L 链表头指针
 * @return true/false
 */
bool Input(STU *p, int i, STU *L)
{
    printf("请输入第%d名考生的学号:", i + 1);
    scanf("%s", p->sch_num);
    if (stuExist(L, p->sch_num))
    {
        printf("该考生已存在，请重新输入！\n");
        return false;
    }
    printf("请输入第%d名考生的姓名:", i + 1);
    scanf("%s", p->name);
    printf("请输入第%d名考生的身份证号:", i + 1);
    scanf("%s", p->no);
    printf("请输入第%d名考生的性别（女性为0，男性为1）:", i + 1);
    scanf("%d", &p->xb);
    if (p->xb != 0 && p->xb != 1)
    {
        printf("性别输入错误，请重新输入！\n");
        return false;
    }
    printf("请输入第%d名考生的年龄:", i + 1);
    scanf("%d", &p->age);
    p->countStuents = 0;
    p->countAnswerTimes = 0;
    p->countRightTimes = 0;
    p->password[0] = '\0';
    return true;
}

/**
 * @brief 输出学生信息
 * @param L 链表头指针
 */
void Output(STU *L)
{
    STU *p = L->next;
    while (p != NULL)
    {
        printstu(p);
        p = p->next;
    }
}

/**
 * @brief 学生模式
 * @param root json根节点
 * @param L 链表头指针
 */
void changepassword(STU *p)
{
    char password[20];
    disableEcho("请输入原密码:", password);
    if (strcmp(p->password, password) != 0)
    {
        printf("密码错误！\n");
        return;
    }
    else
    {
        disableEcho("请输入新密码:", password); 
        char password2[20];
        disableEcho("请再次输入密码:", password2);
        if (strcmp(password, password2) != 0)
        {
            printf("两次密码输入不一致！\n");
            return;
        }
        strcpy(p->password, password);
        printf("密码修改成功！\n");
        return;
    }
}

/**
 * @brief 学生模式
 * @param root json根节点
 * @param L 链表头指针
 */
void studentmodel(cJSON *root, STU *L)
{
    int item;
    char num[20];
    char password[20];
    STU *p = L->next;
    printf("请输入学号:");
    scanf("%s", num);
    while (strcmp(p->sch_num, num) != 0)
    {
        p = p->next;
        if (p == NULL)
        {
            printf("没有找到相关信息！\n");
            getchar();
            printf("\n请按任意键返回主菜单\n");
            getchar();
            system("clear");
            return;
        }
    }
    if (p->password[0] == '\0')
    {
        disableEcho("由于您是第一次登录，请设置密码:", password);
        char password2[20];
        disableEcho("请再次输入密码:", password2);
        if (strcmp(password, password2) != 0)
        {
            printf("两次密码输入不一致！\n");
            getchar();
            printf("\n请按任意键返回主菜单\n");
            getchar();
            system("clear");
            return;
        }
        strcpy(p->password, password);
    }
    else
    {
        disableEcho("请输入密码:", password);
        if (strcmp(p->password, password) != 0)
        {
            printf("密码错误！\n");
            getchar();
            printf("\n请按任意键返回主菜单\n");
            getchar();
            system("clear");
            return;
        }
    }
    printf("欢迎您，%s同学!\n", p->name);

    do
    {
        studentMenu();
        printf("请输入相应的数字，进行相应的操作:\n");
        scanf("%d", &item);
        system("clear");
        switch (item)
        {
        case 1:
            // 添加答题功能代码
            run_better_tui_mode();
            p->countStuents += 1;
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

/**
 * @brief 管理员模式
 * @param root json根节点
 * @param L 链表头指针
 */
void teachermodel(cJSON *root, STU *L)
{
    int item, n;
    char nam[30];
    char password[30];
    printf("请输入管理员账号:");
    scanf("%s", nam);
    if (strcmp(nam, ADMIN_NAME) != 0)
    {
        printf("账号错误，请重新输入！\n");
        getchar();
        printf("\n请按任意键返回主菜单\n");
        getchar();
        system("clear");
        return;
    }
    disableEcho("请输入密码:", password);
    if (strcmp(password, ADMIN_PASSWORD) != 0)
    {
        printf("密码错误，请重新输入！\n");
        getchar();
        printf("\n请按任意键返回主菜单\n");
        getchar();
        system("clear");
        return;
    }
    system("clear");
    do
    {
        teacherMenu();
        printf("请输入相应的数字，进行相应的操作:\n");
        scanf("%d", &item);
        system("clear");
        switch (item)
        {
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
