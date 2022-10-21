#include <iostream>
#include <EssCurl.h>

string Version = "2.0.0";

#include "Funzioni.h"
#include "Classi.h"

using namespace std;

int main()
{
    CLInterface cli;
    TextColor tc;
    DirUtils du;
    Settaggi settaggi;
    ConsoleUtils cu;
    
    VectString MenuPrincipale;

    bool CicloMenu = true;

    MenuPrincipale.push_back("Esci");
    MenuPrincipale.push_back("Connetti Sessione");
    MenuPrincipale.push_back("Crea Client");
    MenuPrincipale.push_back("Comandi Comuni");
    MenuPrincipale.push_back("Impostazioni");

    settaggi.GetSettings();

    cu.SetConsoleWindowSize({ settaggi.DimensioniFinestra.X, settaggi.DimensioniFinestra.Y });

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

                            settaggi.DimensioniFinestra.X = cu.GetConsoleWindowSize().X;
                            settaggi.DimensioniFinestra.Y = cu.GetConsoleWindowSize().Y;
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