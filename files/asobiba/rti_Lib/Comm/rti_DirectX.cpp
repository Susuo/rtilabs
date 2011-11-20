/**************************************************************/
//���e�B���C�u���� DirectDraw �G�~�����[�V�������[�h.
/**************************************************************/

#include <.\Comm\rti_Comm.h>
#include <.\Comm\rti_DirectX.h>
#include <.\Base\rti_error_message.h>
#include <.\Base\rti_debug.h>
#include <.\Graphics\rti_backsurface.h>
#include <.\Game\rti_GameMain.h>

#pragma comment(lib,"ddraw.lib")

#define	SetErrorMessage( ErrCase, String )	\
	case ErrCase: return (String); break;

static char* DDCreateFail( HRESULT hResult )
{
	switch( hResult ){
//		SetErrorMessage( DD_OK , "�v���͐��������������B" );
		SetErrorMessage( DDERR_ALREADYINITIALIZED , "�I�u�W�F�N�g�͂��łɏ���������Ă���B" );
		SetErrorMessage( DDERR_BLTFASTCANTCLIP , "DirectDrawClipper �I�u�W�F�N�g��IdirectDrawSurface3::BltFast���\�b�h�̌Ăяo���ɓn�����]�����T�[�t�F�X�ɃA�^�b�`����Ă���B" );
		SetErrorMessage( DDERR_CANNOTATTACHSURFACE , "�v�����ꂽ�T�[�t�F�X�ɂ̓T�[�t�F�X���A�^�b�`�ł��Ȃ��B" );
		SetErrorMessage( DDERR_CANNOTDETACHSURFACE , "�v�����ꂽ�T�[�t�F�X����T�[�t�F�X���f�^�b�`�ł��Ȃ��B" );
		SetErrorMessage( DDERR_CANTCREATEDC , "Windows�͂���ȏ�̂����Ȃ�f�o�C�X�R���e�L�X�g�i DC �j����邱�Ƃ��ł��Ȃ��B" );
		SetErrorMessage( DDERR_CANTDUPLICATE , "�v���C�}���T�[�t�F�X�A3D�T�[�t�F�X�A�ÖٓI�ɍ����T�[�t�F�X�͕����ł��Ȃ��B" );
		SetErrorMessage( DDERR_CANTLOCKSURFACE , "DCI �T�|�[�g�Ȃ��Ńv���C�}���T�[�t�F�X�̃��b�N�����悤�Ƃ������߁A���̃T�[�t�F�X�ւ̃A�N�Z�X�����ۂ��ꂽ�B" );
		SetErrorMessage( DDERR_CANTPAGELOCK , "�T�[�t�F�X�̃y�[�W ���b�N�����s�����B �y�[�W ���b�N�̓f�B�X�v���C ������ �T�[�t�F�X���邢�̓G�~�����[�g���ꂽ�v���C�}���T�[�t�F�X�̏�ł͓��삵�Ȃ��B" );
		SetErrorMessage( DDERR_CANTPAGEUNLOCK , "�T�[�t�F�X�̃y�[�W ���b�N���������s�����B �y�[�W ���b�N�����̓f�B�X�v���C ������ �T�[�t�F�X���邢�̓G�~�����[�g���ꂽ�v���C�}���T�[�t�F�X�̏�ł͓��삵�Ȃ��B" );
		SetErrorMessage( DDERR_CLIPPERISUSINGHWND , "�E�B���h�E�n���h�������łɃ��j�^���Ă���DirectDrawClipper �I�u�W�F�N�g�ɃN���b�s���O���X�g���Z�b�g���悤�Ƃ����B" );
		SetErrorMessage( DDERR_COLORKEYNOTSET , "�]�����J���[�L�[ ���w�肳��Ă��Ȃ��B" );
		SetErrorMessage( DDERR_CURRENTLYNOTAVAIL , "���݃T�|�[�g����Ă��Ȃ��B" );
		SetErrorMessage( DDERR_DCALREADYCREATED , "�f�o�C�X�R���e�L�X�g�iDC�j�͂��łɂ��̃T�[�t�F�X�ɑ΂��Ԃ���Ă���B�P�T�[�t�F�X�ɂ�1��DC�����擾���邱�Ƃ͂ł��Ȃ��B" );
//		SetErrorMessage( DDERR_DEVICEDOESNTOWNSURFACE, "1 �� DirectDraw �f�o�C�X�ɂ���č쐬���ꂽ�T�[�t�F�X�́A�ʂ� DirectDraw �f�o�C�X�����ڎg�p���邱�Ƃ͂ł��Ȃ��B" );
		SetErrorMessage( DDERR_DIRECTDRAWALREADYCREATED , "DirectDraw�I�u�W�F�N�g�͂��łɍ쐬����Ă���B" );
		SetErrorMessage( DDERR_EXCEPTION , "�v�����ꂽ�������s���Ԃɗ�O�����������B" );
		SetErrorMessage( DDERR_EXCLUSIVEMODEALREADYSET , "���łɔr���I���[�h���Z�b�g����Ă���ɂ�������炸�A�������x�����Z�b�g���悤�Ƃ����B" );
		SetErrorMessage( DDERR_GENERIC , "����`�̃G���[��Ԃł���B" );
		SetErrorMessage( DDERR_HEIGHTALIGN , "�^����ꂽ��`�̍����͕K�v�Ƃ���鐮��̔{���ł͂Ȃ��B" );
		SetErrorMessage( DDERR_HWNDALREADYSET , "DirectDraw�������x�� �E�B���h�E�n���h���͂��łɐݒ肳��Ă���B�E�B���h�E�n���h���̓v���Z�X���T�[�t�F�X���邢�͐��������p���b�g��ێ����Ă���Ԃ̓��Z�b�g�ł��Ȃ��B" );
		SetErrorMessage( DDERR_HWNDSUBCLASSED , "DirectDraw�������x�� �E�B���h�E�n���h���� �T�u�N���X������Ă��邽�߁ADirectDraw�̏�Ԃ̃��X�g�A���j�~���ꂽ�B" );
		SetErrorMessage( DDERR_IMPLICITLYCREATED , "�ÖٓI�ɍ��ꂽ�T�[�t�F�X�����X�g�A���悤�Ƃ����B" );
		SetErrorMessage( DDERR_INCOMPATIBLEPRIMARY , "�v���C�}���T�[�t�F�X �쐬�v���������̃v���C�}���T�[�t�F�X�ƈ�v���Ă��Ȃ��B" );
		SetErrorMessage( DDERR_INVALIDCAPS , "�R�[���o�b�N�֐��ɓn���ꂽ1�ȏ�̔\�̓r�b�g���s���ł���B" );
		SetErrorMessage( DDERR_INVALIDCLIPLIST , "DirectDraw �͗^����ꂽ�N���b�s���O���X�g ���T�|�[�g���Ȃ��B" );
		SetErrorMessage( DDERR_INVALIDDIRECTDRAWGUID , "DirectDrawCreate �֐��ɓn�����O���[�o�����j�[�N���ʎq�i GUID �j�͗L���� DirectDraw �h���C�o���ʎq�ł͂Ȃ��B" );
		SetErrorMessage( DDERR_INVALIDMODE , "DirectDraw �͗v�����ꂽ���[�h���T�|�[�g���Ȃ��B" );
		SetErrorMessage( DDERR_INVALIDOBJECT , "DirectDraw �͖�����DirectDraw �I�u�W�F�N�g�̃|�C���^���󂯎�����B" );
		SetErrorMessage( DDERR_INVALIDPARAMS , "���\�b�h�ɓn���ꂽ1�ȏ�̃p�����[�^���������Ȃ��B" );
		SetErrorMessage( DDERR_INVALIDPIXELFORMAT , "�w�肳�ꂽ�s�N�Z���t�H�[�}�b�g�͖����ł���B" );
		SetErrorMessage( DDERR_INVALIDPOSITION , "�]�����̃I�[�o�[���C�̈ʒu���s���ł���B" );
		SetErrorMessage( DDERR_INVALIDRECT , "�^����ꂽ��`�������ł���B" );
		SetErrorMessage( DDERR_INVALIDSURFACETYPE , "�T�[�t�F�X���Ԉ�����^�C�v�ł��������߁A�v�����ꂽ���������s�ł��Ȃ������B" );
		SetErrorMessage( DDERR_LOCKEDSURFACES , "1�ȏ�̃T�[�t�F�X�����b�N����Ă���A�v�����ꂽ�����Ɏ��s�����B" );
//		SetErrorMessage( DDERR_MOREDATA, "�w�肵���o�b�t�@�T�C�Y���ۗL�ł���ȏ�̃f�[�^�����݂���B" );
		SetErrorMessage( DDERR_NO3D , "3D�n�[�h�E�F�A���邢�̓G�~�����[�V���������݂��Ȃ��B" );
		SetErrorMessage( DDERR_NOALPHAHW , "�A���t�@ �A�N�Z�����[�V���� �n�[�h�E�F�A�����݂��Ȃ������邢�͗��p�ł��Ȃ��������߁A�v�����ꂽ�����Ɏ��s�����B" );
		SetErrorMessage( DDERR_NOBLTHW , "�u���b�N�]���n�[�h�E�F�A�����݂��Ȃ��B" );
		SetErrorMessage( DDERR_NOCLIPLIST , "�N���b�s���O���X�g�����p�ł��Ȃ��B" );
		SetErrorMessage( DDERR_NOCLIPPERATTACHED , "���̃T�[�t�F�X �I�u�W�F�N�g�ɂ�DirectDrawClipper �I�u�W�F�N�g���A�^�b�`�ł��Ȃ��B" );
		SetErrorMessage( DDERR_NOCOLORCONVHW , "�F�ϊ��n�[�h�E�F�A�����݂��Ȃ��A���邢�͗��p�ł��Ȃ����߁A���������s�ł��Ȃ��B" );
		SetErrorMessage( DDERR_NOCOLORKEY , "�T�[�t�F�X�͌��݃J���[�L�[�������Ă��Ȃ��B" );
		SetErrorMessage( DDERR_NOCOLORKEYHW , "�]����J���[�L�[�ɑ΂���n�[�h�E�F�A�T�|�[�g���Ȃ����߁A���������s�ł��Ȃ��B" );
		SetErrorMessage( DDERR_NOCOOPERATIVELEVELSET , "IdirectDraw2::SetCooperativeLevel���\�b�h���Ăяo�����ƂȂ��쐬�֐����Ăяo���ꂽ�B" );
		SetErrorMessage( DDERR_NODC , "DC�́A����܂łɂ��̃T�[�t�F�X�p�ɍ���Ă��Ȃ������B" );
		SetErrorMessage( DDERR_NODDROPSHW , "DirectDraw ���X�^�����i ROP �j�n�[�h�E�F�A�����p�ł��Ȃ��B" );
		SetErrorMessage( DDERR_NODIRECTDRAWHW , "�n�[�h�E�F�A��pDirectDraw �I�u�W�F�N�g���쐬�ł��Ȃ��B�h���C�o�̓n�[�h�E�F�A���T�|�[�g���Ȃ��B" );
		SetErrorMessage( DDERR_NODIRECTDRAWSUPPORT , "���݂̃f�B�X�v���C �h���C�o��DirectDraw���T�|�[�g���Ă��Ȃ��B" );
		SetErrorMessage( DDERR_NOEMULATION , "�\�t�g�E�F�A �G�~�����[�V���������p�ł��Ȃ��B" );
		SetErrorMessage( DDERR_NOEXCLUSIVEMODE , "�A�v���P�[�V�����ɑ΂��r���I���[�h���v�����ꂽ���A�A�v���P�[�V�����͔r���I���[�h�������Ă��Ȃ��B" );
		SetErrorMessage( DDERR_NOFLIPHW , "���T�[�t�F�X�̓t���b�s���O�ł��Ȃ��B" );
		SetErrorMessage( DDERR_NOGDI , "GDI �����݂��Ȃ��B" );
		SetErrorMessage( DDERR_NOHWND , "�N���b�s���O�ʒm���E�B���h�E�n���h����K�v�Ƃ���A���邢�̓E�B���h�E�n���h�����������x�� �E�B���h�E�n���h���Ƃ��Ă��炩���߃Z�b�g����Ă��Ȃ��B" );
		SetErrorMessage( DDERR_NOMIPMAPHW , "�~�b�v�}�b�v �e�N�X�`�� �}�b�s���O�̃n�[�h�E�F�A�����݂��Ă��Ȃ����A���邢�͗��p�ł��Ȃ��̂ŏ��������s�ł��Ȃ��B" );
		SetErrorMessage( DDERR_NOMIRRORHW , "�~���[�����O �n�[�h�E�F�A�����݂��Ȃ������邢�͗��p�ł��Ȃ����߁A���������s�ł��Ȃ��B" );
//		SetErrorMessage( DDERR_NONONLOCALVIDMEM, "�񃍁[�J���r�f�I���������T�|�[�g���Ȃ��f�o�C�X����A�񃍁[�J���r�f�I�����������蓖�Ă悤�Ƃ����B" );
//		SetErrorMessage( DDERR_NOOPTIMIZEHW, "�f�o�C�X�́A�œK�����ꂽ�T�[�t�F�C�X���T�|�[�g���Ȃ��B" );
		SetErrorMessage( DDERR_NOOVERLAYDEST , "�]����m���̂��߂�IdirectDrawSurface3::UpdateOverlay���\�b�h�Ăяo�������Ă��Ȃ��I�[�o�[���C�ɑ΂��AIdirectDrawSurface3::GetOverlayPosition���\�b�h���Ăяo���ꂽ�B" );
		SetErrorMessage( DDERR_NOOVERLAYHW , "�I�[�o�[���C �n�[�h�E�F�A�����݂��Ă��Ȃ������邢�͗��p�ł��Ȃ����߁A���������s�ł��Ȃ��B" );
		SetErrorMessage( DDERR_NOPALETTEATTACHED , "�p���b�g �I�u�W�F�N�g�����̃T�[�t�F�X�ɃA�^�b�`����Ă��Ȃ��B" );
		SetErrorMessage( DDERR_NOPALETTEHW , "16�F���邢��256�F�̃p���b�g�ɑ΂���n�[�h�E�F�A�T�|�[�g���Ȃ��B" );
		SetErrorMessage( DDERR_NORASTEROPHW , "�K�؂ȃ��X�^�����n�[�h�E�F�A�����݂��Ȃ������邢�͗��p�ł��Ȃ����߁A���������s�ł��Ȃ��B" );
		SetErrorMessage( DDERR_NOROTATIONHW , "��]�n�[�h�E�F�A�����݂��Ă��Ȃ����A���邢�͗��p�ł��Ȃ����ߏ��������s�ł��Ȃ��B" );
		SetErrorMessage( DDERR_NOSTRETCHHW , "�g��n�[�h�E�F�A�����݂��Ă��Ȃ������邢�͗��p�ł��Ȃ����߁A���������s�ł��Ȃ��B" );
		SetErrorMessage( DDERR_NOT4BITCOLOR , "DirectDrawSurface �I�u�W�F�N�g�́A�v�����ꂽ�S�r�b�g�F�p���b�g���g���Ă��Ȃ��B" );
		SetErrorMessage( DDERR_NOT4BITCOLORINDEX , "DirectDrawSurface �I�u�W�F�N�g�́A�v�����ꂽ�S�r�b�g�̃J���[�C���f�b�N�X�p���b�g���g���Ă��Ȃ��B" );
		SetErrorMessage( DDERR_NOT8BITCOLOR , "DirectDrawSurface �I�u�W�F�N�g�́A�v�����ꂽ�W�r�b�g�F�p���b�g���g���Ă��Ȃ��B" );
		SetErrorMessage( DDERR_NOTAOVERLAYSURFACE , "��I�[�o�[���C�T�[�t�F�X�ɑ΂��A�I�[�o�[���C �R���|�[�l���g���Ăяo���ꂽ�B" );
		SetErrorMessage( DDERR_NOTEXTUREHW , "�e�N�X�`�� �}�b�s���O �n�[�h�E�F�A�����݂��Ă��Ȃ������邢�͗��p�ł��Ȃ����߁A���������s�ł��Ȃ��B" );
		SetErrorMessage( DDERR_NOTFLIPPABLE , "�t���b�s���O�ł��Ȃ��T�[�t�F�X���t���b�s���O���悤�Ƃ����B" );
		SetErrorMessage( DDERR_NOTFOUND , "�v�����ꂽ���ڂ͌�����Ȃ������B" );
		SetErrorMessage( DDERR_NOTINITIALIZED , "�I�u�W�F�N�g�������������O�ɁACoCreateInstance�ō쐬���ꂽ DirectDraw �I�u�W�F�N�g�̃C���^�[�t�F�C�X ���\�b�h���Ăяo�����Ƃ����B" );
//		SetErrorMessage( DDERR_NOTLOADED, "�T�[�t�F�C�X�͍œK������Ă��邪�A�܂��������͊��蓖�Ă��ĂȂ��B" );
		SetErrorMessage( DDERR_NOTLOCKED , "���b�N����Ă��Ȃ��T�[�t�F�X�̃��b�N�������s��ꂽ�B" );
		SetErrorMessage( DDERR_NOTPAGELOCKED , "�������̃y�[�W ���b�N���Ȃ��T�[�t�F�X�̃y�[�W ���b�N�������s��ꂽ�B" );
		SetErrorMessage( DDERR_NOTPALETTIZED , "�g�p���Ă���T�[�t�F�X�̓p���b�g �x�[�X�̃T�[�t�F�X�ł͂Ȃ��B" );
		SetErrorMessage( DDERR_NOVSYNCHW , "�����A�����������ɑ΂���n�[�h�E�F�A�T�|�[�g���Ȃ����߁A���������s�ł��Ȃ��B" );
		SetErrorMessage( DDERR_NOZBUFFERHW , "Z�o�b�t�@�ɑ΂���n�[�h�E�F�A�T�|�[�g���Ȃ����߁A�f�B�X�v���C �������ł� Z �o�b�t�@�쐬���邢�� Z �o�b�t�@���g�p�����u���b�N�]�������s�ł��Ȃ��B" );
		SetErrorMessage( DDERR_NOZOVERLAYHW , "�I�[�o�[���C�T�[�t�F�X�́A�n�[�h�E�F�A���I�[�o�[���C Z�I�[�_�[���T�|�[�g���Ȃ����߁AZ�I�[�_�[���������ꂽ�B" );
		SetErrorMessage( DDERR_OUTOFCAPS , "�v�����ꂽ�����ɕK�v�ȃn�[�h�E�F�A�����łɊ��蓖�Ă��Ă���B" );
		SetErrorMessage( DDERR_OUTOFMEMORY , "DirectDraw���������s���̂ɏ\���ȃ��������Ȃ��B" );
		SetErrorMessage( DDERR_OUTOFVIDEOMEMORY , "DirectDraw���������s���̂ɏ\���ȃf�B�X�v���C ���������Ȃ��B" );
		SetErrorMessage( DDERR_OVERLAYCANTCLIP , "�n�[�h�E�F�A�̓N���b�s���O �I�[�o�[���C���T�|�[�g���Ȃ��B" );
		SetErrorMessage( DDERR_OVERLAYCOLORKEYONLYONEACTIVE , "�I�[�o�[���C���1�ȏ�̗L���ȃJ���[�L�[�����Ƃ��Ƃ����B" );
		SetErrorMessage( DDERR_OVERLAYNOTVISIBLE , "IdirectDrawSurface3::GetOverlayPosition���\�b�h���s���I�[�o�[���C��ŌĂяo���ꂽ�B" );
		SetErrorMessage( DDERR_PALETTEBUSY , "�p���b�g������1�̃X���b�h�ɂ���ă��b�N����Ă��邽�߁A�A�N�Z�X�����ۂ��ꂽ�B" );
		SetErrorMessage( DDERR_PRIMARYSURFACEALREADYEXISTS , "���̏����͂��łɃv���C�}���T�[�t�F�X���쐬���Ă���B" );
		SetErrorMessage( DDERR_REGIONTOOSMALL , "IdirectDrawClipper::GetClipList���\�b�h�ɓn���ꂽ�̈悪����������B" );
		SetErrorMessage( DDERR_SURFACEALREADYATTACHED , "���łɃA�^�b�`���Ă������1�̃T�[�t�F�X�ɃT�[�t�F�X���A�^�b�`���悤�Ƃ����B" );
		SetErrorMessage( DDERR_SURFACEALREADYDEPENDENT , "���łɑ��̃T�[�t�F�X�Ɉˑ����Ă���T�[�t�F�X���ˑ������悤�Ƃ����B" );
		SetErrorMessage( DDERR_SURFACEBUSY , "�T�[�t�F�X������1�̃X���b�h�ɂ���ă��b�N����Ă��邽�߁A�T�[�t�F�X�ւ̃A�N�Z�X�����ۂ��ꂽ�B" );
		SetErrorMessage( DDERR_SURFACEISOBSCURED , "�T�[�t�F�X�ւ̃A�N�Z�X���A�T�[�t�F�X���B������Ă��邽�ߋ��ۂ��ꂽ�B" );
		SetErrorMessage( DDERR_SURFACELOST , "�T�[�t�F�X �������̕s���̂��߁A�T�[�t�F�X�ւ̃A�N�Z�X�����ۂ��ꂽ�B ���̃T�[�t�F�X��\��DirectDrawSurface �I�u�W�F�N�g�ɂ�IdirectDrawSurface3::Restore���\�b�h���Ăяo����Ȃ���΂Ȃ�Ȃ��B" );
		SetErrorMessage( DDERR_SURFACENOTATTACHED , "�v�����ꂽ�T�[�t�F�X�̓A�^�b�`�ł��Ȃ��B" );
		SetErrorMessage( DDERR_TOOBIGHEIGHT , "DirectDraw �ɂ���ėv�����ꂽ�������傫������B" );
		SetErrorMessage( DDERR_TOOBIGSIZE , "DirectDraw �ɂ���ėv�����ꂽ�傫�����傫������B�������A�ʂ̍����ƕ��͖�肪�Ȃ��B" );
		SetErrorMessage( DDERR_TOOBIGWIDTH , "DirectDraw �ɂ���ėv�����ꂽ�����傫������B" );
		SetErrorMessage( DDERR_UNSUPPORTED , "�T�|�[�g����Ă��Ȃ������ł������B" );
		SetErrorMessage( DDERR_UNSUPPORTEDFORMAT , "�v�����ꂽ FourCC �t�H�[�}�b�g��DirectDraw �ɂ���ăT�|�[�g����Ă��Ȃ��B" );
		SetErrorMessage( DDERR_UNSUPPORTEDMASK , "�v�����ꂽ�s�N�Z���t�H�[�}�b�g�ł� �r�b�g�}�X�N ��DirectDraw �ɂ���ăT�|�[�g����Ă��Ȃ��B" );
		SetErrorMessage( DDERR_UNSUPPORTEDMODE , "�f�B�X�v���C�͌��݃T�|�[�g����Ă��Ȃ����[�h�ł���B" );
		SetErrorMessage( DDERR_VERTICALBLANKINPROGRESS , "�����A�����Ԃł���B" );
//		SetErrorMessage( DDERR_VIDEONOTACTIVE, "�r�f�I�|�[�g�́A��A�N�e�B�u�ł���B" );
		SetErrorMessage( DDERR_WASSTILLDRAWING , "���̃T�[�t�F�X����A�܂��̓T�[�t�F�X�ւ̑O��̃u���b�N�]�����삪�������Ă��Ȃ��B" );
		SetErrorMessage( DDERR_WRONGMODE , "�قȂ������[�h�ō��ꂽ���߁A���̃T�[�t�F�X�̓��X�g�A���邱�Ƃ��ł��Ȃ��B" );
		SetErrorMessage( DDERR_XALIGN , "�^����ꂽ��`��K�v�Ƃ����͈͂ɐ������ɕ��ׂ邱�Ƃ��ł��Ȃ������B" );
 	}
	return 0;
}

