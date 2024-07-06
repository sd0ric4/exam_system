#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cjson/cJSON.h>

/**
 * 加载题库
 * @param filename 题库文件名
 * @return 题库JSON对象
 */
cJSON *load_question_bank(const char *filename)
{
    // 实现读取文件和解析JSON的代码
    /**
     *   {
    "chapter": "ch16",
    "type": "单选题",
    "title": "【单选题】当今世界正经历百年未有之大变局，国际力量对比深刻变化，下列说法中错误的是（）。",
    "options": [
      { "A": "“西升东降”“西治中乱”" },
      { "B": "多个发展中心在世界各地区逐渐形成" },
      { "C": "发达国家内部矛盾重重、实力相对下降" },
      { "D": "新兴市场国家和发展中国家群体性崛起" }
    ],
    "correctAnswer": "A"
  },
     */
    // 题目格式为上，title为题目，options为题干
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

// 随机选择题目
void select_random_questions(cJSON *questions, int count)
{
    // 实现随机选择题目的代码
    // 随机选择count个题目，每个题目不能重复，然后存到新建的用户名+系统时间.json文件中
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

// 显示题目并接收答案
void display_question_and_get_answer(cJSON *question)
{
    // 实现显示题目和接收答案的代码
}

// 比较答案并记录得分
void compare_answers_and_score(cJSON *question, const char *user_answer)
{
    // 实现比较答案和记录得分的代码
}

int main()
{
    // 初始化随机数生成器
    srand(time(NULL));

    // 加载题库
    cJSON *question_bank = load_question_bank("ch16.json");

    // 练习功能或考试功能的实现
    // ...
    printf("题库加载成功\n");
    // 展示题目
    cJSON *question = cJSON_GetArrayItem(question_bank, 0);
    printf("题目：%s\n", cJSON_GetObjectItem(question, "title")->valuestring);
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
    // 清理资源
    cJSON_Delete(question_bank);

    return 0;
}