// RGISSlp.cpp: RGISSlp �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISSlp.h"
#include "RStdioFile.h"
#include "RFileUtil.h"



//.slp�t�@�C���� x, y �̂��ꂼ��̒���.
const int SLP_LONG_MAX	=	8;

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISSlp::RGISSlp()
{
	Count = 0;
	Points= NULL;
}

RGISSlp::~RGISSlp()
{
	if (this->Points != NULL) delete [] this->Points;
}

void RGISSlp::Create(const string & inFilename,const RGISSlm * inSlm) throw(RException)
{
	ASSERT(Points == NULL);


	int lineCount = RFileUtil::LineCount(inFilename);
	if (lineCount <= 0) throw RException(EXCEPTIONTRACE + "0�s�̃t�@�C���ł�");

	this->Count  = lineCount - 1;
	this->Points = new UPOINT[this->Count];

	RStdioFile file;
	file.Open(inFilename , "rb");

	FILE * fp = file.getHandle();

	//0170970000897100 �̐�����ǂݍ���
	//�^���� x , y �ɕ�����.
	char buffer[MAX_PATH];
	char x[SLP_LONG_MAX + 1];
	for(int i = 0 ; ! feof(fp) ; i++)
	{
		buffer[0] = '\0';
		fgets( buffer ,MAX_PATH, fp);
		if ( buffer[0] == '\0') break;

		memcpy(x , buffer , SLP_LONG_MAX );
		x[SLP_LONG_MAX] = '\0';

		long cx,cy;	// - ���܂܂�邽�� unsigned �ɂ��Ȃ�. �ő�8�P�^�Ȃ̂ő������v
		cx = atol(x) ;
		cy = atol(buffer + SLP_LONG_MAX)  ;
		this->Points[i] = inSlm->ConvertGlobal( cx , cy );

	}
}

//�X�P�[����ύX���A�f�B�X�v���C���W�ɂ��܂��B
void RGISSlp::ConvertDisplayScal() throw(RException)
{
	int count = this->Count;
	for(int i = 0 ; i < count ; i++)
	{

		this->Points[i].x = RGISMathZantei::ConvertDisplayScalX( this->Points[i].x );
		this->Points[i].y = RGISMathZantei::ConvertDisplayScalY( this->Points[i].y );
	}
}


//�ő�ŏ��̏ꏊ�̎擾.
LRECT RGISSlp::getRect() const
{
	LRECT rc;
	rc.left = 0xffffffff;
	rc.top = 0xffffffff;
	rc.right = 0;
	rc.bottom = 0;

	int count = this->Count;
	for(int i = 0 ; i < count ; i++)
	{
		const UPOINT * p = &this->Points[i];
		if ( p->x < rc.left  ) rc.left = p->x;
		if ( p->y < rc.top   ) rc.top = p->y;
		if ( p->x > rc.right  ) rc.right = p->x;
		if ( p->y > rc.bottom) rc.bottom = p->y;
	}

	return rc;
}

//TestCV���ُ�ȂƂ�����h���Ă��Ȃ����`�F�b�N����.
bool RGISSlp::TestCV(GISINTArray* inArray)
{
	int count = inArray->getCount();
	for(int i = 0; i < count;i++ )
	{
		unsigned int p = inArray->getPoint(i);

		if (p > Count)
		{
			ASSERT(0);
			return false;
		}
	}
	return true;
}


void RGISSlp::test()
{
	//���y�n���@�̗������Ȃ��ƃe�X�g�f�[�^�ł��N�Q�ɂȂ邩������Ȃ��̂ŁA
	//�e�X�g���[�`���͓������܂���
	try
	{
		/*
		RGISSlm sm;
		RGISSlp gs;
		gs.Create("test/test.slp" , &sm );

		unsigned int r;
		ASSERT((r = gs.getCount() ) == 10);

		UPOINT rr;
		UPOINT tt;

//		tt.x = 1709700;		tt.y = 897100;
		tt.x = 17097;		tt.y = 8971;
		rr = gs.getPoint(0);
		ASSERT( (rr.x == tt.x) && (rr.y == tt.y) );

//		tt.x = 2681700;		tt.y = 618100;
		tt.x = 26817;		tt.y = 6181;
		rr = gs.getPoint(4);
		ASSERT( (rr.x == tt.x) && (rr.y == tt.y) );

//		tt.x = 3174700;		tt.y = 2061100;
		tt.x = 31747;		tt.y = 20611;
		rr = gs.getPoint(9);
		ASSERT( (rr.x == tt.x) && (rr.y == tt.y) );
		*/
	}
	catch(RException e)
	{
		ASSERT(0);
		throw;
	}
}

