#ifndef	___LIM3D_MODELMAKE__H	//�d���h�~
#define ___LIM3D_MODELMAKE__H

//#include <./Lim3D/rti_ClassCaretaker.h>
#include <./Lim3D/rti_SuperList.h>
#include <./Lim3D/rti_Lim3DModel.h>

class TLim3DMake : public __TSuperListServer<TLim3DModel>
{
public:

	//�R���X�g���N�^
	TLim3DMake(){m_ParenceModel = NULL;}
	//�e���f����������.
	bool Mount_form_Model(TLim3DModel* ParenceModel)
	{
		if (m_ParenceModel != NULL)	return false;	//��q������.
		m_ParenceModel = ParenceModel;
		return true;
	}
	//�e�̃��f���擾.
	TLim3DModel* GetParenceModel(){ return m_ParenceModel; }
protected:
	TLim3DModel* m_ParenceModel;			//�e���f��.
private:
};

#endif				//�d���h�~