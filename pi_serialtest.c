#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{ float t;
  char input[5];
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
  { if (serialGetchar(fd) == 13){
      for (int i = 0; i<2; i++){
        input[i] = serialGetchar(fd);
      }
      t = input[0]*10+input[1];                //+input[3]*0.1+input[4]*0.01;
      printf("Temperatur: %3d", t);
    }
    //putchar (serialGetchar (fd)) ;
    fflush (stdout) ;
  }
}
