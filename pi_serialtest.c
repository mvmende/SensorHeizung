#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{ float t;
  float h;
  char input[10];
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
  { if (serialGetchar(fd) == 116){
      for (int i = 0; i<6; i++){
        input[i] = serialGetchar(fd);
      }
      t = atof (input);
      printf("Temperatur:%.2f \n", t);
      fflush (stdout) ;
    }
    else{
      if (serialGetchar(fd) == 104){
        for (int i = 0; i<6; i++){
          input[i] = serialGetchar(fd);
        }
      h = atof (input);
      printf("Luftfeuchtigkeit:%.2f \n", h);
      fflush (stdout) ;
      }
    }
    else{
      continue;
    }
  }
 
  //for (;;)
  //{ if (serialGetchar(fd) == 10){
    //  int i=0;
     // while(serialGetchar(fd)!=13){
      //  input[i] = serialGetchar(fd);
       // i++;
     // }
     // t = atof (input);
     // printf("Temperatur:%.2f \n", t);
    //}
    //putchar (serialGetchar (fd)) ;
    //fflush (stdout) ;
  //}
}
