#pragma once

class CLIENT
{
	private:
		struct INFO
		{
			string Versione;
			string UAC;
			string InstallPath;
			string OS;
			string PCname;
			string UserName;
			string IP;
			IPinfo Location;
		};

	public:
		SOCKET sock;
		bool IsConnected = false;
		INFO info;
		bool InTask = false;
};