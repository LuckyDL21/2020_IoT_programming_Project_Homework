
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED 가로 넓이, 픽셀 사이즈
#define SCREEN_HEIGHT 64 // OLED 세로 넓이, 픽셀 사이즈
 
#define OLED_RESET  -1 // 리셋핀 #(또는 -1 아두이노와 리셋핀을 연결하는 경우)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//온습도 라이브러리
#include "DHT.h"
#define DHTPIN 2     // GPIO 2번에 해당
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);


//사용자 조작 수 변수 
int count=0;

//사용자가 지정한 시간부터 시계의 기능이 시작됨.
unsigned long currentHr=02;
unsigned long currentMin=57;

int ledpin=12;//int ledpin=D12;와 동일(LED 핀번호)

//알람시간 조정 변수
int hr_count=0;
int min_count=0;
int alarm=0;

// 스톱워치 조정 변수 
unsigned long st_watch=0;
int val=0;

int stop_sec = 0;
int stop_min = 0;
int stop_hour =0;
int stop_counter =0;

// * 추가기능(1)변수:월,일,요일 표시기능 

int month=5;
int day_num=25;
String day_arr[]={"MON","TUE","WED","THU","FRI","SAT","SUN"};

String day_str="TUE";

int day_plus=1;

// * 추가기능(2): 듀얼타임 표시(LA) 시차:16시간

unsigned long LA_hr;
unsigned long LA_min;
unsigned long LA_sec;



void setup() {
  pinMode(ledpin,OUTPUT);
  digitalWrite(ledpin,LOW);
  
  Wire.begin();
  dht.begin();
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  display.clearDisplay();//첫번째 메세지를 화면에 출력하기 이전에 버퍼를 지운다
  display.display();
  delay(500);
  
}


void alarm_setting(int set_hr,int set_min,unsigned long now_hr,unsigned long now_min,unsigned long now_sec,int on_off){
  if(on_off==1){
  if((set_hr==now_hr)&&(set_min==now_min)){
    if(now_sec<=10){
    
    digitalWrite(ledpin,HIGH);
    }
    //delay(10000);
    else{
    digitalWrite(ledpin,LOW);
    }
    }
  }
  else{
    digitalWrite(ledpin,LOW);
    }
  }



