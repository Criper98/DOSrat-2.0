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
    GeneralUtils gu;
    TcpIP Server;
    Clu = new ClientUtils(settaggi);

    VectString MenuPrincipale;

    bool CicloMenu = true;

    MenuPrincipale.push_back("Esci");
    MenuPrincipale.push_back("Connetti Sessione");
    MenuPrincipale.push_back("Crea Client");
    MenuPrincipale.push_back("Comandi Comuni");
    MenuPrincipale.push_back("Impostazioni");

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
                CicloMenu = false;
            break;

            // Connetti Sessione
            case 1:
                return 0;
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
                            StampaPrefix(1); cout << "Inserisci la porta: ";
                            cin >> settaggi.Porta;
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
                            StampaPrefix(1); cout << "Ridimensiona le finestra a piacimento e premi invio per salvare." << endl;
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