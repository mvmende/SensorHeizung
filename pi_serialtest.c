#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{ int t;
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
  { if (serialGetchar(fd) == 13){
      for (int i = 0; i<2; i++){
        input[i] = serialGetchar(fd);
      }
      t = atoi (input);
      printf("%d",t);
    }
    //putchar (serialGetchar (fd)) ;
    fflush (stdout) ;
  }
}
