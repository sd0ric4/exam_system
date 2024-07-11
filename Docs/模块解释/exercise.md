# exercise

## 头文件

```c
#ifndef EXERCISE_H
#define EXERCISE_H

#include "cJSON.h"

#define UpperLimit 100 //试题题库默认题目上限

typedef struct 试题 {
    char chapter[32]; // 章节
    char question[256]; // 题干
    char choices[4][128]; // 四个选项
    char answer; // 标准答案，A/B/C/D
    char type[10]; // 题型
} Exercise;

typedef struct 试题题库 {
    char ExerciseName[32];
    char CreatorName[16];
    Exercise SetOfExercise[UpperLimit];
    int ExerciseSum; // 当前题目总数
} ExerciseDataBase;

// 函数声明
void initialDB(ExerciseDataBase db);
void addExercise(ExerciseDataBase *db, Exercise exercise);
void deleteExercise(ExerciseDataBase *db, int index);
void modifyExercise(ExerciseDataBase *db, int index, Exercise exercise);
void saveExercises(ExerciseDataBase db);
void queryExercise(const ExerciseDataBase *db, int index);
void browseExercises(const ExerciseDataBase *db);
void generateExercise(ExerciseDataBase *db, int numQuestions);
void generateJsonFile(const ExerciseDataBase *db, const char *filename);

#endif // EXERCISE_H
```

#### 头文件说明

头文件 `exercise.h` 定义了一些函数和结构体，用于处理试题。

##### 宏定义说明

`UpperLimit` 宏定义了试题题库默认题目上限。

##### 结构体说明

`Exercise` 结构体:
| 成员 | 类型 | 说明 |
| --- | --- | --- |
| chapter | char[32] | 章节 |
| question | char[256] | 题干 |
| choices | char[4][128] | 四个选项 |
| answer | char | 标准答案，A/B/C/D |
| type | char[10] | 题型 |

`ExerciseDataBase` 结构体:
| 成员 | 类型 | 说明 |
| --- | --- | --- |
| ExerciseName | char[32] | 题库名称 |
| CreatorName | char[16] | 创建者 |
| SetOfExercise | Exercise[UpperLimit] | 试题集合 |
| ExerciseSum | int | 当前题目总数 |

## 源文件

[`exercise.c`](../../src/exercise.c)

### 函数定义

#### initialDB
```c
/**
 * @brief 初始化数据库
 * @param db 数据库
 */
void initialDB(ExerciseDataBase db) {
    // 复制ExerciseDataBase的基本信息
    strcpy(TrueDB.ExerciseName, db.ExerciseName);
    strcpy(TrueDB.CreatorName, db.CreatorName);
    TrueDB.ExerciseSum = db.ExerciseSum;

    // 深拷贝Exercise结构体数组
    for (int i = 0; i < db.ExerciseSum; i++) {
        strcpy(TrueDB.SetOfExercise[i].chapter, db.SetOfExercise[i].chapter);
        strcpy(TrueDB.SetOfExercise[i].question, db.SetOfExercise[i].question);
        for (int j = 0; j < 4; j++) {
            strcpy(TrueDB.SetOfExercise[i].choices[j], db.SetOfExercise[i].choices[j]);
        }
        TrueDB.SetOfExercise[i].answer = db.SetOfExercise[i].answer;
        strcpy(TrueDB.SetOfExercise[i].type, db.SetOfExercise[i].type);
    }
}
```

#### 函数说明

函数 `initialDB` 用于初始化数据库，将传入的 `ExerciseDataBase` 数据库内容深拷贝到全局的 `TrueDB` 数据库中。

#### 函数算法实现解析

1. 复制传入数据库的基本信息到 `TrueDB`。
2. 深拷贝传入数据库中的每个 `Exercise` 结构体到 `TrueDB` 的相应位置。

---

