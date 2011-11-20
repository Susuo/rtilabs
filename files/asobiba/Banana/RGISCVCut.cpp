// RGISCVCut.cpp: RGISCVCut �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "RGISCVCut.h"
#include "RDiskUtil.h"
#include "RDebug.h"

const int NEXT_LINE_BLOCK	= 4096;
//const int NEXT_LINE_BLOCK	= 3200 * 1000 * 10;



//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RGISCVCut::RGISCVCut()
{

}

RGISCVCut::~RGISCVCut()
{

}

//��������
void RGISCVCut::Write(const string & inDir , const RGISSlp * inSlp,
					  const GISINTArray* inArray ,RGISCVCutDataIml * ioData)
{
	PointSet * p = new PointSet[65535];

	int i;

	//CV�̐��̎擾...
	unsigned short count = inArray->getCount();

	UPOINT lastStep = {0};
	unsigned short trueCVcount = 0;
	//�Ƃ肠�����A���ׂẴf�[�^���u���b�N���Ƃɐ؂蕪���Ă݂܂��B
	for(i = 0 ;  i < count ; i++)
	{
		int pt = inArray->getPoint(i) ;
		if (pt != 0 )
		{
			//���x�� /100 �ɂ������ʁA�ׂ荇���_���܂����������ꏊ�ɔz�u����Ă��閳�ʂȌ�������܂��B
			//�����r�����ăf�[�^�T�C�Y�����������܂�.
			UPOINT getpoint = inSlp->getPoint(pt - 1);

//			TRACE("%I64u, %I64u\n" , getpoint.x , getpoint.y);

			if (!(lastStep.x == getpoint.x && lastStep.y == getpoint.y ))
			{
				lastStep = getpoint;
				p[trueCVcount].po = getpoint;
				p[trueCVcount].lastBlock = selectBlock( &p[trueCVcount].po );
				trueCVcount++;
			}
		}
	}
	if (trueCVcount != 0) 
	{
		WriteMain(inDir ,  p , trueCVcount , ioData);
	}

	delete [] p;
}

