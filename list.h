/*
 * 2014 年度 データ構造・アルゴリズム 課題
 * 自治体情報管理プログラム::線形リスト
 */

#ifndef _LIST_H

#define _LIST_H

#include "municipality.h"

typedef struct list_node {
  municipality_t* value;
  struct list_node* next;
} list_node_t;

typedef struct list {
  list_node_t* head;
  list_node_t* tail;
  size_t length;
} list_t;

typedef void (*list_callback_t)(municipality_t* value, void** params);

typedef int (*list_pred_callback_t)(municipality_t* value, void** params);

// リストを初期化する
void list_init(list_t* list);
// リストで使われている領域を解放する
void list_free(list_t* list);
// リストが空かどうかを返す
int list_is_empty(list_t* list);
// リストの長さを返す
size_t list_length(list_t* list);
// リストの末尾にノードを追加する
void list_append(list_t* list, municipality_t* data);
// リストの先頭のノードを消去する
void list_delete_head(list_t* list);
// data と ID が等しい最初のノードを消去する
void list_delete_matched(list_t* list, municipality_t* data);
// リストを空にする
void list_clear(list_t* list);
// 条件に合う最初の自治体を返す
municipality_t* list_search(
  list_t* list, list_pred_callback_t pred, void** params
);
// リストの各項目について繰り返す
void list_for_each(
  list_t* list,list_callback_t callback, void** params
);
// src 内の条件に合うノードを dest に追加する
void list_filter(
  list_t* src, list_t* dest, list_pred_callback_t pred, void** params
);

#endif