//������.
static bool DDXInit(HWND hwnd,int w,int h,int bps);
//���̃f�B�X�v���C���[�h���g���邩�ǂ����`�F�b�N.
static bool FindOutDisplayModes();
//�f�B�X�v���C���[�h����.
static HRESULT WINAPI EnumModesCallBack( DDSURFACEDESC *lpDDSurfaceDesc , void * lpContext);
//�I������.
static bool DDXEnd();
//�t�@�C���ǂݍ���
static bool DDXGraphicsMake(BMD puf,const char* filename);
static bool DDXGraphicsMakeSub(BMD bmd,HDC memDC,HBITMAP hBit);
//�̈�m��
static bool DDXGraphicsAlloc(BMD bmd);
//�̈���
static bool DDXGraphicsFree(BMD bmd);
//���b�N
static bool DDXGraphicsLock(BMD bmd);
//�A�����b�N
static bool DDXGraphicsUnLock(BMD bmd);
//������ bmd �� DC ���쐬���� (^^;;;
static HDC DDXGetDC(BMD bmd);
//DC ����������. (DC �ɑ΂��鑀�삪�t�B�[�h�o�b�N�����.)
static bool DDXDeleteDC(BMD bmd,HDC hdc);
//DC->BMD �̒��g���X�V����.
static bool DDXUpdataDC(BMD bmd,HDC hdc);
//�ĊG��
static void DDXRef(HWND hwnd);
//���݂̊G�惂�[�h�̎擾
static UseDrawMode  DDXGetMode();
//���t���b�V�����i.
static void DDXRef(HWND hwnd);
//�t���X�N�[����
static bool DDXDoFullScreen(bool CoerceChange);
//�E�B���h���[�h
static bool DDXDoWindow(bool CoerceChange);
//�v���C�}���[�T�[�t�F�[�X�̍쐬.
static bool ReMakePrimarySurface();
//�G��|�C���g�̐ݒ�(�E�B���h���[�h�݂̂ɗL���ł�.)
static void DDXSetDrawPoint(int x,int y);


