#include <./Lim3D/rti_Lim3DMaster.h>

//�e��Ǘ��N���X
TLim3DEyeCaretaker		TLim3DMaster::m_EyeCaretaker;
TLim3DModelCaretaker	TLim3DMaster::m_ModelCaretaker;
TLim3DLightCaretaker	TLim3DMaster::m_LightCaretaker;
TLim3DMaterialCaretaker	TLim3DMaster::m_MaterialCaretaker;

//�A�j���[�V����.
TLim3DAnimeControl		TLim3DMaster::m_AnimeControl;

//�e�N�X�`���[
TBmpChar				TLim3DMaster::m_TextureCaretaker;

//�l�[���T�[�o�[
TStringHashTable		TLim3DMaster::NameServer;
//ZBuffer
TZBuffer				TLim3DMaster::m_ZBuffer;
//�|���S���� ZBuffer
TLim3DZSortWithZBuffer	TLim3DMaster::m_ZSortBuffer;
//�I������Ă��鎋�_.
TLim3DEye*				TLim3DMaster::m_SelectEye;
//�G�悷���ʗ̈�.
BMD						TLim3DMaster::m_SelectBMD;

