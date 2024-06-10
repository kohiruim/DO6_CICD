CC = gcc
CFLAGS = -Wall -Werror -Wextra

all: s21_cat

s21_cat: s21_cat.o
	$(CC)  s21_cat.o -o s21_cat

s21_cat.o: s21_cat.c
	$(CC)  -c s21_cat.c -o s21_cat.o

test: all
	bash t_cat.sh

check:
	cp ../../materials/linters/.clang-format .
	clang-format -n --Werror *.c *.h

clean:
	rm -rf s21_cat
	rm -rf s21_cat.o

rebuild: clean all
