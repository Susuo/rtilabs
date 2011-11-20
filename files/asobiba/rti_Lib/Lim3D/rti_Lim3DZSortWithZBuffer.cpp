#include <./Lim3D/rti_Lim3DZSortWithZBuffer.h>

TLim3DZSortWithZBuffer::TLim3DZSortWithZBuffer()
{
	Clear();
}

TLim3DZSortWithZBuffer::~TLim3DZSortWithZBuffer()
{
	Clear();
}

//�j��.
void TLim3DZSortWithZBuffer::Release()
{
	delete this;
}


//�V�K�|���S���o�^.
bool TLim3DZSortWithZBuffer::Add(TLim3DPolygon* inPolygon)
{
	__RTI_CHECKER(inPolygon != NULL);
	//����ȏ�o�^�s��.
	__RTI_CHECKER(m_NowLast <  MAX_POLYGON);

	//���̃|���S���̓o�^.
	m_PoolPolygon[m_NowLast++] = inPolygon;
	return true;
}

//���ׂẴ|���S���̔j��.
void TLim3DZSortWithZBuffer::Clear()
{
	m_NowLast = 0;
}

//���_�ɋ߂������Ȃ�ׂĂ����܂�.
void TLim3DZSortWithZBuffer::Sort()
{
	if (m_NowLast > 1)	QuickSort(m_PoolPolygon,  0 , m_NowLast - 1);
}

//���ׂĊG��.
void TLim3DZSortWithZBuffer::AllDraw()
{
	TLim3DPolygon**	theData = m_PoolPolygon;
	int				theI  ;
	int				theEnd = m_NowLast;
	for( theI = 0 ; theI < theEnd ; theI ++ , theData++)
	{	//���ۂɉ�ʂɏ�������.
		(*theData)->TrueDraw();
	}
}

void TLim3DZSortWithZBuffer::QuickSort(TLim3DPolygon*  a[], int first, int last)
{
	__RTI_CHECKER(a != NULL);

	int i, j;
	TLim3DPolygon  *x, *t;

	x = a[(first + last) / 2];
	i = first;  j = last;
	for ( ; ; ) {
		while (a[i]->GetEyeAvgZ() < x->GetEyeAvgZ() ) i++;
		while (x->GetEyeAvgZ() < a[j]->GetEyeAvgZ() ) j--;
		if (i >= j) break;
		t = a[i];  a[i] = a[j];  a[j] = t;
		i++;  j--;
	}
	if (first  < i - 1) QuickSort(a, first , i - 1);
	if (j + 1 < last) QuickSort(a, j + 1, last);
}
