//PTU2F3 �X�C�b�`�𒼐ڐ��䂷��R�[�h�ł�


#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <setupapi.h>
extern "C"
{
	//ddk�����Ă��������B
	#include <ddk/hidsdi.h>
}

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "hid.lib")

//iVendorID iProductID �ŗ��p����USB�f�o�C�X����肵�A�n���h�����J���܂��B
HANDLE __stdcall HIDOpen(int iVendorID, int iProductID)
{
	GUID hidGuid;
	HANDLE hDevHandle = INVALID_HANDLE_VALUE;
	HDEVINFO hDevInfo;
	HIDD_ATTRIBUTES Attributes;
	PSP_DEVICE_INTERFACE_DETAIL_DATA pspDidd;
	SP_DEVICE_INTERFACE_DATA spDid;

	HidD_GetHidGuid(&hidGuid);
	hDevInfo = SetupDiGetClassDevs(&hidGuid, NULL, NULL, DIGCF_PRESENT|DIGCF_DEVICEINTERFACE|DIGCF_ALLCLASSES);
	{
		USHORT i;
		for(i = 0; i < 128;i++){
			memset(&spDid, 0, sizeof(SP_DEVICE_INTERFACE_DATA)); // spDid Clear;
			spDid.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
			if(!SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &hidGuid, i, &spDid)) continue;
			DWORD dwRequiredLength = 0;
			SetupDiGetDeviceInterfaceDetail(hDevInfo, &spDid, NULL, 0, &dwRequiredLength, NULL);
			pspDidd = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(dwRequiredLength);
			pspDidd->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			if(SetupDiGetDeviceInterfaceDetail(hDevInfo, &spDid, pspDidd, dwRequiredLength, &dwRequiredLength, NULL)){
				hDevHandle = CreateFile(pspDidd->DevicePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
				if(hDevHandle != INVALID_HANDLE_VALUE){
					Attributes.Size = sizeof(Attributes);
					if(HidD_GetAttributes(hDevHandle, &Attributes)){
						if(iVendorID == Attributes.VendorID && iProductID == Attributes.ProductID){
							free(pspDidd);
							break;
						}
					}
					CloseHandle(hDevHandle);
				}
			}
			free(pspDidd);
		}
	}
	return hDevHandle;
}

//�X�e�[�^�X��ǂ�
bool ReadStatus(HANDLE handle ,BYTE status[9] )
{
	//���b�Z�[�W�𑗂�(�Ȃ����Ȃ��Ă�����w )
	BYTE featureBuffer[3] = { 0x00, 0xff , 0xff };
	HidD_SetFeature( handle , featureBuffer , 3 );

	//�o���N�]���œǂނ炵��

	//�X�e�[�^�X����!!!
	DWORD updatesize=0;
	BYTE readstatus[9] = {0,1,0,0,0,0,0,0,0};	//�X�e�[�^�X���擾����R�[�h
	BOOL ret = WriteFile( handle , readstatus , 9 , &updatesize , NULL ); 
	if (!ret)
	{
		printf("can not write!");
		return false;
	}

	memset(status,0,sizeof(BYTE) * 9 );
	ret = ReadFile( handle , status , 9 , &updatesize , NULL ); 
	if (!ret)
	{
		printf("can not read!");
		return false;
	}
	return true;
}

//�X�e�[�^�X��ǂ�
bool UpdateSwitch(HANDLE handle ,int switchid,bool onoff )
{
	//�X�e�[�^�X��ǂ�
	BYTE status[9];
	if ( ! ReadStatus(handle,status) )
	{
		printf("�X�e�[�^�X�̎擾�Ɏ��s���܂���");
		return false;
	}
	BYTE code;
	if (status[1] == 5) //alloff
	{
		code = 3;
	}
	else if (status[1] == 4) //4 only
	{
		code = 2;
	}
	else if (status[1] == 1) //5 only
	{
		code = 1;
	}
	else if (status[1] == 0) //allon
	{
		code = 0;
	}
	else
	{
		printf("���m�̃X�e�[�^�X %x ����f���܂���" , status[1]);
		return false;
	}

	if (switchid == 4)
	{
		if (onoff) 
		{
			code = code & ~0x01;
		}
		else
		{
			code = code | 0x01;
		}
	}
	else if (switchid == 5)
	{
		if (onoff) 
		{
			code = code & ~0x02;
		}
		else
		{
			code = code | 0x02;
		}
	}
	else
	{
		printf("���m�̃{�^�� %d ���w�肳��܂���" , switchid);
		return false;
	}


	//���b�Z�[�W�𑗂�(�Ȃ����Ȃ��Ă�����w )
	BYTE featureBuffer[3] = { 0x00, 0xff , 0xff };
	HidD_SetFeature( handle , featureBuffer , 3 );


	//�X�e�[�^�X����ꍞ�݂܂��B
	//3�o�C�g�ڂɏ����X�C�b�`��1�ɂ��ăt���O�����Ă܂�
	DWORD updatesize=0;
	BYTE readstatus[9] = {0,2,0,0,0,0,0,0,0};	
//	BYTE setallon[9]   = {0,2,0,0,0,0,0,0,0};
//	BYTE setalloff[9]  = {0,2,3,0,0,0,0,0,0};
	readstatus[2] = code;

	BOOL ret = WriteFile( handle , readstatus , 9 , &updatesize , NULL ); 
	if (!ret)
	{
		printf("can not write!");
		return false;
	}

	return true;
}


int main()
{
	HANDLE handle = HIDOpen(0x711,0x28);
	if (handle == INVALID_HANDLE_VALUE)
	{
		CloseHandle(handle);
		printf("�f�o�C�X��������܂���ł���");
		return -1;
	}
	//todo �R�R��ς��Ă�
	if ( ! UpdateSwitch(handle, 4 , true) )
	{
		CloseHandle(handle);
		printf("�X�e�[�^�X�X�V�Ɏ��s���܂���");
		return -2;
	}

	CloseHandle(handle);
	printf("���튮��");
	return 0;
}

