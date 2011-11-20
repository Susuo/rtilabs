// RTableEasy.cpp: RTableEasy �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RTableEasy.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
#include "RfilesLocal.h"

//�e�X�g.
void RTableEasy::test()
{
	RfilesLocal p(__RFILETYPE_LOCALFILE_REPORT,"c:\\test\\");
	RTableEasy a(__FILECOLUMN_TYPE_EXT,true);
	RfileLocalReport* rlr;

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ggg.dat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag4.dat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag3.pat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag2.aat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag.eat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag.gat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag.mat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag.nat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag.pat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag.fat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	rlr = new RfileLocalReport();
	rlr->Attach(&p,"ag.bat",100,100,100);
	rlr->SortAttach(__FILECOLUMN_TYPE_EXT);
	a.add( rlr );

	a.Sort();


	a.createAllList(  );

	DisplayDebugPrintf d;
	d.setRfileItems( a.getAllSize() , a.getAllList( ) );

	string DebugString = "ag.bat �� �t�@�C�� ag.eat �� �t�@�C�� ag.fat �� �t�@�C�� ag.gat �� �t�@�C�� ag.mat �� �t�@�C�� ag.nat �� �t�@�C�� ag.pat �� �t�@�C�� ag2.aat �� �t�@�C�� ag3.pat �� �t�@�C�� ag4.dat �� �t�@�C�� ggg.dat �� �t�@�C�� ";
	if ( d.DebugBuffer != DebugString )
	{
		TRACE("\n%s\n",d.DebugBuffer.c_str());
		ASSERT(0);
	}
}
