#include <./Lim3D/3DRead/rti_Lim3DRoku.h>
#include <stdio.h>

/***********************************************************/
//�R���X�g���N�^
//����							�Ȃ�
//�߂�l						�Ȃ�
/***********************************************************/
TReadRoku::TReadRoku()
{
	//�}�e���A��.
	m_Material			= NULL;
	//�}�e���A���̐�.
	m_MaterialNum		= 0;
}

/***********************************************************/
//�f�X�g���N�^
//����							�Ȃ�
//�߂�l						�Ȃ�
/***********************************************************/
TReadRoku::~TReadRoku()
{
	Init();
}

/***********************************************************/
//������.
//����							�Ȃ�
//�߂�l						�Ȃ�
/***********************************************************/
void TReadRoku::Init()
{
	//�}�e���A��.
	if (m_Material) delete [] m_Material;
	m_Material			= NULL;
	//�}�e���A���̐�.
	m_MaterialNum		= 0;
	//�_�R���N�V�����̔j��.
	m_VecCaretaker.Clear();
	//���R���N�V�����̔j��.
	m_LineCaretaker.Clear();
	//�ʃR���N�V�����̔j��.
	m_FaceCaretaker.Clear();
}

/***********************************************************/
//�t�@�C���̓ǂݍ���.
//����							inFileName	�ǂݍ��ރt�@�C����.
//�߂�l						���� true
/***********************************************************/
bool TReadRoku::Open(const char *inFileName)
{
	Init();

	FILE *		theFp;
	theFp = fopen(inFileName , "rb");
	if (theFp == NULL)	return false;

	//�t�@�C���T�C�Y���擾.
	::fseek( theFp , 0 , SEEK_END);
	m_FileDataLen = ::ftell(theFp);
	::fseek( theFp , 0 , SEEK_SET);

	//�̈�̊m��.
	char *theFileData = new char[m_FileDataLen];

	//�̈�Ƀf�[�^�𗬂�����.
	::fread( theFileData , sizeof(char) , m_FileDataLen , theFp );

	::fclose( theFp );

	char* theReadPointer = theFileData;
		
	//�w�b�_�`�F�b�N.
	if ( ! CheckHeader(&theReadPointer) )
	{//���������f�[�^�Ȃ̂ŁA�I��.
		Init();
		return false;
	}
	//�_�f�[�^�̓ǂݍ���
	if ( ! GetPointes(&theReadPointer) )
	{//���������f�[�^�Ȃ̂ŁA�I��.
		Init();
		return false;
	}
	//���f�[�^�̓ǂݍ���.
	if ( ! GetLines(&theReadPointer) )
	{//���������f�[�^�Ȃ̂ŁA�I��.
		Init();
		return false;
	}
	//�ʃf�[�^�̓ǂݍ���.
	if ( ! GetFaces(&theReadPointer) )
	{//���������f�[�^�Ȃ̂ŁA�I��.
		Init();
		return false;
	}
	//�}�e���A���f�[�^�̓ǂݍ���.
	if ( ! GetMaterial(&theReadPointer) )
	{//���������f�[�^�Ȃ̂ŁA�I��.
		Init();
		return false;
	}
	//�Ȃ񂩁A�����t�@�C���̒��g����܂���.
	delete theFileData;

	return true;
}

/***********************************************************/
//�N���[�Y.
//����							�Ȃ�
//�߂�l						���� true
/***********************************************************/
bool TReadRoku::Close()
{
	Init();
	return true;
}

/***********************************************************/
//���f���̎擾.
//����							inObjectName	�ǂݍ��ރ��f���̖��O.
//								inModel			�ǂݍ��܂��郂�f��.
//�߂�l						���� true
/***********************************************************/
bool TReadRoku::GetModel(const char* inObjectName , MODEL inModel)
{
	__RTI_CHECKER(m_Material != NULL);
	__RTI_CHECKER(inObjectName != NULL);

	//���f������`����Ă��Ȃ�������A�l�[���T�[�o�[�ɂ��̃��f����
	//�������O���m�ۂ��āA�����Ɋi�[���܂�.
	MODEL theModel = inModel;
	if (theModel == NULL)
	{
		theModel = TLim3DMaster::NewModel();
		//�l�[���T�[�o�[�ɒǉ�����.
		TLim3DMaster::NameServer.Add( inObjectName , theModel );
	}
	//�ڍׂȏ����擾���܂�.
	if( !GetModelDetail(-1 , theModel) ) return false;

	return true;
}

