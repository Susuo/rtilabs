// RFlashPlayer.h: RFlashPlayer クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFLASHPLAYER_H__850BD331_47F2_4CBE_BC2C_AD6E646B4157__INCLUDED_)
#define AFX_RFLASHPLAYER_H__850BD331_47F2_4CBE_BC2C_AD6E646B4157__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RActiveXWindow.h"
#include "RMediaViewPlayer.h"
#include "RNotSupportException.h"
#include "AtlActiveXBrige.h"
#import "C:\WINNT\system32\Macromed\Flash\Flash.ocx" named_guids raw_interfaces_only exclude("IServiceProvider") include("DISPPARAMS")

class RFlashPlayer : 
	public RMediaViewPlayer, 
	public RActiveXWindow  
{
public:
	RFlashPlayer();
	virtual ~RFlashPlayer();
	//実態の作成
	void Create(HINSTANCE inINstance,HWND isParentWindow) throw();

	//ファイルを開く
	virtual void Open(const string inFilename )	throw() ;

	//再生する.
	virtual void Play() throw() ;

	//停止する.
	virtual void Stop() throw() ;

	//一時停止 / 解除
	virtual void Pause(bool inON)	throw() ;

	//ファイルクローズ
	virtual void Close()	throw() ;

	//ボリューム
	virtual int getVolum() const throw() ;

	virtual void setVolum(int inVolum) throw() ;

	//再生位置
	virtual int  getPos() const throw() ;

	virtual void setPos(int inPos) throw() ;

	//再生中?
	virtual bool IsPlay() const ;

	//一時停止中?
	virtual bool IsPause() const ;
	//サポートしてます?
	static bool IsSupport(const string inFilename);

	virtual const RWindow*	getWindow() const
	{
		return (const RWindow*)this;
	}

	//タイトルの取得
	virtual string getTitle() const ;

private:

	CComPtr<ShockwaveFlashObjects::IShockwaveFlash>	ShockwaveFlash;
};

#endif // !defined(AFX_RFLASHPLAYER_H__850BD331_47F2_4CBE_BC2C_AD6E646B4157__INCLUDED_)
