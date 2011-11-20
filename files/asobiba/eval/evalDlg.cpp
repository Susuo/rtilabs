// evalDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "eval.h"
#include "evalDlg.h"

#include "RecursiveDescentParsing.h"
#include "TokenParser.h"
#include "MSScriptEval.h"
#include "XLException.h"
#include <comdef.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEvalDlg �_�C�A���O

CEvalDlg::CEvalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEvalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEvalDlg)
	m_EvalString = _T("1+2*3+(1+1)*2");
	m_AnswerString = _T("");
	//}}AFX_DATA_INIT
	// ����: LoadIcon �� Win32 �� DestroyIcon �̃T�u�V�[�P���X��v�����܂���B
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEvalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEvalDlg)
	DDX_Text(pDX, IDC_EVALSTRING_EDIT, m_EvalString);
	DDX_Text(pDX, IDC_ANSWER_EDIT, m_AnswerString);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEvalDlg, CDialog)
	//{{AFX_MSG_MAP(CEvalDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDRECURSIVE_DESCENT_PARSING, OnDescentParsing)
	ON_BN_CLICKED(IDCHEART, OnCheart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEvalDlg ���b�Z�[�W �n���h��

BOOL CEvalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�
	
	// TODO: ���ʂȏ��������s�����͂��̏ꏊ�ɒǉ����Ă��������B
	
	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B

void CEvalDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// �N���C�A���g�̋�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R����`�悵�܂��B
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// �V�X�e���́A���[�U�[���ŏ����E�B���h�E���h���b�O���Ă���ԁA
// �J�[�\����\�����邽�߂ɂ������Ăяo���܂��B
HCURSOR CEvalDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//�G���[�̈ʒu�����o�I�ɓ`����}�[�J�[��������쐬���܂�.
CString CEvalDlg::MakeErrorMakerString(int inErrorPosstion) const
{	
	CString r;
	for(int i = 0 ; i < inErrorPosstion ; i ++)
	{
		r += ' ';
	}
	r += "��������ӂł�";

	return r;
}

//�ċA���~�\����� �ŉ���
void CEvalDlg::OnDescentParsing() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	this->UpdateData(TRUE);
	
	//�v�Z����N���X
	RecursiveDescentParsing rdp;
	//����p�[�X
	TokenParser tp;
	bool r = tp.Set((const char*)this->m_EvalString);
	if (!r)
	{
		CString kokorahen = MakeErrorMakerString(tp.getErrorPosstion());
		
		CString message;
		message.Format("%d�����ڕt�߂������͒��ɃG���[��������܂���\r\n%s\r\n%s"
			,tp.getErrorPosstion()
			,(const char*)this->m_EvalString
			,(const char*)kokorahen
			);
		AfxMessageBox(message,MB_ICONHAND);
		return ;
	}
	double ans = 0;
	try
	{
		//�ċA���~�\�����
		ans = rdp.Run(&tp);
	}
	catch(XLException & e)
	{
		CString message;
		message.Format("�������s���ɃG���[���������܂���\r\n%s" , e.getMessage() );
		AfxMessageBox(message,MB_ICONHAND);
		return ;
	}

	//���ʂ̕\��
	this->m_AnswerString.Format("%.10g" , ans);

	this->UpdateData(FALSE);
}

//�`�[�g �ŉ���
void CEvalDlg::OnCheart() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	this->UpdateData(TRUE);

	//���s���� jscript �G���W��
	MSScriptEval mse;
	double ans = 0;
	try
	{
		//���s����
		ans = mse.Eval((const char*)this->m_EvalString);
	}
	catch(_com_error & e)
	{
		CString message;
		message.Format("���s���ɃG���[���������܂���\r\n%s" , e.ErrorMessage() );
		AfxMessageBox(message,MB_ICONHAND);
		return ;
	}
	if (mse.IsError())
	{
		CString kokorahen = MakeErrorMakerString(mse.getErrorPosstion());
		
		CString message;
		message.Format("%d�����ڕt�߂ɃG���[��������܂���\r\n�G���[���e:%s\r\n%s\r\n%s"
			,mse.getErrorPosstion()
			,mse.getErrorMessage().c_str()			//VC++�ł͂��̏������ł����v�Ȃ͂�
			,(const char*)this->m_EvalString
			,(const char*)kokorahen
			);
		AfxMessageBox(message,MB_ICONHAND);
		return ;
	}

	//���ʂ̕\��
	this->m_AnswerString.Format("%.10g" , ans);

	this->UpdateData(FALSE);
}



void CEvalDlg::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������

	//�Ƃ肠�����A�ʏ�̂����Ōv�Z���������Ă݂�.
	this->OnDescentParsing();

//	CDialog::OnOK();
}

void CEvalDlg::OnCancel() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	
	CDialog::OnCancel();
}
