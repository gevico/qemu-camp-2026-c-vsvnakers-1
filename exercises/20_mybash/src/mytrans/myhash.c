// hash_table.c
#include "myhash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *dup_string(const char *src) {
  size_t len;
  char *dst;

  if (src == NULL) {
    return NULL;
  }

  len = strlen(src) + 1;
  dst = (char *)malloc(len);
  if (dst == NULL) {
    return NULL;
  }

  memcpy(dst, src, len);
  return dst;
}

// djb2 哈希函数（经典字符串哈希，分布均匀）
unsigned long hash_function(const char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; // hash * 33 + c
  return hash;
}

// 创建哈希表
HashTable *create_hash_table() {
  HashTable *table = malloc(sizeof(HashTable));
  if (!table)
    return NULL;
  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    table->buckets[i] = NULL;
  }
  return table;
}

// 释放单个节点
void free_node(HashNode *node) {
  if (node) {
    free(node->key);
    free(node->value);
    free(node);
  }
}

// 释放整个哈希表
void free_hash_table(HashTable *table) {
  if (!table)
    return;
  for (int i = 0; i < HASH_TABLE_SIZE; i++) {
    HashNode *curr = table->buckets[i];
    while (curr) {
      HashNode *tmp = curr;
      curr = curr->next;
      free_node(tmp);
    }
  }
  free(table);
}

// 插入键值对
int hash_table_insert(HashTable *table, const char *key, const char *value) {
  if (!table || !key || !value)
    return 0;

  unsigned long hash = hash_function(key) % HASH_TABLE_SIZE;
  HashNode *node = table->buckets[hash];

    // TODO: 在这里添加你的代码
  while (node) {
    if (strcmp(node->key, key) == 0) {
      char *new_value = dup_string(value);
      if (!new_value) {
        return 0;
      }
      free(node->value);
      node->value = new_value;
      return 1;
    }
    node = node->next;
  }

  HashNode *new_node = (HashNode *)malloc(sizeof(HashNode));
  if (!new_node) {
    return 0;
  }

  new_node->key = dup_string(key);
  new_node->value = dup_string(value);
  if (!new_node->key || !new_node->value) {
    free(new_node->key);
    free(new_node->value);
    free(new_node);
    return 0;
  }

  new_node->next = table->buckets[hash];
  table->buckets[hash] = new_node;

  return 1;
}

// 查找键
const char *hash_table_lookup(HashTable *table, const char *key) {
  if (!table || !key)
    return NULL;

  unsigned long hash = hash_function(key) % HASH_TABLE_SIZE;
  HashNode *node = table->buckets[hash];

    // TODO: 在这里添加你的代码
  while (node) {
    if (strcmp(node->key, key) == 0) {
      return node->value;
    }
    node = node->next;
  }

  return NULL; // 未找到
}
