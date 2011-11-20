#include <./Lim3D/3DRead/rti_Lim3DMetasequoia.h>
#include <stdio.h>

/***********************************************************/
//�R���X�g���N�^
//����							�Ȃ�
//�߂�l						�Ȃ�
/***********************************************************/
TReadMetasequoia::TReadMetasequoia()
{
	//�}�e���A��.
	m_Material			= NULL;
	//�}�e���A���̐�.
	m_MaterialNum		= 0;
	//�t�@�C������ǂݍ��񂾃f�[�^.
	m_FileData			= NULL;
	//�f�[�^�̒���.
	m_FileDataLen		= 0;
}

/***********************************************************/
//�f�X�g���N�^
//����							�Ȃ�
//�߂�l						�Ȃ�
/***********************************************************/
TReadMetasequoia::~TReadMetasequoia()
{
	Init();
}

/***********************************************************/
//������.
//����							�Ȃ�
//�߂�l						�Ȃ�
/***********************************************************/
void TReadMetasequoia::Init()
{
	//�}�e���A��.
	if (m_Material) delete [] m_Material;
	m_Material			= NULL;
	//�t�@�C������ǂݍ��񂾃f�[�^.
	if (m_FileData) delete [] m_FileData;
	m_FileData			= NULL;
	//�f�[�^�̒���.
	m_FileDataLen		= 0;
	//�}�e���A���̐�.
	m_MaterialNum		= 0;
}

/***********************************************************/
//�t�@�C���̓ǂݍ���.
//����							inFileName	�ǂݍ��ރt�@�C����.
//�߂�l						���� true
/***********************************************************/
bool TReadMetasequoia::Open(const char *inFileName)
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
	m_FileData = new char[m_FileDataLen];

	//�̈�Ƀf�[�^�𗬂�����.
	::fread( m_FileData , sizeof(char) , m_FileDataLen , theFp );

	::fclose( theFp );
	
	//�w�b�_�`�F�b�N.
	if ( ! CheckHeader() )
	{//���������f�[�^�Ȃ̂ŁA�I��.
		Init();
		return false;
	}
	//�}�e���A���擾.
	if ( ! GetMaterial() )
	{//���������f�[�^�Ȃ̂ŁA�I��.
		Init();
		return false;
	}
	return true;
}

/***********************************************************/
//�N���[�Y.
//����							�Ȃ�
//�߂�l						���� true
/***********************************************************/
bool TReadMetasequoia::Close()
{
	Init();
	return true;
}


/***********************************************************/
//�w�b�_�`�F�b�N.
//����							�Ȃ�
//�߂�l						���� true
/***********************************************************/
bool TReadMetasequoia::CheckHeader()
{
	__RTI_CHECKER(m_FileData != NULL);

	char	theFormatType[10];
	float	theVer;

	::sscanf(m_FileData , "Metasequoia Document\r\nFormat %s Ver %f" 
												,theFormatType , &theVer );
	//�t�H�[�}�b�g�`�����e�L�X�g�Ŗ�����Ζv.
	if ( stricmp(theFormatType,"Text") != 0 ) return false;
	//�o�[�W������ 1.0�����͖v.
	if ( theVer < 1.0 ) return false;

	//OK.
	return true;
}

