#include <LFlash.h>
#include <LSD.h>
#include <LStorage.h>
#include <LGPS.h>
#include <LWiFi.h>
#include <LWiFiClient.h>

#define WIFI_AP "IIITTF141"
#define WIFI_PASSWORD "123456789"
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.
#define SITE_URL "192.168.43.2"
#define tempPin A0

int tempValue = 0;

LWiFiClient c;


//uncomment the storage you want to use
#define Drv LFlash          // use Internal 10M Flash
// #define Drv LSD           // use SD card

gpsSentenceInfoStruct info; //needed to get GPS data

double latitude = 0.00;
double longitude = 0.00;
float altitude = 0.00;
float dop = 100.00; //dilution of precision
float geoid = 0.00;
float k_speed = 0.00, m_speed = 0.00; //speed in knots and speed in m/s
float track_angle = 0.00;
int fix = 0;
int hour = 0, minute = 0, second = 0;
int sat_num = 0; //number of visible satellites
int day = 0, month = 0, year = 0;
String time_format = "00:00:00", date_format = "00:00:0000";
String lat_format = "0.00000", lon_format = "0.00000";
char file[15] = "";
int pause = 3000; //time in milliseconds between two logs

void setup()
{
  delay(10000);
  LWiFi.begin();
  Serial.begin(115200);
  Serial.print("Initializing memory...");
  pinMode(10, OUTPUT); //needed for SD card
  if(!Drv.begin())
  {
    Serial.println("Error initalizing memory.");  
    while(true);
  }
  Serial.println("OK.");
  LGPS.powerOn();
  Serial.println("GPS started.");
    
  
  

  // keep retrying until connected to AP
  Serial.println("Connecting to AP");
  while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
  {
    
    delay(1000);
  }
}




void loop()
{
  delay(10000);
  if (getData(&info) > 3)
  {
    String str = "";
    str += date_format;
    str += ",";
    str += time_format;
    str += ",";
    str += lat_format;
    str += ",";
    str += lon_format;
    str += ",";
    str += altitude;
    str += ",";
    str += dop;
    str += ",";
    str += geoid;
    str += ",";
    str += track_angle;
    str += ",";
    str += m_speed;
    str += ",";
    str += k_speed;
    str += ",";
    str += fix;
    str += ",";
    str += sat_num;
    Serial.println(str);
    String file_name = date_format;
    file_name += ".txt";
    file_name.toCharArray(file, 14);
    Serial.println(file);
    LFile dataFile = Drv.open(file, FILE_WRITE);
    if (dataFile)
    {
      dataFile.println(str);
      dataFile.close();
      Serial.println("File written.");
      
    }
    else Serial.println("Error opening file.");
  }
  else Serial.println("Less then 4 satelites.");
  delay(pause);

  
}

/**
*Converts degrees from (d)ddmm.mmmm to (d)dd.mmmmmm
*@param str the string rappresentation of the angle in (d)ddmm.mmmm format
*@param dir if true the direction is south, and the angle is negative.
*@return the given angle in dd.mmmmmm format.
*/
float convert(String str, boolean dir)
{
  double mm, dd;
  int point = str.indexOf('.');
  dd = str.substring(0, (point - 2)).toFloat();
  mm = str.substring(point - 2).toFloat() / 60.00;
  return (dir ? -1 : 1) * (dd + mm);
}

