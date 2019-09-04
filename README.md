# Pre-maiduino
Pre-maid AI x Arduino STM32 collaboration

# このページについて
プリメイドAI に内蔵されているSTM32F102 マイコンで Arduinoを使うための非公式ゾーンです。


# はじめに

事前知識として、プリメイドAIはマイコンSTM32<b> F102 </b>を使用しています。

私が作った自作回路は<b> F103 </b>搭載です。

こちらで紹介するArduino書き換え方法は、本来は<b> F103 </b>向けです。

どちらもほぼ同じスペックのため、対象が違っても動作しているようです。

（スペック比較はこちらのツイート　→　https://twitter.com/devemin/status/1151756254835986432?s=20 ）

ファーム書き換えは自己責任にてお願いします。

保護のかかったファームを書き換え後は、元ファームは消去され、保存できません。

間違いや追加、動作報告などがありましたら、シェア致しますので是非お教えください(^^)

# 環境構築

書き換えに必要なものは、ST Linkというアダプタです。

Amazonや秋月電子で販売しています。（1000～3000円）

http://akizukidenshi.com/catalog/g/gM-04012/

また、ボード上のSWDという4～5個のピンにワイヤーを接続する必要がありますので、必要に応じて用意してください。

秋月でしたら、ブレッドボード用のジャンパワイヤーとICクリップタイプ－小型クリップのケーブルなどが良いのではないでしょうか。

用意できましたら、ボード上のSWD, SWDCLK, GND, 3.3V, (Reset不要かも？)をケーブルにて接続してください。

他の場所との電気的ショートに要注意です。

# ファーム書き換え　準備

プリメイドAIにも搭載されているF102にはファーム書き換え保護機能があるようです。

STM32CubeProgrammer というソフトでその設定をオン・オフできます。

https://www.st.com/ja/development-tools/stm32cubeprog.html#tools-software

ST社サイトへの登録が必要です。

こんな感じです。

https://twitter.com/devemin/status/1150963921752215552?s=20

ダウンロードし、ST Linkで接続して起動すると「読取エラー」が出る時は、保護機能が働いているためです。

左側メニュー「OB」アイコン、Option Bytes より保護オプションタブを開き、一番うえのRead Out Protection のチェックを外し、Apply します。

（注意！）この操作でファームが消去されます。

# ファーム書き換え　Arduino

さて、ファーム書き換えが可能になったあとは、こちらのサイトが詳しく書いてあります。（F103向け）

 →　https://qiita.com/nanbuwks/items/ba37f049f45378da0e2b

必要なところだけ記載すると、

　Arduino IDE ダウンロード＆インストール

　STM32ボード情報をArduino IDEに追加（下記をダウンロード）

→　https://github.com/rogerclarkmelbourne/Arduino_STM32/

Arduino IDE ディレクトリに、hardware/Arduino_STM32-master/ という形になるようにフォルダごとコピー。

その後Arduino IDEを起動すると、メニューのツール→ボード　に「Generic STM32F103C series」が追加されていますので選択。

Variant は(20k RAM, 64K flash) の方を選択。

Upload Method : ST Link を選択。

CPU Speed　は48MHz を選択。

どうやらブートローダーの書き込みまではいらないようです。

別途持っているSTM32F103搭載のBluePillというボードでは、ボード上ピンBOOt0:0, BOOT1:0 で書き込み、動作とも出来ました。

F102は本来RAMが10k なのですが、一応先ほどのArduino_STM32のフォルダの中のほうにboards.txt があり、そのなかで20480 を10240に変更できるようですが、どのように適用されるのかわかっていませんので、私は今はそのままにしています。

# Arduino STM32のtips

STM32 のArduinoは一部通常のArduinoと変数など扱いが異なる部分があるようです。

下記サイトではSTM32 Arduinoについてのリファレンスを翻訳されています。ありがたく参考にさせてもらいましょう！

https://scrapbox.io/ArduinoSTM32/Arduino_STM32_%E3%83%AA%E3%83%95%E3%82%A1%E3%83%AC%E3%83%B3%E3%82%B9_%E6%97%A5%E6%9C%AC%E8%AA%9E%E7%89%88

ちなみに、ArduinoでなくST社公式の「STM32CubeIDE」というソフトでも開発を進めることができます。

