#include <./Lim3D/rti_Lim3DAnime.h>
#include <./Lim3D/rti_Lim3DMaster.h>
#include <stdio.h>

/*******************************************************************************/
//TLim3DAnimeControl�N���X	�A�j���[�V�����̊Ǘ��������ǂ�܂�
//							���[�U�[�͂��̃N���X�̓��e��m���Ă���΁A
//							���̑��̉������N���X�͒m��Ȃ��Ă� OK �Ȃ̂�[.
/*******************************************************************************/

/*******************************************************************************/
//�^�O���T�[�`����.
//����						inStart			�V�[���̊J�n�ꏊ.
//							inEnd			�V�[���̏I���ꏊ
//							inTag			�^�O��.	Ex. "<MODEL = "
//							outEnd			������������̍Ō�.(���s�̎��͖���`)
//�߂�l					������������̐擪.
/*******************************************************************************/
char* TLim3DAnimeControl::SearchTag
		(const char* inStart , const char* inEnd , const char* inTag , char** outEnd)
{
	__RTI_CHECKER(inStart != NULL);
	__RTI_CHECKER(inEnd != NULL);
	__RTI_CHECKER(inTag != NULL);
	__RTI_CHECKER(outEnd != NULL);

	//�ŏ��Ƀq�b�g����ꏊ��Ԃ�.
	char * theNowPos  = ::strstr( inStart , inTag);
	//�擾���s��������v.
	if (theNowPos == NULL || theNowPos >= inEnd) return NULL;

	//���Ƀq�b�g����ꏊ��T��.
	*outEnd = ::strstr( theNowPos+1 , inTag);
	//�擾�ł��Ȃ� or inEnd �I�[�o�[�̎��́A inEnd ���I�[�Ƃ���.
	if (*outEnd == NULL || *outEnd >= inEnd) *outEnd = (char*)inEnd;

	return theNowPos;
}

/*******************************************************************************/
//�V�[���f�[�^ & �^�X�N�f�[�^�̔j��.
//����						�Ȃ�
//�߂�l					�Ȃ�
/*******************************************************************************/
void	TLim3DAnimeControl::Clear()
{
	{
		std::vector<TLim3DAnimeSeen*>::iterator it = 	m_Seenvector.begin();
		std::vector<TLim3DAnimeSeen*>::iterator end= 	m_Seenvector.end();
		for( ; it != end ; it++)	
		{	
			delete *it;
		}
		m_Seenvector.clear();
	}
	{
		std::vector<TLim3DAnimeTask*>::iterator it = 	m_Taskvector.begin();
		std::vector<TLim3DAnimeTask*>::iterator end= 	m_Taskvector.end();
		for( ; it != end ; it++)	
		{	
			delete *it;
		}
		m_Taskvector.clear();
	}
}

/*******************************************************************************/
//�A�j���[�V�����t�@�C����ǂݍ��݂܂�.
//����						inFileName		�ǂݍ��ރt�@�C����.
//�߂�l					���� true
/*******************************************************************************/
bool TLim3DAnimeControl::Read(const char * inFileName)
{
	__RTI_CHECKER(inFileName != NULL);

	FILE *		theFp;
	theFp = fopen(inFileName , "rb");
	if (theFp == NULL)	return false;

	char *	theFileData;
	int		theFileLen;

	//�t�@�C���T�C�Y���擾.
	::fseek( theFp , 0 , SEEK_END);
	theFileLen = ::ftell(theFp);
	::fseek( theFp , 0 , SEEK_SET);

	//�̈�̊m��.
	theFileData = new char[theFileLen];

	//�̈�Ƀf�[�^�𗬂�����.
	::fread( theFileData , sizeof(char) , theFileLen , theFp );
	//�t�@�C���̃N���[�Y.
	::fclose( theFp );

	//�V�[����ǉ������肵�Ă���.
	bool theRet = SeenSplit( theFileData , &theFileData[theFileLen] );
	if (theRet == false) return false;

	//�ǂݍ��񂾃f�[�^�̔j��.
	delete [] theFileData;

	return true;
}

