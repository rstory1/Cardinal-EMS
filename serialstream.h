#ifndef SERIALSTREAM_H
#define SERIALSTREAM_H


class serialStream
{
private:
    int i, n,
        cport_nr=24,        /* /dev/ttyS0 (COM1 on windows) */
        bdrate=9600;       /* 9600 baud */

    unsigned char buf[4096];

    char mode[4]={'8','N','1',0};
public:
    void connect();
    void read();
};

/**************************************************
This will set up the connection to the serial port and read in the sentence.

These are the cport_nr values to be used.
    Linux   windows
0   ttyS0   COM1
1   ttyS1   COM2
2   ttyS2   COM3
3   ttyS3   COM4
4   ttyS4   COM5
5   ttyS5   COM6
6   ttyS6   COM7
7   ttyS7   COM8
8   ttyS8   COM9
9   ttyS9   COM10
10  ttyS10  COM11
11  ttyS11  COM12
12  ttyS12  COM13
13  ttyS13  COM14
14  ttyS14  COM15
15  ttyS15  COM16
16  ttyUSB0   n.a.
17  ttyUSB1   n.a.
18  ttyUSB2   n.a.
19  ttyUSB3   n.a.
20  ttyUSB4   n.a.
21  ttyUSB5   n.a.
22  ttyAMA0   n.a.
23  ttyAMA1   n.a.
24  ttyACM0   n.a.
25  ttyACM1   n.a.


**************************************************/

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"

#endif // SERIALSTREAM_H
