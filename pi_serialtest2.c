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

    
    printf(buff);
    
  }
}