/***********************************************************/
//���f���̎擾.
//����							inObjectName	�ǂݍ��ރ��f���̖��O.
//								inModel			�ǂݍ��܂��郂�f��.
//�߂�l						���� true
/***********************************************************/
bool TReadMetasequoia::GetModel(const char* inObjectName , MODEL inModel)
{
	__RTI_CHECKER(m_FileData != NULL);
	__RTI_CHECKER(m_Material != NULL);
	__RTI_CHECKER(inObjectName != NULL);

	//���ݏ������Ă���ꏊ.
	char *theNowProc = m_FileData;
	//�I�u�W�F�N�g�̖��O.
	char theModelName[64];

	while( 1 )
	{
		//���f���^�O��T��.
		theNowProc = ::strstr( theNowProc , "Object " );
		//�^�O������Ȃ�.
		if ( theNowProc == NULL)	return false;
		//�I�u�W�F�N�g���A�w�肳�ꂽ�I�u�W�F�N�g�Ɠ��������`�F�b�N.
		::sscanf( theNowProc , "Object \"%63[^\"]\" {" , theModelName);
		//���ꂪ����������ׂ�A�������Ȃ烋�[�v�𔲂���.
		if ( strcmp(inObjectName , theModelName) == 0)	break;
		//�������Ȃ��̂ŁA���̃I�u�W�F�N�g�������ǁA strstr �𐬌������邽�߂� +1��
		//������̃|�C���^�����炵�܂�.
		theNowProc += 1;
	}
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
	GetModelDetail(theNowProc , theModel);

	return true;
}
/***********************************************************/
//���ׂẴ��f���̎擾.
//����							�Ȃ�
//�߂�l						���� true
/***********************************************************/
bool TReadMetasequoia::GetAllModel()
{
	__RTI_CHECKER(m_FileData != NULL);
	__RTI_CHECKER(m_Material != NULL);

	//���ݏ������Ă���ꏊ.
	char *theNowProc = m_FileData;
	//�I�u�W�F�N�g�̖��O.
	char theModelName[64];
	//�ꎞ���f��.
	MODEL	theDummyModel;

	while( 1 )
	{
		//���f���^�O��T��.
		theNowProc = ::strstr( theNowProc , "Object " );
		//�^�O������Ȃ�.
		if ( theNowProc == NULL)	return false;
		//�I�u�W�F�N�g���A�w�肳�ꂽ�I�u�W�F�N�g�Ɠ��������`�F�b�N.
		::sscanf( theNowProc , "Object \"%63[^\"]\" {" , theModelName);

		//�l�[���T�[�o�[�ɂ��̃��f���Ɠ������O���m�ۂ��āA�����Ɋi�[���܂�.
		theDummyModel = TLim3DMaster::NewModel();
		//�l�[���T�[�o�[�ɒǉ�����.
		TLim3DMaster::NameServer.Add( theModelName , theDummyModel );
		//�ڍׂȏ����擾���܂�.
		GetModelDetail(theNowProc , theDummyModel);

		//���̃I�u�W�F�N�g�������ǁA strstr �𐬌������邽�߂� +1��
		//������̃|�C���^�����炵�܂�.
		theNowProc += 1;
	}

	return true;
}

/***********************************************************/
//���f���̏ڍׂȏ����擾���܂�.
//����							inObjectData	���f���^�O�̓��e.
//								outModel		�ǂݍ��܂��郂�f��.
//�߂�l						�Ȃ�
/***********************************************************/
void TReadMetasequoia::GetModelDetail(const char* inObjectData , MODEL outModel)
{
	__RTI_CHECKER(inObjectData != NULL);
	__RTI_CHECKER(outModel != NULL);

	//�����ŁA ���݂� �I�u�W�F�N�g�͏������ׂ��I�u�W�F�N�g�ɂȂ��Ă��܂�.
	char *	theTargetPos;
	//���_�����@�̃T�[�`.
	theTargetPos = ::strstr(inObjectData , "vertex ");
	if (theTargetPos != NULL) GetModelDetailVertex(theTargetPos , outModel);
	//�ʏ����@�̃T�[�`.
	theTargetPos = ::strstr(inObjectData , "face ");
	if (theTargetPos != NULL) GetModelDetailFace(theTargetPos , outModel);
	//�V�F�[�f�B���O���@�̃T�[�`.
	theTargetPos = ::strstr(inObjectData , "shading ");
	if (theTargetPos != NULL) GetModelDetailShading(theTargetPos , outModel);

	return;
}

