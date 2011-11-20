// RfilesPreviewOne.cpp: RfilesPreviewOne �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RfilesPreviewOne.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RfilesPreviewOne::RfilesPreviewOne(string inPath)
{
	Path = inPath;
}
RfilesPreviewOne::~RfilesPreviewOne(){}

//�ꗗ�擾
void RfilesPreviewOne::ls()
{
	RTable * rt = getWritebleTable();

	rt->clear();

	RfilePreviewOne* p = (RfilePreviewOne*)RfileFactory::New(__RFILETYPE_PREVIEWONE);
	p->Attach( this->getPath().c_str() );
	rt->add( p );

//		rt->Sort();
	rt->createAllList( );
}
//�ꗗ�擾
void RfilesPreviewOne::ls(string inFilter)
{
	//��ʖ���
	ls();
}
//�폜
void RfilesPreviewOne::rm(string inFilename)
{
	//�T�|�[�g���܂���
}
//���l�[��
void RfilesPreviewOne::rename(string inSrcFilename,string inDestFilename)
{
	//�T�|�[�g���܂���
}
//��ނ�Ԃ�.
__RFILETYPE		RfilesPreviewOne::getRfileType()	const
{
	return __RFILETYPE_PREVIEWONE;
}
//�p�X�̎擾
string RfilesPreviewOne::getPath() const 
{
	return Path;
}
//�p�X�̐ݒ�
void		RfilesPreviewOne::setPath(const string inPath)  
{
	this->Path = inPath;
}

//RTable���쐬���܂��B
//ls ���g���O�ɌĂ�ł��������B
//�쐬�ł��Ȃ� __FILECOLUMN_TYPE ���n���ꂽ���O RNotSupportException��Ԃ��܂��B
void RfilesPreviewOne::MakeTable(__FILECOLUMN_TYPE inKey,bool IsAscending) throw()
{
	//�t�@�C����������Ȃ�����\�[�g�Ƃ����Ă�(w
	setTable( new RTableOnlyOne );
}

//�e�X�g.
void RfilesPreviewOne::test()
{
	{
		RfilesPreviewOne p("c:\\test\\2get.jpg");
		p.MakeTable(__FILECOLUMN_TYPE_EXT,true);

		try
		{
			//�ꗗ�擾
			p.ls(  );
		}
		catch(...)
		{
			return;
		}

		const RTable* rt = p.getTable();

		//��Έ�����Ȃ����B
		ASSERT( rt->getAllSize() == 1);

		//��Ɏw�肵���t�@�C�����������Ă��邩�`�F�b�N.
		const Rfile* *rfiles = rt->getAllList();
		const Rfile* one = rfiles[0];

		if (one->getVars(__FILECOLUMN_TYPE_NAME) != "c:\\test\\2get.jpg")
		{
			ASSERT(0);
		}
	}
}
