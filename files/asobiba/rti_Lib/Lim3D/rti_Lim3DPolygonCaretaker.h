#ifndef	___LIM3D_POLYGONCARETAKER__H	//�d���h�~
#define ___LIM3D_POLYGONCARETAKER__H

//#include <./Lim3D/rti_ClassCaretaker.h>
#include <./Lim3D/rti_SuperList.h>
#include <./Lim3D/rti_Lim3DPolygon.h>
#include <./base/rti_debug.h>

class TLim3DModel;

class TLim3DPolygonCaretaker : public __TSuperListServer<TLim3DPolygon>
{
public:
	TLim3DPolygonCaretaker(){m_ParenceModel = NULL;}
	~TLim3DPolygonCaretaker(){}

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
	TLim3DModel* m_ParenceModel;	//�e�̃��f��.
private:
};

#include <./Lim3D/rti_Lim3DModel.h>

#endif				//�d���h�~