//��������
void RGISCVCut::Write(const string & inDir , 
					  const RGIS2500PointArray* inArray ,RGISCVCutDataIml * ioData)
{
	PointSet * p = new PointSet[65535];

	int i;

	//CV�̐��̎擾...
	unsigned short count = inArray->getCount();

	UPOINT lastStep = {0};
	unsigned short trueCVcount = 0;
	//�Ƃ肠�����A���ׂẴf�[�^���u���b�N���Ƃɐ؂蕪���Ă݂܂��B
	for(i = 0 ;  i < count ; i++)
	{
		//���x�� /100 �ɂ������ʁA�ׂ荇���_���܂����������ꏊ�ɔz�u����Ă��閳�ʂȌ�������܂��B
		//�����r�����ăf�[�^�T�C�Y�����������܂�.
		UPOINT getpoint= inArray->getPoint(i) ;


//			TRACE("%I64u, %I64u\n" , getpoint.x , getpoint.y);

		if (!(lastStep.x == getpoint.x && lastStep.y == getpoint.y ))
		{
			lastStep = getpoint;
			p[trueCVcount].po = getpoint;
			p[trueCVcount].lastBlock = selectBlock( &p[trueCVcount].po );
			trueCVcount++;
		}
	}
	if (trueCVcount != 0) 
	{
		WriteMain(inDir ,  p , trueCVcount , ioData);
	}

	delete [] p;
}
//��������
void RGISCVCut::WriteMain(const string & inDir ,
						   PointSet* p ,unsigned int trueCVcount , RGISCVCutDataIml * ioData)
{
	UPOINT * writePointBuffer = new UPOINT[65535];

	RStdioFile file;
	int i;
	int breakPoint = 0;
	for(i = 0 ;  i < trueCVcount-1 ; i++)
	{
		if (p[i].lastBlock != p[i + 1].lastBlock)
		{
			//�O��ƈႤ�u���b�N�ɑ����Ă��鍁��t�𔭌�!

			//�Ƃ肠�����A���O�܂ł̐��ʂ��������݂܂��B
			ASSERT((i + 1 + 1 - breakPoint) <= 65535);
			int cvCount = i + 1 + 1 - breakPoint;		//�r���œ_�𕪊�����̂� + 1 ���Ă���.

			for(int n = breakPoint ; n < i+1 ; n++)
			{
				writePointBuffer[ n - breakPoint ] = p[n].po;
			}

			//�ŁA����̓_�̂��߂ɕ����_�����߂܂��B
			UPOINT newPoint = getMidpoint( &p[i].po , &p[i + 1].po );
			writePointBuffer[ n - breakPoint ]  = newPoint;

			//��������.
			file.Open(inDir + num2str(p[breakPoint].lastBlock) , "a+b");
			PointWrite256( &file , p[breakPoint].lastBlock , writePointBuffer , cvCount , ioData);
//			ioData->OnWriteHeader(&file , p[breakPoint].lastBlock , writePointBuffer , cvCount);	//�t���f�[�^�̏�������
//			file.fwrite( &cvCount , sizeof(cvCount) );	
//			PointWrite( &file, writePointBuffer , cvCount);

			//���葤�̓_����݂������_�����߂܂��B
			newPoint = getMidpoint( &p[i + 1].po , &p[i].po );	

			int de = selectBlock( &newPoint );
			ASSERT( de ==  p[i + 1].lastBlock);

			//�ׂ荇��Ȃ��u���b�N�̏ꍇ�͑�3�u���b�N���o�R���Ă���킯�ł���̏���.
			if (! isTexanRude(p[i].lastBlock , p[i + 1].lastBlock) )
			{
				PointSet start,goal;
				start = p[i];
				goal = p[i + 1];
				while(1)
				{
					PointSet therd[2];
					therd[0].po = getTexanRudeMidpoint(&start.po , &goal.po);
					therd[0].lastBlock = selectBlock( &therd[0].po );
					therd[1].po = getMidpoint(&therd[0].po , &goal.po);
					therd[1].lastBlock = selectBlock( &therd[0].po );

					//���R�A������͓���u���b�N�ɂ���킯��...
					ASSERT( therd[0].lastBlock == therd[1].lastBlock );
					//�ŁA���R�A[0] �� p[i] �ׂ͗̃u���b�N��
					ASSERT( isTexanRude(therd[0].lastBlock , start.lastBlock) );
					//�������A[1] �� p[i + 1] �ׂ͗̃u���b�N
//					ASSERT( isTexanRude(therd[1].lastBlock , goal.lastBlock) );

					//�V�����_����������.
					cvCount = 2;		//����Ƃ���Əo��Ƃ����

					writePointBuffer[0] = therd[0].po;
					writePointBuffer[1] = therd[1].po;

					file.Open(inDir + num2str(therd[0].lastBlock) , "a+b");
					PointWrite256( &file , therd[0].lastBlock , writePointBuffer , cvCount , ioData);
//					ioData->OnWriteHeader(&file , therd[0].lastBlock , writePointBuffer , cvCount);	//�t���f�[�^�̏�������
//					file.fwrite( &cvCount , sizeof(cvCount) );
//					PointWrite( &file, writePointBuffer , cvCount);

					//�������A[1] �� p[i + 1] �ׂ͗̃u���b�N�ɂȂ�������.
					if (isTexanRude(therd[1].lastBlock , goal.lastBlock))
					{
						break;
					}
					//�� N �̃u���b�N���o�R���Ă��钷����͂܂������𑱂���.
					start = therd[0];
				}
			}

			//���݂� po �͂����������񂾂̂Ŕj�����܂�.
			p[i].po = newPoint;
			p[i].lastBlock = p[i + 1].lastBlock;
				
			breakPoint = i ;
		}
	}
	//�Ō�̈��
	if ( i != breakPoint)
	{
		int cvCount = i + 1 - breakPoint;
		for(int n = breakPoint ; n < i +1; n++)
		{
			writePointBuffer[ n - breakPoint ] = p[n].po;
		}

		file.Open(inDir + num2str(p[breakPoint].lastBlock) , "a+b");
		PointWrite256( &file , p[breakPoint].lastBlock , writePointBuffer , cvCount , ioData);
	}


	delete [] writePointBuffer;
}

