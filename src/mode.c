#include "mode.h"
#include "manage.h"
#include "question.h"
#include "ui.h"
#include "exercise.h"
#include "file_utils.h"
#include "student.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "cJSON.h"

#define QUESTION_BANK_PATH "./QuestionBank"
#define PORT 8888
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

/**
 * @brief 运行TUI模式
 */
void run_tui_mode()
{
    read_question_bank(QUESTION_BANK_PATH);
    printf("\033[1m请输入题库名:\033[0m\n");
    char full_path[512]; // 假设这个大小足够存储 QUESTION_BANK_PATH 和 filename 的连接
    char filename[256];
    scanf("%s", filename);
    strcpy(full_path, QUESTION_BANK_PATH); // 首先复制 QUESTION_BANK_PATH 到缓冲区
    strcat(full_path, "/");                // 然后连接 /
    strcat(full_path, filename);           // 然后连接 filename
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
    printf("\033[32m正确答题数：%d\n\033[0m", correct_count); // 绿色
    printf("\033[34m总题数：%d\n\033[0m", question_count);             // 蓝色
    double final_score = score * correct_count / question_count;
    printf("\033[35m最终得分：%.2f\n\033[0m", final_score); // 紫色
    cJSON_Delete(question_bank);
}

/**
 * @brief 运行学生模式
 */
void run_manage_mode()
{
    int model; // model用于接收登陆身份
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
        // 已经有数据，直接使用
    }
    else
    {
        // 错误：不是数组
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

/**
 * @brief 运行练习管理模式
 */
void run_exercise_management_mode()
{
    setlocale(LC_ALL, "");
    char filename[256];
    printf("请输入题库名：");
    scanf("%s", filename);
    char creator[256];
    printf("请输入作者名：");
    scanf("%s", creator);
    ExerciseDataBase db = {0}; // 初始化数据库
    strcpy(db.ExerciseName, filename);
    strcpy(db.CreatorName, creator);
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