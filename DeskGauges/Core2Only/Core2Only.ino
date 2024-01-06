#include <M5Core2.h>
#include "font.h"
#include "sett.h"
#include "boldFont.h"




TFT_eSprite spr = TFT_eSprite(&M5.Lcd);
RTC_TimeTypeDef RTCtime;
RTC_DateTypeDef RTCDate;

String h,m,s;
int daysInMonth;
int weekDay,month,day,year,firstDay;
float vol, curr;

int setT[6]={0,0,0};
int maxT[6]={24,60,60,31,12,100};

String lblT[]={"HOUR","MIN","SEC","DAY","MONTH","YEAR"};

int posTX[6]={50,160,280,50,160,280};
int posTY[6]={90,90,90,90,90,90};
bool timeOrDate=0;
String lblTod[2]={"DATE","TIME"};
String lblBoW[2]={"BRIGHTNES","WEEKDAY"};
int BoW[2]={2,3};
bool mode=0;

String days2[7]={"SUNDAY","MONDAY","TUESDAY","WEDNESDAY","THURSDAY","FRIDAY","SATURDAY"};
String days[7]={"Mo","Tu","We","Th","Fr","Sa","Su"};
String monthNames[12]={"JANUARY","FEBRUARY","MARCH","APRIL","MAY","JUNE","JULY","AUGUST","SEPTEMBER","OCTOBER","NOVEMBER","DECEMBER"};

unsigned short grays[13];
unsigned short blue=0x1BB7;
unsigned short blue2=0x469F;
unsigned short darkOrange=0xC460 ;

float tmp      = 0.0;
float hum      = 0.0;
float pressure = 0.0;

bool light=0;
int y=0;
int x=0;

int brightness[7]={2580,2650,2725,2820,2900,3000,3200};

void setupTime() {
    RTCtime.Hours   = setT[0];  // Set the time.
    RTCtime.Minutes = setT[1];
    RTCtime.Seconds = setT[2];
    if (!M5.Rtc.SetTime(&RTCtime)) Serial.println("wrong time set!");
    RTCDate.Year  = setT[5]+2000;  // Set the date.  
    RTCDate.Month = setT[4];
    RTCDate.Date  = setT[3];
    RTCDate.WeekDay  = BoW[1];
    if (!M5.Rtc.SetDate(&RTCDate)) Serial.println("wrong date set!");
}

