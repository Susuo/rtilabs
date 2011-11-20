// YSettingTXT.cpp: YSettingTXT �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "YSettingTXT.h"
#include "RConfigFile.h"
#include "YHttpdConf.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

YSettingTXT::YSettingTXT()
{

}

YSettingTXT::~YSettingTXT()
{

}

void YSettingTXT::Load(const string inBoardName)
{
	
	RConfigFile	Setting;

	const string filename = YHttpdConf::getInstance()->getDocumentRoot() + "/" + inBoardName + "/SETTING.TXT";
	//�ݒ�����[�h����̂ł���[.
	Setting.Load(filename);

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
	BBS_TITLE = Setting.LoadOrDefault("BBS_TITLE" , string("��������") );
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
	BBS_NONAME_NAME = Setting.LoadOrDefault("BBS_NONAME_NAME" , string("���������񁗂��Ȃ������ς�") );
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
	BBS_PROXY_CHECK = Setting.LoadOrDefault("BBS_PROXY_CHECK" , SettingTxtBool(false) );
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
	BBS_NO_ID = Setting.LoadOrDefault("BBS_NO_ID" , SettingTxtBool(false) );
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
	BBS_FORCE_ID = Setting.LoadOrDefault("BBS_FORCE_ID" , SettingTxtBool(false) );
	/*
		BBS_DELETE_NAME	�폜���̖��O

		��:
			BBS_DELETE_NAME=���ځ[��
		�ڍ�
			�폜���Ɏg�p����閼�O�ł��B

		�ȗ���
			���ځ[��
	*/
	BBS_DELETE_NAME = Setting.LoadOrDefault("BBS_DELETE_NAME" , string("���ځ[��") );
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
	BBS_SUBJECT_COUNT = Setting.LoadOrDefault("BBS_SUBJECT_COUNT" , 60 );
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
	BBS_NAME_COUNT = Setting.LoadOrDefault("BBS_NAME_COUNT" , 30 );
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
	BBS_MAIL_COUNT = Setting.LoadOrDefault("BBS_MAIL_COUNT" , 30 );
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
	BBS_MESSAGE_COUNT = Setting.LoadOrDefault("BBS_MESSAGE_COUNT" , 2048 );
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
	SUBBBS_CGI_ON = Setting.LoadOrDefault("SUBBBS_CGI_ON" , SettingTxtBool(false) );
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
	NANASHI_CHECK = Setting.LoadOrDefault("NANASHI_CHECK" , SettingTxtBool(false) );
	/*
		BBS_NOTJP_FILTER	.jp�ȊO�̃z�X�g����̏����݋֎~

		��:
			BBS_NOTJP_FILTER=checked
			BBS_NOTJP_FILTER=
		�ڍ�
			.jp(JPNIC���Ǘ�����)IP�ȊO����̏����݋֎~���֎~���܂��B
			checked �ɂ���Ə����݋֎~�֎~�Ƃ��ē��삵�܂��B
			���������Ȃ��ꍇ�͋K���͔�������܂���B

		�ȗ���
			�Ȃ�
	*/
	BBS_NOTJP_FILTER = Setting.LoadOrDefault("BBS_NOTJP_FILTER" , SettingTxtBool(false) );

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
	SAKUBBS = Setting.LoadOrDefault("SAKUBBS" , string("DANGER") );
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
	IS_SAKUBBS = Setting.LoadOrDefault("IS_SAKUBBS" , SettingTxtBool(false) );
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
	DELETE_WAIT = Setting.LoadOrDefault("DELETE_WAIT" , 86400 );
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
	BBS_DISP_IP = Setting.LoadOrDefault("BBS_DISP_IP" , SettingTxtBool(false) );
}

