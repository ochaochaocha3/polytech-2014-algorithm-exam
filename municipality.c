/*
 * 2014 年度 データ構造・アルゴリズム 課題
 * 自治体情報管理プログラム::自治体情報
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "debug.h"
#include "municipality.h"

void municipality_init(municipality_t* municipality) {
  NULL_CHECK(municipality, "municipality_init: municipality");

  municipality->id = 0;
  memset(municipality->name, '\0', MUNICIPALITY_NAME_SIZE);
  municipality->area = 0.0;
}

void municipality_copy(municipality_t* src, municipality_t* dest) {
  NULL_CHECK(src, "municipality_copy: src");
  NULL_CHECK(dest, "municipality_copy: dest");

  dest->id = src->id;
  strncpy(dest->name, src->name, MUNICIPALITY_NAME_SIZE);
  dest->area = src->area;
}

int municipality_id_equal(municipality_t* municipality, void** params) {
  int id;

  NULL_CHECK(municipality, "municipality_id_equal: municipality");
  NULL_CHECK(params[0], "municipality_id_equal: params[0]");

  id = *(int*)params[0];

  return municipality->id == id;
}

int municipality_name_equal(municipality_t* municipality, void** params) {
  char* name;

  NULL_CHECK(municipality, "municipality_name_equal: municipality");
  NULL_CHECK(params[0], "municipality_name_equal: params[0]");

  name = (char*)params[0];

  return strncmp(municipality->name, name, MUNICIPALITY_NAME_SIZE) == 0;
}

int municipality_name_like(municipality_t* value, void** params) {
  char* name;
  char* pattern;
  int len_name;
  int len_pattern;
  int skip_table[UCHAR_MAX + 1];
  unsigned char lower_ch_name;
  int i, j;

  NULL_CHECK(value, "name_contains_pattern: value");
  NULL_CHECK(params[0], "name_contains_pattern: params[0]");

  name = value->name;
  pattern = (char*)params[0];

  len_name = strlen(name);
  len_pattern = strlen(pattern);

  // 簡易 Boyer-Moore 法

  // スキップテーブルの作成
  for (i = 0; i <= UCHAR_MAX; ++i) {
    skip_table[i] = len_pattern;
  }

  for (i = 0; i < len_pattern - 1; ++i) {
    skip_table[tolower(pattern[i])] = len_pattern - i - 1;
  }

  // 探索
  i = len_pattern - 1;
  while (i < len_name) {
    j = len_pattern - 1; // pattern の最後の文字に着目

    // 大文字小文字を区別せず名称がパターンと一致するかどうか調べる
    while (tolower(name[i]) == tolower(pattern[j])) {
      if (j == 0) {
        // 探索成功
        return 1;
      }

      --j;
      --i;
    }

    // スキップテーブルに従って指す場所を移動する
    lower_ch_name = tolower(name[i]);
    i += (skip_table[lower_ch_name] > len_pattern - j) ?
      skip_table[lower_ch_name] : (len_pattern - j);
  }

  // 探索失敗
  return 0;
}

int municipality_area_in_range(
  municipality_t* municipality, void** params
) {
  double area, area_min, area_max;

  NULL_CHECK(municipality, "municipality_area_in_range: municipality");
  NULL_CHECK(params[0], "municipality_area_in_range: params[0]");
  NULL_CHECK(params[1], "amunicipality_rea_in_range: params[1]");

  area = municipality->area;
  area_min = *(double *)params[0];
  area_max = *(double *)params[1];

  return area >= area_min && area <= area_max;
}
