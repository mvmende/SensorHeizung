#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{ float temp;
  char input[5];
  int fd ;
  int i;

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
    for(i=0, i<5, i++) {
    input[i] = serialGetchar (fd);
    }
    temp = atof(input);
    printf("%f\n",temp);
    //putchar (serialGetchar (fd)) ;
    fflush (stdout) ;
  }
}
