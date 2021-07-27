#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* YOU WILL HAVE TO CHANGE THESE FIVE LINES TO MATCH YOUR INFLUXDB CONFIG */
#define PORT        8086          /* Port number as an integer - web server default is 80 */
#define IP_ADDRESS "127.0.0.1"    /* IP Address as a string */
#define DATABASE "SensorKlima"
#define USERNAME "klima"
#define PASSWORD "amsprofi!"

/* client endpoint details for a tag: replace with your hostname or use gethostname() */
#define HOSTNAME "NanoPi-NEO2-Black"
#define SECONDS 120
//#define LOOPS   100
#define BUFSIZE 8196

pexit(char * msg)
{
    perror(msg);
    exit(1);
}

int main ()
{ 
  float t;      //Variablen initialisieren zur spaeteren Speicherung der Parameter
  float h;
  float heiz;
  float luft;
  char in_temp[10];     //Input Arrays initialisieren
  char in_hum[10];
  char in_heiz[10];
  char in_luft[10];
  int fd ;
  int sockfd;
  int loop;
  int ret;
  char header[BUFSIZE];
  char body[BUFSIZE];
  char result[BUFSIZE];
  
  if ((fd = serialOpen ("/dev/ttyACM0", 9600)) < 0)     //serielles Geraet mit Adresse "/dev/ttyACM0" oeffnen
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ; //Fehler ausgeben falls Oeffnen nicht moeglich
    return 1 ;
  }

  for (;;)
  { if (serialGetchar(fd) == 't')       //Anfangszeichnen "t" suchen
    {
      for (int i = 0; i<5; i++){        //5 Zeichen nacheinander aus dem Input Stream
        in_temp[i] = serialGetchar(fd); //in das Array in_temp schreiben
      }
      for (int i = 0; i<5; i++){        //5 Zeichen aus Input Stream
        in_hum[i] = serialGetchar(fd);  //in in_hum schreiben
      }
      for (int i = 0; i<4; i++){        //4 Zeichen aus Input Stream
        in_heiz[i] = serialGetchar(fd); //in in_heiz schreiben
      }
      for (int i = 0; i<4; i++){        //4 Zeichen aus Input Stream
        in_luft[i] = serialGetchar(fd); //in in_luft schreiben
      }
    }
      t = atof (in_temp);               //Input Arrays in float Variablen umwandeln
      h = atof (in_hum);
      heiz = atof (in_heiz)*100;
      luft = atof (in_luft)*100;

      static struct sockaddr_in serv_addr; /* static is zero filled on start up */

      printf("Connecting socket to %s and port %d\n", IP_ADDRESS, PORT);
      if((sockfd = socket(AF_INET, SOCK_STREAM,0)) <0) 
            pexit("socket() failed");

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
        serv_addr.sin_port = htons(PORT);
        if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <0) 
            pexit("connect() failed");
           
        /* InfluxDB line protocol*/
        sprintf(body, "temperatur,host=%s temperatur=%.2f   \n", HOSTNAME, t);          //Temperatur uebertragen

        /* Note spaces are important and the carriage-returns & newlines */
        /* db= is the datbase name, u= the username and p= the password */
        sprintf(header, 
            "POST /write?db=%s&u=%s&p=%s HTTP/1.1\r\nHost: influx:8086\r\nContent-Length: %ld\r\n\r\n", 
             DATABASE, USERNAME, PASSWORD, strlen(body));

        printf("Send to InfluxDB the POST request bytes=%d \n->|%s|<-\n",strlen(header), header);
        write(sockfd, header, strlen(header));
        if (ret < 0)
            pexit("Write Header request to InfluxDB failed");

        printf("Send to InfluxDB the data bytes=%d \n->|%s|<-\n",strlen(body), body);
        ret = write(sockfd, body, strlen(body));
        if (ret < 0)
            pexit("Write Data Body to InfluxDB failed");

        /* Get back the acknwledgement from InfluxDB */
        /* It worked if you get "HTTP/1.1 204 No Content" and some other fluff */
        ret = read(sockfd, result, sizeof(result));
        if (ret < 0)
            pexit("Reading the result from InfluxDB failed");
        result[ret] = 0; /* terminate string */
        printf("Result returned from InfluxDB. Note:204 is Success\n->|%s|<-\n",result);

        /* InfluxDB line protocol */
        sprintf(body, "luftfeuchtigkeit,host=%s luftfeuchtigkeit=%.2f   \n", HOSTNAME, h);          //Luftfeuchtigkeit uebertragen

        /* Note spaces are important and the carriage-returns & newlines */
        /* db= is the datbase name, u= the username and p= the password */
        sprintf(header, 
            "POST /write?db=%s&u=%s&p=%s HTTP/1.1\r\nHost: influx:8086\r\nContent-Length: %ld\r\n\r\n", 
             DATABASE, USERNAME, PASSWORD, strlen(body));

        printf("Send to InfluxDB the POST request bytes=%d \n->|%s|<-\n",strlen(header), header);
        write(sockfd, header, strlen(header));
        if (ret < 0)
            pexit("Write Header request to InfluxDB failed");

        printf("Send to InfluxDB the data bytes=%d \n->|%s|<-\n",strlen(body), body);
        ret = write(sockfd, body, strlen(body));
        if (ret < 0)
            pexit("Write Data Body to InfluxDB failed");

        /* Get back the acknwledgement from InfluxDB */
        /* It worked if you get "HTTP/1.1 204 No Content" and some other fluff */
        ret = read(sockfd, result, sizeof(result));
        if (ret < 0)
            pexit("Reading the result from InfluxDB failed");
        result[ret] = 0; /* terminate string */
        printf("Result returned from InfluxDB. Note:204 is Success\n->|%s|<-\n",result);
      
        /* InfluxDB line protocol */
        sprintf(body, "heizleistung,host=%s heizleistung=%.2f   \n", HOSTNAME, heiz);           //Heizleistung uebertragen

        /* Note spaces are important and the carriage-returns & newlines */
        /* db= is the datbase name, u= the username and p= the password */
        sprintf(header, 
            "POST /write?db=%s&u=%s&p=%s HTTP/1.1\r\nHost: influx:8086\r\nContent-Length: %ld\r\n\r\n", 
             DATABASE, USERNAME, PASSWORD, strlen(body));

        printf("Send to InfluxDB the POST request bytes=%d \n->|%s|<-\n",strlen(header), header);
        write(sockfd, header, strlen(header));
        if (ret < 0)
            pexit("Write Header request to InfluxDB failed");

        printf("Send to InfluxDB the data bytes=%d \n->|%s|<-\n",strlen(body), body);
        ret = write(sockfd, body, strlen(body));
        if (ret < 0)
            pexit("Write Data Body to InfluxDB failed");

        /* Get back the acknwledgement from InfluxDB */
        /* It worked if you get "HTTP/1.1 204 No Content" and some other fluff */
        ret = read(sockfd, result, sizeof(result));
        if (ret < 0)
            pexit("Reading the result from InfluxDB failed");
        result[ret] = 0; /* terminate string */
        printf("Result returned from InfluxDB. Note:204 is Success\n->|%s|<-\n",result);

        /* InfluxDB line protocol */
        sprintf(body, "lüfterleistung,host=%s lüfterleistung=%.2f   \n", HOSTNAME, luft);           //Luefterleistungs uebertragen

        /* Note spaces are important and the carriage-returns & newlines */
        /* db= is the datbase name, u= the username and p= the password */
        sprintf(header, 
            "POST /write?db=%s&u=%s&p=%s HTTP/1.1\r\nHost: influx:8086\r\nContent-Length: %ld\r\n\r\n", 
             DATABASE, USERNAME, PASSWORD, strlen(body));

        printf("Send to InfluxDB the POST request bytes=%d \n->|%s|<-\n",strlen(header), header);
        write(sockfd, header, strlen(header));
        if (ret < 0)
            pexit("Write Header request to InfluxDB failed");

        printf("Send to InfluxDB the data bytes=%d \n->|%s|<-\n",strlen(body), body);
        ret = write(sockfd, body, strlen(body));
        if (ret < 0)
            pexit("Write Data Body to InfluxDB failed");

        /* Get back the acknwledgement from InfluxDB */
        /* It worked if you get "HTTP/1.1 204 No Content" and some other fluff */
        ret = read(sockfd, result, sizeof(result));
        if (ret < 0)
            pexit("Reading the result from InfluxDB failed");
        result[ret] = 0; /* terminate string */
        printf("Result returned from InfluxDB. Note:204 is Success\n->|%s|<-\n",result);
      
        printf(" - - - sleeping for %d secs\n",SECONDS);
        sleep(SECONDS);
  
    close(sockfd);
   
    memset(in_temp, 0, 10);         //Input Arrays leeren
    memset(in_hum, 0, 10);
    memset(in_heiz, 0, 10);
    memset(in_luft, 0, 10);
   //   printf("Temperatur:%.2f Grad Celsius \n", t);
   //   printf("Luftfeuchtigkeit:%.2f %% \n", h);
      fflush (stdout) ;
  }
}