void setup() {
    M5.begin(true, true, true, true);   
    M5.Axp.SetLcdVoltage(brightness[BoW[0]]);
    M5.Axp.SetBusPowerMode(0);
    spr.createSprite(320,240);
    
     int co=220;
     for(int i=0;i<13;i++)
     {
     grays[i]=M5.Lcd.color565(co, co, co);
     co=co-20;
     }
 
  
}
void draw()
{

  if(mode==1)
  {
  spr.fillSprite(TFT_BLACK);
  spr.setTextDatum(4);
  
  for(int i=0;i<3;i++)
      {
        spr.setTextColor(grays[2],TFT_BLACK);
        spr.setFreeFont(&DSEG14_Modern_Bold_56);
        spr.drawString(String(setT[i+timeOrDate*3]),posTX[i+timeOrDate*3],posTY[i+timeOrDate*3]);
        spr.setFreeFont(&DejaVu_LGC_Sans_Mono_15);
        spr.setTextColor(TFT_ORANGE,TFT_BLACK);
        spr.drawString(lblT[i+timeOrDate*3],posTX[i+timeOrDate*3],posTY[i+timeOrDate*3]-44);
        
        spr.setFreeFont(&Dialog_plain_30);
        spr.setTextColor(grays[5],TFT_BLACK);
        spr.drawString("+",posTX[i+timeOrDate*3],posTY[i+timeOrDate*3]-72);
        spr.drawString("-",posTX[i+timeOrDate*3],posTY[i+timeOrDate*3]+50);
       
        
      }
        spr.fillRoundRect(0, 180, 60, 60, 4, 0x024E);
        spr.fillRoundRect(260, 180, 60, 60, 4, 0x024E);
        spr.setTextColor(TFT_WHITE,0x024E);
        spr.drawString("SET/OK",30,210,2);
        spr.drawString(lblTod[timeOrDate],290,210,2);
        spr.setTextColor(TFT_ORANGE,TFT_BLACK);
        spr.drawString(lblBoW[timeOrDate],160,198,2);
        if(timeOrDate)
        spr.drawString(days2[BoW[timeOrDate]],160,222,2);
        else
        spr.drawString(String(BoW[timeOrDate]),160,222,2);

        spr.fillRoundRect(65, 190, 50, 50, 3, grays[9]);
        spr.fillRoundRect(205, 190, 50, 50, 3, grays[9]);
        spr.setTextColor(grays[2],grays[9]);
        spr.drawString("<",90,215,2);
        spr.drawString(">",230,215,2);
  }
  
  if(mode==0){
  spr.fillSprite(TFT_BLACK);
  spr.setTextDatum(0);
  spr.fillRect(5,0,252,4,blue);
  
  spr.setFreeFont(&DSEG14_Modern_Bold_56);
  spr.setTextColor(TFT_WHITE,TFT_BLACK);
  spr.drawString(h+":"+m,10,15);
  spr.setFreeFont(&DSEG14_Modern_Bold_26);
  spr.setTextColor(0xD69A,TFT_BLACK);
  spr.drawString(s,212,44);
  
  spr.fillRect(274,0,40,18,darkOrange);
  spr.fillRect(314,4,4,10,darkOrange);
  spr.setTextColor(TFT_BLACK,darkOrange);
  spr.drawString(String(vol),278,1,2);
  spr.setTextColor(grays[3],TFT_BLACK);
  spr.drawString(String(curr),80,220,2);

  spr.fillRoundRect(273,25,47,78,3,blue);
  spr.fillRoundRect(282,28,27,37,8,grays[0]);
  spr.fillRoundRect(288,55,15,37,4,grays[0]);
  spr.fillRoundRect(292,65,7,10,2,blue);
  spr.fillRect(282,28,27,15,blue);
  spr.fillRect(282,48,27,3,blue);

  if(light)
  {
    for(int i=0;i<4;i++)
    spr.fillRoundRect(282+(i*8),29,3,9,2,grays[0]);
  }

  
  //YEAR
   spr.setTextColor(darkOrange,TFT_BLACK);
   spr.fillRect(210,82,46,2,darkOrange);
   spr.drawString(String(year),224,85,2);
   spr.drawString("BRIGHT",80,178,2);
   spr.drawString("POWER",80,205,2);

  // bright
   for(int i=0;i<7;i++)
   spr.fillRect(80+i*7,195,3,8,grays[8]);

   for(int i=0;i<BoW[0];i++)
   spr.fillRect(80+i*7,195,3,8,grays[2]);

  spr.setTextDatum(4);
  spr.fillRoundRect(0, 110, 64, 60, 4, grays[9]);
  spr.fillRoundRect(70, 110, 64, 60, 4, grays[9]);
  spr.setTextColor(grays[2],grays[9]);

   spr.drawString(String((int)tmp)+"C",32,155,4);
   spr.drawString(String((int)hum)+"%",102,155,4);
   spr.setFreeFont(&DejaVu_LGC_Sans_Mono_15);
   spr.setTextColor(blue2,grays[9]);
   spr.drawString("TMP",32,124);
   spr.drawString("HUM",102,124);
   spr.setTextColor(grays[4],TFT_BLACK);
   spr.drawString(monthNames[month-1]+" "+String(day)+", "+days2[weekDay],12,92);

   
   spr.fillRoundRect(140, 110, 180, 130, 6, grays[9]);
   spr.fillRect(150,134,160,1,grays[4]);
   spr.fillRect(12,134,40,1,darkOrange);
   spr.setTextColor(grays[0],grays[9]);
   for(int i=0;i<7;i++)
   spr.drawString(days[i],160+(i*23),124,2);

   
   int n=1;
   bool found=false;
   for(int i=0;i<5;i++)
   for(int j=0;j<7;j++)
   {
   if(j==firstDay ) found=true;
     if(found && n<=daysInMonth)  
     {
     if(n==day)
     spr.setTextColor(blue2,grays[9]);
     else
     spr.setTextColor(grays[4],grays[9]);
     spr.drawString(String(n),160+(j*23),144+(i*18),2);
     n++;
     }
   }
  spr.pushImage(2,175,64,64,sett);
  }
  spr.pushSprite(0,0);
}

