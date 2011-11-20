// YDeleteWait.h: YDeleteWait �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YDELETEWAIT_H__39039959_ED97_4302_ACC5_F4AEDBEFA557__INCLUDED_)
#define AFX_YDELETEWAIT_H__39039959_ED97_4302_ACC5_F4AEDBEFA557__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"


//��x�ɍ폜�˗����o���鐔.
const int DELETE_REQUEST_MAX	=	12;
const int DELETE_TXT_LINE		=	1024;

//�}�X�N�ł���P��̍ő�l
const int MASK_TEXT_MAX	=	5;

class YDeleteWaitData
{
public:
	/**
	 * YDeleteWaitData:				�폜�Ɏg�p����f�[�^��ݒ肵�܂��B
	 *
	 * @param inDeleteRequest		�폜�˗�����
	 * @param inDeleteTarget		�폜�Ώ�����
	 * @param inDeleteTime			�폜�����s����鎞��
	 * @param inSize				���̐�.
	 * @param inDelete				�폜�Ώۏ�������.
	 * @return  
	 */
	YDeleteWaitData(DATNUMBER inDeleteRequest , DATNUMBER inDeleteTarget , 
					time_t inDeleteTime , int inSize ,const int*	inDelete)
	{
		this->DeleteRequest = inDeleteRequest;
		this->DeleteTarget = inDeleteTarget;
		this->DeleteTime = inDeleteTime;
		this->DeleteSize = inSize;

		memcpy( this->Delete , inDelete , sizeof(int) * inSize );
	}

	/**
	 * getCompateLine:				�폜�Ɏg�p����f�[�^����s�̕�����Ƃ��ĕԂ��܂��B
	 *
	 * @return const string 
	 */
	const string getCompateLine() const
	{
		string ret =	num2str(this->DeleteRequest) + "<>" + 
						num2str(this->DeleteTarget) + "<>" +
						num2str(this->DeleteTime) + "<>" ;
		for(int i = 0; i < this->DeleteSize ; i ++)
		{
			ret += num2str( this->Delete[i] ) + "<>";
		}
		return ret;
	}

	bool isTimeLimit(time_t inTime) const
	{
		return inTime >= this->DeleteTime;
	}
	//���ځ[����s.
	void DoAborn(char* outShareBuffer,const string inRequestIta , const string inTargetIta);

	//���Ȃ����Ǘ����Ă��郊�N�G�X�g�ł���?
	bool IsYourRequest(DATNUMBER inDeleteRequest) const
	{
		return inDeleteRequest == this->DeleteRequest;
	}

	DATNUMBER getDeleteTarget() const
	{
		return this->DeleteTarget;
	}


	//DeleteTime �Ń\�[�g���܂�.
	static bool Compre(class YDeleteWaitData *const _X, class YDeleteWaitData *const _Y)
	{
		return _X->DeleteTime < _Y->DeleteTime;
	}


private:
	DATNUMBER	DeleteRequest;		//�폜�˗�����
	DATNUMBER	DeleteTarget;		//�폜�Ώ�����

	time_t		DeleteTime;			//�폜�����s����鎞��

	int			DeleteSize;			//���̐�.
	int			Delete[DELETE_REQUEST_MAX];	//�폜�Ώۏ�������.
};

class YDeleteWait  
{
public:
	YDeleteWait();
	virtual ~YDeleteWait();

	void Load(const string inBoard) ;
	void Save(const string inBoard) ;

	/**
	 * Check:				���Ԃ������Ă����炠�ځ[�񂷂�.
	 *
	 * @param outShareBuffer
	 * @param inRequestIta 
	 * @param inTargetIta 
	 * @return void 
	 */
	void Check(char* outShareBuffer,const string inRequestIta , const string inTargetIta);
	//�폜�˗��̒�o.
	void Presentation(DATNUMBER inDeleteRequest , DATNUMBER inDeleteTarget , time_t inDeleteTime , int inSize ,const int*	inDelete);
	//�폜�˗��̃L�����Z��.
	bool Cancel(DATNUMBER inDeleteRequest);
	//���Ȃ����ێ����Ă���폜�˗��ł���?
	DATNUMBER IsYourRequest(DATNUMBER inDeleteRequest);

	static void YDeleteWait::test();

private:
	//���ׂč폜
	void YDeleteWait::Clear();

		//�폜����f�[�^.
	//��ɁA�폜���鎞�����L�[�ɏ����Ƀ\�[�g����Ă��܂�.
	typedef deque<YDeleteWaitData*>	YDELETE_WAIT_DATA;
	YDELETE_WAIT_DATA	Pool;
};

#endif // !defined(AFX_YDELETEWAIT_H__39039959_ED97_4302_ACC5_F4AEDBEFA557__INCLUDED_)
