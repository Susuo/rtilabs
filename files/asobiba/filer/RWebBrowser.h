// RWebBrowser.h: RWebBrowser �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RWEBBROWSER_H__624F33B3_0EFF_4348_AE82_F4A9C38F4257__INCLUDED_)
#define AFX_RWEBBROWSER_H__624F33B3_0EFF_4348_AE82_F4A9C38F4257__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RWebBrowser;
class RWebBrowserSink;
class DisplayWebBrowserView;

#include "comm.h"
#include "RActiveXWindow.h"
#include <Exdisp.h>
#include <Exdispid.h>

//Web�u���E�U�[
class RWebBrowser :
	public RActiveXWindow  
{
public:
	RWebBrowser();
	virtual ~RWebBrowser();

	//�쐬.
	//���s	RNotSupportException 
	//		RDoNotInitaliseException
	void Create(HINSTANCE inINstance,
		HWND isParentWindow,int inID , DWORD inFlg) throw();
	//�e�̃Z�b�g.
	void setParent(DisplayWebBrowserView* inParent)
	{
		Parent = inParent;
	}

	//�\��.
	void Navigate2(const string inUrl);
	//�^�C�g�����擾����
	string getTitle() const ;

public:
	//�u���E�Y���J�n���ꂽ�Ƃ��ɌĂ΂��.
	HRESULT OnBeforNavigate2(IDispatch* pDisp, VARIANT* url, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, VARIANT_BOOL* Cancel);
	//�I���̂Ƃ��Ă΂��
	HRESULT OnQuit();
	//�V�����E�B���h�E���J�����Ƃ��ɌĂ΂��
	void OnNewWindow2(IDispatch** ppDisp,VARIANT_BOOL* Cancel);

	//�ǂݍ��݂��I������Ƃ�
	void OnNavigateComplete2(IDispatch* pDisp, VARIANT* URL);

	//�G���[���o���Ƃ�
	void OnNavigateError(IDispatch* pDisp,VARIANT* URL, VARIANT* TargetFrameName,	VARIANT* StatusCode,VARIANT_BOOL* &Cancel);

	//�^�C�g�����ύX����܂���
	void OnTitleChange(BSTR Text);

	//�X�e�[�^�X�o�[�̃e�L�X�g���ύX����܂���
	void OnStatusTextChange(BSTR Text);

	//�v���O���X�o�[���ω����܂���.
	void OnProgressChange(long Progress,	long ProgressMax);

	//�`��܂ł��ׂďI�������Ƃ��ɌĂ΂��.
	//OnNavigateComplete2 -> OnDocumentComplete �̏���
	//	OnNavigateComplete2	�ʐM�œ��e�����ׂĎ擾�ł��܂����B
	//	OnDocumentComplete	�`��܂ŏI���܂����B
	void OnDocumentComplete(IDispatch* pDisp, VARIANT* URL);


private:
	CComPtr<IWebBrowser2>	WebBrowser;
	CComObject<RWebBrowserSink>* Sink;

	DisplayWebBrowserView*		Parent;
	//�^�C�g��.
	string Title;
};



#define SINKID_COUNTEREVENTS 1

//�u���E�U�Ŕ��������C�x���g��߂܂���.
//�Q�l	http://www.nitoyon.com/vc/tips/ie_component.htm
//		http://www.s34.co.jp/cpptechdoc/misc/comevent/index.html
class RWebBrowserSink :
	public CComObjectRootEx<CComSingleThreadModel>	,
	public IDispEventImpl< SINKID_COUNTEREVENTS ,RWebBrowserSink , &DIID_DWebBrowserEvents2 , &LIBID_SHDocVw, 1, 0>
{
private:
	BEGIN_COM_MAP(RWebBrowserSink)
		COM_INTERFACE_ENTRY_IID(DIID_DWebBrowserEvents2, RWebBrowserSink)
	END_COM_MAP()
	BEGIN_SINK_MAP(RWebBrowserSink)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_BEFORENAVIGATE2 , OnBeforNavigate2)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_ONQUIT , OnQuit)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_NEWWINDOW2 , OnNewWindow2)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_NAVIGATECOMPLETE2 , OnNavigateComplete2)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_NAVIGATEERROR , OnNavigateError)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_TITLECHANGE , OnTitleChange)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_STATUSTEXTCHANGE , OnStatusTextChange)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_PROGRESSCHANGE , OnProgressChange)
	  SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2,	DISPID_DOCUMENTCOMPLETE , OnDocumentComplete)
	END_SINK_MAP()
