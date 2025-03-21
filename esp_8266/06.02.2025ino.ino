

#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>



const char* ssid = "gautam"; //"Redmi Note";//
const char* password = ""; //"0987654321";/
const char* mqtt_server = "172.16.28.174 "; //"172.16.28.174"; //"91.121.93.94";// test.mosquitto.org "broker.mqtt-dashboard.com";

Ticker timer;
WiFiClient espClient;
PubSubClient client(espClient);
#define MSG_BUFFER_SIZE	(100)
char msg[MSG_BUFFER_SIZE];
//#define LED D4
#define SDA D1
#define SCL D2
#define StartButton D0
#define StopButton D3 
#define RELAY1 D5
#define LIGHTSENSOR D6
#define METALSENSOR D4
#define ledPin  D8


//hw_timer_t *Timer0_Cfg = NULL;
unsigned char counter=0,ReadRTC=0,digit; 
unsigned char RTCdate,RTCday,RTChour,RTCmin,RTCmonth,RTCsec,RTCyear,EVENT_ID,ON_OFF_ST;
unsigned char readChar = 0x00,received_data[20];
unsigned char b1_press=0,Switch1_Press=0,SWI_C,b1_Blocked=0;
unsigned char b2_press=0,Switch2_Press=0,SWI2_C,b2_Blocked=0;
unsigned char Switch1_triggered = 0,Switch2_triggered = 0,Switch3_triggered = 0;
unsigned char b3_press=0,Switch3_Press=0,SWI3_C,b3_Blocked=0;
unsigned int Button_Count = 0,Switch4_Press=0,b4_Press=0,SWI4_C=0,tcry = 0;

int buttonState = 0;        // current state of the button
int lastButtonState = 0;    // previous state of the button
int buttonState2 = 0;        // current state of the button
int lastButtonState2 = 0;    // previous state of the button
// int day, month, year, hour, minute, second;
int logCounter = 1;  // Initialize the log counter

unsigned int i=0,EVENT_CONT,temp;

unsigned long int SERIAL_NO=0;
void initGPIO();
void setDS3231Time(unsigned char , unsigned char , unsigned char ,unsigned char ,unsigned char ,unsigned char,unsigned char );
void readDS3231Time(unsigned char *seconds, unsigned char *minutes, unsigned char *hours, unsigned char *days, unsigned char *dates, unsigned char *months, unsigned char *years);

void I2C_delay(void);
void I2C_start(void);
void I2C_stop(void);
unsigned char I2C_readByte(unsigned char ack);
unsigned char I2C_writeByte(unsigned char data);
void READ_EEPROM(void);
void WRITE_EEPROM(void);
void READ_EVENTN(void);
void Event_log(void);
void READ_SERNO(void);
unsigned char lsb_adr,msb_adr,RTCfREAD,RTCcnt,EE_ARRAY[10];


void ICACHE_RAM_ATTR onTime() 
{ 
  counter++;
  	
//Serial.print("Total Ticks:");
//	Serial.println(counter);
  if(counter>10)
  {
    
    counter = 0;
    readChar = 0;
    //digitalWrite(LED, !digitalRead(LED));
    ReadRTC=1;
    buttonState2 = digitalRead(METALSENSOR);
                if (buttonState2 != lastButtonState2) {
                  if (buttonState2 == HIGH) {
                    b4_Press = 1;
                  
                  }
                }
                lastButtonState2 = buttonState2;
  }

 //unsigned char x=0;
 //   x = (digitalRead(StartButton));//P2IN & 0x01 ; //check if switch is pressed or not

    // typedef enum {
    //     RELAY_2 = 7,
    //     RELAY_1 = 6,
    //     GREEN_BUTTON = 5,
    //     SENSOR_1 = 4,
    //     SENSOR_2 = 3,
    //     RED_BUTTON   = 2,
    //     EMERGENCY_STOP = 1
    // } SwitchPriority;

 
  // Count how many buttons are pressed
  Button_Count = Switch1_Press + Switch2_Press + Switch3_Press;


    if(digitalRead(StartButton) && digitalRead(StopButton))
    {
    if(digitalRead(StopButton))
        Switch2_Press = 1;
    else
        Switch2_Press = 0;
      
      if((b2_press  ==  0) && (Switch2_Press  ==  1) && (b2_Blocked  ==  0) && (Button_Count == 2))// switch pressed
      {
          SWI2_C++;
      }
      else
          SWI2_C = 0;

      if(SWI2_C > 11)
      {
          b2_press = 1;

          SWI2_C = 0;    // log button on event
      }
    }


else
    {
    if(digitalRead(StartButton))
        Switch1_Press = 1;
    else
        Switch1_Press = 0;


    if((b1_press == 0) && (Switch1_Press  ==  1) && (b1_Blocked  ==  0) && (Button_Count <= 2))// switch pressed
      {
          SWI_C++;
      }
    else
          SWI_C = 0;


    if(SWI_C>11)
      {
          b1_press = 1;

          SWI_C = 0;    // log button on event
      }

    
    if(digitalRead(StopButton))
        Switch2_Press = 1;
    else
        Switch2_Press = 0;
      
      if((b2_press  ==  0) && (Switch2_Press  ==  1) && (b2_Blocked  ==  0) && (Button_Count <= 2))// switch pressed
      {
          SWI2_C++;
      }
      else
          SWI2_C = 0;

      if(SWI2_C > 11)
      {
          b2_press = 1;
          // b2_Blocked = 1;     //stop button blocked
          // b1_Blocked = 0;     //green button Unblocked
          // b3_Blocked = 0;     //relay Unblocked
          SWI2_C = 0;    // log button on event
      }


    if(digitalRead(RELAY1))
        Switch3_Press = 1;
    else
        Switch3_Press = 0;
      
      if((b3_press == 0) && (Switch3_Press == 1) && (Button_Count <= 2 ) && (b3_Blocked  ==  0)) // switch pressed
      {
          SWI3_C++;
      }
      else
          SWI3_C = 0;

      if(SWI3_C > 11)
      {
          b3_press = 1;
         // b2_Blocked = 0;
         // b1_Blocked = 0;

          b3_Blocked = 1;
          SWI3_C = 0;    // log button on event
      }



    }
    /*
      if(digitalRead(LIGHTSENSOR))
        Switch4_Press = 1;
      else
        Switch4_Press = 0;
        
      if((b4_Press == 0) && (Switch4_Press == 1) )//&& (Button_Count <= 2 ) && (b3_Blocked  ==  0)) // switch pressed
      {
          SWI4_C++;
      }

        if(SWI4_C > 5)
      {
          b4_Press = 1;
         // b2_Blocked = 0;
         // b1_Blocked = 0;

          //b3_Blocked = 1;
          SWI4_C = 0;    // log button on event
      }
      */
  }











