// BUserDataIconDlg.cpp: BUserDataIconDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BUserDataIconDlg.h"
#include "WindowsMain.h"
#include "resrc1.h"
#include "BConf.h"
#include "RMessageBox.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

BUserDataIconDlg::BUserDataIconDlg()
{

}

BUserDataIconDlg::~BUserDataIconDlg()
{

}

void BUserDataIconDlg::Create(const RGISSmallShareImage* poolShareImage , int inNowIconNumber ,
							  const string & inName , const string & inCatego , const string & inSubCatego) 
{

	try
	{
		//�����t�@�C���̓ǂݍ���.
		Load();
	}
	catch(RException e)
	{
		RMessageBox::Message( e.getMessage() );
	}

	this->PoolShareImage = poolShareImage;
	this->NowIconNumber = inNowIconNumber;
	this->Name = inName;
	this->Catego = inCatego;
	this->SubCatego = inSubCatego;
}

void BUserDataIconDlg::OnInit() 
{
	ScrollBar.MountWindow( this->GetDlgItem(IDC_ICON_SCROLLBAR) );
	IconMessageEdit.MountWindow( this->GetDlgItem(IDC_ICON_MESSAGE_EDIT) );

	//�A�C�R���̐��� �X�N���[���o�[�͈̔͂ɂȂ�܂�.
	const RDIBBitmap * icon = this->PoolShareImage->getIconImage();
	int scrollMax = icon->getWidth() / 16;

	SCROLLINFO info = {0};
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	info.nMin = 0;
	info.nMax = scrollMax - 1;
	info.nPos = this->NowIconNumber;
	ScrollBar.SetScrollInfo(SB_CTL , &info , TRUE);

	//�I������Ă���A�C�R���̐������e�L�X�g�{�b�N�X�ɕ\������.
	IconMessageEdit.setText( getIconMessage(this->NowIconNumber) );
}


void BUserDataIconDlg::OnPaint()
{
	RECT rc;
	this->getControlClientRect(this->GetDlgItem(IDC_ICON_PALLET) ,&rc);

	//�����ɓ��邾���A�C�R����`��.
	int width = (rc.right - rc.left) / 16 * 16;

    HDC hdc;
    PAINTSTRUCT paint;

	WindowsMain * winMain = WindowsMain::getMainSystem();
	HWND window = this->getWindowHandle();
    hdc = BeginPaint( window , &paint);
	::PatBlt(hdc , rc.left , rc.top , rc.right - rc.left , 16 , BLACKNESS );
	::PatBlt(hdc , rc.left - 5, rc.top - 5, 16 + 5 , 16 + 5, WHITENESS );

	::BitBlt(hdc  , rc.left , rc.top , width , 16 , 
		PoolShareImage->getIconImage()->getConstDC() , 16 * NowIconNumber , 0 , SRCCOPY);
    EndPaint(window, &paint);
}

void BUserDataIconDlg::OnScroll(WPARAM inWParam)
{
	//���݂̏�Ԃ��擾.
	SCROLLINFO info = {0};
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	ScrollBar.GetScrollInfo(SB_CTL , &info );

	//�X�N���[���ʂ̌v�Z
	if ( !ScrollBar.CalsScroll(&info , inWParam) )	return ;

	//�X�N���[���o�[���X�V.
	ScrollBar.SetScrollInfo(SB_CTL , &info , TRUE);

	//���ݑI������Ă���A�C�R���̍X�V.
	this->NowIconNumber = info.nPos;

	//�I������Ă���A�C�R���̐������e�L�X�g�{�b�N�X�ɕ\������.
	IconMessageEdit.setText( getIconMessage(this->NowIconNumber) );

	//��ʂ��X�V.
	InvalidateRect( this->getWindowHandle() ,  NULL , FALSE);
}