void RGISCVCut::PointWrite256(RStdioFile *iofile , unsigned long inBlock , const UPOINT * inPtArray ,unsigned int inCount, RGISCVCutDataIml * ioData)
{
	UPOINT writePointBuffer[256 + 1];	

	writePointBuffer[0] = inPtArray[0];

	int writePoint = 1;
	for(int i = 1 ; i < inCount ; i++)
	{
		writePointBuffer[writePoint] = inPtArray[i];

		while(1)
		{
			if (abs(writePointBuffer[writePoint - 1].x - writePointBuffer[writePoint].x)<=127 && 
				abs(writePointBuffer[writePoint - 1].y - writePointBuffer[writePoint].y)<=127 )
			{
				break;
			}
			UPOINT t = LineHoseiChar(&writePointBuffer[writePoint - 1] ,  &writePointBuffer[writePoint]);
			ASSERT(abs(writePointBuffer[writePoint - 1].x - t.x)<=127 && 
				   abs(writePointBuffer[writePoint - 1].y - t.y)<=127 );

			//���̒l������ɂ��炵�āA�ۊǂ����l������⊮�����܂�.
			writePointBuffer[writePoint + 1] = writePointBuffer[writePoint];
			//�⊮�����������܂�.
			writePointBuffer[writePoint] = t;
			//��i�߂܂�.
			writePoint++;

			if (writePoint >= 255)
			{
				unsigned char cvCount = 255;
				ioData->OnWriteHeader(iofile , inBlock , writePointBuffer , cvCount);	//�t���f�[�^�̏�������
				iofile->fwrite( &cvCount , sizeof(cvCount) );	//�Ō�܂Ŋ܂߂邽�� + 1 ���Ă���.
				PointWrite( iofile, writePointBuffer , cvCount);

				writePointBuffer[0] = writePointBuffer[writePoint - 1];
				writePointBuffer[1] = writePointBuffer[writePoint];
				writePoint = 1;
			}
		}
		writePoint++;
		if (writePoint >= 255)
		{
			unsigned char cvCount = 255;
			ioData->OnWriteHeader(iofile , inBlock , writePointBuffer , cvCount);	//�t���f�[�^�̏�������
			iofile->fwrite( &cvCount , sizeof(cvCount) );	//�Ō�܂Ŋ܂߂邽�� + 1 ���Ă���.
			PointWrite( iofile, writePointBuffer , cvCount);

			writePointBuffer[0] = writePointBuffer[writePoint - 1];
			writePointBuffer[1] = writePointBuffer[writePoint];
			writePoint = 1;
		}
	}

	//���ʂ��������݂܂��B
	unsigned char cvCount = writePoint ;
	ioData->OnWriteHeader(iofile, inBlock , writePointBuffer , cvCount);	//�t���f�[�^�̏�������
	iofile->fwrite( &cvCount , sizeof(cvCount) );	//�Ō�܂Ŋ܂߂邽�� + 1 ���Ă���.
	PointWrite( iofile, writePointBuffer , cvCount);
}

UPOINT RGISCVCut::LineHoseiChar(const UPOINT * inSrc , const UPOINT * inDest )
{
	UPOINT limit;
	if (inSrc->x > inDest->x)	limit.x = inSrc->x-127;
	else						limit.x = inSrc->x+127;
	if (inSrc->y > inDest->y)	limit.y = inSrc->y-127;
	else						limit.y = inSrc->y+127;

	UPOINT xy;
	if (inDest->y == inSrc->y)
	{
		//y ���ɓ������ꍇ�͕ʃ��[�`��.
		xy.y = inSrc->y;
		xy.x = limit.x;
	}
	else if (inDest->x == inSrc->x)
	{
		//x ���ɓ������ꍇ�͕ʃ��[�`��.
		xy.y = limit.y;
		xy.x = inSrc->x;
	}
	else
	{
		xy.y = (inDest->y - inSrc->y) * ( limit.x - inSrc->x) / (inDest->x - inSrc->x) + inSrc->y;
		xy.x = (limit.y - inSrc->y) * (inDest->x - inSrc->x)  / (inDest->y - inSrc->y) + inSrc->x;

		if (xy.y <= inSrc->y-127 || xy.y >= inSrc->y+127)
		{
			//x���̗p.
			xy.y = limit.y;
		}
		else
		{
			//y���̗p.
			xy.x = limit.x;
		}
	}
	ASSERT( (xy.y >= inSrc->y-127 && xy.y <= inSrc->y+127) && (xy.x >= inSrc->x-127 && xy.x <= inSrc->x+127) );

	return xy;
}

