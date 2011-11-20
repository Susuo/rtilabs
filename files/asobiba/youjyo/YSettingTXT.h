// YSettingTXT.h: YSettingTXT �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YSETTINGTXT_H__22F20E01_266A_40C8_94AC_D0B644FEFD15__INCLUDED_)
#define AFX_YSETTINGTXT_H__22F20E01_266A_40C8_94AC_D0B644FEFD15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RIOCannotOpenException.h"
	//SettingTxt�ł� bool�^.
	class SettingTxtBool
	{
	public:
		SettingTxtBool(bool inB)
		{
			Check = inB;
		}
		SettingTxtBool(string inStr)
		{
			if ( inStr == "1" )	Check = true;
			else if ( inStr == "checked" )	Check = true;
			else		Check = false;
		}
		bool get() const
		{
			return Check;
		}

		operator bool()
		{
			return Check;
		}
	private:
		bool	Check;

	};
	

class YSettingTXT  
{
public:
	YSettingTXT();
	virtual ~YSettingTXT();

	void Load(const string inBoardName);


	/*
		BBS_TITLE		�̖��O

		��:
			BBS_TITLE=�i���Z��2ch�f����
		�ڍ�
			�f���̖��O�ł��B
			�����Őݒ肵�����O�� index.html �Ȃǂɕ\������܂��B
		�ȗ���
			��������
	*/
	string getBBS_TITLE() const{	return BBS_TITLE;	}
	/*
		BBS_NONAME_NAME	�f�B�t�H���g�̖��O

		��:
			BBS_NONAME_NAME=�������̊i����
		�ڍ�
			�f���̖��O�ł��B
			�����Őݒ肵�����O�� index.html �Ȃǂɕ\������܂��B
		�ȗ���
			���������񁗂��Ȃ������ς�
	*/
	string getBBS_NONAME_NAME() const{	return BBS_NONAME_NAME;	}
	/*
		BBS_PROXY_CHECK	PROXY����

		��:
			BBS_PROXY_CHECK=checked
			BBS_PROXY_CHECK=
		�ڍ�
			�v���L�V�`�F�b�N�����邩�ǂ����ł��B
			checked �ɂ���ƃv���L�V�`�F�b�N�ɂȂ�܂��B
			���������Ȃ��ꍇ�̓v���L�V�`�F�b�N�͍s��Ȃ��Ƃ������Ƃł��B
		�ȗ���
			�`�F�b�N�Ȃ�
	*/
	bool getBBS_PROXY_CHECK() const{	return BBS_PROXY_CHECK;	}
	/*
		BBS_NO_ID	�C��ID

		��:
			BBS_NO_ID=checked
			BBS_NO_ID=
		�ڍ�
			�C��ID�����邩�ǂ����ł��B
			checked �ɂ���ƔC��ID�ɂȂ�܂��B
			���������Ȃ��ꍇ�͔C��ID�͍s��Ȃ��Ƃ������Ƃł��B
		�ȗ���
			�C��ID�Ȃ�
	*/
	bool getBBS_NO_ID() const{	return BBS_NO_ID;	}
	/*
		BBS_FORCE_ID	����ID

		��:
			BBS_FORCE_ID=checked
			BBS_FORCE_ID=
		�ڍ�
			����ID�����邩�ǂ����ł��B
			checked �ɂ���Ƌ���ID�ɂȂ�܂��B
			���������Ȃ��ꍇ�͋���ID�͍s��Ȃ��Ƃ������Ƃł��B

			���̃p�����[�^��BBS_NO_ID ��苭���̂ŁA �����w�肳�ꂽ�ꍇ�́A
			BBS_FORCE_ID���D�悳��܂��B
		�ȗ���
			�C��ID�Ȃ�
	*/
	bool getBBS_FORCE_ID() const{	return BBS_FORCE_ID;	}
	/*
		BBS_DELETE_NAME	�폜���̖��O

		��:
			BBS_DELETE_NAME=���ځ[��
		�ڍ�
			�폜���Ɏg�p����閼�O�ł��B

		�ȗ���
			���ځ[��
	*/
	string getBBS_DELETE_NAME() const{	return BBS_DELETE_NAME;	}
	/*
		BBS_SUBJECT_COUNT	�^�C�g���̒���

		��:
			BBS_SUBJECT_COUNT=60
		�ڍ�
			�^�C�g���Ɏg�p�ł��镶����̒����ł��B
			>>1 ���񂪂����ˁB

		�ȗ���
			60
	*/
	int getBBS_SUBJECT_COUNT() const{	return BBS_SUBJECT_COUNT;	}
	/*
		BBS_NAME_COUNT	���O�̒���

		��:
			BBS_NAME_COUNT=30
		�ڍ�
			���O�̒����ł��B
			�ʏ폑�����ݎ��ɂ��閼�O�ł��ˁB
			���� ���������� �Ȃ̂Ŋ֌W�Ȃ��ł���(w

		�ȗ���
			30
	*/
	int getBBS_NAME_COUNT() const{	return BBS_SUBJECT_COUNT;	}
	/*
		BBS_MAIL_COUNT	���[���A�h���X�̒���

		��:
			BBS_MAIL_COUNT=30
		�ڍ�
			���[���A�h���X�̒����ł��B
			�ʏ폑�����ݎ��ɂ��郁�[���A�h���X�ł��ˁB
			���� age/sage �Ȃ̂Ŋ֌W�Ȃ��ł���(w

		�ȗ���
			30
	*/
	int getBBS_MAIL_COUNT() const{	return BBS_MAIL_COUNT;	}
	/*
		BBS_MESSAGE_COUNT	���b�Z�[�W�̃p�C�g��

		��:
			BBS_MESSAGE_COUNT=2048
		�ڍ�
			���[���A�h���X�̒����ł��B
			�ʏ폑�����ݎ��ɂ��郁�[���A�h���X�ł��ˁB
			���� age/sage �Ȃ̂Ŋ֌W�Ȃ��ł���(w

		�ȗ���
			2048
	*/
	int getBBS_MESSAGE_COUNT() const{	return BBS_MESSAGE_COUNT;	}
	/*
		SUBBBS_CGI_ON	�V�K�X���b�h���Ă鎞�̊m�F����

		��:
			SUBBBS_CGI_ON=checked
			SUBBBS_CGI_ON=
		�ڍ�
			�V�K�X���b�h���Ă鎞�̊m�F�����邩�ǂ����ł��B
			checked �ɂ���ƐV�K�X���b�h���Ă鎞�̊m�F�ɂȂ�܂��B
			���������Ȃ��ꍇ�͐V�K�X���b�h���Ă鎞�̊m�F�͍s��Ȃ��Ƃ������Ƃł��B

		�ȗ���
			�Ȃ�
	*/
	bool getSUBBBS_CGI_ON() const{	return SUBBBS_CGI_ON;	}
	/*
		NANASHI_CHECK	�����̋֎~

		��:
			NANASHI_CHECK=checked
			NANASHI_CHECK=
		�ڍ�
			���O�̏ȗ��������Ȃ����ǂ����ł��B
			checked �ɂ���Ɩ��O�̏ȗ��������Ȃ��Ȃ�܂��B
			���������Ȃ��ꍇ�͖��O�̏ȗ��������ABBS_NONAME_NAME �Őݒ肵�����O�ɂȂ�܂��B

		�ȗ���
			�Ȃ�
	*/
	bool getNANASHI_CHECK() const{	return NANASHI_CHECK;	}
	/*
		BBS_NOTJP_FILTER	.jp�ȊO�̃z�X�g����̏������݋֎~

		��:
			BBS_NOTJP_FILTER=checked
			BBS_NOTJP_FILTER=
		�ڍ�
			.jp(JPNIC���Ǘ�����)IP�ȊO����̏������݂��֎~���܂��B
			checked �ɂ���Ə������݋֎~�Ƃ��ē��삵�܂��B
			���������Ȃ��ꍇ�͋K���͔�������܂���B

		�ȗ���
			�Ȃ�
	*/
	bool	getBBS_NOTJP_FILTER() const{	return BBS_NOTJP_FILTER;	}
	/*
		SAKUBBS	�폜�˗��f���̖��O (�������폜�˗��̏ꍇ�͌��̖��O)

		��:
			SAKUBBS=animesaku
		�ڍ�
			���̌f���ɑ΂���폜�˗��̖��O�ł��B
			�������폜�˗��̏ꍇ�́A���̖��O�������Ă��������B

		�ȗ���
			�ȗ��͊�{�I�ɋ�����܂���!!!
	*/
	string getSAKUBBS() const{	return SAKUBBS;	}
	/*
		IS_SAKUBBS	�������폜�˗��Ȃ̂��ǂ���

		��:
			IS_SAKUBBS=checked
			IS_SAKUBBS=
		�ڍ�
			�������폜�˗��ł��邩�ǂ����ł��B
			checked �ɂ���ƍ폜�˗��Ƃ��ē��삵�܂��B
			���������Ȃ��ꍇ�͒ʏ�̔Ƃ��ē��삵�܂��B

		�ȗ���
			�Ȃ�
	*/
	bool getIS_SAKUBBS() const{	return IS_SAKUBBS;	}
	/*
		DELETE_WAIT	�폜�܂Ńv�[������b��

		��:
			DELETE_WAIT=86400
		�ڍ�
			�폜�˗���������Ă���A�폜����܂ł̕b���ł��B
			�f�B�t�H���g�͈���ł��B

		�ȗ���
			86400
	*/
	int getDELETE_WAIT() const{	return DELETE_WAIT;	}
	/*
		BBS_DISP_IP	IP�̕\��

		��:
			BBS_DISP_IP=checked
			BBS_DISP_IP=
		�ڍ�
			IP��\�����邩�ǂ����ł��B
			checked �ɂ����IP�\���Ƃ��ē��삵�܂��B
			���������Ȃ��ꍇ��IP��\�����܂���B
			�폜�˗��ł� �\�����邱�Ƃ������߂��܂��B

		�ȗ���
			�Ȃ�
	*/
	bool	getBBS_DISP_IP() const{ return BBS_DISP_IP; }

private:
	/*
		BBS_TITLE		�̖��O

		��:
			BBS_TITLE=�i���Z��2ch�f����
		�ڍ�
			�f���̖��O�ł��B
			�����Őݒ肵�����O�� index.html �Ȃǂɕ\������܂��B
		�ȗ���
			��������
	*/
	string	BBS_TITLE;
	/*
		BBS_NONAME_NAME	�f�B�t�H���g�̖��O

		��:
			BBS_NONAME_NAME=�������̊i����
		�ڍ�
			�f���̖��O�ł��B
			�����Őݒ肵�����O�� index.html �Ȃǂɕ\������܂��B
		�ȗ���
			���������񁗂��Ȃ������ς�
	*/
	string	BBS_NONAME_NAME;
	/*
		BBS_PROXY_CHECK	PROXY����

		��:
			BBS_PROXY_CHECK=checked
			BBS_PROXY_CHECK=
		�ڍ�
			�v���L�V�`�F�b�N�����邩�ǂ����ł��B
			checked �ɂ���ƃv���L�V�`�F�b�N�ɂȂ�܂��B
			���������Ȃ��ꍇ�̓v���L�V�`�F�b�N�͍s��Ȃ��Ƃ������Ƃł��B
		�ȗ���
			�`�F�b�N�Ȃ�
	*/
	bool	BBS_PROXY_CHECK;
	/*
		BBS_NO_ID	�C��ID

		��:
			BBS_NO_ID=checked
			BBS_NO_ID=
		�ڍ�
			�C��ID�����邩�ǂ����ł��B
			checked �ɂ���ƔC��ID�ɂȂ�܂��B
			���������Ȃ��ꍇ�͔C��ID�͍s��Ȃ��Ƃ������Ƃł��B
		�ȗ���
			�C��ID�Ȃ�
	*/
	bool	BBS_NO_ID;
	/*
		BBS_FORCE_ID	����ID

		��:
			BBS_FORCE_ID=checked
			BBS_FORCE_ID=
		�ڍ�
			����ID�����邩�ǂ����ł��B
			checked �ɂ���Ƌ���ID�ɂȂ�܂��B
			���������Ȃ��ꍇ�͋���ID�͍s��Ȃ��Ƃ������Ƃł��B

			���̃p�����[�^��BBS_NO_ID ��苭���̂ŁA �����w�肳�ꂽ�ꍇ�́A
			BBS_FORCE_ID���D�悳��܂��B
		�ȗ���
			�C��ID�Ȃ�
	*/
	bool	BBS_FORCE_ID;
	/*
		BBS_DELETE_NAME	�폜���̖��O

		��:
			BBS_DELETE_NAME=���ځ[��
		�ڍ�
			�폜���Ɏg�p����閼�O�ł��B

		�ȗ���
			���ځ[��
	*/
	string	BBS_DELETE_NAME;
	/*
		BBS_SUBJECT_COUNT	�^�C�g���̒���

		��:
			BBS_SUBJECT_COUNT=60
		�ڍ�
			�^�C�g���Ɏg�p�ł��镶����̒����ł��B
			>>1 ���񂪂����ˁB

		�ȗ���
			60
	*/
	int		BBS_SUBJECT_COUNT;
	/*
		BBS_NAME_COUNT	���O�̒���

		��:
			BBS_NAME_COUNT=30
		�ڍ�
			���O�̒����ł��B
			�ʏ폑�����ݎ��ɂ��閼�O�ł��ˁB
			���� ���������� �Ȃ̂Ŋ֌W�Ȃ��ł���(w

		�ȗ���
			30
	*/
	int		BBS_NAME_COUNT;
	/*
		BBS_MAIL_COUNT	���[���A�h���X�̒���

		��:
			BBS_MAIL_COUNT=30
		�ڍ�
			���[���A�h���X�̒����ł��B
			�ʏ폑�����ݎ��ɂ��郁�[���A�h���X�ł��ˁB
			���� age/sage �Ȃ̂Ŋ֌W�Ȃ��ł���(w

		�ȗ���
			30
	*/
	int		BBS_MAIL_COUNT;
	/*
		BBS_MESSAGE_COUNT	���b�Z�[�W�̃p�C�g��

		��:
			BBS_MESSAGE_COUNT=2048
		�ڍ�
			���[���A�h���X�̒����ł��B
			�ʏ폑�����ݎ��ɂ��郁�[���A�h���X�ł��ˁB
			���� age/sage �Ȃ̂Ŋ֌W�Ȃ��ł���(w

		�ȗ���
			2048
	*/
	int		BBS_MESSAGE_COUNT;
	/*
		SUBBBS_CGI_ON	�V�K�X���b�h���Ă鎞�̊m�F����

		��:
			SUBBBS_CGI_ON=checked
			SUBBBS_CGI_ON=
		�ڍ�
			�V�K�X���b�h���Ă鎞�̊m�F�����邩�ǂ����ł��B
			checked �ɂ���ƐV�K�X���b�h���Ă鎞�̊m�F�ɂȂ�܂��B
			���������Ȃ��ꍇ�͐V�K�X���b�h���Ă鎞�̊m�F�͍s��Ȃ��Ƃ������Ƃł��B

		�ȗ���
			�Ȃ�
	*/
	bool	SUBBBS_CGI_ON;
	/*
		NANASHI_CHECK	�����̋֎~

		��:
			NANASHI_CHECK=checked
			NANASHI_CHECK=
		�ڍ�
			���O�̏ȗ��������Ȃ����ǂ����ł��B
			checked �ɂ���Ɩ��O�̏ȗ��������Ȃ��Ȃ�܂��B
			���������Ȃ��ꍇ�͖��O�̏ȗ��������ABBS_NONAME_NAME �Őݒ肵�����O�ɂȂ�܂��B

		�ȗ���
			�Ȃ�
	*/
	bool	NANASHI_CHECK;
	/*
		BBS_NOTJP_FILTER	.jp�ȊO�̃z�X�g����̏������݋֎~

		��:
			BBS_NOTJP_FILTER=checked
			BBS_NOTJP_FILTER=
		�ڍ�
			.jp(JPNIC���Ǘ�����)IP�ȊO����̏������݂��֎~���܂��B
			checked �ɂ���Ə������݋֎~�Ƃ��ē��삵�܂��B
			���������Ȃ��ꍇ�͋K���͔�������܂���B

		�ȗ���
			�Ȃ�
	*/
	bool	BBS_NOTJP_FILTER;
	/*
		SAKUBBS	�폜�˗��f���̖��O (�������폜�˗��̏ꍇ�͌��̖��O)

		��:
			SAKUBBS=animesaku
		�ڍ�
			���̌f���ɑ΂���폜�˗��̖��O�ł��B
			�������폜�˗��̏ꍇ�́A���̖��O�������Ă��������B

		�ȗ���
			�ȗ��͊�{�I�ɋ�����܂���!!!
	*/
	string	SAKUBBS;
	/*
		IS_SAKUBBS	�������폜�˗��Ȃ̂��ǂ���

		��:
			IS_SAKUBBS=checked
			IS_SAKUBBS=
		�ڍ�
			�������폜�˗��ł��邩�ǂ����ł��B
			checked �ɂ���ƍ폜�˗��Ƃ��ē��삵�܂��B
			���������Ȃ��ꍇ�͒ʏ�̔Ƃ��ē��삵�܂��B

		�ȗ���
			�Ȃ�
	*/
	bool	IS_SAKUBBS;
	/*
		DELETE_WAIT	�폜�܂Ńv�[������b��

		��:
			DELETE_WAIT=86400
		�ڍ�
			�폜�˗���������Ă���A�폜����܂ł̕b���ł��B
			�f�B�t�H���g�͈���ł��B

		�ȗ���
			86400
	*/
	int		DELETE_WAIT;