//DDX�����p���鏉�������[�`�����҂��.
static struct _CommDraw CommDrawDDX = 
	{DDXInit,DDXEnd,DDXGraphicsMake,DDXGraphicsAlloc,DDXGraphicsFree,
		DDXGraphicsLock,DDXGraphicsUnLock,DDXGetDC,DDXDeleteDC,DDXUpdataDC,DDXGetMode,DDXRef,
		DDXDoFullScreen,DDXDoWindow,DDXSetDrawPoint};

static LPDIRECTDRAW			lpDD;		   //DD�I�u�W�F�N�g
static LPDIRECTDRAWSURFACE	lpDDSPrimary;  //�v���C�}���T�[�t�F�[�X
static LPDIRECTDRAWCLIPPER	lpClipper;	   //�E�B���h���[�h�Ŏg�p����N���b�p�[	
static int DrawPointX,DrawPointY;		   //�G��ʒu(�E�B���h���[�h�݂̂ŗL��).	

//DirectX�I�u�W�F�N�g���
#define RELEASEDD(a)  if( (a) ) { (a)->Release(); a = NULL;}      

//DDX�̏����̗p������Ă���.
void DDXAdoption()
{
    lpDD         = NULL;	//�O�̂��ߏ�����
    lpDDSPrimary = NULL;	//����́A��΂ɏ��������Ă����Ă�������. (�����ōŏ��̍쐬����蒼�����̃`�F�b�N�Ɏg������.)
	lpClipper    = NULL;	//�������Ώ��������邱��.
	CommDraw = &CommDrawDDX;
}

