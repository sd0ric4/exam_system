# question

## 头文件
```c
#ifndef QUESTION_H
#define QUESTION_H

#include "cJSON.h"

#define MAX_DISPLAY 5 // 最多显示5个文件名

typedef struct {
    char *title;
    char *options[4];
    char *correctAnswer;
    char *type;
    int n_options;
} Question;

void shuffle_array(cJSON **array, int n);
cJSON *select_questions_randomly(int count, cJSON *questions);
Question parse_question_from_cjson(const cJSON *questionItem);
void free_question(Question *question);

#endif // QUESTION_H
```


#### 头文件说明

头文件 `question.h` 定义了一些函数和结构体，用于处理题目。

##### 宏定义说明

`MAX_DISPLAY` 宏定义了最多显示的文件名数量。

##### 结构体说明

`Question` 结构体:
| 成员 | 类型 | 说明 |
| --- | --- | --- |
| title | char * | 题目 |
| options | char * | 选项 |
| correctAnswer | char * | 正确答案 |
| type | char * | 题目类型 |
| n_options | int | 选项个数 |


## 源文件

[`question.c`](../../src/question.c)

### 函数定义

#### shuffle_array
```c
/**
 * @brief 打乱题目数组顺序
 * @param array 题目数组
 * @param n 题目数量
 */
void shuffle_array(cJSON **array, int n) {
    if (array == NULL || n <= 0) {
        fprintf(stderr, "打乱数组时出错\n");
        return;
    }

    if (n > 1) {
        srand(time(NULL));
        for (int i = 0; i < n - 1; i++) {
            int j = i + rand() / (RAND_MAX / (n - i) + 1);
            cJSON *temp = array[j];
            array[j] = array[i];
            array[i] = temp;
        }
    }
}
```

#### 函数说明

函数 `shuffle_array` 用于打乱题目数组的顺序。

#### 函数算法实现解析

1. 检查数组是否为空或数量是否小于等于0，如果是则输出错误信息并返回。
2. 使用随机种子初始化随机数生成器。
3. 通过 Fisher-Yates 洗牌算法打乱数组中的元素顺序。

---

#### select_questions_randomly
```c
/**
 * @brief 从题库中随机选择题目
 * @param count 题目数量
 * @param questions 题库
 * @return 随机选择的题目
 */
cJSON *select_questions_randomly(int count, cJSON *questions) {
    cJSON *selected_questions = cJSON_CreateArray();
    int n_questions = cJSON_GetArraySize(questions);
    cJSON **questions_array = (cJSON **)malloc(n_questions * sizeof(cJSON *));

    if (questions_array == NULL) {
        fprintf(stderr, "分配内存时出错\n");
        exit(EXIT_FAILURE);
    }
    if (count > n_questions) {
        count = n_questions;
    }

    for (int i = 0; i < n_questions; ++i) {
        questions_array[i] = cJSON_GetArrayItem(questions, i);
    }

    shuffle_array(questions_array, n_questions);

    for (int i = 0; i < count; ++i) {
        cJSON_AddItemToArray(selected_questions, cJSON_Duplicate(questions_array[i], 1));
    }

    free(questions_array);
    return selected_questions;
}
```

#### 函数说明

函数 `select_questions_randomly` 从题库中随机选择指定数量的题目。

#### 函数算法实现解析

1. 创建一个空的 cJSON 数组来存放选中的题目。
2. 获取题库中的题目数量，分配内存存放题目指针数组。
3. 如果内存分配失败，输出错误信息并退出程序。
4. 如果需要选择的题目数量大于题库中的数量，调整选择数量为题库的数量。
5. 将题库中的题目指针存入数组。
6. 调用 `shuffle_array` 函数打乱题目数组。
7. 从打乱后的数组中选择指定数量的题目，并添加到 `selected_questions` 数组中。
8. 释放题目指针数组内存，返回选中的题目数组。

---

#### parse_question_from_cjson
```c
/**
 * @brief 从 cJSON 对象中解析题目
 * @param questionItem 题目的 cJSON 对象
 * @return 解析出的题目
 */
Question parse_question_from_cjson(const cJSON *questionItem) {
    Question question;
    const cJSON *title = cJSON_GetObjectItem(questionItem, "title");
    const cJSON *options = cJSON_GetObjectItem(questionItem, "options");
    const cJSON *answer = cJSON_GetObjectItem(questionItem, "correctAnswer");
    const cJSON *type = cJSON_GetObjectItem(questionItem, "type");
    const cJSON *optionItem;

    if (!cJSON_IsString(title) || !cJSON_IsArray(options) || !cJSON_IsString(answer) || !cJSON_IsString(type)) {
        fprintf(stderr, "解析题目时出错\n");
        exit(EXIT_FAILURE);
    }

    question.title = strdup(title->valuestring);
    question.correctAnswer = strdup(answer->valuestring);
    question.type = strdup(type->valuestring);
    question.n_options = cJSON_GetArraySize(options);

    for (int i = 0; i < question.n_options; ++i) {
        optionItem = cJSON_GetArrayItem(options, i);
        question.options[i] = strdup(optionItem->valuestring);
    }

    return question;
}
```

#### 函数说明

函数 `parse_question_from_cjson` 从 cJSON 对象中解析题目信息，返回一个 `Question` 结构体。

#### 函数算法实现解析

1. 获取 cJSON 对象中的各个字段（标题、选项、正确答案和类型）。
2. 检查每个字段的类型是否合法，如果不合法则输出错误信息并退出程序。
3. 使用 `strdup` 函数复制字符串，分配内存存放题目信息。
4. 获取选项数量，并逐个复制选项内容到 `Question` 结构体的选项数组中。
5. 返回解析出的 `Question` 结构体。

---

#### free_question
```c
/**
 * @brief 释放题目的内存
 * @param question 题目
 */
void free_question(Question *question) {
    free(question->title);
    free(question->correctAnswer);
    free(question->type);
    for (int i = 0; i < question->n_options; ++i) {
        free(question->options[i]);
    }
}
```

#### 函数说明

函数 `free_question` 释放 `Question` 结构体中分配的内存。

#### 函数算法实现解析

1. 释放 `Question` 结构体中各个字符串字段的内存。
2. 逐个释放选项数组中每个选项的内存。