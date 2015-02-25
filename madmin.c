/*
 * 2014 年度 データ構造・アルゴリズム 課題
 * 自治体情報管理プログラム
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "municipality.h"
#include "list.h"
#include "route_search.h"
#include "graph.h"

// コマンド
typedef enum command {
  COMMAND_EXIT,              // 終了
  COMMAND_APPEND,            // 挿入
  COMMAND_DELETE,            // 削除
  COMMAND_SEARCH,            // 探索
  COMMAND_SEARCH_AND_DELETE, // 探索 & 削除
  COMMAND_ROUTE_SEARCH,      // 経路探索
  COMMAND_PRINT_GRAPH,       // グラフ表示
  COMMAND_LIST,              // 一覧
  COMMAND_TOTAL              // ダミー：総数を知るために使う
} command_t;

// 探索カテゴリ
typedef enum search_category {
  SEARCH_BACK,          // 戻る
  SEARCH_BY_ID,         // ID で探索
  SEARCH_BY_NAME,       // 名称で探索
  SEARCH_BY_NAME_LIKE,  // 曖昧な名称で探索
  SEARCH_BY_POPULATION, // 人口で探索
  SEARCH_BY_AREA,       // 面積で探索
  SEARCH_TOTAL          // ダミー：総数を知るために使う
} search_category_t;

// 探索 & 削除カテゴリ
typedef enum search_and_delete_category {
  SEARCH_AND_DELETE_BACK,    // 戻る
  SEARCH_AND_DELETE_BY_ID,   // ID で探索
  SEARCH_AND_DELETE_BY_NAME, // 名称で探索
  SEARCH_AND_DELETE_TOTAL    // ダミー：総数を知るために使う
} search_and_delete_category_t;

// グラフ表示カテゴリ
typedef enum graph_category {
  GRAPH_BACK,       // 戻る
  GRAPH_POPULATION, // 人口
  GRAPH_AREA,       // 面積
  GRAPH_TOTAL       // ダミー：総数を知るために使う
} graph_category_t;

// コマンド選択画面を表示し、選んだコマンドを返す
command_t select_command(void);
// 初期データを追加する
void add_initial_data(list_t* list);
// データを解放するコールバック関数
void free_data(municipality_t* value, void** params);
// 自治体情報を出力するコールバック関数
void print_municipality(municipality_t* value, void** params);
// 自治体情報の見出しを出力する
void print_header(void);
// 1 件の自治体情報を出力する
void print_a_municipality(municipality_t* municipality);
// 一覧表示する
void print_data(list_t* list);
// データをリストの末尾に追加する
void append_data(list_t* list);
// リストの先頭のデータを削除する
void delete_data(list_t* list);
// データを探索する
void search_data(list_t* list);
// 探索カテゴリの選択画面を表示し、選んだカテゴリを返す
search_category_t select_search_category(void);
// 探索 & 削除を行う
void search_and_delete_data(list_t* list);
// 探索 & 探索カテゴリの選択画面を表示し、選んだカテゴリを返す
search_and_delete_category_t select_search_and_delete_category(void);
// 経路探索を行う
void route_search(list_t* list);
// グラフを表示する
void print_graph(list_t* list);
// グラフカテゴリの選択画面を表示し、選んだカテゴリを返す
graph_category_t select_graph_category(void);

// ID で探索する
municipality_t* search_by_id(list_t* list);
// 名称で探索する
municipality_t* search_by_name(list_t* list);
// 曖昧な名称で探索する
// SQL の LIKE に相当する
int search_by_name_like(list_t* list);
// 人口で探索する
int search_by_population(list_t* list);
// 面積で探索する
int search_by_area(list_t* list);

// ID で探索 & 削除を行う
int search_and_delete_by_id(list_t* list);
// 名称で探索 & 削除を行う
int search_and_delete_by_name(list_t* list);
// 削除するかどうか確認する
int confirm_delete(void);

int main(void) {
  list_t list;
  command_t command;

  list_init(&list);
  add_initial_data(&list);

  puts("自治体情報管理プログラム");

  while (1) {
    printf("\nデータ件数: %lu\n", (unsigned long)list_length(&list));
    command = select_command();

    if (command == COMMAND_EXIT) {
      break;
    }

    switch (command) {
    case COMMAND_APPEND:
      append_data(&list);
      break;
    case COMMAND_LIST:
      print_data(&list);
      break;
    case COMMAND_DELETE:
      delete_data(&list);
      break;
    case COMMAND_SEARCH:
      search_data(&list);
      break;
    case COMMAND_SEARCH_AND_DELETE:
      search_and_delete_data(&list);
      break;
    case COMMAND_ROUTE_SEARCH:
      route_search(&list);
      break;
    case COMMAND_PRINT_GRAPH:
      print_graph(&list);
      break;
    default:
      break;
    }
  }

  list_for_each(&list, free_data, NULL);
  list_free(&list);

  return 0;
}

command_t select_command(void) {
  int command;

  printf(
    "[%d]挿入 [%d]削除 [%d]探索 [%d]探索 & 削除\n"
    "[%d]経路探索 [%d]グラフ表示 [%d]一覧 [%d]終了\n",
    COMMAND_APPEND,
    COMMAND_DELETE,
    COMMAND_SEARCH,
    COMMAND_SEARCH_AND_DELETE,
    COMMAND_ROUTE_SEARCH,
    COMMAND_PRINT_GRAPH,
    COMMAND_LIST,
    COMMAND_EXIT
  );

  do {
    printf("> ");
    scanf("%d", &command);
  } while (command < COMMAND_EXIT || command >= COMMAND_TOTAL);

  return (command_t)command;
}

#define INITIAL_DATA_LENGTH 23
void add_initial_data(list_t* list) {
  // データは以下の URL から入手した
  // * http://toukei.pref.shizuoka.jp/toukeikikakuhan/data/01-040/h24_02_01.html
  // * https://toukei.pref.shizuoka.jp/jinkoushugyouhan/data/02-030/2702jinkou.html
  municipality_t* data[INITIAL_DATA_LENGTH];
  int i = 0;

  NULL_CHECK(list, "add_initial_data: list");

  for (i = 0; i < INITIAL_DATA_LENGTH; ++i) {
    data[i] = malloc(sizeof(municipality_t));
  }

  i = 0;
  municipality_init(data[i], 1, "Shizuoka", 705754, 1411.93);
  data[i]->adjacency_list[0] = 19;
  data[i]->adjacency_list[1] = 20;
  data[i]->adjacency_list[2] = 21;
  data[i]->adjacency_list[3] = 22;

  ++i;
  municipality_init(data[i], 2, "Hamamatsu", 790475, 1558.04);
  data[i]->adjacency_list[0] = 11;
  data[i]->adjacency_list[1] = 12;

  ++i;
  municipality_init(data[i], 11, "Kosai", 58656, 86.65);
  data[i]->adjacency_list[0] = 2;

  ++i;
  municipality_init(data[i], 12, "Iwata", 164608, 164.08);
  data[i]->adjacency_list[0] = 2;
  data[i]->adjacency_list[1] = 13;

  ++i;
  municipality_init(data[i], 13, "Fukuroi", 85151, 108.56);
  data[i]->adjacency_list[0] = 12;
  data[i]->adjacency_list[1] = 14;

  ++i;
  municipality_init(data[i], 14, "Kakegawa", 114033, 265.63);
  data[i]->adjacency_list[0] = 13;
  data[i]->adjacency_list[1] = 15;
  data[i]->adjacency_list[2] = 16;
  data[i]->adjacency_list[3] = 17;

  ++i;
  municipality_init(data[i], 15, "Omaezaki", 32727, 65.86);
  data[i]->adjacency_list[0] = 14;
  data[i]->adjacency_list[1] = 16;
  data[i]->adjacency_list[2] = 18;

  ++i;
  municipality_init(data[i], 16, "Kikugawa", 46059, 94.24);
  data[i]->adjacency_list[0] = 14;
  data[i]->adjacency_list[1] = 15;
  data[i]->adjacency_list[2] = 17;
  data[i]->adjacency_list[3] = 18;

  ++i;
  municipality_init(data[i], 17, "Shimada", 97941, 315.88);
  data[i]->adjacency_list[0] = 14;
  data[i]->adjacency_list[1] = 16;
  data[i]->adjacency_list[2] = 18;
  data[i]->adjacency_list[3] = 19;

  ++i;
  municipality_init(data[i], 18, "Makinohara", 45829, 111.68);
  data[i]->adjacency_list[0] = 15;
  data[i]->adjacency_list[1] = 16;
  data[i]->adjacency_list[2] = 17;

  ++i;
  municipality_init(data[i], 19, "Fujieda", 143451, 194.03);
  data[i]->adjacency_list[0] = 1;
  data[i]->adjacency_list[1] = 17;
  data[i]->adjacency_list[2] = 20;

  ++i;
  municipality_init(data[i], 20, "Yaizu", 139269, 70.62);
  data[i]->adjacency_list[0] = 1;
  data[i]->adjacency_list[1] = 17;

  ++i;
  municipality_init(data[i], 21, "Fuji", 250154, 245.02);
  data[i]->adjacency_list[0] = 1;
  data[i]->adjacency_list[1] = 22;
  data[i]->adjacency_list[2] = 23;
  data[i]->adjacency_list[3] = 25;

  ++i;
  municipality_init(data[i], 22, "Fujinomiya", 131255, 388.99);
  data[i]->adjacency_list[0] = 1;
  data[i]->adjacency_list[1] = 21;

  ++i;
  municipality_init(data[i], 23, "Numazu", 193232, 187.13);
  data[i]->adjacency_list[0] = 21;
  data[i]->adjacency_list[1] = 24;
  data[i]->adjacency_list[2] = 27;

  ++i;
  municipality_init(data[i], 24, "Mishima", 110398, 62.13);
  data[i]->adjacency_list[0] = 23;
  data[i]->adjacency_list[1] = 25;

  ++i;
  municipality_init(data[i], 25, "Susono", 53204, 138.17);
  data[i]->adjacency_list[0] = 21;
  data[i]->adjacency_list[1] = 24;
  data[i]->adjacency_list[2] = 26;

  ++i;
  municipality_init(data[i], 26, "Gotemba",  87962, 194.85);
  data[i]->adjacency_list[0] = 25;

  ++i;
  municipality_init(data[i], 27, "Izunokuni", 48672, 94.71);
  data[i]->adjacency_list[0] = 23;
  data[i]->adjacency_list[1] = 28;
  data[i]->adjacency_list[2] = 29;

  ++i;
  municipality_init(data[i], 28, "Atami", 37474, 61.61);
  data[i]->adjacency_list[0] = 27;
  data[i]->adjacency_list[1] = 30;

  ++i;
  municipality_init(data[i], 29, "Izu", 31622, 363.97);
  data[i]->adjacency_list[0] = 23;
  data[i]->adjacency_list[1] = 27;
  data[i]->adjacency_list[2] = 30;

  ++i;
  municipality_init(data[i], 30, "Ito", 68992, 24.13);
  data[i]->adjacency_list[0] = 28;
  data[i]->adjacency_list[1] = 29;

  ++i;
  municipality_init(data[i], 31, "Shimoda", 23265, 104.71);

  for (i = 0; i < INITIAL_DATA_LENGTH; ++i) {
    list_append(list, data[i]);
  }
}

void free_data(municipality_t* value, void** params) {
  municipality_free(value);
}

void print_municipality(municipality_t* value, void** params) {
  int i;

  NULL_CHECK(value, "print_municipality: value");

  printf(
    "%4d    %-16.16s    %8ld    %12.2f     ",
    value->id, value->name, value->population, value->area
  );

  for (
    i = 0;
    i < MUNICIPALITY_ADJ_SIZE && value->adjacency_list[i] > 0;
    ++i
  ) {
    if (i > 0) {
      printf(", ");
    }

    printf("%2d", value->adjacency_list[i]);
  }

  putchar('\n');
}

void print_header(void) {
  puts(
    "  ID    名称                    人口     面積 [km^2]     隣接\n"
    "-----------------------------------------------------------------------"
  );
}

void print_a_municipality(municipality_t* municipality) {
  NULL_CHECK(municipality, "print_a_municipality: municipality");

  print_header();
  print_municipality(municipality, NULL);
}

void print_data(list_t* list) {
  NULL_CHECK(list, "print_data: list");

  putchar('\n');

  if (list_is_empty(list)) {
    puts("データが存在しません");
  } else {
    print_header();
    list_for_each(list, print_municipality, NULL);
  }
}

void append_data(list_t* list) {
  municipality_t data;
  municipality_t* new_municipality;
  void* pred_params[1];
  int i;

  NULL_CHECK(list, "append_data: list");

  puts("\n挿入するデータを入力してください");

  do {
    printf("ID (1 以上): ");
    scanf("%d", &data.id);
  } while (data.id < 1);

  pred_params[0] = (void*)&data.id;
  if (list_search(list, municipality_id_equal, pred_params)) {
    printf("ID %d のデータは既に存在します\n", data.id);
    return;
  }

  printf("名称: ");
  scanf("%s", data.name);

  printf("人口: ");
  scanf("%ld", &data.population);

  printf("面積 [km^2]: ");
  scanf("%lf", &data.area);

  new_municipality = malloc(sizeof(municipality_t));
  NULL_CHECK(new_municipality, "append_data: new_municipality");
  municipality_init(
    new_municipality, data.id, data.name, data.population, data.area
  );

  for (i = 0; i < MUNICIPALITY_ADJ_SIZE; ++i) {
    printf("隣接 ID %d (0 以下でなしに設定): ", i);
    scanf("%d", &new_municipality->adjacency_list[i]);

    if (new_municipality->adjacency_list[i] < 1) {
      break;
    }
  }

  list_append(list, new_municipality);

  puts("1 件のデータを末尾に挿入しました");
}

void delete_data(list_t* list) {
  NULL_CHECK(list, "delete_data: list");

  putchar('\n');

  if (list_is_empty(list)) {
    puts("データが存在しません");
  } else {
    municipality_t* value = list->head->value;
    list_delete_head(list);
    municipality_free(value);

    puts("先頭の 1 件のデータを削除しました");
  }
}

void search_data(list_t* list) {
  int category;

  NULL_CHECK(list, "search_data: list");

  if (list_is_empty(list)) {
    puts("データが存在しません");
    return;
  }

  category = select_search_category();

  if (category == SEARCH_BACK) {
    return;
  }

  switch (category) {
  case SEARCH_BY_ID:
    search_by_id(list);
    break;
  case SEARCH_BY_NAME:
    search_by_name(list);
    break;
  case SEARCH_BY_NAME_LIKE:
    search_by_name_like(list);
    break;
  case SEARCH_BY_POPULATION:
    search_by_population(list);
    break;
  case SEARCH_BY_AREA:
    search_by_area(list);
    break;
  default:
    break;
  }
}

search_category_t select_search_category(void) {
  int category;

  printf(
    "\n[%d]ID [%d]名称 [%d]名称 (曖昧) [%d]人口 [%d]面積 [%d]戻る\n",
    SEARCH_BY_ID, SEARCH_BY_NAME, SEARCH_BY_NAME_LIKE,
    SEARCH_BY_POPULATION, SEARCH_BY_AREA,
    SEARCH_BACK
  );

  do {
    printf("> ");
    scanf("%d", &category);
  } while (category < SEARCH_BACK || category >= SEARCH_TOTAL);

  return (search_category_t)category;
}

void search_and_delete_data(list_t* list) {
  int category;

  NULL_CHECK(list, "search_and_delete_data: list");

  if (list_is_empty(list)) {
    puts("データが存在しません");
    return;
  }

  category = select_search_and_delete_category();

  if (category == SEARCH_AND_DELETE_BACK) {
    return;
  }

  switch (category) {
  case SEARCH_AND_DELETE_BY_ID:
    search_and_delete_by_id(list);
    break;
  case SEARCH_AND_DELETE_BY_NAME:
    search_and_delete_by_name(list);
    break;
  default:
    break;
  }
}

search_and_delete_category_t select_search_and_delete_category(void) {
  int category;

  printf(
    "\n[%d]ID [%d]名称 [%d]戻る\n",
    SEARCH_AND_DELETE_BY_ID, SEARCH_AND_DELETE_BY_NAME,
    SEARCH_AND_DELETE_BACK
  );

  do {
    printf("> ");
    scanf("%d", &category);
  } while (
    category < SEARCH_AND_DELETE_BACK ||
      category >= SEARCH_AND_DELETE_TOTAL
  );

  return (search_and_delete_category_t)category;
}

void route_search(list_t* list) {
  int from_id, to_id;
  municipality_t* from;
  municipality_t* to;
  void* pred_params[1];

  NULL_CHECK(list, "route_search: list");

  printf("\n始点 ID: ");
  scanf("%d", &from_id);

  pred_params[0] = (void*)&from_id;
  from = list_search(list, municipality_id_equal, pred_params);
  if (from) {
    printf("始点: %d %s\n", from_id, from->name);
  } else {
    printf("ID %d のデータは存在しません\n", from_id);
    return;
  }

  printf("終点 ID: ");
  scanf("%d", &to_id);

  pred_params[0] = (void*)&to_id;
  to = list_search(list, municipality_id_equal, pred_params);
  if (to) {
    if (to == from) {
      puts("始点と終点が同じです");
      return;
    }

    printf("終点: %d %s\n", to_id, to->name);
  } else {
    printf("ID %d のデータは存在しません\n", to_id);
    return;
  }

  do_route_search(list, from, to);
}

void print_graph(list_t* list) {
  int category;

  NULL_CHECK(list, "print_graph: list");

  if (list_is_empty(list)) {
    puts("データが存在しません");
    return;
  }

  category = select_graph_category();

  if (category == GRAPH_BACK) {
    return;
  }

  putchar('\n');
  switch (category) {
  case GRAPH_POPULATION:
    print_population_graph(list);
    break;
  case GRAPH_AREA:
    print_area_graph(list);
    break;
  default:
    break;
  }
}

graph_category_t select_graph_category(void) {
  int category;

  printf(
    "\n[%d]人口 [%d]面積 [%d]戻る\n",
    GRAPH_POPULATION, GRAPH_AREA, GRAPH_BACK
  );

  do {
    printf("> ");
    scanf("%d", &category);
  } while (category < GRAPH_BACK || category >= GRAPH_TOTAL);

  return (graph_category_t)category;
}

municipality_t* search_by_id(list_t* list) {
  int id;
  void* pred_params[1];
  municipality_t* result;

  NULL_CHECK(list, "search_by_id: list");

  printf("\n探索する ID: ");
  scanf("%d", &id);

  pred_params[0] = (void*)&id;

  result = list_search(list, municipality_id_equal, pred_params);

  putchar('\n');
  if (result) {
    print_a_municipality(result);
    return result;
  }

  printf("ID %d のデータは存在しません\n", id);
  return NULL;
}

municipality_t* search_by_name(list_t* list) {
  char name[64];
  void* pred_params[1];
  municipality_t* result;

  NULL_CHECK(list, "search_by_name: list");

  printf("\n探索する名称: ");
  scanf("%s", name);

  pred_params[0] = (void*)&name;

  result = list_search(list, municipality_name_equal, pred_params);

  putchar('\n');
  if (result) {
    print_a_municipality(result);
    return result;
  }

  printf("名称「%s」のデータは存在しません\n", name);
  return NULL;
}

int search_by_name_like(list_t* list) {
  list_t matched_list;
  char pattern[64];
  void* pred_params[1];
  int matched;

  NULL_CHECK(list, "search_by_name_like: list");

  printf("\n名称の探索パターン (大文字小文字無区別): ");
  scanf("%s", pattern);

  pred_params[0] = (void *)&pattern;

  list_init(&matched_list);

  list_filter(list, &matched_list, municipality_name_like, pred_params);
  matched = !list_is_empty(&matched_list);
  print_data(&matched_list);

  list_free(&matched_list);

  return matched;
}

int search_by_population(list_t* list) {
  list_t matched_list;
  long population_min, population_max;
  void* pred_params[2];
  int matched;

  NULL_CHECK(list, "search_by_population: list");

  printf("\n人口の最小値: ");
  scanf("%ld", &population_min);

  printf("人口の最大値: ");
  scanf("%ld", &population_max);

  if (population_min > population_max) {
    // 最小値と最大値を入れ替える
    long population_temp;

    population_temp = population_min;
    population_min = population_max;
    population_max = population_temp;

    printf(
      "<人口: %ld-%ld で探索します>\n",
      population_min, population_max
    );
  }

  pred_params[0] = (void *)&population_min;
  pred_params[1] = (void *)&population_max;

  list_init(&matched_list);

  list_filter(
    list, &matched_list, municipality_population_in_range, pred_params
  );
  matched = !list_is_empty(&matched_list);
  print_data(&matched_list);

  list_free(&matched_list);

  return matched;
}

int search_by_area(list_t* list) {
  list_t matched_list;
  double area_min, area_max;
  void* pred_params[2];
  int matched;

  NULL_CHECK(list, "search_by_area: list");

  printf("\n面積 [km^2] の最小値: ");
  scanf("%lf", &area_min);

  printf("面積 [km^2] の最大値: ");
  scanf("%lf", &area_max);

  if (area_min > area_max) {
    // 最小値と最大値を入れ替える
    double area_temp;

    area_temp = area_min;
    area_min = area_max;
    area_max = area_temp;

    printf(
      "<面積 [km^2]: %.2lf-%.2lf で探索します>\n",
      area_min, area_max
    );
  }

  pred_params[0] = (void *)&area_min;
  pred_params[1] = (void *)&area_max;

  list_init(&matched_list);

  list_filter(list, &matched_list, municipality_area_in_range, pred_params);
  matched = !list_is_empty(&matched_list);
  print_data(&matched_list);

  list_free(&matched_list);

  return matched;
}

int search_and_delete_by_id(list_t* list) {
  municipality_t* matched;

  NULL_CHECK(list, "search_and_delete_by_id: list");

  matched = search_by_id(list);
  if (!matched) {
    // 見つからなかったので削除しない
    return 0;
  }

  putchar('\n');
  if (!confirm_delete()) {
    // キャンセルされたので削除しない
    return 0;
  }

  list_delete_matched(list, matched);
  municipality_free(matched);

  puts("1 件のデータを削除しました。");
  return 1;
}

int search_and_delete_by_name(list_t* list) {
  municipality_t* matched;

  NULL_CHECK(list, "search_and_delete_by_id: list");

  matched = search_by_name(list);
  if (!matched) {
    // 見つからなかったので削除しない
    return 0;
  }

  putchar('\n');
  if (!confirm_delete()) {
    // キャンセルされたので削除しない
    return 0;
  }

  list_delete_matched(list, matched);
  municipality_free(matched);

  puts("1 件のデータを削除しました。");
  return 1;
}

int confirm_delete(void) {
  int answer;

  do {
    printf("削除しますか? (Yes: 0 / No: 1) > ");
    scanf("%d", &answer);
  } while (answer != 0 && answer != 1);

  return answer == 0;
}