void JsonConverter(const char* msg, const char* date, const char* time) {
    // Creating a JSON document
    DynamicJsonDocument doc(1024);

    // Assigning values to the JSON structure
    doc["id"] = "1";
    doc["message"] = msg;
    doc["date"] = date;
    doc["time"] = time;

    // doc["id"] = "1";
    // doc["message"] = "Hello tanmay!!";
    // doc["date"] = "04/02/2025";
    // doc["time"] = "14:30";

    // Serialize JSON
    String jsonString;
    serializeJson(doc, jsonString);

    // Publish the JSON message to your MQTT topic
    client.publish("ESP8266RES", jsonString.c_str(), true);

    // Increment the log counter after each message
    logCounter++;
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID 
    String clientId = "ESP8266Client-";
  
    clientId += String(random(0xffff), HEX);  //randomSeed(micros()); are linked
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish("ESP8266RES", "SKIT NESCO");//("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("receive_data");//client.subscribe("ESP8266CMD");//("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
    //pinMode(LED, OUTPUT);
  timer1_attachInterrupt(onTime); // Add ISR Function    timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR, true);
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);//TIM_LOOP  TIM_SINGLE                                 microcontroller 80 MHz ka hai ;  80/16 = 5 MHz 
  timer1_write(31250);//100ms   @80/256 // 2500000 / 5 ticks per us from TIM_DIV16  ==  500,000 us interval              5MHz = 5000000 = 1/5000000 (1/x) = 0.0000002 ; *50000 = 0.01 s
          initGPIO();
          digitalWrite(SCL, 1);//P1OUT |= SCL;              // Initialize SCL high (idle state)
   // timerAlarmWrite(Timer0_Cfg, 50000, true);
    //timerAlarmEnable(Timer0_Cfg);
// setDS3231Time(0x00, 0x25, 0x14,0x02,0x21,0x01,0x25); // Seconds, Minutes, Hours
          pinMode(StartButton, INPUT);//P1DIR |= SCL;   // Set SCL pin as output
          pinMode(StopButton, INPUT);
          pinMode(RELAY1, INPUT);
          pinMode(LIGHTSENSOR, INPUT);
          pinMode(METALSENSOR, INPUT);
          pinMode(ledPin, OUTPUT);
          READ_EVENTN();
          READ_SERNO();
 setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
          digitalRead(RELAY1);
          Serial.print("WECOME NESCO SKIT PROJECT:");
}



