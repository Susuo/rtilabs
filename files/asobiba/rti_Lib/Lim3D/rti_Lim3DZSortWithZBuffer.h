#ifndef	___LIM3D_ZSORT_WITH_ZBUFFER_H	//�d���h�~
#define ___LIM3D_ZSORT_WITH_ZBUFFER_H

#include <./Lim3D/rti_Lim3DModel.h>
#include <./Lim3D/rti_Lim3DDefine.h>

class TLim3DPolygon;

//�ő�1���|���S��!! �ł��邩�Ȃ�(���ߑ�)
#define MAX_POLYGON	10000

//Zbuffer �̗��_�𐶂������߂� ZSort ���܂�(��) �܂�A��O���炩���Ă����܂�.
class	TLim3DZSortWithZBuffer 
{
public:
	TLim3DZSortWithZBuffer();
	~TLim3DZSortWithZBuffer();

	//�V�K�|���S���o�^.
	bool Add(TLim3DPolygon* inPolygon);
	//���ׂẴ|���S���̔j��.
	void Clear();
	//�j��.
	void Release();
	//���_�ɋ߂������Ȃ�ׂĂ����܂�.
	void Sort();
	//���ׂĊG��.
	void AllDraw();
protected:
	//���ۂɃ\�[�g����Ƃ���
	static void QuickSort(TLim3DPolygon*  a[], int first, int last);

	//�|���S�������Ă����z��(���x�D��̂��ߌŒ�ł�(��))
	TLim3DPolygon* 	m_PoolPolygon[MAX_POLYGON];
	int				m_NowLast;
private:
};

#endif							//�d���h�~
