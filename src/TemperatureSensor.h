#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <Arduino.h>

/**
 * Reads temperature from an NTC thermistor using the Beta (β) parameter equation.
 *
 * CIRCUIT CONFIGURATION:
 * The thermistor is configured in a voltage divider:
 *   Vcc (5V) --- [Fixed Resistor] --- [Analog Pin] --- [Thermistor] --- GND
 *
 * HOW IT WORKS:
 * 1. The Arduino ADC reads the voltage at the junction between the fixed resistor and thermistor
 * 2. From this voltage, we calculate the thermistor's resistance
 * 3. Using the Beta equation, we convert resistance to temperature
 *
 * BETA EQUATION:
 * The Steinhart-Hart equation can be simplified to the Beta parameter equation:
 *   T = 1 / (1/T0 + (1/β) × ln(R/R0))
 * Where:
 *   T  = Temperature in Kelvin
 *   T0 = Reference temperature (usually 25°C = 298.15K)
 *   R  = Current thermistor resistance (measured)
 *   R0 = Thermistor resistance at T0 (from datasheet)
 *   β  = Beta value (material constant from datasheet)
 */
class TemperatureSensor
{
private:
    int pin;      // Arduino analog pin (A0, A1, etc.)
    float rFixed; // Fixed resistor value in ohms
    float r0;     // Thermistor resistance at reference temperature (usually 25°C)
    float t0;     // Reference temperature in Kelvin
    float beta;   // Beta coefficient (β) - thermistor material constant
    int adcMax;   // Maximum ADC value (1023 for 10-bit Arduino ADC)

    /**
     * Calculate the thermistor's current resistance from ADC reading
     * Uses voltage divider equation: R_thermistor = R_fixed × (ADC_max / ADC_reading - 1)
     */
    float readResistance();

public:
    /**
     * Constructor
     *
     * @param analogPin      Arduino analog pin connected to the voltage divider junction
     * @param fixedResistor  Fixed resistor value in ohms (DEFAULT: 12000Ω = 12kΩ)
     *                       This value comes from YOUR circuit - measure it with a multimeter
     *                       for best accuracy, or use the resistor's rated value
     *
     * @param r0Resistance   Thermistor resistance at 25°C in ohms (DEFAULT: 12600Ω = 12.6kΩ)
     *                       This comes from your THERMISTOR DATASHEET or can be measured:
     *                       - Remove thermistor from circuit
     *                       - Let it stabilize at exactly 25°C (77°F) room temperature
     *                       - Measure resistance with multimeter
     *                       ~12.6kΩ at room temp, so this is our measured value
     *
     * @param t0Celsius      Reference temperature in Celsius (DEFAULT: 25.0°C)
     *                       This is the standard reference point for thermistor datasheets
     *                       Almost always 25°C unless your datasheet specifies otherwise
     *
     * @param betaValue      Beta coefficient (β) (DEFAULT: 3950K)
     *                       This is a MATERIAL CONSTANT from your thermistor datasheet
     *                       Common values: 3435, 3950, 4050, 4250 (Kelvin)
     *                       3950K is a common value for NTC thermistors
     *                       For better accuracy, check your thermistor's datasheet
     *                       The datasheet might list it as "B25/85" meaning beta from 25°C to 85°C
     *
     * CALIBRATION TIPS:
     * If your readings are off:
     * 1. Verify fixedResistor with a multimeter
     * 2. Measure r0Resistance at exactly 25°C
     * 3. Look up the exact beta value in your thermistor's datasheet
     * 4. If no datasheet, calculate beta using two known temperature points:
     *    β = ln(R1/R2) / (1/T1 - 1/T2)  where T is in Kelvin
     */
    TemperatureSensor(
        int analogPin,
        float fixedResistor = 12000.0, // 12kΩ - YOUR measured fixed resistor
        float r0Resistance = 12600.0,  // 12.6kΩ - YOUR measured thermistor at 25°C
        float t0Celsius = 25.0,        // 25°C - standard reference temperature
        float betaValue = 3950.0);     // 3950K - estimated beta (verify with datasheet)

    /**
     * Initialize the sensor
     * Sets up the analog pin and reference voltage
     * Call this in setup()
     */
    void begin();