void loop() {
  // put your main code here, to run repeatedly:

                      char date[20];
                      char time[20];
            if(b1_press) 
                  {
                    Serial.println("b1_press condition passed");
                    if(Switch1_Press  ==  0 || Switch1_triggered == 1){            //dabaya button chodege tab log hoga zero
                    Serial.println("Inside event log condition");
                      EVENT_ID = 01;   // start button ID
                      ON_OFF_ST = 0xaa; // pressed                  1010 1010
                      Event_log();


                      
                      // char msg[128];
                    
                      
                      // Format the date and time separately
                      snprintf(date, sizeof(date), "%2x/%2x/%2x", EE_ARRAY[2], EE_ARRAY[3], EE_ARRAY[4]); // date
                      snprintf(time, sizeof(time), "%2x:%2x:%2x", EE_ARRAY[5], EE_ARRAY[6], EE_ARRAY[7]); // time

                      // Format the message without date and time
                      snprintf(msg, sizeof(msg), "START PRESSED AT:");

                      // Send the log message via JSON with properly formatted date and time
                      JsonConverter(msg, date, time);  
                      //client.publish("ESP8266RES",msg);
                      delay(200);
                      b1_Blocked = 1;
                      b2_Blocked = 0;
                      b3_Blocked = 0;
                      b1_press = 0;
                      Switch1_triggered = 0;
                    }
                  }
            if(b2_press)
                  {
                    if(Switch2_Press  ==  0 || Switch2_triggered == 1){            //button chodege tab log hoga
                    
                      EVENT_ID = 02;   // start button ID
                      ON_OFF_ST=0xaa; // pressed                  1010 1010
                      Event_log();


                      // char msg[128];
                      snprintf(date, sizeof(date), "%2x/%2x/%2x", EE_ARRAY[2], EE_ARRAY[3], EE_ARRAY[4]); // date
                      snprintf(time, sizeof(time), "%2x:%2x:%2x", EE_ARRAY[5], EE_ARRAY[6], EE_ARRAY[7]); // time
                    
                      snprintf(msg, sizeof(msg), "STOP PRESSED AT:");

                      

                      // Send the log message via JSON
                      JsonConverter(msg, date, time);
                      // client.publish("ESP8266RES",msg);
                      delay(200);  

                      b1_Blocked = 0;
                      b2_Blocked = 1;
                      b3_Blocked = 0;
                      b2_press = 0;
                      Switch2_triggered = 0;

                    }
                  }
            if(b3_press)
                  {
                    if(Switch1_Press == 0 )            //button chodege tab log hoga
                    {
                    
                      EVENT_ID = 03;   // start button ID
                      ON_OFF_ST=0xaa; // pressed                  1010 1010
                      Event_log();

                      // char msg[128];

                      
                      
                      snprintf(date, sizeof(date), "%2x/%2x/%2x", EE_ARRAY[2], EE_ARRAY[3], EE_ARRAY[4]); // date
                      snprintf(time, sizeof(time), "%2x:%2x:%2x", EE_ARRAY[5], EE_ARRAY[6], EE_ARRAY[7]); // time
                    
                      snprintf(msg, sizeof(msg), "RELAY1 ON AT:DATE :");
                      // Send the log message via JSON
                      JsonConverter(msg, date, time);
                      delay(200);  
                      b3_press = 0;

                    }
                  }
                buttonState = digitalRead(LIGHTSENSOR);
                if (buttonState != lastButtonState) {
                  if (buttonState == HIGH) {
                  EVENT_ID = 04;   // start button ID
                      ON_OFF_ST=0xaa; // pressed                  1010 1010
                      Event_log();



                      // char msg[128];

                      
                    // snprintf (msg, MSG_BUFFER_SIZE,                     
                      snprintf(date, sizeof(date), "%2x/%2x/%2x", EE_ARRAY[2], EE_ARRAY[3], EE_ARRAY[4]); // date
                      snprintf(time, sizeof(time), "%2x:%2x:%2x", EE_ARRAY[5], EE_ARRAY[6], EE_ARRAY[7]); // time
                    
                      snprintf(msg, sizeof(msg), "Bottom proximity switch:DATE :");

                    // client.publish("ESP8266RES",msg);
                    JsonConverter(msg, date, time);
                    delay(200);   
                  }
                }
                lastButtonState = buttonState;
                if(b4_Press)
                {
                EVENT_ID = 5;   // start button ID
                      ON_OFF_ST=0xaa; // pressed                  1010 1010
                      Event_log();




                      snprintf(date, sizeof(date), "%2x/%2x/%2x", EE_ARRAY[2], EE_ARRAY[3], EE_ARRAY[4]); // date
                      snprintf(time, sizeof(time), "%2x:%2x:%2x", EE_ARRAY[5], EE_ARRAY[6], EE_ARRAY[7]); // time
              
                      snprintf(msg, sizeof(msg), "OVER CUP ALARM:DATE :");
                      JsonConverter(msg, date, time);
                      
                      // client.publish("ESP8266RES",msg);
                      delay(200);
                      b4_Press=0;
                }
            if (!client.connected()) {
                reconnect();
              }
            client.loop();



if(ReadRTC)
{
ReadRTC=0;/*
readDS3231Time(&RTCsec, &RTCmin, &RTChour,&RTCday,&RTCdate,&RTCmonth,&RTCyear);
//Serial.print("Total Ticks:%d,%d,%d",RTChour,RTCmin,RTCsec);

                           Serial.print("DATE:");
                           
                            digit=(RTCdate>>4);//+0x30;
                           Serial.print(digit,HEX);
                           digit=(RTCdate & 0x0f);//+0x30;
                           Serial.print(digit,HEX);
                           Serial.print('/');
                            
                           digit=(RTCmonth>>4);//+0x30;
                           Serial.print(digit,HEX);
                           digit=(RTCmonth & 0x0f);//+0x30;
                           Serial.print(digit,HEX);
                           Serial.print('/');                           
  
                           digit=(RTCyear>>4);//+0x30;
                           Serial.print(digit,HEX);
                           digit=(RTCyear & 0x0f);//+0x30;
                           Serial.print(digit,HEX);
                          // UART_sendChar('/');       
                           Serial.print("  TIME:");
                           digit=(RTChour>>4);//+0x30;
                           Serial.print(digit,HEX);
                           digit=(RTChour & 0x0f);//+0x30;
                           Serial.print(digit,HEX);
                           Serial.print(':');
                           digit=(RTCmin>>4);//+0x30;
                           Serial.print(digit,HEX);
                           digit=(RTCmin & 0x0f);//+0x30;
                           Serial.print(digit,HEX);
                           Serial.print(':');                          
                           //UART_sendChar(RTCmin);
                            digit=(RTCsec>>4);//+0x30;
                           Serial.print(digit,HEX);
                           digit=(RTCsec & 0x0f);//+0x30;
                           Serial.print(digit,HEX);
                           Serial.print('\n');
                           */
}
  if(Serial.available() > 0)
  {

    byte currentbyte = Serial.read();
    received_data[readChar++] = currentbyte;
  }
if(readChar > 11)
               {
                   readChar=0;
               if(received_data[0] == 'S'&& received_data[1] == 'E'&& received_data[2] == 'T'&& received_data[3] == 'T')
               {
                   // set time here
                   RTCsec=received_data[5];
                   RTCmin=received_data[6];
                   RTChour=received_data[7];
                   RTCday=received_data[8];
                   RTCdate=received_data[9];
                   RTCmonth=received_data[10];
                   RTCyear=received_data[11];
                   setDS3231Time(RTCsec, RTCmin, RTChour,RTCday,RTCdate,RTCmonth,RTCyear); // Seconds, Minutes, Hours
                   Serial.print("TIME SET OK");


               }
               else if(received_data[0] == 'R'&& received_data[1] == 'E'&& received_data[2] == 'A'&& received_data[3] == 'D')
                       {
                           RTCsec=0;
                                   RTCmin=0;
                                   RTChour=0;
                                   RTCday=0;
                                   RTCdate=0;
                                   RTCmonth=0;
                                   RTCyear=0;
                           readDS3231Time(&RTCsec, &RTCmin, &RTChour,&RTCday,&RTCdate,&RTCmonth,&RTCyear);
Serial.print("DATE:");
                           
                            digit=(RTCdate>>4);//+0x30;
                           Serial.print(digit,HEX);
                           digit=(RTCdate & 0x0f);//+0x30;
                           Serial.print(digit,HEX);
                           Serial.print('/');
                            
                           digit=(RTCmonth>>4);//+0x30;
                           Serial.print(digit,HEX);
                           digit=(RTCmonth & 0x0f);//+0x30;
                           Serial.print(digit,HEX);
                           Serial.print('/');                           
  
                           digit=(RTCyear>>4);//+0x30;
                           Serial.print(digit,HEX);
                           digit=(RTCyear & 0x0f);//+0x30;
                           Serial.print(digit,HEX);
                          // UART_sendChar('/');       
                           Serial.print("  TIME:");
                           digit=(RTChour>>4);//+0x30;
                           Serial.print(digit,HEX);
                           digit=(RTChour & 0x0f);//+0x30;
                           Serial.print(digit,HEX);
                           Serial.print(':');
                           digit=(RTCmin>>4);//+0x30;
                           Serial.print(digit,HEX);
                           digit=(RTCmin & 0x0f);//+0x30;
                           Serial.print(digit,HEX);
                           Serial.print(':');                          
                           //UART_sendChar(RTCmin);
                            digit=(RTCsec>>4);//+0x30;
                           Serial.print(digit,HEX);
                           digit=(RTCsec & 0x0f);//+0x30;
                           Serial.print(digit,HEX);
                           Serial.print('\n');                          

                       }
                                       else if(received_data[0] == 'E'&& received_data[1] == 'V'&& received_data[2] == 'E'&& received_data[3] == 'N'&& received_data[4] == 'T'&& received_data[5] == 'N'&& received_data[6] == 'U')
                                       {
                          Serial.print("NUMBER OF EVENT IS: ");
                                           
                                digit=    EVENT_CONT/100;    
                            digit=digit;//+0x30;
                           Serial.print(digit,HEX);
                           
                           temp=EVENT_CONT%100;  
                           digit= temp/10; 
                           
                            digit=digit;//+0x30;
                           Serial.print(digit,HEX);
                            digit= temp%10; 
                            digit=digit;//+0x30;
                           Serial.print(digit,HEX);
                                       }
                                       else if(received_data[0] == 'F'&& received_data[1] == 'A'&& received_data[2] == 'C'&& received_data[3] == 'T'&& received_data[4] == 'O'&& received_data[5] == 'R'&& received_data[6] == 'Y')
                                       {
                                         
     EVENT_CONT=0;
     I2C_start();
     I2C_writeByte(0xAE);        // DS3231 address + write bit // EEPROM ADDR AE            1010 1110
     I2C_writeByte(0x0F);        // Start at register 0 (seconds)                           0000 1111
     I2C_writeByte(0xFA); //   4090                                                         1111 1010
     EE_ARRAY[0]=0;  //msb
     EE_ARRAY[1]=0;  //lsb
      I2C_writeByte(EE_ARRAY[0]);     // Write seconds
      I2C_writeByte(EE_ARRAY[1]);     // Write minutes
      I2C_stop();
delay(5);//for(i=0;i<5000;i++);
Serial.print("Factory INIT DONE");
                                       }
  else if(received_data[0] == 'S'&& received_data[1] == 'E'&& received_data[2] == 'N'&& received_data[3] == 'D'&& received_data[4] == 'L'&& received_data[5] == 'O'&& received_data[6] == 'G')
                                       {
                                         
                                           if(EVENT_CONT == 0)
                                             Serial.print("NO LOG PRESENT");
                                           else
                                           {
                                               for(temp=0;temp<EVENT_CONT;temp++)
                                               {
                                                 //if(EVENT_CONT<512)
                                                      {
                                                         // i=EVENT_CONT*8;
                                                          i=temp*8;
                                                          msb_adr=i>>8;  //msb
                                                          lsb_adr=i & 0x00ff;  //lsb

                                                      }    

                                                 
                                               READ_EEPROM();
                                               if(EE_ARRAY[0]  == 1)
                                                 Serial.print("START BUTTON PRESSED AT: ");
                                                else if(EE_ARRAY[0]  == 2)
                                                  Serial.print("STOP BUTTON PRESSED AT: ");
                                                else if(EE_ARRAY[0]  == 3)
                                                  Serial.print("RELAY ON AT: ");




                                              // UART_sendChar(EE_ARRAY[2]);
                                              // UART_sendChar(EE_ARRAY[3]);UART_sendChar(EE_ARRAY[4]);UART_sendChar(EE_ARRAY[5]);
                                              // UART_sendChar(EE_ARRAY[6]);UART_sendChar(EE_ARRAY[7]);
                                               
                                Serial.print("DATE:");
                           
                            digit=(EE_ARRAY[2]>>4);//+0x30;
                           Serial.print(digit);
                           digit=(EE_ARRAY[2] & 0x0f);//+0x30;          0000 1111
                           Serial.print(digit);
                           Serial.print('/');
                            
                           digit=(EE_ARRAY[3]>>4);//+0x30;
                           Serial.print(digit);
                           digit=(EE_ARRAY[3] & 0x0f);//+0x30;          0000 1111
                           Serial.print(digit);
                           Serial.print('/');                           
  
                           digit=(EE_ARRAY[4]>>4);//+0x30;
                           Serial.print(digit);
                           digit=(EE_ARRAY[4] & 0x0f);//+0x30;
                           Serial.print(digit);
                          // UART_sendChar('/');       
                           Serial.print("  TIME:");
                           digit=(EE_ARRAY[5]>>4);//+0x30;
                           Serial.print(digit);
                           digit=(EE_ARRAY[5] & 0x0f);//+0x30;
                           Serial.print(digit);
                           Serial.print(':');
                           digit=(EE_ARRAY[6]>>4);//+0x30;
                           Serial.print(digit);
                           digit=(EE_ARRAY[6] & 0x0f);//+0x30;
                           Serial.print(digit);
                           Serial.print(':');                          
                           //UART_sendChar(RTCmin);
                            digit=(EE_ARRAY[7]>>4);//+0x30;
                           Serial.print(digit);
                           digit=(EE_ARRAY[7] & 0x0f);//+0x30;
                           Serial.print(digit);                                            
                                               
                                               
                                               
                                               Serial.print('\n');
                                               }
                                           
                                           }
     
                                       }                                      

               }
}










