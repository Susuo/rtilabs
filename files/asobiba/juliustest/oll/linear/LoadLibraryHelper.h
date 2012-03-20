#pragma once

//dll�ǂݍ��݃w���p�[
class LoadLibraryHelper
{
private:
	//DLL �C���X�^���X.
	HMODULE DllInstance;

public:
	LoadLibraryHelper()
	{
		this->DllInstance = NULL;
	}
	virtual ~LoadLibraryHelper()
	{
		if (this->DllInstance != NULL)
		{
			::FreeLibrary(this->DllInstance);
			this->DllInstance = NULL;
		}
	}
	bool Load( const std::string& inDLLName )//std�ǂ�ł��Ȃ����Ί��Ƃ��̂��߂� const char* �ō��.
	{
		this->DllInstance = ::LoadLibraryA(inDLLName.c_str() );
		if ( this->DllInstance == NULL )
		{
			DWORD lastError = ::GetLastError();
			return true;
		}
		return true;
	}
	FARPROC GetProcAddress(const char* inProcName)
	{
		return ::GetProcAddress(this->DllInstance,inProcName);
	}
};