//�I������.
bool DDXInit(HWND hwnd,int w,int h,int bps)
{
	HRESULT			ret;
	NowScreenMode.fFullScreen = true; //DDX ���[�h�́A�t���X�N���[���ŋN��.
	NowScreenMode.iWidth      = w;
	NowScreenMode.iHeight     = h;
	NowScreenMode.iBpp        = bps;
	try
	{
		//DD�I�u�W�F�N�g�쐬
		ret = DirectDrawCreate( NULL, &lpDD, NULL);
		if( ret != DD_OK ) 
			throw (FriendlyError("DDXInit","DirectDraw�I�u�W�F�N�g�쐬",DDCreateFail(ret),"DDX�N���͖����ł�"));
		if ( !DDXDoFullScreen(true) )
			throw (FriendlyError("DDXInit","DirectDraw�t���X�N���[���쐬","���股�́ADirectDraw�N���͕s�\�ł�.","DDX�N���͖����ł�"));
	}
	catch(...)
	{
		return false;
	}
	return true;
}

//�I������.
bool DDXEnd()
{
	//�N���b�p�[����̔j��
	RELEASEDD(lpClipper);
	//�v���C�}���[������o�E
	RELEASEDD( lpDDSPrimary );			

	//�t���X�N���[���̂Ƃ��̓f�B�X�v���C���[�h�����ɖ߂�.	
	if (NowScreenMode.fFullScreen)	lpDD->RestoreDisplayMode();
	//DirectDraw������߂�������.
	RELEASEDD( lpDD );					
	return true;
}

