// evalDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_EVALDLG_H__D892FCD6_B303_4B50_AA8C_5827CA713E3C__INCLUDED_)
#define AFX_EVALDLG_H__D892FCD6_B303_4B50_AA8C_5827CA713E3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CEvalDlg �_�C�A���O

class CEvalDlg : public CDialog
{
// �\�z
public:
	CEvalDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CEvalDlg)
	enum { IDD = IDD_EVAL_DIALOG };
	CString	m_EvalString;
	CString	m_AnswerString;
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CEvalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CEvalDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDescentParsing();
	afx_msg void OnCheart();
	afx_msg void OnCancel();
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//�G���[�̈ʒu�����o�I�ɓ`����}�[�J�[��������쐬���܂�.
	CString MakeErrorMakerString(int inErrorPosstion) const;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EVALDLG_H__D892FCD6_B303_4B50_AA8C_5827CA713E3C__INCLUDED_)
