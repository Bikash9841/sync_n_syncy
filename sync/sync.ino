

// arduino receives the value sent from python through software interrupt..
// after doing operation on the value, arduino request for new value from python
// through hardware interrupt( pressing push button)





//Cicuit:
// pin 4 and 5 ground for 2 leds respecti..... led1-->on (software interrupt)
//                                            led2--> on(hardware interrupt)

// vcc for both leds is given from arduino 5V pin


#include <TimerOne.h>
#define gnd 4
#define gnd2 5
#define rupt 2          //interrupt pins in arduino uno is 2 and 3
String msg = "nex";     //arduino send this msg to python to request for next data
int prev = 0;           //while reading python data,previous value might occur..so keepoing track of prev value
// to serial read again if so happens.

int flag = 0;          // indicator to perform task on received data

// arduino read character from serial comm and we convert it into integer
char v;
int d;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(gnd, OUTPUT);
  pinMode(gnd2, OUTPUT);
  pinMode(rupt, INPUT);       //interrupt pin should be input to arduino as hardware interrupts are external interrupts
  digitalWrite(gnd2, HIGH);

  pinMode(LED_BUILTIN, OUTPUT);
  Timer1.initialize(1000);

  Timer1.attachInterrupt(r_data);   //software interrupt
  attachInterrupt(digitalPinToInterrupt(rupt), request_data, FALLING); //hardware interrupt

}

void loop() {

  // put your main code here, to run repeatedly:

  //  operation on data if flag==1

  if ((flag == 1)) {

    //software interrupt stops while performing task on data,restarts after requesting new data from python
    Timer1.stop();

    digitalWrite(gnd, LOW);
    digitalWrite(gnd2, HIGH);

    flag = 0;
    delay(5000);

  } else {
    digitalWrite(gnd, HIGH);
    digitalWrite(gnd2, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    flag = 0;
  }

}


// ISR for soft interrupt
void r_data() {
  while (Serial.available() > 0) {
    v = Serial.read();

    //this method only converts the single character into integer
    d = v - '0';  

//    if prev value occurs, read again
    while (d == prev) {
      v = Serial.read();
      d = v - '0';
    }
    prev = d;

  }
  if (d == 3) {
    digitalWrite(LED_BUILTIN, HIGH);
    flag = 1;
  }
}

//ISR for hard interrupt
void request_data() {
  digitalWrite(gnd2, LOW);
  digitalWrite(gnd, HIGH);

  //  sending msg to python for next data
  Serial.print(msg);
  Timer1.restart();
}