//�t���X�N�[����
bool DDXDoFullScreen(bool CoerceChange = false)
{
	//���s���Ȃ��ꍇ�́A���łɂ��̏�Ԃ��`�F�b�N
	if (!CoerceChange)
		if (NowScreenMode.fFullScreen)	return false;	//���łɃt���X�N�[�����ł�.

	HRESULT			ret;
	try
	{
	    //�𑜓x�ύX�ł��܂���?
		if ( ! FindOutDisplayModes() )
			throw		FriendlyError("DDXDoFullScreen","�t���X�N�[�����ݒ�",
						"���̃r�f�I�J�[�hor�f�B�X�v���C�́A���̉𑜓x���T�|�[�g���Ă��܂���.",
						"�r�f�I�J�[�h�Ȃǂ��܂Ƃ��Ȃ��̂ƌ������Ă�������");
		//�E�B���h�X�^�C���A�傫���̕ύX.	
		SetWindowLong( GameForm->GethWnd() , GWL_STYLE	, FULLWINDOW_STYLE);
		GameForm->ReClasSize(NowScreenMode.iWidth,NowScreenMode.iHeight);
		//�������x���ݒ�
		ret = lpDD->SetCooperativeLevel(GameForm->GethWnd(),DDSCL_EXCLUSIVE |
										DDSCL_FULLSCREEN | 
										DDSCL_ALLOWREBOOT);
		if( ret != DD_OK )
			throw (FriendlyError("DDXInit","�������x���ݒ�",DDCreateFail(ret),"DDX�N���͖����ł�"));
		//�f�B�X�v���C���[�h�ݒ�
		ret = lpDD->SetDisplayMode(NowScreenMode.iWidth , NowScreenMode.iHeight, NowScreenMode.iBpp);
		if( ret != DD_OK )
			throw (FriendlyError("DDXInit","�f�B�X�v���C���[�h�ݒ�",DDCreateFail(ret),"������Ƀf�B�X�v���C�J�[�h�͑Ή����Ă܂�?"));
		//�N���b�p�[����̔j��
		RELEASEDD(lpClipper);
		//�v���C�}���[�T�[�t�F�[�X�̍č쐬.
		if ( !ReMakePrimarySurface() )
			throw 0;
		NowScreenMode.fFullScreen = true;
	}
	catch(...)
	{
		return false;
	}
	return true;
}

