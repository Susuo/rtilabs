/*------------------------------------------------------------------*/
/********************************************************************/
//         ���e�B's  ���C�u���������ɂ�`
/********************************************************************/
/*------------------------------------------------------------------*/
#pragma warning(disable: 4786)

#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B
#include <windows.h>
#include <windowsx.h>
/********************************************************************/
//�K�i�̈Ⴂ���z������.
/********************************************************************/
#include <.\Comm\rti_comm.h>
#include <.\Comm\rti_GDI.h>
#include <.\Comm\rti_DirectX.h>

/********************************************************************/
//�O���t�B�b�N�X���C�u����
/********************************************************************/
#include <.\Graphics\rti_graphics.h>    //�O���t�B�b�N�X�̃{�X
#include <.\Graphics\rti_bitmaplib.h>   //�r�b�g�}�b�v�L�����N�^�[���삨��єj��
#include <.\Graphics\rti_backsurface.h> //����ʍ쐬
#include <.\Graphics\rti_fontuse.h>       //RTI_Sp�O���t�B�b�N���[�h�Ńt�H���g���g���ꍇ
#include <.\Graphics\rti_anime_ex.h>         //�A�j���[�V����
#include <.\Graphics\rti_avi.h>           //AVI PLAY


/********************************************************************/
//�O���t�B�b�N�G�t�F�N�g ++
/********************************************************************/
#include <.\Graphics\Effect\rti_wave_effect.h>    //�h�炷
#include <.\Graphics\Effect\rti_barst_effect.h>   //����
#include <.\Graphics\Effect\rti_waterfall_effect.h>  //��
#include <.\Graphics\Effect\rti_crash_effect.h>      //�h�炷
#include <.\Graphics\Effect\rti_fall_tile_effect.h>  //������
#include <.\Graphics\Effect\rti_blined_effect.h>     //�u���C���g
#include <.\Graphics\Effect\rti_slow_effect.h>       //�������G��
#include <.\Graphics\Effect\rti_scroll_effect.h>     //�X�N���[��
#include <.\Graphics\Effect\rti_flowt.h>             //�Y��
#include <.\Graphics\Effect\rti_power_effect.h>      //�p���[�`�F���W
#include <.\Graphics\Effect\rti_feade_effect.h>      //�t�F�[�h
#include <.\Graphics\Effect\rti_lay_effect.h>        //����

/********************************************************************/
//�O���t�B�b�N 3D
/********************************************************************/
#include <./Lim3D/rti_Lim3DMaster.h>

/********************************************************************/
//VCL
/********************************************************************/
#include <.\VCL\rti_vcl_apprication.h> //VCL �̊�{
#include <.\VCL\rti_vcl_form.h>        //�t�H�[��
#include <.\VCL\rti_vcl_button.h>      //�{�^��
#include <.\VCL\rti_vcl_grid.h>        //�O���b�h
#include <.\VCL\rti_vcl_progress.h>    //�v���O���X�o�[
#include <.\VCL\rti_vcl_status.h>      //�X�e�[�^�X�o�[
#include <.\VCL\rti_vcl_list.h>        //���X�g�{�b�N�X
#include <.\VCL\rti_vcl_checkbotton.h> //�`�F�b�N�{�b�N�X
#include <.\VCL\rti_vcl_radiobutton.h> //���W�I�{�^��
#include <.\VCL\rti_vcl_groupbox.h>    //���W�I�O���[�v
#include <.\VCL\rti_vcl_radiobox.h>    //���W�I�{�^��
#include <.\VCL\rti_vcl_tabcontrol.h>  //�^�u�R���g���[��
#include <.\VCL\rti_vcl_edit.h>        //�G�f�B�g
#include <.\VCL\rti_vcl_scroll.h>      //�X�N���[���o�[
#include <.\VCL\rti_vcl_static.h>      //�X�^�e�B�b�N�e�L�X�g
#include <.\VCL\rti_vcl_comb.h>		 //�R���{�{�b�N�X
#include <.\VCL\rti_vcl_checkbox.h>    //�`�F�b�N�{�b�N�X
#include <.\VCL\rti_vcl_toolbar.h>     //�c�[���o�[
#include <.\VCL\rti_vcl_menu.h>        //���j���[
#include <.\VCL\rti_vcl_object.h>      //VCL�̊�{�N���X
#include <.\VCL\rti_use_scrollbar.h>    //WS_VSCROLL�Ȃǂ̃X�N���[���o�[

/********************************************************************/
//DEBUG
/********************************************************************/
#include <.\Base\rti_debug.h>       //�f�o�b�O�֌W

/********************************************************************/
//�_�C�A���O
/********************************************************************/
#include <.\Base\rti_error_message.h> //�x����G���[���b�Z�[�W�Ȃ�
#include <.\Base\rti_dialog.h>           //�_�C�A���O�{�b�N�X

/********************************************************************/
//���[�W����
/********************************************************************/
#include <.\Base\rti_rgn.h>           //�����[�W����


/*****************************************************************/
//���d�N���̖h�~    �A�g����(�S�r����Ȃ���(����) )
/*****************************************************************/
#include <.\Base\rti_nonDuplication.h>

/********************************************************************/
//�v�Z
/********************************************************************/
#include <.\Base\rti_glookup.h>          //���b�N�A�b�v�e�[�u�� ���̑�

/********************************************************************/
//�W���B�X�e�B�b�N
/********************************************************************/
#include <.\Base\rti_joystick.h>         //�W���C�X�e�B�b�N 

/********************************************************************/
//���y
/********************************************************************/
#include <.\Music\rti_music.h>              //MIDI�Đ�
#include <.\Music\rti_cd.h>                 //CD�Đ�
#include <.\Music\rti_wave.h>               //WAVE�ǂݍ���
#include <.\Music\rti_wave_player.h>        //WAVE�Đ�

/********************************************************************/
//GDI
/********************************************************************/
#include <.\Graphics\rti_nomal_gdi_bitmap.h> //GDI���g�����G�̓ǂݏo���Ƃ�

/********************************************************************/
//�v�Z
/********************************************************************/
#include <.\Base\rti_glookup.h>

/********************************************************************/
//�L�[����
/********************************************************************/
#include <.\Base\rti_key.h>

/********************************************************************/
//�������e
/********************************************************************/
#include <.\Base\rti_time_action.h>

/********************************************************************/
//�Q�[���֌W
/********************************************************************/
#include <.\Game\rti_Game.h>

/********************************************************************/
//�𑜓x�ύX
/********************************************************************/
#include <.\Base\rti_ChangeDisplay.h>

/********************************************************************/
//���[�U�[��`
/********************************************************************/
//#include <StartLib.h>			  //���[�U��`�̃��C�u����

