/*
 * ICS_test.ino
 * 
 * Arduino_STM32 with Kondo ICS servo 
 * Sample coding.
 * 
 * Apache 2.0 License
 * 
 * 2019/7/30
 * 
 * devemin
 * https://github.com/devemin
 * https://twitter.com/devemin
 * 
 * 
 */


#include <libmaple/usart.h>

#define MV_MOTORNUM 4
#define HV_MOTORNUM 1



//シリアル通信レジスタ
struct usart_reg_map *regmap2 = USART2_BASE;
struct usart_reg_map *regmap3 = USART3_BASE;


void setup() {
  // put your setup code here, to run once:

  //Bluetooth RN42 用通信設定
  Serial1.begin(115200);

  //ICSサーボ設定　信号線を起動時500msec Highにすることで、シリアル通信になる（PWM ×）
  digitalWrite(PA2, HIGH);
  delay(505);
  digitalWrite(PA2, LOW);
  
  digitalWrite(PB10, HIGH);
  delay(505);
  digitalWrite(PB10, LOW);

  //ICSサーボ通信設定 (8bit,even)
  Serial2.begin(1250000,SERIAL_8E1);
  Serial2.setTimeout(500);
  Serial3.begin(1250000,SERIAL_8E1);
  Serial3.setTimeout(500);

  //1-wire USART 用設定　逆に設定すると動かない、不要？
  //pinMode(PA2, OUTPUT_OPEN_DRAIN);
  //pinMode(PB10, OUTPUT_OPEN_DRAIN);

  //HDSELビットをオンにし、1-wire USARTを有効にする
  regmap2->CR3 = regmap2->CR3 | 0b00000000000000000000000000001000;
  regmap3->CR3 = regmap3->CR3 | 0b00000000000000000000000000001000;


  //レジスタ表示デバッグ用
/*
  //シリアル通信レジスタ
  //regmap3->SR;  //regmap3->DR;  //regmap3->BRR;  //regmap3->CR1;
  //regmap3->CR2;  //regmap3->CR3;  //regmap3->GTPR;

  String tmpstr;
  int strsize=0;
  
  Serial1.print("USART1 register reading\r\n");
  ///////////////////////////////////////////////////////////////////////
  Serial1.print("Register  SR: 0b");
  tmpstr = String(regmap3->SR,BIN);
  for (int a=tmpstr.length()-1; a<=50-1; a++) {    tmpstr = '0' + tmpstr;  }
  Serial1.print(tmpstr + "\r\n");
  ///////////////////////////////////////////////////////////////////////
  Serial1.print("Register  DR: 0b");
  tmpstr = String(regmap3->DR,BIN);
  for (int a=tmpstr.length()-1; a<=50-1; a++) {    tmpstr = '0' + tmpstr;  }
  Serial1.print(tmpstr + "\r\n");
  ///////////////////////////////////////////////////////////////////////
  Serial1.print("Register BRR: 0b");
  tmpstr = String(regmap3->BRR,BIN);
  for (int a=tmpstr.length()-1; a<=50-1; a++) {    tmpstr = '0' + tmpstr;  }
  Serial1.print(tmpstr + "\r\n");
  ///////////////////////////////////////////////////////////////////////
  Serial1.print("Register CR1: 0b");
  tmpstr = String(regmap3->CR1,BIN);
  for (int a=tmpstr.length()-1; a<=50-1; a++) {    tmpstr = '0' + tmpstr;  }
  Serial1.print(tmpstr + "\r\n");
  ///////////////////////////////////////////////////////////////////////
  Serial1.print("Register CR2: 0b");
  tmpstr = String(regmap3->CR2,BIN);
  for (int a=tmpstr.length()-1; a<=50-1; a++) {    tmpstr = '0' + tmpstr;  }
  Serial1.print(tmpstr + "\r\n");
  ///////////////////////////////////////////////////////////////////////
  Serial1.print("Register CR3: 0b");
  tmpstr = String(regmap3->CR3,BIN);
  for (int a=tmpstr.length()-1; a<=50-1; a++) {    tmpstr = '0' + tmpstr;  }
  Serial1.print(tmpstr + "\r\n");
  ///////////////////////////////////////////////////////////////////////
  Serial1.print("RegisterGTPR: 0b");
  tmpstr = String(regmap3->GTPR,BIN);
  for (int a=tmpstr.length()-1; a<=50-1; a++) {    tmpstr = '0' + tmpstr;  }
  Serial1.print(tmpstr + "\r\n");
  
  Serial1.print("\r\n\r\n");
  delay(2000);
*/

}


