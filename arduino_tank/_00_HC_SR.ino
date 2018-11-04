class Hc_sr
/*
 * HC-SR04 sensor
*/
{
  private:
      int trig;
      int echo;
      long echo_time;
      int distance;

      int max_range = 200;
      int min_range = 4;
  
  public:
      Hc_sr(int t, int e)
      {
        trig = t;
        echo = e;

        //set pin modes
        pinMode(trig, OUTPUT);
        pinMode(echo, INPUT);  
      }

      //gets measurement in cm
      int measure()
      {
        // set TRIG low for 2us
        digitalWrite(trig, LOW);
        delayMicroseconds(2);
       
        // set TRIG high for 10us
        digitalWrite(trig, HIGH);
        delayMicroseconds(10);
       
        // set TRIG low - measure start
        digitalWrite(trig, LOW);
       
        // measure high state time on ECHO
        echo_time = pulseIn(echo, HIGH);
       
        // calculate distance in cm
        distance = echo_time / 58;
       
        // check measurement range
        if (distance >= max_range)
        {
          distance = 200;  
        }
        else if (distance <= min_range)
        {
          distance = 0;
        }  
      return distance;
      }

      void send_data()
      {
        //Serial3.print(distance);
        //Serial3.print("\n\r");
      }
};
