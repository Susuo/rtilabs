class TSuperString
{
public:
	//�w�肳�ꂽ�����������.
	static int FindString( const char * inBase , int inBaseLen  ,
					       const char * inChallenge , int inChallengeLen ,
						   bool inFindStartPos=false);
	//���̍s��.
	static int NextLine( const char * inBase , int inBaseLen  ,
										 const char * inRetCode="\r\n" );
	//�w�肳�ꂽ�����������.
	static int FindString( const char * inBase	,
				 const char * inChallenge	,	bool inFindStartPos=false);
	//���̍s��.
	static int NextLine( const char * inBase , const char * inRetCode="\r\n" );
};