//�E�B���h�E�v���V�[�W��.
LRESULT BUserDataIconDlg::DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam)
{
	switch(inMsg)
	{
	case WM_INITDIALOG:
		OnInit() ;
#ifdef _WIN32_WCE
		SHINITDLGINFO shidi;
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
		shidi.hDlg = ioHwnd;
		SHInitDialog(&shidi);
#endif
		return TRUE;
	case WM_COMMAND:
		{
			switch (LOWORD(inWParam)) 
			{
			case IDOK:	//��������
				this->EndDialog(1);
				break;
			case IDCANCEL:	//��ݾ�
				this->EndDialog(0);
				break;
			case IDC_AUTO_BUTTON:	//�����I��.
				{
					int no = AutoMatch(Name , Catego , SubCatego);
					if (no != -1)
					{
						//�ݒ�̍X�V
						this->NowIconNumber = no;

						//�X�N���[���o�[�̐ݒ�
						ScrollBar.setPos(no , SB_CTL);

						//�I������Ă���A�C�R���̐������e�L�X�g�{�b�N�X�ɕ\������.
						IconMessageEdit.setText( getIconMessage(this->NowIconNumber) );

						//��ʂ��X�V.
						InvalidateRect( this->getWindowHandle() ,  NULL , FALSE);
					}
				}
				break;
			}
		}
		break;
	case WM_HSCROLL:
		OnScroll(inWParam);
		break;
	case WM_PAINT:
		OnPaint();
		break;
	}
	return FALSE;
}


//�����̓ǂݍ���
void BUserDataIconDlg::Load()
{
	RStdioFile file;
	file.Open(BConf::getConf()->getCurrentDirectory() + "tool_bit.txt" , "rb");

	char messageBuffer[256];
	char matchListBuffer[256];
	
	Text.clear();

	char spaceBuffer[1024];

	for(int nowLine = 0 ;  !file.IsEof() ; nowLine ++ )
	{
		string line = file.ReadLine();
		//��s�܂��́A�R�����g�͖���.
		if ( line.empty() || line[0] == '#' || line == "\r\n") continue;

		int r = sscanf(line.c_str() , "%256s%1024[ ]%s256", messageBuffer , spaceBuffer , matchListBuffer);
		if (r < 1) 
		{
			throw RException(EXCEPTIONTRACE + "tool_bit.txt �t�@�C����" + num2str(nowLine) + "�s�ڂ̓ǂݍ��݂Ɏ��s���܂���");
		}

		//�������Ȃ��ꍇ����肪�����Ȃ��悤�ɁB
		if (r != 3)
		{
			matchListBuffer[0] = '\0';
		}
		//�i�[.
		BToolBitText* p = new BToolBitText;
		p->Create(messageBuffer , matchListBuffer);
		Text.push_back( p );
	}
}

//�����I��
int BUserDataIconDlg::AutoMatch(const string & inName , const string & inCatego , const string & inSubCatego) const
{
	if ( Text.empty() ) return -1;

	newVector<BToolBitText>::const_iterator i = Text.begin();
	newVector<BToolBitText>::const_iterator e = Text.end();

	//�q�b�g�񐔕ۑ���.
	int * hitCount = new int[ Text.size() + 1 ];
	memset(hitCount , 0 , Text.size() * sizeof(int) );

	int num;
	for(num = 0 ; i != e ; i++ , num ++)
	{
		const RStringList* list = (*i)->getMatchList();
		//���O�Ƀq�b�g���Ă����� 1�_
		if ( TextFind(list , inName) )
		{
			hitCount[num] += 1;
		}
		//�J�e�S���Ƀq�b�g���Ă����� 2�_
		if ( TextFind(list , inCatego) )
		{
			hitCount[num] += 2;
		}
		//�T�u�J�e�S���Ƀq�b�g���Ă����� 2�_
		if ( TextFind(list , inSubCatego) )
		{
			hitCount[num] += 2;
		}
	}

	//��ԃq�b�g���Ă������Ȃ��I��.
	int maxIndex = - 1;
	int max = 0;
	for( ; num >= 0; num --)
	{
		if (hitCount[num] > max)
		{
			max = hitCount[num] ;
			maxIndex = num;
		}
	}

	//��ԃq�b�g���Ă�������A��.
	return maxIndex;
}

//���X�g�̒��̕������ inName �̕�������v���Ă��邩����.
bool BUserDataIconDlg::TextFind(const RStringList* inList , const string & inName) const
{
	RStringList::const_iterator i = inList->begin();
	RStringList::const_iterator e = inList->end();

	for( ; i != e ; i ++)
	{
		string aaa = *i;

		unsigned int size = inName.size();
		if ( i->size() >= size) continue;

		//�Ōオ��v���Ă��邩����.
		const char * p = inName.c_str() + inName.size() - i->size();
		if ( strcmp( i->c_str() , p ) == 0 ) return true;
	}

	return false;
}


//�w�肳�ꂽ�A�C�R���̐����̎擾
const string BUserDataIconDlg::getIconMessage(int inIconSelect) const
{
	if (Text.size() <= (unsigned int) inIconSelect ) return "";

	return Text[inIconSelect]->getMessage();
}
