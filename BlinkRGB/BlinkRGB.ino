#include <avr/sleep.h>
#include <avr/power.h>

const int red{0};
const int green{1};
const int blue{2};
const int swtch{3};
const int sleepTime{500};
const int cycles{2};

void go_to_sleep()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  ADCSRA = 0; //Disable ADC because it's never used and just drains battery.
  power_all_disable(); //Power off ADC, timers (0 and 1), and the serial interface
  sleep_enable();
  sleep_cpu();
  sleep_disable();
  power_all_enable();
}

void write_rgb_time(int redValue, int greenValue, int blueValue, int delayTime)
{
  digitalWrite(red,redValue);
  digitalWrite(green,greenValue);
  digitalWrite(blue,blueValue);
  delay(delayTime);  
}

void light_show()
{
  for (int i{0};i<cycles;i++){
    write_rgb_time(LOW, LOW, HIGH, sleepTime);
    write_rgb_time(HIGH, LOW, LOW, sleepTime);
    write_rgb_time(HIGH, HIGH, LOW, sleepTime);
    write_rgb_time(LOW, HIGH, LOW, sleepTime);
    write_rgb_time(LOW, HIGH, HIGH, sleepTime);
    write_rgb_time(LOW, LOW, HIGH, sleepTime);
  }
  write_rgb_time(LOW, LOW, LOW, sleepTime);
}

ISR (PCINT0_vect)
{
  sleep_disable();
  digitalWrite(red,HIGH);
  digitalWrite(green,HIGH);
  digitalWrite(blue,HIGH);  
}

void setup() 
{
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(swtch, INPUT);

  //DeepSleepModeStuff
  ADCSRA = 0; //Disable ADC because it's never used and just drains battery.
  SREG = 0b10000000; //Enable global interrupts
  GIFR = 0b00100000;    // clear any outstanding interrupts
  GIMSK = 0b00100000;  //Enable pin change interrupts
  PCMSK = 0b00001000;  //Enable pin 3 interrupt
}

void loop() 
{
  light_show();
  go_to_sleep();
  delay(500);
}
