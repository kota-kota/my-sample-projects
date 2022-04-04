# my-sample-projects

## ビルド環境

### Windows

`cmake-gui` を使用して、Visual Studio のソリューションファイルを生成します。

### Linux

Visual Studio Code の拡張機能 `Remote Containers` を使用して、開発環境のDockerコンテナに入ります。

GCCコンパイラを使用してビルドします。

```bash
mkdir build_gcc
cd build_gcc
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain.gcc.cmake
```

Clangコンパイラを使用してビルドします。

```bash
mkdir build_clang
cd build_clang
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain.clang.cmake -DCMAKE_BUILD_TYPE=Debug
```

## thread

### ThreadUtility

スレッドに関わる実装の集約

- スレッド名を付ける
- スレッドIDを取得する

### Timerクラス

タイマー
