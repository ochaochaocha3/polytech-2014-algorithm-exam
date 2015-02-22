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

typedef enum command {
  COMMAND_EXIT,   // 終了
  COMMAND_APPEND, // 挿入
  COMMAND_DELETE, // 削除
  COMMAND_SEARCH, // 探索
  COMMAND_LIST,   // 一覧
  COMMAND_TOTAL   // ダミー：総数を知るために使う
} command_t;

typedef enum search_category {
  SEARCH_BACK,         // 戻る
  SEARCH_BY_ID,        // ID で探索
  SEARCH_BY_NAME,      // 名称で探索
  SEARCH_BY_NAME_LIKE, // 名称で探索（曖昧）
  SEARCH_BY_AREA,      // 面積で探索
  SEARCH_TOTAL         // ダミー：総数を知るために使う
} search_category_t;

command_t select_command(void);
void add_initial_data(list_t* list);
void print_municipality(municipality_t* value, void** params);
void print_header(void);
void print_a_municipality(municipality_t* municipality);
void print_data(list_t* list);
void append_data(list_t* list);
void delete_data(list_t* list);
void search_data(list_t* list);
search_category_t select_search_category(void);

void search_by_id(list_t* list);
void search_by_name(list_t* list);
void search_by_name_like(list_t* list);
void search_by_area(list_t* list);

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
    default:
      break;
    }
  }

  list_free(&list);

  return 0;
}

command_t select_command(void) {
  int command;

  printf(
    "[%d]挿入 [%d]削除 [%d]探索 [%d]一覧 [%d]終了\n",
    COMMAND_APPEND,
    COMMAND_DELETE,
    COMMAND_SEARCH,
    COMMAND_LIST,
    COMMAND_EXIT
  );

  do {
    printf("> ");
    scanf("%d", &command);
  } while (command < COMMAND_EXIT || command >= COMMAND_TOTAL);

  return (command_t)command;
}

void add_initial_data(list_t* list) {
  // データは以下の URL から入手した
  // http://toukei.pref.shizuoka.jp/toukeikikakuhan/data/01-040/h24_02_01.html
  municipality_t data[] = {
    {1,  "Shizuoka",   1411.93},
    {2,  "Hamamatsu",  1558.04},
    {11, "Atami",        61.61},
    {12, "Mishima",      62.13},
    {13, "Numazu",      187.13},
    {14, "Fujinomiya",  388.99},
    {15, "Fuji",        245.02},
    {16, "Fujieda",     194.03},
    {17, "Shimada",     315.88},
    {18, "Kakegawa",    265.63},
    {19, "Fukuroi",     108.56},
    {20, "Iwata",       164.08},
    {21, "Kosai",        86.65},
  };
  int length = sizeof(data) / sizeof(data[0]);
  int i;

  NULL_CHECK(list, "add_initial_data: list");

  for (i = 0; i < length; ++i) {
    list_append(list, &data[i]);
  }
}

void append_data(list_t* list) {
  municipality_t data;
  void* pred_params[1];

  NULL_CHECK(list, "add_data: list");

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

  printf("面積 [km^2]: ");
  scanf("%lf", &data.area);

  list_append(list, &data);

  puts("1 件のデータを末尾に挿入しました");
}

void delete_data(list_t* list) {
  NULL_CHECK(list, "delete_data: list");

  putchar('\n');

  if (list_is_empty(list)) {
    puts("データが存在しません");
  } else {
    list_delete_head(list);
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
    "\n[%d]ID [%d]名称 [%d]名称 (曖昧) [%d]面積 [%d]戻る\n",
    SEARCH_BY_ID, SEARCH_BY_NAME, SEARCH_BY_NAME_LIKE, SEARCH_BY_AREA,
    SEARCH_BACK
  );

  do {
    printf("> ");
    scanf("%d", &category);
  } while (category < SEARCH_BACK || category >= SEARCH_TOTAL);

  return (search_category_t)category;
}

void search_by_id(list_t* list) {
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
  } else {
    printf("ID %d のデータは存在しません\n", id);
  }
}

void search_by_name(list_t* list) {
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
  } else {
    printf("名称「%s」のデータは存在しません\n", name);
  }
}

void search_by_name_like(list_t* list) {
  list_t matched_list;
  char pattern[64];
  void* pred_params[1];

  NULL_CHECK(list, "search_by_name_like: list");

  printf("\n名称の探索パターン (大文字小文字無区別): ");
  scanf("%s", pattern);

  pred_params[0] = (void *)&pattern;

  list_init(&matched_list);

  list_filter(list, &matched_list, municipality_name_like, pred_params);
  print_data(&matched_list);

  list_free(&matched_list);
}

void search_by_area(list_t* list) {
  list_t matched_list;
  double area_min, area_max;
  void* pred_params[2];

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
  print_data(&matched_list);

  list_free(&matched_list);
}

void print_municipality(municipality_t* value, void** params) {
  NULL_CHECK(value, "print_municipality: value");

  printf(
    "%2d  %-16s    %11.2f\n",
    value->id, value->name, value->area
  );
}

void print_header(void) {
  puts(
    "ID  名称                面積 [km^2]\n"
    "-----------------------------------"
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
