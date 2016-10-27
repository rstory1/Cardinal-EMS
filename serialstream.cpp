#include "serialstream.h"

void serialStream::connect()
{
      if(RS232_OpenComport(cport_nr, bdrate, mode))
      {
        printf("Can not open comport\n");

        //return(0);
      }

      while(1)
      {
        read();
        }

    #ifdef _WIN32
        Sleep(100);
    #else
        usleep(100000);  /* sleep for 100 milliSeconds */
    #endif
      }

 }

void serialStream::read() {
    n = RS232_PollComport(cport_nr, buf, 4095);

    if(n > 0)
    {
      buf[n] = 0;   /* always put a "null" at the end of a string! */

      for(i=0; i < n; i++)
      {
        if(buf[i] < 32)  /* replace unreadable control-codes by dots */
        {
          buf[i] = '.';
        }
      }

      printf("received %i bytes: %s\n", n, (char *)buf);
    }
}
