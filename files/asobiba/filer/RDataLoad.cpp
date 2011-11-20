// RDataLoad.cpp: RDataLoad �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////


#include "comm.h"

#include "regex.h"

#include "RDataLoad.h"
#include "RConv.h"
#include "RStdioFile.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RDataLoad::RDataLoad()
{

}

RDataLoad::~RDataLoad()
{

}

string RDataLoad::Load(string inSetupFilename,string inSection,string inKey) 
				throw(RNotFoundException,RIOCannotOpenException,RIOCannotReadException)
{
	RStdioFile	StdFile;
	StdFile.Open(inSetupFilename ,"rb");

	string inNowSection = "����������@���Ȃ������ς�";
	string buf;

	//�Z�N�V�����V�[�N.
	while( !StdFile.IsEof() )
	{
		buf = RConv::rtrim( StdFile.ReadLine() );
		
		if (buf[0] == ':' && buf.substr(1) == inSection) break;
	}

	int keysize = inKey.size();
	boost::reg_expression<char> dataRegex  = "\\s(.*)$";

	//�L�[
	while( !StdFile.IsEof() )
	{
		buf = RConv::rtrim( StdFile.ReadLine() );
		
		if (buf[0] == ':') break;		//�ʂ̃Z�N�V�����ɂ͂������̂ŁA���̃L�[�͂Ȃ��Ƃ������Ƃ�.

		if (buf.size() > keysize)
		{
			if (inKey == buf.substr(0,keysize) )
			{
				boost::match_results<const char*> results;
				if ( boost::regex_search(buf.c_str() , results, dataRegex) )	return results[1];
			}
		}
	}

	throw RNotFoundException("�����Ώۂ� %s:%s �́A����܂���" ,inSection.c_str() ,inKey.c_str() );
}

void RDataLoad::Save(string inSetupFilename,string inSection,string inKey, string inData) 
			throw(RNotFoundException,RIOCannotOpenException,RIOCannotWriteException)
{
}