/***********************************************************/
//���f���̏ڍׂȏ����擾���܂�.
//����							inNo			�O���[�v.
//								outModel		�ǂݍ��܂��郂�f��.
//�߂�l						�Ȃ�
/***********************************************************/
bool TReadRoku::GetModelDetail(int inNo , MODEL outModel)
{
	__RTI_CHECKER(outModel != NULL);

	TRokuFace* theFace ;
	TRokuLine* theLine ;

	//�e��߂�l.
	bool theRet;
	//�|���S���f�[�^�̐�.
	int theCount = m_FaceCaretaker.GetCount();
	//�|���S���̊i�[�f�[�^.
	PolygonData  thePolygonData[4];

	//���炩���ߎg���|���S���Ɠ_�̗̈���m��.
	theRet = GetModelCompriseCache(inNo , outModel);
	if (!theRet) return true;

	XYZFixed theXYZ[2];
	int		 theVecNum = 0;
	for(int i = 0 ; i < theCount ; i++)
	{
		theFace = m_FaceCaretaker[i];
		//�g�p���郉�C���f�[�^�̐�.
		int theUseLineNum = theFace->FaceBlock;
		__RTI_CHECKER(theUseLineNum <= 4 && theUseLineNum >= 3);

		//�F(�}�e���A��)�f�[�^.
		__RTI_CHECKER(theFace->ColorNum < m_MaterialNum && theFace->ColorNum >= 0);
		MATERIAL theMaterial = m_Material[ theFace->ColorNum ];

		XYZFixed	theVecData[4];
		//���_���̎擾.
		for(int l = 0 ; l < theUseLineNum ; l++)
		{
			theLine = m_LineCaretaker[ theFace->LineNum[l]-1 ];
			theRet = SearchVec ( theLine->VecNum[0] , & (theXYZ[0])  );
			if (!theRet) return false;
			theRet = SearchVec ( theLine->VecNum[1] , & (theXYZ[1])  );
			if (!theRet) return false;

			//�ŏ��̃f�[�^.
			if (l == 0)
			{// 0 �� 1�ǂ������Ƃ�̂��́A���̓_���܂܂�Ă���ق�.
				XYZFixed	theNextVec[2];

				theLine = m_LineCaretaker[ theFace->LineNum[1]-1 ];
				theRet = SearchVec ( theLine->VecNum[0] , & (theNextVec[0])  );
				if (!theRet) return false;
				theRet = SearchVec ( theLine->VecNum[1] , & (theNextVec[1])  );
				if (!theRet) return false;

				if ( VectorCmp(&theXYZ[0] , &theNextVec[0] ) ||
					 VectorCmp(&theXYZ[0] , &theNextVec[1] ) )
									VectorSet(&theVecData[0]  , &theXYZ[0]);
				else if ( VectorCmp(&theXYZ[1] , &theNextVec[0] ) ||
						  VectorCmp(&theXYZ[1] , &theNextVec[1] ) )
								VectorSet(&theVecData[0]  , &theXYZ[1]);
				else	return false;
			}
			else
			{// 0 �� 1�ǂ������Ƃ�̂��́A�O�̓_�ł͂Ȃ��ق�.
				if ( VectorCmp(&theXYZ[0] , & theVecData[l-1] ) )
				{// 0 �ƑO�񂪓������̂� 1 ���Ƃ�܂�.
					VectorSet(&theVecData[l] , &theXYZ[1]);
				}
				else	if ( VectorCmp(&theXYZ[1] , & theVecData[l-1] ) )
				{// 1 �ƑO�񂪓������̂� 0 ���Ƃ�܂�.
					VectorSet(&theVecData[l] , &theXYZ[0]);
				}
				else return false;
			}
			//���̒��_�����L�^(�|���S�������Ƃ��̂Ȃ���f�[�^).
			thePolygonData[l].Link = theVecNum++;
		}
		//���_�̒ǉ�.
		outModel->AddVec(theVecData , theUseLineNum);

		//�|���S���̍쐬.
		POLYGON thePolygon = outModel->NewPolygon();
		if (thePolygon == NULL)
		{
			return false;
		}
		//�Ȃ���f�[�^�̐ݒ�.
		thePolygon->SetLink(thePolygonData , theUseLineNum);
		//�V�F�[�f�B���O.
		thePolygon->SetShading(NONE_SHADING);
		//�}�e���A��.
		thePolygon->SetMaterial(theMaterial);

	}
	return true;
}

