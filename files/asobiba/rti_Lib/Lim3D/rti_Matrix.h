#ifndef	___MAXTRIX___H	//�d���h�~
#define ___MAXTRIX___H

#include <./rti_define.h>
#include <./Base/rti_vector.h>

class TMatrix
{
	public:
		TMatrix();					//�R���X�g���N�^
		TMatrix(TMatrix &Ma);				//�R�s�[�R���X�g���N�^

		void Unit();					//�P�ʍs��̍쐬.
		void FastRev(TMatrix* inMat);	//�����t�s��
		void VectorMul(XYZFixed *Dest,  //�x�N�g���Ƃ̂����Z.
                XYZFixed const *Src);	
		void Synthesis(TMatrix *Ma);		//����.
		void MakeTrans(Angle AngleX,		//��]�s��쐬
			Angle AngleY,Angle AngleZ , Fixed Tx, Fixed Ty,Fixed Tz);
		void MakeTransYXZ(Angle AngleX,		//��]�s��쐬 (���_�Ȃ�)
			Angle AngleY,Angle AngleZ , Fixed Tx, Fixed Ty,Fixed Tz);
	protected:
		friend class TMatrix;
		Fixed	m_Matrix[4+4+4+4];
};


#endif			//�d���h�~
