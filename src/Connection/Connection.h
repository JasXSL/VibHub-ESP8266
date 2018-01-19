#include "../SocketIOClient/SocketIOClient.h"
#include "../Config/Config.h"

class Connection{

	public:
		Connection();
		void ini( Config co );
		bool wifiConnected();
		void loop();
	private:
		SocketIOClient client;
		bool wifiConnect();
		void socketConnect( bool reconnect );
		Config conf;

};
