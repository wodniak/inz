class Tank
/*
 * Sterowanie: 2 bajty, wartosci 0-127 (do przodu) 128-255 (do tylu)
 * Example: 127 127 (full forward), 
 *          128 128 (full rear), 
 *          127 64 (turn left)
*/
{
  private:
    //right 
    int AIN_1  = 3;
    int AIN_2  = 2;
    int pwm_1  = 0;
    bool dir_1 = true;  //forward, false=rear
    
    //left
    int BIN_1  = 4;
    int BIN_2  = 5;
    int pwm_2  = 0;
    bool dir_2 = true;

    //sensors
    Hc_sr hc_right = Hc_sr(11, 10); //trigger = 11, echo = 10
    Hc_sr hc_left  = Hc_sr(9, 8);   //trigger = 9, echo = 8
    Hc_sr hc_front = Hc_sr(7, 6);   //trigger = 7, echo = 6

  public:
    void init()
    {
       //enable pins 
       pinMode(AIN_1, OUTPUT);
       pinMode(AIN_2, OUTPUT);
       pinMode(BIN_1, OUTPUT);
       pinMode(BIN_2, OUTPUT);

       //enable serial
       Serial3.begin(115200);

    }

    //reads serial, speed in range <0 - 128 - 255> (rearmax, 0, forwardmax)
    void get_speed()
    {
      while (Serial3.available() > 0)
        {
           //right wheel
           if (dir_1 == false)    //from rear to front
              dir_1 = !dir_1;
              
           pwm_1 = Serial3.read();
           
           if (pwm_1 > 127)
           {
              pwm_1 = 255 - pwm_1;
              if (dir_1 == true)  //from front to rear
                dir_1 = !dir_1;
           }

           //left wheel
           if (dir_2 == false)  //from rear to front
              dir_2 = !dir_2;
              
           pwm_2 = Serial3.read();
           
           if (pwm_2 > 127) 
           {
              pwm_2 = 255 - pwm_2;
              if (dir_2 == true)  //from front to rear
                dir_2 = !dir_2;
           }

          //bug solution to getting -2 at output
          if(pwm_2 < 0)
            pwm_2 = 0;
          if(pwm_1 < 0)
            pwm_1 = 0;
            
          //double values to be in range 0-255
          pwm_1 *= 2;
          pwm_2 *= 2;
        }

        
        Serial.print(pwm_1);
        Serial.print("   ");
        Serial.print(pwm_2);
        Serial.print("   ");
        Serial.print(dir_1);
        Serial.print("   ");
        Serial.print(dir_2);
        Serial.print("\n");
        delay(100);
        
    }

    //sets speed of tanks both catterpilars
    void drive()
    {
        //left side
        if (dir_1 == true)
        {
            digitalWrite(AIN_1, LOW);
            analogWrite(AIN_2, pwm_1);
        }
        else
        {
            digitalWrite(AIN_2, LOW);
            analogWrite(AIN_1, pwm_1);
        }
        
        //right side
        if (dir_2 == true)
        {
            digitalWrite(BIN_1, LOW);
            analogWrite(BIN_2, pwm_2);
        }
        else
        {
            digitalWrite(BIN_2, LOW);
            analogWrite(BIN_1, pwm_2);
        }
    }

    void use_sensors()
    {
        int all_sensors[3];
        all_sensors[0] = hc_right.measure();
        all_sensors[1] = hc_left.measure();
        all_sensors[2] = hc_front.measure();
        Serial3.print("[");
        Serial3.print(all_sensors[0]);
        Serial3.print(" ");
        Serial3.print(all_sensors[1]);
        Serial3.print(" ");
        Serial3.print(all_sensors[2]);
        Serial3.print("]");
        Serial3.print("\n\r");
        delay(10);
    }
};
