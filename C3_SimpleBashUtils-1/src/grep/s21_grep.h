#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

struct flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
};

void init(struct flags *fl);
void open_file(struct flags *fl, char **argv, int file_num, char *find_str,
               char *search_o);
int s21_grep(struct flags *fl, int file_num, char *search, FILE *fp,
             char *filename, char *search_o);
int flag_to_int(char *flags, char flag);
void outputFlagC(struct flags *fl, int count_c, int count_vc, char *filename,
                 int file_num, int flag_l);
void outputFlagL(struct flags *fl, int count_c, int count_vc, char *filename,
                 int file_num, int count_lc);
void *s21_to_lower(const char *str);
void *s21_to_upper(const char *str);
char *convert_for_char(unsigned int num);
void s21_grep_core(struct flags *fl, int count, char *line);
int parse_flag(char **argv, struct flags *fl, int i, char *search);

#endif  // SRC_GREP_S21_GREP_H_