/**
*Gets gps informations
*@param info gpsSentenceInfoStruct is a struct containing NMEA sentence infomation
*@return the number of hooked satellites, or 0 if there was an error getting informations
*/
int getData(gpsSentenceInfoStruct* info)
{
  Serial.println("Collecting GPS data.");
  LGPS.getData(info);
  Serial.println((char*)info->GPGGA);
  if (info->GPGGA[0] == '$')
  {
    Serial.print("Parsing GGA data....");
    String str = (char*)(info->GPGGA);
    str = str.substring(str.indexOf(',') + 1);
    hour = str.substring(0, 2).toInt();
    minute = str.substring(2, 4).toInt();
    second = str.substring(4, 6).toInt();
    time_format = "";
    time_format += hour;
    time_format += ":";
    time_format += minute;
    time_format += ":";
    time_format += second;
    str = str.substring(str.indexOf(',') + 1);
    latitude = convert(str.substring(0, str.indexOf(',')), str.charAt(str.indexOf(',') + 1) == 'S');
    int val = latitude * 1000000;
    String s = String(val);
    lat_format = s.substring(0, (abs(latitude) < 100) ? 2 : 3);
    lat_format += '.';
    lat_format += s.substring((abs(latitude) < 100) ? 2 : 3);
    str = str.substring(str.indexOf(',') + 3);
    longitude = convert(str.substring(0, str.indexOf(',')), str.charAt(str.indexOf(',') + 1) == 'W');
    val = longitude * 1000000;
    s = String(val);
    lon_format = s.substring(0, (abs(longitude) < 100) ? 2 : 3);
    lon_format += '.';
    lon_format += s.substring((abs(longitude) < 100) ? 2 : 3);
    //Converting latitude and longitude to string
    String latPlot = String(latitude);
    String longPlot = String(longitude);
     Serial.println("*******************************************");
    Serial.println(latPlot) ;
    Serial.println("*******************************************");
   Serial.println("*******************************************");
    Serial.println(longPlot) ;
    Serial.println("*******************************************");


  // keep retrying until connected to website
  Serial.println("Connecting to WebSite");
  while (0 == c.connect(SITE_URL, 80))
  {
    Serial.println("Re-Connecting to WebSite");
    delay(1000);
  }
  // send HTTP request, ends with 2 CR/LF
 char d1[]="temp=21.03&lat=";
 char d2[] = "&long=";
 String thisData = d1 + latPlot +d2+longPlot;
 //create a char array for sending data 
//send the header
c.println("POST /mediatek/database_connection.php HTTP/1.1");           //define POST path
 
c.println("Host: " SITE_URL);           //define hostname  
c.println("Connection: close");
 
c.println("Content-Type: application/x-www-form-urlencoded");     //define Content-Type 
c.print("Content-Length: ");                     //define Content-Length 
c.println(31);
c.println();
 
// send the body (variables)
c.print(thisData);



    
    str = str.substring(str.indexOf(',') + 3);
    fix = str.charAt(0) - 48;
    str = str.substring(2);
    sat_num = str.substring(0, 2).toInt();
    str = str.substring(3);
    dop = str.substring(0, str.indexOf(',')).toFloat();
    str = str.substring(str.indexOf(',') + 1);
    altitude = str.substring(0, str.indexOf(',')).toFloat();
    str = str.substring(str.indexOf(',') + 3);
    geoid = str.substring(0, str.indexOf(',')).toFloat();
    Serial.println("done.");
    
    if (info->GPRMC[0] == '$')
    {
      Serial.print("Parsing RMC data....");
      str = (char*)(info->GPRMC);
      int comma = 0;
      for (int i = 0; i < 60; ++i)
      {
        if (info->GPRMC[i] == ',')
        {
          comma++;
          if (comma == 7)
          {
            comma = i + 1;
            break;
          }
        }
      }

      str = str.substring(comma);
      k_speed = str.substring(0, str.indexOf(',')).toFloat();
      m_speed = k_speed * 0.514;
      str = str.substring(str.indexOf(',') + 1);
      track_angle = str.substring(0, str.indexOf(',')).toFloat();
      str = str.substring(str.indexOf(',') + 1);
      day = str.substring(0, 2).toInt();
      month = str.substring(2, 4).toInt();
      year = str.substring(4, 6).toInt();
      date_format = "20";
      date_format += year;
      date_format += "-";
      date_format += month;
      date_format += "-";
      date_format += day;
      Serial.println("done.");
      return sat_num;
    }
  }
  else
  {
    Serial.println("No GGA data");
  }
  return 0;
}
