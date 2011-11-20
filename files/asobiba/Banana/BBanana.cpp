// RBanana.cpp: RBanana �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "BBanana.h"
#include "WindowsMain.h"
#include "RGISJoin.h"
#include "RDiskUtil.h"
#include "RDialogUtil.h"
#include "resrc1.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

BBanana::BBanana()
{
	this->GisMaster= NULL;
	this->PenColor = PEN_TYPE_NONE;
}

BBanana::~BBanana()
{
	GPSOff();

	delete this->GisMaster;
}


void BBanana::Create() throw(RException)
{
	WindowsMain * winMain = WindowsMain::getMainSystem();

	RDialog::Create(winMain->getInstance() ,    NULL , 	"IDD_BANANA_DIALOG");

	ShareImage.Create();
	BBananaConfig::getInstance()->Load();

#ifndef VIEWONLY
	{
		BMakeDlg dlg;
		if ( dlg.DoModel(winMain->getInstance() ,    NULL , "IDD_MAKE") )
		{
			//��ʂ̍쐬
			InitScreen();

			//�쐬�����n�}�̃��[�h
			MapLoad( dlg.getMapName() , BBananaConfig::getInstance()->getUserDataDirectory() );
		}
		else
		{
			//��ʂ̍쐬
			InitScreen();
		}
	}
#else
	//��ʂ̍쐬
	InitScreen();
#endif

	FindEkiDlg.Create(this);
	FindAddressDlg.Create(this);
	GPSStateDlg.Create();
	UserDataEditDlg.Create(this , &ShareImage);
	RotateMode.Create(this);
	BananaConfigDlg.Create(this);
	GPSConfigDlg.Create(this);

	this->Show();

	Update();
}

//��ʂ̍쐬
void BBanana::InitScreen() throw(RException)
{
	WindowsMain * winMain = WindowsMain::getMainSystem();
	try
	{
		HWND window = winMain->getWindowHandle();

 		RECT rc;
		::GetClientRect(window , &rc);
		int width  = (rc.right  - rc.left) / 2 * 2;
		int height = (rc.bottom - rc.top) / 2 * 2 - 32;

		Dib.Release();
		Caption.Release();

		Dib.Create(width,height , 16);
		Caption.Create(width,32 , 16);

		try
		{
			MapLoad( BBananaConfig::getInstance()->getMapFilename() , BBananaConfig::getInstance()->getUserDataDirectory() );
		}
		catch(RException e)
		{
			this->MapDefualtLoad();

			RMessageBox::Message( e.getMessage() );
		}
	}
	catch(RException e)
	{
		//printf(e.getMessage() );
		throw;
	} 
}

//�_�C�A���O���E�B���h�E�̉��ɂ������܂��B
void BBanana::SetDialogPos(RDialog * ioDialog)
{
#ifndef _WIN32_WCE
	//�_�C�A���O���E�B���h�E�̉��ɂ������܂��B
	RECT rc;
	this->GetWindowRect(&rc);

	RECT tragetRC;
	ioDialog->GetClientRect(&tragetRC);

	ioDialog->MoveWindow( 0 , rc.bottom - tragetRC.bottom + ::GetSystemMetrics(SM_CYSIZEFRAME));
#endif
}

void BBanana::OnKeyDown(unsigned char inKey)
{
	switch(inKey)
	{
	case 'W':
	case VK_UP:
		GisMaster->MoveAt( 0  , -100);
		break;
	case 'X':
	case VK_DOWN:
		GisMaster->MoveAt( 0 ,  100);
		break;
	case 'D':
	case VK_RIGHT:
		GisMaster->MoveAt(100 , 0);
		break;
	case 'A':
	case VK_LEFT:
		GisMaster->MoveAt(-100 , 0);
		break;
	case 'Q':
		GisMaster->setScale( GisMaster->getScale() + 1 , GisMaster->getDeg() );
		break;
	case 'E':
		GisMaster->setScale( GisMaster->getScale() - 1 , GisMaster->getDeg() );
		break;
	case 'Z':
		GisMaster->setScale( GisMaster->getScale() , GisMaster->getDeg() + 1);
		break;
	case 'C':
		GisMaster->setScale( GisMaster->getScale() , GisMaster->getDeg() - 1);
		break;
	case 'R':
		SetDialogPos(&FindEkiDlg);
		FindEkiDlg.Show();
		break;
	case 'T':
		SetDialogPos(&FindAddressDlg);
		FindAddressDlg.Show();
		break;
	}
	Update();
}

void BBanana::OnDestory()
{
	GPSOff();

	//�ݒ�̕ۑ�.
	BBananaConfig::getInstance()->UpNowPoint( GisMaster->getPos() , GisMaster->getScale() , GisMaster->getDeg() );
	BBananaConfig::getInstance()->Save();
}

