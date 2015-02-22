2014 年度 データ構造・アルゴリズム 課題
=======================================

リスト構造を使ってデータを管理するプログラムを作る課題。

独自の機能を追加すれば加点されていくらしいので、いろいろ作ってみるつもり。

ビルド
------

以下を実行する。

```bash
make
```

ToDo
----

* 簡単な経路探索を実装する。
    * 隣への移動コストは必ず 1 とする。
    * 幅優先探索で行う。
    * 訪問済みかどうかの管理は ID をキーとするハッシュテーブルで行う。
