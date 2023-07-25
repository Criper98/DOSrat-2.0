#pragma once

// Classe per la gestione dei settaggi del Server
class SettaggiServer
{
private:
	DirUtils du;
	SettingsFile sf;

	void InitSettings()
	{
		if (!sf.CheckSetting("Port"))
			sf.SetSetting("Port", to_string(Porta));

		if (!sf.CheckSetting("CheckUpdate"))
			sf.SetSetting("CheckUpdate", (VerificaAggiornamenti) ? "true" : "false");

		if (!sf.CheckSetting("AutoUpdate"))
			sf.SetSetting("AutoUpdate", (AutoAggiornamento) ? "true" : "false");

		if (!sf.CheckSetting("WindowSizeX"))
			sf.SetSetting("WindowSizeX", to_string(DimensioniFinestra.X));

		if (!sf.CheckSetting("WindowSizeY"))
			sf.SetSetting("WindowSizeY", to_string(DimensioniFinestra.Y));
	}

public:
	int Porta = 6969;
	bool VerificaAggiornamenti = true;
	bool AutoAggiornamento = true;
	COORD DimensioniFinestra{ 800, 600 };

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
		MenuImpostazioni[4].Name = "Dimensioni Finestra";
		MenuImpostazioni[4].Value = to_string(DimensioniFinestra.X);
		MenuImpostazioni[4].SecValue = to_string(DimensioniFinestra.Y);
		MenuImpostazioni[4].Escape = true;

		return cli.MenuSettings(MenuImpostazioni);
	}
};