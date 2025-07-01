#include <arduinoFFT.h>

#define N   128        // 128 点に減らす
#define F_S 9600
ArduinoFFT<float> FFT; // double → float へ
float vReal[N], vImag[N];
const int ledPin =13;
const uint8_t TARGET_BIN = 40;  // 3kHzに相当するピン番号
#define THRESHOLD 23 // 振幅しきい値

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // 初期状態で消灯
  // Wiring check: blink LED twice at startup
  for (int i = 0; i < 2; i++) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
  ADCSRA = (ADCSRA & 0b11111000) | 0x04; // prescaler=16
}

void loop() {
  /* 1. サンプリング */
  for (uint16_t i = 0; i < N; i++) {
    vReal[i] = analogRead(A0);
    vImag[i] = 0;
  }

  /* 2. FFT */
  FFT.windowing(vReal, N, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(vReal, vImag, N, FFT_FORWARD);
  FFT.complexToMagnitude(vReal, vImag, N);


  if(vReal[TARGET_BIN] > THRESHOLD){
    digitalWrite(ledPin, HIGH); //しきい値を超えたら点灯
  } else{
    digitalWrite(ledPin,LOW); //それ以外は消灯
  }

  /* 3. 出力 */
  for (uint16_t i = 1; i < N / 2; i++) {
    Serial.print((uint16_t)vReal[i]);
    Serial.print(i == N / 2 - 1 ? '\n' : ',');
  }
}