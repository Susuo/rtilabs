// RArchiveFactory.cpp: RArchiveFactory �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RArchiveFactory.h"
#include "RArchiveLha.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RArchiveFactory::RArchiveFactory()
{

}

RArchiveFactory::~RArchiveFactory()
{

}

//�T�|�[�g���Ă��Ȃ���� RNotSupportException
//!�������ł��Ȃ������� RDoNotInitaliseException
//!�֐����ǂݍ��߂Ȃ������� RDoNotInitaliseException
RArchive* RArchiveFactory::New(__ARCHIVETYPE inArchiveType) throw()
{
	RArchive* p = NULL;
	try
	{
		switch(inArchiveType)
		{
		case __ARCHIVETYPE_LZH:
			p = new RArchiveLha();
			p->Create();
			return p;
		}
	}
	catch(RNotSupportException e)
	{
		delete p;
		throw ;
	}
	catch(RDoNotInitaliseException e)
	{
		delete p;
		throw ;
	}
	throw RNotSupportException("RArchiveFactory::�w�肳�ꂽ���k�t�@�C�� %d �̓T�|�[�g������?",inArchiveType);
	return NULL;
}

