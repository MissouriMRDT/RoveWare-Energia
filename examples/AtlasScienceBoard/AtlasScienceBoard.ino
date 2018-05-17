/*
 * Science Board Software
 * Rev 1, 2018
 * Used with Science Board Hardware Rev 1
 * Andrew Van Horn, Judah Schad
 * 
 * Controls spectrometer laser, spectrometer mirror motor, mirror motor encoder, and photodiode sensors
 */



#include "RoveWare.h"
#include "MPL3115A2.h"

//Energia Libraries
#include <Wire.h>
#include <EthernetClient.h>
#include <EthernetServer.h>

#define USE_SERIAL_DEBUG 1 //Set to 1 to output sensor data to Serial

////////////////////
//   Board Pins   //
////////////////////
/////////////////////////////////////////////////
const int SPECTROMETER_LASER_PIN          = PD_4;
const int SPECTROMETER_MIRROR_ENCODER_PIN = PM_0; //pulseIn()
const int SPECTROMETER_MIRROR_MOTOR_PIN_1 = PK_4;
const int SPECTROMETER_MIRROR_MOTOR_PIN_2 = PK_5;
const int SPECTROMETER_PHOTODIODE_PIN_1   = PD_0;
const int SPECTROMETER_PHOTODIODE_PIN_2   = PD_1;

//Sensors/////////////////////////////////////////
const int PRESSURE_I2C                    = 0;
const int METHANE_PIN                     = PE_0;
const int AMMONIA_PIN                     = PE_1;
const int UV_PIN                          = PE_2;
const int AIR_HUMIDITY_PIN                = PE_3;
const int AIR_TEMPERATURE_PIN             = PD_7;

////////////////////
// Sensor Config  //
////////////////////
//Outputs///////////////////////////
uint16_t air_pressure_pascals;
uint16_t methane_ppm;
uint16_t ammonia_ppm;
uint16_t uv_intensity;
uint16_t air_humidity_rh;
uint16_t air_temperature_farenheit;

//Map Consts/////////////////////////////////////////////////////////////////////////
#define METHANE_MAX_ADC            1 //ADC
#define METHANE_MAX_ACTUAL         1 //Real Sensor (Enter as Float, ie 1.234 => 1234)
#define METHANE_MIN_ADC            1
#define METHANE_MIN_ACTUAL         1

#define AMMONIA_MAX_ADC            1
#define AMMONIA_MAX_ACTUAL         1
#define AMMONIA_MIN_ADC            1
#define AMMONIA_MIN_ACTUAL         1

#define UV_MAX_ADC                 1
#define UV_MAX_ACTUAL              1
#define UV_MIN_ADC                 1
#define UV_MIN_ACTUAL              1

#define AIR_HUMIDITY_MAX_ADC       1
#define AIR_HUMIDITY_MAX_ACTUAL    1
#define AIR_HUMIDITY_MIN_ADC       1
#define AIR_HUMIDITY_MIN_ACTUAL    1

#define AIR_TEMPERATURE_MAX_ADC    1
#define AIR_TEMPERATURE_MAX_ACTUAL 1
#define AIR_TEMPERATURE_MIN_ADC    1
#define AIR_TEMPERATURE_MIN_ACTUAL 1

////////////////////
// RoveComm Setup //
////////////////////
////////////////////////////////////////////////////////////////////
RoveCommEthernetUdp RoveComm;
EthernetServer      SpeectrometerTcpServer(ROVE_ETHERNET_TCP_PORT);

//Read Variables
uint16_t data_id;
size_t command_data_size;
uint8_t data_value[2];

MPL3115A2 PressureI2C;

//Function Prototypes
void spectrometerRun();

void sensorSerialDebug();
void spectrometerSerialDebug(uint16_t spectrometer_photodiode_1, uint16_t spectrometer_photodiode_2);