/***********************************************************/
//�V�F�[�f�B���O���̎擾.
//����							inTagData		�V�F�[�f�B���O�̓��e.
//								outModel		�ǂݍ��܂��郂�f��.
//�߂�l						�Ȃ�
/***********************************************************/
void TReadMetasequoia::GetModelDetailShading(const char* inTagData , MODEL outModel)
{
	__RTI_CHECKER(inTagData != NULL);
	__RTI_CHECKER(outModel != NULL);

	//�擾����V�F�[�f�B���O.
	int theShading = 0;
	::sscanf(inTagData , " shading %d" , &theShading);

	//Lim3D ���C�u�����Ŏg���V�F�[�f�B���O.
	ShadingLevel thePolygonShading ;
	switch(theShading)
	{
	case 0:
		thePolygonShading = FLAT_SHADING;		break;
	case 1:
		thePolygonShading = GROW_SHADING;		break;
	default:
		thePolygonShading = NONE_SHADING;		break;
	}
	
	//���ׂẴ|���S���ɃA�N�Z�X���Ĥ�V�F�[�f�B���O���x���̐ݒ�.
	TLim3DPolygon* thePoly = outModel->FindFirstPolygon();
	if (thePoly == NULL)	return ;
	do
	{
		thePoly->SetShading(thePolygonShading);
	}
	while( thePoly = outModel->FindNextPolygon() );
}

/***********************************************************/
//���_���̎擾.
//����							inTagData		���_�̓��e.
//								outModel		�ǂݍ��܂��郂�f��.
//�߂�l						�Ȃ�
/***********************************************************/
void TReadMetasequoia::GetModelDetailVertex(const char* inTagData , MODEL outModel)
{
	__RTI_CHECKER(inTagData != NULL);
	__RTI_CHECKER(outModel != NULL);

	//�擾���钸�_���.
	int theVertex = 0;
	sscanf(inTagData , " vertex %d {" , &theVertex);
	if (theVertex <= 0) return ;
	
	//���ݏ������Ă���Ƃ���.
	char * theNowProc = (char*) inTagData;
	//���̐������̒��_�����܂�.
	XYZFixed * theVertexBuffer = new XYZFixed[theVertex];
	if (theVertexBuffer == NULL) return;

	int theI;
	for(theI = 0 ; theI < theVertex ; theI++)
	{
		//���s�܂œǂݔ�΂�.
		theNowProc         = ::strstr(theNowProc , "\r\n" );
		if (theNowProc == NULL)
		{
			delete [] theVertexBuffer;
			return ;
		}
		//���s�R�[�h���΂��܂�.
		theNowProc += sizeof("\r\n");
		//���̎��̍s�ɂ͒��_��񂪂���͂��Ȃ̂ŁA�ǂݍ��݂܂�.
		GetVertexCaseByCase(  theNowProc , &theVertexBuffer[theI]);
	}
	//���̒��_�̒ǉ�.
	outModel->SetVec( theVertexBuffer , theVertex );
	//�s�v�ȍ�ƃo�b�t�@�̃N���A.
	delete [] theVertexBuffer;
}

/***********************************************************/
//���̎��̍s�ɂ͒��_��񂪂���͂��Ȃ̂ŁA�ǂݍ��݂܂�.
//����							inVertexData	���_�̓��e.
//								outVertex		���_���i�[����o�b�t�@.
//�߂�l						�Ȃ�
/***********************************************************/
void TReadMetasequoia::GetVertexCaseByCase
						(const char* inVertexData , XYZFixed * outVertex)
{
	__RTI_CHECKER(inVertexData != NULL);
	__RTI_CHECKER(outVertex != NULL);

	float theX,theY,theZ;
	::sscanf(inVertexData , " %f %f %f" , &theX, &theY, &theZ);

	outVertex->x = Float2Fixed(theX);
	outVertex->y = Float2Fixed(theY);
	outVertex->z = Float2Fixed(theZ);
}

