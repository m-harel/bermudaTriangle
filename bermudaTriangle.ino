#include "Tlc5940.h"

void setup()
{
  Serial.begin(115200);
  Tlc.init(4095); // initialise TLC5940 and set all channels 
  randomSeed(analogRead(0));
}
const uint16_t Gamma [256] = {0,0,0,0,0,0,0,0,0,1,1,1,2,2,3,4,4,5,6,6,7,8,9,11,12,13,14,16,17,19,20,22,24,26,28,30,32,34,37,39,42,44,47,50,53,56,59,62,65,69,72,76,79,83,87,91,95,100,104,108,113,118,123,128,133,138,143,148,154,160,165,171,177,184,190,196,203,209,216,223,230,237,245,252,260,267,275,283,291,300,308,317,325,334,343,352,361,371,380,390,400,410,420,430,441,451,462,473,484,495,507,518,530,542,553,566,578,590,603,616,629,642,655,668,682,696,710,724,738,752,767,781,796,811,827,842,858,873,889,905,922,938,955,972,988,1006,1023,1040,1058,1076,1094,1112,1131,1149,1168,1187,1206,1225,1245,1264,1284,1304,1325,1345,1366,1386,1407,1429,1450,1471,1493,1515,1537,1560,1582,1605,1628,1651,1674,1698,1721,1745,1769,1793,1818,1843,1867,1893,1918,1943,1969,1995,2021,2047,2074,2100,2127,2154,2182,2209,2237,2265,2293,2321,2350,2379,2408,2437,2466,2496,2526,2556,2586,2616,2647,2678,2709,2740,2772,2804,2836,2868,2900,2933,2966,2999,3032,3066,3099,3133,3168,3202,3237,3271,3306,3342,3377,3413,3449,3485,3522,3558,3595,3632,3670,3707,3745,3783,3821,3860,3898,3937,3977,4016,4056,4095};
const int Rs[3] = {0,3,6};
const int Gs[3] = {1,4,7};
const int Bs[3] = {2,5,8}; 



void setAllOff()
{
  for (int i = 0; i < 9; i++)
    Tlc.set(i, 4095);     
  
  Tlc.update();
}

//Convert 0-256 to 4095-0
int byteToShort(int c)
{
  if (c<=0)
    return 4095;
  //Serial.println(c);
  if (c>200)
  {
    c= 400-c;
  }
  //Serial.println(c);
  return Gamma[255-c]; 
}

void setColor(int strip, int R, int G, int B)
{
  Tlc.set(Rs[strip],byteToShort(R));
  Tlc.set(Gs[strip],byteToShort(G));
  Tlc.set(Bs[strip],byteToShort(B));    
}

void progRotateColor(unsigned int delay_ms, unsigned long total_prog_ms)
{

  unsigned long prog_counter = 0; 
  while(prog_counter * delay_ms  < total_prog_ms )
  {    
    for (int k=0; k<3; k++) 
    {
      for (int i=0; i<200; i++) 
      {
        setColor(k%3,i,(200-i),0); 
        setColor((k+1)%3,0,i,(200-i)); 
        setColor((k+2)%3,(200-i),0,i); 
        Tlc.update();
        delay(delay_ms);
        prog_counter++;
      }
    }
  }  
}

void progOffsetBreath(unsigned int delay_ms, unsigned long total_prog_ms)
{

  unsigned long prog_counter = 0; 
  while(prog_counter * delay_ms  < total_prog_ms )
  {   
    int RGB[3]={1,2,3};

    for (int k=0; k<3; k++) 
    {
    
      for (int i=5; i<400; i++) 
      { 
        int R = (RGB[0]*i)%400;
        int G = (RGB[1]*i)%400;
        int B = (RGB[2]*i)%400;
        setColor(0,R,G,B); 
        setColor(1,B,R,G); 
        setColor(2,G,B,R);
        Tlc.update();
        delay(delay_ms*2);
        prog_counter++;
      }  
    }
  }  
}

void progBreathing(unsigned int delay_ms, unsigned long total_prog_ms)
{
  unsigned long prog_counter = 0; 
  while(prog_counter * delay_ms  < total_prog_ms )
  {    
      int k = random(0,3);
      float RGB[3]={0,0,0};
      RGB[k] = 1;
      if(random(2)) 
      {
        k = ((k+random(1,3))%3);
        RGB[k] = 1;
      }
      for (int j=0; j<random(5,15); j++) 
     {
        for (int i=5; i<400; i++) 
        { 
          int R = RGB[0]*i;
          int G = RGB[1]*i;
          int B = RGB[2]*i;
          setColor(0,R,G,B); 
          setColor(1,R,G,B); 
          setColor(2,R,G,B); 
          Tlc.update();
          delay(delay_ms*2);
          prog_counter++;
        }     
      }
    
  }  
}



void allRed(unsigned int delay_ms, unsigned long total_prog_ms)
{
  setColor(0,200,0,0); 
  setColor(1,200,0,0); 
  setColor(2,200,0,0); 
  Tlc.update();
  delay(total_prog_ms);

}

void loop()
{

  //Params: (preivitive dealy in ms, total prog time in ms) 
 //progOffsetBreath(25,10*1000L);
 if(random(2))
    progBreathing(20, 60*1000L);
 else
    progRotateColor(20, 60*1000L);
  
  

}
