#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "cJSON.h"
//定义一个考生结构体
typedef struct student
{
    char name[40];
    char no[18];
    int xb;
    int age;
    char sch_num[20];
    char password[20];
    struct student *next;
    int countStuents;
}STU;

//函数声明放在这里
STU *LIstInit(STU *L);
STU *ListCreate(STU *L, int n);
void ListInsert(STU *L);
void ListDelete(STU *L);
void LIstSearch(STU *L);
bool Input(STU *p,int i,STU *L);
void Output(STU *L);
void studentmodel(cJSON *root, STU *L);
void teachermodel(cJSON *root, STU *L);
void teacherMenu();

//创建表头，初始化链表
STU *LIstInit(STU *L)
{
    STU *head=NULL;
    head=(STU *)malloc(sizeof(STU));
    head->next=NULL;
    L = head;
    return L;
}

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

void read_student_from_json(cJSON *root, int rootNum, STU *L) {
    for(int i = 0; i < rootNum; i++){
        STU *stu = (STU *)malloc(sizeof(STU));
        cJSON *item = cJSON_GetArrayItem(root, i);
        strcpy(stu->name , cJSON_GetObjectItem(item, "name")->valuestring);
        strcpy(stu->no , cJSON_GetObjectItem(item, "no")->valuestring);
        stu->xb = cJSON_GetObjectItem(item, "xb")->valueint;
        stu->age = cJSON_GetObjectItem(item, "age")->valueint;
        strcpy(stu->sch_num , cJSON_GetObjectItem(item, "sch_num")->valuestring);
        strcpy(stu->password , cJSON_GetObjectItem(item, "password")->valuestring);
        stu->countStuents = cJSON_GetObjectItem(item, "countStuents")->valueint;
        stu->next = L->next;
        L->next = stu;
    }
}

cJSON *saveRoot(cJSON *root, STU *L) {
    STU *p = L->next;
    cJSON_Delete(root);
    root = cJSON_CreateArray();
    while(p!= NULL){
        add_student_to_json(root, p);
        p = p->next;
    }
    return root;
}

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

cJSON *read_students_from_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char *buffer = (char*)malloc(size + 1);
    fread(buffer, size, 1, file);
    buffer[size] = '\0';
    
    cJSON *root = cJSON_Parse(buffer);
    free(buffer);
    fclose(file);
    return root;
}

bool stuExist(STU *L, char *sch_num) {
    STU *p = L->next;
    while (p!= NULL) {
        if (strcmp(p->sch_num, sch_num) == 0) {
            return true;
        }
        p = p->next;
    }
    return false;
}

//创建链表，将新生成的节点插入到链表的表头
STU *ListCreate(STU *L, int n)
{
    int i; 
    for (i = 0; i<n; i++)
    {
        STU *p;
        //将新生成的节点插入到链表中
        p=NULL;
        p = (STU *)malloc(sizeof(STU));
        if(Input(p,i,L) == false){
            i--;
        }else{
            p->next=L->next;
            L->next=p;
        }
    }
    return L;
}

//对链表进行节点的插入操作
void ListInsert(STU *L)
{
    STU *s = NULL;
    STU *p=L->next;
    //生成一个新节点s
    s = (STU *)malloc(sizeof(STU));
    printf("请输入您要插入的考生的学号:");
    scanf("%s",s->sch_num);
    if(stuExist(L,s->sch_num))
    {
        printf("该考生已存在，无法插入！\n");
        return;
    }
    printf("请输入您要插入的考生的姓名:");
    scanf("%s",s->name);
    printf("请输入您要插入的考生的身份证号:");
    scanf("%s",s->no);
    printf("请输入您要插入的考生的性别（女性为0，男性为1）:");
    scanf("%d",&s->xb);
    printf("请输入您要插入的考生的年龄:");
    scanf("%d",&s->age);
    s->countStuents = 1;
    s->password[0] = '\0';
    s->next = L->next;
    L->next = s;
}

//对链表的进行节点的删除操作
void ListDelete(STU *L)
{
    char n[40];
    char choose;
    STU *p=L->next,*pre=L;  //定义p指针指向头节点的指向，定义pre指向头节点，pre始终指向p的前驱节点
    if(p==NULL)
        printf("数据为空，无法删除！");
    else
    {
        printf("请输入您要删除的考生的学号:");
        scanf("%s",n);
        while(strcmp(p->sch_num,n)!=0)
        {
            pre=p;
            p=pre->next;
            if(p==NULL)
            {
                printf("没有找到相关信息，无法删除\n");
                return;
            }
        }
        printf("您要删除的考生信息如下：\n");
        printf("姓名\t\t身份证号\t\t性别\t年龄\t学号\t\t答题次数\t密码\n");
        printstu(p);
        printf("\n是否确认删除该考生信息？(Y/N)\n");
        getchar();
        scanf("%c",&choose);
        if(choose=='Y'||choose=='y')
        {
            pre->next=p->next;
            free(p);
            printf("删除成功!");
            return;
        }else if (choose=='N'||choose=='n')
        {
            printf("已取消删除");
            return;
        }else{
            printf("输入有误！");
            return;
        }
    }
}