/***********************************************************/
//���_���擾���܂�.
//����							inVecNo			�擾���������_�̃i���o�[.
//								outXYZ			���̒��_�� XYZ
//�߂�l						���� true.
/***********************************************************/
bool TReadRoku::SearchVec(int inVecNo , XYZFixed * outXYZ)
{
//	__RTI_CHECKER(inVecNo >= 0 && inVecNo < m_VecCaretaker.GetCount() );
	__RTI_CHECKER(outXYZ != NULL);

	TRokuVec * theVec ;
	int        theVecCount = m_VecCaretaker.GetCount();

	for(int i = 0; i < theVecCount ; i++)
	{
		theVec = m_VecCaretaker[i];
		//DEBUG3("%d		%d", theVec->No, inVecNo);
		if ( theVec->No == inVecNo)
		{
			outXYZ->x = theVec->x;
			outXYZ->y = theVec->y;
			outXYZ->z = theVec->z;
			return true;
		}
	}
	//������Ȃ�.
	return false;
}

/***********************************************************/
//�g�p����|���S���̐��ƒ��_���Z�o���܂�.
//����							inNo			�O���[�v.
//								outModel		�ǂݍ��܂��郂�f��.
//�߂�l						�|���S���ƒ��_�����Ȃ��Ƃ� 1 �ȏ�͂���.
/***********************************************************/
bool TReadRoku::GetModelCompriseCache(int inNo ,  MODEL outModel)
{
	//�|���S���f�[�^�̐�.
	int theCount = m_FaceCaretaker.GetCount();

	int thePolygonNum = 0;
	int theVecNum = 0;

	for(int i = 0 ; i < theCount ; i++)
	{
		TRokuFace* theFace = m_FaceCaretaker[i];

		//�g�p����|���S���̐��𑝂₷.
		thePolygonNum++;
		//�g�p���郉�C���f�[�^�̐�.
		theVecNum += theFace->FaceBlock;
	}
	//�|���S���܂��́A���_���܂܂�Ă��Ȃ�.
	if (thePolygonNum <= 0 || theVecNum <= 0) return false;
	//���_�̈�̈ꊇ�m��.
	outModel->GetVecCaretaker()->Alloc(theVecNum);
	//�|���S���̈�̈ꊇ�m��.
	//outModel->GetPolygonCaretaker()->Alloc(thePolygonNum);

	return true;
}