void initGPIO() {
    pinMode(SCL, OUTPUT);//P1DIR |= SCL;   // Set SCL pin as output
    // pinMode(SDA, INPUT);//P1DIR &= ~SDA;  // Set SDA pin as input
    pinMode(SDA, OUTPUT);// P1DIR |= SDA;   // Set SCL pin as output
    digitalWrite(SCL, 1);//P1OUT |= SCL;   // Initialize SCL high (idle state)
    digitalWrite(SDA, 1);//P1OUT |= SDA;   // Initialize SCL high (idle state)
}

void setDS3231Time(unsigned char seconds, unsigned char minutes, unsigned char hours, unsigned char days, unsigned char dates, unsigned char months, unsigned char years) {
    I2C_start();
    I2C_writeByte(0xD0);        // DS3231 address + write bit           1101 0000
    I2C_writeByte(0x00);        // Start at register 0 (seconds)        0000 0000
    I2C_writeByte(seconds);     // Write seconds
    I2C_writeByte(minutes);     // Write minutes
    I2C_writeByte(hours);       // Write hours
    I2C_writeByte(days);       // Write day
    I2C_writeByte(dates);       // Write date
    I2C_writeByte(months);       // Write mnth
    I2C_writeByte(years);       // Write year
    I2C_stop();
}
void I2C_delay() {
  unsigned int j=0;
    for(j=0;j<250;j++); //delay_cycles(10);  // Adjust delay based on your clock frequency and timing requirements
}
void I2C_start() {
    digitalWrite(SDA, 1);//P1OUT |= SDA;         // SDA high
    digitalWrite(SCL, 1);//P1OUT |= SCL;         // SCL high 
    I2C_delay();
    digitalWrite(SDA, 0);//P1OUT &= ~SDA;        // SDA low
    I2C_delay();
    digitalWrite(SCL, 0);//P1OUT &= ~SCL;        // SCL low
    I2C_delay();
}

void I2C_stop() {
    digitalWrite(SDA, 0);//PP1OUT &= ~SDA;        // SDA low
    I2C_delay();
    digitalWrite(SCL, 1);//P1OUT |= SCL;         // SCL high
    I2C_delay();
    digitalWrite(SDA, 1);//P1OUT |= SDA;         // SDA high
    I2C_delay();


}
                  //Function(Parameter)
