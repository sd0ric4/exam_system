# mode

## 头文件

```c
#ifndef MODE_H
#define MODE_H

void run_backend_mode();
void run_better_tui_mode();
void run_tui_mode();
void run_manage_mode();
void run_exercise_management_mode();

#endif // MODE_H
```

#### 头文件说明

头文件 `mode.h` 定义了一些函数，用于不同模式的运行。

## 源文件

[`mode.c`](../../src/mode.c)

### 函数定义

#### run_backend_mode
```c
/**
 * @brief 运行后端模式
 */
void run_backend_mode()
{
    struct MHD_Daemon *daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL, &handle_request, NULL, MHD_OPTION_END);
    if (NULL == daemon)
    {
        printf("Failed to start HTTP server.\n");
        exit(1);
    }

    printf("HTTP server is running on port %d\n", PORT);
    getchar();
    MHD_stop_daemon(daemon);
}
```

#### 函数说明

函数 `run_backend_mode` 用于启动后端HTTP服务器。

#### 函数算法实现解析

1. 启动HTTP服务器，监听指定端口。
2. 如果服务器启动失败，输出错误信息并退出程序。
3. 成功启动后，等待用户输入以停止服务器。

---

#### run_better_tui_mode
```c
/**
 * @brief 运行更好的TUI模式
 */
void run_better_tui_mode()
{
    correct_count = 0;
    char *selected_file = navigate_question_bank(QUESTION_BANK_PATH);
    if (selected_file == NULL)
    {
        fprintf(stderr, "题库选择失败\n");
        exit(1);
    }

    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s/%s", QUESTION_BANK_PATH, selected_file);
    cJSON *question_bank = load_question_bank(filepath);
    int count = input_count_page();
    cJSON *selected_questions = select_questions_randomly(count, question_bank);
    const cJSON *questionItem = NULL;

    cJSON_ArrayForEach(questionItem, selected_questions)
    {
        Question question = parse_question_from_cjson(questionItem);
        process_question(question);
        free_question(&question);
    }
    score = INIT_SCORE * correct_count / count;
    display_final_result(score, count, correct_count);
}
```

#### 函数说明

函数 `run_better_tui_mode` 用于运行更好的TUI模式。

#### 函数算法实现解析

1. 初始化正确答题计数。
2. 浏览并选择题库文件。
3. 加载题库文件并选择随机题目。
4. 显示并处理每个题目，更新正确答题计数。
5. 显示最终结果，包括得分、题目总数和正确答题数。

---

#### run_tui_mode
```c
/**
 * @brief 运行TUI模式
 */
void run_tui_mode()
{
    read_question_bank(QUESTION_BANK_PATH);
    printf("\033[1m请输入题库名:\033[0m\n");
    char full_path[512];
    char filename[256];
    scanf("%s", filename);
    strcpy(full_path, QUESTION_BANK_PATH);
    strcat(full_path, "/");
    strcat(full_path, filename);
    printf("您选择的题库路径是:\033[34m%s\033[0m\n", full_path);
    choose_question_bank(full_path);
    printf("\033[32m题库加载成功\033[0m\n");
    double score = INIT_SCORE;
    printf("\033[1m请选择你要多少道题目:\033[0m");
    scanf("%d", &question_count);

    int questions_to_display = question_count;
    int question_bank_size = cJSON_GetArraySize(question_bank);
    correct_count = 0;
    for (int i = 0; i < questions_to_display; i++)
    {
        int index = rand() % question_bank_size;
        cJSON *question = cJSON_GetArrayItem(question_bank, index);
        if (display_question_and_get_answer(question))
        {
            correct_count++;
        }
    }
    printf("\033[32m正确答题数：%d\n\033[0m", correct_count);
    printf("\033[34m总题数：%d\n\033[0m", question_count);
    double final_score = score * correct_count / question_count;
    printf("\033[35m最终得分：%.2f\n\033[0m", final_score);
    cJSON_Delete(question_bank);
}
```

#### 函数说明

函数 `run_tui_mode` 用于运行TUI模式。

#### 函数算法实现解析

1. 读取题库目录并提示用户输入题库名。
2. 加载选择的题库文件。
3. 提示用户输入题目数量。
4. 随机选择题目并显示，获取用户答案。
5. 计算并显示最终得分、正确答题数和总题数。