void loop() {
  
  unsigned long currentMillis = millis();
  unsigned long hr = currentHr + currentMillis/3600000;//시
  currentMillis = currentMillis%3600000;
  unsigned long minute = currentMin + currentMillis/60000;//분
  unsigned long second = currentMillis%60000;
  unsigned long sec = second/1000;//초

  if( minute >= 60 ){
      hr = hr + 1;
      minute = minute - 60;
    }
  //일,요일 구현 
  if(hr>=24){
    hr=0;
    day_num+=1;
    day_plus+=1;
    if(day_plus==7){
      day_plus=0;
      }
    day_str=day_arr[day_plus];
    
    } 

  //월 구현 
  if((month==4)||(month==6)||(month==9)||(month==11)){
    if(day_num>=31){
      day_num=day_num-30;
      month+=1;
      }
    }
  else if ((month==1)||(month==3)||(month==5)||(month==7)||(month==8)||(month==10)||(month==12)){
    if(day_num>=32){
      day_num=day_num-31;
      month+=1;
      }
    }
  else if(month==2){
    if(day_num>=29){
      day_num=day_num-28;
      month+=1;
      }
    }
  else if(month==13){
    month=1;
    }

  // 듀얼시간 구현 

  LA_sec=sec;
  LA_min=minute;
  String LA_time="";

  if(hr<4){
    LA_hr=(hr+24)-16;
    LA_time="AM";
    
    }
  else if(4<=hr<16){
    LA_hr=(hr+12)-16;
    LA_time="PM";
    }
  else{
    LA_hr=hr-16;
    LA_time="AM";
    }

  //온습도
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }


  alarm_setting(hr_count,min_count,hr,minute,sec,alarm);//알람켜기 함수 실행 

  
  if((currentMillis-st_watch>1000)&&val==1){//스톱워치 작동
    st_watch=currentMillis;
    stop_sec++;

    if(stop_sec>=60){
      stop_min=stop_min+1;
      stop_sec=stop_sec-60;
      }

    if(stop_min>=60){
      stop_hour=stop_hour+1;
      stop_min=stop_min-60;
      }


    delay(500);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(5,10);
    display.println("stop watch");
    display.setCursor(15,35);
    display.setTextSize(2);
    display.print(stop_hour);
    display.print(":");
    display.print(stop_min);
    display.print(":");    
    display.println(stop_sec);
    display.display();
    
      
    }


  if(count==0){//기능(1):시간 display+추가기능:월,일,요일 표시,온습도,듀얼시계
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1.5);
    display.setCursor(15,10);
    display.print(month);
    display.print("  ");
    display.print(day_num);
    display.print("  ");
    display.println(day_str);
    

    
    
    display.setTextSize(1.5);
    display.setCursor(2,25);
    if( hr <12 ) {
      display.print("AM");
    }
    else if( hr <24 ) {
      display.print("PM");
    }
    else {
      hr = hr -24;
      display.print("AM");
    }
    display.setTextSize(2);
    display.setCursor(20,25);
    //if( minute >= 60 ) {
      //hr = hr + 1;
      //minute = minute - 60;
    //}
    display.print((unsigned long)hr);
    display.print(":");
    display.print((unsigned long)minute);
    display.print(":");
    display.println((unsigned long)sec);   

    display.setTextSize(1.5);
    display.setCursor(2,45);
    display.print("LA  ");
    display.print(LA_time);
    display.print(" ");
    display.print(LA_hr);
    display.print(":");
    display.print(LA_min);
    display.print(":");
    display.print(LA_sec);

    display.setTextSize(1);
    display.setCursor(2,55);
    display.print("Humid:");
    display.print(int(h));
    display.print("% Temp:");
    display.print(int(t));
    display.print("C");
    
    
    display.display();
    delay(500);

    }

    if(Serial.available()){//사용자로부터 입력 받음
      long get_num1=Serial.parseInt();
      long get_num2=Serial.parseInt();

      if(get_num1==1){//1 번키를 눌러야 작동한다는 
        count=count+1;
        //Serial.print("count:");
        //Serial.println(count);
        if((count==1)&&(get_num2==0)){//기능(2) 알람시간 설정해주는 기능 
            delay(500);
            display.clearDisplay();
            display.setTextColor(WHITE);
            display.setTextSize(2);
            display.setCursor(5,10);
            display.println("alarm ON");
            display.setCursor(5,30);
            display.println("push 1-1");
            display.display();
            
          }

       if((get_num2==-2)&&(count==1)){//알람 끄기 기능 
        alarm=0;
        delay(500);
        display.clearDisplay();
        display.setTextColor(WHITE);
        display.setTextSize(2);
        display.setCursor(5,10);
        display.println("alarm OFF");
        display.display();
        delay(3000);
        count=0;
        
        }
          
        if((get_num2==-1)&&(count==2)){
          alarm=1;//알람 켜준다. 
          //Serial.println("*시 시간 조정*");
          if(hr_count<24){
            hr_count=hr_count+1;
          }
          else{
            hr_count=hr_count-24;
            }
          delay(500);
          display.clearDisplay();
          display.setTextColor(WHITE);
          display.setTextSize(1);
          display.setCursor(5,10);
          display.println("Hour setting");
          display.setTextSize(3);
          display.setCursor(5,30);
          if(hr_count<10){
            display.print("0");
            display.print(hr_count);
            display.print(":00");
            }
          else{
            display.print(hr_count);
            display.print(":00");
            }
            
          display.display();
          
          count=count-1;//count수가 늘어 나지 않도록 
          }

        if((get_num2==-2)&&(count==2)){
          //Serial.println("*분 시간 조정*");
          if(min_count<60){
            min_count=min_count+1;
            }
          else{
            min_count=min_count-60;
            }
          delay(500);
          display.clearDisplay();
          display.setTextColor(WHITE);
          display.setTextSize(1);
          display.setCursor(5,10);
          display.println("Min setting");
          display.setTextSize(3);
          display.setCursor(5,30);
          if(min_count<10){
            if(hr_count<10){
              display.print("0");
              display.print(hr_count);
              display.print(":0");
              display.print(min_count);
              }
            else{
              display.print(hr_count);
              display.print(":0");
              display.print(min_count);
              }
          }
         else{
          if(hr_count<10){
            display.print("0");
            display.print(hr_count);
            display.print(":");
            display.print(min_count);
            }
          else{
            display.print(hr_count);
            display.print(":");
            display.print(min_count);
            }
          }
          display.display();
          count=count-1;//count수가 늘어 나지 않도록 
         }
        if((count==2)&&(get_num2==0)){//기능(3) 스톱워치 기능
          //Serial.println("*스톱워치*");
          delay(500);
          display.clearDisplay();
          display.setTextColor(WHITE);
          display.setTextSize(2);
          display.setCursor(5,10);
          display.println("stop watch");
          display.setCursor(10,35);
          display.setTextSize(2);
          display.println("00:00:00");
          display.display();
        }

          if((get_num2==-1)&&(count==3)&&(val==0)){
            //Serial.println("*스톱워치 작동*");
            val=1;
            count=count-1;
            }

          if((get_num2==-1)&&(count==3)&&(val==1)){
            //Serial.println("*스톱워치 멈춤*");
            val=0;
            count=count-1;
            
            }

         if((get_num2==-2)&&(count==3)){
          val=0;
          stop_sec=0;
          stop_min=0;
          stop_hour=0;
          delay(500);
          display.clearDisplay();
          display.setTextColor(WHITE);
          display.setTextSize(2);
          display.setCursor(5,10);
          display.println("stop watch");
          display.setCursor(15,35);
          display.setTextSize(2);
          display.print(stop_hour);
          display.print(":");
          display.print(stop_min);
          display.print(":");    
          display.println(stop_sec);
          display.display();

          count=count-1;
          }

        if((count==3)&&(get_num2==0)){
           count=0;
          
          }

            
       }
    

    }
      
      
    }
