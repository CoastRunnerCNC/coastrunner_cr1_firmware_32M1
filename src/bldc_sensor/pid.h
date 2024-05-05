#ifndef _PID_H_
	#define _PID_H_
	
	// Min and Max value for result
	#define PID_MIN_VALUE 150
	#define PID_MAX_VALUE 255

	// PID coefficients
	#define PID_COEFF_P 8L
	#define PID_COEFF_I 3L
	#define PID_COEFF_D 0L
	#define PID_COEF_SCALE_LOG2 14 // Number of fractional bits used to represent PID

	#define PID_KICKSTART_DUTY PID_MIN_VALUE

    //#define SPINDLE_MODE_OPEN_LOOP
    #define SPINDLE_MODE_CLOSED_LOOP

    #define OPEN_LOOP_STATIC_PSC_DUTY_CYCLE 250 //0: 0% PWM //255: 100% PWM

    #define PID_UPDATE_PERIOD_MICROSECONDS 1000

    #define MIN_ALLOWED_RPM 1500 //must be larger than ADC_COUNTS_TO_RPM__OFFSET (or spindle won't stop spinning)

	uint8_t pid_dutyCycle_get(void);
	void pid_dutyCycle_set(uint8_t newDuty);
    void pid_scheduler(void);
	void pid_reset();
    uint8_t pid_update(int16_t error,int16_t dt);
	void pid_loop(int16_t dt);

#endif
