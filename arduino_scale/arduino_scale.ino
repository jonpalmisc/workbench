#include <LiquidCrystal.h>
#include <Q2HX711.h>

#define LED_PIN      8   /* Pin for indicator LED */
#define TICK_RATE_MS 100 /* System polling rate */

// Uptime, measured in number of ticks.
unsigned long g_tickCount = 0;

//===-- LCD Display -------------------------------------------------------===//

#define LCD_RS_PIN 12 /* RS pin */
#define LCD_EN_PIN 11 /* EN pin */
#define LCD_D4_PIN 5  /* D4 pin */
#define LCD_D5_PIN 4  /* D5 pin */
#define LCD_D6_PIN 3  /* D6 pin */
#define LCD_D7_PIN 2  /* D7 pin */

// LCD display handle.
LiquidCrystal g_lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

//===-- Force Sensor ------------------------------------------------------===//

#define HX711_DATA_PIN  6 /* HX711 sensor data pin */
#define HX711_CLOCK_PIN 7 /* HX711 sensor clock pin */

// HX711 force sensor handle.
Q2HX711 g_hx711(HX711_DATA_PIN, HX711_CLOCK_PIN);

/// Read the raw weight from the force sensor, uncalibrated.
inline long rawWeight()
{
	return g_hx711.read();
}

//===-- Calibration -------------------------------------------------------===//

#define CALIBRATION_SAMPLES 20    /* Number of calibration samples to use */
#define UNITS_TO_GRAMS      435.0 /* Scaling factor to convert to grams */

// Samples of sensor readings to use for calibration.
long g_calibrationData[CALIBRATION_SAMPLES] = {};

// Offset to subtract from sensor readings.
long g_calibrationOffset = 0;

/// Get the current weight reading, calibrated, measured in grams.
double calibratedWeight()
{
	auto weight = (double)(rawWeight() - g_calibrationOffset);
	weight /= UNITS_TO_GRAMS;

	return weight > 0 ? weight : 0;
}

/// Record a weight sample for calibration.
void recordCalibrationSample()
{
	g_lcd.setCursor(0, 0);
	g_lcd.print(g_tickCount);

	// Flash LED during calibration.
	digitalWrite(LED_PIN, g_tickCount % 2 == 0 ? HIGH : LOW);

	g_calibrationData[g_tickCount++] = rawWeight();
}

/// Complete the calibration process and update the global tare amount.
void finishCalibration()
{
	long long total = 0;

	for (unsigned i = 0; i < CALIBRATION_SAMPLES; ++i)
		total += g_calibrationData[i];

	g_calibrationOffset = total / CALIBRATION_SAMPLES;
	g_tickCount++;
}

//===-- Dog Math ----------------------------------------------------------===//

#define DAILY_SERVING_COUNT 3.0   /* Number of servings per day */
#define CALORIES_PER_CUP    400.0 /* Average calories/cup of dog food */
#define CUPS_TO_GRAMS       224.0 /* Scale factor for cups to grams */
#define TOLERANCE           0.07  /* Serving over/under tolerance */
#define MIN_WEIGHT_KG       3.0   /* Minimum allowed dog weight */
#define MAX_WEIGHT_KG       45.0  /* Maximum supported dog weight */
#define WEIGHT_RANGE_KG     MAX_WEIGHT_KG - MIN_WEIGHT_KG

/// Calculate the needed daily calories given a dog's weight.
inline float dailyCalorieNeed(float weight)
{
	return 110.0 * pow(weight, 0.75);
}

/// Calculate number of grams of food needed daily given a dog's weight.
inline float dailyServingNeed(float weight)
{
	auto calorieNeed = dailyCalorieNeed(weight);
	return (calorieNeed / CALORIES_PER_CUP) * CUPS_TO_GRAMS;
}

/// Calculate the size of a single food serving in grams given a dog's weight.
inline float singleServingSize(float weight)
{
	return dailyServingNeed(weight) / DAILY_SERVING_COUNT;
}

//===-- Main Program ------------------------------------------------------===//

// Work buffer for formatting LCD lines; has some extra room.
char g_lineBuffer[24] = { 0 };

/// Update external actuators.
inline void update()
{
	float normWeight = (float)analogRead(A0) / 1023.0;
	float dogWeightKg = normWeight * WEIGHT_RANGE_KG + MIN_WEIGHT_KG;

	float currentWeight = calibratedWeight();
	float servingAmount = singleServingSize(dogWeightKg);

	// Embedded programming has its quirks...
	sprintf(g_lineBuffer, "%-16s", " ");
	dtostrf(dogWeightKg, 5, 2, &g_lineBuffer[0]);
	g_lineBuffer[5] = 'k';
	g_lineBuffer[6] = 'g';
	dtostrf(servingAmount, 6, 2, &g_lineBuffer[9]);
	g_lineBuffer[15] = 'g';

	g_lcd.clear();
	g_lcd.setCursor(0, 0);
	g_lcd.print(g_lineBuffer);

	// Clear the line buffer with spaces.
	sprintf(g_lineBuffer, "%-16s", " ");

	// Indicate status via LCD & LED displays
	if ((currentWeight > servingAmount * (1 + TOLERANCE))
	    || (currentWeight < servingAmount * (1 - TOLERANCE)))
		digitalWrite(LED_PIN, LOW);
	else
		digitalWrite(LED_PIN, HIGH);

	dtostrf(currentWeight, 6, 2, &g_lineBuffer[9]);
	g_lineBuffer[15] = 'g';

	g_lcd.setCursor(0, 1);
	g_lcd.print(g_lineBuffer);
}

void setup()
{
	g_lcd.begin(16, 2);
	pinMode(LED_PIN, OUTPUT);
}

void loop()
{
	if (g_tickCount < CALIBRATION_SAMPLES)
		recordCalibrationSample();
	else if (g_tickCount == CALIBRATION_SAMPLES)
		finishCalibration();
	else
		update();

	delay(TICK_RATE_MS);
}
