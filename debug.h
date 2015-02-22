/*
 * 2014 年度 データ構造・アルゴリズム 課題
 * 自治体情報管理プログラム::デバッグ用マクロ
 */

#ifndef _DEBUG_H

#define _DEBUG_H

#include <stdio.h>

#define DEBUG /* デバッグ中 */

#ifdef DEBUG

#define NULL_CHECK(x, s) \
  do { \
    if ((x) == NULL) { \
      fprintf(stderr, "%s is NULL!\n", s); \
      exit(1); \
    } \
  } while (0)

#else

#define NULL_CHECK(x, s) \
  do { /* 何もしない */ } while (0)

#endif

#endif
