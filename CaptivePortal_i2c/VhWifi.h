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
		"width:50%;"
		"min-width:300px;"
		"display:inline-block;"
		"margin-bottom:5vmax;"
	"}"
	"div, span{ position:relative; }"
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
	"input{"
		"border-radius:0.5vmax;"
		"padding:0.5vmax;"
	"}"
	"button, div.wifiNode{"
        "box-shadow:0 0 3vmax rgba(0,0,0,.5);"
        "border:0;"
        "border-radius:1vmax;"
        "background:linear-gradient(to bottom, #edfeff 0%,#d0fdff 100%);"
        "padding:2vmax;"
        "font-size:2vmax;"
        "width:100%;"
        "margin:1vmax 0 0 0;" 
        "color:#000;"
        "border:0.5vmax solid #FFF;"
		"cursor:pointer;"
		"border:0.5vmax solid #DDD;"
        "transition: transform 0.25s ease-out, box-shadow 0.25s ease-out;"
		"outline:none;"
		"width:90%;"
    "}"
	"button:hover, div.wifiNode:hover{"
    	"transition:none;"
        "border-color: #FFF;"
    "}"
    "button:active, div.wifiNode:active{"
     	"transform:scale(0.98,0.98);"
        "box-shadow:0 0 1vmax rgba(0,0,0,.75);"
    "}"
	"button.setup{"
        "background:linear-gradient(to bottom, #eeffed 0%,#d3ffd1 100%);"
        "font-size:4vmax;"
        "font-weight:bold;"
		"width:100%;"
    "}"
	".q{"
		"width:auto;"
		"padding-left:3vmax;"
	"}"
	"div.wifiNode{"
		"text-align: left;"
		"background: linear-gradient(to bottom, #f6f6f6 0%,#e8e8e8 100%);"
		"padding:0.5vmax;"
		"width:auto;"
	"}"
	"div.wifiNode a{"
		"text-decoration:none;"
		"color:#000;"
	"}"
	"div.wifiNode span{"
		"background:none;"
	"}"
	"div.wifiNode svg{"
		"height:2.5vmax;" 
		"width:2.5vmax;" 
		"position:absolute;" 
		"left:0;" 
		"top:0.5vmax;"
	"}"
	"label.title{"
		"font-size:1.5vmax;"
		"text-align:left;"
		"display:block;"
		"color:#666;"
		"font-style:italic;"
	"}"
	"a.reset{font-style:normal;color:#000;}"
	"dl{text-align:left; line-height:100%;}"
	"dt{font-size:2vmax; font-style:italic; color:#666;}"
	"dd{margin-bottom:1vmax;}"

"</style>";

