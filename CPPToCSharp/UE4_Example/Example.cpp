// Fill out your copyright notice in the Description page of Project Settings.


#include "Example.h"
#include "Misc/FileHelper.h"

typedef int(*GetCharSize)();
typedef char(*GetWebsiteContent)(int index);
typedef void(*SetWebsiteLength)(int i);
typedef char(*PostToWebsite)(char c_fullpath, bool lastIndex, int index);

GetCharSize m_getCharSize;
GetWebsiteContent m_getWebsiteContent;
SetWebsiteLength m_setWebsiteLength;
PostToWebsite m_postToWebsite;

void* v_dllHandle;

#pragma region Load DLL
bool UExample::importDLL(FString folder, FString name)
{
	FString filePath = *FPaths::ProjectPluginsDir() + folder + "/" + name;
	if (FPaths::FileExists(filePath))
	{
		v_dllHandle = FPlatformProcess::GetDllHandle(*filePath);
		if (v_dllHandle != nullptr)
			return true;
	}
	return false;
}

int UExample::importGetCharSize()
{
	if (v_dllHandle != NULL)
	{
		m_getCharSize = NULL;
		FString procName = "GetCharSize";
		m_getCharSize = (GetCharSize)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
	}
	if (m_getCharSize != NULL)
	{
		int out = m_getCharSize(); // Call the DLL method with arguments corresponding to the exact signature and return type of the method.
		return out;
	}
	return -32202;    // Return an error.
}

FString UExample::importGetWebsiteContent(int index)
{
	if (v_dllHandle != NULL)
	{
		m_getWebsiteContent = NULL;
		FString procName = "GetWebsiteContent";    // Needs to be the exact name of the DLL method.
		m_getWebsiteContent = (GetWebsiteContent)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
	}
	if (m_getWebsiteContent != NULL)
	{
		FString s = "";
		s += m_getWebsiteContent(index);
		return s;
	}
	return "Failed";
}

void UExample::importSetWebsiteLength(int i)
{
	if (v_dllHandle != NULL)
	{
		m_setWebsiteLength = NULL;
		FString procName = "SetWebsiteLength";    // Needs to be the exact name of the DLL method.
		m_setWebsiteLength = (SetWebsiteLength)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
	}
	if (m_setWebsiteLength != NULL)
		m_setWebsiteLength(i);
}

FString UExample::importPostToWebsite(FString c_fullpath, FString inputChar)
{
	if (v_dllHandle != NULL)
	{
		m_postToWebsite = NULL;
		FString procName = "PostToWebsite";
		m_postToWebsite = (PostToWebsite)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
	}
	char c = c_fullpath[0];
	if (m_postToWebsite != NULL)
	{
		FString OutFile = "Messanger.txt";
		bool b = FFileHelper::SaveStringToFile(inputChar, *(FPaths::GameSourceDir() +  OutFile), FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_NoReplaceExisting);
		if (!b)
			return "Failed posting to file";
		int l = c_fullpath.Len()+1;
		for (int i = 0; i < l; i++)
		{
			if (i != l - 1)
			{
				c = m_postToWebsite(c_fullpath[i], false, i);
				continue;
			}
			c = m_postToWebsite('\0', true, i);
		}
	}
	FString out = "";
	out += c;
	return out;
}

void UExample::freeDLL()
{
	if (v_dllHandle != NULL)
	{
		m_getCharSize = NULL;
		m_getWebsiteContent = NULL;
		m_setWebsiteLength = NULL;
		m_postToWebsite = NULL;
		FPlatformProcess::FreeDllHandle(v_dllHandle);
		v_dllHandle = NULL;
	}
}

#pragma endregion