unsigned char I2C_writeByte(unsigned char data) {
    unsigned char bit;

    for (bit = 0; bit < 8; bit++) {
        if (data & 0x80) {     // 1000 0000
           digitalWrite(SDA, 1);//P1OUT |= SDA;   // Set SDA high
        } else {
            digitalWrite(SDA, 0);//P1OUT &= ~SDA;  // Set SDA low
        }
        data <<= 1;         // Shift data to the left
        I2C_delay();
        digitalWrite(SCL, 1);//P1OUT |= SCL;       // Set SCL high
        I2C_delay();
        digitalWrite(SCL, 0);//P1OUT &= ~SCL;      // Set SCL low
    }
    pinMode(SDA, INPUT);//P1DIR &= ~SDA;          // Release SDA line
    I2C_delay();
    digitalWrite(SCL, 1);//P1OUT |= SCL;           // Set SCL high
    I2C_delay();
    unsigned char ack = digitalRead(SDA);//(P1IN & SDA); // Read ACK bit
    digitalWrite(SCL, 0);//P1OUT &= ~SCL;          // Set SCL low
    pinMode(SDA, OUTPUT);//P1DIR |= SDA;           // Set SDA as output
    return ack;
}

void readDS3231Time(unsigned char *seconds, unsigned char *minutes, unsigned char *hours, unsigned char *days, unsigned char *dates, unsigned char *months, unsigned char *years) {
    I2C_start();                   // DEVICE SELECT
    //function(Argument);
    I2C_writeByte(0xD0);        // DS3231 address + write bit     1101 0000 30
    I2C_writeByte(0x00);        // Start at register 0 (seconds)  0000 0000 00
    I2C_stop();
    I2C_start();
    I2C_writeByte(0xD1);        // DS3231 address + read bit      1101 0001 31
    *seconds = I2C_readByte(0);  // Read seconds  
    *minutes = I2C_readByte(0);  // Read minutes
    *hours = I2C_readByte(0);    // Read hours
    *days = I2C_readByte(0);    // Read day
    *dates = I2C_readByte(0);    // Read date
    *months = I2C_readByte(0);    // Read month
    *years = I2C_readByte(1);    // Read year
    I2C_stop();
}

unsigned char I2C_readByte(unsigned char ack) {
    unsigned char bit, data = 0;

    pinMode(SDA, INPUT);//P1DIR &= ~SDA;          // Set SDA as input

    for (bit = 0; bit < 8; bit++) {
        data <<= 1;
        I2C_delay();
        digitalWrite(SCL, 1);//P1OUT |= SCL;       // Set SCL high
        if(digitalRead(SDA))//;// (P1IN & SDA) 
        {
            data |= 0x01;   // Read data bit
        }
        I2C_delay();
        digitalWrite(SCL, 0);//P1OUT &= ~SCL;      // Set SCL low
    }

    if (ack) {
        digitalWrite(SDA, 1);//P1OUT |= SDA;       // Send NACK
    } else {
        digitalWrite(SDA, 0);//P1OUT &= ~SDA;      // Send ACK
    }
    pinMode(SDA, OUTPUT);//P1DIR |= SDA;           // Set SDA as output
    I2C_delay();
    digitalWrite(SCL, 1);//P1OUT |= SCL;           // Set SCL high
    I2C_delay();
    digitalWrite(SCL, 0);//P1OUT &= ~SCL;          // Set SCL low
    return data;
}

void READ_EEPROM(void) {
      

  
    I2C_start();                   // DEVICE SELECT
    I2C_writeByte(0xAE);        // DS3231 address + write bit
//if(EVENT_CONT<512)
{
//    i=EVENT_CONT*8;

//    msb_adr=i>>8;  //msb
//    lsb_adr=i & 0x00ff;  //lsb

}    
    
    I2C_writeByte(msb_adr);        // Start at register 0 (seconds)
    I2C_writeByte(lsb_adr);
    I2C_stop();
    I2C_start();
    I2C_writeByte(0xAF);        // DS3231 address + read bit
    EE_ARRAY[0] = I2C_readByte(0);  // Read seconds
    EE_ARRAY[1] = I2C_readByte(0);  // Read minutes
    EE_ARRAY[2] = I2C_readByte(0);    // Read hours
    EE_ARRAY[3] = I2C_readByte(0);    // Read day
    EE_ARRAY[4] = I2C_readByte(0);    // Read date
    EE_ARRAY[5] = I2C_readByte(0);    // Read month
    EE_ARRAY[6] = I2C_readByte(0);    // Read month
    EE_ARRAY[7] = I2C_readByte(1);    // Read year
    I2C_stop();
}

void WRITE_EEPROM(void) {
    //unsigned char lsb_adr,msb_adr;

    I2C_start();
    I2C_writeByte(0xAE);        // DS3231 address + write bit // EEPROM ADDR AE
if(EVENT_CONT<512)
{
    i=EVENT_CONT*8;

    msb_adr=i>>8;  //msb
    lsb_adr=i & 0x00ff;  //lsb

}

    I2C_writeByte(msb_adr);        // Start at register 0 (seconds)
    I2C_writeByte(lsb_adr);
    I2C_writeByte(EE_ARRAY[0]);     // Write seconds
    I2C_writeByte(EE_ARRAY[1]);     // Write minutes
    I2C_writeByte(EE_ARRAY[2]);       // Write hours
    I2C_writeByte(EE_ARRAY[3]);       // Write day
    I2C_writeByte(EE_ARRAY[4]);       // Write date
    I2C_writeByte(EE_ARRAY[5]);       // Write mnth
    I2C_writeByte(EE_ARRAY[6]);       // Write year
    I2C_writeByte(EE_ARRAY[7]);       // Write year
    I2C_stop();
    delay(5);
}

void Event_log(void)
{
    readDS3231Time(&RTCsec, &RTCmin, &RTChour,&RTCday,&RTCdate,&RTCmonth,&RTCyear);

    EE_ARRAY[0]=EVENT_ID;
    EE_ARRAY[1]=ON_OFF_ST;
    EE_ARRAY[2]=RTCdate;
    EE_ARRAY[3]=RTCmonth;
    EE_ARRAY[4]=RTCyear;
    EE_ARRAY[5]=RTChour;
    EE_ARRAY[6]=RTCmin;
    EE_ARRAY[7]=RTCsec;
     WRITE_EEPROM();



     EVENT_CONT++;
     I2C_start();
     I2C_writeByte(0xAE);        // DS3231 address + write bit // EEPROM ADDR AE        1010 1110
     I2C_writeByte(0x0F);        // Start at register 0 (seconds)                       0000 1111
     I2C_writeByte(0xFA); //   4090                                                     1111 1010
     EE_ARRAY[0]=EVENT_CONT>>8;         //msb
     EE_ARRAY[1]=EVENT_CONT & 0x00ff;  //lsb
      I2C_writeByte(EE_ARRAY[0]);     // Write seconds
      I2C_writeByte(EE_ARRAY[1]);     // Write minutes
      I2C_stop();
    delay(5);
//for(i=0;i<5000;i++);
}


