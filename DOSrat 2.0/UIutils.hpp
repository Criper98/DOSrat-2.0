#pragma once

class UIutils
{
private:

public:

    static void StampaTitolo(short Returns = 0)
    {
        TextColor tc;

        tc.SetColor(tc.Green);

        cout << "  ______  _______ ______                      ______    _____  " << endl;
        cout << " (______)(_______) _____)              _     (_____ \\  (_____) " << endl;
        cout << "  _     _ _     ( (____   ____ _____ _| |_     ____) ) _  __ _ " << endl;
        cout << " | |   | | |   | \\____ \\ / ___|____ (_   _)   / ____/ | |/ /| |" << endl;
        cout << " | |__/ /| |___| |____) ) |   / ___ | | |_   | (_____ |   /_| |" << endl;
        cout << " |_____/  \\_____(______/|_|   \\_____|  \\__)  |_______|_)_____/ " << endl << endl;
        cout << "| v" << Version << endl;
        cout << "| Created by Criper98" << endl;

        for (int i = 0; i < Returns; i++)
            cout << endl;

        tc.SetColor(tc.Default);
    }

    static void StampaPrefix(string PC = "", string USER = "")
    {
        TextColor tc;

        tc.SetColor(tc.Yellow);
        cout << "DOSrat 2.0";
        tc.SetColor(tc.Default);

        if (PC != "")
        {
            cout << " [";
            tc.SetColor(tc.Purple);
            cout << PC;
            tc.SetColor(tc.Default);
            cout << "->";
            tc.SetColor(tc.Purple);
            cout << USER;
            tc.SetColor(tc.Default);
            cout << "]";
        }

        cout << "> ";
    }

    static void StampaHelp(string Cmd, string Desc)
    {
        ConsoleUtils cu;

        COORD Cpos;
        COORD Csize = cu.GetConsoleSize();

        cout << Cmd;

        Cpos = cu.GetCursorPos();

        if (Desc.size() < Csize.X - Cpos.X)
        {
            cout << Desc << endl;
            return;
        }

        for (int i = 0; i < Desc.size(); i++)
        {
            if (cu.GetCursorPos().X == Csize.X - 1)
            {
                cout << endl;
                Cpos.Y++;
                cu.SetCursorPos({ Cpos.X, Cpos.Y });
            }

            cout << Desc[i];
        }

        cout << endl;
    }

    static void StampaIncompatibile()
    {
        TextColor tc;

        tc.SetColor(tc.Red);
        cout << "Questo comando non e' compatibile con il Client.\nAggiorna il Client con uno piu' recente tramite il comando \"Update\"." << endl;
        tc.SetColor(tc.Default);
    }

