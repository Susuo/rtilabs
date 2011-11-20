// BMakeDlg.cpp: BMakeDlg �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BMakeDlg.h"
#include "WindowsMain.h"
#include "RDialogUtil.h"
#include "resrc1.h"
#include "RDiskUtil.h"
#include "RFileUtil.h"
#include "RConv.h"
#include "RGISGyoseiD.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


BMakeDlg::BMakeDlg()
{

}

BMakeDlg::~BMakeDlg()
{

}


void BMakeDlg::PreCreate() throw(RWin32Exception)
{
	Example.MountWindow( this->GetDlgItem(IDC_EXAMPLE_EDIT) );
	AreaSrc.MountWindow( this->GetDlgItem(IDC_AREASRC_EDIT) );
	AreaList.MountWindow( this->GetDlgItem(IDC_AREALIST) );
	Temp.MountWindow( this->GetDlgItem(IDC_TEMP_EDIT) );
	Map.MountWindow( this->GetDlgItem(IDC_MAP_EDIT) );

	AreaList.InsertColumn(0 , 0 , "�R�[�h" , 50, LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM , LVCFMT_LEFT);
	AreaList.InsertColumn(0 , 0 , "����" , 220, LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM , LVCFMT_LEFT);


	Example.setText("map                  �����̃f�B���N�g���� (2) �ɃZ�b�g\r\n"
					" |-25000             1/25000�̒n�}\r\n"
					"     |---01101\r\n"
					"           |---01101.slm\r\n"
					"           |---01101.slp\r\n"
					"           .............\r\n"
					"     |---01102\r\n"
					"     |---01103\r\n"
					"     .............\r\n"
					" |-2500              1/2500�̒n�}\r\n"
					"     |---01101\r\n"
					"           |---12NB592\r\n"
					"           |---12NB594\r\n"
					"           .............\r\n"
					"     |---01102\r\n"
					"     |---01103\r\n"
					"     .............\r\n"
					" |-address           �X�惌�x���ʒu�Q�Ə��\r\n"
					"     |---01101JGD.csv\r\n"
					"     |---01102JGD.csv\r\n"
					"     |---01103JGD.csv\r\n"
					"     .............\r\n"
					);
}

void BMakeDlg::OnCancel()
{
	this->EndDialog(0);
}

const string BMakeDlg::getMapName() const
{
	return MapFilename;
}



void BMakeDlg::Scan() throw(RException)
{
	RStringList			numberList;

	//�f�B���N�g���X�L����
	numberList.clear();
	AreaList.DeleteItemAll();
	{
		int listItemIndex = 0;
		WIN32_FIND_DATA data;
		const string path = AreaSrc.getText() + "\\25000";
		const string findpath = path + "\\*.*";
		HANDLE handle = ::FindFirstFile(findpath.c_str() , &data );
		if (handle == INVALID_HANDLE_VALUE )
		{
			return ;
		}
		do
		{
			if ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY  && 
				!(data.cFileName[0] == '.' && (data.cFileName[1] == '\0' || (data.cFileName[1] == '.' && data.cFileName[2] == '\0') ) ) )
			{
					string read = path + "\\" + data.cFileName + "\\" + data.cFileName + ".slm";
					if ( RDiskUtil::Exist(read) )
					{
						//�s���̖��O�̓ǂݍ���.
						read = path + "\\" + data.cFileName + "\\" + data.cFileName + "GD.sal";
						try
						{
							const string name = ParseGyouseiName(read , atoi(data.cFileName) );
							AreaList.SetItem(listItemIndex  , 0 , name.c_str() );	//���O
							AreaList.SetItem(listItemIndex  , 1 , data.cFileName );	//����

							printf("���X�g�ɒǉ� %d �Ԗ� : �R�[�h:%d:%s\r\n" ,listItemIndex, atoi(data.cFileName) ,  name.c_str() );

							listItemIndex++;
						}
						catch(RException & )
						{
							::FindClose(handle);
							throw ;
						}
					}
			}
		}
		while( ::FindNextFile(handle , &data) );
		::FindClose(handle);
	}
}


