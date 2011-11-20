#ifndef __KEY_H
#define __KEY_H

/********************************************************************************/
//�Q�C�c�̃{�P������Ă��Ȃ� �L�[��`.
/********************************************************************************/
#define VK_EXECUTE      0x2B      //Execute�L�[
#define VK_SNAPSHOT     0x2C      //Print Screen�L�[ (Windows 3.0�ȍ~�p)
#define VK_INSERT       0x2D      //Ins�L�[
#define VK_DELETE       0x2E      //Del�L�[
#define VK_HELP 0x2F      //Help�L�[
#define VK_0    0x30      //0�L�[
#define VK_1    0x31      //1�L�[
#define VK_2    0x32      //2�L�[
#define VK_3    0x33      //3�L�[
#define VK_4    0x34      //4�L�[
#define VK_5    0x35      //5�L�[
#define VK_6    0x36      //6�L�[
#define VK_7    0x37      //7�L�[
#define VK_8    0x38      //8�L�[
#define VK_9    0x39      //9�L�[
#define VK_A    0x41      //A�L�[
#define VK_B    0x42      //B�L�[
#define VK_C    0x43      //C�L�[
#define VK_D    0x44      //D�L�[
#define VK_E    0x45      //E�L�[
#define VK_F    0x46      //F�L�[
#define VK_G    0x47      //G�L�[
#define VK_H    0x48      //H�L�[
#define VK_I    0x49      //I�L�[
#define VK_J    0x4A      //J�L�[
#define VK_K    0x4B      //K�L�[
#define VK_L    0x4C      //L�L�[
#define VK_M    0x4D      //M�L�[
#define VK_N    0x4E      //N�L�[
#define VK_O    0x4F      //O�L�[
#define VK_P    0x50      //P�L�[
#define VK_Q    0x51      //Q�L�[
#define VK_R    0x52      //R�L�[
#define VK_S    0x53      //S�L�[
#define VK_T    0x54      //T�L�[
#define VK_U    0x55      //U�L�[
#define VK_V    0x56      //V�L�[
#define VK_W    0x57      //W�L�[
#define VK_X    0x58      //X�L�[
#define VK_Y    0x59      //Y�L�[
#define VK_Z    0x5A      //Z�L�[
#define VK_F1   0x70      //F1�L�[
#define VK_F2   0x71      //F2�L�[
#define VK_F3   0x72      //F3�L�[
#define VK_F4   0x73      //F4�L�[
#define VK_F5   0x74      //F5�L�[
#define VK_F6   0x75      //F6�L�[
#define VK_F7   0x76      //F7�L�[
#define VK_F8   0x77      //F8�L�[
#define VK_F9   0x78      //F9�L�[
#define VK_F10  0x79      //F10�L�[
#define VK_F11  0x7A      //F11�L�[
#define VK_F12  0x7B      //F12�L�[
#define VK_F13  0x7C      //F13�L�[
#define VK_F14  0x7D      //F14�L�[
#define VK_F15  0x7E      //F15�L�[
#define VK_F16  0x7F      //F16�L�[
#define VK_F17  0x80     //F17�L�[
#define VK_F18  0x81     //F18�L�[
#define VK_F19  0x82     //F19�L�[
#define VK_F20  0x83     //F20�L�[
#define VK_F21  0x84     //F21�L�[
#define VK_F22  0x85     //F22�L�[
#define VK_F23  0x86     //F23�L�[
#define VK_F24  0x87     //F24�L�[

//���̃L�[��������Ă��邩���f
bool KeyPush(int vkey);
//�����͂�Ԃ�
bool KeyPushOne(int vkey);
unsigned long CheckKeyOnes(UINT Code,...);
unsigned long CheckKey(UINT Code,...);

extern int MousePX,MousePY;  //�}�E�X�̌��ݍ��W
extern int MouseRX,MouseRY;  //�}�E�X�̌��ݍ��W���A���^�C��
extern bool MouseCL,MouseCR;  //�}�E�X��������Ă���(1�񂾂�)
extern bool MouseEL,MouseER;  //�}�E�X��������Ă���
extern unsigned char LastKey;

#endif
