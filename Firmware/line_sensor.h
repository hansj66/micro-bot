#ifndef _LINE_SENSOR_H_
#define _LINE_SENSOR_H_
#include "mbed.h"
#include "IOExpander.h"

// "LINE" == white response
// "ARENA" == dark response

#define ARENA true
#define LINE false

class LineSensorArray
{
    public:
        LineSensorArray(IOExpander * io);
        void update();
    
        bool NW;
        bool NE;
        bool SW;
        bool SE;
        IOExpander * _io;
};
    

#endif