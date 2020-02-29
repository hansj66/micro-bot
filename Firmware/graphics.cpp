#include "MicroBit.h"
#include "MicroBitDisplay.h"

extern MicroBitDisplay display;
const int ARROWS = 5;
MicroBitImage arrowRight[ARROWS] = {
                MicroBitImage("0,0,0,0,0\n0,0,0,0,0\n1,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n"),
                MicroBitImage("0,0,0,0,0\n1,0,0,0,0\n1,1,0,0,0\n1,0,0,0,0\n0,0,0,0,0\n"),
                MicroBitImage("0,0,0,0,0\n0,1,0,0,0\n0,1,1,0,0\n0,1,0,0,0\n0,0,0,0,0\n"),
                MicroBitImage("0,0,0,0,0\n0,0,1,0,0\n0,0,1,1,0\n0,0,1,0,0\n0,0,0,0,0\n"),
                MicroBitImage("0,0,0,0,0\n0,0,0,1,0\n0,0,0,1,1\n0,0,0,1,0\n0,0,0,0,0\n")
};

MicroBitImage lineSensorCanvas("0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n");
MicroBitImage rangeSensorCanvas("0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n");
MicroBitImage blankCanvas("0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n");

void RightArrowAnimation()
{
    for (int i=0; i<ARROWS; i++) 
    {
        display.print(arrowRight[i]);
        wait_ms(200);
    }
}

void RightArrowBump()
{
    display.print(arrowRight[3]);
    wait_ms(200);
    display.print(arrowRight[4]);
    wait_ms(200);
}

void RenderLineSensorResponse(int nw, int ne, int sw, int se)
{
    lineSensorCanvas.setPixelValue(4,0,!sw);
    lineSensorCanvas.setPixelValue(0,0,!se);
    lineSensorCanvas.setPixelValue(4,4,!nw);
    lineSensorCanvas.setPixelValue(0,4,!ne);
    display.print(lineSensorCanvas);
    wait_ms(50);
    display.print(blankCanvas);
    wait_ms(50);
}

int limitRange(int value)
{
    if (value > 500)
        value = 500;
    return value;
}

void RenderRangeSensorResponse(int left, int center, int right)
{
    // Fudging the numbers a bit and limiting the range in order
    // to get a better test visualization

    left = limitRange(left);
    center = limitRange(center);
    right = limitRange(right);

    rangeSensorCanvas.setPixelValue(4, (left-20) / 50, 1);
    rangeSensorCanvas.setPixelValue(2, (center-20) / 50, 1);
    rangeSensorCanvas.setPixelValue(0, (right-20) / 50, 1);
    display.print(rangeSensorCanvas);
    wait_ms(50);
    rangeSensorCanvas.clear();
    display.print(blankCanvas);
    wait_ms(50);
}


void DisplayFail()
{
     display.print("- HARDWARE ERROR -"); 
}

void DisplayReady()
{
    display.print("- READY -");
}