/*******************************************************************************/
//�A�j���[�V�����̎��s�̂��߂̃^�X�N���쐬����.
//����						inAnimeName			�A�j���[�V������.
//�߂�l					���� TLim3DAnimeTask*   / ���s NULL
/*******************************************************************************/
TLim3DAnimeTask*	TLim3DAnimeControl::TaskCreate(const char * inAnimeName)
{
	__RTI_CHECKER(inAnimeName != NULL);

	//�l�[���T�[�o�[���AtheModelName �̃��f����T��.
	TLim3DAnimeSeen* theAnimeSeen = 
				(TLim3DAnimeSeen*) TLim3DMaster::NameServer.Get( inAnimeName );
	if (theAnimeSeen == NULL)	return NULL;

	//�^�X�N�̍쐬.
	TLim3DAnimeTask* theTask = new TLim3DAnimeTask(theAnimeSeen);
	if (theTask == NULL) return NULL;

	//���X�g�ɒǉ�����.
	m_Taskvector.push_back(theTask);

	return theTask;
}

/*******************************************************************************/
//�V�[���𕪉����܂�.
//����						inStart			�V�[���̊J�n�ꏊ.
//							inEnd			�V�[���̏I���ꏊ
//�߂�l					���� true
/*******************************************************************************/
bool TLim3DAnimeControl::SeenSplit(const char * inStart ,const char * inEnd)
{
	__RTI_CHECKER(inStart != NULL);
	__RTI_CHECKER(inEnd != NULL);

	//�ꏊ����.
	char * theNext =  (char*)inStart;
	char * theOld ;

	//�V�[���̖��O.
	char theSeenName[64];

	while(1)
	{
		theOld = theNext;
		theOld = TLim3DAnimeControl::SearchTag( theOld , inEnd , "<SEEN = " , &theNext );
		if (theOld == NULL) break;
		//���̃V�[���̖��O�̎擾.
		::sscanf(theOld , "<SEEN = \"%63[^\"]\">" , &theSeenName);
		//�V�[�����.
		SeenAnalysis(theSeenName, theOld , theNext);
	}
	return true;
}

/*******************************************************************************/
//�V�[�����	Start �� End ���V�[���f�[�^
//����						inSeenName		�V�[���̖��O
//							inStart			�V�[���̊J�n�ꏊ.
//							inEnd			�V�[���̏I���ꏊ
//�߂�l					���� true
/*******************************************************************************/
bool TLim3DAnimeControl::SeenAnalysis(const char * inSeenName,
								  const char * inStart , const char * inEnd)
{
	__RTI_CHECKER(inSeenName != NULL);
	__RTI_CHECKER(inStart != NULL);
	__RTI_CHECKER(inEnd != NULL);

	bool theRet;
	//�܂��A�V�[���f�[�^���쐬����.
	TLim3DAnimeSeen* theSeen = new TLim3DAnimeSeen;
	if (theSeen == NULL)
	{//�������s��.
		return false;
	}
	//�V�[�����t���[���P�ʂŕ������܂�.
	theSeen->FreamSplit( inStart , inEnd);

	//�l�[���T�[�o�[�ɒǉ�����.
	theRet = TLim3DMaster::NameServer.Add( inSeenName , theSeen );
	if (theRet == false)
	{//�l�[���T�[�o�[�ɓo�^���s.
		delete theSeen;
		return false;
	}
	//�V�[�������X�g�ɒǉ����܂�.
	m_Seenvector.push_back(theSeen);

	return true;
}

/*******************************************************************************/
//TLim3DAnimeTask�N���X		�A�j���[�V�������s���g���.
//����							
//�߂�l					�܂��p�� true / �����܂� false
/*******************************************************************************/
bool TLim3DAnimeTask::Action()
{
	__RTI_CHECKER(m_Seen != NULL);

	bool	theRet;
	//�A�j���[�V�����w�߂��o��.
	theRet = m_Seen->Action( m_FreamNum , m_Time);	
	//���Ԃ𑝂₵�܂�.
	m_Time++;
	//�A�j���[�V�����͂܂��p�������܂�.
	if (theRet == true)	return true;

	//���݂� m_FreamNo�̃t���[���̃A�j���[�V�����͏I�����܂���.
	//���̃t���[���ɃZ�b�g���āA���Ԃ����Z�b�g���܂�.
	m_FreamNum++;
	m_Time = 0;

	//�Ƃ���ŁA�A�j���[�V�����͂܂��Â��� ?
	theRet = m_Seen->IsContinue(m_FreamNum);
	if (theRet == true)	return true;

	//�����A�����܂��̏ꍇ.

	switch( m_Option )
	{
	case __ENUM_LIM3D_ANIME_TASK_OPTION_NONE	:
		break;
	case __ENUM_LIM3D_ANIME_TASK_OPTION_REPERT	:
		m_FreamNum = 0;
		m_Time = 0;
		break;
	case __ENUM_LIM3D_ANIME_TASK_OPTION_AUTOKILL:
		break;
	}

	return false;

}