//////////////////////////////////////////////////////////
void setup() 
{  
  Wire.begin();
  delay(10);
  PressureI2C.begin();
  PressureI2C.setModeBarometer();
  PressureI2C.setOversampleRate(7);
  PressureI2C.enableEventFlags();
  delay(10);
  RoveComm.begin(192, 168, 1, SCIENCEBOARD_FOURTH_OCTET); 
  delay(10);
  SpeectrometerTcpServer.begin();
  delay(10);
  
  if(USE_SERIAL_DEBUG)
    Serial.begin(9600);
  delay(10);

  pinMode(METHANE_PIN,                     INPUT);
  pinMode(AMMONIA_PIN,                     INPUT);
  pinMode(UV_PIN,                          INPUT);
  pinMode(AIR_HUMIDITY_PIN,                INPUT);
  pinMode(AIR_TEMPERATURE_PIN,             INPUT);
  
  pinMode(SPECTROMETER_PHOTODIODE_PIN_1,   INPUT);
  pinMode(SPECTROMETER_PHOTODIODE_PIN_2,   INPUT);
  pinMode(SPECTROMETER_LASER_PIN,          OUTPUT);
  pinMode(SPECTROMETER_MIRROR_ENCODER_PIN, OUTPUT);
  pinMode(SPECTROMETER_MIRROR_MOTOR_PIN_1, OUTPUT);
  pinMode(SPECTROMETER_MIRROR_MOTOR_PIN_2, OUTPUT);

 //ToDo:Watchdog
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  RoveComm.read(&data_id, &command_data_size, data_value);

  switch (data_id)
  {
    case(LASER_ON):
      digitalWrite(SPECTROMETER_LASER_PIN, HIGH);
      break;
    case(LASER_OFF):
      digitalWrite(SPECTROMETER_LASER_PIN, LOW);
      break;
    case(SPECTROMETER_RUN):
      spectrometerRun();
      break;
    default:
      break;
  }

  //Read and Map sensor Vales
  air_pressure_pascals       = PressureI2C.readPressure();
  methane_ppm                = 0.001*map(analogRead(METHANE_PIN),         METHANE_MIN_ADC,         METHANE_MAX_ADC,         METHANE_MIN_ACTUAL,         METHANE_MAX_ACTUAL);
  ammonia_ppm                = 0.001*map(analogRead(AMMONIA_PIN),         AMMONIA_MIN_ADC,         AMMONIA_MAX_ADC,         AMMONIA_MIN_ACTUAL,         AMMONIA_MAX_ACTUAL);
  uv_intensity               = 0.001*map(analogRead(UV_PIN),              UV_MIN_ADC,              UV_MAX_ADC,              UV_MIN_ACTUAL,              UV_MAX_ACTUAL);
  air_humidity_rh            = 0.001*map(analogRead(AIR_HUMIDITY_PIN),    AIR_HUMIDITY_MIN_ADC,    AIR_HUMIDITY_MAX_ADC,    AIR_HUMIDITY_MIN_ACTUAL,    AIR_HUMIDITY_MAX_ACTUAL);
  air_temperature_farenheit  = 0.001*map(analogRead(AIR_TEMPERATURE_PIN), AIR_TEMPERATURE_MIN_ADC, AIR_TEMPERATURE_MAX_ADC, AIR_TEMPERATURE_MIN_ACTUAL, AIR_TEMPERATURE_MAX_ACTUAL);

  //Send Senor Values
  //RoveComm.write(AIR_PRESSURE_SCI_SENSOR_0,    sizeof(float), PressureI2C To Do      );

  RoveComm.write(AIR_PRESSURE_SCI_SENSOR_0,    sizeof(float), &air_pressure_pascals       );
  RoveComm.write(METHANE_SCI_SENSOR_1,         sizeof(float), &methane_ppm                );
  RoveComm.write(AMMONIA_SCI_SENSOR_2,         sizeof(float), &ammonia_ppm                );
  RoveComm.write(UV_SCI_SENSOR_3,              sizeof(float), &uv_intensity               );
  RoveComm.write(AIR_HUMIDITY_SCI_SENSOR_4,    sizeof(float), &air_humidity_rh            );
  RoveComm.write(AIR_TEMPERATURE_SCI_SENSOR_5, sizeof(float), &air_temperature_farenheit  );

  if(USE_SERIAL_DEBUG) sensorSerialDebug();
}

