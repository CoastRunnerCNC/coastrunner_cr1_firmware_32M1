#include "grBLDC.h"
#include <stdio.h>


//Spindle RPM PID controller
int32_t errorPrevious=0; // Previous error
int32_t errorAcc=0; // Accumulated error
int32_t errorAccMax = 4177920/PID_COEFF_I;
uint8_t dutyPID = 0; //Closed loop PSC PWM Duty Cycle //0:255

// Getter/setter for PID
uint8_t pid_dutyCycle_get() { return dutyPID; }
void pid_dutyCycle_set(uint8_t newDuty) { dutyPID = newDuty; }

// Actual PID algorithm
uint8_t pid_update(int16_t error,int16_t dt)
{
	static int32_t pidFrac = 0; // Result in fractional form
	int32_t pidInt; // Result integer part
	
  	// Accumulate error to compute I
	errorAcc+=error*dt;
	
	if(errorAcc>errorAccMax)
		errorAcc = errorAccMax;
	
	if(errorAcc<-errorAccMax)
		errorAcc = -errorAccMax;

	// Compute P, I and D parts
	int32_t pVal = PID_COEFF_P*error;
	int32_t iVal = PID_COEFF_I*errorAcc;
	int32_t dVal = PID_COEFF_D*(error-errorPrevious)/dt;
	

	
	// Compute PID result in fractional form
	pidFrac = (pVal+iVal+dVal);
	
	if(pidFrac<0)
		pidFrac = 0;

	// Compute integer part of PID
	pidInt = PID_MIN_VALUE+(pidFrac>>PID_COEF_SCALE_LOG2);
	
  // Bound min/max values of control
	if(pidInt > PID_MAX_VALUE)
	pidInt = PID_MAX_VALUE;
	if(pidInt < PID_MIN_VALUE)
	pidInt = PID_MIN_VALUE;
	
	return (uint8_t) pidInt;
}

// Reset PID static parameters
void pid_reset()
{
	errorPrevious=0;
	errorAcc=0;
}

void pid_loop(int16_t dt)
{
    #ifdef SPINDLE_MODE_CLOSED_LOOP
    if(adc_goalRPM_get() > MIN_ALLOWED_RPM)
    {
	    int16_t error = (int16_t)adc_goalRPM_get()-(int16_t)timing_measuredRPM_get();
	    dutyPID = pid_update(error,dt);
    }
    else
    {
	    pid_reset();
	    dutyPID=0;
	    motor_stop();
    }
    #elif defined SPINDLE_MODE_OPEN_LOOP
    dutyPID = OPEN_LOOP_STATIC_PSC_DUTY_CYCLE;
    #endif
}

// Pid scheduler: Adjusts motor control depending on error
// Note: currently not used !
void pid_scheduler(void)
{
  static uint16_t timeSinceLastUpdate_PID = 0;
  

  if(++timeSinceLastUpdate_PID >= 200)
  {
    pid_loop(1);
    timeSinceLastUpdate_PID = 0;
  }
}