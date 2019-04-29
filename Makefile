# makefile for led_driver cpputest

CPPUTEST_HOME = ../../../cpputest

MODULE = circular_buffer
TARGET = $(MODULE)_test

CC      := gcc
CFLAGS    := -g -Wall
CFLAGS  += -std=c99
CFLAGS  += -D CPPUTEST            # 编译测试文件时, 忽略led_driver.c的main函数, led_driver.c的代码中用了宏CPPUTEST

# CPPUTest 是C++写的, 所以用 g++ 来编译 测试文件
CPP     := g++
CPPFLAGS  := -g -Wall -fprofile-arcs -ftest-coverage
CPPFLAGS  += -I$(CPPUTEST_HOME)/include 

LDFLAGS := -L$(CPPUTEST_HOME)/lib -lCppUTest -lgcov --coverage


# 追加的测试程序编译
$(TARGET): $(MODULE).o $(TARGET).o
	$(CPP) -g -o $@ $(MODULE).o $(TARGET).o $(LDFLAGS)

$(TARGET).o: $(MODULE).h $(TARGET).c
	$(CPP) -g -c -o $@ $(TARGET).c $(CPPFLAGS)

$(MODULE).o: $(MODULE).h $(MODULE).c
	$(CC) -g -c -o $@ $(MODULE).c $(CFLAGS)

.PHONY:	clean
clean:
	@echo "clean..."
	rm -f $(TARGET) $(MODULE)
	rm -f $(MODULE).o $(TARGET).o
	rm -f $(TARGET).gcda $(TARGET).gcno