    /**
     * Get temperature in Celsius
     * @return Temperature in °C, or NAN if reading is invalid
     */
    float getTempC();

    /**
     * Get temperature in Fahrenheit
     * @return Temperature in °F, or NAN if reading is invalid
     */
    float getTempF();

    /**
     * Get temperature in Kelvin
     * @return Temperature in K, or NAN if reading is invalid
     */
    float getTempK();

    /**
     * Check if the current ADC reading is valid
     * @return true if ADC is between 1 and 1022 (not at extremes)
     */
    bool isValid();

    /**
     * Get raw ADC value for debugging
     * @return ADC reading (0-1023 for 10-bit ADC)
     */
    int getRawADC();
};

#endif

/**
 * EXAMPLE USAGE:
 *
 * #include "TemperatureSensor.h"
 *
 * // Using default values (12kΩ fixed, 12.6kΩ thermistor, β=3950)
 * TemperatureSensor tempSensor(A0);
 *
 * // OR specify all parameters for precise calibration:
 * TemperatureSensor tempSensor(
 *   A0,      // Analog pin
 *   12000.0, // 12kΩ fixed resistor (measured with multimeter)
 *   12600.0, // 12.6kΩ thermistor at 25°C (measured)
 *   25.0,    // Reference temp (25°C standard)
 *   3950.0   // Beta value (from datasheet)
 * );
 *
 * void setup() {
 *   Serial.begin(9600);
 *   tempSensor.begin();
 * }
 *
 * void loop() {
 *   if (tempSensor.isValid()) {
 *     float tempC = tempSensor.getTempC();
 *     float tempF = tempSensor.getTempF();
 *
 *     Serial.print("Temperature: ");
 *     Serial.print(tempC, 2);
 *     Serial.print(" °C / ");
 *     Serial.print(tempF, 2);
 *     Serial.println(" °F");
 *   } else {
 *     Serial.println("Sensor reading out of range");
 *     Serial.print("Raw ADC: ");
 *     Serial.println(tempSensor.getRawADC());
 *   }
 *
 *   delay(1000);
 * }
 *
 * TROUBLESHOOTING:
 * - Reading stuck at 0 or 1023: Check wiring, thermistor might be disconnected
 * - Temperature way off: Verify beta value in thermistor datasheet
 * - Readings unstable: Add a 0.1µF capacitor across thermistor terminals
 * - Need better accuracy: Use 4-wire measurement or consider an RTD like PT1000
 */

// /** Reads temperature from an NTC (Negative Temperature Coefficient) thermistor 
//  * using the Beta (β) parameter equation.
//  * 
//  * WHAT IS AN NTC THERMISTOR?
//  * An NTC thermistor is a temperature-sensitive resistor whose resistance DECREASES
//  * as temperature INCREASES. This is the opposite of most materials, and makes NTC
//  * thermistors ideal for temperature sensing applications.
//  * 
//  * Key characteristics:
//  * - Resistance drops predictably as temperature rises
//  * - "Negative" = resistance goes down when temp goes up
//  * - Most common type of thermistor for temperature measurement (90%+ of market)
//  * - Used in: HVAC systems, appliances, 3D printers, automotive, medical devices
//  * 
//  * HOW TO VERIFY YOU HAVE AN NTC THERMISTOR:
//  * 1. Measure resistance at room temperature with a multimeter
//  * 2. Gently pinch the thermistor between your fingers for 30-60 seconds to warm it
//  * 3. Measure resistance again while holding it
//  * 4. If resistance DECREASED, it's NTC (correct for this class)
//  *    If resistance INCREASED, it's PTC (this class won't work - PTC is rare in temp sensing)
//  * 
//  * Example: 12.6kΩ at 25°C → 10kΩ at 30°C (typical NTC behavior)
//  * 
//  * CIRCUIT CONFIGURATION:
//  * The thermistor is configured in a voltage divider:
//  *   Vcc (5V) --- [Fixed Resistor] --- [Analog Pin] --- [Thermistor] --- GND
//  * 
//  * As the thermistor heats up and its resistance drops, more voltage appears 
//  * across the fixed resistor, and the Arduino reads a higher ADC value.
//  * /