/*
 * 2014 年度 データ構造・アルゴリズム 課題
 * 自治体情報管理プログラム::経路探索
 */

#ifndef _ROUTE_SEARCH_H

#define _ROUTE_SEARCH_H

#include "list.h"

void do_route_search(
  list_t* list, municipality_t* start, municipality_t* end
);

#endif