//�E�B���h���[�h
bool DDXDoWindow(bool CoerceChange = false)
{
	//���s���Ȃ��ꍇ�́A���łɂ��̏�Ԃ��`�F�b�N
	if (!CoerceChange)
		if (!NowScreenMode.fFullScreen)	return false;	//���łɃE�B���h���[�h�ł�.
	HRESULT			ret;
	DDSURFACEDESC   ddsd;
	try
	{
		//�f�B�X�v���C���[�h�����ɖ߂�.	
		ret = lpDD->RestoreDisplayMode();
		if( ret != DD_OK )
		{
			ShowWindow(GameForm->GethWnd(),SW_HIDE);
			throw	FriendlyError("DDXDoWindow","�E�B���h���[�h�ɂ��邽�߂ɉ�ʂ����ɖ߂��ݒ�",
							DDCreateFail(ret),"?");
		}
		//���̃f�B�X�v���C���[�h�擾
		ddsd.dwSize = sizeof(ddsd);	//��΂ɂ����Y����!!! ���ʂ�!
		ret = lpDD->GetDisplayMode(&ddsd);
		if( ret != DD_OK )
		{
			ShowWindow(GameForm->GethWnd(),SW_HIDE);
			throw	FriendlyError("DDXDoWindow","���݂̉�ʃ��[�h�擾�Ɏ��s",DDCreateFail(ret),"?");
		}
		//���̃��[�h�� Window���[�h���ł��܂���?
		if (ddsd.ddpfPixelFormat.dwRGBBitCount != NowScreenMode.iBpp)
		{
			ShowWindow(GameForm->GethWnd(),SW_HIDE);
			throw	FriendlyError("DDXDoWindow","���̉𑜓x�� Window���[�h����������邱��",
					"���̉𑜓x�́A���̃A�v���P�[�V���������s������̂��߂̊��ɓK���Ă��܂���.","�E�B���h���[�h�͂�����߂Ă�������.");
		}
		//�E�B���h�֌W�̍X�V
		SetWindowLong( GameForm->GethWnd() , GWL_STYLE	, WINDOW_STYLE);
		GameForm->ReClasSize(NowScreenMode.iWidth,NowScreenMode.iHeight);
		//�������x���ݒ�
		ret = lpDD->SetCooperativeLevel(GameForm->GethWnd(),DDSCL_NORMAL);
		if( ret != DD_OK )
		{
			ShowWindow(GameForm->GethWnd(),SW_HIDE);
			throw (FriendlyError("DDXDoWindow","�������x���ݒ�",DDCreateFail(ret),"Window���[�h�N���͖���"));
		}
		//�v���C�}���[�T�[�t�F�[�X�̍č쐬.
		if ( !ReMakePrimarySurface() )
					throw 0;
		//�N���b�p�[����̍X�V.
		RELEASEDD(lpClipper);	//�O�̂��ߌ��݂̃N���b�p�[�N���n��.
		ret = lpDD->CreateClipper(0,&lpClipper,NULL);
		if (ret != DD_OK )
		{
			ShowWindow(GameForm->GethWnd(),SW_HIDE);
			throw (FriendlyError("DDXDoWindow","�N���b�p�[�쐬",DDCreateFail(ret),"Window���[�h�N���͖���"));
		}
		//�ڕW�E�B���h�ɃN���b�p�[�N�����t����.
		ret = lpClipper->SetHWnd(0,GameForm->GethWnd());
		if (ret != DD_OK )
		{
			ShowWindow(GameForm->GethWnd(),SW_HIDE);
			throw (FriendlyError("DDXDoWindow","�N���b�p�[�����C���E�B���h�ɐݒ肷�邱��",DDCreateFail(ret),"Window���[�h�N���͖���"));
		}
		//�v���C�}���[�ɃN���b�p�[��߈˂�����.
		ret = lpDDSPrimary->SetClipper( lpClipper );
		if (ret != DD_OK )
		{
			ShowWindow(GameForm->GethWnd(),SW_HIDE);
			throw (FriendlyError("DDXDoWindow","�N���b�p�[���v���C�}�[�ɐݒ肷�邱��",DDCreateFail(ret),"Window���[�h�N���͖���"));
		}
		NowScreenMode.fFullScreen = false;
        //GameForm->Align(ALIGN_MOVE_CENTER);
		GameForm->Move(100,100);
	}
	catch(...)
	{
		DDXDoFullScreen(true);	//�����I�Ƀt���X�N�[�����ɖ߂�
		return false;
	}

	return true;
}