void READ_EVENTN(void) {
    I2C_start();                   // DEVICE SELECT
    I2C_writeByte(0xAE);        // DS3231 address + write bit         1010 1110
    I2C_writeByte(0x0F);        // Start at register 0 (seconds)      0000 1111
    I2C_writeByte(0xFA);      //                                      1111 0001
    I2C_stop();
    I2C_start();
    I2C_writeByte(0xAF);        // DS3231 address + read bit          1010 1111
    EE_ARRAY[0] = I2C_readByte(0);  // Read seconds 
    EE_ARRAY[1] = I2C_readByte(0);  // Read minutes
   EVENT_CONT= (EE_ARRAY[0]<<8) | EE_ARRAY[1];
    //EE_ARRAY[2] = I2C_readByte(0);    // Read hours
    //EE_ARRAY[3] = I2C_readByte(0);    // Read day
    //EE_ARRAY[4] = I2C_readByte(0);    // Read date
    //EE_ARRAY[5] = I2C_readByte(0);    // Read month
   // EE_ARRAY[6] = I2C_readByte(1);    // Read year
    I2C_stop();
}
void READ_SERNO(void){
    I2C_start();                   // DEVICE SELECT
    I2C_writeByte(0xAE);        // DS3231 address + write bit         1010 1110
    I2C_writeByte(0x0F);        // Start at register 0 (seconds)      0000 1111
    I2C_writeByte(0xFC);      //                                      1111 0001
    I2C_stop();
    I2C_start();
    I2C_writeByte(0xAF);        // DS3231 address + read bit          1010 1111
    EE_ARRAY[0] = I2C_readByte(0);  // Read seconds 
    EE_ARRAY[1] = I2C_readByte(0);  
    EE_ARRAY[2] = I2C_readByte(0);  
    EE_ARRAY[3] = I2C_readByte(0); 
    SERIAL_NO= (EE_ARRAY[0]<<24) | (EE_ARRAY[1]<<16)| (EE_ARRAY[2]<<8) | EE_ARRAY[3];
    I2C_stop();
}




