// BAddressMatching.cpp: BAddressMatching クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BAddressMatching.h"
#include "RConv.h"
#include "RStdioFile.h"
#include "RFileUtil.h"
#include "BConf.h"
#include "BBanana.h"



	//都道府県マッチングテーブルなのですよー.
	static const char* Todoufuken[] = {
		"北海道",
		"青森県",
		"岩手県",
		"宮城県",
		"秋田県",
		"山形県",
		"福島県",
		"茨城県",
		"栃木県",
		"群馬県",
		"埼玉県",
		"千葉県",
		"東京都",
		"神奈川県",
		"新潟県",
		"富山県",
		"石川県",
		"福井県",
		"山梨県",
		"長野県",
		"岐阜県",
		"静岡県",
		"愛知県",
		"三重県",
		"滋賀県",
		"京都府",
		"大阪府",
		"兵庫県",
		"奈良県",
		"和歌山県",
		"鳥取県",
		"島根県",
		"岡山県",
		"広島県",
		"山口県",
		"徳島県",
		"香川県",
		"愛媛県",
		"高知県",
		"福岡県",
		"佐賀県",
		"長崎県",
		"熊本県",
		"大分県",
		"宮崎県",
		"鹿児島県",
		"沖縄県",
	};


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////


//アドレスにマッチするコードを取得する.
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

	//マッチしませんでした.
	return NULL;
}

bool BAddressParse::Parse(const string &inAddress ,  const  AddressToCodeMatchingTable* inMatchTable , int inTableSize)
{
	if (inAddress.empty() ) return false;

	int skip = SkipTodoufuKen(inAddress);
	string ad = inAddress.c_str() + skip;

	//都道府県だけで検索した?
	if (ad.empty() )
	{
		return false;
	}

	//コードマッチング.
	const AddressToCodeMatchingTable* codeTable = ToCode( ad ,inMatchTable , inTableSize);
	if (codeTable == NULL)
	{
		return false;	//失敗.
	}

	this->Code = codeTable->Code;

	//マッチしたので、次は番地の取得に行きます。
	//残ったところが MachiCyoume なわけ.

	//コードの部分の読み飛ばし.
	ad = ad.c_str() + codeTable->Address.size();
	if ( ad.empty() )
	{
		return false;	//失敗.
	}

	skip = 0;
	const char * ban;
	do
	{
		//丁目という指示が明確にある場合...
		if ( ( ban = getChyoume(ad.c_str() , "丁目" , &skip) ) != NULL) break;

		//一-一-一 という方式
		if ( ( ban = getChyoume(ad.c_str() , "-" , &skip) ) != NULL) break;

		//一−一−一 という方式
		if ( ( ban = getChyoume(ad.c_str() , "−" , &skip) ) != NULL) break;

		//一の一の一 という方式
		if ( ( ban = getChyoume(ad.c_str() , "の" , &skip) ) != NULL) break;

		//一ノ一ノ一 という方式
		if ( ( ban = getChyoume(ad.c_str() , "ノ" , &skip) ) != NULL) break;

	}
	while(0);	//ダミーループ.


	string r;
	if (ban != NULL) 
	{
		//丁目があったのでそこまで.
		r = string(ad.c_str() , 0 , ban - ad.c_str()  );
		r += "丁目";
	}
	else if (( ban = getChyoume(ad.c_str() , &skip) ) != NULL) 
	{
		//丁目がない場合 2バイト以外の文字列が出てくるまで.
		r = string(ad.c_str() , 0 , ban - ad.c_str()  );
	}
	else
	{
		//それすらもない場合は認識できたところまで
		this->MachiCyoume = ad ;
		this->Banchi = 0;

		return true;
	}

	this->MachiCyoume = RConv::atok(r);
	r = RConv::atos(ban + skip);
	this->Banchi = atoi( r.c_str() );

	return true;
}

//丁目という指示が明確にある場合...
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

//都道府県があったら読み飛ばし.
int BAddressParse::SkipTodoufuKen(const string & inAddress) const
{

	const char * mas = inAddress.c_str();


	for(int i = 0 ; i < TABLE_COUNT_OF(Todoufuken) ; i++)
	{
		//先に零になったほうが負け.
		const char * p = mas;
		const char * n = Todoufuken[i];
		for( ; *n && *p && *n==*p; n ++ , p++)	
			;
		if (*n == '\0')	return p - mas;
	}
	//都道府県はついてないらしい.
	return 0;
} 


void BAddressParse::test()
{
	AddressToCodeMatchingTable table[4];
	table[0].Address = "川崎市川崎区";
	table[0].Code = 14131;
	table[1].Address = "札幌市中央区";
	table[1].Code = 1101;
	table[2].Address = "札幌市北区";
	table[2].Code = 1102;
	table[3].Address = "札幌市東区";
	table[3].Code = 1103;

	{
		BAddressParse parse;

		parse.Parse("神奈川県川崎市川崎区野望王国1192", table , 4);
		ASSERT( parse.getCode() == 14131 );
		ASSERT( parse.getMachiCyoume() == "野望王国" );
		ASSERT( parse.getBanchi() == 1192 );
	}
	{
		BAddressParse parse;

		parse.Parse("北海道札幌市中央区旭ケ丘123", table , 4);
		ASSERT( parse.getCode() == 1101 );
		ASSERT( parse.getMachiCyoume() == "旭ケ丘" );
		ASSERT( parse.getBanchi() == 123 );
	}
	{
		BAddressParse parse;

		parse.Parse("北海道札幌市中央区旭ケ丘一丁目1番", table , 4);
		ASSERT( parse.getCode() == 1101 );
		ASSERT( parse.getMachiCyoume() == "旭ケ丘一丁目" );
		ASSERT( parse.getBanchi() == 1 );
	}
	{
		BAddressParse parse;

		parse.Parse("北海道札幌市中央区旭ケ丘一丁目" , table , 4);
		ASSERT( parse.getCode() == 1101 );
		ASSERT( parse.getMachiCyoume() == "旭ケ丘一丁目" );
		ASSERT( parse.getBanchi() == 0 );
	}
}

