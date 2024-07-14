OBJS_FOLDER = obj
INCS_FOLDER = inc
SRCS_FOLDER = src
SRCS := $(wildcard src/main.c src/function_A.c src/function_B.c)
OBJS := $(patsubst src/%.c, $(OBJS_FOLDER)/%.o, $(SRCS))
CC = gcc
CFLAGS = -Wall
EXEC = lab02

.PHONY: all clean uninstall reinstall

all: $(OBJS_FOLDER) $(EXEC)

$(OBJS_FOLDER):
	mkdir $(OBJS_FOLDER)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $^ -lm

$(OBJS_FOLDER)/%.o:$(SRCS_FOLDER)/%.c
	$(CC) $(CFLAGS) -g -c $< -o $@ -I $(INCS_FOLDER)

clean:
	rm -rf $(OBJS_FOLDER)

uninstall: clean
	rm -f $(EXEC)

reinstall: uninstall all

