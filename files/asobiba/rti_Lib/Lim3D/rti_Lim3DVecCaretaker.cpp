#include <./Lim3D/rti_Lim3DVecCaretaker.h>

//�V�K�Ƀ�������ǉ������������߂ɁA�������u���b�N���ω������ꍇ�ɌĂ΂��.
//�������A�����߂����āA�������A���P�[�V�������[�`�������˂� >> ��. �΂����ˁ[.
//����: OldUseSize �� ���ł�.
void	TLim3DVecCaretaker::ReAllocMemory(VecWorldEye* OldMemoryAddress , 
								  VecWorldEye* NewMemoryAddress ,
								  int OldUseSize)
{
	m_ParenceModel->ReAllocMemoryVec(OldMemoryAddress , NewMemoryAddress , OldUseSize);
}
