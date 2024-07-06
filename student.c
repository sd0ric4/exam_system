#include "student.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cjson/cJSON.h>
#include <string.h>

/**
 * 加载题库
 * @param filename 题库文件名
 * @return 题库JSON对象
 */
cJSON *load_question_bank(const char *filename)
{
    //  读取文件
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("fopen");
        return NULL;
    }

    // 获取文件大小
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // 读取文件内容
    char *file_content = (char *)malloc(file_size + 1);
    fread(file_content, 1, file_size, fp);
    file_content[file_size] = '\0';

    // 解析JSON
    cJSON *question_bank = cJSON_Parse(file_content);

    // 释放资源
    free(file_content);

    // 关闭文件
    fclose(fp);

    return question_bank;
}

/**
 * 随机选择题目
 * @param questions 题库JSON对象
 * @param count 题目数量
 */
void select_random_questions(cJSON *questions, int count)
{
    // 实现随机选择题目的代码
    for (int i = 0; i < count; i++)
    {
        int index = rand() % cJSON_GetArraySize(questions);
        cJSON *question = cJSON_GetArrayItem(questions, index);
        // 保存到文件
        char filename[64];
        sprintf(filename, "%ld.json", time(NULL));
        FILE *fp = fopen(filename, "w");
        if (fp == NULL)
        {
            perror("fopen");
            return;
        }
        char *question_str = cJSON_Print(question);
        fprintf(fp, "%s", question_str);
        fclose(fp);
        free(question_str);
    }
}

/**
 * 显示题目并获取答案
 * @param question 题目JSON对象
 */
void display_question_and_get_answer(cJSON *question)
{
    // 实现显示题目和接收答案的代码
    printf("题目：\n%s\n", cJSON_GetObjectItem(question, "title")->valuestring);
    cJSON *options = cJSON_GetObjectItem(question, "options");
    if (options != NULL)
    {
        for (int i = 0; i < cJSON_GetArraySize(options); i++)
        {
            cJSON *option = cJSON_GetArrayItem(options, i);
            if (option != NULL)
            {
                // 假设每个选项只有一个键值对
                cJSON *key = option->child; // 直接获取第一个子项
                if (key != NULL)
                {
                    printf("%s: %s\n", key->string, key->valuestring);
                }
            }
        }
    }
    // 接收答案
    char user_answer[16];
    printf("请输入答案：");
    scanf("%s", user_answer);
    // 比较答案并记录得分
    compare_answers_and_score(question, user_answer);
}

/**
 * 比较答案并记录得分
 * @param question 题目JSON对象
 * @param user_answer 用户答案
 */
void compare_answers_and_score(cJSON *question, const char *user_answer)
{
    const char *correct_answer = cJSON_GetObjectItem(question, "correctAnswer")->valuestring;
    // 创建一个新的字符串数组来存储大写的答案
    char upper_case_answer[strlen(user_answer) + 1]; // +1 为了 '\0' 结尾

    // 小写转大写
    for (int i = 0; i < strlen(user_answer); i++)
    {
        if (user_answer[i] >= 'a' && user_answer[i] <= 'z')
        {
            upper_case_answer[i] = user_answer[i] - 32;
        }
        else
        {
            upper_case_answer[i] = user_answer[i];
        }
    }
    upper_case_answer[strlen(user_answer)] = '\0'; // 确保字符串正确结束

    if (strcmp(correct_answer, upper_case_answer) == 0)
    {
        printf("回答正确！\n");
    }
    else
    {
        printf("回答错误！正确答案是：%s\n", correct_answer);
    }
}

/**
 * 主函数
 */
int main()
{
    // 初始化随机数生成器
    srand(time(NULL));

    // 加载题库
    cJSON *question_bank = load_question_bank("./QuestionBank/ch16.json");
    if (question_bank == NULL)
    {
        printf("题库加载失败。\n");
        return 1;
    }

    printf("题库加载成功\n");
    printf("请选择你要多少道题目：");
    int count;
    scanf("%d", &count);
    // 随机选择并展示题目
    int questions_to_display = count;
    int question_bank_size = cJSON_GetArraySize(question_bank);
    for (int i = 0; i < questions_to_display; i++)
    {
        int index = rand() % question_bank_size;
        cJSON *question = cJSON_GetArrayItem(question_bank, index);
        display_question_and_get_answer(question);
    }

    // 清理资源
    cJSON_Delete(question_bank);

    return 0;
}