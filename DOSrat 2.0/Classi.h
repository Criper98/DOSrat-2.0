#pragma once

class Client
{
	private:
		

	public:
		

};

// Classe per la gestione dei settaggi
class Settaggi
{
	private:
		DirUtils du;
		SettingsFile sf;

		// Struttura contenete i colori della console
		struct COLORI
		{
			short Conferma = 10;
			short Negativo = 12;
			short Avviso = 14;
		};


	public:
		int Porta = 0;
		bool VerificaAggiornamenti = true;
		COLORI Colori;
		COORD DimensioniFinestra{800, 600};
		
		// Costruttore
		Settaggi()
		{
			sf.SetFileName(du.GetFilePath() + "settings.ini");
		}

		// Popola i settaggi ottenendoli dal relativo file
		bool GetSettings()
		{
			if (!sf.CheckSetting("Port"))
				return false;

			Porta = stoi(sf.GetSetting("Port"));
			if (sf.GetSetting("CheckUpdate") == "true")
				VerificaAggiornamenti = true;
			else
				VerificaAggiornamenti = false;
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
			/*if (!sf.CheckSetting("Port"))
				return false;*/

			sf.SetSetting("Port", to_string(Porta));
			if(VerificaAggiornamenti)
				sf.SetSetting("CheckUpdate", "true");
			else
				sf.SetSetting("CheckUpdate", "false");
			sf.SetSetting("ColorOk", to_string(Colori.Conferma));
			sf.SetSetting("ColorError", to_string(Colori.Negativo));
			sf.SetSetting("ColorWarning", to_string(Colori.Avviso));
			sf.SetSetting("WindowSizeX", to_string(DimensioniFinestra.X));
			sf.SetSetting("WindowSizeY", to_string(DimensioniFinestra.Y));

			return true;
		}
};