public:
	HRESULT AdviseTo(RWebBrowser* inCallBack , IUnknown* pUnkIE)
	{
		//�R�[���o�b�N�o�^.
		CallBack = inCallBack;

		// �^�C�v ���C�u�����ւ̃|�C���^��o�[�W�������̐ݒ�
		AtlGetObjectSourceInterface( pUnkIE, &m_libid, &m_iid, &m_wMajorVerNum, &m_wMinorVerNum) ;
		// �C�x���g��Dispatch
		HRESULT hr = this->DispEventAdvise( pUnkIE);
		return hr ;
	}
private:
    
	//�u���E�Y���J�n���ꂽ�Ƃ��ɌĂ΂��.
	HRESULT _stdcall OnBeforNavigate2(IDispatch* pDisp, VARIANT* url, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, VARIANT_BOOL* Cancel)
	{
		return CallBack->OnBeforNavigate2( pDisp, url, Flags, TargetFrameName, PostData, Headers, Cancel);
	}
	//�I���̂Ƃ��Ă΂��
	HRESULT _stdcall OnQuit()
	{
		return CallBack->OnQuit();
	}
	//�V�����E�B���h�E���J�����Ƃ��ɌĂ΂��
	void _stdcall OnNewWindow2(IDispatch** ppDisp,VARIANT_BOOL* Cancel)
	{
		CallBack->OnNewWindow2(ppDisp , Cancel );
	}

	//�ǂݍ��݂��I������Ƃ�
	void _stdcall OnNavigateComplete2(IDispatch* pDisp, VARIANT* URL)
	{
		CallBack->OnNavigateComplete2(pDisp,URL);
	}

	//�G���[���o���Ƃ�
	void _stdcall OnNavigateError(IDispatch* pDisp,VARIANT* URL, VARIANT* TargetFrameName,	VARIANT* StatusCode,VARIANT_BOOL* &Cancel)
	{
		CallBack->OnNavigateError(pDisp, URL, TargetFrameName,	StatusCode,Cancel);
	}

	//�^�C�g�����ύX����܂���
	void _stdcall OnTitleChange(BSTR Text)
	{
		CallBack->OnTitleChange(Text);
	}

	//�X�e�[�^�X�o�[�̃e�L�X�g���ύX����܂���
	void _stdcall OnStatusTextChange(BSTR Text)
	{
		CallBack->OnStatusTextChange(Text);
	}
	
	//�v���O���X�o�[���ω����܂���.
	void _stdcall OnProgressChange(long Progress,	long ProgressMax)
	{
		CallBack->OnProgressChange(Progress , ProgressMax);
	}

	//�`��܂ł��ׂďI�������Ƃ��ɌĂ΂��.
	//OnNavigateComplete2 -> OnDocumentComplete �̏���
	//	OnNavigateComplete2	�ʐM�œ��e�����ׂĎ擾�ł��܂����B
	//	OnDocumentComplete	�`��܂ŏI���܂����B
	void _stdcall OnDocumentComplete(IDispatch* pDisp, VARIANT* URL)
	{
		CallBack->OnDocumentComplete(pDisp , URL);
	}

	//�R�[���o�b�N��.
	RWebBrowser* CallBack;
};

#include "DisplayWebBrowserView.h"

#endif // !defined(AFX_RWEBBROWSER_H__624F33B3_0EFF_4348_AE82_F4A9C38F4257__INCLUDED_)
