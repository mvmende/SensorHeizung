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
  
 
  if ((fd = serialOpen ("/dev/ttyACM0", 9600)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

// Loop, getting and printing characters

//  while (serialDataAvail (fd))
  //  {delay(2000);
    //  printf ("%3d", serialGetchar (fd)) ;
      //fflush (stdout) ;
    //}
  
  for (;;)
  { 
    if (serialGetchar(fd) == 't'){
      int i = 0;
      while (serialGetchar(fd) != '\r'){
        in_temp[i] = serialGetchar(fd);
        i = i+1;
      }
    }
     if (serialGetchar(fd) == 'h'){
       int i = 0;
       while (serialGetchar(fd) != '\r'){
        in_hum[i] = serialGetchar(fd);
        i = i+1;
      }
     }
     if (serialGetchar(fd) == 'a'){
       int i = 0;
       while (serialGetchar(fd) != '\r'){
        in_heiz[i] = serialGetchar(fd);
        i = i+1;
      }
     }
     if (serialGetchar(fd) == 'b'){
       int i = 0;
       while (serialGetchar(fd) != '\r'){
        in_luft[i] = serialGetchar(fd);
        i = i+1;
      }
     }
      t = atof (in_temp);
      h = atof (in_hum);
      heiz = atof (in_heiz);
      luft = atof (in_luft);
      printf("Temperatur:%.2f Grad Celsius \n", t);
      printf("Luftfeuchtigkeit:%.2f %% \n", h);
      printf("Heizleistung: %.2f %% \n", heiz);
      printf("Lüfterleistung: %.2f %% \n", luft);
      
      fflush (stdout) ;
      delay(10000);
   }



 
}
