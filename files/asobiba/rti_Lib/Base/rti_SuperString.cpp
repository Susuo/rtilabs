#include <./Base/rti_SuperString.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*****************************************************/
//�w�肳�ꂽ�����������.
//����				inBase				��������镶����
//					inBaseLen			��������镶����̒���
//					inChallenge			�������镶����
//					inChallengeLen		�������镶����̒���
//					inFindStartPos		true�̏ꍇ�q�b�g�����ꏊ�̐擪 / false�̓q�b�g�����ꏊ�̎�
//�߂�l			�ꏊ / ���s -1
/*****************************************************/
int TSuperString::FindString( const char * inBase , int inBaseLen  ,
							  const char * inChallenge , int inChallengeLen ,
							  bool inFindStartPos/*=false*/)
{
	int theLen ;
	int theHit = 0 ;

	for( theLen = 0; theLen < inBaseLen ; theLen++ )
	{
		if (inBase[theLen] == inChallenge[theHit])
		{
			theHit++;
			if (theHit >= inChallengeLen) 
			{//��v.
				//���̏ꏊ��Ԃ�.
				if (inFindStartPos == false)	return theLen + 1;
				//�q�b�g�����ꏊ�̐擪��Ԃ�.
				else							return (theLen - inChallengeLen) + 1;
			}
		}
		else
		{	//�ŏ����猟�����Ȃ���.
			theHit = 0;
		}
	}
	//������Ȃ�.
	return -1;
}

/*****************************************************/
//���̍s��.
//����				inBase				��������镶����
//					inBaseLen			��������镶����̒���
//					inRetCode			���^�[���R�[�h
//�߂�l			�ꏊ / ���s -1
/*****************************************************/
int TSuperString::NextLine( const char * inBase , int inBaseLen  ,
							const char * inRetCode/*="\r\n"*/ )
{
	return TSuperString::FindString( inBase , inBaseLen  ,
							inRetCode	,	strlen(inRetCode)	,	true );
}


/*****************************************************/
//�w�肳�ꂽ�����������.
//����				inBase				��������镶����
//					inChallenge			�������镶����
//					inFindStartPos		true�̏ꍇ�q�b�g�����ꏊ�̐擪 / false�̓q�b�g�����ꏊ�̎�
//�߂�l			�ꏊ / ���s -1
/*****************************************************/
int TSuperString::FindString( const char * inBase	,
				 const char * inChallenge	,	bool inFindStartPos/*=false*/)
{
	int theLen ;
	int theHit = 0 ;

	for( theLen = 0; inBase[theLen]!='\0' ; theLen++ )
	{
		if (inBase[theLen] == inChallenge[theHit])
		{
			theHit++;
			if ( inChallenge[theHit]=='\0' ) 
			{//��v.
				//���̏ꏊ��Ԃ�.
				if (inFindStartPos == false)	return theLen + 1;
				//�q�b�g�����ꏊ�̐擪��Ԃ�.
				else							return (theLen - theHit) + 1;
			}
		}
		else
		{	//�ŏ����猟�����Ȃ���.
			theHit = 0;
		}
	}
	//������Ȃ�.
	return -1;
}

/*****************************************************/
//���̍s��.
//����				inBase				��������镶����
//					inRetCode			���^�[���R�[�h
//�߂�l			�ꏊ / ���s -1
/*****************************************************/
int TSuperString::NextLine( const char * inBase , const char * inRetCode/*="\r\n"*/ )
{
	return TSuperString::FindString( inBase	,	inRetCode	,	false );
}