void printstu(STU *p){
    printf("%s\t",p->name);
    printf("\t%s",p->no);
    printf("\t%d",p->xb);
    printf("\t%d",p->age);
    printf("\t%s",p->sch_num);
    printf("\t%d",p->countStuents);
    printf("\t\t%s",p->password);
    printf("\n");
}

void LIstSearch(STU *L)
{
    char n[40];
    STU *p=L->next;
    if (p == NULL)
        printf("数据为空，无法查找！");
    else
    {
        printf("请输入您要查找的考生的学号:");
        scanf("%s",n);
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
        printf("姓名\t\t身份证号\t\t性别\t年龄\t学号\t\t答题次数\t密码\n");
        printstu(p);
    }
}

void ListModify(STU *L)
{
    int x,a,item;
    int flag = 1;
    char nam[40],no[18],number[20],n[40];
    STU *p=L->next;
    if (p == NULL)
        printf("数据为空，无法修改！");
    else
    {
        printf("请输入您要修改的考生的学号:");
        scanf("%s",n);
        while(strcmp(p->sch_num, n) != 0)
        {
            p = p->next;
            if (p == NULL){
                printf("没有找到相关信息\n");
                return;
            }
        }
        printf("请选择您要修改的考生信息类型:\n");
        printf("1、姓名\n2、身份证号\n3、性别\n4、年龄\n5、学号\n6、密码\n");
        scanf("%d",&item);
        while (flag == 1)
        {
            switch(item)
            {
                case 1:
                    printf("请输入您修改后的姓名:");
                    scanf("%s",nam);
                    strcpy(p->name,nam);
                    flag = 0;
                    break;
                case 2:
                    printf("请输入您修改后的身份证号:");
                    scanf("%s",no);
                    strcpy(p->no,no);
                    flag = 0;
                    break;
                case 3:
                    printf("请输入您修改后的性别:");
                    scanf("%d",&x);
                    p->xb=x;
                    flag = 0;
                    break;
                case 4:
                    printf("请输入您修改后的年龄:");
                    scanf("%d",&a);
                    p->age=a;
                    flag = 0;
                    break;
                case 5:
                    printf("请输入您修改后的学号:");
                    scanf("%s",number);
                    strcpy(p->sch_num,number);
                    flag = 0;
                    break;
                case 6:
                    printf("请输入您修改后的密码:");
                    scanf("%s",number);
                    strcpy(p->password,number);
                    flag = 0;
                    break;
                default:
                    printf("输入错误，请重新输入！\n");
                    scanf("%d",&item);
                    flag = 1;
                    break;
            }
        }
        printf("修改成功!");
    }
}

bool Input(STU *p,int i,STU *L)
{   

    printf("请输入第%d名考生的学号:",i+1);
    scanf("%s",p->sch_num);
    if(stuExist(L,p->sch_num))
    {
        printf("该考生已存在，请重新输入！\n");
        return false;
    }
    printf("请输入第%d名考生的姓名:",i+1);
    scanf("%s",p->name);
    printf("请输入第%d名考生的身份证号:",i+1);
    scanf("%s",p->no);
    printf("请输入第%d名考生的性别（女性为0，男性为1）:",i+1);
    scanf("%d",&p->xb);
    printf("请输入第%d名考生的年龄:",i+1);
    scanf("%d",&p->age);
    p->countStuents = 0;
    p->password[0] = '\0';
    return true;
}

void Output(STU *L)
{
    STU *p=L->next;
    while(p!=NULL)
    {
        printstu(p);
        p=p->next;
    }
}

void changepassword(STU *p)
{
    char password[20];
    printf("请输入旧密码:");
    scanf("%s",password);
    if(strcmp(p->password,password)!=0)
    {
        printf("密码错误！\n");
        return;
    }else{
        printf("请输入新密码:");
        scanf("%s",password);
        strcpy(p->password,password);
        printf("密码修改成功！\n");
        return;
    }
}

