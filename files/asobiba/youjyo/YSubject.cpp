// YSubject.cpp: YSubject �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YSubject.h"
#include "YDATReader.h"
#include "YDATWritter.h"	//�e�X�g�Ŏg��.
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YSubject::YSubject()
{
	this->No = 0;
}

YSubject::~YSubject()
{
	RAutoWriteSpinLock al(&Lock);

	//All Aborn!
	for(int i = 0; i < this->No ; i++)
	{
		delete DATs[i];
	}
}


void YSubject::Age(const YDAT* inDAT)
{
	RAutoWriteSpinLock al(&Lock);

	Update();

	DATNUMBER num = inDAT->getDatNumber();
	if (this->No  >= 1)
	{
		//���͊��Ƀg�b�v
		if (DATs[0]->getDatNumber() == num)
		{
			DATs[0]->SubjectChange( inDAT->getSubjectAndLastNo() );
			return ;
		}

		//�ǂ����ɂ���?
		for( int i = 1 ; i < this->No ; i++ )
		{
			if (DATs[i]->getDatNumber() == num)
			{
				YNiseDAT* p = DATs[i];
				memmove( &DATs[1] , DATs , sizeof(YNiseDAT*) * i);
				DATs[0] = p;
				p->SubjectChange( inDAT->getSubjectAndLastNo() );
				return ;
			}
		}
	}
	//�Ȃ������Ƃ������Ƃł��ˁB
	if (this->No < MAX_SUBJECT)
	{
		memmove( &DATs[1] , DATs , sizeof(YNiseDAT*) * this->No );
		DATs[0] = new YNiseDAT( num , inDAT->getSubjectAndLastNo() );
		this->No ++;
		return ;
	}

	//�����A�Ō�̃��c���C�P�j�G.

	//DAT��������.
	delete DATs[this->No - 1];
	memmove( &DATs[1] , DATs , sizeof(YNiseDAT*) * (this->No-1) );
	DATs[0] = new YNiseDAT( num , inDAT->getSubjectAndLastNo() );

	return ;
}

void YSubject::Sage(const YDAT* inDAT)
{
	RAutoWriteSpinLock al(&Lock);

	Update();

	DATNUMBER num = inDAT->getDatNumber();
	//�ǂ����ɂ���?
	for( int i = 0 ; i < this->No ; i++ )
	{
		if (DATs[i]->getDatNumber() == num)
		{
			DATs[i]->SubjectChange( inDAT->getSubjectAndLastNo() );
			return ;
		}
	}
	//�Ȃ�? ���ᖳ��.
	return ;
}


void YSubject::Load(const string inBoardName)
{
	RAutoWriteSpinLock al(&Lock);
	//1036488047.dat<>�X�p�C�����`�������J�` �����X������4 (33)

	string filename =  YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\subject.txt";

	FILE * fp = fopen( filename.c_str() ,"rb");
	if (fp == NULL)
	{
		return;
	}

	char buf[MAX_SUBJECT_LINE];

	int i = 0;
	while( !feof(fp) )
	{
		if (i >= MAX_SUBJECT) break;
		if ( fgets(buf , MAX_SUBJECT_LINE , fp) == NULL ) break;
		
		const char* datnumber = buf;
		const char* subject = YDATReader::next(buf);
		if (subject == NULL) break;
		const char* end = YDATReader::nextnr(subject , subject + strlen(subject));
		if (end == NULL) break;

		string datnumberstring = string(datnumber , subject - datnumber - 2);

		
		DATs[i] = new YNiseDAT( atol( datnumberstring.c_str() ) , string(subject, end - subject) );
		i++; 
	}
	this->No = i;

	fclose(fp);

	Update();
}

void YSubject::Save(const string inBoardName)
{
	RAutoWriteSpinLock al(&Lock);

	string filename =  YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "\\subject.txt";

	FILE * fp = fopen( filename.c_str() ,"wb");
	if (fp == NULL)
	{
		return;
	}

	string str;
	for(int i = 0; i < this->No ; i++)
	{
		str = DATs[i]->CompateLine();
		fprintf(fp, str.c_str() );
	}

	fclose(fp);
}


