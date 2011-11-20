// RHtmlTemplate.h: RHtmlTemplate �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RHTMLTEMPLATE_H__D00CDA57_C017_4322_AF00_78222B5F7B19__INCLUDED_)
#define AFX_RHTMLTEMPLATE_H__D00CDA57_C017_4322_AF00_78222B5F7B19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RParseException.h"
#include "RIOCannotOpenException.h"


/**
 * class RHtmlTemplateContent:		.tpl ���p�[�X�������ʁ@�C���`�L�I�[�g�}�g���̒P��
 *
 */
class RHtmlTemplateContent
{
public:
	enum TemplateFlg
	{
		StaticContent,
		DynamicContent,
	};
	//�{���� �p���ł�肽���񂾂��ǁA virtual �̏����̓C�����C���������Ȃ��̂ŁA�t���O���g���܂��B
	RHtmlTemplateContent(TemplateFlg inFlg , const string inUseData)
	{
		this->Flg = inFlg;
		this->UseData = inUseData;
	}
	//�f�[�^�̎擾.
	inline const string getConect(const RStringMap * inReplaceStringMap) const
	{
		if (this->Flg == StaticContent)
		{	//���̂܂܂̃f�[�^.
			return this->UseData;
		}
		else
		{	//�u�����ĕԂ�.
			RStringMap::const_iterator i = inReplaceStringMap->find(this->UseData);
			if (i == inReplaceStringMap->end() ) return "";

			return (*i).second;
		}
	}
	//�f�[�^�̒����̎擾.
	inline int getConectLength(const RStringMap * inReplaceStringMap) const
	{
		if (this->Flg == StaticContent)
		{	//���̂܂܂̃f�[�^.
			return this->UseData.size();
		}
		else
		{	//�u�����ĕԂ�.
			RStringMap::const_iterator i = inReplaceStringMap->find(this->UseData);
			if (i == inReplaceStringMap->end() ) return 0;

			return (*i).second.size();
		}
	}

	//�f�o�b�O�p�� Dump.
	string Dump() const
	{
		return this->UseData;
	}
private:
	TemplateFlg		Flg;
	string			UseData;
};

/**
 * class RHtmlTemplate:		�f�U�C��
 *
 */
class RHtmlTemplate  
{
public:
	RHtmlTemplate();
	virtual ~RHtmlTemplate();

	//.tpl ��ǂݍ���
	void Load(const string inFilename) throw(RIOCannotOpenException,RParseException);
	//�q���̏ꍇ.
	void Load(const RHtmlTemplate* inPerent ,const string inContect, int inStartLine) throw(RParseException);

	//���[�v�̎擾
	RHtmlTemplate* Select(const string inPartName) const
	{
		RHtmlTemplateMap::const_iterator i = Loop.find(inPartName);
		if (i == Loop.end() ) return NULL;
		return (*i).second;
	}
	/**
	 * Replace:						�u�����s��
	 *
	 * @param inReplaceStringMap	�u�����镶����̃}�b�v
	 * @return string				���� 
	 */
	const string Replace(const RStringMap * inReplaceStringMap) const;

	void Dump() const;
	static void test();

private:
	/**
	 * Parse:						���e����͂��A�ÓI�A���[�v�A�u�� �̍��ڕʂɃ��X�g�\���ɕϊ����܂��B
	 *								�p�[�X�͉����Ȃ�܂���(w
	 *
	 * @param inTargetBuffer		��͂����l �Ō�� \0 �������Ă��邱�Ƃ͕ۏ؂����
	 * @param inSize				��͂����l�̃T�C�Y
	 * @param inStartLine			.tpl �t�@�C���ł�inTargetBuffer������s�� �G���[�s���o�͂���Ƃ��Ɏg��
	 * @return void					
	 * @exception	RParseException	�\����̓G���[
	 */
	void Parse(const char* inTargetBuffer , int inSize, int inStartLine) throw(RParseException);
	/**
	 * ParseValue:					���e����͂��A�ÓI�A�u���A�̍��ڕʂɃ��X�g�\���ɕϊ����܂��B
	 *								�p�[�X�͉����Ȃ�܂���(w
	 *
	 * @param inTargetBuffer		��͂����l �Ō�� \0 �������Ă��邱�Ƃ͕ۏ؂����
	 * @param inSize				��͂����l�̃T�C�Y
	 * @param inStartLine			.tpl �t�@�C���ł�inTargetBuffer������s�� �G���[�s���o�͂���Ƃ��Ɏg��
	 * @return void					
	 * @exception	RParseException	�\����̓G���[
	 */
	void ParseValue(const char* inTargetBuffer,int inSize , int inStartLine) throw(RParseException);

	/**
	 * getline:				�s���𐔂���
	 *
	 * @param inStart		��������͂��܂���
	 * @param inTarget		�����܂�
	 * @return int			�s��
	 */
	int RHtmlTemplate::getline(const char * inStart , const char* inEnd) const;

	/**
	 * CleanUp:				new �Ŋm�ۂ������̂̉��
	 *
	 * @return void 
	 */
	void CleanUp();
private:
	typedef map<string,RHtmlTemplate*>	RHtmlTemplateMap;
	typedef deque<RHtmlTemplateContent*>	RHtmlTemplateValueMap;

	//�������ێ����郋�[�v.
	RHtmlTemplateMap		Loop;
	//�������ێ�����u���ꏊ�� index. ��������}��.
	RHtmlTemplateValueMap	ReplaceIndex;

	const RHtmlTemplate* Perent;
};

#endif // !defined(AFX_RHTMLTEMPLATE_H__D00CDA57_C017_4322_AF00_78222B5F7B19__INCLUDED_)