//�s�������擾���܂�.
string BMakeDlg::ParseGyouseiName(const string & inGyoseiDFilename,unsigned long inCode) const throw(RException) 
{
	RGISGyoseiD gyoseiD;
	gyoseiD.Create(inGyoseiDFilename , inCode);
	int gyoseiCount = gyoseiD.getCount();
	if (gyoseiCount <= 0)
	{
		throw RException("�s���f�[�^:%s �Ɉ�s���L���ȍs������܂���" , inGyoseiDFilename.c_str() );
	}
	for(int i = 0 ; i < gyoseiCount ; i ++)
	{
		const GyoseiDData* gyoseiDData = gyoseiD.getPoint(i);
		if (! gyoseiDData->Name.empty() )
		{
			return gyoseiDData->Name;
		}
	}
	throw RException("�s���f�[�^:%s �ɗL���ȍs�͂���܂������A�݂�Ȗ��O����ł�", inGyoseiDFilename.c_str());
}

void BMakeDlg::Make() 
{
	RStringList numberList;

	//���ݑI������Ă���Ƃ�������ׂă��X�g�ɓ����.
	int index = -1;
	while((index = ListView_GetNextItem(AreaList.getWindowHandle(), index, LVNI_ALL | LVNI_SELECTED))!=-1)
	{
		if ( AreaList.GetItemState(index, LVIS_SELECTED) == LVIS_SELECTED )
		{
			numberList.push_back( AreaList.GetItem(index , 1) );
		}
	}
	if (numberList.empty() )
	{
		RMessageBox::Message("(3)�̃G���A������I������Ă��܂���");
		return ;
	}
	if (AreaSrc.getText().empty() )
	{
		RMessageBox::Message("(2)�̃f�B���N�g�����w�肵�Ă�������");
		return ;
	}
	if (Temp.getText().empty() )
	{
		RMessageBox::Message("(4)�̃f�B���N�g�����w�肵�Ă�������");
		return ;
	}
	if (Temp.getText().empty() )
	{
		RMessageBox::Message("(5)�̃t�@�C�������w�肵�Ă�������");
		return ;
	}

	try
	{
		//�ϊ��J�n
		SmallPack(AreaSrc.getText() , Temp.getText() + "\\temp", Map.getText() , numberList );

		this->MapFilename = Map.getText();
	}
	catch(RException e)
	{
		RMessageBox::Message(string("") + "�G���[���������܂���:\r\n" + e.getMessage() , "�G���[");
		this->EndDialog(0);
	}
	catch(std::bad_alloc)
	{
		RMessageBox::Message(string("") + "BadAlloc���������܂���" , "�G���[");
		this->EndDialog(0);
	}
	catch(...)
	{
		RMessageBox::Message(string("") + "���m�̃G���[���������܂���" , "�G���[");
		this->EndDialog(0);
	}

	//����
	this->EndDialog(1);
}

