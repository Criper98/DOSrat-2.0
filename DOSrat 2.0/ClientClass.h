#pragma once

class CLIENT
{
	private:
	struct INFO
	{
		string Versione;
		bool UAC;
		string InstallPath;
		string OS;
		string PCname;
		string UserName;
		string IP;
		string Nazione;
	};

	public:
	SOCKET sock;
	bool IsConnected = false;
	INFO info;

};