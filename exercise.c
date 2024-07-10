#include "exercise.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cjson/cJSON.h>

// 真实数据库TrueDB
static ExerciseDataBase TrueDB;

//初始化数据库
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

// 添加试题
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

// 删除试题
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

// 修改试题
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

// 保存试题（增 删 改 需要保存）
void saveExercises(ExerciseDataBase db) {
    initialDB(db);
    printf("题库已保存\n");
}

// 查询试题
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

// 浏览所有试题
void browseExercises(const ExerciseDataBase *db) {
    for (int i = 0; i < db->ExerciseSum; i++) {
        queryExercise(db, i);
    }
}

// 生成随机整数，用于随机选择题目
int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

// 检查数组中是否已包含某个值
int contains(int array[], int size, int value) {
    for (int i = 0; i < size; i++) {
        if (array[i] == value) return 1;
    }
    return 0;
}

// 将试卷输出到文件
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
        for (int j = 0; j < 4; j++) {
            fprintf(file, "%c: %s\n", 'A' + j, db->SetOfExercise[nextIndex].choices[j]);
        }
    }

    fclose(file);
}

// 将答案输出到文件
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

// 生成 JSON 文件
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

// 主组卷功能
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

    // 生成 JSON 文件
    char JsonFilename[32];
    sprintf(JsonFilename, "./QuestionBank/test_Exercise_%d.json", numQuestions);
    generateJsonFile(db, JsonFilename);
    printf("试卷 JSON 文件名为: %s\n", JsonFilename);
}
