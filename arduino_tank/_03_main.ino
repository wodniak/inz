Tank tank;

void setup() 
{
  tank.init();
  Serial.begin(115200);
}
void loop() 
{
  tank.get_speed();
  tank.drive();
  tank.use_sensors();
}
