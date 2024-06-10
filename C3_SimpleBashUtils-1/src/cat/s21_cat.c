#include "s21_cat.h"

#include <stdio.h>
#include <string.h>

void init(struct flags *fl) {
  fl->b = 0;
  fl->e = 0;
  fl->n = 0;
  fl->s = 0;
  fl->t = 0;
  fl->v = 0;
}

int main(int argc, char **argv) {
  int error = 0;
  struct flags fl;
  init(&fl);
  if (argc > 1) {
    error += parse(argc, argv, &fl);
    argv = shift(argc, argv);
    error += s21_cat(&fl, argv);
  } else {
    error++;
    fprintf(stderr, "cat: no arguments");
  }
  return error;
}

int parse(int argc, char **argv, struct flags *fl) {
  int error = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == '-') {
      if (strcmp("--number-nonblank", argv[i]) == 0) {
        fl->b = 1;
      } else if (strcmp("--number", argv[i]) == 0) {
        fl->n = 1;
      } else if (strcmp("--squeeze-blank", argv[i]) == 0) {
        fl->s = 1;
      } else {
        error++;
        fprintf(stderr, "cat: illegal option");
      }
    } else if (argv[i][0] == '-') {
      if (strcmp("-b", argv[i]) == 0) {
        fl->b = 1;
      } else if (strcmp("-e", argv[i]) == 0) {
        fl->e = 1;
        fl->v = 1;
      } else if (strcmp("-v", argv[i]) == 0) {
        fl->v = 1;
      } else if (strcmp("-n", argv[i]) == 0) {
        fl->n = 1;
      } else if (strcmp("-s", argv[i]) == 0) {
        fl->s = 1;
      } else if (strcmp("-t", argv[i]) == 0) {
        fl->t = 1;
        fl->v = 1;
      } else if (strcmp("-T", argv[i]) == 0) {
        fl->t = 1;
      } else if (strcmp("-E", argv[i]) == 0) {
        fl->e = 1;
      } else {
        error++;
        fprintf(stderr, "cat: illegal option");
      }
    } else {
      break;
    }
  }
  return error;
}

int s21_cat(struct flags *fl, char **argv) {
  int f_s = 0, error = 0;
  for (int i = 0; argv[i] != 0; i++) {
    char c, last;
    int count = 1, f = 0, s = 0;
    FILE *fp;
    fp = fopen(argv[i], "r");
    if (fp != NULL) {
      while ((c = getc(fp)) != EOF) {
        if (fl->n == 1 && fl->b == 0) {
          if (f == 0 && fl->n == 1) {
            f = 1;
            printf("%6d\t", count);
          } else if (fl->n == 1 && last == '\n' && fl->s == 0) {
            printf("%6d\t", ++count);
          } else if (fl->n == 1 && last == '\n' && fl->s == 1 && f_s == 0) {
            printf("%6d\t", ++count);
          }
        }
        if (fl->b == 1) {
          if (f == 0 && fl->b == 1 && c != '\n') {
            f = 1;
            printf("%6d\t", count++);
          } else if (fl->b == 1 && last == '\n' && c != '\n') {
            printf("%6d\t", count++);
          }
        }

        if (((fl->e == 1 && c != '\n') || fl->e == 0) && fl->s == 0) {
          print(fl, c);
        } else if (fl->s == 1) {
          if (c == '\n' && last == '\n') {
            s++;
            if (s < 2) {
              f_s = 0;
              if (fl->e == 0) {
                print(fl, c);
              }
            } else {
              f_s++;
            }
          } else {
            s = 0;
            f_s = 0;
            if (fl->e == 0 || (fl->e == 1 && c != '\n')) {
              print(fl, c);
            }
          }
        }
        if ((fl->e == 1 && c == '\n' && f_s == 0)) {
          printf("$\n");
        }
        last = c;
      }
      fclose(fp);
    } else {
      error++;
      fprintf(stderr, "cat: No such file or directory");
    }
  }
  return error;
}

void print(struct flags *fl, char c) {
  if (fl->t == 1) {
    if (c != '\t') {
      if (fl->v == 1) {
        if ((c >= 0 && c < 9) || (c > 10 && c < 32)) {
          printf("^%c", c + 64);
        } else if (c == 127) {
          printf("^%c", c - 64);
        } else {
          printf("%c", c);
        }
      } else {
        printf("%c", c);
      }
    } else {
      printf("^I");
    }
  } else {
    if (fl->v == 1) {
      if ((c >= 0 && c < 9) || (c > 10 && c < 32)) {
        printf("^%c", c + 64);
      } else if (c == 127) {
        printf("^%c", c - 64);
      } else {
        printf("%c", c);
      }
    } else {
      printf("%c", c);
    }
  }
}

char **shift(int argc, char **argv) {
  char **out;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      out = &(argv[i]);
      break;
    }
  }
  return out;
}
