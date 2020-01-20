# Name

"TinyShell"

# What is TinyShell ?

"ログイン機能付きのマルチクライアント⇔サーバ間の簡易的なシェルです。"

# Requirement

* gcc 5.4.0以降（gcc 7.4.0でも動作確認済）

# Installation

```bash
git clone https://github.com/YuyaFuna/TinyShell
cd TinyShell
make
```

# Usage

"サーバー側"

```bash
#起動
./TinyShell server <FORK LIMIT> <PORT> #default PORT = 8000 ,なくても可
#終了
pgrep TinyShell | xargs kill -9
```

"クライアント側"

```bash
./TinyShell client <SERVER IP> <PORT> #default PORT 8000 ,なくても可
```
