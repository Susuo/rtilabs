// YYoujyoGC.cpp: YYoujyoGC �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YYoujyoGC.h"
#include "YBBSManager.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YYoujyoGC::YYoujyoGC()
{

}

YYoujyoGC::~YYoujyoGC()
{

}

/**
 * Run:	���s.
 *
 * @return virtual unsigned int 
 */
unsigned int YYoujyoGC::Run()
{
	while( this->IsHoldUp() )
	{
		YBBSManager::getInstance()->GCTask();
		/*ChangeLog.txt:2003/01/26	YYoujyoGC�̏I�������Ɏ��Ԃ��������Ă����̂ŏC��
		*/
		//1.5���Ɉ�񓮂��܂��B
		this->Sleep(1000 * 90 * 1);
	}
	return 0;
}

/**
 * Shutdown:	��~���ߑ��M.
 *
 */
void YYoujyoGC::Shutdown()
{
	this->HoldUp();
}

