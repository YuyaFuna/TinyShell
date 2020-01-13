# TinyShell

マルチクライアント⇔サーバ間の簡易的なシェルです。
ログイン機能つき

コンパイル：
  makeで完了。（うまくいかない場合、1度make cleanしてからmake）

実行:
  サーバ      ：./TinyShell server <FORK LIMIT> <PORT>(デフォルトは8000)
    サーバを切るには「$ pgrep TinyShell | xargs kill -9」とbashに入力
    
  クライアント：./TinyShell client <SERVER IP> <PORT>(デフォルトは8000)
