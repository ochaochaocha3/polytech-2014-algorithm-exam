/*
 * 2014 年度 データ構造・アルゴリズム 課題
 * 自治体情報管理プログラム::経路探索
 */

#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "municipality.h"
#include "list.h"
#include "visit_table.h"
#include "route_search.h"

// 幅優先探索のノード
typedef struct route_search_node {
  int id;
  struct route_search_node* from;
} route_search_node_t;

// 幅優先探索のノードを初期化する
static void route_search_node_init(route_search_node_t* node);
// 最短経路を幅優先探索で見つける
static route_search_node_t* route_search_bfs(
  municipality_t* start, municipality_t* end,
  visit_table_t* visit_table, route_search_node_t* queue, size_t queue_size
);
// 見つかった最短経路を出力する
static void route_search_print_path(
  route_search_node_t* node, visit_table_t* visit_table
);
// 訪問テーブルに自治体情報を追加するコールバック関数
static void add_to_visit_table(municipality_t* municipality, void** params);

void do_route_search(
  list_t* list, municipality_t* start, municipality_t* end
) {
  visit_table_t visit_table;
  route_search_node_t* queue;
  route_search_node_t* end_node;
  size_t n_municipalities;
  size_t queue_size = 1;
  int start_id, end_id;
  void* pred_params[1];
  size_t i;

  NULL_CHECK(list, "do_route_search: list");
  NULL_CHECK(start, "do_route_search: start");
  NULL_CHECK(end, "do_route_search: end");

  start_id = start->id;
  end_id = end->id;

  printf(
    "\n経路探索: %d %s -> %d %s\n",
    start_id, start->name, end_id, end->name
  );

  // 訪問テーブルの準備
  visit_table_init(&visit_table);

  pred_params[0] = (void*)&visit_table;
  list_for_each(list, add_to_visit_table, pred_params);

  // キューの準備
  n_municipalities = list_length(list);
  for (i = 0; i < n_municipalities; ++i) {
    queue_size *= 2;
  }
  if (queue_size < 64) {
    // 最低でも以下の領域を確保する
    queue_size = 64;
  }

  queue = malloc(queue_size * sizeof(route_search_node_t));
  NULL_CHECK(queue, "do_route_search: queue");
  for (i = 0; i < queue_size; ++i) {
    route_search_node_init(&queue[i]);
  }

  end_node = route_search_bfs(
    start, end, &visit_table, queue, queue_size
  );

  if (end_node) {
    route_search_print_path(end_node, &visit_table);
    putchar('\n');
  } else {
    puts("到達できませんでした");
  }

  free(queue);
  visit_table_free(&visit_table);
}

static void route_search_node_init(route_search_node_t* node) {
  NULL_CHECK(node, "route_search_node_init: node");

  node->id = 0;
  node->from = NULL;
}

static route_search_node_t* route_search_bfs(
  municipality_t* start, municipality_t* end,
  visit_table_t* visit_table, route_search_node_t* queue, size_t queue_size
) {
  int adjacent_id;
  size_t head_idx, tail_idx;
  route_search_node_t* head;
  municipality_t* municipality;
  int i;

  NULL_CHECK(visit_table, "route_search_bfs: visit_table");
  NULL_CHECK(queue, "route_search_bfs: queue");

  head_idx = 0;
  queue[0].id = start->id;
  tail_idx = 1;

  while (head_idx < tail_idx) {
    head = &queue[head_idx++];

    if (head->id == end->id) {
      return head;
    }

    municipality = visit_table_municipality(visit_table, head->id);
    for (i = 0; i < MUNICIPALITY_ADJ_SIZE; ++i) {
      adjacent_id = municipality->adjacency_list[i];
      if (adjacent_id < 1) {
        break;
      }

      if (!visit_table_visited(visit_table, adjacent_id)) {
        if (tail_idx >= queue_size) {
          fprintf(stderr, "経路探索キューが溢れました\n");
          return NULL;
        }

        queue[tail_idx].id = adjacent_id;
        queue[tail_idx].from = head;

        ++tail_idx;
      }
    }
  }

  // 終点へ到達できなかった場合
  return NULL;
}

static void route_search_print_path(
  route_search_node_t* node, visit_table_t* visit_table
) {
  municipality_t* municipality;

  NULL_CHECK(node, "route_search_print_path: node");
  NULL_CHECK(node, "route_search_print_path: visit_table");

  municipality = visit_table_municipality(visit_table, node->id);
  if (node->from) {
    route_search_print_path(node->from, visit_table);
    printf(" -> %s", municipality->name);
  } else {
    printf("%s", municipality->name);
  }
}

static void add_to_visit_table(
  municipality_t* municipality, void** params
) {
  visit_table_t* visit_table;

  NULL_CHECK(municipality, "add_to_visit_table: municipality");
  NULL_CHECK(params[0], "add_to_visit_table: params[0]");

  visit_table = (visit_table_t*)params[0];

  visit_table_add(visit_table, municipality);
}
