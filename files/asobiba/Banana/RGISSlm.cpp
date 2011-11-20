// RGISSlm.cpp: RGISSlm �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISSlm.h"
#include "RStdioFile.h"
#include "RFileUtil.h"
#include "RGISSlp.h"
#include "Sec2Utm.h"


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISSlm::RGISSlm()
{
	this->X = 0;
	this->Y = 0;
	this->Key = 0;
	this->Scale = 0;
}

RGISSlm::~RGISSlm()
{

}

void RGISSlm::Create(const string & inFilename,int inScale) throw(RException)
{
	RStdioFile file;
	file.Open(inFilename , "rb");

	//�t�@�C�����͕K�� 01234.slm �̂悤�Ȑ����Ȃ�̂ł�����擾.
	this->Key = ParseKey(inFilename);
	if (this->Key == 0)
	{
		throw RException(EXCEPTIONTRACE + "�t�@�C�����͕K�� 01234.slm �̂悤�Ȑ����łȂ���΂Ȃ�܂���");
	}
	
	FILE * fp = file.getHandle();

	//5083260000,1547900000 ��ǂݍ��� __int64 �̃f�[�^�Ȃ̂Œ���
	char buffer[MAX_PATH];
	buffer[0] = '\0';
	fgets( buffer ,MAX_PATH, fp);
	if ( buffer[0] == '\0') throw RException(EXCEPTIONTRACE + "1�s�ڂ̋������ǂݏo���܂���");

	char * camma = strchr(buffer , ',' );
	if ( camma == NULL) throw RException(EXCEPTIONTRACE + "1�s�ڂ̋������J���}��؂�̃f�[�^�ɂȂ��Ă��܂���");
	*camma = '\0';

	this->X = _atoi64(buffer);
	this->Y = _atoi64(camma + 1);
	this->Scale = inScale;

	//6780000,3220000 2�s�ڂ̃f�[�^��ǂ�
	buffer[0] = '\0';
	fgets( buffer ,MAX_PATH, fp);
	if ( buffer[0] == '\0') throw RException(EXCEPTIONTRACE + "2�s�ڂ̋������ǂݏo���܂���");

	camma = strchr(buffer , ',' );
	if ( camma == NULL) throw RException(EXCEPTIONTRACE + "2�s�ڂ̋������J���}��؂�̃f�[�^�ɂȂ��Ă��܂���");
	*camma = '\0';
}


//�t�@�C�����͕K�� 01234.slm �̂悤�Ȑ����Ȃ�̂ł�����擾.
int RGISSlm::ParseKey(const string & inFilename) const
{
	int fileSep = inFilename.rfind('/');	//�����͎g���Ȃ�������v���悽�Ԃ�...
	if (fileSep == -1) fileSep = inFilename.rfind('\\');
	ASSERT(fileSep != -1);
	if (fileSep == -1) return 0;

	string number = inFilename.substr( fileSep + 1, inFilename.size() - fileSep - sizeof(".slm") );

	ASSERT( atoi( number.c_str() ) > 0);
	return atoi( number.c_str() );
}

//25000��ԃf�[�^�n�}�̍��W�n�� 
//ulong �Ɏ��܂�悤�� /100 ���� 10cm �̐��x�ɗ��Ƃ��܂��B
//�i�r�P�[�V�����Ȃ̂ŕʂɖ��Ȃ��͂�
UPOINT RGISMathZantei::ConvertScale(unsigned __int64 inX,unsigned __int64 inY) const
{
	ASSERT( this->Scale >= 100);

	UPOINT p;
	p.x = (unsigned __int64)(inX / this->Scale );
	p.y = (unsigned __int64)(inY / this->Scale );

	return p;
}


//�ǂ����Ō������Ƃ�����A�ӂ��C�̂�������.
//25000 ��ԃf�[�^���o�ܓx�ɕϊ�.
void RGISMathZantei::Kukan25000ToKeiido(__int64 inX, __int64 inY , RGISKeiidoData * ioToukei , RGISKeiidoData * ioHokui)
{
    double x = inX;
    double y = inY;
    double tmp1 = (double)(int)(x / (double)1000) * (double)1000;
    double tmp2 = (double)(int)(x / (double)100) * (double)100;
    if(tmp2 - tmp1 > (double)400)
        x = tmp1 + (double)1000;
    tmp1 = (double)(int)(y / (double)1000) * (double)1000;
    tmp2 = (double)(int)(y / (double)100) * (double)100;
    if(tmp2 - tmp1 > (double)400)
        y = tmp1 + (double)1000;
	//junk?
    double tmp3 = x + (double)83000;
    double tmp4 = y - (double)119000;
    tmp1 = (double)(int)(tmp3 / (double)1000) * (double)1000;
    tmp2 = (double)(int)(tmp3 / (double)100) * (double)100;
    if(tmp2 - tmp1 > (double)400)
        tmp3 = tmp1 + (double)1000;
    tmp1 = (double)(int)(tmp4 / (double)1000) * (double)1000;
    tmp2 = (double)(int)(tmp4 / (double)100) * (double)100;
    if(tmp2 - tmp1 > (double)400)
        tmp4 = tmp1 + (double)1000;
   {
        double ans = x / (double)36000000;
//        s = "���o " + s + (int)ans + "�x";
		ioToukei->Do = ans;

        ans -= (int)ans;
        ans *= (double)60;
//        s = s + (int)ans + "��";
		ioToukei->Fun = ans;

        ans -= (int)ans;
        ans *= (double)60;
//        s = s + (int)ans + ".";
		ioToukei->Byou = ans;

        ans -= (int)ans;
        ans = (int)(ans * (double)10);
//        s = s + (int)ans + "�b";
		ioToukei->M = ans;

        ans = y / (double)36000000;
//        s = s + " �k�� " + (int)ans + "�x";
		ioHokui->Do = ans;

        ans -= (int)ans;
        ans *= (double)60;
//        s = s + (int)ans + "��";
		ioHokui->Fun = ans;

        ans -= (int)ans;
        ans *= (double)60;
//        s = s + (int)ans + ".";
		ioHokui->Byou = ans;

        ans -= (int)ans;
        ans = (int)(ans * (double)10);
//        s = s + (int)ans + "�b";
		ioHokui->M = ans;
    }
}

UPOINT RGISMathZantei::RevKukan25000ToKeiido(const RGISKeiidoData * inToukei ,const RGISKeiidoData * inHokui ) 
{
	__int64 x,y ;
	x = inToukei->toDouble() * 36000000;
	y = inHokui->toDouble() * 36000000;

	__int64 t = x + 1000.0;
	double tmp1 = (double)(int)(x / (double)1000) * (double)1000;
	double tmp2 = (double)(int)(x / (double)100) * (double)100;
	if(!  (tmp2 - tmp1 > (double)400))
		x = t;
	t = y + 1000.0;
	tmp1 = (double)(int)(y / (double)1000) * (double)1000;
	tmp2 = (double)(int)(y / (double)100) * (double)100;
	if(! (tmp2 - tmp1 > (double)400))
		y = t;

	UPOINT ret;
	ret.x = x;
	ret.y = y;

	return ret;
}



void RGISSlm::test()
{
	{
		RGISKeiidoData toukei;
		RGISKeiidoData hokui;

		UPOINT xy;

		RGISMathZantei a;
		a.Kukan25000ToKeiido(5083260000,1547900000, &toukei , &hokui);
		xy = a.RevKukan25000ToKeiido(&toukei , &hokui );

		ASSERT(xy.x == 5083260000);
		ASSERT(xy.y == 1547900000);

	}
}