/***********************************************************/
//�w�b�_�`�F�b�N.
//����							�Ȃ�
//�߂�l						���� true
/***********************************************************/
bool TReadRoku::CheckHeader(char ** ioReadPointer)
{
	__RTI_CHECKER(ioReadPointer != NULL);

	char	theFormatType[7];
	/*
	ROKU4               //�@�Z�p�剤�f�[�^�̎n�܂�
		-0.785398163    //�@Rmatobj pp  �i�Z�p����v���O�����Q�ƁA
	    -0.314159265    //�@Rmatobj pq�@�@�傫��1.0x1.0�̉��z��ʂւ̕ϊ��s��j
		 0.000000000    //  Rmatobj pr
	     0.000000000    //  Rmatobj dx
		 0.000000000    //  Rmatobj dy
	     1.500000000    //  Rmatobj dz
		 1.000000000    //  Rmatobj bx
	     1.000000000    //  Rmatobj by
		1               //�@Rmatobj pers
	    0               //�@0�F�΂ߌ����@1�FYZ���ʂ̐��ʁ@2�F�� ZX����  3�F��XY����
		368.750000000    //�@��ʏ���W�ł̂w�����̊g�嗦�iRmatobj �œ������W��
		368.750000000    //�@��ʏ���W�ł̂x�����̊g�嗦�@�g�夕��s�ړ����ĉ�ʕ\��
		217.000000000    //�@��ʏ���W�ł̂w�����̈ړ��ʁ@����Ƃ��́A�g��/�ړ��ʁj
		147.000000000    //�@��ʏ���W�ł̂x�����̈ړ���
	*/
	::sscanf( *ioReadPointer ,	"%5s\r",theFormatType );
	//�Z�p�剤�`���łȂ���Ζv.
	if ( stricmp(theFormatType,"ROKU4") != 0 ) return false;

	//OK.
	return true;
}

//���̍s��.
char* TReadRoku::NextLine(char * inLine)
{
	//���s�܂�
	char* theNextLine = ::strstr(inLine , "\r");
	if (theNextLine == NULL) return NULL;

	//���s�̃T�C�Y�v���X.
	theNextLine += sizeof("\r");

	return theNextLine;
}

//�w�肵�����A���̍s�ɐi��.
char* TReadRoku::NextLine(char * inLine , int inSkipLines)
{
	char* theNextLine = inLine;
	int	  theLine = inSkipLines;
	
	for( ; theLine > 0 ; theLine--)
	{
		theNextLine = NextLine(theNextLine);
		if (theNextLine == NULL) return NULL;
	}
	return theNextLine;
}

//�_�f�[�^�̓ǂݍ���.
bool TReadRoku::GetPointes(char ** ioReadPointer) 
{
	__RTI_CHECKER(ioReadPointer != NULL);
	//POINT�^�O��T��.
	char * thePointString = ::strstr( *ioReadPointer , "POINT" );
	if (thePointString == NULL) return false;
	//����LINE0�^�O.
	char * theLineString = ::strstr( *ioReadPointer , "LINE0" );
	if (theLineString == NULL) return false;

	//POINT �̎��̍s��.
	thePointString = NextLine(thePointString);
	if (thePointString == NULL) return false;

	/*
	    1               //�@�_�̔ԍ�
		0               //�@�O���[�v���������̃O���[�v�ԍ��i�O���[�v�͊K�w���s�j
	    0               //�@�O�Ȃ�\���B�P�Ȃ�B���B
		0               //�@�_�~�[
	    2               //�@�O�Ȃ�Q�����̓_�B�O�ȊO�Ȃ獶�E�Ώ̂ȓ_�̔ԍ��B
		-0.354941037    //�@�_�̂w���W
	     0.270283061    //�@�_�̂x���W
		 0.095652076    //�@�_�̂y���W
	*/
	TRokuVec	theRokuVec;
	int			theIntDummy;
	float		theX,theY,theZ;

	m_VecCaretaker.Alloc(10000);
	//���C���^�O�ɂȂ�܂ŁA�J��Ԃ��܂�.
	for( ; theLineString > thePointString; )
	{
		sscanf(thePointString , "	%d\r	%d\r	%d\r"
								"	%d\r	%d\r"
								"	%f\r	%f\r	%f\r" ,
				&(theRokuVec.No)	,	&(theRokuVec.GroupNum),	&(theRokuVec.IsHide),
				&(theIntDummy)		,	&(theRokuVec.SymmetricalNum),
				&theX				,	&theY		,	&theZ	);
		//�傫���̕ύX.
		theRokuVec.x = Float2Fixed(theX * 64   );
		theRokuVec.y = Float2Fixed(theY * -64   );
		theRokuVec.z = Float2Fixed(theZ * 64  );
		//�R���N�V�����ɓo�^.
		m_VecCaretaker.Add(&theRokuVec,1);

		//8�s������. 
		thePointString = NextLine(thePointString,8);
		if (thePointString == NULL) return false;
	}
	//���̃f�[�^��.
	*ioReadPointer = theLineString;

	return true;
}