void BMakeDlg::SmallPack(const string inForm , const string & inTo,
						 const string & inKmap , const RStringList & inTargetList) throw(RException)
{
	this->Hide();

	puts("SmallPack ................");
	RGISSlmINdex	SlmIndex;


	int inScale = 100;
	SlmIndex.Create(inForm + "\\25000", inTargetList , inScale);

	try
	{
		RDiskUtil::Delete(inTo);
	}
	catch(RException e)
	{
	}
	RDiskUtil::MakeDirectory(inTo + "\\");

	puts("�������擾���Ă��܂�");

	int count = SlmIndex.getCount();

	for(int i = 0 ; i < count ; i++)
	{
		const RGISSlm* slm = SlmIndex.getPoint(i);
		int key = slm->getKey();

		printf("�i�� %d/%d Area(%d)\n" , i,count , key);

		RGIS * gis = new RGIS;
		try
		{
			gis->SmallPack(inForm ,inTo , key , inScale);

			TRACE("Convert!:%u\n",gis->getKey() );
		}
		catch(RException e)
		{
			TRACE(e.getMessage() );
			delete gis;

			throw RException (EXCEPTIONTRACE + "�n�}�ϊ����̃G���A("  + num2str(gis->getKey()) + ")�ŃG���[���������܂���:"  + e.getMessage() );
		}
		delete gis;
	}

	puts("��������");

	//��������.
	JoinTempFormatScript script[SEGMENT_MAX];
	int segmentCount = RGIS::getJoinScript(&script[0] , SEGMENT_MAX);

	JoinHeadFormatScript head[SEGMENT_MAX];
	int segmentHeadCount = RGIS::getJoinHeadScript(&head[0] , SEGMENT_MAX);

	//��������.
	RGISJoin Join;
	Join.Join( inTo , inTo + "/kuma.map" , script , segmentCount , head , segmentHeadCount);

	try
	{
		puts("�ȈՃ`�F�b�N");
		//���Ă��Ȃ����`�F�b�N.
		Join.Check(inTo + "/kuma.map");
	}
	catch(RException e)
	{
		ASSERT(0);
		throw RException( EXCEPTIONTRACE + "�ȈՃ`�F�b�N���ɃG���[���������܂���:" + e.getMessage() );
	}

	//���ʕ��̃R�s�[
	puts("�쐬�����n�}������̗̈�ɃR�s�[���Ă��܂�");
	RDiskUtil::Move(inTo + "/kuma.map" , inKmap );

	//��Ɨ̈�̃N���A
	puts("��ƂɎg�����̈��j�����Ă��܂�");
	try
	{
		RDiskUtil::Delete(inTo);
	}
	catch(RException e)
	{
		ASSERT(0);
		throw RException( EXCEPTIONTRACE + "��ƂɎg�����̈��j�����ɃG���[���������܂���:" + e.getMessage() );
	}
}


//�E�B���h�E�v���V�[�W��.
LRESULT BMakeDlg::DlgProc(HWND ioHwnd, UINT inMsg, WPARAM inWParam, LPARAM inLParam)
{
	switch(inMsg)
	{
	case WM_INITDIALOG:
#ifdef _WIN32_WCE
		SHINITDLGINFO shidi;
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIPDOWN | SHIDIF_SIZEDLGFULLSCREEN;
		shidi.hDlg = ioHwnd;
		SHInitDialog(&shidi);
#endif
		PreCreate();
		return TRUE;
	case WM_COMMAND:
		{
			switch (LOWORD(inWParam)) 
			{
			case IDCANCEL:	//��ݾ�
				OnCancel();
				break;
			case IDC_AREASRC_BUTTON:	//�\�[�X�p�X
				{
					string name = RDialogUtil::SelectPath( this->getWindowHandle() , 
						"�\�z����f�[�^�������Ă�f�B���N�g�����w�肵�Ă�������" , AreaSrc.getText() );
					AreaSrc.setText(name);
				}
				break;
			case IDC_TEMP_BUTTON:
				{
					string name = RDialogUtil::SelectPath( this->getWindowHandle() , 
						"��Ɨp�̃f�B���N�g�����w�肵�Ă�������" , Temp.getText() );
					Temp.setText(name);
				}
				break;
			case IDC_MAP_BUTTON:
				{
					string name = RDialogUtil::FileSaveDialog( this->getWindowHandle() , 
						"�쐬���� .map �t�@�C������I�����Ă�������" , Map.getText() , "kuma.map" , _T("map�t�@�C�� (*.map)\0*.map\0\0") );
					Map.setText(name);
				}
				break;
			case IDC_AREASRC_SCAN:	//Scan�{�^�����������Ƃ�
				this->Hide();
				Scan();
				this->Show();
				break;
			case IDC_MAKE_BUTTON:	//Make�{�^�����������Ƃ�
				Make();
				break;
			}
		}
		break;
	}
	return FALSE;
}

