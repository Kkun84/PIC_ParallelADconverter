# PIC_ParallelADconverter
Parallel output of 10 bit analog value.

外部からの入力に応じて複数のアナログ入力の中から一つのアナログ値を10bitでパラレル出力する。

## 入出力
* アナログ入力(入力)
    * A0, A1, A2, A3
* 10bit出力(出力)
    * [B7 ~ B0] + [C7, C6]
* 出力開始フラグ(入力))
    * A7
* 出力完了フラグ(出力)
    * A6
* 出力選択ピン(入力)
    * [A5, A4]