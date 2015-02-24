#include <stdio.h>
#include "debug.h"
#include "municipality.c"
#include "visit_table.h"

const char* visited_str(visit_table_t* table, int id) {
  int result = visit_table_visited(table, id);
  switch (result) {
  case -1:
    return "見つからない";
  case 0:
    return "未訪問";
  case 1:
    return "訪問済み";
  }

  return NULL;
}

void visit_table_print(visit_table_t* table) {
  visit_table_value_t* value;
  int i;

  putchar('\n');
  for (i = 0; i < VISIT_TABLE_SIZE; ++i) {
    value = table->bucket[i];
    printf("%2d: ", i);
    if (value) {
      while (value) {
        if (value != table->bucket[i]) {
          printf(" -> ");
        }

        if (value->visited) {
          putchar('*');
        }

        printf("[%d]%s", value->municipality->id, value->municipality->name);

        value = value->next;
      }
      putchar('\n');
    } else {
      puts("Empty");
    }
  }
  putchar('\n');
}

int main(void) {
  municipality_t m10 = {10, "m10", 100};
  municipality_t m20 = {20, "m20", 200};
  municipality_t m3  = { 3, "m3",   30};

  visit_table_t table;

  visit_table_init(&table);

  visit_table_add(&table, &m10);
  puts("10 を追加");
  visit_table_print(&table);

  printf("10: %s\n", visited_str(&table, 10));
  visit_table_set_visited(&table, 10);
  puts("10 を訪問");
  printf("10: %s\n", visited_str(&table, 10));

  printf("100: %s\n", visited_str(&table, 100));

  visit_table_add(&table, &m20);
  puts("20 を追加");
  visit_table_print(&table);

  visit_table_add(&table, &m3);
  puts("3 を追加");
  visit_table_print(&table);

  printf("3: %s\n", visited_str(&table, 3));
  visit_table_set_visited(&table, 3);
  puts("3 を訪問");
  printf("3: %s\n", visited_str(&table, 3));
  visit_table_print(&table);

  puts("解放");
  visit_table_free(&table);
  visit_table_print(&table);

  return 0;
}
