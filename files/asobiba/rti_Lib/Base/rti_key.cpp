#include <windows.h>
#include <.\Base\rti_key.h>

#define KEY_MAX 256
int MousePX,MousePY;  //�}�E�X�̌��ݍ��W
int MouseRX,MouseRY;  //�}�E�X�̌��ݍ��W���A���^�C��
bool MouseCL,MouseCR;  //�}�E�X��������Ă���(1�񂾂�)
bool MouseEL,MouseER;  //�}�E�X��������Ă���
unsigned char LastKey;


unsigned long CheckKey(UINT Code,...)
{
	int i;    // �ėp�J�E���^�[
	unsigned long Key; // �����ꂽ�L�[�R�[�h
    unsigned long ret = 0;

	for (i=0;*(&Code+i)!=NULL;i++)
	{
		Key=*(&Code+i);
		if (GetAsyncKeyState(Key) & 0x8000)
		{
            ret |= 0x01 << i;
		}
	}
    return ret;

}

unsigned long CheckKeyOnes(UINT Code,...)
{
	int i;    // �ėp�J�E���^�[
	unsigned long Key; // �����ꂽ�L�[�R�[�h
    unsigned long ret = 0;

	for (i=0;*(&Code+i)!=NULL;i++)
	{
		Key=*(&Code+i);
		if (  (GetAsyncKeyState(Key) & 0x8001) == 0x8000   )
		{
            ret |= 0x01 << i;
		}
	}
    return ret;

}

//���̃L�[��������Ă��邩���f
bool KeyPush(int vkey)
{
    return (bool)(GetAsyncKeyState(vkey) & 0x8000);
}

//�����͂�Ԃ�
bool KeyPushOne(int vkey)
{
    return ( (GetAsyncKeyState(vkey) & 0x8001) == 0x8000 );
}


