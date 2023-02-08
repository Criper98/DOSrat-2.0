#include <iostream>
#include <EssNet.h>
#include <Essentials.h>
#include <json.hpp>

using json = nlohmann::json;

string Version = "2.0.0 B1";

#include "Classi.h"
#include "Funzioni.h"

using namespace std;

int main()
{
    TcpIP Client;
    SystemUtils su;
    DirUtils du;

    su.GetCPUload(); // Altrimenti con il GetInfo la prima volta da sempre il 6%

    Client.Host = "192.168.1.150";
    Client.Port = 5555;

    if (du.CheckDir("VXBkYXRl"))
        du.DelDir("VXBkYXRl");

    if (Client.StartClient() != 0)
    {
        cout << "Errore nell'inizializzazione del Client" << endl;
        Client.Stop();
        _getch();
        return 0;
    }

    for (bool i = true; i;)
    {
        while (!Client.Connect()) {}
        cout << "Connesso" << endl;
        if (COMUNICAZIONI::Inizializzazione(Client.Sock))
        {
            switch (Sessione(Client))
            {
                case 0:
                    closesocket(Client.Sock);
                    break;

                case 1:
                    closesocket(Client.Sock);
                    Client.Stop();
                    return 0;
                    break;
            }
            cout << "Disconnesso" << endl;

        }
        Client.Stop();
        Client.StartClient();
    }

    system("pause");

    return 0;
}