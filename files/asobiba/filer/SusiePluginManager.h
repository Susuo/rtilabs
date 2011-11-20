// SusiePluginManager.h: SusiePluginManager �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUSIEPLUGINMANAGER_H__F17020E2_ACF9_11D6_A5D3_D0D6ADCDBD4F__INCLUDED_)
#define AFX_SUSIEPLUGINMANAGER_H__F17020E2_ACF9_11D6_A5D3_D0D6ADCDBD4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "SusiePluginWapper.h"

typedef deque<string> SUSIE_EXT;

class SusiePluginManager  
{
private:
	//singleton
	SusiePluginManager();
public:
	virtual ~SusiePluginManager();

	//�C���X�^���X�擾
	static SusiePluginManager*	getInstance();

	//�g���q�ꗗ�̎擾
	SUSIE_EXT getSupportExt() ;


	//�T�|�[�g���Ă���v���O�C�����ڂ��
	//�݂���Ȃ��ꍇ�� RNotSupportException 
	const SusiePluginWapper* getSupportPlugin(const string inFilename,const string inExt) throw();
private:
	//�v���O�C�����[�h
	string PluginLoad(const string inPluginPath);
	//�Ή�����v���O�C�����}�b�s���O.
	void AppendSupportBy(SusiePluginWapper* inSusiePlugin) throw();
private:
	typedef deque<SusiePluginWapper*>		PLUGINES;
	typedef multimap< string, SusiePluginWapper*>	EXTMAPPER;
	typedef EXTMAPPER::iterator MULTIMAP_RANGE_ITERATOR;

	EXTMAPPER	ExtMapper;
	PLUGINES	Plugines;
};

#endif // !defined(AFX_SUSIEPLUGINMANAGER_H__F17020E2_ACF9_11D6_A5D3_D0D6ADCDBD4F__INCLUDED_)
