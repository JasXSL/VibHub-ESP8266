#include "EasingLibrary.h"

#include "TweenDuino.h"

TweenDuino::Timeline timeline;
float val = 0;

long lastfree;

int loops;

void fill_timeline(){
	// Serial.println("fill_timeline");
	for(int i =0; i<20; ++i){
		timeline.addTo(val, 1.0*i*i, 5+(5*i));
	}
}


void setup(){
	lastfree = ESP.getFreeHeap();
	
	Serial.begin(115200);
	fill_timeline();
	
	// Serial.println("timeline.begin");
	timeline.begin(millis());
	
	// Serial.println("setup done");
}

void loop(){
	uint32_t time = millis();
	timeline.update(time);
	
	Serial.printf("Val: %f\n", val);
	
	if (timeline.isComplete() & loops < 20) {
		loops++;
		Serial.printf("Loops: %i\n", loops);
		// if (loops % 1000 == 0) {
			// Serial.printf("Loops: %i\n", loops);
		// }
		timeline.cleanAndReset();
		fill_timeline();
		timeline.begin(millis());
	}
	
	long free = ESP.getFreeHeap();
	if (free != lastfree) {
		Serial.printf("Loop: %i  Free memory: %i  Change: %i\n", loops, free, (free-lastfree));
		lastfree = free;
	}
	
	delay(5);
}