#### addExercise
```c
/**
 * @brief 添加试题
 * @param db 数据库
 * @param exercise 新试题
 */
void addExercise(ExerciseDataBase *db, Exercise exercise) {
    if (db->ExerciseSum >= UpperLimit) {
        printf("题库数目已达上限\n");
        return;
    }

    strcpy(db->SetOfExercise[db->ExerciseSum].chapter, exercise.chapter);
    strcpy(db->SetOfExercise[db->ExerciseSum].question, exercise.question);
    strcpy(db->SetOfExercise[db->ExerciseSum].type, exercise.type);

    for (int i = 0; i < 4; i++) {
        strcpy(db->SetOfExercise[db->ExerciseSum].choices[i], exercise.choices[i]);
    }

    db->SetOfExercise[db->ExerciseSum].answer = exercise.answer;
    db->ExerciseSum++;
}
```

#### 函数说明

函数 `addExercise` 用于向数据库添加一个新的试题。

#### 函数算法实现解析

1. 检查数据库是否已达上限，如果是则输出提示信息并返回。
2. 将新试题的信息复制到数据库的当前末尾位置。
3. 增加数据库中的试题数量。

---

#### deleteExercise
```c
/**
 * @brief 删除试题
 * @param db 数据库
 * @param index 题号
 */
void deleteExercise(ExerciseDataBase *db, int index) {
    if (index < 0 || index >= db->ExerciseSum) {
        printf("题号错误！删除失败\n");
        return;
    }
    for (int i = index; i < db->ExerciseSum - 1; i++) {
        db->SetOfExercise[i] = db->SetOfExercise[i + 1];
    }
    db->ExerciseSum--;
}
```

#### 函数说明

函数 `deleteExercise` 用于删除数据库中的一个试题。

#### 函数算法实现解析

1. 检查题号是否有效，如果无效则输出错误信息并返回。
2. 将后续试题前移覆盖被删除的试题。
3. 减少数据库中的试题数量。

---

#### modifyExercise
```c
/**
 * @brief 修改试题
 * @param db 数据库
 * @param index 题号
 * @param exercise 修改后的试题
 */
void modifyExercise(ExerciseDataBase *db, int index, Exercise exercise) {
    if (index < 0 || index >= db->ExerciseSum) {
        printf("题号错误！修改失败\n");
        return;
    }

    strcpy(db->SetOfExercise[index].chapter, exercise.chapter);
    strcpy(db->SetOfExercise[index].question, exercise.question);
    strcpy(db->SetOfExercise[index].type, exercise.type);

    for (int i = 0; i < 4; i++) {
        strcpy(db->SetOfExercise[index].choices[i], exercise.choices[i]);
    }

    db->SetOfExercise[index].answer = exercise.answer;
}
```

#### 函数说明

函数 `modifyExercise` 用于修改数据库中的一个试题。

#### 函数算法实现解析

1. 检查题号是否有效，如果无效则输出错误信息并返回。
2. 将修改后的试题信息复制到数据库的对应位置。

---

#### saveExercises
```c
/**
 * @brief 保存试题
 * @param db 数据库
 */
void saveExercises(ExerciseDataBase db) {
    initialDB(db);
    printf("题库已保存\n");
}
```

#### 函数说明

函数 `saveExercises` 用于保存数据库内容到全局数据库 `TrueDB` 中。

#### 函数算法实现解析

1. 调用 `initialDB` 函数将传入的数据库内容保存到全局数据库 `TrueDB` 中。
2. 输出保存成功的信息。

---

#### queryExercise
```c
/**
 * @brief 查询试题
 * @param db 数据库
 * @param index 题号
 */
void queryExercise(const ExerciseDataBase *db, int index) {
    if (index < 0 || index >= db->ExerciseSum) {
        printf("题号错误！查询失败\n");
        return;
    }

    printf("章节: %s\n", db->SetOfExercise[index].chapter);
    printf("第%d题: %s\n", index + 1, db->SetOfExercise[index].question);
    for (int i = 0; i < 4; i++) {
        printf("%c: %s\n", 'A' + i, db->SetOfExercise[index].choices[i]);
    }
    printf("正确答案: %c\n", db->SetOfExercise[index].answer);
}
```

#### 函数说明

函数 `queryExercise` 用于查询数据库中的一个试题并输出其信息。

