/**************************************************************************************/
// Z Buffer
/**************************************************************************************/

#ifndef ___ZBUFFERH
#define ___ZBUFFERH

#include <string.h>
#include <.\Base\rti_fixed.h>

class TZBuffer
{
    int   BufferW,BufferH;
    int   BufferSize;   //ZBuffer�̃T�C�Y
    Fixed *ZBuffer;     //ZBuffer�̖{��.
public:
    //�R���X�g���N�^
    TZBuffer()
    {
        BufferSize = 0;
        ZBuffer    = NULL;
    }
    //�f�X�g���N�^
    ~TZBuffer()
    {
        Destroy();
    }
    //Z Buffer �̊m��
    bool Alloc(int w,int h)
    {
        Destroy();  //�O�̂��ߌ��ݎg���Ă���o�b�t�@���N���A.
        BufferW = w;
        BufferH = h;
        ZBuffer = new Fixed[ w*h ];
        BufferSize = sizeof(Fixed) * w * h;    //�o�b�t�@�T�C�Y(�o�C�g)

        if (!ZBuffer) return false;
        return true;
    }
    //Z Buffer �̏�����
    void Clear()
    {
        //ZBuffer �́A�߂�����f�J�C�l�ŏ��������Ă���.
        memset(ZBuffer , 0x7f7f7f7f,BufferSize);
    }
    //Z Buffer �̔j��.
    void Destroy()
    {
        delete [] ZBuffer;
		ZBuffer = NULL;
    }
    //�������M�̔j��.
    void Release()
    {
        delete this;
    }
    //ZBuffer �̎擾(�N���X�̃J�v�Z������j�󂷂�ƌ����Δj�󂵂܂��ˁ@���̃��\�b�h)
    Fixed* GetZBuffer()
    {
        return ZBuffer;
    }
    //�w�肵���ʒu�̃o�b�t�@�̈ʒu��Ԃ�.(�N���X�̃J�v�Z�������@�ȉ�����)
    Fixed* GetZBuffer(int w,int h)
    {
        return (ZBuffer + (h * BufferW) + w);
    }
    //�o�b�t�@�̃T�C�Y��Ԃ�(�o�C�g�P��.)
    int GetSize()
    {
        return BufferSize;
    }
    //1���C���̒���(�o�C�g�ɂ��炸)��Ԃ��܂�.
    int GetPitch()
    {
        return BufferW;
    }
};

typedef TZBuffer* LPZBUFFER;

#endif
