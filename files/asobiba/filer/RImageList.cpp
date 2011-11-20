// RImageList.cpp: RImageList �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RImageList.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RImageList::RImageList()
{
	this->ImageList = NULL;
}

RImageList::~RImageList()
{
	Destroy();
}

//�C���[�W���X�g�̍쐬
void RImageList::Create(int inWidth,int inHegith,int inInitCount,int inMax)  throw()
{
	if (this->ImageList) throw RDoNotInitaliseException("���łɃC���[�W���X�g���쐬����Ă��܂�");

	this->ImageList = ImageList_Create(inWidth , inHegith ,ILC_COLORDDB ,  inInitCount , inMax);

	if (this->ImageList == NULL) throw RDoNotInitaliseException("�C���[�W���X�g���쐬�ł��܂���");

//	TRACE("�C���[�W���X�g���쐬���܂��� %p \n" , this->ImageList);
}

//�C���[�W���X�g�̔j��
void RImageList::Destroy()
{
//	TRACE("�C���[�W���X�g��j�����܂��� \n" );
	ImageList_Destroy(this->ImageList);
	this->ImageList = NULL;
}


//�ǉ�
int RImageList::Add(HICON inIcon)
{
	if ( RemoveList.empty())
	{//�󂫗̈悪�Ȃ��̂ŕ��ʂɒǉ�.
//		TRACE("ImageList_ReplaceIcon �V�K %d \n" , ImageList_GetImageCount(this->ImageList ) - 1 );
		return ImageList_AddIcon( this->ImageList , inIcon);
	}
	else
	{//�󂫗̈������.
		const int index = this->RemoveList.back();
		this->RemoveList.pop_back();

//		TRACE("ImageList_ReplaceIcon �ė��p %d / %d %p\n" , index , ImageList_GetImageCount(this->ImageList) ,this->ImageList);

		return ImageList_ReplaceIcon( this->ImageList ,index , inIcon);
	}
}

//�w�肵�� index ���폜
//����ɂ߂��肵�Ȃ��̂ŁA�}�}�����S�ł��B
void RImageList::Remove(int inIndex)
{
	int count = ImageList_GetImageCount(this->ImageList ) - 1;
	if ( count == inIndex)
	{//�Ō�������ꍇ�͂߂�.
		ImageList_Remove(this->ImageList , inIndex);
		TRACE("ImageList_Remove %d �߂܂���\n",inIndex);
		count--;
		//���̑O�������Ă����炳��ɏ���.
		for( ; count >= 0 ; count--)
		{
			REMOVELIST::iterator it = 
				find(this->RemoveList.begin() , this->RemoveList.end() , count);
			if (it == this->RemoveList.end())
			{//������Ȃ�
				break;
			}
			//�݂����̂ō폜.
			ImageList_Remove(this->ImageList , count);
			this->RemoveList.erase(it);
//			TRACE("ImageList_Remove %d ����ɁA�߂܂���\n",count);
		}
	}
	else
	{//�Ō�łȂ��ꍇ�́A�폜�}�[�N�𗧂Ă�.
		this->RemoveList.push_back(inIndex);
	}
}

//�S������.
void RImageList::RemoveAll()
{
	this->RemoveList.clear();
	ImageList_RemoveAll(this->ImageList);
}