#### 函数算法实现解析

1. 检查题号是否有效，如果无效则输出错误信息并返回。
2. 输出试题的章节、问题、选项和正确答案。

---

#### browseExercises
```c
/**
 * @brief 浏览试题
 * @param db 数据库
 */
void browseExercises(const ExerciseDataBase *db) {
    for (int i = 0; i < db->ExerciseSum; i++) {
        queryExercise(db, i);
    }
}
```

#### 函数说明

函数 `browseExercises` 用于浏览数据库中的所有试题。

#### 函数算法实现解析

1. 循环遍历数据库中的所有试题，调用 `queryExercise` 函数输出每个试题的信息。

---

#### randomInt
```c
/**
 * @brief 生成随机整数
 * @param min 最小值
 * @param max 最大值
 * @return 随机整数
 */
int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}
```

#### 函数说明

函数 `randomInt` 用于生成指定范围内的随机整数。

#### 函数算法实现解析

1. 使用 `rand` 函数生成随机数并调整到指定范围内。

---

#### contains
```c
/**
 * @brief 判断数组中是否包含某个值
 * @param array 数组
 * @param size 数组大小
 * @param value 值
 * @return 如果包含返回1，否则返回0
 */
int contains(int array[], int size, int value) {
    for (int i = 0; i < size; i++) {
        if (array[i] == value) return 1;
    }
    return 0;
}
```

#### 函数说明

函数 `contains` 用于判断数组中是否包含某个值。

#### 函数算法实现解析

1. 遍历数组，检查每个元素是否等于指定值，如果是则返回1，否则返回0。

---

#### writeQuestionsToFile
```c
/**
 * @brief 将题目写入文件
 * @param db 数据库
 * @param filename 文件名
 * @param numQuestions 题目数
 */
void writeQuestionsToFile(const ExerciseDataBase *db, const char *filename, int numQuestions) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("打开文件失败\n");
        return;
    }

    srand(time(NULL)); // 初始化随机种子
    int selected[numQuestions];
    for (int i = 0; i < numQuestions; i++) {
        int nextIndex;
        do {
            nextIndex = randomInt(0, db->ExerciseSum - 1);
        } while (nextIndex < 0 || nextIndex >= db->ExerciseSum || contains(selected, numQuestions, nextIndex));

        selected[i] = nextIndex;
        fprintf(file, "第%d题: %s\n", i + 1, db->SetOfExercise[nextIndex].question);
        for (int

 j = 0; j < 4; j++) {
            fprintf(file, "%c: %s\n", 'A' + j, db->SetOfExercise[nextIndex].choices[j]);
        }
    }

    fclose(file);
}
```

#### 函数说明

函数 `writeQuestionsToFile` 用于将指定数量的随机题目写入文件。

#### 函数算法实现解析

1. 打开文件，如果打开失败则输出错误信息并返回。
2. 初始化随机种子。
3. 随机选择指定数量的题目并写入文件。
4. 关闭文件。

---

#### writeAnswersToFile
```c
/**
 * @brief 将答案写入文件
 * @param db 数据库
 * @param filename 文件名
 * @param numQuestions 题目数
 * @param selected 选中的题目
 */
void writeAnswersToFile(const ExerciseDataBase *db, const char *filename, int numQuestions, int selected[]) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("打开文件失败\n");
        return;
    }

    for (int i = 0; i < numQuestions; i++) {
        fprintf(file, "第%d题答案: %c\n", i + 1, db->SetOfExercise[selected[i]].answer);
    }

    fclose(file);
}
```

#### 函数说明

函数 `writeAnswersToFile` 用于将指定数量的题目答案写入文件。

#### 函数算法实现解析

1. 打开文件，如果打开失败则输出错误信息并返回。
2. 将指定数量的题目答案写入文件。
3. 关闭文件。

---

