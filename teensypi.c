#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{
  int fd ;

  if ((fd = serialOpen ("/dev/ttyUSB0", 9600)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

// Loop, getting and printing characters
int i
  for (i=0;i<10;i++)
  {
    putchar (serialGetchar (fd)) ;
    fflush (stdout) ;
  }
}