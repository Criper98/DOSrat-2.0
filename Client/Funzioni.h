#pragma once

string GetOS()
{
	/*RegUtils ru;

	ru.MasterKey = ru.HKLM;

	if(stoi(ru.RegRead("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CurrentBuild", REG_SZ)) >= 20000)
		return "Windows 11 " + ru.RegRead("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CompositionEditionID", REG_SZ);

	return ru.RegRead("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName", REG_SZ);*/

	GeneralUtils gu;

	return SimpleFind(gu.GetCMDOutput("powershell wmic os get Caption"), "Microsoft ", "  ");
}