//�_�̏�������
void RGISCVCut::Write(const string & inDir ,const UPOINT * inPoint,RGISCVCutDataIml * ioData)
{
	UPOINT writePointBuffer[1];
	unsigned long  block = selectBlock( inPoint );

	RStdioFile file;
	file.Open(inDir + num2str(block) , "a+b");
	writePointBuffer[0] = *inPoint;
	ioData->OnWriteHeader(&file , block , writePointBuffer , 1);	//�t���f�[�^�̏�������
	PointWrite( &file, writePointBuffer , 1);
}


//�_�̏�������
void RGISCVCut::Test2(const string & inDir ,unsigned long block , int inMeshKankaku)
{
	int oneLine = 65535/inMeshKankaku + 1;
	int feildSize = oneLine * oneLine;
	RMmap mem;


	mem.Create( inDir + num2str(block) , feildSize , OPEN_ALWAYS);
	unsigned char* mapMem = (unsigned char*)mem.getMemory();

		static no = 0;
		no ++;
		string out;
		out.reserve(6553500);
		out += "=======MH=============-\n";
		for (int y = 0 ; y < oneLine ; y++)
		{
			for (int x = 0 ; x < oneLine ; x++)
			{
				char buf[10];
				
				::sprintf(buf, "%2d" , (int)mapMem[x + y * oneLine] );
				out += (string) buf+ " ";
			}
			out += "\n";
		}
		out += "\n";
		RDebug::MemoryToFile("test/mh_log." + num2str(no) , out);
}


//�_�̏�������
void RGISCVCut::PointWrite(RStdioFile *iofile , const UPOINT * inPtArray ,unsigned int inCount)
{
	unsigned short shortSize;
	shortSize = inPtArray[0].x % 65536 ; iofile->fwrite( &shortSize , sizeof(shortSize)  );
	shortSize = inPtArray[0].y % 65536 ; iofile->fwrite( &shortSize , sizeof(shortSize)  );

	char charSize;
	for(int i = 1; i < inCount ; i++)
	{
		ASSERT( abs((int)inPtArray[i].x - (int)inPtArray[i - 1].x) <= 127 );
		ASSERT( abs((int)inPtArray[i].y - (int)inPtArray[i - 1].y) <= 127 );

		charSize = (int)inPtArray[i].x - (int)inPtArray[i - 1].x;
		iofile->fwrite( &charSize , sizeof(charSize)  );
		charSize = (int)inPtArray[i].y - (int)inPtArray[i - 1].y;
		iofile->fwrite( &charSize , sizeof(charSize)  );
	}
}


//Src ���� Dest �֐����Ƃ��ɋ��E���ɂԂ���|�C���g�̎擾.
UPOINT RGISCVCut::getMidpoint(const UPOINT * inSrc , const UPOINT * inDest) const
{
	BPOINT limit; 
	if (inSrc->x > inDest->x)	limit.x = 0;
	else						limit.x = 65535;
	if (inSrc->y > inDest->y)	limit.y = 0;
	else						limit.y = 65535;

	UPOINT scalUP;

	scalUP.x = (inSrc->x / 65536 * 65536);
	scalUP.y = (inSrc->y / 65536 * 65536);

	BPOINT src , dest;
	src.x = inSrc->x - scalUP.x;
	src.y = inSrc->y - scalUP.y;
	dest.x = inDest->x - scalUP.x;
	dest.y = inDest->y - scalUP.y;

	long x,y;
	if (inDest->y == inSrc->y)
	{
		//y ���ɓ������ꍇ�͕ʃ��[�`��.
		y = src.y;
		x = limit.x;
	}
	else if (inDest->x == inSrc->x)
	{
		//x ���ɓ������ꍇ�͕ʃ��[�`��.
		y = limit.y;
		x = src.x;
	}
	else
	{
		y = ((double)(dest.y - src.y) / (dest.x - src.x)) * (limit.x - src.x) + src.y;
//		x = (limit.y - src.y) / ((double)(dest.y - src.y) / (dest.x - src.x))  + src.x;
		x = (limit.y - src.y) * (double)(dest.x - src.x) / (dest.y - src.y) + src.x;

		if (y < 0 || y >= 65536)
		{
			//x���̗p.
			y = limit.y;
		}
		else
		{
			//y���̗p.
			x = limit.x;
		}
		ASSERT( (y >= 0 && y < 65536) && (x >= 0 && x < 65536) );
	}
	UPOINT ret;
	ret.x = x + scalUP.x;
	ret.y = y + scalUP.y;

	return ret;
}

