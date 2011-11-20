// YYoujyoLogger.cpp: YYoujyoLogger �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YYoujyoLogger.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


YLogNotify* YLogNotify::getInstance()
{
	static YLogNotify ycn;
	return &ycn;
}


YYoujyoLogger::YYoujyoLogger()
{
	LogFile = fopen( "c:/secret.log" , "ab");
}

YYoujyoLogger::~YYoujyoLogger()
{
	if (LogFile)
	{
		fclose(LogFile);
	}
}

/**
 * Run:	���s.
 *
 * @return virtual unsigned int 
 */
unsigned int YYoujyoLogger::Run()
{
	int i = 0;
	while( 1 )
	{
		string *log;
		YLogNotify::getInstance()->Wait(&log);
		if (log == NULL)
		{
			break;	//�I��.
		}

		if (LogFile)
		{
			fwrite( log->c_str() , log->size() , sizeof(char) , LogFile );
			if (i == 100)	//100��Ɉ�x�t���b�V��.
			{
				fflush(LogFile);
				i = 0;
			}
			i++;
		}
		delete log;
	}
	return 0;
}

/**
 * Shutdown:	��~���ߑ��M.
 *
 */
void YYoujyoLogger::Shutdown()
{
	this->HoldUp();
	YLogNotify::getInstance()->Shutdown();
}