void BBanana::OnPaint()
{
    HDC hdc;
    PAINTSTRUCT paint;

	WindowsMain * winMain = WindowsMain::getMainSystem();
	HWND window = winMain->getWindowHandle();
    hdc = BeginPaint( window , &paint);
	::BitBlt(hdc  , 0 , 0 , Caption.getWidth() , Caption.getHeight() , Caption.getDC() , 0 , 0 , SRCCOPY);
	::BitBlt(hdc  , 0 , 32 , Dib.getWidth() , Dib.getHeight() , Dib.getDC() , 0 , 0 , SRCCOPY);
    EndPaint(window, &paint);
}

void BBanana::OnCommand(int inID)
{
	switch(inID)
	{
	case IDOK:
	case ID_MENU_END:
		::SendMessage(this->getWindowHandle() ,WM_CLOSE, 0 , 0 );
		break;
	case ID_MENU_EKI:
		SetDialogPos(&FindEkiDlg);
		FindEkiDlg.Show();
		break;
	case ID_MENU_ADDRESS:
		SetDialogPos(&FindAddressDlg);
		FindAddressDlg.Show();
		break;
	case ID_MENU_GPSSETTING:
		SetDialogPos(&GPSConfigDlg);
		GPSConfigDlg.Show();
		break;
	case ID_MENU_GPSSTATE:
		SetDialogPos(&GPSStateDlg);
		GPSStateDlg.Show();
		break;
	case ID_MENU_BIG:
		OnKeyDown('E');
		break;
	case ID_MENU_SMALL:
		OnKeyDown('Q');
		break;
	case ID_MENU_USERDATA_APPEND:
		SetDialogPos(&UserDataEditDlg);
		UserDataEditDlg.Show( GisMaster->getPos() );
		break;
	case ID_MENU_CONFIG:
		SetDialogPos(&BananaConfigDlg);
		BananaConfigDlg.Show();
		break;
	case ID_MENU_PEN1:
		this->PenColor = PEN_TYPE_1;
		break;
	case ID_MENU_PEN2:
		this->PenColor = PEN_TYPE_2;
		break;
	case ID_MENU_PEN3:
		this->PenColor = PEN_TYPE_3;
		break;
	case ID_MENU_PEN4:
		this->PenColor = PEN_TYPE_4;
		break;
	case ID_MENU_PEN5:
		this->PenColor = PEN_TYPE_5;
		break;
	case ID_MENU_PEN6:
		this->PenColor = PEN_TYPE_6;
		break;
	case ID_MENU_ERASE:
		this->PenColor = PEN_TYPE_ERASE;
		break;
	case ID_MENU_PENEND:
		this->PenColor = PEN_TYPE_NONE;
		break;
	case ID_MENU_PEN_LOAD:
		{
			string name = RDialogUtil::FileOpenDialog( this->getWindowHandle() , 
				"�g�p����v������I�����Ă�������" , "" , "" , _T("plan�t�@�C�� (*.plan)\0*.plan\0\0") );
			if (!name.empty())
			{
				PenFile.Load(name);
				Update();
			}
		}
		break;
	case ID_MENU_PEN_SAVE:
		{
			string name = RDialogUtil::FileSaveDialog( this->getWindowHandle() , 
				"�������ރv������I�����Ă�������" , "" , "" , _T("plan�t�@�C�� (*.plan)\0*.plan\0\0") );
			if (!name.empty())
			{
				PenFile.Save(name);
			}
		}
		break;
	}
}


void BBanana::OnGPSNotify()
{
	//GPS���g�p���܂���?
	if ( GPS.IsAlive() )
	{
		//���݂̒l�������Ă�������.
		LPOINT xy;
		int speed;
		int direction;
		SatellitesState satellitesState[SATELLITES_MAX];

		GPSStateDlg.OnTailLog( GPS.getTailLog() );

		if ( this->GPS.getParam(&xy ,&speed , &direction , satellitesState) )
		{
			GisMaster->LookAt(xy);

			GPSStateDlg.OnGPSNotify(&xy ,speed , direction , satellitesState);

			if (this->GPSTrackLog)
			{
				TrackLog.Line( &xy , 1 , PEN_TYPE_TRACKLOG);
			}

			Update();
		}
	}
}

void BBanana::OnLBottonMove(int inX,int inY)
{
	if (this->PenColor != PEN_TYPE_NONE)
	{
		//��΍��W�ɕϊ����܂�.
		POINT pt = {inX,inY};
		LPOINT lpt = GisMaster->getEffect()->RevAttach(&pt);

		PenFile.Line(&lpt , (1 << GisMaster->getScale() ) / 4  , this->PenColor );

		return;
	}

	if ( BBananaConfig::getInstance()->getRotateFlg() )
	{
		if ( RotateMode.isAlive() )
		{
			RAngle ang = RotateMode.OnMouseMove(inX, inY);

			if ( GisMaster->getDeg() != ang )
			{
				TRACE("%d\n" , (int)ang);
				GisMaster->setScale( GisMaster->getScale() , ang);
				GisMaster->Draw();
				WindowsMain::Update();
			}
		}
	}
}