//Src ���� Dest �֐����Ƃ��ɋ��E���̎��ɂ���u���b�N�̎擾
UPOINT RGISCVCut::getTexanRudeMidpoint(const UPOINT * inSrc , const UPOINT * inDest) const
{
	BPOINT limit; 
	if (inSrc->x > inDest->x)	limit.x = -1;
	else						limit.x = 65536;
	if (inSrc->y > inDest->y)	limit.y = -1;
	else						limit.y = 65536;

	UPOINT scalUP;
	scalUP.x = (inSrc->x / 65536 * 65536);
	scalUP.y = (inSrc->y / 65536 * 65536);

	BPOINT src , dest;
	src.x = inSrc->x - scalUP.x;
	src.y = inSrc->y - scalUP.y;
	dest.x = inDest->x - scalUP.x;
	dest.y = inDest->y - scalUP.y;

	long x,y;
	if (inDest->y == inSrc->y)
	{
		//y ���ɓ������ꍇ�͕ʃ��[�`��.
		y = src.y;
		x = limit.x;
	}
	else if (inDest->x == inSrc->x)
	{
		//x ���ɓ������ꍇ�͕ʃ��[�`��.
		y = limit.y;
		x = src.x;
	}
	else
	{
		y = ((double)(dest.y - src.y) / (dest.x - src.x)) * (limit.x - src.x) + src.y;
//		x = (limit.y - src.y) / ((double)(dest.y - src.y) / (dest.x - src.x))  + src.x;
		x = (limit.y - src.y) * (double)(dest.x - src.x) / (dest.y - src.y) + src.x;

		if (y < 0 || y >= 65536)
		{
			//x���̗p.
			y = limit.y;
		}
		else
		{
			//y���̗p.
			x = limit.x;
		}
		ASSERT( (y >= -1 && y < 65537) && (x >= -1 && x < 65537) );
	}
	UPOINT ret;
	ret.x = x + scalUP.x;
	ret.y = y + scalUP.y;

	return ret;
}

//�u���b�N�ɕ��f
unsigned long RGISCVCut::selectBlock(const UPOINT * inPT) 
{
	return (inPT->y / 65536) * (NEXT_LINE_BLOCK) + (inPT->x /65536);
}

//�ׂ荇���u���b�N��?
bool RGISCVCut::isTexanRude(unsigned long inA , unsigned long  inB) const
{
	if (inA-1 == inB) return true;
	if (inA+1 == inB) return true;
	if (inA-NEXT_LINE_BLOCK == inB) return true;
	if (inA+NEXT_LINE_BLOCK == inB) return true;

	return false;
}



UPOINT* RGISCVCut::DebugRead(const string & inDir ,unsigned short inBlock ,const string & inData  )
{
	RStdioFile file;
	file.Open(inDir + num2str(inBlock) , "r+b");

	//�t���f�[�^�̓ǂݏo��.
	char buffer[MAX_PATH];
	file.fread(buffer , inData.size() + 1);

	unsigned short cvCount;
	file.fread(&cvCount , sizeof(cvCount) );

	unsigned short sr;
	UPOINT * storePoint = new UPOINT[cvCount];

	for(int i = 0 ; i < cvCount ; i ++)
	{
		ASSERT(file.IsEof() == false );
		file.fread( &sr , sizeof(sr) );	storePoint[i].x = sr;
		ASSERT(file.IsEof() == false );
		file.fread( &sr , sizeof(sr) );	storePoint[i].y = sr;
	}

	return storePoint;
}

