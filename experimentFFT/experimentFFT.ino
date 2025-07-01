#include <arduinoFFT.h>

#define N   128        // 128 点に減らす
#define F_S 9600
ArduinoFFT<float> FFT; // double → float へ
float vReal[N], vImag[N];

void setup() {
  Serial.begin(115200);
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

  /* 3. 出力 */
  for (uint16_t i = 1; i < N / 2; i++) {
    Serial.print((uint16_t)vReal[i]);
    Serial.print(i == N / 2 - 1 ? '\n' : ',');
  }
}