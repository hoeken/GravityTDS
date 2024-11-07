/***************************************************
 DFRobot Gravity: Analog TDS Sensor/Meter
 <https://www.dfrobot.com/wiki/index.php/Gravity:_Analog_TDS_Sensor_/_Meter_For_Arduino_SKU:_SEN0244>

 ***************************************************
 This sample code shows how to read the tds value and calibrate it with the
 standard buffer solution. 707ppm(1413us/cm)@25^c standard buffer solution is
 recommended.

 Created 2018-1-3
 By Jason <jason.ling@dfrobot.com@dfrobot.com>

 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution.
 ****************************************************/

#include "GravityTDS.h"

GravityTDS::GravityTDS() {
  this->temperature = 25.0;
  this->aref = 5.0;
  this->adcRange = 1024.0;
  this->kValue = 1.0;
}

GravityTDS::~GravityTDS() {}

void GravityTDS::setTemperature(float temp) { this->temperature = temp; }

void GravityTDS::setAref(float value) { this->aref = value; }

void GravityTDS::setAdcRange(float range) { this->adcRange = range; }

void GravityTDS::begin() {}

float GravityTDS::getKvalue() { return this->kValue; }

void GravityTDS::update(int reading) {
  this->analogValue = reading;
  this->voltage = this->analogValue / this->adcRange * this->aref;
  this->ecValue =
      (133.42 * this->voltage * this->voltage * this->voltage -
       255.86 * this->voltage * this->voltage + 857.39 * this->voltage) *
      this->kValue;
  this->ecValue25 =
      this->ecValue /
      (1.0 + 0.02 * (this->temperature - 25.0)); // temperature compensation
  this->tdsValue = ecValue25 * TdsFactor;
}

float GravityTDS::getTdsValue() { return tdsValue; }

float GravityTDS::getEcValue() { return ecValue25; }

void GravityTDS::setKvalue(float kvalue) { this->kValue = kvalue; }

// void GravityTDS::ecCalibration(byte mode) {
//   char *cmdReceivedBufferPtr;
//   static boolean ecCalibrationFinish = 0;
//   static boolean enterCalibrationFlag = 0;
//   float KValueTemp, rawECsolution;
//   switch (mode) {
//   case 0:
//     if (enterCalibrationFlag)
//       Serial.println(F("Command Error"));
//     break;

//   case 1:
//     enterCalibrationFlag = 1;
//     ecCalibrationFinish = 0;
//     Serial.println();
//     Serial.println(F(">>>Enter Calibration Mode<<<"));
//     Serial.println(
//         F(">>>Please put the probe into the standard buffer solution<<<"));
//     Serial.println();
//     break;

//   case 2:
//     // cmdReceivedBufferPtr = strstr(cmdReceivedBuffer, "CAL:");
//     // cmdReceivedBufferPtr += strlen("CAL:");
//     rawECsolution = strtod(cmdReceivedBufferPtr, NULL) / (float)(TdsFactor);
//     rawECsolution = rawECsolution * (1.0 + 0.02 * (temperature - 25.0));
//     if (enterCalibrationFlag) {
//       // Serial.print("rawECsolution:");
//       // Serial.print(rawECsolution);
//       // Serial.print("  ecvalue:");
//       // Serial.println(ecValue);

//       KValueTemp =
//           rawECsolution /
//           (133.42 * voltage * voltage * voltage - 255.86 * voltage * voltage
//           +
//            857.39 * voltage); // calibrate in the  buffer solution, such as
//                               // 707ppm(1413us/cm)@25^c
//       if ((rawECsolution > 0) && (rawECsolution < 2000) &&
//           (KValueTemp > 0.25) && (KValueTemp < 4.0)) {
//         Serial.println();
//         Serial.print(F(">>>Confrim Successful,K:"));
//         Serial.print(KValueTemp);
//         Serial.println(F(", Send EXIT to Save and Exit<<<"));
//         kValue = KValueTemp;
//         ecCalibrationFinish = 1;
//       } else {
//         Serial.println();
//         Serial.println(F(">>>Confirm Failed,Try Again<<<"));
//         Serial.println();
//         ecCalibrationFinish = 0;
//       }
//     }
//     break;

//   case 3:
//     if (enterCalibrationFlag) {
//       Serial.println();
//       if (ecCalibrationFinish) {
//         EEPROM_write(kValueAddress, kValue);
//         Serial.print(F(">>>Calibration Successful,K Value Saved"));
//       } else
//         Serial.print(F(">>>Calibration Failed"));
//       Serial.println(F(",Exit Calibration Mode<<<"));
//       Serial.println();
//       ecCalibrationFinish = 0;
//       enterCalibrationFlag = 0;
//     }
//     break;
//   }
// }