void studentmodel(cJSON *root, STU *L)
{
    int item;
    char num[20];
    char password[20];
    STU *p=L->next;
    printf("请输入学号:");
    scanf("%s",num);
    while (strcmp(p->sch_num, num) != 0)
        {
            p = p->next;
            if (p == NULL)
            {
                printf("没有找到相关信息！\n");
                getchar();
                printf("\n请按任意键返回主菜单\n");
		        getchar();
		        system("cls");
                return;
            }
        }
    if(p->password[0] == '\0'){
        printf("请设置密码:");
        scanf("%s",password);
        strcpy(p->password,password);
    }else{
        printf("请输入密码:");
        scanf("%s",password);
        if(strcmp(p->password,password)!=0)
            {
                printf("密码错误！\n");
                getchar();
                printf("\n请按任意键返回主菜单\n");
		        getchar();
		        system("cls");
                return;
            }
    }
    printf("欢迎您，%s同学!\n",p->name);

    do
    {
        studentMenu();
        printf("请输入相应的数字，进行相应的操作:\n");
        scanf("%d",&item);
        system("cls");
        switch(item)
        {
            case 1:

                

            case 2:
                changepassword(p);
                getchar();
                printf("\n请按任意键返回主菜单\n");
                getchar();
                system("cls");
                break;
            case 0:
                printf("即将退出考生信息管理系统.....\n");
                getchar();
                save_students_to_file(saveRoot(root, L));
                printf("\n请按任意键继续\n");
				getchar();
				system("cls");
                return;
            default:
                printf("您输入的命令有误，请重新输入\n");
                getchar();
                printf("\n请按任意键返回主菜单\n");
		        getchar();
                system("cls");
        }
    } while (item);
    
}
void teachermodel(cJSON *root, STU *L)
{   
    int item,n;
    char nam[30];
    char password[30];
    printf("请输入管理员账号:");
    scanf("%s",nam);
    if(strcmp(nam,"admin")!=0)
    {
        printf("账号错误，请重新输入！\n");
        getchar();
        printf("\n请按任意键返回主菜单\n");
		getchar();
		system("cls");
        return;
    }
    printf("请输入密码:");
    scanf("%s",password);
    if(strcmp(password,"123456")!=0)
    {
        printf("密码错误，请重新输入！\n");
        getchar();
        printf("\n请按任意键返回主菜单\n");
		getchar();
		system("cls");
        return;
    }
    system("cls");
    do
    { 

        teacherMenu();
        printf("请输入相应的数字，进行相应的操作:\n");
        scanf("%d",&item);
        system("cls");
        switch(item)
        {
            case 1:
                printf("请输入您要录入的考生人数:");
                scanf("%d",&n);
                L=ListCreate(L,n);
                getchar();
                printf("\n请按任意键返回主菜单\n");
				getchar();
				system("cls");
                break;
            case 2:
                printf("全部考生信息如下:\n");
                printf("姓名\t\t身份证号\t\t性别\t年龄\t学号\t\t答题次数\t密码\n");
                Output(L);
                getchar();
                printf("\n请按任意键返回主菜单\n");
				getchar();
				system("cls");
                break;
            case 3:
                LIstSearch(L);
                getchar();
                printf("\n请按任意键返回主菜单\n");
				getchar();
				system("cls");
                break;                                             
            case 4:
                ListModify(L);
                getchar();
                printf("\n请按任意键返回主菜单\n");
				getchar();
				system("cls");
                break;
            case 5:
                ListDelete(L);
                getchar();
                printf("\n请按任意键返回主菜单\n");
				getchar();
				system("cls");
                break;
            case 6:
                ListInsert(L);
                getchar();
                printf("\n请按任意键返回主菜单\n");
				getchar();
				system("cls");
                break;
            case 7:
                save_students_to_file(saveRoot(root, L));
                getchar();
                printf("\n保存成功！");
                printf("\n请按任意键返回主菜单\n");
				getchar();
				system("cls");
                break;
            case 0:
                getchar();
                printf("即将退出考生信息管理系统.....\n");
                printf("\n请按任意键继续\n");
				getchar();
				system("cls");
                save_students_to_file(saveRoot(root, L));
                return;
            default:
                printf("您输入的命令有误，请重新输入");
        }
        printf("\n\n\n\n");
    }while(item);
}

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

int main()
{
    int model;         //model用于接收登陆身份       
    STU *L=NULL;
    L=LIstInit(L);
    STU *p=L->next;
    char *filename = "student.json";
    cJSON *root = read_students_from_file(filename);
    if (!root) {
        root = cJSON_CreateArray();
    } else if (cJSON_IsArray(root)) {
        // 已经有数据，直接使用
    } else {
        // 错误：不是数组
        cJSON_Delete(root);
        root = cJSON_CreateArray();
    }
    
    int rootNum = cJSON_GetArraySize(root);
    read_student_from_json(root, rootNum, L);

    do
    {
        mainMenu();
        scanf("%d",&model);
        getchar();
        system("cls");
        switch (model)
        {
        case 1:
            teachermodel(root, L);
            break;
        case 2:
            studentmodel(root,L);
            break;
        case 0:
            printf("即将退出考试系统.....");

            exit(0);
        default:
            printf("您输入的选项有误，请重新输入！");
            break;
        }
    } while (model);
    
    
    return 0;
}
