#ifndef	___LIM3D_EYECARETAKER__H	//�d���h�~
#define ___LIM3D_EYECARETAKER__H

#include <./Lim3D/rti_ClassCaretaker.h>
#include <./Lim3D/rti_Lim3DEye.h>

class TLim3DEyeCaretaker : public TClassCaretaker<TLim3DEye>
{
public:
protected:
	//�쐬�����N���X�Ɏ������e���Ɠ`����.(TClassCaretaker���)
	virtual	bool Mount(TLim3DEye* AllocData)
	{	//�_�~�[
		return true;
	}
private:
};

#endif				//�d���h�~
