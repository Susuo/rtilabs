// BAddressMatching.cpp: BAddressMatching �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BAddressMatching.h"
#include "RConv.h"
#include "RStdioFile.h"
#include "RFileUtil.h"
#include "BConf.h"
#include "BBanana.h"



	//�s���{���}�b�`���O�e�[�u���Ȃ̂ł���[.
	static const char* Todoufuken[] = {
		"�k�C��",
		"�X��",
		"��茧",
		"�{�錧",
		"�H�c��",
		"�R�`��",
		"������",
		"��錧",
		"�Ȗ،�",
		"�Q�n��",
		"��ʌ�",
		"��t��",
		"�����s",
		"�_�ސ쌧",
		"�V����",
		"�x�R��",
		"�ΐ쌧",
		"���䌧",
		"�R����",
		"���쌧",
		"�򕌌�",
		"�É���",
		"���m��",
		"�O�d��",
		"���ꌧ",
		"���s�{",
		"���{",
		"���Ɍ�",
		"�ޗǌ�",
		"�a�̎R��",
		"���挧",
		"������",
		"���R��",
		"�L����",
		"�R����",
		"������",
		"���쌧",
		"���Q��",
		"���m��",
		"������",
		"���ꌧ",
		"���茧",
		"�F�{��",
		"�啪��",
		"�{�茧",
		"��������",
		"���ꌧ",
	};


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


//�A�h���X�Ƀ}�b�`����R�[�h���擾����.
const AddressToCodeMatchingTable* BAddressParse::ToCode(const string & inAddress , const  AddressToCodeMatchingTable* inMatchTable , int inTableSize) const
{
	ASSERT(inMatchTable != NULL);

	int addresSize = inAddress.size();

	for(int i = 0; i < inTableSize ; i++)
	{
		const AddressToCodeMatchingTable * table = &inMatchTable[i];

		int size = addresSize < (int) table->Address.size() ? addresSize : table->Address.size();
		if ( memcmp(inAddress.c_str() , table->Address.c_str() , size) == 0)
		{
			return table;
		}
	}

	//�}�b�`���܂���ł���.
	return NULL;
}

bool BAddressParse::Parse(const string &inAddress ,  const  AddressToCodeMatchingTable* inMatchTable , int inTableSize)
{
	if (inAddress.empty() ) return false;

	int skip = SkipTodoufuKen(inAddress);
	string ad = inAddress.c_str() + skip;

	//�s���{�������Ō�������?
	if (ad.empty() )
	{
		return false;
	}

	//�R�[�h�}�b�`���O.
	const AddressToCodeMatchingTable* codeTable = ToCode( ad ,inMatchTable , inTableSize);
	if (codeTable == NULL)
	{
		return false;	//���s.
	}

	this->Code = codeTable->Code;

	//�}�b�`�����̂ŁA���͔Ԓn�̎擾�ɍs���܂��B
	//�c�����Ƃ��낪 MachiCyoume �Ȃ킯.

	//�R�[�h�̕����̓ǂݔ�΂�.
	ad = ad.c_str() + codeTable->Address.size();
	if ( ad.empty() )
	{
		return false;	//���s.
	}

	skip = 0;
	const char * ban;
	do
	{
		//���ڂƂ����w�������m�ɂ���ꍇ...
		if ( ( ban = getChyoume(ad.c_str() , "����" , &skip) ) != NULL) break;

		//��-��-�� �Ƃ�������
		if ( ( ban = getChyoume(ad.c_str() , "-" , &skip) ) != NULL) break;

		//��|��|�� �Ƃ�������
		if ( ( ban = getChyoume(ad.c_str() , "�|" , &skip) ) != NULL) break;

		//��̈�̈� �Ƃ�������
		if ( ( ban = getChyoume(ad.c_str() , "��" , &skip) ) != NULL) break;

		//��m��m�� �Ƃ�������
		if ( ( ban = getChyoume(ad.c_str() , "�m" , &skip) ) != NULL) break;

	}
	while(0);	//�_�~�[���[�v.


	string r;
	if (ban != NULL) 
	{
		//���ڂ��������̂ł����܂�.
		r = string(ad.c_str() , 0 , ban - ad.c_str()  );
		r += "����";
	}
	else if (( ban = getChyoume(ad.c_str() , &skip) ) != NULL) 
	{
		//���ڂ��Ȃ��ꍇ 2�o�C�g�ȊO�̕����񂪏o�Ă���܂�.
		r = string(ad.c_str() , 0 , ban - ad.c_str()  );
	}
	else
	{
		//���ꂷ����Ȃ��ꍇ�͔F���ł����Ƃ���܂�
		this->MachiCyoume = ad ;
		this->Banchi = 0;

		return true;
	}

	this->MachiCyoume = RConv::atok(r);
	r = RConv::atos(ban + skip);
	this->Banchi = atoi( r.c_str() );

	return true;
}

