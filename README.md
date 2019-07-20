# Pre-maiduino
Pre-maid AI x Arduino STM32 collaboration

# このページについて
プリメイドAI に内蔵されているSTM32F102 マイコンで Arduinoを使うための非公式ゾーンです。


# はじめに

事前知識として、プリメイドAIはマイコンSTM32<b> F102 </b>を使用しています。

私が作ったボードも<b> F102 </b>搭載です。

こちらで紹介するArduino書き換え方法は、本来は<b> F103 </b>向けです。

どちらもほぼ同じスペックのため、対象が違っても動作しているようです。

（スペック比較はこちらのツイート　→　https://twitter.com/devemin/status/1151756254835986432?s=20 ）

また、ファーム書き換えは自己責任にてお願いします。

保護のかかったファームを書き換え後は、元ファームは消去され、保存できません。

間違いがありましたら、お教えください。

このページは、あくまで私が作った<b> F102 </b>ボード向けの情報となっている体裁ですのでよろしくお願いします。
  
（メーカー様に怒られないよう）


# 参考情報

超速ハックの @GOROman さんによる解析データです。

https://docs.google.com/spreadsheets/d/1c6jqMwkBroCuF74viU_q7dgSQGzacbUW4mJg-957_Rs/edit#gid=191676625

ST公式データシート

F102 F103はピン配置は全く同じのようです（LQFP48）

# 環境構築

書き換えに必要なものは、ST Linkというアダプタです。

Amazonや秋月電子で販売しています。（1000～3000円）

http://akizukidenshi.com/catalog/g/gM-04012/

また、ボード上のSWDという4～5個のピンにワイヤーを接続する必要がありますので、必要に応じて用意してください。

秋月でしたら、ブレッドボード用のジャンパワイヤーとICクリップタイプ－小型クリップのケーブルなどが良いのではないでしょうか。

用意できましたら、ボード上のSWD, SWDCLK, GND, 3.3V, (Reset不要かも？)をケーブルにて接続してください。

他の場所との電気的ショートに要注意です。

しつこいようですが、書き換えについては、私はまだプリメイドAIは届いてないので、あくまで自作ボード用の話という体裁です。


# ファーム書き換え　準備

プリメイドAIにも搭載されているF102にはファーム書き換え保護が設定されているようです。

STM32CubeProgrammer というソフトでその設定をオン・オフできます。

https://www.st.com/ja/development-tools/stm32cubeprog.html#tools-software

ST社サイトへの登録が必要です。

こんな感じです。

https://twitter.com/devemin/status/1150963921752215552?s=20

ダウンロードし、ST　Linkで接続して起動すると「読取エラー」が出る時は、保護機能が働いているためです。

左側メニュー「OB」アイコン、Option Bytes より保護オプションタブを開き、一番うえのRead Out Protection のチェックを外し、Apply します。

# ファーム書き換え　Arduino

さて、ファーム書き換えが可能になったあとは、こちらのサイトが詳しく書いてあります。（F103向け）

 →　https://qiita.com/nanbuwks/items/ba37f049f45378da0e2b

必要なところだけ記載すると、

　Arduino IDE ダウンロード＆インストール

　STM32ボード情報をArduino IDEに追加（下記をダウンロード）

→　https://github.com/rogerclarkmelbourne/Arduino_STM32/

Arduino IDE ディレクトリに、Arduino/hardware/Arduino_STM32-master/ という形になるようにフォルダごとコピー。

その後Arduino IDEを起動すると、メニューのツール→ボード　に「Generic STM32F103C series」が追加されていますので選択。

Variant は(20k RAM, 64K flash) の方を選択。

Upload Method : ST Link を選択。

CPU Speed　は48MHz を選択。

どうやらブートローダーの書き込みまではいらないようです。

別途持っているSTM32F103搭載のBluePillというボードでは、ボード上ピンBOOt0:0, BOOT1:0 で書き込み、動作とも出来ました。

詳細飛んでいるところもあるかもしれませんが、ひとまず書きました。

# Arduino STM32のtips

STM32 のArduinoは一部通常のArduinoと変数など扱いが異なる部分があるようです。

下記サイトではSTM32 Arduinoについてのリファレンスを翻訳されています。ありがたく参考にさせてもらいましょう！

https://scrapbox.io/ArduinoSTM32/Arduino_STM32_%E3%83%AA%E3%83%95%E3%82%A1%E3%83%AC%E3%83%B3%E3%82%B9_%E6%97%A5%E6%9C%AC%E8%AA%9E%E7%89%88

# Bluetooth RN42

シリアル接続ですので簡単です。
<code>

setup() で

Serial2.begin(115200);

読取処理で

Serial2.print("hoge");

などでBluetooth 接続のホスト相手にデータを送信できます。  

</code>

# 各種ライブラリの使い方(近藤科学ICSサーボ、ジャイロMPU-6050など)

# 近藤科学サーボ　公式ライブラリ

公式のサンプル KrsServo1 はそのままですと動きません。変数を&Serial から&Serial1 等に変更してください。

USART1,3 がシリアルサーボに接続されているようです。

USART2 はBluetooth チップRN42につながっています。

Serial3がうまく接続できなくて調査中です。

先程の翻訳リファレンスによると、書き込み方法が変わると、Serialの扱いが変わるようです。

処理はこちらに書いてありました。

Arduino\arduino-1.8.8\hardware\Arduino_STM32-master\STM32F1\variants\generic_gd32f103c\board.cpp

さて、サンプルコードの修正箇所です。

<code>

//物理的接続はPA9?

const byte EN_PIN = PA2;
const long BAUDRATE = 1250000;

IcsHardSerialClass krs(&Serial1,EN_PIN,BAUDRATE,TIMEOUT);  //インスタンス＋ENピン(2番ピン)およびUARTの指定

//あるいは

//物理的接続はPA2?

const byte EN_PIN = PA9;
const long BAUDRATE = 1250000;

IcsHardSerialClass krs(&Serial2,EN_PIN,BAUDRATE,TIMEOUT);  //インスタンス＋ENピン(2番ピン)およびUARTの指定

//Serial3は調査中

ピン対応は、

Arduino\arduino-1.8.8\hardware\Arduino_STM32-master\STM32F1\variants\generic_gd32f103c\board\board.h

に記載があります。

</code>

（書き込み方法をST LinkでなくSerialケーブルで書き込む方は、この処置はいりません。）

# ジャイロ MPU-6050 について

こちらのサイトを参考にさせてもらおうと思っています。

https://qiita.com/Qikoro/items/d24057b434c44fcdf74e

# ハードウェア追加について(SPI、I2Cなど)

プリメイドAIのボード上にはSPI端子用コネクタ（ZH）があるようです。

周辺機器接続あるいは他端末と高速通信が出来るとおもいます。




# 注意点
(私はプリメイドAI自体がまだ届いていませんので、あくまで自作のSTM32F102ボード向けの情報となっています。)

(メーカー様に怒られない範囲で)ワクワク楽しくハックしていきたいと思います！

(定型文)自己責任、自己範囲内でお楽しみ下さい。

devemin

https://twitter.com/devemin/
