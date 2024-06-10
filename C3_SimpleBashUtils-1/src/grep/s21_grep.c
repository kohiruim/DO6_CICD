#include "s21_grep.h"

void init(struct flags *fl) {
  fl->e = 0;
  fl->i = 0;
  fl->v = 0;
  fl->c = 0;
  fl->l = 0;
  fl->n = 0;
  fl->h = 0;
  fl->s = 0;
  fl->f = 0;
  fl->o = 0;
}

void *s21_to_lower(const char *string) {
  int error = 1;
  char *pString = calloc((strlen(string) + 1), sizeof(char));
  if (pString) {
    strcpy(pString, string);
    for (char *p = (char *)pString; *p != '\0'; p++) {
      if (*p >= 'A' && *p <= 'Z') {
        *p += 32;
      }
    }
    error = 0;
  }
  return (error) ? NULL : (void *)pString;
}

void *s21_to_upper(const char *string) {
  short isError = 1;
  char *pString = calloc((strlen(string) + 1), sizeof(char));
  if (pString) {
    strcpy(pString, string);
    for (char *p = (char *)pString; *p != '\0'; p++) {
      if (*p >= 'a' && *p <= 'z') {
        *p -= 32;
      }
    }
    isError = 0;
  }
  return (isError) ? NULL : (void *)pString;
}

int flag_to_int(char *flags, char flag) {
  int tmp = 0;
  for (int i = 0; i < (int)strlen(flags); i++) {
    if (flags[i] == flag) {
      tmp = 1;
      break;
    }
  }
  return tmp;
}

void open_file(struct flags *fl, char **argv, int file_num, char *find_str,
               char *search_o) {
  FILE *fp;
  char *filename;
  int find = 0;
  argv++;
  while (*argv) {
    if (flag_to_int(*argv, '-')) {
      if (flag_to_int(*argv, 'e') || flag_to_int(*argv, 'f')) argv++;
      argv++;
    } else {
      if (find == 0 && fl->e == 0 && fl->f == 0) {
        find++;
        argv++;
      } else if ((fp = fopen(*argv, "r")) == NULL) {
        if (fl->s == 0) {
          fprintf(stderr, "grep: %s: No such file or directory\n", filename);
        }
        argv++;
      } else {
        filename = *argv++;
        s21_grep(fl, file_num, find_str, fp, filename, search_o);
        fclose(fp);
      }
    }
  }
}

void s21_grep_core(struct flags *fl, int count, char *line) {
  if (fl->n && !(fl->c) && !(fl->l)) {
    printf("%d:", count);
    printf("%s", line);
  } else if (!fl->c && !fl->l && !fl->o) {
    printf("%s", line);
  }
}

void outputFlagL(struct flags *fl, int count_c, int count_vc, char *filename,
                 int file_num, int count_lc) {
  if (fl->l && !(fl->v)) {
    if (fl->c > 0) {
      if (file_num > 1 && fl->h == 0) {
        printf("%s:", filename);
      }
      printf("%d\n", count_lc);
    }
    if (count_c != 0) {
      printf("%s\n", filename);
    }
  } else if (fl->l && fl->v) {
    if (count_vc != 0) {
      printf("%s\n", filename);
    }
  } else if (fl->v > 0 && fl->c > 0) {
    if (file_num < 2 || fl->h > 0) {
      printf("%d\n", count_vc);
    } else {
      printf("%s:%d\n", filename, count_vc);
    }
  } else if (fl->c == 1) {
    if (file_num < 2 || fl->h > 0) {
      if (fl->v == 0) {
        printf("%d\n", count_c);
      } else {
        printf("%d\n", count_vc);
      }
    } else {
      if (fl->h == 0) {
        printf("%s:%d\n", filename, count_c);
      }
    }
  }
}