/**
 * Show:				�\��			 YTextFile���p��
 *
 * @param outRPack		����Ɋi�[
 * @return void 
 */
void YSubject::Show(RPack * outRPack) const
{
	RAutoReadSpinLock al(&Lock);

	string ret;
	int no = this->No;

	ret.reserve(20 * no); //20�͓K��.
	for(int i = 0; i < no ; i++)
	{
		ret += DATs[i]->CompateLine().c_str() ;
	}
	outRPack->Write( ret.c_str() , ret.size() );
}


int YSubject::getNumbers(DATNUMBER * outNumbers , int inSize) const
{
	RAutoReadSpinLock al(&Lock);
	if ( inSize > this->No ) inSize = this->No;

	for(int i = 0; i < inSize ; i++)
	{
		outNumbers[i] = DATs[i]->getDatNumber();
	}
	return inSize;
}

int YSubject::WhatPosition(DATNUMBER inDATNumber) const
{
	RAutoReadSpinLock al(&Lock);

	const int n = this->No;
	for(int i = 0; i < n ; i++)
	{
		if (inDATNumber == DATs[i]->getDatNumber() ) return i;
	}
	
	ASSERT(0);
	return -1;
}

/**
 * DrawThreadIndex:			�X���b�h�ꗗ�̕`��
 *
 * @param inTemplate		�`��Ɏg���e���v���[�g
 * @param inReadCGIPath		read.cgi�̃p�X
 * @param inCount			���`�悷�邩?
 * @return string			�`�悵������	
 */
const string YSubject::DrawThreadIndex(const RHtmlTemplate* inTemplate , const string inReadCGIPath ,  int inCount) const
{
	RAutoReadSpinLock al(&Lock);

	RStringMap	smap;
	const int n = min( this->No , inCount);
	if (n <= 0) return "";

	string ret;
	ret.reserve(n * 512);	//512�͓K��.

	for(int i = 0; i < n ; i++)
	{
		smap["YThreadIndexNo"] = num2str(i+1);
		smap["YThreadIndexSubjectAndNo"] = DATs[i]->getSubject();
		smap["YThreadL50"] = inReadCGIPath + num2str( DATs[i]->getDatNumber() ) + "/l50";

		ret += inTemplate->Replace(&smap);
	}
	
	return ret;
}