//���f�[�^�̓ǂݍ���.
bool TReadRoku::GetLines(char ** ioReadPointer) 
{
	__RTI_CHECKER(ioReadPointer != NULL);
	//LINE0�^�O��T��.
	char * theLineString = ::strstr( *ioReadPointer , "LINE0" );
	if (theLineString == NULL) return false;
	//����FACE0�^�O.
	char * theFaceString = ::strstr( *ioReadPointer , "FACE0" );
	if (theFaceString == NULL) return false;

	//LINE0 �̎��̍s��.
	theLineString = NextLine(theLineString);
	if (theLineString == NULL) return false;

	TRokuLine	theRokuLine;
	/*
	    3               //�@�Ȃ���_�i���̂P�j�̔ԍ�
		1               //�@�Ȃ���_�i���̂Q�j�̔ԍ�
	    0               //�@�O���[�v�̔ԍ��i�O�̎��̓O���[�v������Ă��Ȃ��j
		0               //�@�O�̂Ƃ��\���@�P�̂Ƃ��B��
	*/
	m_LineCaretaker.Alloc(5000);
	//�t�F�B�X�^�O�ɂȂ�܂ŁA�J��Ԃ��܂�.
	for( ; theFaceString > theLineString ; )
	{
		sscanf(theLineString , "	%d\r	%d\r	%d\r	%d\r"	,
				&(theRokuLine.VecNum[0])	,	&(theRokuLine.VecNum[1]),
				&(theRokuLine.GroupNum)		,	&(theRokuLine.SymmetricalNum)	);
		//�R���N�V�����ɒǉ�.
		m_LineCaretaker.Add(&theRokuLine,1);
		//4�s������. 
		theLineString = NextLine(theLineString,4);
		if (theLineString == NULL) return false;
	}
	*ioReadPointer = theFaceString;
	return true;
}

//�ʃf�[�^�̓ǂݍ���.
bool TReadRoku::GetFaces(char ** ioReadPointer)
{
	__RTI_CHECKER(ioReadPointer != NULL);
	//FACE0�^�O��T��.
	char * theFaceString = ::strstr( *ioReadPointer , "FACE0" );
	if (theFaceString == NULL) return false;
	//����END00�^�O.
	char * theEndString = ::strstr( *ioReadPointer , "END00" );
	if (theEndString == NULL) return false;

	//FACE0 �̎��̍s��.
	theFaceString = NextLine(theFaceString);
	if (theFaceString == NULL) return false;

	TRokuFace	theRokuFace;
	/*
	    3               //�@�ʂ̉搔
		2               //�@�F�̔ԍ�
	    1               //�@�ʂ̔ԍ�
	---------------
		5               //�@�ʂ̗֊s�����̂P�̔ԍ�
	    3               //�@�ʂ̗֊s�����̂Q�̔ԍ�
		4               //�@�ʂ̗֊s�����̂R�̔ԍ�
	*/
	m_FaceCaretaker.Alloc(1500);
	//�G���h�^�O�ɂȂ�܂ŁA�J��Ԃ��܂�.
	for( ; theEndString > theFaceString ; )
	{
		sscanf(theFaceString , "	%d\r	%d\r	%d\r"	,
			&(theRokuFace.FaceBlock)	,	&(theRokuFace.ColorNum),	&(theRokuFace.No)	);
		//3�s������. 
		theFaceString = NextLine(theFaceString,3);
		if (theFaceString == NULL) return false;

		//�ʂ��\�����鐔�������i���o�[��ǂݍ��݂܂�.
		switch (theRokuFace.FaceBlock)
		{
		case 3:
			sscanf(theFaceString , "	%d\r	%d\r	%d\r"	,
				&(theRokuFace.LineNum[0])	,	&(theRokuFace.LineNum[1]),
				&(theRokuFace.LineNum[2])	);
			break;
		case 4:
			sscanf(theFaceString , "	%d\r	%d\r	%d\r	%d\r"	,
				&(theRokuFace.LineNum[0])	,	&(theRokuFace.LineNum[1]),
				&(theRokuFace.LineNum[2])	,	&(theRokuFace.LineNum[3])	);
			break;
		default:
			return false;
		}

		//theRokuFace.FaceBlock�s������. 
		theFaceString = NextLine(theFaceString, theRokuFace.FaceBlock );
		if (theFaceString == NULL) return false;

		//�R���N�V�����ɒǉ�.
		m_FaceCaretaker.Add(&theRokuFace,1);
	}
	*ioReadPointer = theEndString;

	return true;
}

