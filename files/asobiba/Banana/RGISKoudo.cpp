//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

#include "RGISKoudo.h"
#include "RFileUtil.h"
#include "RStdioFile.h"
#include "RGIS.h"
#include "RDiskUtil.h"
#include "RGISCVCut.h"
#include "RGISNameDatabaseDisk.h"
#include "RGISCvLight.h"
#include "RDebug.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////




RGISKoudo::RGISKoudo()
{
	Count = 0;
	Points= NULL;

}

RGISKoudo::~RGISKoudo()
{
	Delete();
}


void RGISKoudo::Delete()
{
	if (this->Points != NULL)
	{
		delete [] this->Points;
		this->Points = NULL;
	}
}
typedef vector<int>	TestFiledToVectorData;

typedef vector<POINT>	TESTFILED_POINTES;
class RGISKoudoTestFiled
{
public:
	RGISKoudoTestFiled()
	{
		this->TestFiled = NULL;
		this->TestFiledRealNumber = NULL;
	}
	~RGISKoudoTestFiled()
	{
		delete [] this->TestFiled;
		delete [] this->TestFiledRealNumber;
	}
	//�e�X�g�̈�̊m�ۂƏ�����.
	void Create(const LRECT & inFiled ,int inMeshKankaku)
	{
		this->Filed = inFiled;
		this->MeshKankaku = inMeshKankaku;

		//�e�X�g�̈�̊m�ۂƏ�����.
		this->Width = (this->Filed.right - this->Filed.left) / inMeshKankaku + 1;
		this->Hegiht = (this->Filed.bottom - this->Filed.top) / inMeshKankaku + 1;
		this->TestFiled = new unsigned char[this->Width * this->Hegiht];
		memset(this->TestFiled ,0, this->Width * this->Hegiht * sizeof(unsigned char) );

		//slp�ł̈ʒu���Ă��邩.
		this->TestFiledRealNumber = new long[this->Width * this->Hegiht];
		memset(this->TestFiledRealNumber , 0 , this->Width * this->Hegiht * sizeof(unsigned long) );
	}
	//�n�_�̒ǉ�.
	void setPos(const UPOINT & xy,long inSlpNumer , unsigned char inLevel)
	{
		ASSERT( inLevel <= 20);
		ASSERT(xy.x >= this->Filed.left);
		ASSERT(xy.y >= this->Filed.top);
		ASSERT(xy.x <= this->Filed.right);
		ASSERT(xy.y <= this->Filed.bottom);

		int x = (xy.x - this->Filed.left) / this->MeshKankaku;
		int y = (xy.y - this->Filed.top) / this->MeshKankaku;

		int i = calePos(x,y);
		ASSERT(this->TestFiled[i] == 0);
		this->TestFiled[i] = inLevel ;
		this->TestFiledRealNumber[i] = inSlpNumer;
	}
	//�����̎擾
	int getWidth() const
	{
		return this->Width;
	}
	//�c���̎擾
	int getHegiht() const
	{
		return this->Hegiht;
	}
	//�L���ȓ_��T���܂�.
	POINT EnablePoint() const
	{
		POINT xy;
		for(xy.y = 0; xy.y < this->Hegiht ; xy.y++)
		{
			for(xy.x = 0; xy.x < this->Width ; xy.x++)
			{
				if ( this->getPosEasy(xy.x , xy.y) >= 10) return xy;
			}
		}

		//�����Ȃ��ł��B
		xy.x = -1;
		xy.y = -1;

		return xy;
	}


private:
	int calePos(int x,int y) const
	{
		ASSERT(x < Width && x >= 0);
		ASSERT(y < Hegiht && y >= 0);
		return y * Width + x;
	}
	POINT revCalePos(int xy) const
	{
		POINT ret;
		ret.y = xy / Width;
		ret.x = xy % Width;
		ASSERT(ret.x < Width && ret.x >= 0);
		ASSERT(ret.y < Hegiht && ret.y >= 0);
		return ret;
	}

