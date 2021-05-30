#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);

// D6 is slower
// D5 is faster
BufferedSerial xbee(D1, D0);

BBCar car(pin5, pin6, servo_ticker);
DigitalIn encoder(D11);
DigitalOut led1(LED3);

volatile int stepcounter0 = 0;
Ticker encoder_ticker;



volatile int last = 0;

void encoder_read() {
   int value = encoder;
   if (!last && value) stepcounter0++;
   last = value;
}


int main() {
   // set BBCar
   // please contruct you own calibration table with each servo
   double pwm_table0[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
   double speed_table0[] = {-21.126, -22.322, -21.684, -21.844, -10.357, 0.000, 11.111, 21.924, 21.525, 22.083, 21.445};
   double pwm_table1[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};
   double speed_table1[] = {-12.038, -11.719, -11.958, -10.603, -5.022, 0.000, 5.820, 9.806, 10.125, 11.161, 10.683};
   // first and fourth argument : length of table
   car.setCalibTable(11, pwm_table0, speed_table0, 11, pwm_table1, speed_table1);
   // set op
   encoder_ticker.attach(&encoder_read, .01);
 
   // set Xbee
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");
   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
   RPC::call(buf, outbuf);
   }
}