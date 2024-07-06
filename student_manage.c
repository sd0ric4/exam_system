#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//定义一个考生结构体
typedef struct student
{
    char name[40];
    char no[18];
    int xb;
    int age;
    char sch_num[20];
    struct student *next;
}STU;

//函数声明放在这里
STU *LIstInit(STU *L);
STU *ListCreate(STU *L, int n);
void ListInsert(STU *L);
void ListDelete(STU *L);
void LIstSearch(STU *L);
void Input(STU *p,int i);
void Output(STU *L);
void PrintFile(STU *L);
void Menu();

//创建表头，初始化链表
STU *LIstInit(STU *L)
{
    STU *head=NULL;
    head=(STU *)malloc(sizeof(STU));
    head->next=NULL;
    L = head;
    return L;
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
        Input(p,i);
        p->next = L->next;
        L->next = p;
    }
    return L;
}

//对链表进行节点的插入操作
void ListInsert(STU *L)
{
    STU *s = NULL;
    //生成一个新节点s
    s = (STU *)malloc(sizeof(STU));
    printf("请输入您要插入的考生的姓名:");
    scanf("%s",s->name);
    printf("请输入您要插入的考生的身份证号:");
    scanf("%s",s->no);
    printf("请输入您要插入的考生的性别（女性为0，男性为1）:");
    scanf("%d",&s->xb);
    printf("请输入您要插入的考生的年龄:");
    scanf("%d",&s->age);
    printf("请输入您要插入的考生的学号:");
    scanf("%s",s->sch_num);
    s->next = L->next;
    L->next = s;
}

//对链表的进行节点的删除操作
void ListDelete(STU *L)
{
    char n[40];
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
        pre->next=p->next;
        free(p);
        printf("删除成功");
    }
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
        printf("该考生的信息如下:\n");
        printf("姓名\t\t身份证号\t\t性别\t年龄\t学号\n");
        printf("%s\t",p->name);
        printf("\t%s",p->no);
        printf("\t%d",p->xb);
        printf("\t%d",p->age);
        printf("\t%s",p->sch_num);
        printf("\n");
    }
}

void ListModify(STU *L)
{
    int x,a,item;
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
            if (p == NULL)
                printf("没有找到相关信息\n");
        }
        printf("请选择您要修改的考生信息类型:\n");
        printf("1、姓名\n2、身份证号\n3、性别\n4、年龄\n5、学号\n");
        scanf("%d",&item);
        switch(item)
        {
            case 1:
                printf("请输入您修改后的姓名:");
                scanf("%s",nam);
                strcpy(p->name,nam);
                break;
            case 2:
                printf("请输入您修改后的身份证号:");
                scanf("%s",no);
                strcpy(p->no,no);
                break;
            case 3:
                printf("请输入您修改后的性别:");
                scanf("%d",&x);
                p->xb=x;
                break;
            case 4:
                printf("请输入您修改后的年龄:");
                scanf("%d",&a);
                p->age=a;
                break;
            case 5:
                printf("请输入您修改后的学号:");
                scanf("%s",number);
                strcpy(p->sch_num,number);
                break;
        }
        printf("修改成功!");
    }
}

void Input(STU *p,int i)
{
    printf("请输入第%d名考生的姓名:",i+1);
    scanf("%s",p->name);
    printf("请输入第%d名考生的身份证号:",i+1);
    scanf("%s",p->no);
    printf("请输入第%d名考生的性别（女性为0，男性为1）:",i+1);
    scanf("%d",&p->xb);
    printf("请输入第%d名考生的年龄:",i+1);
    scanf("%d",&p->age);
    printf("请输入第%d名考生的学号:",i+1);
    scanf("%s",p->sch_num);
}

void Output(STU *L)
{
    STU *p=L->next;
    while(p!=NULL)
    {
        printf("%s\t",p->name);
        printf("\t%s",p->no);
        printf("\t%d",p->xb);
        printf("\t%d",p->age);
        printf("\t%s",p->sch_num);
        printf("\n");
        p=p->next;
    }
}

void PrintFile(STU *L)
{
    STU *p=L->next;
    FILE *fp;
    fp=fopen("student.txt","w");
    fprintf(fp,"姓名\t\t身份证号\t\t\t\t性别\t年龄\t学号\n");
    while(p!=NULL)
    {
        fprintf(fp,"%s\t",p->name);
        fprintf(fp,"\t%s",p->no);
        fprintf(fp,"\t%d",p->xb);
        fprintf(fp,"\t%d",p->age);
        fprintf(fp,"\t%s",p->sch_num);
        fprintf(fp,"\n");
        p=p->next;
    }
    fclose(fp);
    printf("保存成功!");
}

void Menu()
{
    printf("\n\n");
	printf("\t\t\t===================考试报名管理系统==================\n");
	printf("\t\t\t*                                                   *\n");
	printf("\t\t\t*          1>. 录入考生信息                         *\n");
	printf("\t\t\t*          2>. 全部考生信息                         *\n");
	printf("\t\t\t*          3>. 查看考生信息                         *\n");
	printf("\t\t\t*          4>. 修改考生信息                         *\n");
	printf("\t\t\t*          5>. 删除考生信息                         *\n");
	printf("\t\t\t*          6>. 插入考生信息                         *\n");
	printf("\t\t\t*          7>. 保存考生信息                         *\n");
	printf("\t\t\t*          0>. 退出管理系统                         *\n");
	printf("\t\t\t*                                    欢迎使用本系统!*\n");
	printf("\t\t\t=====================================================\n");
	printf("\t\t\t输入选项，按回车进入选项:                             \n");
}

int main()
{
    int item,n;         //item用于接收输入的命令，n用于接收输入的考生人数
    char nam[30];         
    STU *L=NULL;
    L=LIstInit(L);
    do
    { 
        Menu();
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
                printf("姓名\t\t身份证号\t\t性别\t年龄\t学号\n");
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
                PrintFile(L);
                getchar();
                printf("\n请按任意键返回主菜单\n");
				getchar();
				system("cls");
                break;
            case 0:
                printf("即将退出考生信息管理系统.....");
                exit(0);
            default:
                printf("您输入的命令有误，请重新输入");
        }
        printf("\n\n\n\n");
    }while(item);
    return 0;
}

