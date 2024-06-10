#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

struct flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  char *file;
};

void init(struct flags *fl);
int parse(int argc, char **argv, struct flags *fl);
int s21_cat(struct flags *fl, char **argv);
char **shift(int argc, char **argv);
void print(struct flags *fl, char c);

#endif  // SRC_CAT_S21_CAT_H_
