#include <U8g2lib.h>
#include "PMS.h"

// OLED 초기화 (소프트웨어 I2C 사용, 클럭 핀: GPIO 22, 데이터 핀: GPIO 21)
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 22, /* data=*/ 21, /* reset=*/ U8X8_PIN_NONE);

// PMS 센서 초기화
PMS pms(Serial2);
PMS::DATA data;

const int powerPin = 13;  // 전원 공급을 위한 GPIO 핀 정의

void setup()
{
  // GPIO 13번 핀을 전원 공급 핀으로 설정
  pinMode(powerPin, OUTPUT);  // GPIO 13번 핀을 출력 모드로 설정
  digitalWrite(powerPin, HIGH);  // GPIO 13번 핀을 HIGH로 설정하여 전원 공급

  delay(100);  // 잠시 대기하여 전원이 안정화될 시간을 줌

  // PMS 센서 시리얼 통신 설정
  Serial2.begin(9600, SERIAL_8N1, 16, 17);   // TX = GPIO 17, RX = GPIO 16
  Serial.begin(9600);  // 시리얼 모니터 설정

  // OLED 초기화
  u8g2.begin();
  u8g2.clearDisplay();
}

void loop()
{
  if (pms.read(data))
  {
    // PM2.5 값을 시리얼 모니터에 출력
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);

    // PM2.5 값을 세 자리 숫자로 포맷팅
    char pm25str[4];  // 세 자리 숫자와 null terminator를 위한 공간
    snprintf(pm25str, sizeof(pm25str), "%03d", data.PM_AE_UG_2_5);


    int randomOffset0 = random(0, 5); // -5에서 5픽셀 사이의 랜덤 오프셋
    int randomOffset1 = random(-2, 5); // -5에서 5픽셀 사이의 랜덤 오프셋

    // OLED에 PM2.5 값만 출력
    u8g2.clearBuffer();                        // 버퍼 지우기
    u8g2.setFont(u8g2_font_fub42_tn);          // 큰 폰트 설정 (높이 약 42 픽셀)
    int x = 11; // 텍스트를 중앙 정렬하기 위한 x 좌표 계산
    int y = 62;        // 텍스트를 중앙에 맞추기 위한 y 좌표 계산
    u8g2.drawStr(x- randomOffset0, y - randomOffset1, pm25str);                // OLED 화면에 텍스트 출력

    // 'ug' 단위를 작은 폰트로 표시
    u8g2.setFont(u8g2_font_fur11_tr);           // 작은 폰트 설정 (높이 약 11 픽셀)
    //u8g2.drawStr(112- randomOffset0, 57 - randomOffset1, "ug"); 
    u8g2.drawStr(x + 101 - randomOffset0, y - 5 - randomOffset1, "ug");  
    u8g2.drawStr(x+3 - randomOffset0, y - 50 - randomOffset1, "PM2.5"); 
    u8g2.sendBuffer();                         // 디스플레이 업데이트

    delay(5000);  // 5초 대기 후 다시 데이터 읽기
  }
}
