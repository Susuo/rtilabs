#include <StartWithExeFile.h>
#ifdef ZONDE_LIFE
/**************************************************************/
//�菑�������F�����[�`�� (�]���f�@) (�\��(^^;)
/**************************************************************/
#include <windows.h>
#include <stdio.h>
#include <.\Graphics\rti_graphics.h>
#include <\.AI\rti_Ai_Zonde.h>
#include <.\Base\rti_error_message.h>
#include <.\Base\rti_debug.h>



/**************************************************************/
//�]���f�@�̏�����
/**************************************************************/
void TAIZonde::Init()
{
	if (! LoadLog(LOG_NAME) )
	{
		ClearLog();	
	}
}
/**************************************************************/
//�w�肵���G�ɂ�����Ă��镶���f�[�^��F�����܂�!!
/**************************************************************/
void TAIZonde::Start(BMD bmd,int MojiColor)
{
	//�����Q�b�g���܂�.
	ZondeGetData(bmd,MojiColor);
	//���̏��ƃf�[�^�x�[�X���r���܂�!
	ZondeMatting();
}

/**************************************************************/
//�]���f�@�ŋ��߂����ʂɂ������č��ۂ������n��!!
/**************************************************************/
void TAIZonde::HumanCheck(unsigned char SekaiMoji)
{
	if (SekaiMoji == Char)
	{
		//�吳��!! �����ȂłȂł��Ă�����.
		CharPattern[Char][Char_PatternNo].OkPattern ++;
	}
	else
	{
		//�s�����Ȃ̂ŁA�����̕����̃f�[�^���ׂăP�c�o�b�g!!
		for(int i = 0; i < MAX_PATTERN ; i++)
		{
			//�P�c�o�b�g��H��킹��!! �΂��� 
			CharPattern[Char][i].OkPattern --;
		}
		//�����̕����̔F���f�[�^���Ԃ�����.
		InsertData(SekaiMoji);
	}
}

/**************************************************************/
//�]���f�@�̏I��
/**************************************************************/
void TAIZonde::End()
{
	SaveLog(LOG_NAME);
}

/**************************************************************/
//�ߋ��̋L�^���Q�b�g���܂�.
/**************************************************************/
bool TAIZonde::LoadLog(char *filename)
{
	FILE *fp;
	//�t�@�C����! �J��!
	fp = fopen(filename,"rb");
	if (!fp)
	{
		//�t�@�C�����J���Ȃ������񂪁[
        FILE_NOT_OPEN("TAIZonde::LoadLog",filename);
		return false;
	}
	//�������݂��񂪁[
	fread(CharPattern,sizeof(struct _CharPattern),ALL_CHAR * MAX_PATTERN,fp);
	//�t�@�C����A����!!
	fclose(fp);
	return true;
}

/**************************************************************/
//�ߋ��̋L�^���܂�.
/**************************************************************/
bool TAIZonde::SaveLog(char *filename)
{
	FILE *fp;
	//�t�@�C����! �J��!
	fp = fopen(filename,"wb");
	if (!fp)
	{
		//�t�@�C�����J���Ȃ������񂪁[
        FILE_NOT_OPEN("TAIZonde::SaveLog",filename);
		return false;
	}
	//�������݂��񂪁[
	fwrite(CharPattern,sizeof(struct _CharPattern),ALL_CHAR * MAX_PATTERN,fp);
	//�t�@�C����A����!!
	fclose(fp);
	return true;
}

/**************************************************************/
//�L���r���ɂ��܂�(��
/**************************************************************/
void TAIZonde::ClearLog()
{
	int i,l,m;
	for(i = 0; i < ALL_CHAR ; i++)
	{
		for(l = 0; l < MAX_PATTERN ; l++)
		{
			for (m = 0 ; m < ZONDE_LINE ; m++)
			{
				CharPattern[i][l].LineHit[m] = 100;
			}
			CharPattern[i][l].OkPattern = 0;
			CharPattern[i][l].DataAlive = false;
		}
	}
}