void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload, length);
  String task = doc["task"];
  String id = doc["id"];

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

    // StaticJsonDocument<200> doc;
    // DeserializationError error = deserializeJson(doc, payload, length);

    // String task = doc["task"];
    // String id = doc["id"];




    if (id.equals("2")) { { // Change this ID based on ESP's machine ID
        if (task.equals("start")) {
            digitalWrite(ledPin, HIGH);
            Serial.println("LED turned ON");
        } else if (task.equals("stop")) {
            digitalWrite(ledPin, LOW);
            Serial.println("LED turned OFF");
        }
    }



  //if (message == "on") 
    if((char)payload[12] == 'o'&& (char)payload[13] == 'n'){
    digitalWrite(ledPin, HIGH);  // Turn LED ON
    Serial.println("LED turned ON");
  } //else if (message == "off") 
   else if((char)payload[12] == 'o'&& (char)payload[13] == 'f'&& (char)payload[14] == 'f')
  {
    digitalWrite(ledPin, LOW);   // Turn LED OFF
    Serial.println("LED turned OFF");
  }
  // Switch on the LED if an 1 was received as first character




    if((char)payload[12] == 'C'&& (char)payload[13] == '1'&& (char)payload[14] == '-'&& (char)payload[15] == '0' && (char)payload[16] == '0'&& (char)payload[17] == '1'){

      if (b1_Blocked  ==  0){

        Switch1_triggered = 1;
        b1_press = 1;
        b1_Blocked  =  1;
        Serial.println("StartButtonPressed");
      }
    }
    if((char)payload[12] == 'C'&& (char)payload[13] == '1'&& (char)payload[14] == '-'&& (char)payload[15] == '0' && (char)payload[16] == '0'&& (char)payload[17] == '2'){


      if (b2_Blocked  ==  0){

        Switch2_triggered = 1;
        b2_press = 1;
        b2_Blocked  =  1;
        Serial.println("StopButtonPressed");
      }
    }
    if((char)payload[12] == 'C'&& (char)payload[13] == '1'&& (char)payload[14] == '-'&& (char)payload[15] == '0' && (char)payload[16] == '0'&& (char)payload[17] == '3'){

      if (b3_Blocked  ==  0){

        Switch3_triggered = 1;
        b3_press = 1;
        b3_Blocked  =  1;
        Serial.print("Relay1TurnedON");
      }
    }


  if(length>11)
  {
              if((char)payload[0] == 'S'&& (char)payload[1] == 'E'&& (char)payload[2] == 'T'&& (char)payload[3] == 'T')
               {
                   // set time here
                   RTCsec=payload[5];
                   RTCmin=payload[6];
                   RTChour=payload[7];
                   RTCday=payload[8];
                   RTCdate=payload[9];
                   RTCmonth=payload[10];
                   RTCyear=payload[11];
                   setDS3231Time(RTCsec, RTCmin, RTChour,RTCday,RTCdate,RTCmonth,RTCyear); // Seconds, Minutes, Hours
                   Serial.print("TIME SET OK");
                   client.publish("ESP8266RES","TIME SET OK");


               }
              else if((char)payload[0] == 'R'&& (char)payload[1] == 'E'&& (char)payload[2] == 'A'&& (char)payload[3] == 'D')
               {
                    RTCsec=0;
                    RTCmin=0;
                    RTChour=0;
                    RTCday=0;
                    RTCdate=0;
                    RTCmonth=0;
                    RTCyear=0;
                    readDS3231Time(&RTCsec, &RTCmin, &RTChour,&RTCday,&RTCdate,&RTCmonth,&RTCyear);
                    Serial.print("DATE:");
                    //client.publish("ESP8266RES", "THE DATE IS:");
                    snprintf (msg, MSG_BUFFER_SIZE, "DATE :%2x/%2x/%2x TIME: %2x:%2x:%2x", RTCdate,RTCmonth,RTCyear,RTChour,RTCmin,RTCsec);
                    client.publish("ESP8266RES", msg);
                           
                    digit=(RTCdate>>4);//+0x30;
                    Serial.print(digit,HEX);
                    digit=(RTCdate & 0x0f);//+0x30;
                    Serial.print(digit,HEX);
                    Serial.print('/');
                    
                    digit=(RTCmonth>>4);//+0x30;
                    Serial.print(digit,HEX);
                    digit=(RTCmonth & 0x0f);//+0x30;
                    Serial.print(digit,HEX);
                    Serial.print('/');                           

                    digit=(RTCyear>>4);//+0x30;
                    Serial.print(digit,HEX);
                    digit=(RTCyear & 0x0f);//+0x30;
                    Serial.print(digit,HEX);
                  // UART_sendChar('/');       
                    Serial.print("  TIME:");
                    digit=(RTChour>>4);//+0x30;
                    Serial.print(digit,HEX);
                    digit=(RTChour & 0x0f);//+0x30;
                    Serial.print(digit,HEX);
                    Serial.print(':');
                    digit=(RTCmin>>4);//+0x30;
                    Serial.print(digit,HEX);
                    digit=(RTCmin & 0x0f);//+0x30;
                    Serial.print(digit,HEX);
                    Serial.print(':');                          
                    //UART_sendChar(RTCmin);
                    digit=(RTCsec>>4);//+0x30;
                    Serial.print(digit,HEX);
                    digit=(RTCsec & 0x0f);//+0x30;
                    Serial.print(digit,HEX);
                    Serial.print('\n');                          

                  }
              else if((char)payload[0] == 'E'&& (char)payload[1] == 'V'&& (char)payload[2] == 'E'&& (char)payload[3] == 'N'&& (char)payload[4] == 'T'&& (char)payload[5] == 'N'&& (char)payload[6] == 'U')
                  {
                    Serial.print("NUMBER OF EVENT IS: ");
                                           
                    digit=    EVENT_CONT/100;    
                    digit=digit;//+0x30;
                    Serial.print(digit,HEX);

                    temp=EVENT_CONT%100;  
                    digit= temp/10; 

                    digit=digit;//+0x30;
                    Serial.print(digit,HEX);
                    digit= temp%10; 
                    digit=digit;//+0x30;
                    Serial.print(digit,HEX);
                    snprintf (msg, MSG_BUFFER_SIZE, "NUMBER OF EVENT IS: %3d",EVENT_CONT);
                    client.publish("ESP8266RES",msg);
                  }
              else if((char)payload[0] == 'F'&& (char)payload[1] == 'A'&& (char)payload[2] == 'C'&& (char)payload[3] == 'T'&& (char)payload[4] == 'O'&& (char)payload[5] == 'R'&& (char)payload[6] == 'Y')
                  {
                                         
                    EVENT_CONT=0;
                    I2C_start();
                    I2C_writeByte(0xAE);        // DS3231 address + write bit // EEPROM ADDR AE  1010 1110
                    I2C_writeByte(0x0F);        // Start at register 0 (seconds)                 0000 1111
                    I2C_writeByte(0xFA); //   4090                                               1111 1010
                    EE_ARRAY[0]=0;  //msb most significant bit
                    EE_ARRAY[1]=0;  //lsb least significant bit
                    I2C_writeByte(EE_ARRAY[0]);     // Write seconds
                    I2C_writeByte(EE_ARRAY[1]);     // Write minutes
                    I2C_stop();
                    delay(5);//for(i=0;i<5000;i++);
                    Serial.print("Factory INIT DONE");
                    client.publish("ESP8266RES","Factory INIT DONE");
                  }

              else if((char)payload[0] == 'S'&& (char)payload[1] =='E'&& (char)payload[2] == 'R'&& (char)payload[3] == 'I'&& (char)payload[4] == 'A'&& (char)payload[5] == 'L'&& (char)payload[6] == 'S')
                  {
                                         
                    EVENT_CONT=0;
                    I2C_start();
                    I2C_writeByte(0xAE);        // DS3231 address + write bit // EEPROM ADDR AE  1010 1110
                    I2C_writeByte(0x0F);        // Start at register 0 (seconds)                 0000 1111
                    I2C_writeByte(0xFC); //   4092/92/94/95   4b for  serial no.                                               1111 1010
                    EE_ARRAY[0]=payload[8];  //msb most significant bit
                    EE_ARRAY[1]=payload[9];  //lsb least significant bit
                    EE_ARRAY[2]=payload[10];  //msb most significant bit
                    EE_ARRAY[3]=payload[11];  //lsb least significant bit

                  SERIAL_NO=EE_ARRAY[0]<<24;
                  SERIAL_NO |=EE_ARRAY[1]<<16;
                  SERIAL_NO |=EE_ARRAY[2]<<8;
                  SERIAL_NO |=EE_ARRAY[3];




                    I2C_writeByte(EE_ARRAY[0]);     // Write seconds
                    I2C_writeByte(EE_ARRAY[1]);     // Write minutes
                    I2C_writeByte(EE_ARRAY[2]);     // Write seconds
                    I2C_writeByte(EE_ARRAY[3]);     // Write minutes
                    I2C_stop();
                    delay(5);//for(i=0;i<5000;i++);
                    Serial.print("SERIAL NUMBER SET OKAY");
                    client.publish("ESP8266RES","SERIAL NUMBER SET DONE");
                  }
                      else if((char)payload[0] == 'R'&& (char)payload[1] == 'E'&& (char)payload[2] == 'A'&& (char)payload[3] == 'D'&& (char)payload[4] == 'S'&& (char)payload[5] == 'R'&& (char)payload[6] == 'N')
                  {
                    Serial.print("SERIAL NUMBER IS: ");
                    digit=    SERIAL_NO/1000;    
                    digit=digit;//+0x30;
                    Serial.print(digit,HEX);
                   
                    temp=SERIAL_NO%1000; 
                    digit=    SERIAL_NO/100;    
                    digit=digit;//+0x30;
                    Serial.print(digit,HEX);

                    temp=SERIAL_NO%100;  
                    digit= temp/10; 

                    digit=digit;//+0x30;
                    Serial.print(digit,HEX);
                    digit= temp%10; 
                    digit=digit;//+0x30;
                    Serial.print(digit,HEX);
                    snprintf (msg, MSG_BUFFER_SIZE, "SERIAL NUMBER IS: %4d",SERIAL_NO);
                    client.publish("ESP8266RES",msg);
                  }

                else if((char)payload[0] == 'S'&& (char)payload[1] == 'E'&& (char)payload[2] == 'N'&& (char)payload[3] == 'D'&& (char)payload[4] == 'L'&& (char)payload[5] == 'O'&& (char)payload[6] == 'G')
                  {
                                         
                      if(EVENT_CONT == 0)
                      {
                        Serial.print("NO LOG PRESENT");
                        client.publish("ESP8266RES","NO LOG PRESENT");
                      }
                      else
                      {
                        for(temp=0;temp<EVENT_CONT;temp++)
                            {
                      //if(EVENT_CONT<512)
                              {
                                // i=EVENT_CONT*8;
                                i=temp*8;
                                msb_adr=i>>8;  //msb
                                lsb_adr=i & 0x00ff;  //lsb

                              }    


                              READ_EEPROM();
                              if(EE_ARRAY[0]  == 1)
                              Serial.print("START BUTTON PRESSED AT: ");
                              else if(EE_ARRAY[0]  == 2)
                              Serial.print("STOP BUTTON PRESSED AT: ");
                              else if(EE_ARRAY[0]  == 3)
                              Serial.print("RELAY1 ON AT: ");
                             else if(EE_ARRAY[0]  == 4)
                              Serial.print("Bottom proximity switch: ");

                      // UART_sendChar(EE_ARRAY[2]);
                      // UART_sendChar(EE_ARRAY[3]);UART_sendChar(EE_ARRAY[4]);UART_sendChar(EE_ARRAY[5]);
                      // UART_sendChar(EE_ARRAY[6]);UART_sendChar(EE_ARRAY[7]);

                              Serial.print("DATE:");

                              digit=(EE_ARRAY[2]>>4);//+0x30;
                              Serial.print(digit);
                              digit=(EE_ARRAY[2] & 0x0f);//+0x30;
                              Serial.print(digit);
                              Serial.print('/');

                              digit=(EE_ARRAY[3]>>4);//+0x30;
                              Serial.print(digit);
                              digit=(EE_ARRAY[3] & 0x0f);//+0x30;
                              Serial.print(digit);
                              Serial.print('/');                           

                              digit=(EE_ARRAY[4]>>4);//+0x30;
                              Serial.print(digit);
                              digit=(EE_ARRAY[4] & 0x0f);//+0x30;
                              Serial.print(digit);
                      // UART_sendChar('/');       
                              Serial.print("  TIME:");
                              digit=(EE_ARRAY[5]>>4);//+0x30;
                              Serial.print(digit);
                              digit=(EE_ARRAY[5] & 0x0f);//+0x30;
                              Serial.print(digit);
                              Serial.print(':');
                              digit=(EE_ARRAY[6]>>4);//+0x30;
                              Serial.print(digit);
                              digit=(EE_ARRAY[6] & 0x0f);//+0x30;
                              Serial.print(digit);
                              Serial.print(':');                          
                              //UART_sendChar(RTCmin);
                              digit=(EE_ARRAY[7]>>4);//+0x30;
                              Serial.print(digit);
                              digit=(EE_ARRAY[7] & 0x0f);//+0x30;
                              Serial.print(digit); 
                              Serial.print('\n');
                              if(EE_ARRAY[0]  == 1)
                             // client.publish("ESP8266RES","START BUTTON PRESSED AT: ");
                              //snprintf (msg, MSG_BUFFER_SIZE, "START BUTTON PRESSED AT: ");
                              snprintf (msg, MSG_BUFFER_SIZE, "START PRESSED AT:DATE :%2x/%2x/%2x TIME: %2x:%2x:%2x",EE_ARRAY[2],EE_ARRAY[3],EE_ARRAY[4],EE_ARRAY[5],EE_ARRAY[6],EE_ARRAY[7]);
                              else if(EE_ARRAY[0]  == 2)
                              snprintf (msg, MSG_BUFFER_SIZE, "STOP PRESSED AT:DATE :%2x/%2x/%2x TIME: %2x:%2x:%2x",EE_ARRAY[2],EE_ARRAY[3],EE_ARRAY[4],EE_ARRAY[5],EE_ARRAY[6],EE_ARRAY[7]);
                              else if(EE_ARRAY[0]  == 3)
                              snprintf (msg, MSG_BUFFER_SIZE, "RELAY1 ON AT:DATE :%2x/%2x/%2x TIME: %2x:%2x:%2x",EE_ARRAY[2],EE_ARRAY[3],EE_ARRAY[4],EE_ARRAY[5],EE_ARRAY[6],EE_ARRAY[7]);
                              else if(EE_ARRAY[0]  == 4)
                              snprintf (msg, MSG_BUFFER_SIZE, "Bottom proximity switch:DATE :%2x/%2x/%2x TIME: %2x:%2x:%2x",EE_ARRAY[2],EE_ARRAY[3],EE_ARRAY[4],EE_ARRAY[5],EE_ARRAY[6],EE_ARRAY[7]);
                              else if(EE_ARRAY[0]  == 5)
                              snprintf (msg, MSG_BUFFER_SIZE, "OVER CUP ALARM:DATE :%2x/%2x/%2x TIME: %2x:%2x:%2x",EE_ARRAY[2],EE_ARRAY[3],EE_ARRAY[4],EE_ARRAY[5],EE_ARRAY[6],EE_ARRAY[7]);

                              client.publish("ESP8266RES",msg);
                              delay(200);
                      }

                      }
     
                  }



/*
  if ((char)payload[0]  ==  '1') {
    //digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    client.publish("ESP8266RES", "THE DATE IS:");// it is active low on the ESP-01)
    client.publish("ESP8266RES", "26/10/24");// it is active low on the ESP-01)
  } else {
    client.publish("ESP8266RES", "zero received");//digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
*/




  }

}


 }