//�F�̎擾(�}�e���A��)
bool TReadRoku::GetMaterial(char ** ioReadPointer)
{
	__RTI_CHECKER(ioReadPointer != NULL);
	__RTI_CHECKER(m_Material == NULL);
	//PALC0�^�O��T��.
	char * thePalString = ::strstr( *ioReadPointer , "PALC0" );
	if (thePalString == NULL) return false;
	//����ENDP0�^�O.
	char * theEndPalString = ::strstr( *ioReadPointer , "ENDP0" );
	if (theEndPalString == NULL) return false;

	//PALC0 �̎��̍s��.
	thePalString = NextLine(thePalString);
	if (thePalString == NULL) return false;

	/*
		13                  //�@�F�̐��i���݂�13�̂݃T�|�[�g�j
		0                   //�@�I��F�i���݂̓_�~�[�j
	*/
	int			theColorMax;
	int			theIntDummy;
	sscanf(thePalString , "	%d\r	%d\r", &theColorMax , &theIntDummy);
	//�p���b�g�f�[�^�� 2�s�ǂݔ�΂�.
	thePalString = NextLine(thePalString , 2);
	if (thePalString == NULL) return false;

	//�F�̍ő吔�� �}�C�i�X��������v.
	if (theColorMax < 0) return false;
	//�̈�m��.
	m_Material = new MATERIAL[theColorMax];
	if (m_Material == NULL) return false;
	m_MaterialNum = theColorMax;

	/*
		255                 //�@�P�Ԗڂ̐F�F���邢���̐F(R) (0�`255)
		255                 //�@�P�Ԗڂ̐F�F���邢���̐F(G)
		255                 //�@�P�Ԗڂ̐F�F���邢���̐F(B)
		128                 //�@�P�Ԗڂ̐F�F�Â����̐F(R)
		128                 //�@�P�Ԗڂ̐F�F�Â����̐F(G)
		128                 //�@�P�Ԗڂ̐F�F�Â����̐F(B)
		0                   //�@�\��
	*/
	//�t�F�B�X�^�O�ɂȂ�܂ŁA�J��Ԃ��܂�.
	for( ; theColorMax > 0 ; theColorMax--)
	{
		int	theLightR,theLightG,theLightB,theDarkR,theDarkG,theDarkB;
		MATERIAL* theMaterial = & m_Material[m_MaterialNum - theColorMax];

		sscanf(thePalString ,	"	%d\r	%d\r	%d\r"
								"	%d\r	%d\r	%d\r"
								"	%d\r",
						&theLightR , &theLightG , &theLightB ,
						&theDarkR  , &theDarkG  , &theDarkB  ,
						&theIntDummy);

		//�Ƃ肠�����A�}�X�^�[�Ƀ}�e���A���̍쐬��\�����܂�.
		*theMaterial = TLim3DMaster::NewMaterial();
		if (*theMaterial == NULL) return false;
		//�F�̐ݒ�. ���邢�F���̗p���܂�(�蔲��).
		(*theMaterial)->SetColor( RTIRGB(theLightR , theLightG , theLightB) );
		//(*theMaterial)->SetColor( RGB(255 , 255 , 255) );

		//7�s������. 
		thePalString = NextLine(thePalString,7);
		if (thePalString == NULL) return false;
	}
	*ioReadPointer = thePalString;

	return true;
}