void BBanana::OnLBottonUp(int inX,int inY)
{
	if (this->PenColor != PEN_TYPE_NONE)
	{
		PenFile.LastClear();
		Update();
		return;
	}

	RotateMode.OnMouseUp();

	if (ClickMode.OnMouseUp(inX , inY))
	{
			//��΍��W�ɕϊ����܂�.
			POINT pt = {inX,inY};
			LPOINT lpt = GisMaster->getEffect()->RevAttach(&pt);

			//��ʂ̒��S�Ɏ����Ă��܂�.
			GisMaster->LookAt(lpt);
			GisMaster->Draw();
			WindowsMain::Update();

			//���[�U��`�̃f�[���Ȃ�������.
			LRECT rc;
			pt.x = inX - 16;
			pt.y = inY - 16;
			lpt =  GisMaster->getEffect()->RevAttach(&pt);
			rc.left = lpt.x;
			rc.top  = lpt.y;

			pt.x = inX + 16;
			pt.y = inY + 16;
			lpt =  GisMaster->getEffect()->RevAttach(&pt);
			rc.right  = lpt.x;
			rc.bottom = lpt.y;

			try
			{
				RGISSmallUserDataRead userData;
				if ( GisMaster->UserDataFind(&rc , &userData ) )
				{
					SetDialogPos(&UserDataEditDlg);
					UserDataEditDlg.Show( &userData );
				}
			}
			catch(RException e)
			{
			}
	}
}

//�X�N���[�����^�b�v���ꂽ�ꍇ
void BBanana::OnLBottonDown(int inX,int inY)
{

	if ( !RotateMode.isAlive() )
	{
		RotateMode.OnMouseDown(inX , inY , GisMaster->getDeg() );
	}

	ClickMode.OnMouseDown(inX , inY);
	return ;

}

//�E�B���h�E�v���V�[�W��.
LRESULT BBanana::DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam)
{
	switch(inMsg)
	{
	case WM_INITDIALOG:
#ifdef _WIN32_WCE
		{
			WindowsMain* winMain = WindowsMain::getMainSystem();


			SHMENUBARINFO	cbi = { 0 };
			cbi.cbSize		= sizeof( SHMENUBARINFO );
			cbi.hwndParent	= ioHwnd;	// �_�C�A���O�̃E�C���h�E�n���h��
			cbi.nToolBarId	= IDR_MENU1;	// ���j���[���\�[�X�� ID
			cbi.hInstRes	= winMain->getInstance();
			cbi.dwFlags		= SHCMBF_HMENU;
			BOOL ret;
			if ( ret = SHCreateMenuBar( &cbi ) )
			{
				RpCommandBar = cbi.hwndMB;
			}
			else
			{
				RpCommandBar = NULL;
			}
			SHINITDLGINFO shidi = {0};
			shidi.dwMask = SHIDIM_FLAGS;
			shidi.dwFlags = SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
			shidi.hDlg = ioHwnd;
			SHInitDialog(&shidi);
            memset (&Sai, 0, sizeof (Sai));
            Sai.cbSize = sizeof (Sai);
		}
#endif
		return TRUE;
#ifdef _WIN32_WCE
	case WM_ACTIVATE:
	// Notify shell of our activate message
		SHHandleWMActivate(ioHwnd, inWParam, inLParam, &Sai, FALSE);
		break;
	case WM_SETTINGCHANGE:
		SHHandleWMSettingChange(ioHwnd, inWParam, inLParam, &Sai);
		break;
#endif
	case WM_LBUTTONDOWN:
		OnLBottonDown( LOWORD( inLParam ) , 	HIWORD( inLParam ) - 32 );
		break;
	case WM_LBUTTONUP:
		OnLBottonUp( LOWORD( inLParam ) , 	HIWORD( inLParam ) - 32 );
		break;
	case WM_MOUSEMOVE:
		if (inWParam & MK_LBUTTON)
			OnLBottonMove(LOWORD( inLParam ) , 	HIWORD( inLParam ) - 32 );
		break;
#ifndef _WIN32_WCE
	case 0x020A:	//WM_MOUSEWHEEL
		if ( (short)HIWORD(inWParam) < 0)
		{
			//�g��
			OnKeyDown('E');
		}
		else
		{
			//�k��
			OnKeyDown('Q');
		}
		break;
	case WM_SIZE:
		if (! (inWParam & SIZE_MINIMIZED) )
		{
			InitScreen();
			Update();
		}
		break;
#endif
	case WM_PAINT:
		OnPaint();
		break;
	case WM_KEYDOWN:
		OnKeyDown( (unsigned char)inWParam );
		break;
	case WM_TIMER:
		OnGPSNotify();
		break;
//	case WM_CANCELMODE:
	case WM_CLOSE:
		OnDestory();
		DestroyWindow(ioHwnd);
		return 0;
    case WM_DESTROY:
#ifdef _WIN32_WCE
		//���j���o�[���폜
		if (RpCommandBar)		CommandBar_Destroy(RpCommandBar);
#endif
		PostQuitMessage(0);
        return 0;
	case WM_COMMAND:
		OnCommand( LOWORD(inWParam) );
		break;
	}
	return FALSE;
}

