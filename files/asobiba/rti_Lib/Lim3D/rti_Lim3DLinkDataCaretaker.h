#ifndef	___LIM3D_LINKDATACARETAKER__H	//�d���h�~
#define ___LIM3D_LINKDATACARETAKER__H

#include <./Lim3D/rti_Lim3DDefine.h>
#include <./Lim3D/rti_Caretaker.h>

//���_�f�[�^�Ǘ�.
class TLim3DLinkDataCaretaker : _TCaretaker<PolygonDataEx , PolygonDataEx>
{
	public:
	protected:
		//���.
		virtual	void Storage(PolygonDataEx *writeObject , 
					const  PolygonDataEx *inputData) 
		{
			*writeObject = *inputData;
		}
	private:
};

#endif			//�d���h�~
