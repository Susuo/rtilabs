// MVReaderThread.cpp: MVReaderThread �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MVReaderThread.h"
#include "MVImage.h"
#include "MVStaticZLib.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

MVReaderThread::MVReaderThread()
{
	this->PoolData = NULL;
	this->PoolPageData = NULL;
}

MVReaderThread::~MVReaderThread()
{
	delete [] PoolData;

	//���ʕ���N���󂯎���Ă���Ȃ�������A�����ŏ�������.
	delete [] PoolPageData;
}

/**
 * Setting:			��ƂɕK�v�ȍޗ����X���b�h�ɓn��
 *					���̎��X���b�h�͂܂����삵�Ă��Ȃ��B
 *
 * @param poolData				���k����Ă���f�[�^ (�X���b�h����ƏI����A�ӔC�������ĊJ�����܂�)
 * @param inSize				poolData �̃T�C�Y
 * @param inUncompressedSize	poolData ���𓀂����ꍇ�̃T�C�Y
 * @param compressionMethod		���k�Ɏg�p����Ă���A���S���Y��
 * @param inImageConvert		�ϊ��Ɏg�p����p�����[�^
 * @return void 
 */
void MVReaderThread::Setting(char* poolData , int inSize , int inUncompressedSize  , int compressionMethod ,
							 const MVImagePreConvert * inImageConvertParam)
{
	//�����Ă̓_����.
	ASSERT( ! this->IsAlive() );
	ASSERT( this->PoolPageData == NULL);

	this->PoolData = poolData;
	this->Size = inSize;
	this->CompressionMethod = compressionMethod;
	this->UncompressedSize = inUncompressedSize;

	this->ImageConvertParam = *inImageConvertParam;


	ASSERT( (this->CompressionMethod == 0 && (this->UncompressedSize  == this->Size)) ||
			(this->CompressionMethod != 0)
		);
}




/**
 * TakeoverPageData:���ʕ������������܂��B
 *					���̎��X���b�h�͓�����I�����Ă��Ȃ���΂Ȃ�܂���.
 *
 *					��x�ĂԂƃX���b�h�� ���ʕ� ���A�Ă񂾓z�ɂ킽�������̂��Ǝv���f�[�^�̏��L����j������̂Œ���
 *
 * @return MVPageData*	���ʕ�	���̐��ʕ��́A�󂯎�����z���ӔC�������ĊJ������K�v������܂�.
 */
MVPageData* MVReaderThread::TakeoverPageData() throw(RException)
{
	//�I����Ă��Ȃ���_����.
	ASSERT( ! this->IsAlive() );
	//���ʂ��ł��Ă��Ȃ��Ƃ������Ƃ́A��x�������Ă��Ȃ���
	ASSERT( this->PoolPageData != NULL);


	//�G���[���N���Ă���Ȃ�A�G���[���O�Ƃ��ĕԂ�.
	if ( ! this->Error.empty() )
	{
		//���ʕ��͉��Ă���Ǝv���̂Ŕj������.
		delete this->PoolPageData;
		this->PoolPageData = NULL;

		throw RException( this->Error );
	}

	MVPageData* ret = this->PoolPageData;

	//�����̊Ǌ�����͂Ȃ�邩��A�����m��ˁ[
	this->PoolPageData = NULL;

	return ret;

}



//�X���b�h����
unsigned int MVReaderThread::Run() 
{
	try
	{
		UnComp();
	}
	catch(RException & e)
	{
		this->Error = e.getMessage();
	}
	//���k���ꂽ�f�[�^�͂����s�v�Ȃ̂Ŕj�����܂�.
	delete [] PoolData;
	this->PoolData = NULL;

	return 0;
}


/**
 * UnComp:				�X���b�h�����Ŏ��ۂ̓W�J������S������.
 *
 * @return void 
 */
void MVReaderThread::UnComp()  throw(RException)
{
	//���������Ă��܂����
	ASSERT( this->PoolData != NULL);

	//�X���b�h���Ő�������f�[�^�͂܂�����Ă��܂�����
	ASSERT( this->PoolPageData == NULL);

	//���ʕ������߂�̈�.
	this->PoolPageData = new MVPageData;


	if (this->CompressionMethod == 8)
	{
		RBuffer bitmapMemory;
		//�K�v�ȕ����������m�ۂ���.
		bitmapMemory.reserve(this->UncompressedSize);
		bitmapMemory.resize(this->UncompressedSize);

		//���k
		MVStaticZLib::getInstance()->UnCompressed((unsigned char*) &bitmapMemory[0] , this->UncompressedSize ,
												(const unsigned char*) this->PoolData , this->Size );

		//�W�J
		this->PoolPageData->Load(&bitmapMemory[0] , this->UncompressedSize);
	}
	else if (this->CompressionMethod == 0 && this->UncompressedSize == this->Size)
	{
		//�����k

		//�W�J
		this->PoolPageData->Load(this->PoolData , this->Size);
	}
	else
	{
		throw RException("���Ή��� zip " + num2str(this->CompressionMethod) + "�`�����n����܂���");
	}


	if ( this->ImageConvertParam.getJustFit() )
	{
		switch ( this->ImageConvertParam.getScrollType() )
		{
		case MVConfig::YOKO:
			{
				//�c���ɂ��킹��.
				double scale = (double)this->ImageConvertParam.getScreenHeight() / this->PoolPageData->getHeight();

				if (scale != 0)
				{
					this->PoolPageData->ChangeScale(this->PoolPageData->getWidth() * scale , 
													this->PoolPageData->getHeight() * scale );
				}
			}
			break;
		case MVConfig::TATE:
			{
				//�����ɂ��킹��.
				double scale = (double)this->ImageConvertParam.getScreenWidth() / this->PoolPageData->getWidth();

				if (scale != 0)
				{
					this->PoolPageData->ChangeScale(this->PoolPageData->getWidth() * scale , 
													this->PoolPageData->getHeight() * scale );
				}
			}
			break;
		}
	}

}

