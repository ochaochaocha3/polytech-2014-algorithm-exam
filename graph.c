/*
 * 2014 年度 データ構造・アルゴリズム 課題
 * 自治体情報管理プログラム::グラフ表示
 */

#include <stdio.h>
#include "debug.h"
#include "municipality.h"
#include "list.h"

// 人口グラフの 1 行分を出力するコールバック関数
static void print_population_graph_line(
  municipality_t* municipality, void** params
);
// 面積グラフの 1 行分を出力するコールバック関数
static void print_area_graph_line(
  municipality_t* municipality, void** params
);

void print_population_graph(list_t* list) {
  NULL_CHECK(list, "print_population_graph: list");

  puts(
    "人口グラフ\n"
    "+/10,000 人\n"
    "----------------------+"
  );
  list_for_each(list, print_population_graph_line, NULL);
}

void print_area_graph(list_t* list) {
  NULL_CHECK(list, "print_area_graph: list");

  puts(
    "面積グラフ\n"
    "+/10 km^2\n"
    "----------------------+"
  );
  list_for_each(list, print_area_graph_line, NULL);
}

static void print_population_graph_line(
  municipality_t* municipality, void** params
) {
  int length;
  int i;

  NULL_CHECK(municipality, "print_population_graph_line: municipality");

  printf("%4d %16.16s | ", municipality->id, municipality->name);

  length = municipality->population / 10000;
  for (i = 0; i < length; ++i) {
    putchar(((i + 1) % 10 == 0) ? '|' : '+');
  }

  putchar('\n');
}

static void print_area_graph_line(
  municipality_t* municipality, void** params
) {
  int length;
  int i;

  NULL_CHECK(municipality, "print_area_graph_line: municipality");

  printf("%4d %16.16s | ", municipality->id, municipality->name);

  length = municipality->area / 10;
  for (i = 0; i < length; ++i) {
    putchar(((i + 1) % 10 == 0) ? '|' : '+');
  }

  putchar('\n');
}
