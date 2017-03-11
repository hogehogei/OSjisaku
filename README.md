# OSjisaku
OS自作入門をLinuxツールで作成する<br>
OS自作入門の内容をLinuxツールのみで進めています。
環境はこちらです。

4.4.39-gentoo #1 SMP Sat Jan 21 12:21:44 JST 2017 x86_64 AMD FX-8320E Eight-Core Processor AuthenticAMD GNU/Linux<br>

x86_64 ですがコンパイラオプションで32ビットバイナリを吐き出すようにしています。<br>
qemuも64ビット版ですが32ビットも動作します。<br>
エミュレータは virsh 経由で動かすようにしています。付属の osjisaku.xml を参考にしてみてください。<br>
下記コマンドでエミュレータを起動できると思います。<br>

virsh define osjisaku.xml<br>
virsh edit osjisaku<br>
/// 環境に応じて編集 ///<br>
virsh start osjisaku<br>