    static void DrunkeranEgg()
    {
        TextColor tc;
        ConsoleUtils cu;
        SystemUtils su;
        WindowUtils wu;

        COORD ScreenRes = su.GetDesktopSize();
        short PosX = su.GetRandomNumber(ScreenRes.X - 810), PosY = su.GetRandomNumber(ScreenRes.Y - 610);
        bool AddX = true, AddY = true;
        COORD WinSize = { 700, 625 };
        COORD OldWinSize = wu.GetWindowSize();
        COORD OldWinPos = wu.GetWindowPos();

        su.OpenURL("https://www.instagram.com/drunkeran.e.co/");
        wu.SetWindowSize(WinSize);
        system("cls");

        tc.SetColor(tc.Lime);
        cout << "                                :TGKj}r:`                                  " << endl;
        cout << "                             =eQMv- '\"^YIPPY:      -)YVykkVc}?~-           " << endl;
        cout << "                          _u$I;           `<k0dv-^DBX]?*^<*|}wPEQ$y,       " << endl;
        cout << "                        }QZ^                  _cQ#!             `*V#k`     " << endl;
        cout << "                      !BW:             _==.      *d`            y_LcBE`    " << endl;
        cout << "                      $R            `vv:``~}Y`    (v            -z *iZE`   " << endl;
        cout << "                     \"@* ~=`      `}}`      \"U\"  ,Yl             :K-G!dM   " << endl;
        cout << "              `(kaIzUd@.  !g* `,_`m:          vsm^}Y              ,0*Y~DI  " << endl;
        cout << "           :cMPi\"     K    -Qyl:,!=               E!               ,w }~#x " << endl;
        cout << "         `aO:         `     _#x                  ,g                 *T.cw@'" << endl;
        cout << "        .$U                  \"QV      `!^^^*^:  `$*        -U=       >qVr@." << endl;
        cout << "      -]Qx                    `z$\" .rr<'    `:xyDM         VQk9v!=<**!` <8 " << endl;
        cout << "    :OZ0c                       ~gd}`          T8`        Tg` _Dc       I} " << endl;
        cout << "   :#v_D                          TQv         ^8Vxxv\\vv]xT@Y    ym     _#' " << endl;
        cout << "  `BU R:                      .)xxv~^Hx`     >g`          -PR=   dP==~-Du  " << endl;
        cout << "  LB`ry                    =xv*.      rZV_  \"Bir|xxxx\\r~!,.`=RR. `0U**Qw   " << endl;
        cout << " `#) 0`                 :ix\"     `:?ix]YTyMTR:.`````'-\"=^v}Vzd#`  Lb ~Q    " << endl;
        cout << " }# ^8TYi(\"          `rx!    `*VyT^.       ,.       `TL`     .Bx  ^#uB:    " << endl;
        cout << " QH q\"   `!v]x, `yP]TV-   _xTx: `}UV.     -V}w*   `}Qxv$y'    5R   _^`     " << endl;
        cout << "`@y`R        `*V68!@^   ry)`   *z: .VL` `x}`  *q  y#,   )gU-:gV            " << endl;
        cout << ":@x=k          i@v`@!`VI\"      'Yx`  -Vuu'   ~V-   \"dk`   >dE_             " << endl;
        cout << "!@xr}         x#!  BoD~          _z(       -y*       ;RY`       |`         " << endl;
        cout << " Bz*T        !@^   ^80             ]V     :P          'B3     ,QB*         " << endl;
        cout << " b$`^        $X     .#Y          'x=       :x.       ~B}      `b#y         " << endl;
        cout << " v@<        r#`      =#x        rL    `VL`   vi`    LB=   `Tx   L}         " << endl;
        cout << "  ^Qb\"      gT        -6I` =)  o:    xm-`xx` *}'  `Hd`   TB}}Q)            " << endl;
        cout << "    vB\\    '@!          v9Go   :}= :U>    `xu:    )Q^  _$G-  _Rqd;         " << endl;
        cout << "     -#r   >@-           T#.     _ix                *KIQ<      *@!         " << endl;
        cout << "      !#r  *@-           `x6DM:                       ,`       r@`         " << endl;
        cout << "       ~@K!,@:              !@*           -          ``        EO          " << endl;
        cout << "        ')e$@}              `#x           d`         =\\       >@:          " << endl;
        cout << "                             )#:        'O=          K'      \"#v           " << endl;
        cout << "                              *Qu:    ,yB@U`       .6B}.   \"y#Y            " << endl;
        cout << "                                rhbPMMz)`-M@Mi^!~?M8> ^zdddh*`             " << endl;
        cout << "                                           `^ThIuv\"                        " << endl;
        tc.SetColor(tc.Default);

        for (int i = 0; i < 2500; i++)
        {
            cu.SetCursorVisible(false);

            wu.SetWindowSize(WinSize);
            wu.SetFocusWindow();
            wu.SetWindowPos({ PosX, PosY });

            if (PosX <= 0)
                AddX = true;
            else if (PosX + WinSize.X >= ScreenRes.X)
                AddX = false;

            if (PosY <= 0)
                AddY = true;
            else if (PosY + WinSize.Y >= ScreenRes.Y)
                AddY = false;

            if (AddX)
                PosX += 2;
            else
                PosX -= 2;

            if (AddY)
                PosY += 2;
            else
                PosY -= 2;

            Sleep(1);
        }

        wu.SetWindowSize(OldWinSize);
        wu.SetWindowPos(OldWinPos);
    }
};