//���[�U�[�f�[�^�̕ҏW.
void BBanana::UserDataModify(const LPOINT * inPoint ,
						const int * inDeleteNumber , int inDeleteCount ,
						const char * inData , int inSize,int inCount ) throw(RException)
{
	GisMaster->UserDataModify(inPoint ,inDeleteNumber , inDeleteCount, inData ,inSize ,  inCount);
	Update();
}

void BBanana::MapLoad(const string & mapName , const string & mapUserDirName) throw(RException)
{
	RAngle deg;
	int scale;
	LPOINT xy;

	if (this->GisMaster)
	{
		deg = this->GisMaster->getDeg();
		scale = this->GisMaster->getScale();
		xy = this->GisMaster->getPos();
	}
	else
	{
		deg = BBananaConfig::getInstance()->getDeg();
		scale = BBananaConfig::getInstance()->getScale();
		xy = BBananaConfig::getInstance()->getPoint();
	}
	delete this->GisMaster;
	this->GisMaster = new RGISSmallMaster;

	this->GisMaster->Create(mapName , mapUserDirName, &Dib , &Caption , &this->ShareImage , &PenFile);
	this->GisMaster->LookAt( xy );
	this->GisMaster->setScale( scale , deg );
	
	BBananaConfig::getInstance()->UpMapDirectory(mapName , mapUserDirName);

}

//���̏ꏊ����ʂ̒��S�Ɏ����Ă��܂��B
void BBanana::LookAt(const LPOINT & inPoint)
{
	GisMaster->LookAt(inPoint);
	Update();
}

//�f�B�t�H���g�̃}�b�v��ǂݍ���.
void BBanana::MapDefualtLoad()
{
	this->MapLoad(BConf::getConf()->getCurrentDirectory() + "dummy.map" , 
		BConf::getConf()->getCurrentDirectory() + "userdata\\");
}

void BBanana::GPSOn(int port ,int speed ,int type ,int checkTime ,int logType ,const string & logDir , int trackLog)
{
	if (this->GPS.IsAlive())
	{
		return ;
	}

	try
	{
		this->GPS.Create( port , speed , type ,  logType , logDir );
		this->GPS.Start();

		this->GPSTrackLog = (trackLog == 1);

		GPSTimer.Set( this->getWindowHandle() , 1 ,checkTime * 1000 );
	}
	catch(RException e)
	{
		RMessageBox::Message(e.getMessage() , "GPS�������Ɏ��s���܂���");
	}
}

void BBanana::GPSOff()
{
	if (! this->GPS.IsAlive())
	{
		return ;
	}

	try
	{
		this->GPS.HoldUp();
		this->GPS.Join();

		GPSTimer.Reset();
	}
	catch(RException e)
	{
		RMessageBox::Message(e.getMessage() , "GPS�̏I���Ɏ��s���܂���");
	}

}

bool BBanana::IsGPSAlive() const
{
	return  this->GPS.IsAlive();
}


bool BBanana::IsDialogMessage(MSG * inMessage)
{
	return 
		(FindEkiDlg.getWindowHandle() != 0 &&		::IsDialogMessage( FindEkiDlg.getWindowHandle() , inMessage) ) || 
		(FindAddressDlg.getWindowHandle() != 0 &&	::IsDialogMessage( FindAddressDlg.getWindowHandle() , inMessage) ) || 
		(GPSConfigDlg.getWindowHandle() != 0 &&		::IsDialogMessage( GPSConfigDlg.getWindowHandle() , inMessage) ) || 
		(GPSStateDlg.getWindowHandle() != 0 &&		::IsDialogMessage( GPSStateDlg.getWindowHandle() , inMessage) ) || 
		(BananaConfigDlg.getWindowHandle() != 0 &&	::IsDialogMessage( BananaConfigDlg.getWindowHandle() , inMessage) ) || 
		(UserDataEditDlg.getWindowHandle() != 0 &&	::IsDialogMessage( UserDataEditDlg.getWindowHandle() , inMessage) ) ;

}

void BBanana::Update()
{
	GisMaster->Draw();
	WindowsMain::Update();
}