void RGISCVCut::test()
{

	{
		RGISCVCut cv;
		UPOINT src  = {50327201 , 4294967197};
		UPOINT dest = {50327791 , 4282104863};

		int xdiff = abs( dest.x - src.x );
		int ydiff = abs( dest.y - src.y );

		unsigned long x = dest.x ;
		unsigned long y = dest.y ;

		UPOINT xy = cv.LineHoseiChar(&src , &dest);
	}

	return ;
	{
		UPOINT src , dest;
		RGISCVCut cv;

		src.x = 6357075;	src.y = 252968753;
		dest.x = 6356988;		dest.y = 252968970;
		UPOINT po = cv.getMidpoint(&src , &dest);

		ASSERT(po.x == 6356992 && po.y == 252968959);
	}
	{
		UPOINT src , dest;
		RGISCVCut cv;

		src.x = 3275080;	src.y = 1012220;
		dest.x = 3282260;		dest.y = 1012220;
		int a = cv.selectBlock(&src);
		int b = cv.selectBlock(&dest);

		UPOINT po = cv.getMidpoint(&src , &dest);


		ASSERT(po.x == 3276799 && po.y == 1012220);
	}
	{
		UPOINT src , dest;
		RGISCVCut cv;

		src.x = 5504870;	src.y = 1185930;
		dest.x = 5469540;		dest.y = 1114200;
		int a = cv.selectBlock(&src);
		int b = cv.selectBlock(&dest);

		UPOINT po = cv.getMidpoint(&src , &dest);


		ASSERT(po.x == 5501775 && po.y == 1179648);
	}
	{
		UPOINT src , dest;
		RGISCVCut cv;

		src.x = 1834240;	src.y = 166750;
		dest.x = 1843600;		dest.y = 151950;
		int a = cv.selectBlock(&src);
		int b = cv.selectBlock(&dest);

		UPOINT po = cv.getMidpoint(&src , &dest);


		ASSERT(po.x == 1835007 && po.y == 165537);
	}
	{
		UPOINT src , dest;
		RGISCVCut cv;

		src.x = 100000;	src.y = 100;
		dest.x = 10000;		dest.y = 100;
		UPOINT po = cv.getMidpoint(&src , &dest);

		ASSERT(po.x == 65536 && po.y == 100);
	}
	{
		RGISCVCut::PointSet src , dest;
		RGISCVCut cv;

		src.po.x = 5243183;		src.po.y = 252313284;
		dest.po.x = 5242777;	dest.po.y = 252313749;
		src.lastBlock = cv.selectBlock(&src.po);
		dest.lastBlock = cv.selectBlock(&dest.po);

		UPOINT po = cv.getMidpoint(&src.po , &dest.po);
		ASSERT(po.x == 5242907 && po.y == 252313599);

		//src �ƕ����_�͓���u���b�N.
		ASSERT(src.lastBlock == cv.selectBlock(&po) );
		//src �� dest �͂��������ׂ荇��Ȃ�
		ASSERT( ! cv.isTexanRude(src.lastBlock , dest.lastBlock) );

		RGISCVCut::PointSet therd[2];
		therd[0].po = cv.getTexanRudeMidpoint(&src.po , &dest.po);
		therd[0].lastBlock = cv.selectBlock( &therd[0].po );
		therd[1].po = cv.getMidpoint(&therd[0].po , &dest.po);
		therd[1].lastBlock = cv.selectBlock( &therd[0].po );

		//���R�A������͓���u���b�N�ɂ���킯��...
		ASSERT( therd[0].lastBlock == therd[1].lastBlock );
		//�ŁA���R�A[0] �� p[i] �ׂ͗̃u���b�N��
		ASSERT( cv.isTexanRude(therd[0].lastBlock , src.lastBlock) );
		//�������A[1] �� p[i + 1] �ׂ͗̃u���b�N
		ASSERT( cv.isTexanRude(therd[1].lastBlock , dest.lastBlock) );
		//Y���ɓ˂�������̂�.
		ASSERT(therd[0].po.y == 252313599 + 1);
	}

	class RGISCVCutDataImlDummy  : public RGISCVCutDataIml
	{
	public:
		string WriteData;

		virtual void OnWriteHeader(RStdioFile * ioFile, unsigned long inBlock,const UPOINT * writePointArray ,unsigned int inPointCount)
		{
			ioFile->fwrite(WriteData.c_str() , WriteData.size() + 1);
		}
	};

	try
	{
		RGISCVCut cv;
		RGISSlp slp;
		RGISSlm slm;
		GISINTArray array;
		slp.Create("test/cvtest.slp",&slm);
		array.Create("1,2,3,4,5");
		RDiskUtil::Delete("test/cv");
		RDiskUtil::MakeDirectory("test/cv");

		RGISCVCutDataImlDummy dummyData;
		dummyData.WriteData = "data";

		cv.Write( "test/cv/" , &slp , &array , &dummyData );

		char buffer[5];
		UPOINT* pointes = RGISCVCut::DebugRead("test/cv/" ,0 , dummyData.WriteData);
//		ASSERT(data == buffer);
		ASSERT(pointes[0].x == 10);
		ASSERT(pointes[0].y == 100);
		ASSERT(pointes[1].x == 100);
		ASSERT(pointes[1].y == 100);
		ASSERT(pointes[2].x == 1000);
		ASSERT(pointes[2].y == 100);
		ASSERT(pointes[3].x == 10000);
		ASSERT(pointes[3].y == 100);
		ASSERT(pointes[4].x == 65535);
		ASSERT(pointes[4].y == 100);
		delete [] pointes;

		pointes = RGISCVCut::DebugRead("test/cv/" ,1 , dummyData.WriteData );
//		ASSERT(data == buffer);
		ASSERT(pointes[0].x == 0);
		ASSERT(pointes[0].y == 100);
		ASSERT(pointes[1].x == 34464);
		ASSERT(pointes[1].y == 100);
		delete [] pointes;
	}
	catch(RException e)
	{
		ASSERT(0);
	}
}



