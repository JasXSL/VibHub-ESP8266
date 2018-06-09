
/*
	RandObject for VibHub motor programs
	https://github.com/JasXSL/VibHub-Client/wiki/REST-Program-API
*/
#ifndef VhProgramStage_h
#define VhProgramStage_h
#include <ArduinoJson.h>
#include "RandObject.h"
// Motor class extending PWM class
class VhProgramStage{

	public:
		VhProgramStage(JsonObject &data){
			intensity = RandObject();
			duration = RandObject();
			ease = TweenDuino::Tween::LINEAR;
			easeType = TweenDuino::Tween::IN;
			yoyo = false;

			

		};

		RandObject intensity;
		RandObject duration;
		TweenDuino::Tween::Ease ease = TweenDuino::Tween::LINEAR;
		TweenDuino::Tween::EaseType easeType = TweenDuino::Tween::IN;
		RandObject repeats;
		bool yoyo = false;

};

#endif

