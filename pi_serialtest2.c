#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{ 
  float t;
  float h;
  float heiz;
  float luft;
  char in_temp[10];
  char in_hum[10];
  char in_heiz[10];
  char in_luft[10];
  int fd ;
  char buff[50];
  
 
  if ((fd = serialOpen ("/dev/ttyACM0", 9600)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

  for(;;)
  {
    if (serialGetchar(fd) == 't')
    {
      int i = 0;
      while (serialGetchar(fd) != '\r')
      {
        buff[i] = serialGetchar(fd);
        i = i+1;
      }
    }
    int j = 0;
    if (buff[j] == 't')
    {
     for (int i = 0; i<4; i++)
     {
       in_temp[i] = buff[j];
       j = j+1;
     }
    }
    
    //if (buff[j] == 'h')
    //{
     for (int i = 0; i<4; i++)
     {
       in_hum[i] = buff[j];
       j = j+1;
     }
    //}
    
    //if (buff[j] == 'a')
    //{
     for (int i = 0; i<3; i++)
     {
       in_heiz[i] = buff[j];
       j = j+1;
     }
    //}
    
    //if (buff[j] == 'b')
    //{
     for (int i = 0; i<3; i++)
     {
       in_luft[i] = buff[j];
       j = j+1;
     }
    //}
    
    t = atof (in_temp);
    h = atof (in_hum);
    heiz = atof (in_heiz);
    luft = atof (in_luft);
    printf("Temperatur:%.2f Grad Celsius \n", t);
    printf("Luftfeuchtigkeit:%.2f %% \n", h);
    printf("Heizleistung: %.2f %% \n", heiz);
    printf("Lüfterleistung: %.2f %% \n", luft);
      
    
  }
}