	/*
		BBS_DISP_IP	IP�̕\��

		��:
			BBS_DISP_IP=checked
			BBS_DISP_IP=
		�ڍ�
			IP��\�����邩�ǂ����ł��B
			checked �ɂ����IP�\���Ƃ��ē��삵�܂��B
			���������Ȃ��ꍇ��IP��\�����܂���B
			�폜�˗��ł� �\�����邱�Ƃ������߂��܂��B

		�ȗ���
			�Ȃ�
	*/
	bool	BBS_DISP_IP;


/*
	�@����̈Ⴂ�ɂ��T�|�[�g�܂���
	��ɐF�֌W�̓e���v���[�g���g�����߃T�|�[�g���܂���B

//../2ch.gif << �o�i�[�摜��URL
string	BBS_TITLE_PICTURE;
//#000000
string	BBS_TITLE_COLOR;
//http://www.2ch.net/info.html
//�X���b�h���Ă����ł�
int		BBS_THREAD_TATESUGI=64
string	BBS_TITLE_LINK;
string	BBS_BG_PICTURE=../ba.gif
string	BBS_BG_COLOR=#FFFFFF
string	BBS_MAKETHREAD_COLOR=#CCFFCC
string	BBS_MENU_COLOR=#CCFFCC
string	BBS_THREAD_COLOR=#EFEFEF
string	BBS_TEXT_COLOR=#000000
string	BBS_NAME_COLOR=forestgreen
string	BBS_LINK_COLOR=#0000FF
string	BBS_ALINK_COLOR=#FF0000
string	BBS_VLINK_COLOR=#660099
string	BBS_SUBJECT_COLOR=#FF0000
bool	BBS_PASSWORD_CHECK=
bool	BBS_NAMECOOKIE_CHECK;
bool	BBS_MAILCOOKIE_CHECK=checked

  �Ӗ����킩��Ȃ�����T�|�[�g���܂���(w
string	BBS_SLIP=
string	BBS_UNICODE=change
bool	BBS_RAWIP_CHECK;
int		timecheck=1
int		timecount=10
int		timeclose=3
bool	BBS_NEWSUBJECT;
string	BBS_AD2
string	BBS_OVERSEA_THREAD=checked
string	BBS_OVERSEA_PROXY=checked

  	int		BBS_THREAD_NUMBER=20
	int		BBS_CONTENTS_NUMBER=10
	int		BBS_LINE_NUMBER=30
	int		BBS_MAX_MENU_THREAD=100
*/
};

#endif // !defined(AFX_YSETTINGTXT_H__22F20E01_266A_40C8_94AC_D0B644FEFD15__INCLUDED_)
