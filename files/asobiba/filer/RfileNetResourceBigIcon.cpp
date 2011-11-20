// RfileNetResourceBigIcon.cpp: RfileNetResourceBigIcon �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfileNetResourceBigIcon.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//�o�^���܂��B
void RfileNetResourceBigIcon::Attach(const char* inName,const char* inComment,DWORD inDisplayType )
{
	this->Name = inName;
	if (inComment)	this->Comment = inComment;

	FileModifyChacheLargeWin32* fmc = FileModifyChacheLargeWin32::getInstance();
	switch (  inDisplayType )
	{
	case RESOURCEDISPLAYTYPE_DOMAIN:		//�h���C��.
	case RESOURCEDISPLAYTYPE_NETWORK:
	case RESOURCEDISPLAYTYPE_SHAREADMIN:
		this->FileModify = fmc->SearchSystem( FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NETRESOURCE_DOMAIN );
		break;
	case RESOURCEDISPLAYTYPE_SERVER:		//�T�[�o.
		this->FileModify = fmc->SearchSystem( FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NETRESOURCE_SERVER );
		break;
	case RESOURCEDISPLAYTYPE_SHARE:			//���L.
		this->FileModify = fmc->SearchSystem( FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NETRESOURCE_SHAREPOINT_DISK );
		break;
	case RESOURCEDISPLAYTYPE_GROUP:			//���[�N�O���[�v.
		this->FileModify = fmc->SearchSystem( FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NETRESOURCE_WORKGROUP );
		break;
	default:								//�s��.
		this->FileModify = fmc->SearchSystem( FileModifyChacheLargeWin32::Modifty::__SYSTEMICON_NORMALFILE );
		break;
	}

	ASSERT(this->FileModify);
}


//�l�̎擾.
const string RfileNetResourceBigIcon::getVars(__FILECOLUMN_TYPE inType)  const
{
	switch(inType)
	{
	case __FILECOLUMN_TYPE_NAME:			//���O
		return Name;
	case __FILECOLUMN_TYPE_COMMENT:			//�R�����g
		return Comment;
	}
	return "";
}

//�C���[�W�̎擾.
const int RfileNetResourceBigIcon::getImageIndex()  const
{
	return FileModify->getIndex();
}