//�f�B�X�v���C���[�h�񋓂̃R�[���o�b�N.
HRESULT WINAPI EnumModesCallBack( DDSURFACEDESC *lpDDSurfaceDesc , void * lpContext)
{
	//�T���Ă����f�B�X�v���C���[�h����?
	if (lpDDSurfaceDesc->dwWidth == NowScreenMode.iWidth &&
		lpDDSurfaceDesc->dwHeight == NowScreenMode.iHeight &&
		lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount == NowScreenMode.iBpp)
	{
		//�t���X�N�[������ OK.
		*((BOOL*)(lpContext)) = TRUE;
		return DDENUMRET_CANCEL;
	}
	return DDENUMRET_OK;
}


//���̃f�B�X�v���C���[�h���g���邩�ǂ����`�F�b�N.
bool FindOutDisplayModes()
{
	DDSURFACEDESC DDSurfaceDesc = {0};
	BOOL retCheck = FALSE;
	//DDSURFACEDESC �ɒT���Ă��郂�[�h��ݒ�.
	DDSurfaceDesc.dwSize  = sizeof(DDSURFACEDESC);
	DDSurfaceDesc.dwFlags = DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_WIDTH;
	DDSurfaceDesc.dwWidth = NowScreenMode.iWidth;
	DDSurfaceDesc.dwHeight= NowScreenMode.iHeight;
	DDSurfaceDesc.ddpfPixelFormat.dwRGBBitCount = NowScreenMode.iBpp;

	lpDD->EnumDisplayModes(DDEDM_REFRESHRATES , &DDSurfaceDesc , &retCheck , EnumModesCallBack)	;
	return (retCheck==TRUE);
}


//�v���C�}���[�T�[�t�F�[�X�̍쐬.
bool ReMakePrimarySurface()
{
	HRESULT			ret;
	DDSURFACEDESC	ddsd;
	try
	{
		//���܂Ŏg���Ă����A�v���C�}���[������o�E
		RELEASEDD( lpDDSPrimary );			
		//�v���C�}���T�[�t�F�[�X�쐬
		memset( &ddsd, 0, sizeof(ddsd) );
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS;						//
		ddsd.dwBackBufferCount = 0;						//�o�b�N�o�b�t�@��
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;   //�v���C�}���T�[�t�F�[�X�w��
		ret = lpDD->CreateSurface(&ddsd, &lpDDSPrimary ,NULL);	//�T�[�t�F�[�X�쐬
		if( ret != DD_OK ) 
			throw FriendlyError("DDXInit","�v���C�}���T�[�t�F�[�X�쐬",DDCreateFail(ret),"�Ȃ��Ȃ񂾂��[");
	}
	catch(...)
	{
		return false;
	}
	return true;
}

//�r�b�g�}�b�v�̓ǂݍ���
bool DDXGraphicsMake(BMD puf,const char* filename)
{
    BITMAP bmp_info;
    HDC memDC = NULL;            //�������f�o�C�X�R���e�L�X�g
    HDC hdc   = NULL;
    HBITMAP hBit = NULL;         //�r�b�g�}�b�v�����
    try
    {
	    hdc = GetDC(NULL);                     //�f�o�C�X�R���e�L�X�g�擾
        if (!hdc)
			throw (FriendlyError("GraphicsMake","�f�o�C�X�R���e�L�X�g�擾","?","?"));

    	memDC = CreateCompatibleDC(hdc); //������DC�擾
        if (!memDC)
			throw (FriendlyError("GraphicsMake","MemDC�擾","?","?"));

        if ( !ReleaseDC(NULL,hdc) )		//DC �J��
			 throw (FriendlyError("GraphicsMake","�f�o�C�X�R���e�L�X�g�j��","",""));

	    hBit = (HBITMAP)LoadImage(NULL,filename,
            IMAGE_BITMAP,0,0,LR_LOADFROMFILE);         //�r�b�g�}�b�v�n���h������
	    if (!hBit) throw (FileNotOpen("GraphicsMake",filename) );

        GetObject(hBit, sizeof(BITMAP), &bmp_info);
        puf->w = bmp_info.bmWidth;
        puf->h = bmp_info.bmHeight;
    
	    SelectObject(memDC,hBit);   //�r�b�g�}�b�v�I��

		//�̈�m��.
		if ( !DDXGraphicsAlloc(puf))
			 throw (FriendlyError("GraphicsMake","�T�[�t�F�[�X�쐬","����������Ă܂���?","�������𑝂₵�Ă�������"));
		//�̈�m��.   
	    if ( !DDXGraphicsMakeSub(puf,memDC,hBit) )
			 throw (FriendlyError("GraphicsMake","�摜���T�[�t�F�[�X�ɓ]�������","?","?"));
        if ( DeleteDC(memDC) ) memDC = NULL;       //������DC�J��
        else  throw (FriendlyError("GraphicsMake","MemDC�폜","?","?"));

        if ( DeleteObject(hBit) ) hBit = NULL;    //�r�b�g�}�b�v�n���h���폜
        else  throw (FriendlyError("GraphicsMake","HBITMAP�폜","?","?"));
    }
    catch(...)
    {
		if (hdc)	ReleaseDC(NULL,hdc);		   //DC�j��
	    if (memDC)  DeleteDC(memDC);       //������DC�J��
	    if (hBit)   DeleteObject(hBit);    //�r�b�g�}�b�v�n���h���폜
	   	return false;
    }
	return true;
}

