/*
 * 2014 年度 データ構造・アルゴリズム 課題
 * 自治体情報管理プログラム::経路探索::訪問テーブル
 *
 * チェイン法のハッシュテーブルで実装している
 */

#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "visit_table.h"

// 指定した ID に対応する値を返す
// 名前は Ruby の Hash#fetch から
// http://docs.ruby-lang.org/ja/2.2.0/class/Hash.html#I_FETCH
static visit_table_value_t* visit_table_fetch(visit_table_t* table, int id);
// 値を初期化する
static void visit_table_value_init(
  visit_table_value_t* value, municipality_t* municipality
);
// 値の領域を解放する
static void visit_table_value_free(visit_table_value_t* value);

void visit_table_init(visit_table_t* table) {
  int i;

  NULL_CHECK(table, "visit_table_init: table");

  for (i = 0; i < VISIT_TABLE_SIZE; ++i) {
    table->bucket[i] = NULL;
  }
}

void visit_table_free(visit_table_t* table) {
  visit_table_value_t* current_value;
  visit_table_value_t* next_value;
  int i;

  NULL_CHECK(table, "visit_table_free: table");

  for (i = 0; i < VISIT_TABLE_SIZE; ++i) {
    if (table->bucket[i]) {
      current_value = table->bucket[i];
      while (current_value) {
        next_value = current_value->next;
        visit_table_value_free(current_value);
        current_value = next_value;
      }

      table->bucket[i] = NULL;
    }
  }
}

void visit_table_add(visit_table_t* table, municipality_t* municipality) {
  int hash;
  visit_table_value_t* new_value;
  visit_table_value_t* tail_value;

  NULL_CHECK(table, "visit_table_add: table");

  new_value = malloc(sizeof(visit_table_value_t));
  NULL_CHECK(new_value, "visit_table_add: new_value");
  visit_table_value_init(new_value, municipality);

  hash = municipality->id % VISIT_TABLE_SIZE;
  if (table->bucket[hash]) {
    // 既に同じハッシュ値の値が存在する
    tail_value = table->bucket[hash];
    while (tail_value->next) {
      tail_value = tail_value->next;
    }

    tail_value->next = new_value;
  } else {
    // そのハッシュ値の値は存在しない
    table->bucket[hash] = new_value;
  }
}

static visit_table_value_t* visit_table_fetch(
  visit_table_t* table, int id
) {
  int hash;
  visit_table_value_t* value;

  NULL_CHECK(table, "visit_table_fetch: table");

  hash = id % VISIT_TABLE_SIZE;
  value = table->bucket[hash];
  while (value) {
    if (value->municipality->id == id) {
      return value;
    }

    value = value->next;
  }

  // 指定した ID が見つからなかった場合
  return NULL;
}

municipality_t* visit_table_municipality(visit_table_t* table, int id) {
  visit_table_value_t* value;

  NULL_CHECK(table, "visit_table_municipality: table");

  value = visit_table_fetch(table, id);
  return value ? value->municipality : NULL;
}

int visit_table_visited(visit_table_t* table, int id) {
  visit_table_value_t* value;

  NULL_CHECK(table, "visit_table_visited: table");

  value = visit_table_fetch(table, id);

  return value ? value->visited : -1;
}

int visit_table_set_visited(visit_table_t* table, int id) {
  visit_table_value_t* value;

  NULL_CHECK(table, "visit_table_set_visited: table");

  value = visit_table_fetch(table, id);
  if (value) {
    value->visited = 1;
    return 1;
  }

  // 指定した ID が見つからなかった場合
  return 0;
}

static void visit_table_value_init(
  visit_table_value_t* value, municipality_t* municipality
) {
  NULL_CHECK(value, "visit_table_value_init: value");
  NULL_CHECK(value, "visit_table_value_init: municipality");

  value->municipality = municipality;
  value->visited = 0;
  value->next = NULL;
}

static void visit_table_value_free(visit_table_value_t* value) {
  free(value);
}