/**************************************************************/
//�����Q�b�g���܂�.
/**************************************************************/
void TAIZonde::ZondeGetData(BMD bmd,int MojiColor)
{
	//�œK�����Ȃ��{�P �Q�C�c�N�̃R���p�C���p�ɂ����ł����Ă����܂��傤��.
	int BmdW = bmd->w;
	int BmdH = bmd->h;
	int w4,w2;
	int i;
	//�N���A���Ă����܂��傤��. �{���͐錾�̎��ɂ���Ă�������ł���(^^;;
	ZeroMemory(&TestPattern,sizeof(struct _TestPattern) );

	w4 = BmdW / 4;
	for(i = 0 ; i < BmdW ; i++){
		ZondeCheck(bmd,w4,i,0,MojiColor);
		ZondeCheck(bmd,w4*2,i,1,MojiColor);
		ZondeCheck(bmd,w4*3,i,2,MojiColor);
		ZondeCheck(bmd,i,w4,3,MojiColor);
		ZondeCheck(bmd,i,w4*2,4,MojiColor);
		ZondeCheck(bmd,i,w4*3,5,MojiColor);
		ZondeCheck(bmd,i,i,6,MojiColor);
		ZondeCheck(bmd,BmdW - i,i,7,MojiColor);
	}
	w2 = BmdW / 2;
	for(i = 0 ; i < w2 ; i++){
		ZondeCheck(bmd,i,w2 - i,8,MojiColor);
		ZondeCheck(bmd,i,w2 + i,9,MojiColor);
		ZondeCheck(bmd,i + w2,BmdW - i - 1,10,MojiColor);
		ZondeCheck(bmd,i + w2,i,11,MojiColor);
	}

}

/**************************************************************/
//�`�F�b�N���܂�!
/**************************************************************/
void TAIZonde::ZondeCheck(BMD bmd,int x,int y,int n , int MojiColor)
{
	long c;
		
	c = PGet(bmd,x,y);
	if( c != MojiColor)
	{  //�w�i
		TestPattern.BigLine[n] = false;
	}
	else
	{ //��
		//��������1�Ƃ݂Ȃ��B
		if( ! TestPattern.BigLine[n] )
		{ 
			TestPattern.LineHit[n]++; //���������񐔂��J�E���g
			TestPattern.BigLine[n] = true;
		}
	}
}

/**************************************************************/
//�`�F�b�N�������e���r���Ă݂������.
/**************************************************************/
void TAIZonde::ZondeMatting()
{
	int ihozon,lhozon;
	int min;
	int i,l,d;
	ihozon = 0;
	lhozon = 0;
	min  = 0xffff;

	for(i = 0; i < ALL_CHAR ; i++)
	{
		for(l = 0; l < MAX_PATTERN ; l++)
		{
			d = ZondeDistance(i,l);
			if (d < min)
			{	//�Œ�l�X�V.
				min = d;
				ihozon = i;
				lhozon = l;
			}
		}
	}
	//�Ԃ茌���ǂ�.
	Char = (unsigned char)ihozon;
	Char_PatternNo = lhozon;
}

/**************************************************************/
//�f�[�^�x�[�X�ƌ��݂̃f�[�^�̔�r. (���������߂�) 2��a�����߂�.
/**************************************************************/
int TAIZonde::ZondeDistance(int i,int l)
{
	int m,sum , t;
	struct _CharPattern *CPI;
	sum = 0;
	CPI = &CharPattern[i][l];

	for(m = 0 ; m < 12 ; m++)
	{
		t = ( CPI->LineHit[m] - TestPattern.LineHit[m]);
		sum += t*t;
	}
	return( sum );
}

/**************************************************************/
//�����̃f�[�^�𖄂ߍ���
/**************************************************************/
void TAIZonde::InsertData(unsigned char SekaiMoji)
{
	bool InsertOk = false;
	int  MinOk = 0xffff;
	int  iMin  = 0;
	struct _CharPattern *CP = CharPattern[SekaiMoji];
	struct _CharPattern *CPI;

	for(int i = 0; i < MAX_PATTERN ; i++)
	{
		//�����Ă���f�[�^��T��.
		CPI = &CP[i];
		if ( ! CPI->DataAlive )
		{	//�󂫔���!!
			InsertOk = true;
			memcpy( &CPI->LineHit , &TestPattern.LineHit , sizeof(int) * ZONDE_LINE);
			CPI->OkPattern = 0;
			CPI->DataAlive  = true;
			break;
		}
		//�����Ƃ��F�����̂��f�[�^�N��T���Ă܂�.
		if (MinOk > CPI->OkPattern)
		{	//�Œ�l�X�V!!
			MinOk = CPI->OkPattern;
			iMin  = i;
		}
	}
	if (InsertOk) return ; //�}������܂���. �߂ł��� �߂ł���.

	CPI = & CharPattern[SekaiMoji][iMin];
	//�}������X�y�[�X���Ȃ��̂ŁA
	//�����Ƃ��F�����̈����f�[�^�������āA���̃f�[�^�������.
	memcpy( &CPI->LineHit , &TestPattern.LineHit , sizeof(int) * ZONDE_LINE);
	CPI->OkPattern = 0;
	CPI->DataAlive  = true;
}

#endif	//LIFE END
