#pragma once

// Classe per gestire i file tramite CLI
class CliFileExplorer
{
private:
	ConsoleUtils cu;
	TextColor tc;
	VectString FileExpLayout;
	VectString FileExpHelp;
	VectString FileExpInfo;
	int MinXsize = 0;
	int FileNameRows = 0;
	int FilesOffset = 0;
	int Sel = 0;
	short ConsoleSizeX = cu.GetConsoleSize().X;
	short ConsoleSizeY = cu.GetConsoleSize().Y - 1;
	string FileSpaces = "";
	string InfoSpaces = "";
	string MessageSpaces = string(ConsoleSizeX, ' ');
	COORD FilePos;
	COORD InfoPos;
	COORD FinalPos;

	struct FileExplorerStruct
	{
		bool IsFile; // true file; false dir;
		string Name;
		string Path;
		string FullPath;
		long long int FileSize = 0;
		double FileSizeDouble = 0;
		string Unit;
		string LastEdit;
	};

	struct CutCopyStruct
	{
		bool CutOrCopy = true; // true Copy; false Cut;
		bool IsFile; // true file; false dir;
		string PathToCutCopy;
		string Name;
	};

	vector<FileExplorerStruct> Files;

	void ClearFiles()
	{
		for (int i = 0; i < FileNameRows; i++)
		{
			cu.SetCursorPos({ FilePos.X, (short)(FilePos.Y + i) });

			cout << FileSpaces;
		}
	}

	void ClearInfo()
	{
		for (int i = 0; i < FileExpInfo.size(); i++)
		{
			cu.SetCursorPos({ InfoPos.X, (short)(InfoPos.Y + i) });

			cout << InfoSpaces;
		}
	}

	string to_string_with_precision(double a_value, int n = 2)
	{
		ostringstream out;
		out.precision(n);
		out << fixed << a_value;
		return out.str();
	}

public:
	bool ChangePartition = false;
	int HelpSize = 0;
	int FileNameSize = 0;
	CutCopyStruct CutCopy;

	CliFileExplorer()
	{
		FileExpLayout.push_back("File Explorer");
		FileExpLayout.push_back("Help");
		FileExpLayout.push_back("File Info");

		FileExpHelp.push_back("UP/DOWN: Navigate");
		FileExpHelp.push_back("ENTER: Run/Navigate");
		FileExpHelp.push_back("BACKSPACE: Up Folder");
		FileExpHelp.push_back("DEL: Delete File/Folder");
		FileExpHelp.push_back("R: Rename File/Folder");
		FileExpHelp.push_back("U: Upload");
		FileExpHelp.push_back("D: Download");
		FileExpHelp.push_back("M: Make Directory");
		FileExpHelp.push_back("F: Make File (empty)");
		FileExpHelp.push_back("X: Cut");
		FileExpHelp.push_back("C: Copy");
		FileExpHelp.push_back("V: Paste");
		FileExpHelp.push_back("P: Change Partition");
		FileExpHelp.push_back("Z: Zip/Unzip");
		FileExpHelp.push_back("F5: Refresh");
		FileExpHelp.push_back("ESC: Exit");

		FileExpInfo.push_back("Name: ");
		FileExpInfo.push_back("Path: ");
		FileExpInfo.push_back("Size: <size> <unit>");
		FileExpInfo.push_back("Last Edit: ");
		FileExpInfo.push_back("----------");
		FileExpInfo.push_back("<CpyCut> Buffer: ");
	}