	//�n�_�̐ݒ�
	void setPosEasy(int x,int y,unsigned char inLevel) 
	{
		ASSERT(x < Width && x >= 0);
		ASSERT(y < Hegiht && y >= 0);
		this->TestFiled[calePos(x,y)] = inLevel;
	}
	//�n�_�̎擾
	unsigned char getPosEasy(int x,int y) const
	{
//		ASSERT(x < Width && x >= 0);
//		ASSERT(y < Hegiht && y >= 0);
		if ( !(x < Width && x >= 0) ) return 0;
		if ( !(y < Hegiht && y >= 0) ) return 0;

		ASSERT( this->TestFiled[calePos(x,y)] <= 20);

		return this->TestFiled[calePos(x,y)] ;
	}

	//�n�_�̎擾
	bool isNextRoute(int x,int y,unsigned char inLevel) const
	{
		if ( getPosEasy(x,y) != inLevel) return false;

		unsigned char l;
		l = getPosEasy(x + 1 ,y) ;
		if ( l != 1 && l != inLevel) return true;
		l = getPosEasy(x - 1 ,y);
		if ( l != 1 && l != inLevel) return true;
		l = getPosEasy(x ,y + 1);
		if ( l != 1 && l != inLevel) return true;
		l = getPosEasy(x ,y - 1);
		if ( l != 1 && l != inLevel) return true;

		return false;
	}
	void PaintRev(int inX, int inY , unsigned char inLevel , int inFlg)
	{
	//	if (inX < 0 || inY < 0 || inX >= Width || inY >= Height) return ;

		int l ,r , x;
		//���ɂ�����Ƃ���܂ōs���܂��B
		for(l = inX ; l >= 0 ; l-- )
		{
			if ( getPosEasy( l ,  inY) != inLevel) break;
		}
		l++;

		//�E�ɂ�����Ƃ���܂ōs���܂��B
		for(r = inX ; r < Width ; r++ )
		{
			if ( getPosEasy( r ,  inY) != inLevel) break;
		}
//		r--;

		//OK. ���̊Ԃ�h��Ԃ��܂��B
		for(int n = l ; n < r ; n++)
		{
			setPosEasy(n , inY , 0);
		}
//		::MoveToEx(PoolDC , l , inY , NULL);
//		::LineTo(PoolDC , r , inY );

		//�ŁA���̊Ԃɂ�������̂ЂƂ�̃s�N�Z���œh��Ԃ���������񂾂�����h��Ԃ�.
//		if (inFlg & 1)
		{
			if (inY > 0) 
			{
				int y = inY - 1;
				for(x = l ; x < r ; x++ )
				{
					if ( getPosEasy( x ,  y) == inLevel ) 
						PaintRev(x,y , inLevel ,1 );
				}
			}
		}
		//�܂��A���̊Ԃɂ�������̂ЂƂ��̃s�N�Z���œh��Ԃ���������񂾂�����h��Ԃ�.
//		if (inFlg & 2)
		{
			if (inY < Hegiht) 
			{
				int y = inY + 1;
				for(x = l ; x < r ; x++ )
				{
					if ( getPosEasy( x ,  y) == inLevel ) 
						PaintRev(x,y , inLevel ,2 );
				}
			}
		}
	}
	void ComplementVec(TestFiledToVectorData * ioFailedPointes)
	{
		if (ioFailedPointes->size() < 2) return ;

		int start = ioFailedPointes->at(0);
		int end = ioFailedPointes->at(ioFailedPointes->size() - 1);

		//���ǂ̓x�N�g�������.
		ioFailedPointes->push_back(start);

		//start�n�_��x�����傫���Ƃ���ŏI�����}���Ă��܂��ƁA
		//�t���v���ɂȂ�̂ŁA�C������.
		POINT s = revCalePos(start);
		POINT e = revCalePos(end);
		if ( s.x < e.x )
		{
			//�t���ɂ���.
			reverse( ioFailedPointes->begin() , ioFailedPointes->end() );
		}
	}

public:
	void Dump()
	{
		static no = 0;
		no ++;
		string out;
		out.reserve(6553500);
		out += "=======MH=============-\n";
		for (int y = 0 ; y < Hegiht ; y++)
		{
			for (int x = 0 ; x < Width ; x++)
			{
				char buf[10];
				
				::sprintf(buf, "%2d" , (int)this->getPosEasy(x , y) );
				out += (string) buf+ " ";
			}
			out += "\n";
		}
		out += "\n";
		RDebug::MemoryToFile("test/mh_log." + num2str(no) , out);
	}