const char JS_SHARED[] PROGMEM = "\n"
    "const path = document.location.pathname;\n"
	// Clear default style
	"document.querySelector('body > div').style = '';\n"
	// HOME
	"if(path === '/'){\n"
		// Remove unneeded rowbreaks
		"document.querySelectorAll('br').forEach(el => {\n"
			"el.remove();\n"
		"});\n"
		// Remove the unnecessary H3
		"document.querySelector('h3').remove();\n"
		// Header can be the same
		"document.querySelector('h1').innerText = 'VibHub';\n"
		"document.querySelector('button').className = 'setup';\n"
		// Add version
		"let p = document.createElement('p');\n"
		"let h1 = document.querySelector('h1');\n"
		"p.innerHTML = 'v<span class=\"VH_VERSION\"></span>';\n"
		"p.className = 'subtitle';\n"
		"h1.parentNode.insertBefore(p, h1.nextSibling);\n"
		"document.querySelector(\"form:nth-of-type(1) button\").innerText = \"Configure\";\n"
		"document.querySelector(\"form:nth-of-type(2) button\").innerText = \"Configure (No Scan)\";\n"
	"}\n"

	// Wifi page
	"if(path === '/wifi' || path === '/0wifi'){\n"
		// Adds wifiNode class to all wifiNodes
		"Array.from(document.querySelectorAll('body > div > *')).some(el => {\n"
			"if(el.nodeName !== 'DIV')return true;\n"
			"el.className = 'wifiNode';\n"
		"});\n"
		// Replace the default padlocks that have white backgrounds with transparent ones
		"document.querySelectorAll('body > div > div > span.q.l').forEach(el => {\n"
			"el.innerHTML = '<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 512 512\"><g class=\"\" transform=\"translate(0,0)\" style=\"touch-action: none;\"><path d=\"M254.28 17.313c-81.048 0-146.624 65.484-146.624 146.406V236h49.594v-69.094c0-53.658 43.47-97.187 97.03-97.187 53.563 0 97.032 44.744 97.032 97.186V236h49.594v-72.28c0-78.856-65.717-146.407-146.625-146.407zM85.157 254.688c-14.61 22.827-22.844 49.148-22.844 76.78 0 88.358 84.97 161.5 191.97 161.5 106.998 0 191.968-73.142 191.968-161.5 0-27.635-8.26-53.95-22.875-76.78H85.155zM254 278.625c22.34 0 40.875 17.94 40.875 40.28 0 16.756-10.6 31.23-25.125 37.376l32.72 98.126h-96.376l32.125-98.125c-14.526-6.145-24.532-20.62-24.532-37.374 0-22.338 17.972-40.28 40.312-40.28z\" fill=\"#000000\" fill-opacity=\"1\"></path></g></svg>'+el.innerHTML;\n"
		"});\n"
		// Add titles above each input
		"document.querySelectorAll('input').forEach(el => {\n"
			"let tmp = document.createElement('label');\n"
			"tmp.className = 'title';\n"
			"let conversions = {\n"
				"SSID : 'WiFi SSID',\n"
				"password : 'WiFi Password',\n"
				"'Server Host' : 'Server Host <a class=\"reset\" id=\"resetHost\">[Default]</a>',\n"
				"'Server Port' : 'Server Port <a class=\"reset\" id=\"resetPort\">[Default]</a>',\n"
			"};\n"
			"if(conversions.hasOwnProperty(el.placeholder)){\n"
				"tmp.innerHTML = conversions[el.placeholder];\n"
			"}\n"
			"else{\n"
				"tmp.innerText = el.placeholder;\n"
			"}\n"
			"el.parentNode.insertBefore(tmp, el);\n"
		"});\n"
		// Make the port a number
		"document.getElementById('port').type = 'number';\n"
		"document.getElementById('port').min = 0;\n"
		"document.getElementById('port').max = 65535;\n"
		
		// Update the save button
		"document.querySelector('button[type=submit]').className='setup';\n"
		"document.querySelector(\"button[type=submit]\").innerText=\"Save\"\n"
		// Replace the re-scan button with a back button, since re-scan will wipe any changes made to settings
		"document.querySelector(\"div.c\").remove();\n"
		"let nodes = document.querySelectorAll('body > div > br');\n"
		"nodes[nodes.length-1].remove();\n"
		"document.querySelector('body > div').innerHTML += '<button onClick=\"window.location=\\\'/\\\';\">Cancel</button>';\n"

		// Event bindings
		// Reset buttons
		"document.getElementById('resetHost').onclick = function(){ this.parentNode.nextSibling.value='vibhub.io'; }\n"
		"document.getElementById('resetPort').onclick = function(){ this.parentNode.nextSibling.value='80'; }\n"
		"document.querySelectorAll('div.wifiNode').forEach(el => {\n"
			"el.onclick = function(){ c(this.querySelector('a')); window.location.href = '#p'; };\n"
		"});\n"

		
	"}\n"

	// Credentials saved page
	"if(path === '/wifisave'){\n"
		"document.querySelector('body > div').innerHTML = '<h1>Settings Saved</h1><p>Connecting to network. If it fails, connect to the VibHub device again and update the settings.</p>';\n"
	"}\n"

	// info page
	"if(path === '/i'){\n"
		"document.querySelector('body > div').innerHTML += '<button onClick=\"window.location=\\\'/\\\';\">Back</button>';\n"
		"let title = document.createElement('h1');"
		"title.innerText = 'Device Info';"
		"document.querySelector('body > div').insertBefore(title, document.querySelector('body > div > *'));"
		"let l = document.querySelector('body > div > dl');"
		"l.innerHTML = '<dt>VibHub Version</dt><dd class=\"VH_VERSION\"></dd>'+l.innerHTML;"
	"}\n"
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