---

#### run_manage_mode
```c
/**
 * @brief 运行学生模式
 */
void run_manage_mode()
{
    int model;
    STU *L = NULL;
    L = ListInit(L);
    char *filename = "student.json";
    cJSON *root = read_students_from_file(filename);
    if (!root)
    {
        root = cJSON_CreateArray();
    }
    else if (cJSON_IsArray(root))
    {
    }
    else
    {
        cJSON_Delete(root);
        root = cJSON_CreateArray();
    }

    int rootNum = cJSON_GetArraySize(root);
    read_student_from_json(root, rootNum, L);

    do
    {
        mainMenu();
        scanf("%d", &model);
        getchar();
        system("clear");
        switch (model)
        {
        case 1:
            teachermodel(root, L);
            break;
        case 2:
            studentmodel(root, L);
            break;
        case 0:
            printf("即将退出考试系统.....\n");
            save_students_to_file(saveRoot(root, L));
            exit(0);
        default:
            printf("您输入的选项有误，请重新输入！");
            break;
        }
    } while (model);
}
```

#### 函数说明

函数 `run_manage_mode` 用于运行学生管理模式。

#### 函数算法实现解析

1. 初始化学生链表并从文件读取学生信息。
2. 显示主菜单并根据用户选择进入相应模式。
3. 在教师模式下，可以管理学生信息。
4. 在学生模式下，可以进行答题和修改密码。
5. 退出系统时保存学生信息到文件。

---

#### run_exercise_management_mode
```c
/**
 * @brief 运行练习管理模式
 */
void run_exercise_management_mode()
{
    setlocale(LC_ALL, "");

    ExerciseDataBase db = {0};
    strcpy(db.ExerciseName, "ExampleDB");
    strcpy(db.CreatorName, "Creator");
    exerciseMenu();
    int choice;
    while (1)
    {
        printf("请选择操作：");
        scanf("%d", &choice);
        if (choice == 7)
            break;

        int index;
        Exercise exercise;
        switch (choice)
        {
        case 1:
            printf("输入章节：");
            scanf(" %[^\n]", exercise.chapter);
            printf("输入题型（单选题/多选题/判断题）：");
            scanf(" %[^\n]", exercise.type);
            printf("输入题干：");
            scanf(" %[^\n]", exercise.question);
            for (int i = 0; i < 4; i++)
            {
                printf("输入选项 %c：", 'A' + i);
                scanf(" %[^\n]", exercise.choices[i]);
            }
            printf("输入标准答案（A/B/C/D）：");
            scanf(" %c", &exercise.answer);
            addExercise(&db, exercise);
            break;
        case 2:
            printf("输入要删除的题号：");
            scanf("%d", &index);
            deleteExercise(&db, index - 1);
            break;
        case 3:
            printf("输入要修改的题号：");
            scanf("%d", &index);
            printf("输入章节：");
            scanf(" %[^\n]", exercise.chapter);
            printf("输入题干：");
            scanf(" %[^\n]", exercise.question);
            for (int i = 0; i < 4; i++)
            {
                printf("输入选项 %c：", 'A' + i);
                scanf(" %[^\n]", exercise.choices[i]);
            }
            printf("输入标准答案（A/B/C/D）：");
            scanf(" %c", &exercise.answer);
            modifyExercise(&db, index - 1, exercise);
            break;
        case 4:
            printf("输入要查询的题号：");
            scanf("%d", &index);
            queryExercise(&db, index - 1);
            break;
        case 5:
            browseExercises(&db);
            break;
        case 6:
            printf("输入生成试卷的题目数：");
            int numQuestions;
            scanf("%d", &numQuestions);
            generateExercise(&db, numQuestions);
            break;
        default:
            printf("无效选择\n");
        }

        saveExercises(db);
    }
}
```

#### 函数说明

函数 `run_exercise_management_mode` 用于运行练习管理模式。

#### 函数算法实现解析

1. 初始化数据库并显示练习管理菜单。
2. 根据用户选择执行相应的操作，包括添加、删除、修改、查询、浏览和生成试卷。
3. 提示用户输入相应信息并更新数据库。
4. 保存数据库到文件。