#include "file_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

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