/*******************************************************************************/
//TLim3DAnimeSeen�N���X		�A�j���̃V�[������舵���܂�.
//							�A�j���́A1 �V�[��
/*******************************************************************************/

/*******************************************************************************/
//�w�肳�ꂽ�t���[�������s���߂��o��.
//							ioFream			�Đ�����t���[��.
//							inTime			
//�߂�l					true / �A�j���[�V�����I�� false
/*******************************************************************************/
bool	TLim3DAnimeSeen::Action(int inFreamNo, int inTime)
{
	__RTI_CHECKER(inFreamNo >= 0);
	__RTI_CHECKER(inFreamNo <= m_vector.size());

	bool theEndOfFreram;

	//���̃t���[�����A�j���[�V����.
	theEndOfFreram = (m_vector[inFreamNo])->Action(inTime);
	//���̃t���[���̃A�j���[�V����������������A�����^�[��.
	if (theEndOfFreram == true)  return true;

	//���̃t���[���́A�I���܂����̂ł����߂܂�.
	return false;
}

/*******************************************************************************/
//�t���[���f�[�^�̔j��.
//����						�Ȃ�
//�߂�l					�Ȃ�
/*******************************************************************************/
void	TLim3DAnimeSeen::Clear()
{
	std::vector<TLim3DAnimeFream*>::iterator it = 	m_vector.begin();
	std::vector<TLim3DAnimeFream*>::iterator end= 	m_vector.end();
	for( ; it != end ; it++)	
	{	
		delete *it;
	}
	m_vector.clear();
}

/*******************************************************************************/
//�t���[���𕪉����܂�.
//							inStart			�V�[���̊J�n�ꏊ.
//							inEnd			�V�[���̏I���ꏊ
//�߂�l					���� true
/*******************************************************************************/
bool TLim3DAnimeSeen::FreamSplit(const char *  inStart , const char * inEnd)
{
	__RTI_CHECKER(inStart != NULL);
	__RTI_CHECKER(inEnd != NULL);

	//���̃t���[�������t���[���ŏI��������΂悢�̂�..
	int theFreamTime;

	//�ꏊ����.
	char * theNext = (char*)inStart;
	char * theOld;

	while(1)
	{
		theOld = theNext;
		theOld = TLim3DAnimeControl::SearchTag( theOld , inEnd , "<FREAM = " , &theNext );
		if (theOld == NULL) break;
		//���̃t���[���̏I�������鎞�Ԃ̎擾.
		::sscanf(theOld , "<FREAM = %d" , &theFreamTime);
		//�V�[�����.
		FreamAnalysis(theFreamTime, theOld , theNext);
	}
	return true;
}

/*******************************************************************************/
//�t���[�����	Start �� End ���V�[���f�[�^
//����						inSeenName		�V�[���̖��O
//							inStart			�V�[���̊J�n�ꏊ.
//							inEnd			�V�[���̏I���ꏊ
//�߂�l					���� true
/*******************************************************************************/
bool TLim3DAnimeSeen::FreamAnalysis(int inFreamTime,const char * inStart , const char * inEnd)
{
	__RTI_CHECKER(inFreamTime > 0);
	__RTI_CHECKER(inStart != NULL);
	__RTI_CHECKER(inEnd   != NULL);

	bool	theRet;
	//�V�K�t���[���쐬.
	TLim3DAnimeFream * theFream = new TLim3DAnimeFream;
	if (theFream == NULL) return false;
	//���̃t���[�����e��A�j���[�V�����P�ʂɕ���.
	theRet = theFream->BaseSplit(inFreamTime , inStart , inEnd );
	if (theRet == false)
	{
		delete theFream;
		return false;
	}
	//�t���[����o�^.
	m_vector.push_back(theFream);

	return true;
}

/*******************************************************************************/
//TLim3DAnimeFream�N���X	�A�j���̃V�[������舵���܂�.
//							�A�j���́A1 �V�[��
/*******************************************************************************/
THIS_ANALYSIS	 TLim3DAnimeFream::m_AnalysisTable[] = 
{
	&TLim3DAnimeModel::ThisAnalysis,
	NULL,
};