/////////////////////////////////////////////////////
void spectrometerRun()
{
  EthernetClient SpectrometerTcpClient = SpeectrometerTcpServer.available();

  if(SpectrometerTcpClient)
  {
     uint16_t spectrometer_photodiode_1;
     uint16_t spectrometer_photodiode_2;
     String tcp_buffer;

     //Move Spectrometer Motor Forward
     digitalWrite(SPECTROMETER_MIRROR_MOTOR_PIN_1, HIGH);
     digitalWrite(SPECTROMETER_MIRROR_MOTOR_PIN_1, LOW );

     delay(5000);

     //Turn on Laser
     digitalWrite(SPECTROMETER_LASER_PIN, HIGH);

     //Loop takes analog reading, delays 1/8 second, runs for roughly 30 seconds
     for(int i = 0; i<240; i++)
     {
      //Read Photodiodes
      spectrometer_photodiode_1 = analogRead(SPECTROMETER_PHOTODIODE_PIN_1);
      spectrometer_photodiode_2 = analogRead(SPECTROMETER_PHOTODIODE_PIN_2);

      if(USE_SERIAL_DEBUG) spectrometerSerialDebug(spectrometer_photodiode_1, spectrometer_photodiode_2);

      //Print Data
      SpeectrometerTcpServer.println("photodiode 1:");
      SpeectrometerTcpServer.println(spectrometer_photodiode_1);
      SpeectrometerTcpServer.println("photodiode 2:");
      SpeectrometerTcpServer.println(spectrometer_photodiode_2);
      
      delay(125);
     }

     //Turn off Laser
     digitalWrite(SPECTROMETER_LASER_PIN, LOW);

     //Stop Motor
     digitalWrite(SPECTROMETER_MIRROR_MOTOR_PIN_1, LOW);
     digitalWrite(SPECTROMETER_MIRROR_MOTOR_PIN_1, LOW);

     //End TCP Client
     SpectrometerTcpClient.stop();

     delay(2000);

     //Return Motor to Start
     digitalWrite(SPECTROMETER_MIRROR_MOTOR_PIN_1, LOW );
     digitalWrite(SPECTROMETER_MIRROR_MOTOR_PIN_1, HIGH);
     delay(40000);

     //Stop Motor
     digitalWrite(SPECTROMETER_MIRROR_MOTOR_PIN_1, LOW);
     digitalWrite(SPECTROMETER_MIRROR_MOTOR_PIN_1, LOW);

     delay(10000);
  }
}

////ToDo
float getPressure()
{}

//////////////////////////////////
void sensorSerialDebug()
{
    Serial.println("");
    Serial.print("Pressure Out");
    Serial.println(air_pressure_pascals);
    
    Serial.println("");
    Serial.print("Methane Ain:");
    Serial.println(analogRead(METHANE_PIN));
    Serial.print("Methane Out");
    Serial.println(methane_ppm);

    Serial.println("");
    Serial.print("Ammonia Ain:");
    Serial.println(analogRead(AMMONIA_PIN));
    Serial.print("Ammonia Out");
    Serial.println(ammonia_ppm);

    Serial.println("");
    Serial.print("UV Ain:");
    Serial.println(analogRead(UV_PIN));
    Serial.print("UV Out");
    Serial.println(uv_intensity);

    Serial.println("");
    Serial.print("Air Humidity Ain:");
    Serial.println(analogRead(AIR_HUMIDITY_PIN));
    Serial.print("Air Humidity Out");
    Serial.println(air_humidity_rh);

    Serial.println("");
    Serial.print("Air Temperature Ain:");
    Serial.println(analogRead(AIR_TEMPERATURE_PIN));
    Serial.print("Air Temperature Out");
    Serial.println(air_temperature_farenheit);

    return;
}

void spectrometerSerialDebug(uint16_t spectrometer_photodiode_1, uint16_t spectrometer_photodiode_2)
{
  Serial.print("Photodiode 1: ");
  Serial.println(spectrometer_photodiode_1);
  Serial.print("Photodiode 2: ");
  Serial.println(spectrometer_photodiode_2);
}

