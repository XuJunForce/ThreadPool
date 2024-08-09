# 指定编译器
CC = gcc

# 编译选项：使用C99标准，并启用所有警告
CFLAGS = -Wall -std=c99 -pthread

# 目标文件名
TARGET = threadpool_test

# 目标：链接生成可执行文件
$(TARGET): threadpool.o main.o
	$(CC) $(CFLAGS) -o $(TARGET) threadpool.o main.o

# 规则：编译 threadpool.c 生成对象文件
threadpool.o: threadpool.c threadpool.h
	$(CC) $(CFLAGS) -c threadpool.c

# 规则：编译 main.c 生成对象文件
main.o: main.c threadpool.h
	$(CC) $(CFLAGS) -c main.c

# 清理：删除编译生成的对象文件和可执行文件
clean:
	rm -f *.o $(TARGET)
