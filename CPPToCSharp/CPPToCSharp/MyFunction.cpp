#define _CRT_SECURE_NO_DEPRECATE
//#define outputLog;

#include <cstdlib>
#include <WinSock2.h>
#include <WinInet.h>
#include <iostream>
#include <string>
#include <fstream>

#pragma comment (lib, "Wininet.lib")
char* ourText;
char* uplaodText;

int WebsiteCall();
char* ReadFromWebsite(int* returnLength);
char Upload(char fullPath[]);

char Upload(char fullPath[])
{
	FILE* fp = fopen(fullPath, "r");
	if (fp == nullptr)
		return 'f';
	char* buf;
	fseek(fp, 0, SEEK_END);
	long lSize = ftell(fp);
	rewind(fp);
	buf = (char*)malloc(sizeof(char) * lSize+1); // extra one for '\0'
	if (buf == nullptr)
		return 'n';
	char c;
	int i = 0;
	while ((c = fgetc(fp)) != EOF)
	{
		buf[i] = c;
		i++;
	}
	buf[i] = '\0';
	fclose(fp);


	const LPCSTR url_path = "https://sleepwalker.pythonanywhere.com/welcome/default/p_index";
	DWORD nbytesRead = 0;
	DWORD ntotalBytesRead = 0;
	HINTERNET intOpen = InternetOpenA("Sample", LOCAL_INTERNET_ACCESS, NULL, 0, 0);
	HINTERNET hData = InternetOpenUrlA(intOpen, url_path, NULL, NULL, NULL, NULL);
	BOOL success = HttpSendRequestA(hData, NULL, 0, buf, i); //i more accurate then lSize
	DWORD dwAvailable, dwRead;
	char pOutBuf[4048];
	char* pTempBuf = pOutBuf;
	while(InternetQueryDataAvailable(hData, &dwAvailable, 0, 0) && dwAvailable != 0) //Their response
	{
		DWORD dwRead;
		InternetReadFile(hData, pTempBuf, dwAvailable, &dwRead);
		pTempBuf[dwRead] = 0;
		//std::cout << "\n" << pTempBuf;
		pTempBuf += dwRead;
	}
	InternetCloseHandle(hData);
	InternetCloseHandle(intOpen);
	free(buf);
	free(uplaodText);
	return 'S';
}
char* ReadFromWebsite(int* returnLength)
{
	char placeHolder = 'c';
	char* text = &placeHolder;
	HINTERNET intOpen = InternetOpenA("Sample", LOCAL_INTERNET_ACCESS, NULL, 0, 0);
	HINTERNET hData = InternetOpenUrlA(intOpen, "https://sleepwalker.pythonanywhere.com/welcome/default/g_index", NULL, NULL, NULL, NULL);
	char buf[2048];
	std::string sValue;
	HttpSendRequestA(hData, NULL, 0, NULL, 0);
	DWORD bytesRead = 0;
	while (InternetReadFile(hData, buf, 2000, &bytesRead) && bytesRead != 0)
	{
		buf[bytesRead] = 0;
		sValue += buf;
	}
	InternetCloseHandle(hData);
	InternetCloseHandle(intOpen);
	*returnLength = sValue.length();
	if (text == NULL || *returnLength == 0)
		return text;
	else
	{
		text = (char*)malloc(*returnLength * sizeof(char));
		for (int i = 0; i < *returnLength; i++)
			text[i] = sValue[i];
	}
	return text;
}
int WebsiteCall()
{
	int length = 0;
	ourText = ReadFromWebsite(&length);
	return length;
}
extern "C"
{
	__declspec(dllexport) int GetCharSize()
	{
		return WebsiteCall(); //ReadWebsite
	}

	__declspec(dllexport) char GetWebsiteContent(int index)
	{
		if (ourText == NULL)
			return 'n';
		size_t len = strlen(ourText);
		char c = ourText[index];
		if (index == len - 1)
			free(ourText);
		return c;
	}
	__declspec(dllexport) void SetWebsiteLength(int i)
	{
		uplaodText = (char*)malloc(i * sizeof(char));
	}
	__declspec(dllexport) char PostToWebsite(char c_fullpath, bool lastIndex, int index)
	{
		if (uplaodText == NULL)
			return 'n';
		uplaodText[index] = c_fullpath;
		if (lastIndex)
		{
#ifdef outputLog
			FILE* fp = fopen("C:\\Temp\\CPPToCSharp\\Log.txt", "w");
			if (fp != nullptr)
			{
				fprintf(fp, uplaodText);
				fclose(fp);
			}
#endif // outPutLog
			return Upload(uplaodText);
		}
		return 'c';
	}
}
