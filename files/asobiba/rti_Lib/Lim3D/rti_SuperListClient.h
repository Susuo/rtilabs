#ifndef __TSUPER_LIST_CLIENT
#define __TSUPER_LIST_CLIENT

//�N���C�A���g�f�[�^�̌�.



#include "rti_SuperListClientSrc.h"

//#ifndef __TSUPER_LIST_SERVER
//	class __TSuperListServer;
//#endif
#include "rti_SuperList.h"

class __TSuperListClient;
class __TSuperListClient : public __TSuperListClientSrc 
{
public:
	//�R���X�g���N�^
	__TSuperListClient()
	{
		m_Parence = NULL;
	}
	//�f�X�g���N�^
	virtual ~__TSuperListClient(){}
public:
	//�j��.(virtual �̓I�u�W�F�N�g�v�l�I�ɂ� �� , ���x�I�ɂ� �~)
	virtual void Release()
	{
		if (m_Parence)
			((__TSuperListServer<__TSuperListClient>*)m_Parence)->Del(this);
		delete this;
	}
	//������o�^.
	void Mount(void* inP)
	{
		m_Parence = inP;
	}
protected:
	//�����̐e.
	void* m_Parence;
};


#include "rti_SuperList.h"

#endif