void loop() {

  //複数サーボ　ポジション値送信・戻り値テスト
  int movepos = 7000;
  for (int a=500; a<=1000; a+=250) {
    for (int b=1; b<=MV_MOTORNUM; b++) {
      SetICSServoPos(Serial3, b, movepos - a);
      SetICSServoPos(Serial3, b, movepos + a);
    }  
    for (int c=1; c<=HV_MOTORNUM; c++) {
      SetICSServoPos(Serial2, c, movepos - a);
      SetICSServoPos(Serial2, c, movepos + a);
    }  
  }

  //複数サーボ　温度センサ読み出しテスト
/*
    for (int b=1; b<=MV_MOTORNUM; b++) {
      GetICSServoTemp(Serial3, b);
    }  
    for (int c=1; c<=HV_MOTORNUM; c++) {
      GetICSServoTemp(Serial2, c);
    }  
*/

}


bool SetICSServoPos(HardwareSerial &refSer, uint8 id, uint16 pos) {

  int rxSize;
  int rxLen = 3;
  
  byte txCmd[3];
  byte rxCmd[3];

  String getid;
  String getpos;
  uint16 posnow;

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  txCmd[0] = 0x80 + id; //CMD
  txCmd[1] = ((pos >> 7) & 0x007F); //POS_H
  txCmd[2] = (pos & 0x007F); //POS_L

  //TXビット　オン　/ RXビット　オフ
  regmap2->CR1 = (regmap2->CR1 & 0b111111111111111111111111111110011)  | 0b000000000000000000000000000001000;
  regmap3->CR1 = (regmap3->CR1 & 0b111111111111111111111111111110011)  | 0b000000000000000000000000000001000;
  delay(10);

  refSer.flush();
  refSer.write(txCmd,3);
  refSer.flush();

  //TXビット　オフ　/ RXビット　オン
  regmap2->CR1 = (regmap2->CR1 & 0b111111111111111111111111111110011)  | 0b000000000000000000000000000000100;
  regmap3->CR1 = (regmap3->CR1 & 0b111111111111111111111111111110011)  | 0b000000000000000000000000000000100;
  delay(10);

  rxCmd[0]=0;
  rxCmd[1]=0;
  rxCmd[2]=0;

  refSer.flush();
  rxSize = refSer.readBytes(rxCmd,3);
  refSer.flush();

  if (rxSize != rxLen)
  {
    Serial1.print("rxSize != rxLen error");
    return false;
  }

  getid = String(rxCmd[0], DEC);
  posnow = (rxCmd[1] << 7) | rxCmd[2];
  getpos = String(posnow, DEC);

  Serial1.print("getid: " + getid + ", getpos: " + getpos + "\r\n");

  delay(500);

  return true;
}


byte GetICSServoTemp(HardwareSerial &refSer, uint8 id) {
  
  int rxSize;
  int rxLen = 3;
  
  byte tmp = 0;
  byte txCmd[2];
  byte rxCmd[3];

  //String getid;
  String gettmp;

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////


  txCmd[0] = 0xA0 + id;
  txCmd[1] = 0x04;

  //TXビット　オン　/ RXビット　オフ
  regmap2->CR1 = (regmap2->CR1 & 0b111111111111111111111111111110011)  | 0b000000000000000000000000000001000;
  regmap3->CR1 = (regmap3->CR1 & 0b111111111111111111111111111110011)  | 0b000000000000000000000000000001000;
  delay(10);

  refSer.flush();
  refSer.write(txCmd,2);
  refSer.flush();

  //TXビット　オフ　/ RXビット　オン
  regmap2->CR1 = (regmap2->CR1 & 0b111111111111111111111111111110011)  | 0b000000000000000000000000000000100;
  regmap3->CR1 = (regmap3->CR1 & 0b111111111111111111111111111110011)  | 0b000000000000000000000000000000100;
  delay(10);
 
  rxCmd[0]=0;
  rxCmd[1]=0;
  rxCmd[2]=0;

  refSer.flush();
  rxSize = refSer.readBytes(rxCmd,3);
  refSer.flush();

  if (rxSize != rxLen)
  {
    //Serial1.print("rxSize != rxLen error");
    //return false;
  }

  gettmp = String(rxCmd[0], HEX) +","+ String(rxCmd[1], HEX) +","+ String(rxCmd[2], HEX)  ;

  Serial1.print("gettmp: " + gettmp + "\r\n");

  delay(10);

  return tmp;
}