// void JsonConverter(const char* msg, const char* date, const char* time){

//     // Creating a JSON document
//     DynamicJsonDocument doc(1024);
//     // Directly use constants or global variables in the JSON document
//     // doc["event_id"] = EVENT_ID;       // Event ID
//     // doc["status"] = ON_OFF_ST;        // On/Off status

//     // // Example RTC data (replace with your actual time reading code)
//     // doc["date"] = RTCdate;            // Day
//     // doc["month"] = RTCmonth;          // Month
//     // doc["year"] = RTCyear;            // Year
//     // doc["hour"] = RTChour;            // Hour
//     // doc["minute"] = RTCmin;           // Minute
//     // doc["second"] = RTCsec;           // Seconds
//     char formattedMsg[100];  // Buffer to hold the formatted message
//     sprintf(formattedMsg, "%-30s DATE: %-12s TIME: %-8s", msg, date, time);

//     // Add the serial number (e.g., 1), followed by the formatted log message
//     String serialKey = "[" + String("1") + "]";
//     doc[serialKey] = formattedMsg;  // Add the formatted log message with serial number


//     // Serialize JSON
//     String jsonString;
//     serializeJson(doc, jsonString);

//     // Publish the event data as a JSON string to your MQTT topic
//     client.publish("ESP8266RES", jsonString.c_str(), true);

//     // Increment the log counter after each message
//     logCounter++;
// }


// void JsonConverter(const char* msg, const char* date, const char* time){


