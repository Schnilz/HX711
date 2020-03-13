/**
 *
 * HX711 library for Arduino
 * https://github.com/bogde/HX711
 *
 * MIT License
 * (c) 2018 Bogdan Necula
 *
**/
#ifndef HX711_h
#define HX711_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class HX711
{
	private:
		byte PD_SCK;	// Power Down and Serial Clock Input Pin
		byte DOUT;		// Serial Data Output Pin
		byte GAIN;		// amplification factor
		
		float SMOOTHING_FACTOR = 1;		// smoothing factor (https://en.wikipedia.org/wiki/Exponential_smoothing)
		float SMOOTHING_FACTOR_1 = 0;	//1 -  smoothing factor
		long raw_value = 0; 			// last value read
		float avrg_value = 0;			// smoothed Value

		float OFFSET = 0;	// used for tare weight
		float SCALE = 1;	// used to return weight in grams, kg, ounces, whatever

	public:

		HX711();

		virtual ~HX711();

		// Initialize library with data output pin, clock input pin and gain factor.
		// Channel selection is made by passing the appropriate gain:
		// - With a gain factor of 64 or 128, channel A is selected
		// - With a gain factor of 32, channel B is selected
		// The library default is "128" (Channel A).
		void begin(byte dout, byte pd_sck, byte gain = 128);

		// Check if HX711 is ready
		// from the datasheet: When output data is not ready for retrieval, digital output pin DOUT is high. Serial clock
		// input PD_SCK should be low. When DOUT goes to low, it indicates data is ready for retrieval.
		bool is_ready();

		// Wait for the HX711 to become ready
		void wait_ready(unsigned long delay_ms = 0);
		bool wait_ready_retry(int retries = 3, unsigned long delay_ms = 0);
		bool wait_ready_timeout(unsigned long timeout = 1000, unsigned long delay_ms = 0);

		// set the gain factor; takes effect only after a call to read()
		// channel A can be set for a 128 or 64 gain; channel B has a fixed 32 gain
		// depending on the parameter, the channel is also set to either A or B
		void set_gain(byte gain = 128);

		// reads if the chip is ready and calculates the new average
		bool update_if_ready();

		// waits for the chip to be ready and returns a reading
		long read();

		// returns the calculated average, that is the current value without the tare weight
		double get_value();

		// returns get_value() divided by SCALE, that is the averaged value divided by a value obtained via calibration
		float get_units();

		// set the OFFSET value for tare weight
		void tare();

		// set the SCALE value; this value is used to convert the raw data to "human readable" data (measure units)
		void set_scale(float scale = 1.f);

		// get the current SCALE
		float get_scale();

		// set OFFSET, the value that's subtracted from the actual reading (tare weight)
		void set_offset(float offset = 0);

		// get the current OFFSET
		float get_offset();

		// set OFFSET, the value that's subtracted from the actual reading (tare weight)
		void set_smoothing_factor(float factor = 1);

		// get the current OFFSET
		long get_smoothing_factor();

		// puts the chip into power down mode
		void power_down();

		// wakes up the chip after power down mode
		void power_up();
};

#endif /* HX711_h */
