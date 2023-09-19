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
    COORD OldWinSize = wu.GetWindowSize();
    COORD OldWinPos = wu.GetWindowPos();

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

    for (int i = 0; i < 2500; i++)
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

    wu.SetWindowSize(OldWinSize);
    wu.SetWindowPos(OldWinPos);
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

void AccettaConnessioni(TcpIP& Server, bool& mostraNotifiche)
{
    SystemUtils su;
    DirUtils du;
    int c = 0;

    su.ShowNotifyIcon("DOSrat 2.0");

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
            if (COMUNICAZIONI::Inizializzazione(c, Clients[c].sock) && mostraNotifiche)
                su.SendNotify("Client Connesso", "IP: " + Clients[c].info.IP + "\nPC: " + Clients[c].info.PCname + "\nUser: " + Clients[c].info.UserName + "\nOS: " + Clients[c].info.OS);
    }

    su.HideNotifyIcon();

    return;
}

void VerificaConnessioni()
{
    while (true)
    {
        Sleep(500);

        if (!ServerLoopController)
            break;

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            Sleep(2);

            if (!ServerLoopController)
                break;

            if (Clients[i].IsConnected && !Clients[i].InTask)
            {
                TcpIP::SetTimeout(500, Clients[i].sock);

                if (!TcpIP::SendString(Clients[i].sock, "keepalive"))
                {
                    Clients[i].Kill();
                    Clu->AggiornaTitolo();
                }
                else if (Clients[i].info.CompatibleVer >= 5)
                {
                    string tmp;

                    if (!TcpIP::RecvString(Clients[i].sock, tmp) || tmp == "")
                    {
                        Clients[i].Kill();
                        Clu->AggiornaTitolo();
                    }
                }

                TcpIP::SetTimeout(0, Clients[i].sock);
            }
        }
    }

    return;
}

bool RestartServer(TcpIP &Server, int Port, bool& mostraNotifiche)
{
    ServerLoopController = false;

    Sleep(1000);

    for (int i = 0; i < MAX_CLIENTS; i++)
        if (Clients[i].IsConnected)
        {
            Clients[i].Kill();
            Clu->AggiornaTitolo();
        }

    Server.Stop();
    Server.Port = Port;
    if (Server.StartServer() != 0)
        return false;

    Sleep(1000);

    ServerLoopController = true;

    thread Aconn(AccettaConnessioni, ref(Server), ref(mostraNotifiche));
    thread Vconn(VerificaConnessioni);
    Aconn.detach();
    Vconn.detach();

    return true;
}

string decompress_string(const std::string& str)
{
    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    if (inflateInit(&zs) != Z_OK)
        throw(std::runtime_error("inflateInit failed while decompressing."));

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();

    std::vector<char> outbuffer(32768, 0); // Output buffer with initial size of 32KB
    std::string outstring;

    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer.data());
        zs.avail_out = outbuffer.size();

        int ret = inflate(&zs, 0);
        if (ret == Z_STREAM_ERROR) {
            inflateEnd(&zs);
            throw(std::runtime_error("Exception during zlib decompression."));
        }

        // Calculate the number of bytes written to the buffer
        std::size_t bytes_written = outbuffer.size() - zs.avail_out;

        // append the block to the output string
        outstring.append(outbuffer.data(), bytes_written);

        // Resize the buffer if more space is available
        if (zs.avail_out == 0)
            outbuffer.resize(outbuffer.size() * 2);

    } while (zs.avail_out == 0);

    inflateEnd(&zs);

    return outstring;
}