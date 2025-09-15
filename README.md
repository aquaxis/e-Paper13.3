# e-Paper 13.3インチ ディスプレイ プロジェクト

## 概要
本プロジェクトは13.3インチのe-Paperディスプレイを制御するためのCプログラムです。
BCM2835ライブラリを使用してRaspberry Pi上でe-Paperディスプレイに画像を表示することができます。

## システム要件
- Raspberry Pi (BCM2835対応)
- BCM2835ライブラリ
- libpng
- 13.3インチe-Paperディスプレイ

## ディレクトリ構成
```
.
├── lib/
│   ├── Config/     # 設定ファイル
│   ├── e-Paper/    # e-Paperドライバー
│   ├── GUI/        # GUI関連
│   └── Fonts/      # フォントファイル
├── src/            # ソースコード
├── bin/            # ビルド成果物
└── Makefile        # ビルド設定
```

## ビルド方法
```bash
make
```

## 使用方法
```bash
./epd [画像ファイルパス]
```

## 主な機能
- PNG画像の読み込みと表示
- e-Paperディスプレイ制御
- フォント表示機能
- 割り込み処理（Ctrl+C）

## 開発環境
- 言語: C
- コンパイラ: GCC
- ライブラリ: BCM2835, libpng
- ターゲット: Raspberry Pi