/***********************************************************/
//�ʏ��̎擾.
//����							inTagData		�ʂ̓��e.
//								outModel		�ǂݍ��܂��郂�f��.
//�߂�l						�Ȃ�
/***********************************************************/
void TReadMetasequoia::GetModelDetailFace(const char* inTagData , MODEL outModel)
{
	__RTI_CHECKER(inTagData != NULL);
	__RTI_CHECKER(outModel != NULL);

	//�擾����ʏ��.
	int theFace = 0;
	::sscanf(inTagData , " face %d {" , &theFace);
	if (theFace <= 0) return ;
	
	//���ݏ������Ă���Ƃ���.
	char * theNowProc = (char*) inTagData;

	int theI;
	for(theI = 0 ; theI < theFace ; theI++)
	{
		//���s�܂œǂݔ�΂�.
		theNowProc         = ::strstr(theNowProc , "\r\n" );
		if (theNowProc == NULL)		return ;
		//���s�R�[�h���΂��܂�.
		theNowProc += sizeof("\r\n");
		//���̎��̍s�ɂ͖ʏ�񂪂���͂��Ȃ̂ŁA�ǂݍ��݂܂�.
		bool theRet = GetFaceCaseByCase(  theNowProc , outModel);
		if (theRet == false) return ;
	}
}

/***********************************************************/
//�ʏ��̎擾.
//����							inFaceData		1�ʂ̓��e.
//								outModel		�ǂݍ��܂��郂�f��.
//�߂�l						���� true
/***********************************************************/
bool TReadMetasequoia::GetFaceCaseByCase(const char* inFaceData , MODEL outModel)
{
	__RTI_CHECKER(inFaceData != NULL);
	__RTI_CHECKER(outModel != NULL);

	//���_��.
	int theVertexCount;
	::sscanf(inFaceData , " %d " , &theVertexCount);
	//���_����������Ă�����v.
	if (theVertexCount < 3 || theVertexCount > 4)	return false;

	//�����N�ȃf�[�^.
	int			theLink[4];
	//�}�e���A���i���o�[
	int			theMaterial = 0;
	//UV
	float		theUV[4*2];

	if (theVertexCount == 3)
	{//�O�p�`.
		::sscanf(inFaceData , " %d V(%d %d %d) M(%d) UV(%f %f %f %f %f %f)" , 
			&theVertexCount , &theLink[2] , &theLink[1] , &theLink[0] ,
			&theMaterial , 
			&theUV[4] , &theUV[5] , &theUV[2] , &theUV[3] , &theUV[0] , &theUV[1] );
		//�l�̃Z�b�g.
		GetFaceCaseByCasePolygonCreate( outModel , 3 , theLink , theMaterial , theUV );
	}
	else
	{//�l�p�`.
		::sscanf(inFaceData , " %d V(%d %d %d %d) M(%d) UV(%f %f %f %f %f %f %f %f)" ,
			&theVertexCount , &theLink[3] , &theLink[2] , &theLink[1] , &theLink[0] ,
			&theMaterial , 
			&theUV[6] , &theUV[7] , &theUV[4] , &theUV[5] ,
			&theUV[2] , &theUV[3] , &theUV[0] , &theUV[1]);
		//�l�̃Z�b�g.
		GetFaceCaseByCasePolygonCreate( outModel , 4 , theLink , theMaterial , theUV );
	}
	return true;
}

/***********************************************************/
//���̂Ȃ���f�[�^���A�|���S�����쐬����.
//����							outModel		�ǂݍ��܂��郂�f��.
//								inNum			���_�̐� 3 or 4.
//								inLink			�Ȃ���
//								inMaterial		�}�e���A��
//								inUV			�e�N�X�`����UV
//�߂�l						�Ȃ�
/***********************************************************/
void TReadMetasequoia::GetFaceCaseByCasePolygonCreate
				(MODEL outModel , int inNum ,
				 const int* inLink , int inMaterial , const float* inUV)
{
	__RTI_CHECKER(outModel != NULL);
	__RTI_CHECKER(inMaterial >= 0);
	__RTI_CHECKER(inMaterial <  m_MaterialNum);

	//�|���S���̒��_�̂Ȃ��������킷�f�[�^.
	PolygonData thePolygonData[4];

	//�e�N�X�`���[.
	BMD theBmd = m_Material[inMaterial]->GetTexture();

	int theI ;
	for(theI = 0 ; theI < inNum ; theI++)
	{
		//���_�̂Ȃ���.
		thePolygonData[theI].Link = inLink[theI];
		//�e�N�X�`���[������Ƃ�.
		if (theBmd != NULL)
		{
			//���_�̃e�N�X�`���[�� UV.
			thePolygonData[theI].Texture.x = ( inUV[theI*2 + 0] * theBmd->w);
			thePolygonData[theI].Texture.y = ( inUV[theI*2 + 1] * theBmd->h);
		}
	}
	//�V�K�|���S���쐬.
	TLim3DPolygon* thePolygon = outModel->NewPolygon();
	//���̃|���S���ɂ������̃f�[�^�����킦�Ċ���.
	thePolygon->SetLink( thePolygonData , inNum);
	//���̃|���S���ƃ}�e���A�����֘A�t���܂�.
	thePolygon->SetMaterial( m_Material[inMaterial] );
}


