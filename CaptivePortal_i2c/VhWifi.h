/*
	VibHub wifi
*/
#ifndef VhWifi_h
#define VhWifi_h



//TODO: Currently the ESP32 branch of WifiManager doesn't work correctly on the ESP8266
// #if defined(ESP8266)
// #include "WiFiManager8266.h"
// #elif defined(ESP32)
#include "Configuration.h"
#include "WiFiManager.h"
// #endif

// Offload the constant CSS and JS to be injected into each page into program memory
// CSS Style shared on each page
const char CSS_SHARED[] PROGMEM = "<style>"
	"body{ "
		"margin:0;"
		"font-size:3vmax;"
		"line-height:125%;"
		"text-align:center;"
		"font-family:verdana, arial;"
		"background-image:url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAoAAAAKCAIAAAACUFjqAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAADFJREFUeNpifPTo0du3bxlgQFhYGJnLhEcOyGXCIwfkMuGRAxmORw4kjUdugJ0GEGAATr8+gQ2rX7QAAAAASUVORK5CYII=')"
	"}"
	"body > div{"
		"padding:2vmax;"
		"box-shadow:2px 2px 15px rgba(0,0,0,.5);"
		"background:rgba(255,255,255,.75);"
		"border: 2px solid #9AFAFF;"
		"border-radius:1vmax;"
		"margin-top:1vmax;"
	"}"
	"h1, h3{"
    	"font-size:5vmax;"
        "margin:0 0 1vmax 0;"
    "}"
	"p{"
		"font-size:3vmax;"
		"margin: 0 0 1vmax 0;"
	"}"
	"p.subtitle{"
		"margin:-1vmax 0 1vmax 0;"
		"font-style: italic;"
		"color:#666;"
	"}"
    "h3{"
        "font-size:4vmax;"
    "}"
	"button{"
        "box-shadow:0 0 3vmax rgba(0,0,0,.5);"
        "border:0;"
        "border-radius:1vmax;"
        "background:linear-gradient(to bottom, #edfeff 0%,#d0fdff 100%);"
        "padding:2vmax;"
        "font-size:3vmax;"
        "width:100%;"
        "margin:1vmax 0 0 0;" 
        "color:#000;"
        "border:0.5vmax solid #FFF;"
		"cursor:pointer;"
		"border:0.5vmax solid #DDD;"
        "transition: all 0.25s ease-out;"
		"outline:none;"
    "}"
	"button:hover{"
    	"transition:none;"
        "border-color: #FFF;"
    "}"
    "button:active{"
     	"transform:scale(0.98,0.98);"
        "box-shadow:0 0 1vmax rgba(0,0,0,.75);"
    "}"
	"button.setup{"
        "background:linear-gradient(to bottom, #eeffed 0%,#d3ffd1 100%);"
        "font-size:4vmax;"
        "font-weight:bold;"
    "}"
"</style>";

const char JS_SHARED[] PROGMEM = ""
    "const path = document.location.pathname;"

	// HOME
	"if(path === '/'){"
		// Remove unneeded rowbreaks
		"document.querySelectorAll('br').forEach(el => {"
			"el.remove();"
		"});"
		// Remove the unnecessary H3
		"document.querySelector('h3').remove();"
		// Header can be the same
		"document.querySelector('h1').innerText = 'VibHub';"
		"document.querySelector('button').className = 'setup';"
		// Add version
		"let p = document.createElement('p');"
		"let h1 = document.querySelector('h1');"
		"p.innerHTML = 'v<span class=\"VH_VERSION\"></span>';"
		"p.className = 'subtitle';"
		"h1.parentNode.insertBefore(p, h1.nextSibling);"
	"}"
;


class VhWifi{
	public:
		VhWifi(void) {}
        
		void connect( bool force=false, bool resetSetting=false );
		void clearSettings();

	private:
        WiFiManager* _wifiManager;

		// Any non-constant data needed to be loaded should go in here
		String getCustomJS(){
			String out = "";
			// Anything with class VH_VERSION gets innerText set to the version
			out+= "document.querySelectorAll('.VH_VERSION').forEach(el => {";
				out+="el.innerText='";
				out+= VH_VERSION;
				out+= "';";
			out+= "});";

			return out;
		}
};



extern VhWifi vhWifi;

#endif //VhWifi_h