# 定义编译器
CC=gcc

# 检测操作系统
ifeq ($(OS),Windows_NT)
	RM = del /Q
	TARGET = student.exe
	CFLAGS = -I. -lws2_32 -lcjson
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		RM = rm -f
		TARGET = student
		CFLAGS = -I. -lcjson
    endif
    ifeq ($(UNAME_S),Darwin)
        RM = rm -f
        TARGET = student
        CFLAGS = -I. -lcjson
    endif
endif

# 源文件
SRC=student.c

# 生成目标文件的规则
$(TARGET): $(SRC)
	$(CC) -o $@ $^ $(CFLAGS)

# 清理编译生成的文件
clean:
	$(RM) $(TARGET)