//�G�̓]��
bool DDXGraphicsMakeSub(BMD bmd,HDC memDC,HBITMAP hBit)
{
	HDC hdc ;
	HRESULT			ret;
	IDirectDrawSurface* pdds = (IDirectDrawSurface*) bmd->Tag;	//DirectDraw �I�u�W�F�N�g�̎擾.
	try
	{
		//�Ƃ肠�����A DC ���擾����.
		ret = pdds->GetDC(&hdc);
		if(ret != DD_OK) throw (FriendlyError("DDXGraphicsMakeSub","DC �擾",DDCreateFail(ret),""));
		//�摜�]��(�蔲���`)
		BitBlt(hdc ,0,0,bmd->w,bmd->h, memDC,0,0,SRCCOPY);

		ret = pdds->ReleaseDC(hdc);
		if(ret != DD_OK) throw (FriendlyError("DDXGraphicsMakeSub","DC �ԋp",DDCreateFail(ret),""));

		unsigned char Pow2W;
		//2�ׂ̂�������.
		CheckPow2(&Pow2W,bmd->w);
		bmd->Pow2W = Pow2W;
	}
	catch(...)
	{
		return false;
	}
	return true;
}

//�̈�m��
bool DDXGraphicsAlloc(BMD bmd)
{
	HRESULT			ret;
	DDSURFACEDESC ddsd;			
	IDirectDrawSurface* pdds;	//�T�[�t�F�[�X������
	DDCOLORKEY ddck;			//�J���[�L�[����
	try
	{
		//�T�[�t�F�[�X�̍쐬.
		ZeroMemory(&ddsd,sizeof(ddsd));
		ddsd.dwSize=sizeof(ddsd);
		ddsd.dwFlags=DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
		ddsd.dwWidth=bmd->w;
		ddsd.dwHeight=bmd->h;
		ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY ;
		ret = lpDD->CreateSurface(&ddsd,&pdds,NULL);
		if(ret != DD_OK)
			throw (FriendlyError("DDXGraphicsAlloc","�T�[�t�F�[�X�쐬",DDCreateFail(ret),""));
		//�J���[�L�[�̐ݒ�
		ddck.dwColorSpaceLowValue = 0;		ddck.dwColorSpaceHighValue = 0;
		pdds->SetColorKey(DDCKEY_SRCBLT,&ddck);
	}
	catch(...)
	{
		return false;
	}
	//�T�[�t�F�[�X�d���Ȃ��̂ŁA�� Tag �ɋL�^���Ă����܂�.
	bmd->Tag = pdds;
	return true;						//�听��!! (^-^
}

//�̈���
bool DDXGraphicsFree(BMD bmd)
{
	return true;
}

//���b�N
bool DDXGraphicsLock(BMD bmd)
{ 
	DDSURFACEDESC ddsd;
	register IDirectDrawSurface* pdds = ((IDirectDrawSurface*)(bmd->Tag));

	ddsd.dwSize = sizeof(ddsd);	//��΂ɂ����Y����!!! ���ʂ�!
	if (pdds->Lock(NULL,&ddsd,DDLOCK_WAIT,0) != DD_OK)
	{
		return false;
	}
	bmd->bm = (BYTE*)ddsd.lpSurface;
	return true;
}

//�A�����b�N
bool DDXGraphicsUnLock(BMD bmd)
{
	register IDirectDrawSurface* pdds = ((IDirectDrawSurface*)(bmd->Tag));
	if (pdds->Unlock(bmd->bm) != DD_OK) return false;
	return true;
}

//������ bmd �� DC ���쐬���� (^^;;;
HDC DDXGetDC(BMD bmd)
{
	HDC hdc;
	register IDirectDrawSurface* pdds = ((IDirectDrawSurface*)(bmd->Tag));
	if (pdds->GetDC(&hdc) != DD_OK) return NULL;

	return hdc	;						//�������f�o�C�X��Ԃ�.
}

//DC ����������. (DC �ɑ΂��鑀�삪�t�B�[�h�o�b�N�����.)
bool DDXDeleteDC(BMD bmd,HDC hdc)
{
	register IDirectDrawSurface* pdds = ((IDirectDrawSurface*)(bmd->Tag));
	if (pdds->ReleaseDC(hdc) != DD_OK) return false;

	return true;
}
 

//DC->BMD �̒��g���X�V����.
bool DDXUpdataDC(BMD bmd,HDC hdc)
{
	return true;
}


//���݂̊G�惂�[�h�̎擾
UseDrawMode DDXGetMode()
{
	return USE_DIRECTDRAW_MODE;
}

//�ĊG��
void DDXRef(HWND hwnd)
{
	//�v���C�}���[����� ���X�g�����j�ȉ��� ���Ă�����A
	//�]���r�̂悤�ɑ啜��!!
	if(lpDDSPrimary->IsLost() == DDERR_SURFACELOST)
		lpDDSPrimary->Restore();		//�啜��!
	//�v���C�}���[�����ɒ��ڂԂ�����.
	if (NowScreenMode.fFullScreen)
		lpDDSPrimary->BltFast(0,0,
				((IDirectDrawSurface*)(Sprite->Tag)),NULL,DDBLTFAST_NOCOLORKEY);
	else
	{	//�E�B���h���[�h  �N�\�o�J�Ȏd�l�ɂ��A BitFast �́A�g���Ȃ�.
		RECT DestRc;
		DDBLTFX ddbltfx = {0};
		ddbltfx.dwSize = sizeof(ddbltfx);
		DestRc.left  = DrawPointX;
		DestRc.top   = DrawPointY;
		DestRc.right = DrawPointX + NowScreenMode.iWidth;
		DestRc.bottom= DrawPointY + NowScreenMode.iHeight;
		lpDDSPrimary->Blt(&DestRc,
				((IDirectDrawSurface*)(Sprite->Tag)),NULL,DDBLTFAST_NOCOLORKEY,&ddbltfx);
	}
}

//�G��|�C���g�̐ݒ�(�E�B���h���[�h�݂̂ɗL���ł�.)
void DDXSetDrawPoint(int x,int y)
{
	//�E�B���h���[�h�̏ꍇ�́A���̃p�����[�^�̒l�ɂȂ�.
	DrawPointX = x;	DrawPointY = y;
}	
