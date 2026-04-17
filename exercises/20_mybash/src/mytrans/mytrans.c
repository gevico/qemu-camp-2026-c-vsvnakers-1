// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void to_lowercase(char *str);

static const char *resolve_path(const char *input, char *buf, size_t buf_size) {
  if (access(input, F_OK) == 0) {
    return input;
  }

  if (strncmp(input, "/workspace/", 11) == 0) {
    snprintf(buf, buf_size, ".%s", input + 10);
    if (access(buf, F_OK) == 0) {
      return buf;
    }

    snprintf(buf, buf_size, "..%s", input + 10);
    if (access(buf, F_OK) == 0) {
      return buf;
    }
  }

  return input;
}

void trim(char *str) {
    // TODO: 在这里添加你的代码
    if (!str || str[0] == '\0') {
      return;
    }

    char *start = str;
    while (*start && isspace((unsigned char)*start)) {
      start++;
    }
    if (start != str) {
      memmove(str, start, strlen(start) + 1);
    }

    size_t len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
      str[--len] = '\0';
    }
}

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("无法打开词典文件");
    return -1;
  }

  char line[1024];
  char current_word[100] = {0};
  char current_translation[1024] = {0};
  int in_entry = 0;

    // TODO: 在这里添加你的代码
  while (fgets(line, sizeof(line), file) != NULL) {
    line[strcspn(line, "\n")] = '\0';
    trim(line);

    if (line[0] == '\0') {
      continue;
    }

    if (line[0] == '#') {
      if (in_entry && current_word[0] != '\0' && current_translation[0] != '\0') {
        hash_table_insert(table, current_word, current_translation);
        if (dict_count) {
          (*dict_count)++;
        }
      }

      strncpy(current_word, line + 1, sizeof(current_word) - 1);
      current_word[sizeof(current_word) - 1] = '\0';
      to_lowercase(current_word);
      current_translation[0] = '\0';
      in_entry = 1;
      continue;
    }

    if (strncmp(line, "Trans:", 6) == 0 && in_entry) {
      strncpy(current_translation, line + 6, sizeof(current_translation) - 1);
      current_translation[sizeof(current_translation) - 1] = '\0';
      trim(current_translation);
    }
  }

  if (in_entry && current_word[0] != '\0' && current_translation[0] != '\0') {
    hash_table_insert(table, current_word, current_translation);
    if (dict_count) {
      (*dict_count)++;
    }
  }

  fclose(file);
  return 0;
}

void to_lowercase(char *str) {
  for (; *str; ++str)
    *str = tolower((unsigned char)*str);
}

int __cmd_mytrans(const char* filename) {
  HashTable *table = create_hash_table();
  if (!table) {
    fprintf(stderr, "无法创建哈希表\n");
    return 1;
  }

  printf("=== 哈希表版英语翻译器（支持百万级数据）===\n");
  uint64_t dict_count = 0;
  char input_fallback[512];
  const char *input_path = resolve_path(filename, input_fallback, sizeof(input_fallback));

  char dict_path_buf[1024];
  strncpy(dict_path_buf, input_path, sizeof(dict_path_buf) - 1);
  dict_path_buf[sizeof(dict_path_buf) - 1] = '\0';
  char *slash = strrchr(dict_path_buf, '/');
  if (slash != NULL) {
    *(slash + 1) = '\0';
    strncat(dict_path_buf, "dict.txt", sizeof(dict_path_buf) - strlen(dict_path_buf) - 1);
  } else {
    strncpy(dict_path_buf, "dict.txt", sizeof(dict_path_buf) - 1);
    dict_path_buf[sizeof(dict_path_buf) - 1] = '\0';
  }

  const char *dict_path = dict_path_buf;
  if (access(dict_path, F_OK) != 0) {
    char dict_fallback[512];
    dict_path = resolve_path("/workspace/exercises/20_mybash/src/mytrans/dict.txt", dict_fallback, sizeof(dict_fallback));
  }

  if (load_dictionary(dict_path, table, &dict_count) != 0) {
    fprintf(stderr, "加载词典失败，请确保 dict.txt 存在。\n");
    free_hash_table(table);
    return 1;
  }
  printf("词典加载完成，共计%ld词条。\n", dict_count);

  FILE* file = fopen(input_path, "r");
  if (file == NULL) {
    fprintf(stderr, "无法打开文件 dict.txt。\n");
    free_hash_table(table);
    return 1;
  }

  char line[256];
  while (fgets(line, sizeof(line), file) != NULL) {
    line[strcspn(line, "\n")] = '\0';

    if (strlen(line) == 0) {
        continue;
    }

    // 使用 strtok 按空格分割单词
    char *word = strtok(line, " ");
    while (word != NULL) {
      char normalized[256];
      int pos = 0;
      for (int i = 0; word[i] != '\0' && pos < (int)sizeof(normalized) - 1; i++) {
        unsigned char ch = (unsigned char)word[i];
        if (isalnum(ch) || ch == '\'') {
          normalized[pos++] = (char)ch;
        }
      }
      normalized[pos] = '\0';

      if (normalized[0] != '\0') {
        to_lowercase(normalized);
        const char *translation = hash_table_lookup(table, normalized);
        printf("原文: %s\t", normalized);
        if (translation) {
            printf("翻译: %s\n", translation);
        } else {
            printf("未找到该单词的翻译。\n");
        }
      }

      word = strtok(NULL, " ");
    }
  }

  fclose(file);
  free_hash_table(table);
  return 0;
}