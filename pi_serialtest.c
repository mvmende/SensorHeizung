#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{
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
    putchar (serialGetchar (fd)) ;
    fflush (stdout) ;
  }
}
