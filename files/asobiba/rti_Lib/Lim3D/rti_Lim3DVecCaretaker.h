#ifndef	___LIM3D_VECCORETAKER_H	//�d���h�~
#define ___LIM3D_VECCORETAKER_H

#include <./Lim3D/rti_Caretaker.h>
#include <./Lim3D/rti_Lim3DDefine.h>

class TLim3DModel;

class	TLim3DVecCaretaker : public _TCaretaker<VecWorldEye , XYZFixed>
{
public:

	TLim3DVecCaretaker(){ m_ParenceModel = NULL;}
	~TLim3DVecCaretaker(){}

	//inVecCaretaker�̃R�s�[���Ƃ�.
	void CopyForm(TLim3DVecCaretaker * inVecCaretaker);

	//�e���f����������.
	bool Mount_form_Model(TLim3DModel * Model)
	{
		if (m_ParenceModel != NULL)	return false;	//��q������.
		m_ParenceModel = Model;
		return true;
	}
protected:
	//�f�[�^�R�s�[
	virtual	void	CopyData(VecWorldEye* DestData , XYZFixed* SrcCaretation , int Many)
	{	
		for(int i=0; i < Many ; i++)
		{
			DestData[i].Vec = SrcCaretation[i];
		}
	}
	//�V�K�Ƀ�������ǉ������������߂ɁA�������u���b�N���ω������ꍇ�ɌĂ΂��.
	//�������A�����߂����āA�������A���P�[�V�������[�`�������˂� >> ��. �΂����ˁ[.
	//����: OldUseSize �� ���ł�.
	virtual	void	ReAllocMemory(VecWorldEye* OldMemoryAddress , 
								  VecWorldEye* NewMemoryAddress ,
								  int OldUseSize)	;
	
	//���̒��_�f�[�^���Ǘ����Ă��郂�f���N���X
	TLim3DModel* m_ParenceModel;
private:
};

#include <./Lim3D/rti_Lim3DModel.h>

#endif							//�d���h�~
