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
  COMMAND_EXIT,              // 終了
  COMMAND_APPEND,            // 挿入
  COMMAND_DELETE,            // 削除
  COMMAND_SEARCH,            // 探索
  COMMAND_SEARCH_AND_DELETE, // 探索 & 削除
  COMMAND_LIST,              // 一覧
  COMMAND_TOTAL              // ダミー：総数を知るために使う
} command_t;

typedef enum search_category {
  SEARCH_BACK,         // 戻る
  SEARCH_BY_ID,        // ID で探索
  SEARCH_BY_NAME,      // 名称で探索
  SEARCH_BY_NAME_LIKE, // 名称で探索（曖昧）
  SEARCH_BY_AREA,      // 面積で探索
  SEARCH_TOTAL         // ダミー：総数を知るために使う
} search_category_t;

typedef enum search_and_delete_category {
  SEARCH_AND_DELETE_BACK,    // 戻る
  SEARCH_AND_DELETE_BY_ID,   // ID で探索
  SEARCH_AND_DELETE_BY_NAME, // 名称で探索
  SEARCH_AND_DELETE_TOTAL    // ダミー：総数を知るために使う
} search_and_delete_category_t;

command_t select_command(void);
void add_initial_data(list_t* list);
void free_data(municipality_t* value, void** params);
void print_municipality(municipality_t* value, void** params);
void print_header(void);
void print_a_municipality(municipality_t* municipality);
void print_data(list_t* list);
void append_data(list_t* list);
void delete_data(list_t* list);
void search_data(list_t* list);
search_category_t select_search_category(void);
void search_and_delete_data(list_t* list);
search_and_delete_category_t select_search_and_delete_category(void);

municipality_t* search_by_id(list_t* list);
municipality_t* search_by_name(list_t* list);
int search_by_name_like(list_t* list);
int search_by_area(list_t* list);

int search_and_delete_by_id(list_t* list);
int search_and_delete_by_name(list_t* list);
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
    "[%d]挿入 [%d]削除 [%d]探索 [%d]探索 & 削除 [%d]一覧 [%d]終了\n",
    COMMAND_APPEND,
    COMMAND_DELETE,
    COMMAND_SEARCH,
    COMMAND_SEARCH_AND_DELETE,
    COMMAND_LIST,
    COMMAND_EXIT
  );

  do {
    printf("> ");
    scanf("%d", &command);
  } while (command < COMMAND_EXIT || command >= COMMAND_TOTAL);

  return (command_t)command;
}

