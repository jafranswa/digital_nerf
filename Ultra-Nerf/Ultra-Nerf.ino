// Created By Michael Dixon (Ultrasonic2) Auckland New Zealand

//Battery + "Bucked" down > 9ish volts in at VIN
//Battery - > GND

//Trigger > D2
//Trigger > GND

// Servo Signal (PULSE) > D9
// Servo - > D7

// Trimpot > +5v
// trimpot > A7



//----SETTINGS YOU CAN CHANGE
//Brake time setting
float FallToTime = 4000; //ms
//accelerate time setting
float RiseToTime = 50; //ms. Start with 200ms for 4s

int EnablePot =0 ; // 1 = yes 0 = no
int InveretPotDirection = 0; // 1 = yes  0 =no



// don't change the below setting 

#include <Servo.h>
Servo myservo; 
Servo myservo2; 
int RevPulled; 
int PreviousFlyWheel = 0; //off
float MaxServo = 2000;
float MinServo = 1000;
float CurrentServoTime = MinServo;
float TimefromTriggerPulled;
float TimefromTriggerchanged;


float TimeFromTrigReleaseMS;
float DiffBetweenBLMinMax;
float MStoTrottleGain;
float temp1;
float temp2;
float temp3;
int CurrentFlyWheel;
float PercentofFallToTime;
float TimefromBoot;

int CurrentPot;
//

void setup() {
   myservo.attach(9);
   myservo2.attach(10);
   pinMode(2, INPUT_PULLUP);
   digitalWrite(7,LOW);
   Serial.begin(9600); //hum
 //  myservo.write(MinServo);
  myservo.write(1000);
  myservo2.write(1000);
  delay(9000);
  CurrentPot = MaxServo;
  
}

void Buttonsstate()// de bounce 
{
   CurrentFlyWheel = digitalRead(2); 
   
  if (CurrentFlyWheel != PreviousFlyWheel )
  {
    //Serial.println("CurrentFlyWheel"); //hum
    delay (10);
    RevPulled =digitalRead(2);
    
     if (RevPulled != PreviousFlyWheel )
      {
      TimefromTriggerchanged = millis();
      }
      PreviousFlyWheel = RevPulled;
  }
}// buttonState




void RevDown()// RevDown 
{

  TimeFromTrigReleaseMS = TimefromBoot - TimefromTriggerchanged;
 if (TimeFromTrigReleaseMS > 0){
       PercentofFallToTime = ( TimeFromTrigReleaseMS/FallToTime) *100;  

       temp1 =  ((1000/100*PercentofFallToTime));
       temp2 = 1000 - temp1 + temp3;
            
        if (temp2 < MinServo) {
          temp2 = MinServo;
          }

        CurrentServoTime = temp2;
 }
else
{
   temp3=(CurrentServoTime-1000) ;
}

}// RevDown


void RevUp()
{

   TimeFromTrigReleaseMS = TimefromBoot - TimefromTriggerchanged;

if (TimeFromTrigReleaseMS > 0){
  
       PercentofFallToTime = (TimeFromTrigReleaseMS/RiseToTime) *100;

       temp1 =  ((1000/100*PercentofFallToTime));
       temp2 = 1000 + temp1 + temp3;
       
        if (temp2 > MaxServo) {
          temp2 = MaxServo;
          }
       
       CurrentServoTime = temp2; 
}
else
{
   temp3=(CurrentServoTime-1000) ;
}


}/// REVUP

void pot(){
  CurrentPot = analogRead(7);
   Serial.print(CurrentPot);


  if (InveretPotDirection == 0){
    if (CurrentPot > 1010 ){
      CurrentPot = 2000;
    }
    else{
      CurrentPot = 1000+ CurrentPot ; // yes this is pointless :)
    }
  }
  else{
     if (CurrentPot < 13 ){
      CurrentPot = 2000;
    }
    else{
      CurrentPot = (1023-CurrentPot)+1000 ; // yes this is pointless :)
    }
    
  }
  
 
}


void loop() {
   Buttonsstate();

  TimefromBoot = millis();
  if (EnablePot ==1){
    pot();
  }

  MaxServo = CurrentPot;
   
  if  (RevPulled ==LOW )// pulled, REV UP
  {
    RevUp();
    digitalWrite(13, HIGH);
  }
  else ////// REV DOWN
  {
      RevDown();
      digitalWrite(13, LOW);
      //Stop Motor
  }

 myservo.write(CurrentServoTime);
 myservo2.write(CurrentServoTime);

//delay (1); // TEMPPPPPPPPPPPPPPPPPPPP
}