#### generateJsonFile
```c
/**
 * @brief 生成 JSON 文件
 * @param db 数据库
 * @param filename 文件名
 */
void generateJsonFile(const ExerciseDataBase *db, const char *filename) {
    cJSON *jsonArray = cJSON_CreateArray();

    for (int i = 0; i < db->ExerciseSum; i++) {
        cJSON *jsonQuestion = cJSON_CreateObject();
        cJSON_AddStringToObject(jsonQuestion, "chapter", db->SetOfExercise[i].chapter);
        cJSON_AddStringToObject(jsonQuestion, "type", db->SetOfExercise[i].type);
        cJSON_AddStringToObject(jsonQuestion, "title", db->SetOfExercise[i].question);

        cJSON *jsonOptions = cJSON_CreateArray();
        for (int j = 0; j < 4; j++) {
            cJSON *jsonOption = cJSON_CreateObject();
            char optionLabel[2] = { 'A' + j, '\0' }; // 正确处理选项标签
            cJSON_AddStringToObject(jsonOption, optionLabel, db->SetOfExercise[i].choices[j]);
            cJSON_AddItemToArray(jsonOptions, jsonOption);
        }
        cJSON_AddItemToObject(jsonQuestion, "options", jsonOptions);
        char correctAnswer[2] = { db->SetOfExercise[i].answer, '\0' }; // 正确处理正确答案
        cJSON_AddStringToObject(jsonQuestion, "correctAnswer", correctAnswer);

        cJSON_AddItemToArray(jsonArray, jsonQuestion);
    }

    char *jsonString = cJSON_Print(jsonArray);
    FILE *file = fopen(filename, "w");
    if (file) {
        fprintf(file, "%s", jsonString);
        fclose(file);
    } else {
        printf("无法打开文件\n");
    }

    cJSON_Delete(jsonArray);
    free(jsonString);
}
```

#### 函数说明

函数 `generateJsonFile` 用于生成包含所有试题的 JSON 文件。

#### 函数算法实现解析

1. 创建一个 cJSON 数组对象。
2. 遍历数据库中的每个试题，创建相应的 cJSON 对象并添加到数组中。
3. 将 cJSON 数组对象转换为字符串并写入文件。
4. 释放 cJSON 对象和字符串内存。

---

#### generateExercise
```c
/**
 * @brief 生成试卷
 * @param db 数据库
 * @param numQuestions 题目数
 */
void generateExercise(ExerciseDataBase *db, int numQuestions) {
    if (numQuestions > db->ExerciseSum) {
        printf("指定的题目数超过题库中的题目数\n");
        return;
    }

    char ExerciseFilename[32];
    char AnswerFilename[32];
    sprintf(ExerciseFilename, "test_Exercise_%d.txt", numQuestions);
    sprintf(AnswerFilename, "test_Answer_%d.txt", numQuestions);

    writeQuestionsToFile(db, ExerciseFilename, numQuestions);
    int selected[numQuestions];
    for (int i = 0; i < numQuestions; i++) {
        selected[i] = -1; // 初始化为无效值
    }
    writeAnswersToFile(db, AnswerFilename, numQuestions, selected);
    printf("试卷文件名为: %s\n", ExerciseFilename);
    printf("答案文件名为: %s\n", AnswerFilename);
    printf("试卷和答案已分别生成并保存到文件\n");

    // 生成 JSON 文件,到QUESTION_BANK_PATH目录下
    char JsonFilename[32];

    snprintf(JsonFilename, sizeof(JsonFilename), "%s/%s.json", QUESTION_BANK_PATH, db->ExerciseName);
    generateJsonFile(db, JsonFilename);
    printf("试卷 JSON 文件名为: %s\n", JsonFilename);
}
```

#### 函数说明

函数 `generateExercise` 用于生成包含指定数量题目的试卷和答案，并保存到文件。

#### 函数算法实现解析

1. 检查指定题目数是否超过题库中的题目数，如果是则输出错误信息并返回。
2. 生成试卷和答案文件名。
3. 调用 `writeQuestionsToFile` 函数生成试卷文件。
4. 初始化选中的题目索引数组。
5. 调用 `writeAnswersToFile` 函数生成答案文件。
6. 输出文件名信息。
7. 调用 `generateJsonFile` 函数生成包含试卷的 JSON 文件。