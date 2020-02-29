#include "line_sensor.h"

bool REFRESH_LINE_SENSOR = true;

LineSensorArray::LineSensorArray(IOExpander * io) :
    _io(io)
{
}

void LineSensorArray::update()
{
    NW = _io->get_value(NW_SENSE_PIN);
    NE = _io->get_value(NE_SENSE_PIN);
    SW = _io->get_value(SW_SENSE_PIN);
    SE = _io->get_value(SE_SENSE_PIN);
}