#define INITIAL_DATA_LENGTH 22
void add_initial_data(list_t* list) {
  // データは以下の URL から入手した
  // http://toukei.pref.shizuoka.jp/toukeikikakuhan/data/01-040/h24_02_01.html
  municipality_t* data[INITIAL_DATA_LENGTH];
  int i = 0;

  NULL_CHECK(list, "add_initial_data: list");

  for (i = 0; i < INITIAL_DATA_LENGTH; ++i) {
    data[i] = malloc(sizeof(municipality_t));
  }

  i = 0;
  municipality_init(data[i], 1,  "Shizuoka",   1411.93);
  data[i]->adjacency_list[0] = 19;
  data[i]->adjacency_list[1] = 20;
  data[i]->adjacency_list[2] = 21;
  data[i]->adjacency_list[3] = 22;

  ++i;
  municipality_init(data[i], 2,  "Hamamatsu",  1558.04);
  data[i]->adjacency_list[0] = 11;
  data[i]->adjacency_list[1] = 12;

  ++i;
  municipality_init(data[i], 11, "Kosai",        86.65);
  data[i]->adjacency_list[0] = 2;

  ++i;
  municipality_init(data[i], 12, "Iwata",       164.08);
  data[i]->adjacency_list[0] = 2;
  data[i]->adjacency_list[1] = 13;

  ++i;
  municipality_init(data[i], 13, "Fukuroi",     108.56);
  data[i]->adjacency_list[0] = 12;
  data[i]->adjacency_list[1] = 14;

  ++i;
  municipality_init(data[i], 14, "Kakegawa",    265.63);
  data[i]->adjacency_list[0] = 13;
  data[i]->adjacency_list[1] = 15;
  data[i]->adjacency_list[2] = 16;
  data[i]->adjacency_list[3] = 17;

  ++i;
  municipality_init(data[i], 15, "Omaezaki",     65.86);
  data[i]->adjacency_list[0] = 14;
  data[i]->adjacency_list[1] = 16;
  data[i]->adjacency_list[2] = 18;

  ++i;
  municipality_init(data[i], 16, "Kikugawa",     94.24);
  data[i]->adjacency_list[0] = 14;
  data[i]->adjacency_list[1] = 15;
  data[i]->adjacency_list[2] = 17;
  data[i]->adjacency_list[3] = 18;

  ++i;
  municipality_init(data[i], 17, "Shimada",     315.88);
  data[i]->adjacency_list[0] = 14;
  data[i]->adjacency_list[1] = 16;
  data[i]->adjacency_list[2] = 18;
  data[i]->adjacency_list[3] = 19;

  ++i;
  municipality_init(data[i], 18, "Makinohara",  111.68);
  data[i]->adjacency_list[0] = 15;
  data[i]->adjacency_list[1] = 16;
  data[i]->adjacency_list[2] = 17;

  ++i;
  municipality_init(data[i], 19, "Fujieda",     194.03);
  data[i]->adjacency_list[0] = 1;
  data[i]->adjacency_list[1] = 17;
  data[i]->adjacency_list[2] = 20;

  ++i;
  municipality_init(data[i], 20, "Yaizu",        70.62);
  data[i]->adjacency_list[0] = 1;
  data[i]->adjacency_list[1] = 17;

  ++i;
  municipality_init(data[i], 21, "Fuji",        245.02);
  data[i]->adjacency_list[0] = 1;
  data[i]->adjacency_list[1] = 22;
  data[i]->adjacency_list[2] = 23;
  data[i]->adjacency_list[3] = 25;

  ++i;
  municipality_init(data[i], 22, "Fujinomiya",  388.99);
  data[i]->adjacency_list[0] = 1;
  data[i]->adjacency_list[1] = 21;

  ++i;
  municipality_init(data[i], 23, "Numazu",      187.13);
  data[i]->adjacency_list[0] = 21;
  data[i]->adjacency_list[1] = 24;
  data[i]->adjacency_list[2] = 27;

  ++i;
  municipality_init(data[i], 24, "Mishima",      62.13);
  data[i]->adjacency_list[0] = 23;
  data[i]->adjacency_list[1] = 25;

  ++i;
  municipality_init(data[i], 25, "Susono",      138.17);
  data[i]->adjacency_list[0] = 21;
  data[i]->adjacency_list[1] = 24;
  data[i]->adjacency_list[2] = 26;

  ++i;
  municipality_init(data[i], 26, "Gotemba",     194.85);
  data[i]->adjacency_list[0] = 25;

  ++i;
  municipality_init(data[i], 27, "Izunokuni",    94.71);
  data[i]->adjacency_list[0] = 23;
  data[i]->adjacency_list[1] = 28;
  data[i]->adjacency_list[2] = 29;

  ++i;
  municipality_init(data[i], 28, "Atami",        61.61);
  data[i]->adjacency_list[0] = 27;
  data[i]->adjacency_list[1] = 30;

  ++i;
  municipality_init(data[i], 29, "Izu",         363.97);
  data[i]->adjacency_list[0] = 23;
  data[i]->adjacency_list[1] = 27;
  data[i]->adjacency_list[2] = 30;

  ++i;
  municipality_init(data[i], 30, "Ito",         124.13);
  data[i]->adjacency_list[0] = 28;
  data[i]->adjacency_list[1] = 29;

  for (i = 0; i < INITIAL_DATA_LENGTH; ++i) {
    list_append(list, data[i]);
  }
}

void free_data(municipality_t* value, void** params) {
  municipality_free(value);
}

void append_data(list_t* list) {
  municipality_t data;
  municipality_t* new_municipality;
  void* pred_params[1];
  int i;

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

  new_municipality = malloc(sizeof(municipality_t));
  NULL_CHECK(new_municipality, "append_data: new_municipality");
  municipality_init(new_municipality, data.id, data.name, data.area);

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

void print_municipality(municipality_t* value, void** params) {
  int i;

  NULL_CHECK(value, "print_municipality: value");

  printf(
    "%4d    %-16s    %11.2f     ",
    value->id, value->name, value->area
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
    "  ID    名称                面積 [km^2]     隣接\n"
    "----------------------------------------------------------"
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
