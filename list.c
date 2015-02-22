/*
 * 2014 年度 データ構造・アルゴリズム 課題
 * 自治体情報管理プログラム::線形リスト
 */

#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "municipality.h"
#include "list.h"

static list_node_t* list_node_new(void);
static void list_node_free(list_node_t* node);

void list_init(list_t* list) {
  NULL_CHECK(list, "list_init: list");

  list->head = list->tail = NULL;
  list->length = 0;
}

void list_free(list_t* list) {
  NULL_CHECK(list, "list_free: list");

  list_clear(list);
}

int list_is_empty(list_t* list) {
  NULL_CHECK(list, "list_is_empty: list");

  return list->head == NULL;
}

size_t list_length(list_t* list) {
  NULL_CHECK(list, "list_length: list");

  return list->length;
}

static list_node_t* list_node_new(void) {
  list_node_t* new_node = calloc(1, sizeof(list_node_t));
  NULL_CHECK(new_node, "list_new_node: new_node");

  return new_node;
}

static void list_node_free(list_node_t* node) {
  free(node);
}

void list_append(list_t* list, municipality_t* data) {
  list_node_t* new_node;

  NULL_CHECK(list, "list_append: list");
  NULL_CHECK(data, "list_append: data");

  new_node = list_node_new();
  municipality_copy(data, &new_node->value);

  if (list_is_empty(list)) {
    list->head = list->tail = new_node;
  } else {
    list->tail->next = new_node;
    list->tail = new_node;
  }

  list->length += 1;
}

void list_delete_head(list_t* list) {
  list_node_t* current_head;

  NULL_CHECK(list, "list_delete_head: list");

  if (list_is_empty(list)) {
    // 空だったら何もしない
    return;
  }

  current_head = list->head;

  list->head = list->head->next;

  if (current_head == list->tail) {
    list->tail = NULL;
  }

  list_node_free(current_head);

  list->length -= 1;
}

void list_clear(list_t* list) {
  NULL_CHECK(list, "list_clear: list");

  while (!list_is_empty(list)) {
    list_delete_head(list);
  }
}

void list_for_each(
  list_t* list, list_callback_t callback, void** params
) {
  list_node_t* node;

  NULL_CHECK(list, "list_for_each: list");

  node = list->head;
  while (node) {
    callback(&node->value, params);
    node = node->next;
  }
}

municipality_t* list_search(
  list_t* list, list_pred_callback_t pred, void** params
) {
  list_node_t* node;
  municipality_t* value;

  NULL_CHECK(list, "list_search: list");

  node = list->head;
  while (node) {
    value = &node->value;
    if (pred(value, params)) {
      return value;
    }

    node = node->next;
  }

  return NULL;
}

void list_filter(
  list_t* src, list_t* dest, list_pred_callback_t pred, void** params
) {
  list_node_t* node;

  NULL_CHECK(src, "list_filter: src");
  NULL_CHECK(dest, "list_filter: dest");

  node = src->head;
  while (node) {
    if (pred(&node->value, params)) {
      list_append(dest, &node->value);
    }

    node = node->next;
  }
}