void YSubject::test()
{
	string str;
	//�Ƃ肠�����A��������ł݂�!
	{
		YSubject	sub;
		YDAT* dat = new YDAT(12345);
		RPack pack;
		str = YDATWritter::make("�����Y","age","�N������������!" , "�N�\�X��");
		pack.Write(str.c_str() , str.size() );

		dat->Append(&pack);
		dat->setSubject("�N�\�X��");

		ASSERT(sub.No == 0);
		sub.Age(dat);
		ASSERT(sub.No == 1);
		ASSERT(sub.DATs[0]->getDatNumber() == 12345);
		ASSERT((str = sub.DATs[0]->CompateLine()) == "12345.dat<>�N�\�X�� (1)\n");

		delete dat;
	}
	//2����Ă݂�
	{
		YSubject	sub;
		YDAT* dat = new YDAT(12345);
		RPack pack;
		str = YDATWritter::make("�����Y","age","�N������������!" , "�N�\�X��");
		pack.Write(str.c_str() , str.size() );

		dat->Append(&pack);
		dat->setSubject("�N�\�X��");
		sub.Age(dat);		//����
		ASSERT(sub.No == 1);

		dat->Append(&pack);
		sub.Age(dat);		//1��� �������ꂾ����

		ASSERT(sub.No == 1);

		ASSERT(sub.DATs[0]->getDatNumber() == 12345);
		ASSERT((str = sub.DATs[0]->CompateLine()) == "12345.dat<>�N�\�X�� (2)\n");

		delete dat;
	}
	//��2����Ă݂�
	{
		YSubject	sub;
		YDAT* dat = new YDAT(12345);
		RPack pack;
		str = YDATWritter::make("�����Y","age","�N������������!" , "�N�\�X��");
		pack.Write(str.c_str() , str.size() );

		dat->Append(&pack);
		dat->setSubject("�N�\�X��");
		sub.Age(dat);		//����
		ASSERT(sub.No == 1);
		delete dat;

		dat = new YDAT(54321);
		dat->setSubject("�N�\�X��2");
		dat->Append(&pack);
		sub.Age(dat);		//2���
		ASSERT(sub.No == 2);
		delete dat;

		ASSERT(sub.DATs[0]->getDatNumber() == 54321);
		ASSERT((str = sub.DATs[0]->CompateLine()) == "54321.dat<>�N�\�X��2 (1)\n");

	}

	//A B A �Ɠ���Ă݂�.
	{
		YSubject	sub;
		YDAT* dat = new YDAT(12345);
		RPack pack;
		str = YDATWritter::make("�����Y","age","�N������������!" , "�N�\�X��");
		pack.Write(str.c_str() , str.size() );

		dat->Append(&pack);
		dat->setSubject("�N�\�X��");
		sub.Age(dat);		//����
		ASSERT(sub.No == 1);

		YDAT* dat2 = new YDAT(54321);
		dat2->setSubject("�N�\�X��2");
		dat2->Append(&pack);
		sub.Age(dat2);		//2���
		ASSERT(sub.No == 2);

		dat->Append(&pack);
		sub.Age(dat);		//2���
		ASSERT(sub.No == 2);

		ASSERT(sub.DATs[0]->getDatNumber() == 12345);
		ASSERT((str = sub.DATs[0]->CompateLine()) == "12345.dat<>�N�\�X�� (2)\n");

		delete dat2;
		delete dat;
	}

	//A B A B .... �� MAX_SUBJECT ����Ă݂܂�.
	{
		YSubject	sub;
		YDAT* dat = new YDAT(12345);
		dat->setSubject("�N�\�X��");
		RPack pack;
		str = YDATWritter::make("�����Y","age","�N������������!" , "�N�\�X��");
		pack.Write(str.c_str() , str.size() );
		RPack pack2;
		str = YDATWritter::make("�ȂȂ�����","age","�����[" , "�`����ʯ��");
		pack2.Write(str.c_str() , str.size() );

		YDAT* dat2 = new YDAT(54321);
		dat2->setSubject("�`����ʯ��");

		for(int i = 0; i < MAX_SUBJECT ; i ++)
		{
			dat->Append(&pack);
			sub.Age(dat);		// i*2 ���

			dat2->Append(&pack2);
			sub.Age(dat2);		// i*2+1 ���

			ASSERT(sub.No == 2);
		}

		ASSERT(sub.DATs[0]->getDatNumber() == 54321);
		ASSERT((str = sub.DATs[0]->CompateLine()) == "54321.dat<>�`����ʯ�� (300)\n");

		delete dat2;
		delete dat;
	}
	//A[1] A[2] .... �� MAX_SUBJECT ����Ă݂܂�.
	{
		YSubject	sub;
		for(int i = 0; i < MAX_SUBJECT ; i ++)
		{
			YDAT* dat = new YDAT(i);
			dat->setSubject("�N�\�X��");
			RPack pack;
			str = YDATWritter::make("�����Y","age","�N������������!" , "�N�\�X��");
			pack.Write(str.c_str() , str.size() );
			dat->Append(&pack);
			sub.Age(dat);		// i���

			ASSERT(sub.No == i+1);
			delete dat;
		}

		ASSERT(sub.DATs[0]->getDatNumber() == 299);
		ASSERT((str = sub.DATs[0]->CompateLine()) == "299.dat<>�N�\�X�� (1)\n");

		//����� C �𓊉�.
		RPack pack2;
		str = YDATWritter::make("�ȂȂ�����","age","�����[" , "�`����ʯ��");
		pack2.Write(str.c_str() , str.size() );

		YDAT* dat2 = new YDAT(54321);
		dat2->setSubject("�`����ʯ��");
		dat2->Append(&pack2);
		sub.Age(dat2);		// MAX_SUBJECT���

		ASSERT(sub.DATs[0]->getDatNumber() == 54321);
		ASSERT((str = sub.DATs[0]->CompateLine()) == "54321.dat<>�`����ʯ�� (1)\n");

		delete dat2;
	}
	puts("YSubject::test() OK");
}

