#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <algorithm>
#include <memory>

using namespace std;

void readFile()
{
	wstring pathToFile = L"C:\\Users\\titci\\source\\repos\\sp-2-lab\\sp-2-lab\\bensound-far.mp3";

    //Atidaromas failas skaitymui

	HANDLE hFile = CreateFile(
		pathToFile.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);


	if (hFile == INVALID_HANDLE_VALUE)
	{
		cout << "failed to open file\n";
		return ;
	}

    //Gaunamas failo dydis

	DWORD size = GetFileSize(hFile, nullptr);

	if (size == INVALID_FILE_SIZE)
	{
		cout << "Failed to get file size"<<endl;
		CloseHandle(hFile);
		return;
	}

    //Sukuriamas byte pointer masyvas nuskaitymui išsaugoti

	unique_ptr<BYTE[]> buff(new BYTE[size]);

	DWORD bytesRead;

	if (!ReadFile(hFile, buff.get(), size, &bytesRead, nullptr) || bytesRead != size)
	{
		cout << "Failed to read file\n";
		CloseHandle(hFile);
		return;
	}

	const char* tag = "TAG";

    // Tikrinama ar yra TAG

	if (memcmp(tag, buff.get() + size - 128, 3) != 0)
	{
		cout << "No ID3 tag found\n";
		CloseHandle(hFile);
		return;
	}

	string titleValue(reinterpret_cast<const char*>(buff.get() + size - 125), 30);
	string artistValue(reinterpret_cast<const char*>(buff.get() + size - 95), 30);
	string albumValue(reinterpret_cast<const char*>(buff.get() + size - 65), 30);

	cout << "MP3 title: " << titleValue << endl;
	cout << "MP3 artist name: " << artistValue << endl;
	cout << "MP3 album name: " << albumValue << endl;



	CloseHandle(hFile);
}

void writeSongTitle(const string newTitle)
{
    wstring pathToFile = L"C:\\Users\\titci\\source\\repos\\sp-2-lab\\sp-2-lab\\bensound-far.mp3";

    //Atidaromas failas skaitymui/rašymui

    HANDLE hFile = CreateFile(
        pathToFile.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::cout << "Failed to open file\n";
        return;
    }

    std::unique_ptr<char[]> buff(new char[128]);

    //Failo rodyklė nukreipiama į id3 tag'o pradžią

    if (SetFilePointer(hFile, -128, nullptr, FILE_END) == INVALID_SET_FILE_POINTER)
    {
        std::cout << "Failed to move file pointer\n";
        CloseHandle(hFile);
        return;
    }

    //Nuskaitomas ID3 tag'as

    DWORD bytesRead = 0;

    if (!ReadFile(hFile, buff.get(), 128, &bytesRead, nullptr))
    {
        std::cout << "Failed to read file\n";
        CloseHandle(hFile);
        return;
    }

    if (bytesRead != 128)
    {
        std::cout << "Failed to read ID3 tag\n";
        CloseHandle(hFile);
        return;
    }

    if (memcmp(buff.get(), "TAG", 3) != 0)
    {
        std::cout << "No ID3 tag found\n";
        CloseHandle(hFile);
        return;
    }


    // Ištrinamas senas pavadinimas

    memset(buff.get() + 3, 0, 30);

    // Nukopijuojamas naujas pavadinimas

    size_t newTitleLength = newTitle.size();
    if (newTitleLength > 30)
    {
        newTitleLength = 30;
    }
    
    memcpy(buff.get() + 3, newTitle.c_str(), newTitleLength);
    


    // Įrašomas naujas pavadinimas į failą

    if (SetFilePointer(hFile, -128, nullptr, FILE_END) == INVALID_SET_FILE_POINTER)
    {
        std::cout << "Failed to move file pointer\n";
        CloseHandle(hFile);
        return;
    }

    DWORD bytesWritten = 0;
    if (!WriteFile(hFile, buff.get(), 128, &bytesWritten, nullptr))
    {
        std::cout << "Failed to write file\n";
        CloseHandle(hFile);
        return;
    }

    if (bytesWritten != 128)
    {
        std::cout << "Failed to write ID3 tag\n";
        CloseHandle(hFile);
        return;
    }
    CloseHandle(hFile);
}