/*******************************************************************************/
//�w�肳�ꂽ�t���[�������s���߂��o��.
//����						inTime			���݂̎���.
//�߂�l					true / �A�j���[�V�����I�� false
/*******************************************************************************/
bool	TLim3DAnimeFream::Action(int inTime)
{
	__RTI_CHECKER(inTime <= m_Time);

	//���̃t���[���������Ă��邷�ׂẴA�j���[�V�����P��(TLim3DAnimeBase)��
	//�A�j���[�V�����w�߂��o���܂�.
	std::vector<TLim3DAnimeBase*>::iterator it = 	m_vector.begin();
	std::vector<TLim3DAnimeBase*>::iterator end= 	m_vector.end();
	for( ; it != end ; it++)	
	{	//�A�j���[�V�������s.
		(*it)->Action();
	}
	//���̃t���[���̃A�j���[�V�������I�����Ă��Ȃ��Ȃ��...
	if ( inTime < m_Time)	return true;

	//�I�����܂���.
	return false; 
}


/*******************************************************************************/
//�e��A�j���[�V�����P�ʂ𕪉����܂�.
//����						inSeenName		�V�[���̖��O
//							inStart			�V�[���̊J�n�ꏊ.
//							inEnd			�V�[���̏I���ꏊ
//�߂�l					���� true
/*******************************************************************************/
bool TLim3DAnimeFream::BaseSplit(int inFreamTime,const char * inStart , const char * inEnd)
{
	__RTI_CHECKER(inFreamTime > 0);
	__RTI_CHECKER(inStart != NULL);
	__RTI_CHECKER(inEnd != NULL);

	THIS_ANALYSIS	* theThisAnalysis = TLim3DAnimeFream::m_AnalysisTable;

	for( ; *theThisAnalysis ; theThisAnalysis++)
	{	//���̊e��A�j���[�V�����P�ʂŕ���.
		(*theThisAnalysis)( this , inFreamTime , inStart , inEnd);
	}
	m_Time = inFreamTime;

	return true;
}

/*******************************************************************************/
//�e��A�j���[�V�����P�ʂ̔j��.
//����						�Ȃ�
//�߂�l					�Ȃ�
/*******************************************************************************/
void	TLim3DAnimeFream::Clear()
{
	std::vector<TLim3DAnimeBase*>::iterator it = 	m_vector.begin();
	std::vector<TLim3DAnimeBase*>::iterator end= 	m_vector.end();
	for( ; it != end ; it++)	
	{	
		delete *it;
	}
	m_vector.clear();
}






/*******************************************************************************/
//TLim3DAnimeModel			���f���̃A�j���[�V����.
//							
/*******************************************************************************/


/*******************************************************************************/
//���f���A�j���[�V�����̉��.
//����						inFream			Fream.
//							inSeenName		�V�[���̖��O
//							inStart			�V�[���̊J�n�ꏊ.
//							inEnd			�V�[���̏I���ꏊ
//�߂�l					���� true
/*******************************************************************************/
bool TLim3DAnimeModel::ThisAnalysis
		(TLim3DAnimeFream * inFream ,int inFreamTime,const char * inStart , const char * inEnd)
{
	__RTI_CHECKER(inFreamTime > 0);
	__RTI_CHECKER(inStart != NULL);
	__RTI_CHECKER(inEnd != NULL);
	//�ꏊ����.
	char * theNext = (char*)inStart;
	char * theOld ;

	//���f���̖��O.
	char theModelName[64];
	int	 theX , theY , theZ , theRX , theRY  , theRZ;
	int  theRev;

	while(1)
	{
		theOld = theNext;
		theOld = TLim3DAnimeControl::SearchTag( theOld , inEnd , "<MODEL = " , &theNext );
		if (theOld == NULL) break;
		//���̃��f���̊e��ݒ���擾�A���s������v.
		theRev = ::sscanf(theOld , "<MODEL = \"%63[^\"]\" M(%d %d %d) R(%d %d %d)>" ,
						theModelName , &theX , &theY , &theZ , &theRX , &theRY  , &theRZ);
		if (theRev != 7) return false;

		//�l�[���T�[�o�[���AtheModelName �̃��f����T��.
		MODEL* theModelPointer = (MODEL*)TLim3DMaster::NameServer.GetPointer( theModelName );
		MODEL theModel = *theModelPointer;

		if (theModel == NULL)	return false;

		//���f���̍쐬�A���s������v.
		TLim3DAnimeModel* theAnimeModel = new TLim3DAnimeModel;
		if (theAnimeModel == NULL) return false;

		//�f�[�^�̃Z�b�g.
		theAnimeModel->m_Model = theModelPointer;
		theAnimeModel->DataSet(inFreamTime , (Fixed)theX , (Fixed)theY , (Fixed)theZ , 
											(Angle)theRX , (Angle)theRY ,(Angle)theRZ );
		//Fream�ɓo�^.
		inFream->Add(theAnimeModel);
	}
	return true;
}