	bool PrintLayout()
	{
		for (int i = 0; i < FileExpHelp.size(); i++)
			if (HelpSize < FileExpHelp[i].size())
				HelpSize = FileExpHelp[i].size();

		MinXsize = 5 + FileExpLayout[0].size() + HelpSize;

		if (ConsoleSizeY < FileExpHelp.size() + 4 + FileExpInfo.size() || ConsoleSizeX < MinXsize)
			return false; // oppure forza le dimensioni necessarie

		system("cls");

		FileNameSize = ConsoleSizeX - (4 + HelpSize);
		FileNameRows = ConsoleSizeY - (4 + FileExpInfo.size());

		for (int i = 0; i < FileNameSize; i++) { FileSpaces += " "; }
		for (int i = 0; i < FileNameSize + HelpSize + 2; i++) { InfoSpaces += " "; }

		// ┌─File Explorer───────────────┐┌─Help──────────────────┐
		tc.SetColor(tc.Blue);
		cout << char(218) << char(196);
		tc.SetColor(tc.Default);
		cout << FileExpLayout[0];
		tc.SetColor(tc.Blue);
		for (int i = 0; i < FileNameSize - 1 - FileExpLayout[0].size(); i++)
			cout << char(196);
		cout << char(191) << char(218) << char(196);
		tc.SetColor(tc.Default);
		cout << FileExpLayout[1];
		tc.SetColor(tc.Blue);
		for (int i = 0; i < HelpSize - FileExpLayout[1].size() - 1; i++)
			cout << char(196);
		cout << char(191) << endl;

		FilePos = { (short)(cu.GetCursorPos().X + 1), cu.GetCursorPos().Y };


		// │../                          ││↕: Up/Down             │
		for (int i = 0; i < FileNameRows; i++)
		{
			cout << char(179);
			for (int j = 0; j < FileNameSize; j++)
				cout << " ";
			cout << char(179) << char(179);
			if (i < FileExpHelp.size())
			{
				tc.SetColor(tc.Default);
				cout << FileExpHelp[i];
				tc.SetColor(tc.Blue);
				for (int j = 0; j < HelpSize - FileExpHelp[i].size(); j++)
					cout << " ";
			}
			else
				for (int j = 0; j < HelpSize; j++)
					cout << " ";
			cout << char(179) << endl;
		}


		// └─────────────────────────────┘└───────────────────────┘
		cout << char(192);
		for (int i = 0; i < FileNameSize; i++)
			cout << char(196);
		cout << char(217) << char(192);
		for (int i = 0; i < HelpSize; i++)
			cout << char(196);
		cout << char(217) << endl;


		// ┌─File Info────────────────────────────────────────────┐
		cout << char(218) << char(196);
		tc.SetColor(tc.Default);
		cout << FileExpLayout[2];
		tc.SetColor(tc.Blue);
		for (int i = 0; i < FileNameSize + HelpSize + 1 - FileExpLayout[2].size(); i++)
			cout << char(196);
		cout << char(191) << endl;


		// │Name: A                                               │
		for (int i = 0; i < FileExpInfo.size(); i++)
		{
			if (i == 0)
				InfoPos = { (short)(cu.GetCursorPos().X + 1), cu.GetCursorPos().Y };
			cout << char(179);
			tc.SetColor(tc.Default);
			cout << FileExpInfo[i];
			tc.SetColor(tc.Blue);
			for (int j = 0; j < FileNameSize + HelpSize + 2 - FileExpInfo[i].size(); j++)
				cout << " ";
			cout << char(179) << endl;
		}


		// └──────────────────────────────────────────────────────┘
		cout << char(192);
		for (int i = 0; i < FileNameSize + HelpSize + 2; i++)
			cout << char(196);
		cout << char(217) << endl;

		FinalPos = cu.GetCursorPos();
		tc.SetColor(tc.Default);

		return true;
	}

	void FilesParse(Json j)
	{
		Encode en;
		Files.clear();

		for (int i = 0; i < j["Files"].size(); i++)
		{
			FileExplorerStruct tmp = FileExplorerStruct();

			tmp.IsFile = j["Files"][i]["Type"];
			tmp.Name = en.UnicodeToAscii(j["Files"][i]["Name"]);
			tmp.Path = en.UnicodeToAscii(j["Files"][i]["Path"]);
			tmp.FullPath = en.UnicodeToAscii(j["Files"][i]["FullPath"]);
			tmp.FileSize = j["Files"][i]["Size"];
			tmp.Unit = "Byte";
			tmp.LastEdit = (j["Files"][i]["LastEdit"] != 0) ? DateTime::GetDateTime(j["Files"][i]["LastEdit"], ' ', '/') : "--";

			if (tmp.FileSize > 1000)
			{
				tmp.FileSizeDouble = (long double)tmp.FileSize / 1000;
				tmp.Unit = "Kilobyte";
			}

			if (tmp.FileSizeDouble > 1000)
			{
				tmp.FileSizeDouble = tmp.FileSizeDouble / 1000;
				tmp.Unit = "Megabyte";
			}

			if (tmp.FileSizeDouble > 1000)
			{
				tmp.FileSizeDouble = tmp.FileSizeDouble / 1000;
				tmp.Unit = "Gigabyte";
			}

			Files.push_back(tmp);
		}

		while (Sel >= Files.size() && Files.size() > 0)
		{
			UpdateSelection(false);
		}
	}

