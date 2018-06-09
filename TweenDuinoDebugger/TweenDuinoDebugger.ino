#include "TweenDuino.h"

TweenDuino::Timeline timeline;
float val = 0;

void setup(){
	Serial.begin(115200);
}

void loop(){
	delete timeline;
	timeline = new TweenDuino::Timeline();
	for(int i =0; i<10; ++i){
		timeline.addTo(val, 1.0, 1000);
	}
	long free = ESP.getFreeHeap();
	Serial.printf("Free memory: %i\n", free);
	delay(1000);
}


