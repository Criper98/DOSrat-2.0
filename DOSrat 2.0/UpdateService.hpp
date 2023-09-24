#pragma once

class UpdateService
{
private:

public:

    static bool AutoAggiornamento()
    {
        GitHub gh;
        DirUtils du;
        SystemUtils su;

        string UpdateVBS = "";
        string MFP = du.GetModuleFilePath();

        if (!du.CheckDir(MFP + "Update"))
            du.MakeDir(MFP + "Update");

        if (!gh.DownloadFromRepoRelease("criper98", "dosrat-2.0", "DOSrat2.0.zip", MFP + "Update\\"))
            return false;

        su.NoOutputCMD("PowerShell Expand-Archive '" + MFP + "Update\\DOSrat2.0.zip' '" + MFP + "Update' -force");

        du.DelFile(MFP + "Update\\DOSrat2.0.zip");

        UpdateVBS += "WScript.Sleep 2500\n";
        UpdateVBS += "Set filesys = CreateObject(\"Scripting.FileSystemObject\")\n";
        UpdateVBS += "Set WshShell = WScript.CreateObject(\"WScript.Shell\")\n";
        UpdateVBS += "filesys.DeleteFile \"" + MFP + "DOSrat 2.0.exe\"\n";
        UpdateVBS += "filesys.DeleteFile \"" + MFP + "Build\\Client.exe\"\n";
        UpdateVBS += "filesys.MoveFile \"" + MFP + "Update\\DOSrat 2.0.exe\", \"" + MFP + "DOSrat 2.0.exe\"\n";
        UpdateVBS += "filesys.MoveFile \"" + MFP + "Update\\Build\\Client.exe\", \"" + MFP + "Build\\Client.exe\"\n";
        UpdateVBS += "WScript.Sleep 1000\n";
        UpdateVBS += "WshShell.Run \"\"\"" + MFP + "DOSrat 2.0.exe\"\"\", 1, false\n";
        UpdateVBS += "filesys.DeleteFile \"" + MFP + "Update.vbs\"";

        if (!du.WriteFile(MFP + "Update.vbs", UpdateVBS))
            return false;

        du.WriteFile(MFP + "Release note.txt", gh.GetLastReleaseBody("Criper98", "DOSrat-2.0"));

        du.RunFile(MFP + "Update.vbs");
        du.RunFile(MFP + "Release note.txt");

        return true;
    }

    static void VerificaAggiornamenti(bool AutoUpdate)
    {
        GitHub gh;
        SystemUtils su;
        EasyMSGB msgb;

        string CurrTag = gh.GetLastReleaseTag("criper98", "dosrat-2.0");

        if (CurrTag != Version && CurrTag.find("-1 errore") == string::npos)
        {
            if (AutoUpdate)
            {
                if (AutoAggiornamento())
                    exit(0);
            }
            else
            {
                if (msgb.YesNo("Aggiornamento trovato!\nVuoi andare alla pagina ora?", msgb.Info, "DOSrat 2.0"))
                    su.OpenURL("https://github.com/Criper98/DOSrat-2.0/releases/latest");
            }
        }
    }
};