/*
 * 2014 年度 データ構造・アルゴリズム 課題
 * 自治体情報管理プログラム::自治体情報
 */

#ifndef _MUNICIPALITY_H

#define _MUNICIPALITY_H

#define MUNICIPALITY_NAME_SIZE 64 // 名称の最大長

// 自治体情報を表す構造体
typedef struct municipality {
  int id;                            // 固有の番号
  char name[MUNICIPALITY_NAME_SIZE]; // 自治体名
  double area;                       // 面積 [km^2]
} municipality_t;

// 自治体情報の構造体を初期化する
void municipality_init(municipality_t* municipality);
// 自治体情報の構造体を複製する
void municipality_copy(municipality_t* src, municipality_t* dest);

// リスト操作での比較関数

// ID が等しいかどうか
// params[0]: int* id
int municipality_id_equal(municipality_t* municipality, void** params);
// 名称が等しいかどうか
// params[0]: char* name
int municipality_name_equal(municipality_t* municipality, void** params);
// 名前が大文字小文字を区別せず指定したパターンを含むかどうか
// params[0]: char* pattern
int municipality_name_like(municipality_t* municipality, void** params);
// 面積が範囲内かどうか
// params[0]: double* area_min
// params[1]: double* area_max
int municipality_area_in_range(municipality_t* municipality, void** params);

#endif