/***********************************************************/
//�}�e���A���擾.
//����							�Ȃ�.
//�߂�l						���� true
/***********************************************************/
bool TReadMetasequoia::GetMaterial()
{
	__RTI_CHECKER(m_FileData != NULL);

	//���ݏ������Ă���ꏊ.
	char *theNowProc;

	//�}�e���A���^�O��T��.
	theNowProc = ::strstr( m_FileData , "Material" );
	//�^�O������Ȃ�.
	if ( theNowProc == NULL)	return false;

	//�}�e���A���^�O�ɂ���A�}�e���A�������擾.
	m_MaterialNum = 0;
	sscanf( theNowProc , "Material %d {" , &m_MaterialNum);
	if (m_MaterialNum <= 0) return false;
	//�}�e���A���̈�̊m��.
	m_Material = new MATERIAL[m_MaterialNum];
	if (m_Material == NULL)		return false;

	int MaterialDataNum = 0;
	for(MaterialDataNum = 0 ; MaterialDataNum < m_MaterialNum ; MaterialDataNum++)
	{
		//���s�܂œǂݔ�΂�.
		theNowProc         = ::strstr(theNowProc , "\r\n" );
		if (theNowProc == NULL) return false;
		//���s�R�[�h���΂��܂�.
		theNowProc += sizeof("\r\n");
		//���̎��̍s�ɂ̓}�e���A��������͂��Ȃ̂ŁA�ǂݍ��݂܂�.
		bool theRet = GetMaterialCaseByCase
						(  theNowProc , &m_Material[MaterialDataNum]);
		//�}�e���A���擾���̃G���[.
		if (theRet == false)		return false;
	}
	return true;	
}

