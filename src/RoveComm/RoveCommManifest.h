#ifndef RoveCommManifest_h
#define RoveCommManifest_h

#include <stdint.h>

///////////////////////////////////////////
//              IP Ports                 //
//////////////////////////////////////////////
const uint16_t ROVE_ETHERNET_UDP_PORT = 11000;
const uint16_t ROVE_ETHERNET_TCP_PORT = 11000;

///////////////////////////////////////////
//             IP Addresses              //
///////////////////////////////////////////

// Rover Network Subnet IP Address
const uint8_t ROVE_FIRST_OCTET        = 192;
const uint8_t ROVE_SECOND_OCTET       = 168;
const uint8_t ROVE_THIRD_OCTET        = 1;

//////////////////////////////////////////////
// Board IP Address
const uint8_t DRIVEBOARD_FOURTH_OCTET   = 130;
const uint8_t SCIENCEBOARD_FOURTH_OCTET = 135;
const uint8_t DRILLBOARD_FOURTH_OCTET   = 139;


///////////////////////////////////////////
//              RED Controller           //
///////////////////////////////////////////

/////////////////////////////////////////////////////
// RED Xbox joystick limits
const int16_t RED_MAX_FORWARD =   1000;
const int16_t RED_MAX_REVERSE =  -1000;

///////////////////////////////////////////
//              Data IDs                 //
///////////////////////////////////////////
// RoveComm Data IDs
const uint16_t ROVECOMM_SUBSCRIBE_REQUEST   = 3;
const uint16_t ROVECOMM_UNSUBSCRIBE_REQUEST = 4;

////////////////////////////////////////
// DriveBoard Data IDs
const uint16_t DRIVE_LEFT_RIGHT  = 528;
const uint16_t UNDERGLOW_COLOR   = 2320;
const uint16_t LED_FUNCTION      = 2321;
const uint16_t HEADLIGHTS        = 2336;
const uint16_t DROP_BAY_OPEN     = 1584;
const uint16_t DROP_BAY_CLOSE    = 1585;

////////////////////////////////////////////////
// DrillBoard Data IDs
const uint16_t LEADSCREW_OPEN_LOOP       = 2901;
const uint16_t GENEVA_OPEN_LOOP          = 2902;
const uint16_t DRILL_OPEN_LOOP           = 2900;

const uint16_t LEADSCREW_TO_LIMIT_SWITCH = 2903;
const uint16_t GENEVA_TO_LIMIT_SWITCH    = 2904;

////////////////////////////////////////////////////
// ScienceBoad Data IDs
const uint16_t AIR_PRESSURE_SCI_SENSOR_0     = 1824;
const uint16_t METHANE_SCI_SENSOR_1          = 1830;
const uint16_t AMMONIA_SCI_SENSOR_2          = 1842;
const uint16_t UV_SCI_SENSOR_3               = 1828;
const uint16_t AIR_HUMIDITY_SCI_SENSOR_4     = 1826;
const uint16_t AIR_TEMPERATURE_SCI_SENSOR_5  = 1824;
const uint16_t SCIENCE_COMMANDS              = 1808;


///////////////////////////////////////////
//             Data Commands             //
///////////////////////////////////////////
// DriveBoard Data Commands
const uint8_t DROP_BAY_1  = 0;
const uint8_t DROP_BAY_2  = 1;

//////////////////////////////////////////////////
// DrillBoard Data Commands
const uint8_t GENEVA_REVERSE_TO_LIMIT_SWITCH  = 0;
const uint8_t GENEVA_FORWARD_TO_LIMIT_SWITCH  = 1;

const uint8_t LEADSCREW_BOTTOM_LIMIT_SWITCH   = 0;
const uint8_t LEADSCREW_DROPOFF_LIMIT_SWITCH  = 1;
const uint8_t LEADSCREW_EMPTY_SWITCH          = 2;
const uint8_t LEADSCREW_RELOAD_LIMIT_SWITCH   = 3;
const uint8_t LEADSCREW_TOP_LIMIT_SWITCH      = 4;

/////////////////////////////////////
//Science Commands
const uint16_t LASER_ON         = 2;
const uint16_t LASER_OFF        = 3;
const uint16_t SPECTROMETER_RUN = 6;

#endif // RoveCommManifest_h