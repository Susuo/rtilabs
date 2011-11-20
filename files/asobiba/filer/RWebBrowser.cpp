// RWebBrowser.cpp: RWebBrowser �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RWebBrowser.h"

#include "Filer.h"
#include "RfilesPreviewOne.h"
#include "DisplayFactory.h"
#include "RProgressBar.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

RWebBrowser::RWebBrowser()
{
	Parent = NULL;
}

RWebBrowser::~RWebBrowser()
{
}

//�쐬.
//���s	RNotSupportException 
//		RDoNotInitaliseException
void RWebBrowser::Create(HINSTANCE inINstance,
					   HWND isParentWindow,int inID, DWORD inFlg) throw()
{

	RActiveXWindow::Create(inINstance,"Shell.Explorer.2" , IID_IWebBrowser2, (void**)&WebBrowser,isParentWindow,0, 0 , 0);

	// �V���N�I�u�W�F�N�g�̍쐬.
	CComObject<RWebBrowserSink>::CreateInstance( &Sink);	//���̒��� new ���Ă���炵��.

	// �C�x���g��Dispatch����悤�AIE�R���g���[���ɂ��肢.
	HRESULT hr = Sink->AdviseTo( this , WebBrowser);

}

void RWebBrowser::Navigate2(const string inUrl)
{
	ASSERT(WebBrowser != NULL);

    CComVariant ve;
    CComVariant vurl( inUrl.c_str() );
    WebBrowser->put_Visible(VARIANT_TRUE);
	WebBrowser->put_MenuBar(VARIANT_TRUE);
	WebBrowser->put_AddressBar(VARIANT_TRUE);
	WebBrowser->put_StatusBar(VARIANT_TRUE);
	WebBrowser->put_ToolBar(VARIANT_TRUE);
    WebBrowser->Navigate2(&vurl, &ve, &ve, &ve, &ve);
}

//�u���E�Y���J�n���ꂽ�Ƃ��ɌĂ΂��.
HRESULT RWebBrowser::OnBeforNavigate2(IDispatch* pDisp, VARIANT* url, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, VARIANT_BOOL* Cancel)
{
	USES_CONVERSION;

	const char* p = OLE2CT(url->bstrVal);
	Parent->OnQuickChange( string(p) );

	return S_OK;
}

//�I���̂Ƃ��Ă΂��
HRESULT RWebBrowser::OnQuit()
{
	Sink->DispEventUnadvise( WebBrowser );
	Sink = NULL;

	return S_OK;
}


//�V�����E�B���h�E���J�����Ƃ��ɌĂ΂��
void RWebBrowser::OnNewWindow2(IDispatch** ppDisp,VARIANT_BOOL* Cancel)
{
	//���Ƃ��ƁA�p�X -> �E�B���h�E�쐬�@�Ƃ����l�����ł��܂������A
	//OnNewWindow �� �E�B���h�E�쐬 -> �p�X�Ƃ����t�̎菇�ō��Ȃ��Ă͂Ȃ�Ȃ��̂ŁA
	//�ƂĂ������Ȃ�̂ł����A�����ɃE�B���h�E�����܂��B

	Filer * filer = Filer::getFiler();
	FilerSub * sub = filer->getFilerSub( filer->IsLeftFocus() );

	Rfiles * rfiles = NULL;
	DisplayWebBrowserView* dis = NULL;
	Window* win = NULL;

	try
	{
		win = sub->NewWindow("");

		rfiles = new RfilesPreviewOne("");
		rfiles->MakeTable( __FILECOLUMN_TYPE_NAME , true);
		rfiles->ls();

		dis = (DisplayWebBrowserView*) DisplayFactory::New(__DISPLAYTYPE_PREVIEW_WEBBROWSER , win ,__RFILETYPE_PREVIEWONE);

		win->UpdateWindow( rfiles , dis );
	}
	catch(RException e)
	{
		if (rfiles) rfiles->Dec();
		if (dis) dis->Dec();

		if (win) sub->CloseWindow(win);

		*Cancel = VARIANT_TRUE;
		return ;
	}

	const RWebBrowser*	browser = dis->getBrowser();
	browser->WebBrowser->QueryInterface(IID_IDispatch, (void**)ppDisp);

	*Cancel = VARIANT_FALSE;
}


//�ǂݍ��݂��I������Ƃ�
void RWebBrowser::OnNavigateComplete2(IDispatch* pDisp, VARIANT* URL)
{
}

//�G���[���o���Ƃ�
void RWebBrowser::OnNavigateError(IDispatch* pDisp,VARIANT* URL, VARIANT* TargetFrameName,	VARIANT* StatusCode,VARIANT_BOOL* &Cancel)
{
}

//�^�C�g�����ύX����܂���
void RWebBrowser::OnTitleChange(BSTR Text)
{
	USES_CONVERSION;
	Title = OLE2CT(Text);

	//�e�ɒʒm
	Parent->OnTitleChange();
}

//�X�e�[�^�X�o�[�̃e�L�X�g���ύX����܂���
void RWebBrowser::OnStatusTextChange(BSTR Text)
{
	if ( ocslen(Text) == 0 ) return ;

	Filer * filer = Filer::getFiler();
	FilerSub * sub = filer->getFilerSub( filer->IsLeftFocus() );

	USES_CONVERSION;
	const char*  t = OLE2CT(Text);
	if (t != NULL)	sub->getStatue()->setStatue( string(t) );
	else			sub->getStatue()->setStatue( "" );
}

//�v���O���X�o�[���ω����܂���.
void RWebBrowser::OnProgressChange(long Progress,	long ProgressMax)
{
	Filer * filer = Filer::getFiler();
	FilerSub * sub = filer->getFilerSub( filer->IsLeftFocus() );

	if (ProgressMax == 0)	//�ǂݍ��݂����������肷��� 0 ������炵��.
		sub->getStatue()->setProgressBar( 0 );
	else
		sub->getStatue()->setProgressBar( CHANGE_SCALE( Progress , ProgressMax , 100) );
}

//�`��܂ł��ׂďI�������Ƃ��ɌĂ΂��.
//OnNavigateComplete2 -> OnDocumentComplete �̏���
//	OnNavigateComplete2	�ʐM�œ��e�����ׂĎ擾�ł��܂����B
//	OnDocumentComplete	�`��܂ŏI���܂����B
void RWebBrowser::OnDocumentComplete(IDispatch* pDisp, VARIANT* URL)
{
	Filer * filer = Filer::getFiler();
	FilerSub * sub = filer->getFilerSub( filer->IsLeftFocus() );
	sub->getStatue()->setProgressBar( 100 );
}


//�^�C�g�����擾����
string RWebBrowser::getTitle() const 
{
	return Title;
}