	void Vec2CV(GISINTArray * outArray ,const TestFiledToVectorData * inFailedPointes )
	{
		TestFiledToVectorData::const_iterator i = inFailedPointes->begin();
		TestFiledToVectorData::const_iterator e = inFailedPointes->end();

		int size = inFailedPointes->size();
		unsigned long * ulongs = new unsigned long[ size ] ;
		for(int n = 0 ; i != e; i++, n++)
		{
			ulongs[n] = this->TestFiledRealNumber[*i];
		}
		outArray->Create(ulongs , size);

		delete [] ulongs;
	}
	//��������x�N�g���f�[�^�𒊏o���܂�.
	unsigned char toVector(const POINT & inStart ,  TestFiledToVectorData * outFailedPointes)
	{
		struct MovePoint
		{
			int x,y,houkou;
		};

		static MovePoint MovePoint0[8] = 
		{
			{ -1 , -1 ,	0} ,	//����
			{ 0  , -1 ,	0} ,	//��
			{ 1  , -1 ,	0} ,	//�E��
			{ 1  , 0  ,	1} ,	//�E
			{ 1  , 1  ,	2} ,	//�E��
			{ 0  , 1  ,	2} ,	//��
			{ -1 , 1  ,	2} ,	//����
			{ -1 , 0  ,	3} ,	//��
		};
		static MovePoint MovePoint1[8] = 
		{
			{ 1  , 0  ,	1} ,	//�E
			{ 1  , 1  ,	2} ,	//�E��
			{ 0  , 1  ,	2} ,	//��
			{ -1 , 1  ,	2} ,	//����
			{ -1 , 0  ,	3} ,	//��
			{ -1 , -1 ,	0} ,	//����
			{ 0  , -1 ,	0} ,	//��
			{ 1  , -1 ,	0} ,	//�E��
		};
		static MovePoint MovePoint2[8] = 
		{
			{ 1  , 1  ,	2} ,	//�E��
			{ 0  , 1  ,	2} ,	//��
			{ -1 , 1  ,	2} ,	//����
			{ -1 , 0  ,	3} ,	//��
			{ -1 , -1 ,	0} ,	//����
			{ 0  , -1 ,	0} ,	//��
			{ 1  , -1 ,	0} ,	//�E��
			{ 1  , 0  ,	1} ,	//�E
		};
		static MovePoint MovePoint3[8] = 
		{
			{ -1 , 1  ,	2} ,	//����
			{ -1 , 0  ,	3} ,	//��
			{ -1 , -1 ,	0} ,	//����
			{ 0  , -1 ,	0} ,	//��
			{ 1  , -1 ,	0} ,	//�E��
			{ 1  , 0  ,	1} ,	//�E
			{ 1  , 1  ,	2} ,	//�E��
			{ 0  , 1  ,	2} ,	//��
		};
		static MovePoint* MovePointAll[4];
		MovePointAll[0] = MovePoint0;
		MovePointAll[1] = MovePoint1;
		MovePointAll[2] = MovePoint2;
		MovePointAll[3] = MovePoint3;

		outFailedPointes->clear();
		//���v����8���ʂ𒲂ׂ܂�.
		POINT p = inStart;
		unsigned char mySelf = this->getPosEasy(p.x  , p.y);
		outFailedPointes->push_back(calePos(p.x , p.y ) );

		//�܏��H���甲���o����?
		bool blindAlleyBack = false;
		int backCount ;


		int LastHoukou = 0;

		Dump();
		while(1)
		{
			//�X�^�[�g�n�_�ɖ߂��Ă����炨���܂�.
			if (outFailedPointes->size() > 1 && p.x == inStart.x && p.y == inStart.y)
			{
				outFailedPointes->push_back(calePos(p.x , p.y ) );
				break;
			}

			const MovePoint* useSearchTable = MovePointAll[LastHoukou];

			for(int i = 0; i < TABLE_COUNT_OF(MovePoint0) ;i ++)
			{
				const MovePoint * sp = &useSearchTable[i];
				if ( this->isNextRoute(p.x + sp->x , p.y + sp->y,mySelf) )
				{
					LastHoukou = sp->houkou;
					p.x = p.x + sp->x;	p.y = p.y + sp->y;
					this->setPosEasy(p.x  , p.y ,1) ;
					outFailedPointes->push_back(calePos(p.x , p.y ) );
					blindAlleyBack = false;
					break;
					
				}
			}
			if (i != TABLE_COUNT_OF(MovePoint0)) continue;
			//�ׂ��Ђ��̂悤�ȂƂ���ɖ�������?
			if (!blindAlleyBack)
			{
				backCount = outFailedPointes->size();
				blindAlleyBack = true;
			}
			//�X�^�[�g�n�_�܂Ŗ߂��Ă��_���ȏꍇ
			if (backCount <= 1)
			{
				ComplementVec(outFailedPointes);
				break;
			}
			backCount --;
			p = revCalePos( outFailedPointes->at(backCount) );
		}
		Dump();
		//�`�F�b�N�ɓ���Ă����l�����̒l�ɖ߂��܂�.
		{
			TestFiledToVectorData::iterator i = outFailedPointes->begin();
			TestFiledToVectorData::iterator e = outFailedPointes->end();
			for( ; i != e; i++)
			{
				this->TestFiled[*i] = mySelf;
			}
		}
		Dump();
		//�����F�̂Ƃ���� 0 �œh��Ԃ��܂�.
		PaintRev(inStart.x , inStart.y ,mySelf ,1 + 2 );
		Dump();

		return mySelf;
	}


private:
	LRECT Filed;
	int MeshKankaku;

