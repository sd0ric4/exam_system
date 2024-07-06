# 自动生成的Makefile
CC=gcc
RM=$RM
TARGET=$TARGET
CFLAGS=$CFLAGS
SRC=student.c

$(TARGET): $(SRC)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	$(RM) $(TARGET)

# 添加check目标
check:
	@echo "Running tests..."
	./$(TARGET)
    # 例如: ./test_student