void writeSongAuthor(const string newAuthor)
{
    wstring pathToFile = L"C:\\Users\\titci\\source\\repos\\sp-2-lab\\sp-2-lab\\bensound-far.mp3";

    //Atidaromas failas skaitymui/rašymui

    HANDLE hFile = CreateFile(
        pathToFile.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::cout << "Failed to open file\n";
        return;
    }

    std::unique_ptr<char[]> buff(new char[128]);
    LPVOID virt = VirtualAlloc(buff.get(), 128, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    //Failo rodyklė nukreipiama į id3 tag'o pradžią

    if (SetFilePointer(hFile, -128, nullptr, FILE_END) == INVALID_SET_FILE_POINTER)
    {
        std::cout << "Failed to move file pointer\n";
        CloseHandle(hFile);
        return;
    }

    //Nuskaitomas ID3 tag'as

    DWORD bytesRead = 0;

    if (!ReadFile(hFile, buff.get(), 128, &bytesRead, nullptr))
    {
        std::cout << "Failed to read file\n";
        CloseHandle(hFile);
        return;
    }

    if (bytesRead != 128)
    {
        std::cout << "Failed to read ID3 tag\n";
        CloseHandle(hFile);
        return;
    }

    if (memcmp(buff.get(), "TAG", 3) != 0)
    {
        std::cout << "No ID3 tag found\n";
        CloseHandle(hFile);
        return;
    }


    // Ištrinamas senas autorius

    memset(buff.get() + 33, 0, 30);


    // Nukopijuojamas naujas autorius

    size_t newAuthorLength = newAuthor.size();
    if (newAuthorLength > 30)
    {
        newAuthorLength = 30;
    }

    memcpy(buff.get() + 33, newAuthor.c_str(), newAuthorLength);



    // Įrašomas naujas autorius į failą

    if (SetFilePointer(hFile, -128, nullptr, FILE_END) == INVALID_SET_FILE_POINTER)
    {
        std::cout << "Failed to move file pointer\n";
        CloseHandle(hFile);
        return;
    }

    DWORD bytesWritten = 0;
    if (!WriteFile(hFile, buff.get(), 128, &bytesWritten, nullptr))
    {
        std::cout << "Failed to write file\n";
        CloseHandle(hFile);
        return;
    }

    if (bytesWritten != 128)
    {
        std::cout << "Failed to write ID3 tag\n";
        CloseHandle(hFile);
        return;
    }
    VirtualFree(virt, 0, MEM_RELEASE);
    CloseHandle(hFile);
}
void writeSongAlbum(const string newAlbum)
{
    wstring pathToFile = L"C:\\Users\\titci\\source\\repos\\sp-2-lab\\sp-2-lab\\bensound-far.mp3";

    //Atidaromas failas skaitymui/rašymui

    HANDLE hFile = CreateFile(
        pathToFile.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::cout << "Failed to open file\n";
        return;
    }

    std::unique_ptr<char[]> buff(new char[128]);
    LPVOID virt = VirtualAlloc(buff.get(), 128, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    //Failo rodyklė nukreipiama į id3 tag'o pradžią

    if (SetFilePointer(hFile, -128, nullptr, FILE_END) == INVALID_SET_FILE_POINTER)
    {
        std::cout << "Failed to move file pointer\n";
        CloseHandle(hFile);
        return;
    }

    //Nuskaitomas ID3 tag'as

    DWORD bytesRead = 0;

    if (!ReadFile(hFile, buff.get(), 128, &bytesRead, nullptr))
    {
        std::cout << "Failed to read file\n";
        CloseHandle(hFile);
        return;
    }

    if (bytesRead != 128)
    {
        std::cout << "Failed to read ID3 tag\n";
        CloseHandle(hFile);
        return;
    }

    if (memcmp(buff.get(), "TAG", 3) != 0)
    {
        std::cout << "No ID3 tag found\n";
        CloseHandle(hFile);
        return;
    }


    // Ištrinamas senas albumas

    memset(buff.get() + 63, 0, 30);


    // Nukopijuojamas naujas albumas

    size_t newAlbumLength = newAlbum.size();
    if (newAlbumLength > 30)
    {
        newAlbumLength = 30;
    }

    memcpy(buff.get() + 63, newAlbum.c_str(), newAlbumLength);



    // Įrašomas naujas albumas į failą

    if (SetFilePointer(hFile, -128, nullptr, FILE_END) == INVALID_SET_FILE_POINTER)
    {
        std::cout << "Failed to move file pointer\n";
        CloseHandle(hFile);
        return;
    }

    DWORD bytesWritten = 0;
    if (!WriteFile(hFile, buff.get(), 128, &bytesWritten, nullptr))
    {
        std::cout << "Failed to write file\n";
        CloseHandle(hFile);
        return;
    }

    if (bytesWritten != 128)
    {
        std::cout << "Failed to write ID3 tag\n";
        CloseHandle(hFile);
        return;
    }
    VirtualFree(virt, 0, MEM_RELEASE);
    CloseHandle(hFile);
}

int main()
{
    cout << "Senas failas:" << endl;
	readFile();
    cout << endl;
    writeSongTitle("Naujas pavadinimas");
    writeSongAuthor("Naujas autorius");
    writeSongAlbum("Naujas albumas");
    cout << "Naujas failas:" << endl;
    readFile();
}