	bool UpdateContent()
	{
		string FinalCpyCutStr;
		cu.ExtendAsciiOutput();

		ClearFiles();

		for (int i = 0; i < FileNameRows; i++)
		{
			if (Files.size() - 1 > i + FilesOffset && i == FileNameRows - 1)
			{
				cu.SetCursorPos({ FilePos.X, (short)(FilePos.Y + i) });

				cout << Sel + 1 << "/" << Files.size();
			}
			else if (Files.size() > i + FilesOffset)
			{
				cu.SetCursorPos({ FilePos.X, (short)(FilePos.Y + i) });

				if (Files[i + FilesOffset].IsFile)
					tc.SetColor(tc.Default);
				else
					tc.SetColor(tc.Green);

				if (i + FilesOffset == Sel)
				{
					tc.SetColor(tc.Black);
					tc.SetBackColor(tc.SkyBlue);
				}

				if (Files[i + FilesOffset].Name.size() > FileNameSize)
				{
					string tmp = Files[i + FilesOffset].Name;
					tmp.replace(0, (tmp.size() + 3) - FileNameSize, "...");
					cout << tmp;
				}
				else
					cout << Files[i + FilesOffset].Name;

				tc.SetColor(tc.Default);
			}
			else if (Files.size() == 0 && i == 0)
			{
				cu.SetCursorPos({ FilePos.X, (short)(FilePos.Y + i) });

				tc.SetColor(tc.Red);

				cout << "Empty, probably a junction.";

				tc.SetColor(tc.Default);

				cu.UTF8asciiOutput();

				cu.SetCursorPos(FinalPos);
				cout << MessageSpaces;
				cu.SetCursorPos(FinalPos);
				cout << "> ";

				Sleep(1000);

				return false;
			}
			else
				break;
		}

		ClearInfo();


		cu.SetCursorPos(InfoPos);
		if (Files[Sel].Name.size() + FileExpInfo[0].size() > FileNameSize + HelpSize + 2)
		{
			string tmp = Files[Sel].Name;
			tmp.replace(3, (tmp.size() + FileExpInfo[0].size() + 3) - (FileNameSize + HelpSize + 2), "...");
			cout << FileExpInfo[0] << tmp;
		}
		else
			cout << FileExpInfo[0] << Files[Sel].Name;


		cu.SetCursorPos({ InfoPos.X, (short)(InfoPos.Y + 1) });
		if (Files[Sel].FullPath.size() + FileExpInfo[1].size() > FileNameSize + HelpSize + 2)
		{
			string tmp = Files[Sel].FullPath;
			tmp.replace(3, (tmp.size() + FileExpInfo[1].size() + 3) - (FileNameSize + HelpSize + 2), "...");
			cout << FileExpInfo[1] << tmp;
		}
		else
			cout << FileExpInfo[1] << Files[Sel].FullPath;


		cu.SetCursorPos({ InfoPos.X, (short)(InfoPos.Y + 2) });
		if (Files[Sel].Unit == "Byte")
			cout << StringUtils::PlaceHolder(StringUtils::PlaceHolder(FileExpInfo[2], "<size>", to_string(Files[Sel].FileSize)), "<unit>", Files[Sel].Unit);
		else
			cout << StringUtils::PlaceHolder(StringUtils::PlaceHolder(FileExpInfo[2], "<size>", to_string_with_precision(Files[Sel].FileSizeDouble)), "<unit>", Files[Sel].Unit);


		cu.SetCursorPos({ InfoPos.X, (short)(InfoPos.Y + 3) });
		cout << FileExpInfo[3] << Files[Sel].LastEdit;


		cu.SetCursorPos({ InfoPos.X, (short)(InfoPos.Y + 4) });
		cout << FileExpInfo[4];


		cu.SetCursorPos({ InfoPos.X, (short)(InfoPos.Y + 5) });
		if (CutCopy.CutOrCopy)
			FinalCpyCutStr = StringUtils::PlaceHolder(FileExpInfo[5], "<CpyCut>", "Copy");
		else
			FinalCpyCutStr = StringUtils::PlaceHolder(FileExpInfo[5], "<CpyCut>", "Cut");

		if (CutCopy.PathToCutCopy.size() + FinalCpyCutStr.size() > FileNameSize + HelpSize + 2)
		{
			string tmp = CutCopy.PathToCutCopy;
			tmp.replace(3, (tmp.size() + FinalCpyCutStr.size() + 3) - (FileNameSize + HelpSize + 2), "...");
			cout << FinalCpyCutStr << tmp;
		}
		else
			cout << FinalCpyCutStr << CutCopy.PathToCutCopy;

		cu.UTF8asciiOutput();

		cu.SetCursorPos(FinalPos);
		cout << MessageSpaces;
		cu.SetCursorPos(FinalPos);
		cout << "> ";

		return true;
	}

	// true DOWN; false UP;
	void UpdateSelection(bool UpOrDown)
	{
		if (UpOrDown)
		{
			if (Sel < Files.size() - 1)
			{
				Sel++;

				if (Sel == FileNameRows + FilesOffset - 1 && Sel < Files.size() - 1)
					FilesOffset++;
			}
		}
		else
		{
			if (Sel > 0)
			{
				Sel--;

				if (Sel < FilesOffset)
					FilesOffset--;
			}
		}
	}

	void ResetSelection()
	{
		Sel = 0;
		FilesOffset = 0;
	}

	FileExplorerStruct CurrSelectionInfo()
	{
		return Files[Sel];
	}

	void RealignVisual()
	{
		cu.SetCursorPos({ 0, 0 });
		cu.SetCursorPos(FinalPos);
	}
};