	int Width;
	int Hegiht;

	unsigned char * TestFiled;
	long * TestFiledRealNumber;
};

void RGISKoudo::Create(const string & inSlpFilename,const string & inMHFilename,const RGISSlm * inMeshSlm) throw(RException)
{
	ASSERT(Points == NULL);

	//���b�V���� slp�̓ǂݍ���.
	MeshSlp.Create(inSlpFilename , inMeshSlm);
	MeshSlp.ConvertDisplayScal();

	//�l���̎擾.
	LRECT filed = MeshSlp.getRect();

	//�e�X�g�t�B�[���h�̍쐬.
	RGISKoudoTestFiled testFiled;
	testFiled.Create(filed , 200);


//	this->Count = RGIS25000::GetMaxID(inMHFilename , "MH(ID{MH%d})");
//	if (this->Count <= 0) return ;


	RStdioFile file;
	file.Open(inMHFilename , "rb");

	FILE * fp = file.getHandle();

	string buffer;
	int i;


	for( i = 0  ; !file.IsEof(); i++)
	{
		buffer = file.ReadLine();
		if (RConv::chop(buffer) == "") break;

		int id;
		int rsize = sscanf(buffer.c_str() , "MH(ID{MH%6d})",	&id );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		id = id -1;	//1����n�܂邽��.
		if (id < 0)
		{
			throw RException(EXCEPTIONTRACE + inMHFilename + "�� " + num2str(i) + "�s�ڂ�ID���ςł�!");
		}
		int dummy;
		unsigned int hk;
		unsigned int pm;
		const char * hkOption = strstr(buffer.c_str() , "HK{");
		rsize = sscanf(hkOption  , "HK{%d}",	 &hk  );
		if (rsize != 1) throw RException(EXCEPTIONTRACE + PrintFormat("sscanf�����s���܂��� %s %d �\�[�X[%s]" ,inFilename.c_str() , rsize , buffer.c_str()  ) );

		const char * ptStart = strstr(buffer.c_str() , "PT(");
		if (ptStart == NULL)
		{
			throw RException(inMHFilename + "�� " + num2str(i) + "�s�ڂ̃f�[�^��CV�`�����ӂł�!");
		}
		rsize = sscanf(ptStart , "PT(ID{PM%d}){%d}",	&dummy , &pm);
		if (rsize != 2) continue;	//PT���Ȃ��f�[�^�͖���

		//�e�X�g�t�B�[���h�ɒǉ�.
		testFiled.setPos( MeshSlp.getPoint(pm - 1) , pm , selectKoudoLevel(hk) );
	}

	//�e�X�g�t�B�[���h����x�N�g���̍쐬.
	typedef vector<KoudoData> TestFiledToVectorDataManager;
	TestFiledToVectorDataManager VecManager;
	while(1)
	{
		POINT point = testFiled.EnablePoint();
		if (point.x == -1 && point.y == -1) break;

		TRACE("fill start %d:%d\n" , point.x , point.y);

		KoudoData kd;
		TestFiledToVectorData  vec;
		kd.Level = testFiled.toVector(point , &vec);

		//�Œ�3�s�[�X����Ȃ�Ȃ��悤�ȁA�Ƃ��͔̂r��.
		if (vec.size() > 2)
		{
			testFiled.Vec2CV(&kd.CV , &vec);
			ASSERT( MeshSlp.TestCV(&kd.CV) == true );
			VecManager.push_back(kd);

		}
	}

	this->Count = VecManager.size();
	this->Points = new KoudoData[this->Count];
	//�x�N�g���f�[�^�̊i�[.
	for( i = 0  ;i < this->Count ; i++)
	{
		this->Points[i].CV = VecManager[i].CV;
		this->Points[i].Level = VecManager[i].Level;
		ASSERT( MeshSlp.TestCV(&this->Points[i].CV) == true )
	}


}

