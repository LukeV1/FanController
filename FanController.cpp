#include "FanController.h"

FanController::FanController(byte sensorPin, unsigned int sensorThreshold, byte pwmPin)
{
	_sensorPin = sensorPin;
	_sensorThreshold = sensorThreshold;
	_pwmPin = pwmPin;
	pinMode(pwmPin, OUTPUT);
	_pwmDutyCycle = 100;
}

void FanController::begin()
{
	static byte instance;
	_instance = instance;
	_instances[instance] = this;
	pinMode(_sensorPin, INPUT_PULLUP);
	setDutyCycle(_pwmDutyCycle);
	_attachInterrupt();
	instance++;
}

unsigned int FanController::getSpeed() {
	unsigned int elapsed = millis() - _lastMillis;
	if (elapsed > _sensorThreshold)
	{
		disableInterrupt(_sensorPin);
		double correctionFactor = 1000.0 / elapsed;
		_lastReading = correctionFactor * _halfRevs / 2 * 60;
		_halfRevs = 0;
		_lastMillis = millis();
		_attachInterrupt();
		return _lastReading;
	}
}

void FanController::setDutyCycle(byte dutyCycle) {
	_pwmDutyCycle = min(dutyCycle, 100);
	analogWrite(_pwmPin, 2.55 * _pwmDutyCycle);
}

byte FanController::getDutyCycle() {
	return _pwmDutyCycle;
}

void FanController::_attachInterrupt()
{
	switch (_instance)
	{
	case 0:
		enableInterrupt(_sensorPin, _triggerExt0, FALLING);
		break;
	case 1:
		enableInterrupt(_sensorPin, _triggerExt1, FALLING);
		break;
	case 2:
		enableInterrupt(_sensorPin, _triggerExt2, FALLING);
		break;
	case 3:
		enableInterrupt(_sensorPin, _triggerExt3, FALLING);
		break;
	case 4:
		enableInterrupt(_sensorPin, _triggerExt4, FALLING);
		break;
	case 5:
		enableInterrupt(_sensorPin, _triggerExt5, FALLING);
		break;
	}
}

FanController * FanController::_instances[6];

void FanController::_trigger()
{
	_halfRevs++;
}

void FanController::_triggerCaller(byte instance)
{
	if (FanController::_instances[instance] != nullptr)
	{
		FanController::_instances[instance]->_trigger();
	}
}

void FanController::_triggerExt0() { FanController::_triggerCaller(0); }
void FanController::_triggerExt1() { FanController::_triggerCaller(1); }
void FanController::_triggerExt2() { FanController::_triggerCaller(2); }
void FanController::_triggerExt3() { FanController::_triggerCaller(3); }
void FanController::_triggerExt4() { FanController::_triggerCaller(4); }
void FanController::_triggerExt5() { FanController::_triggerCaller(5); }