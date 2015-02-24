/*
 * 2014 年度 データ構造・アルゴリズム 課題
 * 自治体情報管理プログラム::経路探索::訪問テーブル
 */

#ifndef _VISIT_TABLE_H

#define _VISIT_TABLE_H

#include "municipality.h"

// バケットの大きさ
#define VISIT_TABLE_SIZE 17

// 訪問テーブルの値
typedef struct visit_table_value {
  municipality_t* municipality;   // 自治体情報
  int visited;                    // 訪問したかどうか
  struct visit_table_value* next; // 次の値
} visit_table_value_t;

// 訪問テーブル
// バケットのデータ構造に依存しないように抽象化した
typedef struct visit_table {
  visit_table_value_t* bucket[VISIT_TABLE_SIZE]; // バケット
} visit_table_t;

// 訪問テーブルを初期化する
void visit_table_init(visit_table_t* table);
// 訪問テーブルの領域を解放する
void visit_table_free(visit_table_t* table);
// 項目を追加する
void visit_table_add(visit_table_t* table, municipality_t* municipality);
// 指定した ID の自治体情報を返す
municipality_t* visit_table_municipality(visit_table_t* table, int id);
// 指定した ID を訪れたかどうかを返す
// 訪れていれば 1 を、訪れていなければ 0 を
// 指定した ID が見つからなければ -1 を返す
int visit_table_visited(visit_table_t* table, int id);
// 指定した ID を訪れたことを記録する
// 正常に記録できれば 1 を、指定した ID が見つからなければ 0 を返す
int visit_table_set_visited(visit_table_t* table, int id);

#endif