//���b�V���Ԋu
const int MESH_INTERVAL	=	256;
//�ꃉ�C���ɂ������b�V�������邩?
const int MESH_LINE		=	(65536/MESH_INTERVAL);

RGISCVCutMesh::RGISCVCutMesh()
{
	this->LastBlock = ULONG_MAX;
}

RGISCVCutMesh::~RGISCVCutMesh()
{
	Mem.Flush(MESH_LINE  * MESH_LINE);
}

//�_�̏�������
void RGISCVCutMesh::Write(const string & inDir  , const UPOINT & inPoint,unsigned char inLevel)
{
	ASSERT( (65536%MESH_INTERVAL) == 0);


	unsigned long  block = RGISCVCut::selectBlock( &inPoint );

	//�O��ƈႤ�u���b�N��������A
	//���݂̃u���b�N�̓��e���Z�[�u���A�V�����u���b�N���J��.
	if (block != this->LastBlock)
	{
		if (this->LastBlock != ULONG_MAX)
		{
			Mem.Flush(MESH_LINE  * MESH_LINE);
			Mem.Release();
		}

		Mem.Create( inDir + num2str(block) , MESH_LINE * MESH_LINE , OPEN_ALWAYS);
		this->LastBlock = block;
	}

	unsigned char* mapMem = (unsigned char*)Mem.getMemory();

	int x = inPoint.x % 65536 / MESH_INTERVAL;
	int y = inPoint.y % 65536 / MESH_INTERVAL;
	int targetPos =	( x) + ( y  * MESH_LINE );
	ASSERT(targetPos < (MESH_LINE * MESH_LINE) );

	mapMem[targetPos] = inLevel;

	//�h��̂������Ȃ��悤�ɁA����̗̈�œh���Ă��Ȃ��Ƃ��낪��������h��.
	if (x > 0 )
	{
		targetPos = ( (x-1) ) + ( y * MESH_LINE );
		if (mapMem[targetPos] == 0) mapMem[targetPos] = inLevel;
	}
	if (x < (MESH_LINE-1) )
	{
		targetPos = ( (x+1) ) + ( y * MESH_LINE );
		if (mapMem[targetPos] == 0) mapMem[targetPos] = inLevel;
	}
	if (y > 0 )
	{
		targetPos = ( x ) + ( (y-1) * MESH_LINE );
		if (mapMem[targetPos] == 0) mapMem[targetPos] = inLevel;
	}
	if (y < (MESH_LINE-1) )
	{
		targetPos = ( x ) + ( (y+1) * MESH_LINE );
		if (mapMem[targetPos] == 0) mapMem[targetPos] = inLevel;
	}

	
}