int s21_grep(struct flags *fl, int file_num, char *find_str, FILE *fp,
             char *filename, char *search_o) {
  regex_t regex;
  char *line = NULL;
  size_t len = 0, nmatch = 2, read;
  int count = 1, count_c = 0, count_vc = 0, reti, count_lc = 0;
  regcomp(&regex, find_str, REG_EXTENDED);
  regmatch_t pmatch[2];
  while ((read = getline(&line, &len, fp)) != (size_t)EOF) {
    count_lc = 1;
    size_t count_b_ex = strlen(line);
    char *line_tmp = calloc(strlen(line), sizeof(char));
    if (line[strlen(line)] == '\n' && strlen(line) == 1) {
      strcat(line, "\n");
    }
    if (line[strlen(line) - 1] != '\n') {
      strcat(line, "\n");
    }
    if (fl->i) {
      strcpy(line_tmp, line);
      char *line_tmp2 = s21_to_lower(line_tmp);
      char *line_tmp3 = s21_to_upper(line_tmp);
      int reti_tmp;
      reti_tmp = regexec(&regex, line_tmp3, nmatch, pmatch, 0);
      reti = regexec(&regex, line_tmp2, nmatch, pmatch, 0);
      reti = reti && reti_tmp;
      free(line_tmp2);
      free(line_tmp3);
    } else {
      reti = regexec(&regex, line, 0, NULL, 0);
    }
    if (fl->v == 0) {
      if (!reti) {
        count_c++;
        if (fl->n > 0 && fl->c == 0 && fl->l == 0) {
          if (file_num < 2 || fl->h > 0) {
            printf("%d:", count);
          }
        }
      }
      if (fl->o && !(fl->v) && !reti && fl->c == 0 && fl->l == 0) {
        strcat(search_o, "\0");
        int n = 0;
        if (strlen(line) != 1) {
          for (int i = 0; line[i] != '\0'; i++) {
            for (int q = 0; search_o[q] != '\0'; q++) {
              if (line[i] == search_o[q]) {
                n++;
              }
            }
          }
          n /= strlen(search_o);
          if (file_num < 2) {
            while (n > 0) {
              printf("%s\n", search_o);
              n--;
            }
          } else {
            if (fl->h == 0) {
              printf("%s:", filename);
            }
            if (fl->n > 0) {
              printf("%d:", count);
            }
            while (n > 0) {
              printf("%s\n", search_o);
              n--;
            }
          }
        }
      }
      if (!fl->c && !fl->l) {
        if (!(fl->o) && !reti && fl->v == 0) {
          if (file_num < 2 && fl->h != 0) {
            printf("%s", line);
          } else if (fl->h == 0 && file_num > 1) {
            if (file_num > 1 && fl->h == 0) {
              printf("%s:", filename);
              if (fl->n > 0) {
                printf("%d:", count);
              }
            }
            printf("%s", line);
          } else {
            printf("%s", line);
          }
        } else if (!(fl->o) && reti && fl->v > 0) {
          if (file_num < 2 && fl->h != 0) {
            printf("%s", line);
          } else if (fl->h == 0 && file_num > 1) {
            printf("%s: ", filename);
            printf("%s", line);
          } else {
            printf("%s", line);
          }
        }
      }
      count++;
    } else {
      if (fl->n > 0 && reti) {
        if (file_num < 2 || fl->h > 0) {
          printf("%d:", count);
        }
      }
      if (reti) {
        if (fl->c == 0 && fl->l == 0) {
          if (file_num > 1 && fl->h == 0) {
            printf("%s:", filename);
            if (fl->n > 0) {
              printf("%d:", count);
            }
          }
          printf("%s", line);
        }
        count_vc++;
      }
      count++;
      if (fl->o && !(fl->v) && !reti && fl->v == 0) {
        int n = 0;
        if (strlen(line) != 1) {
          for (int i = 0; line[i] != '\0'; i++) {
            for (int q = 0; search_o[q] != '\0'; q++) {
              if (line[i] == search_o[q]) {
                n++;
              }
            }
          }
          n /= strlen(search_o);
          if (file_num < 2) {
            while (n > 0) {
              printf("%s\n", search_o);
              n--;
            }
          } else {
            if (fl->h == 0) {
              printf("%s: ", filename);
            }
            while (n > 0) {
              printf("%s\n", search_o);
              n--;
            }
          }
        }
      }
    }
    if (line_tmp) {
      free(line_tmp);
    }
  }
  outputFlagL(fl, count_c, count_vc, filename, file_num, count_lc);
  if (line) {
    free(line);
  }
  regfree(&regex);
  return count_c;
}

int parse_flag(char **argv, struct flags *fl, int i, char *search) {
  if (argv[i][0] == '-') {
    for (int q = 1; q < (int)strlen(argv[i]); q++) {
      if (argv[1][q] == 'e') {
        fl->e = 1;
      }
      if (argv[i][q] == 'i') {
        fl->i = 1;
      } else if (argv[i][q] == 'v') {
        fl->v = 1;
      } else if (argv[i][q] == 'c') {
        fl->c = 1;
      } else if (argv[i][q] == 'l') {
        fl->l = 1;
      } else if (argv[i][q] == 'n') {
        fl->n = 1;
      } else if (argv[i][q] == 'h') {
        fl->h = 1;
      } else if (argv[i][q] == 's') {
        fl->s = 1;
        fl->e = 1;
      } else if (argv[i][q] == 'f') {
        fl->e = 1;
      } else if (argv[i][q] == 'o') {
        fl->o = 1;
      }
    }
    if (flag_to_int(argv[i], 'e') || fl->e == 1) {
      fl->e = 1;
      if (fl->e == 1 && fl->f == 0) {
        if (strlen(search) == 1) {
          strcat(search, argv[i + 1]);
        } else {
          strcat(search, "|");
          strcat(search, argv[i + 1]);
        }
      } else {
        if (fl->e > 0 || fl->f > 0) {
          strcat(search, "|");
          strcat(search, argv[i + 1]);
        }
      }
      i++;
    }
    if (flag_to_int(argv[i], 'f') || fl->f == 1) {
      FILE *fp;
      fp = fopen(argv[i + 1], "r");
      if (fp == NULL) exit(EXIT_FAILURE);
      fl->f++;
      char current, prev = 'z';
      if (fl->e > 0 && fl->f > 0) {
        strcat(search, "|");
      }
      while ((current = getc(fp)) != EOF) {
        if (prev == '\n' && current != '\n') {
          strcat(search, "|");
        }
        if (current != '\n') {
          strcat(search, "|");
        }
        if (prev == '\n' && current == '\n') {
          strcat(search, "|");
          strcat(search, "\n");
        }
        prev = current;
      }
      fclose(fp);
      i++;
    }
  }
  return i;
}

int main(int argc, char **argv) {
  int error = 0;
  struct flags fl;
  init(&fl);
  char *search = calloc(1000, sizeof(char));
  char *search_o = calloc(1000, sizeof(char));
  int i = 1, file_num = 0, find = 0;
  strcpy(search, "(");
  while (i < argc) {
    if (argv[i][0] == '-') {
      i = parse_flag(argv, &fl, i, search);
    } else {
      if (fl.e == 0 && fl.f == 0 && find == 0) {
        find++;
        strcat(search, argv[i]);
        strcat(search_o, argv[i]);
      } else {
        file_num++;
      }
    }
    i++;
  }
  if (search) {
    strcat(search, ")");
  }
  open_file(&fl, argv, file_num, search, search_o);
  free(search_o);
  free(search);
  return error;
}