int deb=0;
int deb2=0;
int deb3=0;


void loop() {

     M5.Rtc.GetTime(&RTCtime);
     M5.Rtc.GetDate(&RTCDate);

     TouchPoint_t pos= M5.Touch.getPressPoint();
     y=pos.y;
     x=pos.x;

     if(x>0 && x<70 && y>180 && y<240)
     {
       if(deb2==0){
         deb2=1;
         if(mode==0) {setT[0]=RTCtime.Hours; setT[1]=RTCtime.Minutes; setT[2]=RTCtime.Seconds;
         setT[3]=RTCDate.Date; setT[4]=RTCDate.Month; setT[5]=RTCDate.Year-2000;
         BoW[1]=RTCDate.WeekDay;}

         if(mode==1)
         setupTime();
         mode=!mode;
       }
     }else deb2=0;



     if(mode)
     {
     if(x>0){
     for(int i=0;i<3;i++){
     if(x>posTX[i+timeOrDate*3]-38 && x<posTX[i+timeOrDate*3]+38 && y>posTY[i+timeOrDate*3]-90 && y<posTY[i+timeOrDate*3])
     {setT[i+timeOrDate*3]++; if(setT[i+timeOrDate*3]==maxT[i+timeOrDate*3]) setT[i+timeOrDate*3]=0;}
     if(x>posTX[i+timeOrDate*3]-38 && x<posTX[i+timeOrDate*3]+38 && y>posTY[i+timeOrDate*3] && y<posTY[i+timeOrDate*3]+90)
     {setT[i+timeOrDate*3]--; if(setT[i+timeOrDate*3]<0) setT[i+timeOrDate*3]=maxT[i+timeOrDate*3]-1;}

      }

     if(x>260 && y>180)
     timeOrDate=!timeOrDate;

      if(x>65 && y>190 && x<115 && y<240) 
      {BoW[timeOrDate]--; if(BoW[timeOrDate]<0) BoW[timeOrDate]=6; if(!timeOrDate) M5.Axp.SetLcdVoltage(brightness[BoW[0]]);}
       if(x>205 && y>190 && x<255 && y<240) 
      {BoW[timeOrDate]++; if(BoW[timeOrDate]>6) BoW[timeOrDate]=0; if(!timeOrDate) M5.Axp.SetLcdVoltage(brightness[BoW[0]]);}

     /*
     spr.fillRoundRect(65, 190, 50, 50, 3, grays[9]);
        spr.fillRoundRect(205, 190, 50, 50, 3, grays[9]);
     */
    
     draw();   
     }}
     
     if(mode==0)
     {

     if(x>280)
     {
       if(deb==0){
         deb=1;
         light=!light;
         //if(light)  else 
         
       }
     }else deb=0;
    
    
     daysInMonth=rtcMonthDays(RTCDate.Month, RTCDate.Year);
     weekDay=RTCDate.WeekDay;
     month=RTCDate.Month;
     day=RTCDate.Date;
     year=RTCDate.Year;
     firstDay=getDayOfWeek(weekDay,day);
     vol=M5.Axp.GetBatVoltage();
     curr=M5.Axp.GetBatCurrent();

  

     if(RTCtime.Hours<10) h="0"+String(RTCtime.Hours); else  h=String(RTCtime.Hours);
     if(RTCtime.Minutes<10) m="0"+String(RTCtime.Minutes); else  m=String(RTCtime.Minutes);
     if(RTCtime.Seconds<10) s="0"+String(RTCtime.Seconds); else  s=String(RTCtime.Seconds);
     draw();   

     }

     
}

int rtcMonthDays(int month, int year) {
  if (month == 2) {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
      return 29;  // Prijestupna godina
    } else {
      return 28;  // Nije prijestupna godina
    }
  } else if (month == 4 || month == 6 || month == 9 || month == 11) {
    return 30;
  } else {
    return 31;
  }
}

int getDayOfWeek(int wd, int day) {
  int n=wd;
  for(int i=day;i>0;i--)
  {n--; if(n<0) n=6;}
  return n;
}