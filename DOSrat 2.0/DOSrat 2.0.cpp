#include <iostream>
#include <EssCurl.h>

using namespace std;

string Version = "2.0.0";

void StampaTitolo(short Returns = 0)
{
    TextColor tc;
    tc.SetColor(tc.Green);

    cout << "  ______  _______ ______                      ______    _____  " << endl;
    cout << " (______)(_______) _____)              _     (_____ \\  (_____) " << endl;
    cout << "  _     _ _     ( (____   ____ _____ _| |_     ____) ) _  __ _ " << endl;
    cout << " | |   | | |   | \\____ \\ / ___|____ (_   _)   / ____/ | |/ /| |" << endl;
    cout << " | |__/ /| |___| |____) ) |   / ___ | | |_   | (_____ |   /_| |" << endl;
    cout << " |_____/  \\_____(______/|_|   \\_____|  \\__)  |_______|_)_____/ " << endl << endl;
    cout << "| Versione [" << Version << "]" << endl;
    cout << "| DOSrat 2.0 by Criper98" << endl;

    for (int i = 0; i < Returns; i++)
        cout << endl;

    tc.SetColor(tc.Default);
}

int main()
{
    CLInterface cli;
    TextColor tc;
    DirUtils du;
    SettingsFile sf(du.GetFilePath() + "settings.ini");
    
    VectString MenuPrincipale;
    VectSettings MenuImpostazioni(5);

    MenuPrincipale.push_back("Esci");
    MenuPrincipale.push_back("Connetti Sessione");
    MenuPrincipale.push_back("Crea Client");
    MenuPrincipale.push_back("Comandi Comuni");
    MenuPrincipale.push_back("Impostazioni");

#pragma region Impostazioni

    MenuImpostazioni[0].Escape = true;
    MenuImpostazioni[0].Name = "Torna Indiero";

    MenuImpostazioni[1].Name = "Porta";
    MenuImpostazioni[1].Value = "5555";
    MenuImpostazioni[1].Escape = true;

    MenuImpostazioni[2].Name = "Verifica Aggiornamenti";
    MenuImpostazioni[2].CheckValue = "true";
    MenuImpostazioni[2].Value = "true";

    MenuImpostazioni[3].Name = "Colori";
    MenuImpostazioni[3].Value = "6 colori";
    MenuImpostazioni[3].Escape = true;

    MenuImpostazioni[4].Name = "Dimensioni Finestra";
    MenuImpostazioni[4].Value = "50";
    MenuImpostazioni[4].SecValue = "100";
    MenuImpostazioni[4].Escape = true;

#pragma endregion

    StampaTitolo(1);
    cli.SubTitle("Menu Principale", 60, tc.Green);
    
    switch (cli.MenuSingleSelQuadre(MenuPrincipale))
    {
        // Esci
        case 0:
            return 0;
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
            cli.MenuSettings(MenuImpostazioni);
        break;
    }

    return 0;
}