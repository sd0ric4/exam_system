# 单项选择题标准化考试系统
## 【任务描述】
设计一个单项选择题的考试系统，可实现自动组卷功能。
## 【功能描述】
### 管理员功能
- 试题管理：每个试题包括题干、4个备选答案、标准答案等信息。可进行试题添加、删除、修改、查询、保存、浏览等操作。每个单选试题5分。
- 组卷功能：指定题目数，生成试卷。将试卷输出到文件，将答案输出到另一个文件。
- 考生管理：对考生信息进行添加、删除、修改、查询、保存、浏览等操作。
- 考生答题情况管理：指定考生，统计其做试卷的总次数，以及每次所得的总分。
### 考生功能
- ☑️练习功能：指定题目数，随机选题。对考生的答案与标准答案进行对比，并最终给出成绩。对错误题目，要能给出正确答案。 
- 考试功能：从题库选择一套试卷，考生完成答题后，系统自动判卷并给出成绩。
### 设计提示
- 用户产生的答题文件，应以用户名+系统时间.txt的形式存储，以便于进行管理。


## 编译运行
```bash
./configure
make
```
## 项目结构
```bash
.
├── build
│   └── Debug
├── configure
├── Docs
│   └── 代码解释.md
├── include
│   ├── cJSON.h
│   ├── exercise.h
│   ├── file_utils.h
│   ├── manage.h
│   ├── microhttpd.h
│   ├── mode.h
│   ├── ncurses.h
│   ├── question.h
│   ├── student.h
│   └── ui.h
├── main
├── Makefile
├── obj
│   ├── exercise.o
│   ├── file_utils.o
│   ├── main.o
│   ├── manage.o
│   ├── mode.o
│   ├── question.o
│   ├── student.o
│   └── ui.o
├── QuestionBank
│   ├── ch10.json
│   ├── ch11.json
│   ├── ch12.json
│   ├── ch13.json
│   ├── ch14.json
│   ├── ch15.json
│   ├── ch16.json
│   ├── ch17.json
│   ├── ch1.json
│   ├── ch2.json
│   ├── ch3.json
│   ├── ch4.json
│   ├── ch5.json
│   ├── ch6.json
│   ├── ch7.json
│   ├── ch8.json
│   ├── ch9.json
│   ├── ExampleDB.json
│   └── test_Exercise_1.json
├── Readme.md
├── src
│   ├── exercise.c
│   ├── file_utils.c
│   ├── main.c
│   ├── manage.c
│   ├── mode.c
│   ├── question.c
│   ├── student.c
│   └── ui.c
└── student.json
```

- `src` 存放源代码
- `include` 存放头文件
- `obj` 存放编译生成的目标文件
- `build` 存放编译生成的可执行文件
- `QuestionBank` 存放题库
- `student.json` 存放学生信息
- `configure` 生成Makefile的脚本
- `Makefile` 用于编译项目
- `main` 可执行文件
- `Docs` 存放文档
- `Readme.md` 项目说明

## 模块解释

### `file_utils` 文件操作模块

[`file_utils` 文件操作模块](Docs/模块解释/file_utils.md)

### `question` 试题模块

[`question` 试题模块](Docs/模块解释/question.md)

### `exercise` 练习模块

[`exercise` 练习模块](Docs/模块解释/exercise.md)



