#include "EasingLibrary.h"

#include "TweenDuino.h"

TweenDuino::Timeline* timeline = nullptr;
float val = 0;

long lastfree;

int loops;

void restart_timeline(){
	Serial.printf("restart_timeline  loop: %u\n", loops);
	
	if (timeline) {
		delete timeline;
		timeline = nullptr;
	}
	
	Serial.println("new TweenDuino::Timeline()");
	timeline = new TweenDuino::Timeline();
	
	for(int i =0; i<20; ++i){
		timeline->addTo(val, 1.0*i*i, 10+(5*i));
		timeline->addTo(val, 1.0*i*i, 10+(5*i), TweenDuino::Tween::LINEAR, TweenDuino::Tween::INOUT);
	}
	// timeline->addTo(val, 50, 1000);
	// timeline->addTo(val, 255, 1000);
	
	timeline->begin(millis());
}


void setup(){
	lastfree = ESP.getFreeHeap();
	
	Serial.begin(115200);
	
	// Serial.println("new TweenDuino::Timeline()");
	// timeline = new TweenDuino::Timeline();
	
	restart_timeline();
}

void loop(){
	uint32_t time = millis();
	timeline->update(time);
	
	Serial.printf("Val: %f\n", val);
	
	if (timeline->isComplete()) {
		Serial.printf("Finished Loop: %i\n", loops);
		loops++;
		// if (loops % 1000 == 0) {
			// Serial.printf("Loops: %i\n", loops);
		// }
		// timeline->wipe();
		restart_timeline();
	}
	
	long free = ESP.getFreeHeap();
	if (free != lastfree) {
		Serial.printf("==== Loop: %i  Free memory: %i  Change: %i ====\n", loops, free, (free-lastfree));
		lastfree = free;
	}
	
	delay(2);
}


