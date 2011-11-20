// RTaskLS.cpp: RTaskLS �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////


#include "comm.h"
#include "RTaskLS.h"
#include "RTaskManager.h"
#include "Filer.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RTaskLS::RTaskLS(Rfiles* inNewFiles ,string inMask , Display* inDisp , Window* inCallback) : RTask(inNewFiles,inDisp,inCallback)
{
	Mask = inMask;
}

RTaskLS::~RTaskLS()
{

}


//ls���s�J�n.
void RTaskLS::Execute( )
{
	Filer::getFiler()->getTaskView()->InsertTask( (unsigned long)this , getMessage());

	Files->Inc();
	bool ErrorFlg = false;
	try
	{
		if ( Mask.empty() )		Files->ls(  );
		else					Files->ls( Mask );
	}
	catch(RException e)
	{//ls ���ɃG���[
		ErrorFlg = true;
	}

	Files->Dec();
	Filer::getFiler()->getTaskView()->DeleteTask( (unsigned long)this );

	//�R�[���o�b�N.
	if (ErrorFlg)
		this->ErrorCallBack();
	else
		this->CallBack();
	

}

//�i���� �J�n0 ----> ���S100 
int RTaskLS::Parsent()
{
	return 0;
}

//�^�X�N�r���[�ɕ\�����郁�b�Z�[�W
string RTaskLS::getMessage()
{
	ASSERT(Files != NULL);

	return "�ꗗ�擾 "+ Files->getPath();
}

