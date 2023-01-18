#include <dht.h>


#define dht_apin D0 // Analog Pin sensor is connected to
 
dht DHT;

#include <Servo.h>

Servo myservo;  

int pos = 0; 

void setup(){
 
  Serial.begin(115200);
  myservo.attach(D1);
  delay(500);//Delay to let system boot
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(1000);//Wait before accessing Sensor
 
}//end "setup()"
 
void loop(){
  //Start of Program 
 
    DHT.read11(dht_apin);
    
    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");
    
    delay(5000);//Wait 5 seconds before accessing sensor again.
  if(DHT.temperature > 30)
  {
          for (pos = 180; pos >= 0; pos -= 1) { 
    myservo.write(pos);              
    delay(30);
      
  }
  }  else
  {
   for (pos = 0; pos <= 180; pos += 1) 
    // in steps of 1 degree
    myservo.write(pos);              
    delay(30);                    

  }
  ///Fastest should be once every two seconds.
 
  }// end loop(
