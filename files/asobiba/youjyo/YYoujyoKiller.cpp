// YYoujyoKiller.cpp: YYoujyoKiller �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YYoujyoKiller.h"
#include "YBBSManager.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YYoujyoKiller::YYoujyoKiller()
{

}

YYoujyoKiller::~YYoujyoKiller()
{

}

/**
 * Run:	���s.
 *
 * @return virtual unsigned int 
 */
unsigned int YYoujyoKiller::Run()
{
	while( this->IsHoldUp() )
	{
		YBBSManager::getInstance()->KillerTask(Buffer);
		/*ChangeLog.txt:2003/01/26	YoujyoKiller�̏I�������Ɏ��Ԃ��������Ă����̂ŏC��
		*/
		//�ꕪ�Ɉ�񓮂��܂��B
		this->Sleep(1000 * 60 * 1);
	}
	return 0;
}

/**
 * Shutdown:	��~���ߑ��M.
 *
 */
void YYoujyoKiller::Shutdown()
{
	this->HoldUp();
}