/***********************************************************/
//1�}�e���A���擾.
//����							inMaterialData		1�}�e���A���̃f�[�^.
//								outMaterial			�ǂݍ��ރ}�e���A��.
//�߂�l						���� true
/***********************************************************/
bool TReadMetasequoia::GetMaterialCaseByCase
					(const char* inMaterialData , MATERIAL* outMaterial)
{
	char	theMaterialName[32]={0};		//�ގ���                 31Byte�ȓ�
	float	theR=0,theG=0,theB=0,theTrans=0;//�F�i�q�f�a�j�A�s�����x ���ꂼ��0�`1
	float	theDif=0;						//�g�U��                 0�`1
	float	theAmb=0;						//���͌�                 0�`1
	float	theEmi=0;						//���ȏƖ�               0�`1
	float	theSpc=0;						//���ˌ�                 0�`1
	float	thePow=0;						//���ˌ��̋���           0�`100
	char	theTexName[64]={0};				//�͗l�}�b�s���O��       ���΃p�X�ŋL�q�i63Byte�ȓ��j
	char	theAlphaName[64]={0};			//�����}�b�s���O��       ���΃p�X�ŋL�q�i63Byte�ȓ�)
	char	theBumpName[64]={0};			//���ʃ}�b�s���O��       ���΃p�X�ŋL�q�i63Byte�ȓ��j
	//����Ȋ����ł�.
	//	"mat1" col(1.000 0.275 0.459 1.000) dif(1.000) amb(0.250) emi(0.250) spc(0.000) power(5.00)
	::sscanf(inMaterialData , "	\"%31[^\"]\" " \
			"col(%f %f %f %f) dif(%f) amb(%f) emi(%f) spc(%f) power(%f) " \
			"tex(\"%63[^\"]\") alpha(\"%63[^\"]\") bump(\"%63[^\"]\")",
			theMaterialName , &theR , &theG , &theB , &theTrans,
			&theDif , &theAmb , &theEmi , &theSpc , &thePow ,
			theTexName , theAlphaName , theBumpName			);

	//�Ƃ肠�����A�}�X�^�[�Ƀ}�e���A���̍쐬��\�����܂�.
	*outMaterial = TLim3DMaster::NewMaterial();
	if (*outMaterial == NULL) return false;
	//���������A�A�X�^���N�X����̂͂��邢�̂�...
	MATERIAL theMaterial = *outMaterial;

	//���̃}�e���A�����\�z���܂�.
	//�F�̌���.
	theMaterial->SetColor( ClasColor( theR , theG , theB , theEmi) );
	//�f�B�t���[�Y.
	theMaterial->SetDiffuseReflexFactor( Float2Fixed(theDif) );
	//�A���r�G���g.
	theMaterial->SetAmbientReflexFactor( Float2Fixed(theAmb) );
	//�X�y�L�����[
	theMaterial->SetSpecularReflexFactor( Float2Fixed(theSpc) );
	//�X�y�L�����[(���ˌ��̋���)
	theMaterial->SetVelvetyReflexFactor( Float2Fixed(thePow) );
	//�e�N�X�`���[�}�b�s���O.
	theMaterial->SetTexture( ClasTexture(theTexName) );
	//�ŁA�e�N�X�`���[�͌��ǎg���� ? 
	if ( theMaterial->GetTexture() == NULL)
	{	//No �e�N�X�`���[.
		theMaterial->SetDrawing(BETA_DRAW);
	}
	else
	{	//Use �e�N�X�`���[.
		theMaterial->SetDrawing(TEXTURE_DRAW);
	}
	//���C�e�B���O���f���̌���.
	theMaterial->SetLighting
				( ClasLighting( theDif , theAmb , theSpc , thePow) );
	//�l�[���T�[�o�[�ɒǉ�����.
	TLim3DMaster::NameServer.Add( theMaterialName , theMaterial );
	//����.
	return true;
}


/***********************************************************/
//�J���[���擾.
//����							inR					��.
//								inG					��.
//								inB					��.
//								inEei				�������g�̐F
//�߂�l						�F.
/***********************************************************/
DWORD TReadMetasequoia::ClasColor( float inR , float inG , float inB , float inEmi)
{
	int theR = (int)(inR * inEmi * 255);
	int theG = (int)(inG * inEmi * 255);
	int theB = (int)(inB * inEmi * 255);

	if (theR > 0xff) theR = 0xff;
	if (theG > 0xff) theG = 0xff;
	if (theB > 0xff) theB = 0xff;

	return (theR << 16) + (theG << 8) + (theB);
}

/***********************************************************/
//�e�N�X�`���[�}�b�s���O������ ? ���ꍇ�́A���� �e�N�X�`���[�� BMD��Ԃ�.
//����							inTextureName		�e�N�X�`���[�̃p�X.
//�߂�l						BMD
/***********************************************************/
BMD TReadMetasequoia::ClasTexture(const char * inTextureName)
{
	//�t�@�C�������̂��̂��Ȃ�.
	if (inTextureName[0] == '\0') return NULL;
	//�}�X�^�[�̃e�N�X�`���[���g��.
	return TLim3DMaster::NewTexture(inTextureName);
}

/***********************************************************/
//���C�e�B���O���ʂ̐ݒ�.
//����							inDif
//								inAmb
//								inSpc
//								inPow
//�߂�l						���C�e�B���O���x��.
/***********************************************************/
LightLevel TReadMetasequoia::ClasLighting(float inDif ,float inAmb ,float inSpc ,float inPow)
{
	return LAMBERT_LIGHTING;
}


