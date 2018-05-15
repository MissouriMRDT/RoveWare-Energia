#ifndef RoveCommManifest_h
#define RoveCommManifest_h

#include <stdint.h>

////////////////////////////////////////////
// Rover Network Subnet IP Address
const uint8_t ROVE_FIRST_OCTET        = 192;
const uint8_t ROVE_SECOND_OCTET       = 168;
const uint8_t ROVE_THIRD_OCTET        = 1;

// Board IP Address
const uint8_t DRIVEBOARD_FOURTH_OCTET = 130;

///////////////////////////////////////
// DriveBoard Data Id's
const uint16_t DRIVE_LEFT_RIGHT  = 528;
const uint16_t UNDERGLOW_COLOR   = 2320;
const uint16_t LED_FUNCTION      = 2321;
const uint16_t HEADLIGHTS        = 2336;
const uint16_t DROP_BAY_OPEN     = 1584;
const uint16_t DROP_BAY_CLOSE    = 1585;

// data
const uint8_t DROP_BAY_1  = 0;
const uint8_t DROP_BAY_2  = 1;

#endif // RoveCommManifest_h