//���ڂƂ����w�������m�ɂ���ꍇ...
const char * BAddressParse::getChyoume(const char * inStr,const string inPrefix , int *outSkip) const
{
	const char * p;
	p = strstr(inStr , inPrefix.c_str() );

	*outSkip = inPrefix.size();

	return p;
}
const char * BAddressParse::getChyoume(const char * inStr, int *outSkip) const
{
	const char * p;
	p = FindNot2Byte(inStr );

	*outSkip = 0;

	return p;
}

const char * BAddressParse::FindNot2Byte(const char *inStr) const
{
	int count = strlen(inStr) ;
	if (count <= 1) return NULL;

	for (int i = 0 ; i < count ; i++)
	{
		if ( RConv::isKanji( (unsigned char)inStr[i] , (unsigned char)inStr[i + 1] ) )
		{
			i++;
		}
		else
		{
			return inStr + i;
		}
	}
	return NULL;
}

//�s���{������������ǂݔ�΂�.
int BAddressParse::SkipTodoufuKen(const string & inAddress) const
{

	const char * mas = inAddress.c_str();


	for(int i = 0 ; i < TABLE_COUNT_OF(Todoufuken) ; i++)
	{
		//��ɗ�ɂȂ����ق�������.
		const char * p = mas;
		const char * n = Todoufuken[i];
		for( ; *n && *p && *n==*p; n ++ , p++)	
			;
		if (*n == '\0')	return p - mas;
	}
	//�s���{���͂��ĂȂ��炵��.
	return 0;
} 


void BAddressParse::test()
{
	AddressToCodeMatchingTable table[4];
	table[0].Address = "���s����";
	table[0].Code = 14131;
	table[1].Address = "�D�y�s������";
	table[1].Code = 1101;
	table[2].Address = "�D�y�s�k��";
	table[2].Code = 1102;
	table[3].Address = "�D�y�s����";
	table[3].Code = 1103;

	{
		BAddressParse parse;

		parse.Parse("�_�ސ쌧���s�����]����1192", table , 4);
		ASSERT( parse.getCode() == 14131 );
		ASSERT( parse.getMachiCyoume() == "��]����" );
		ASSERT( parse.getBanchi() == 1192 );
	}
	{
		BAddressParse parse;

		parse.Parse("�k�C���D�y�s�����戮�P�u123", table , 4);
		ASSERT( parse.getCode() == 1101 );
		ASSERT( parse.getMachiCyoume() == "���P�u" );
		ASSERT( parse.getBanchi() == 123 );
	}
	{
		BAddressParse parse;

		parse.Parse("�k�C���D�y�s�����戮�P�u�꒚��1��", table , 4);
		ASSERT( parse.getCode() == 1101 );
		ASSERT( parse.getMachiCyoume() == "���P�u�꒚��" );
		ASSERT( parse.getBanchi() == 1 );
	}
	{
		BAddressParse parse;

		parse.Parse("�k�C���D�y�s�����戮�P�u�꒚��" , table , 4);
		ASSERT( parse.getCode() == 1101 );
		ASSERT( parse.getMachiCyoume() == "���P�u�꒚��" );
		ASSERT( parse.getBanchi() == 0 );
	}
}

