#include "mbed.h"

#include "bbcar.h"


Ticker servo_ticker;

PwmOut pin5(D5), pin6(D6);
// pin 5 is slower one 
// pin 6 is faster one

BBCar car(pin5, pin6, servo_ticker);


int main() {

   // please contruct you own calibration table with each servo
   double pwm_table0[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};

   double speed_table0[] = {-12.038, -11.719, -11.958, -10.603, -5.022, 0.000, 5.820, 9.806, 10.125, 11.161, 10.683};

   double pwm_table1[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};

   double speed_table1[] = {-21.126, -22.322, -21.684, -21.844, -12.357, 0.000, 14.111, 21.924, 21.525, 22.083, 21.445};


   // first and fourth argument : length of table

   car.setCalibTable(11, pwm_table0, speed_table0, 11, pwm_table1, speed_table1);


   while (1) {

      car.goStraightCalib(5);

      ThisThread::sleep_for(5s);

      car.stop();

      ThisThread::sleep_for(5s);

   }

}