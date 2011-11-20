// RSpliter.cpp: RSpliter �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RSpliter.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RSpliter::RSpliter()
{
	this->RubberBandSize = 10;
	this->OldWidth = 0;
}

RSpliter::~RSpliter()
{

}


//�Ђ���{�^���������ꂽ
bool RSpliter::OnLButtonDown(int x,int y)
{
	ASSERT(ParentWnidow != NULL);

	//���E�B���h�E�̕��̎擾
	RECT leftRect;
	this->getWindowRect(true , &leftRect);
	// MouseDiff��ݒ�
	MouseDiff=x-(leftRect.right-leftRect.left);

	// �}�E�X���L���v�`��
	SetCapture(ParentWnidow);
	// �X�v���b�^�[��`��
	this->DrawSpliter(x-MouseDiff);
	OldWidth = x-MouseDiff;

	return false;
}

//�Ђ���{�^���������ꂽ
bool RSpliter::OnLButtonUp(int x,int y)
{
	ASSERT(ParentWnidow != NULL);
	if(GetCapture()==ParentWnidow)
	{
		// �X�v���b�^�[������
		this->DrawSpliter(OldWidth);
		// �����E�B���h�E�̕���ݒ�
		this->setLeftWindowSize(x-MouseDiff);
		// �}�E�X�����
		ReleaseCapture();

		// ���C���E�B���h�E�̃N���C�A���g�T�C�Y���擾
		RECT mainWindowRect;
		GetClientRect(this->ParentWnidow,&mainWindowRect);

		//������Ɖ������@�����ǁA���C���E�B���h�E�� WM_SIZE �𑗐M���܂�(w
		::PostMessage(this->ParentWnidow , WM_SIZE , SIZE_RESTORED , MAKELPARAM(mainWindowRect.right,mainWindowRect.bottom) );

		return true;
	}
	return false;
}

//�}�E�X���ړ�����
bool RSpliter::OnMouseMove(int x,int y)
{
	ASSERT(this->ParentWnidow != NULL);
	if(GetCapture() == this->ParentWnidow)
	{
		// �X�v���b�^�[������
		this->DrawSpliter(OldWidth);
		// �X�v���b�^�[��`��
		this->DrawSpliter(x-MouseDiff);
		// �X�v���b�^�[��`�悵���ʒu���L�^
		OldWidth = x-MouseDiff;
		return true;
	}
	return false;
}

// ���̃E�B���h�E�̃T�C�Y�𐳋K�����ĕԂ�
int RSpliter::RegularWidth(int inWidth) const
{
	//�}�C�i�X�˂����ނ̂͂܂���������.
	if(inWidth < 1)		return 1;

	// ���C���E�B���h�E�̃N���C�A���g�T�C�Y���擾
	RECT MainWindowRect;
	GetClientRect(this->ParentWnidow,&MainWindowRect);

	//���C���E�B���h�E���ł����̂͂܂���������.
	if(inWidth + this->RubberBandSize+1 > MainWindowRect.right)
		return MainWindowRect.right - this->RubberBandSize-1;
	return inWidth;
}


//�X�v���b�^�[��`��.
void RSpliter::DrawSpliter(int inWidth) const
{
	ASSERT(this->ParentWnidow != NULL);

	int width=this->RegularWidth(inWidth);
	// �X�v���b�^�[��`��
	HDC hScreenDc = GetDC( 0 );

	//���C���E�B���h�E��RECT
	RECT mainRect;
	::GetWindowRect(this->ParentWnidow , &mainRect);
	//���E�B���h�E�̕��̎擾
	RECT leftRect;
	this->getWindowRect(true , &leftRect);

	PatBlt(hScreenDc ,
		mainRect.left + leftRect.left + width ,
		mainRect.top + leftRect.top,
		this->RubberBandSize ,
		leftRect.bottom - leftRect.top,
		DSTINVERT);
	ReleaseDC( 0 ,hScreenDc);
}



//���o�[�o���h�̃T�C�Y�̐ݒ�
void RSpliter::setRubberBandSize(int inRubberBandSize)
{
	this->RubberBandSize = inRubberBandSize;
}

//���o�[�����h�̍�/�E�̃T�C�Y�̎擾
void RSpliter::getWindowRect(bool IsLeft,RECT* outRect) const
{
	// ���C���E�B���h�E�̃N���C�A���g�T�C�Y���擾
	RECT mainWindowRect;
	GetClientRect(this->ParentWnidow,&mainWindowRect);

	//���o�[�o���h�̕����@���E�̃E�B���h�E�ł��̂� / 2 �ł��B
	int corrodeSize = this->RubberBandSize / 2;

	//���E�̌v�Z
	RECT retRect;
	if (IsLeft)
	{
		retRect.left = 0;
		retRect.top = 0;
		retRect.right = this->LeftWindowSize - corrodeSize;
		retRect.bottom = mainWindowRect.bottom;
		*outRect = retRect;
	}
	else
	{
		retRect.left = this->LeftWindowSize + corrodeSize;
		retRect.top = 0;
		retRect.right = mainWindowRect.right - corrodeSize;
		retRect.bottom = mainWindowRect.bottom;

		*outRect = retRect;
	}
}


//�e�E�B���h�E
void RSpliter::setParentWnidow(HWND inParentWnidow)
{
	this->ParentWnidow = inParentWnidow;
}

//���E�B���h�E�̕��̐ݒ�
void RSpliter::setLeftWindowSize(int inNewLeftWidth)
{
	this->LeftWindowSize = this->RegularWidth(inNewLeftWidth);
}

//���E�B���h�E�̕��̐ݒ�
void RSpliter::setLeftWindowSize(double inNewLeftParsent)
{
	// ���C���E�B���h�E�̃N���C�A���g�T�C�Y���擾
	RECT mainWindowRect;
	GetClientRect(this->ParentWnidow,&mainWindowRect);

	int newWidth = (int) (mainWindowRect.right * inNewLeftParsent);
	this->setLeftWindowSize(newWidth);
}