//�W�������x���ɕϊ�.
unsigned char RGISKoudo::selectKoudoLevel(int inKM)
{
//	return 10;
	if (inKM <  200) return 10;
	if (inKM <  400) return 11;
	if (inKM <  600) return 12;
	if (inKM <  800) return 13;
	if (inKM < 1000) return 14;
	if (inKM < 1500) return 15;
	if (inKM < 2000) return 16;
	if (inKM < 2500) return 17;
	if (inKM < 3000) return 18;
	if (inKM < 4000) return 19;
	if (inKM < 5000) return 20;
	return 21;
}

void RGISKoudo::Draw(const RGISDraw * inDraw) const
{
}

COLORREF RGISKoudo::selectColor(GISType* inShubetsu) const
{
	return 0;
}
COLORREF RGISKoudo::selectSize(GISType* inShubetsu) const
{
	return 0;
}

void RGISKoudo::SmallPack(const string & inDir )  const throw(RException)
{
	string dir = inDir + "MH/";
	RDiskUtil::MakeDirectory(dir);

	string namedir = inDir + "NX/";
	RDiskUtil::MakeDirectory(namedir);

	//RGISCVCut.Write�̒�����Ăт���钸�_�f�[�^�̃w�b�_�[���������ރ��[�`��.
	class RGISCVCutKoudoData  : public RGISCVCutDataIml
	{
	private:

	public:

		unsigned char Level;				//���x���x��

		RGISCVCutKoudoData()
		{
		}

		//���ꂪ�Ăяo�����.
		virtual void OnWriteHeader(RStdioFile * ioFile, unsigned long inBlock,const UPOINT * writePointArray ,unsigned int inPointCount)
		{
			//
			ioFile->fwrite(&Level , sizeof(Level));
		}
	};

	RGISCVCutKoudoData	data;

	RGISCvLight light;
	RGISCVCut cut;
	for(int n = 0 ; n < Count ; n++)
	{
		const KoudoData* p = &Points[n];
		data.Level =  p->Level;

		GISINTArray lightCV;
		if ( light.DoLight( &MeshSlp , &p->CV , &lightCV) )
		{
			cut.Write(dir , &MeshSlp , &lightCV , &data );
		}
		else
		{
			cut.Write(dir , &MeshSlp , &p->CV , &data );
		}
	}
}

JoinTempFormatScript RGISKoudo::getPackInfo() const
{
	JoinTempFormatScript ret;
	ret.TempFileDirectoryName = "MH";
	ret.BlockStartSimbol = 'H';
	ret.DataSize = sizeof(unsigned char);	//1	=	1
	ret.DataSizeSub = 0;
	ret.CVType = CVTypeMultiPoint;
	ret.WithNode = false;

	return ret;
}


void RGISKoudo::test()
{
/*
	RGISSlm slm;
	RGISKoudo mh;
	slm.Create("test/01101.slm" , 1);
	mh.Create("test/test.mh.slp","test/test.mh",&slm);
*/
}
