/*
 * 2014 年度 データ構造・アルゴリズム 課題
 * 自治体情報管理プログラム::経路探索::訪問テーブル
 */

#ifndef _VISIT_TABLE_H

#define _VISIT_TABLE_H

#include "municipality.h"

#define VISIT_TABLE_SIZE 17

typedef struct visit_table_value {
  municipality_t* municipality;
  int visited;
  struct visit_table_value* next;
} visit_table_value_t;

typedef struct visit_table {
  visit_table_value_t* bucket[VISIT_TABLE_SIZE];
} visit_table_t;

// 訪問テーブルを初期化する
void visit_table_init(visit_table_t* table);
// 訪問テーブルの領域を解放する
void visit_table_free(visit_table_t* table);
// 項目を追加する
void visit_table_add(visit_table_t* table, municipality_t* municipality);
municipality_t* visit_table_municipality(visit_table_t* table, int id);
// 指定した ID を訪れたかどうかを返す
// 訪れていれば 1 を、訪れていなければ 0 を
// 指定した ID が見つからなければ -1 を返す
int visit_table_visited(visit_table_t* table, int id);
// 指定した ID を訪れたことを記録する
// 正常に記録できれば 1 を、指定した ID が見つからなければ 0 を返す
int visit_table_set_visited(visit_table_t* table, int id);

#endif
