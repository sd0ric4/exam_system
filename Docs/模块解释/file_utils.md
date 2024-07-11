# file_utils

## 头文件
```c
#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "cJSON.h"

cJSON *load_question_bank(const char *filename);

#endif // FILE_UTILS_H
```

#### 头文件说明

头文件 `file_utils.h` 定义了一个函数 `load_question_bank`，用于加载题库文件。

## 源文件
[`file_utils.c`](../../src/file_utils.c)

### 函数定义
#### load_question_bank
```c
/**
 * @brief 从文件中加载题库
 * @param filename 文件名
 * @return 题库的 cJSON 对象
 */
cJSON *load_question_bank(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "无法打开文件: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(length + 1);
    if (data == NULL) {
        fprintf(stderr, "分配内存时出错\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fread(data, 1, length, file);
    fclose(file);

    data[length] = '\0';

    cJSON *json = cJSON_Parse(data);
    if (json == NULL) {
        fprintf(stderr, "解析 JSON 数据时出错\n");
        free(data);
        exit(EXIT_FAILURE);
    }

    free(data);
    return json;
}
```

#### 函数说明

函数 `load_question_bank` 从文件中加载题库，返回一个 cJSON 对象。

#### 函数算法实现解析

1. 打开文件，如果文件打开失败，输出错误信息并退出程序。
2. 获取文件长度，分配内存，读取文件内容到内存中。
3. 关闭文件，将内存中的数据以字符串形式传递给 cJSON_Parse 函数。
4. 解析 JSON 数据，如果解析失败，输出错误信息并退出程序。
5. 释放内存，返回 cJSON 对象。