@sabotenboy さんと@hanyazou さんの情報によると、先ほどのProgrammerソフトで確認すると個体によってはflashが128k の場合があるみたいです。

私がDigikeyで買ったIC個体はスペック通り64kでした。

https://twitter.com/Sabotenboy/status/1152479068165562368?s=19

# シリアル通信について

参考：　STM32 を Arduino 開発環境で使う場合のシリアルポートの調査 （STM32duino + STM32F103C8 : Blue Pill)

https://qiita.com/nanbuwks/items/5a01b924b192d5d36b31

シリアル通信は、ST Linkで書き込む場合はSerial.begin でなく、Serial1.begin　などとして使います。

Serial1, Serial2, Serial3 がUSART1-3 に対応しているかと思われます。（近藤科学サーボライブラリでは不明点あり、後述参照）

# Bluetooth RN42

シリアル接続ですので簡単です。

```

setup() で

Serial1.begin(115200);

読取処理で

Serial1.print("hoge");

```

などでBluetooth 接続のホスト相手にデータを送信できるはずです。

# ICS動作サンプル

シリアル通信でバイト列を送れば良いのですが、ライブラリを作りました。

https://github.com/devemin/STM32-Arduino-ICSservo を Arduino IDEにて書き込みしてください。

# 近藤科学サーボ　公式ライブラリについて

https://kondo-robot.com/archives/15255

これは1-wire 半二重シリアル通信の想定ではありません。そのまま使うことはできません。

TX/RX/EN_PIN の2+1線です。回路はICS 3.5/3.6 ソフトウェアマニュアルを参照ください。

# ジャイロ MPU-6050 について

こちらのサイトを参考にさせてもらおうと思っています。

Arduino でMPU-6050をカルマンフィルタを用いてドリフト防止している例の記事です。

https://qiita.com/Qikoro/items/d24057b434c44fcdf74e

また、DMPという当ICの独自機能にコーディングを加えることで、IC側に処理をさせ低ノイズ化など可能なようです。

https://n.mtng.org/ele/arduino/tutorial024.html

http://figrobo.blog.fc2.com/blog-entry-33.html

# ハードウェア追加について(SPI、I2Cなど)

プリメイドAIのボード上にはSPI端子用コネクタ（ZH）があるようです。

周辺機器接続あるいは他端末と高速通信が出来ると思います。

ZHコネクタの圧着は専用器具を使った方が良さそうです。大きめの圧着器具では全然作れませんでした・・・。

# ファーム書き換え STM32CubeIDE

ST社公式のアプリケーション STM32CubeIDEを使うと、グラフィカルにSTMマイコンの開発が出来ます。

一応それがスタンダードというかSTMマイコンの正当な使い方かと思います。

公式の日本語情報がたくさんあります。HALという協力なペリフェラル周辺機器ライブラリもあります。

Arduino使うのが良いか、このツール使うのが良いかは好みで良いと思います。

https://www.stmcu.jp/design/document/reference_manual_j/

# このページの注意点

メーカー様に怒られない範囲で)ワクワク楽しくハックしていきたいと思います！

# 参考情報

超速ハックの @GOROman さんによる解析データです。ピンアサイン、基板回路写真もあります。

https://docs.google.com/spreadsheets/d/1c6jqMwkBroCuF74viU_q7dgSQGzacbUW4mJg-957_Rs/edit#gid=191676625

https://twitter.com/GOROman/status/1151046658354032640?s=19

ST公式データシート

https://www.stmcu.jp/stm32/stm32f102/

F102 F103はピン配置は全く同じのようです（LQFP48）

BluePill 回路図(ちなみに私の自作ボードは、このBluePillの103を102に載せかえたものです。)

https://wiki.stm32duino.com/images/a/ac/Stm32f103c8t6_schematic.png

ST公式　日本語リファレンス　文書番号0008

https://www.stmcu.jp/design/document/reference_manual_j/

Arduino STM32 リファレンス 日本語版　USART support(usart.h)

https://scrapbox.io/ArduinoSTM32/USART_support(usart.h)

ピン対応は、

Arduino\arduino-1.8.8\hardware\Arduino_STM32-master\STM32F1\variants\generic_gd32f103c\board\board.h

に記載があります。



devemin

https://twitter.com/devemin/
