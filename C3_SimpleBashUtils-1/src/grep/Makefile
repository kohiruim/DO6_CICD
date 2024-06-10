CC = gcc
CFLAGS = -Wall -Werror -Wextra

all: s21_grep

s21_grep: s21_grep.o
	$(CC)  s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c
	$(CC)  -c s21_grep.c -o s21_grep.o

test: all
	bash test.sh

check:
	cp ../../materials/linters/.clang-format .     
	clang-format -n --Werror *.c *.h

clean:
	rm -rf s21_grep
	rm -rf s21_grep.o

rebuild: clean all
