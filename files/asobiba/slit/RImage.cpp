// RImage.cpp: RImage �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RImage.h"
#include "RMessageBox.h"
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////



#ifdef _WIN32_WCE

			RImage::RImage()
			{
				MemoryDC = NULL;
				HBitmap = NULL;

				Imgdecmp.Create();

			}

			RImage::~RImage()
			{
				Release();
			}

			void RImage::Release()
			{
				if (MemoryDC) DeleteObject(MemoryDC);
				if (HBitmap) DeleteObject(HBitmap);

				MemoryDC = NULL;
				HBitmap = NULL;
			}

			void RImage::ChangeScale(int inWidth, int inHegiht)
			{
				HDC hdc = GetDC(NULL);
				HDC memoryDC;
				HBITMAP hBitmap;
				try
				{
					memoryDC = CreateCompatibleDC( hdc );
					if( ! memoryDC  )
					{
						throw RWin32Exception(EXCEPTIONTRACE + "CreateCompatibleDC�Ŏ��s");
					}

					hBitmap = CreateCompatibleBitmap(hdc , inWidth , inHegiht );
					if( ! hBitmap )
					{
						throw RWin32Exception(EXCEPTIONTRACE + "CreateCompatibleBitmap �Ŏ��s");
					}

					HGDIOBJ hobj = SelectObject(memoryDC, hBitmap );
					if(NULL == hobj) 
					{
						throw RWin32Exception(EXCEPTIONTRACE + "SelectObject�Ŏ��s");
					}

					::StretchBlt(memoryDC , 0 , 0, inWidth , inHegiht , this->MemoryDC , 0 , 0 , this->Width , this->Height , SRCCOPY);
					
					//���݂̉摜�f�[�^��j��
					Release();

					//�����قǍ�����f�[�^�Ɠ���ւ�
					this->MemoryDC = memoryDC;
					this->HBitmap = hBitmap;
					this->Width = inWidth;
					this->Height = inHegiht;

					::ReleaseDC(NULL,hdc);
				}
				catch(RException )
				{
					if (memoryDC) DeleteObject(memoryDC);
					if (hBitmap) DeleteObject(hBitmap);

					::ReleaseDC(NULL,hdc);
					throw ;
				}
			}



			void RImage::Load(const char * inBuffer , int inSize) throw(RException)
			{
				Release();

				const int	bufferMax = 65535;
				BYTE		* buffer = new BYTE[bufferMax];

				HDC hdc = GetDC(NULL);

				try
				{
						RImage::BufferControl control;
						control.buffer = inBuffer;
						control.size = inSize;
						control.readOf = 0;

						DecompressImageInfo     dii = {0};
						dii.dwSize = sizeof( DecompressImageInfo );
						dii.pbBuffer = buffer;
						dii.dwBufferMax = bufferMax;
						dii.dwBufferCurrent = 0;
						dii.phBM = &this->HBitmap;
						dii.ppImageRender = NULL;
						dii.iBitDepth = GetDeviceCaps(hdc,BITSPIXEL);
						dii.lParam = ( LPARAM ) &control;
						dii.hdc = hdc;
						dii.iScale = 100;
						dii.iMaxWidth = 10000;
						dii.iMaxHeight = 10000;
						dii.pfnGetData = GetImageData;
						dii.pfnImageProgress = ImageProgress;
						dii.crTransparentOverride = ( UINT ) -1;

						HRESULT hr = Imgdecmp.DecompressImageIndirect(&dii);

						if ( ! SUCCEEDED(hr) )
						{
							throw RWin32Exception(EXCEPTIONTRACE + "DecompressImageIndirect �Ɏ��s");
						}

						this->MemoryDC = CreateCompatibleDC( hdc );
						if( ! this->MemoryDC )
						{
							throw RWin32Exception(EXCEPTIONTRACE + "CreateCompatibleDC�Ŏ��s");
						}

						HGDIOBJ hobj = SelectObject(this->MemoryDC, this->HBitmap);
						if(NULL == hobj) 
						{
							throw RWin32Exception(EXCEPTIONTRACE + "SelectObject�Ŏ��s");
						}

						//�T�C�Y�̎擾.
						BITMAP bmp;
						GetObject( this->HBitmap, sizeof(BITMAP), &bmp);

						this->Width = bmp.bmWidth;
						this->Height = bmp.bmHeight;

						ReleaseDC(NULL , hdc);

						delete [] buffer;
				}
				catch(RException & e)
				{
					RMessageBox::Message(e.getMessage() );
					ReleaseDC( NULL , hdc);
					delete [] buffer;

					throw;
				}


			}

			DWORD CALLBACK RImage::GetImageData( LPSTR szBuffer, DWORD dwBufferMax, LPARAM lParam ) 
			{
				RImage::BufferControl * control = (RImage::BufferControl *) lParam;

				int nokori = (control->size - control->readOf);
				int readSize = (dwBufferMax > nokori) ? (nokori) : (dwBufferMax);

//				RMessageBox::Message(	num2str( control->size ) + " " + num2str(control->readOf) + " " + num2str(readSize) );

				memcpy(szBuffer , control->buffer + control->readOf,  readSize );
				control->readOf += readSize;

				return readSize;
			}
			void CALLBACK RImage::ImageProgress(    IImageRender* , BOOL ,  LPARAM  ) 
			{
				//nop
			}
#else
			#include "RComInit.h"

			RImage::RImage()
			{
				MemoryDC = NULL;
				HBitmap = NULL;
			}

			RImage::~RImage()
			{
				Release();
			}

			void RImage::Release()
			{
				if (MemoryDC) DeleteObject(MemoryDC);
				if (HBitmap) DeleteObject(HBitmap);

				MemoryDC = NULL;
				HBitmap = NULL;
			}

			void RImage::Load(const char * inBuffer , int inSize) throw(RException)
			{
				Release();

				IPicture*		olePicture = NULL;
				BYTE *			globalMemory = NULL;
				try
				{
						HRESULT hr;
						//COM �����������Ă��Ȃ���Ώ���������.
						RComInit::UseCom();

						globalMemory = (BYTE*) ::GlobalAlloc(GPTR, inSize );
						memcpy(globalMemory , (void*) inBuffer  , inSize ) ;

						IStream *iStream;
						if(!SUCCEEDED(hr = ::CreateStreamOnHGlobal(globalMemory  ,FALSE,&iStream))) 
						{
							throw RNotSupportException(EXCEPTIONTRACE + "CreateStreamOnHGlobal �Ɏ��s���܂���");
						}
						if(!SUCCEEDED(hr = ::OleLoadPicture(iStream, inSize, FALSE, IID_IPicture, (LPVOID*)&olePicture)))
						{
							iStream->Release();
							throw RNotSupportException(EXCEPTIONTRACE + "OleLoadPicture �Ɏ��s���܂���");
						}
						iStream->Release();

						LONG orignalWidth,orignalHeight;
						olePicture->get_Width(&orignalWidth);
						olePicture->get_Height(&orignalHeight);

						HDC hdc = ::GetDC(NULL);
						this->Width  = ::MulDiv(orignalWidth,::GetDeviceCaps(hdc,LOGPIXELSX),2540);
						this->Height = ::MulDiv(orignalHeight,::GetDeviceCaps(hdc,LOGPIXELSY),2540);

						try
						{
							this->MemoryDC = CreateCompatibleDC( hdc );
							if( ! this->MemoryDC )
							{
								throw RWin32Exception(EXCEPTIONTRACE + "CreateCompatibleDC�Ŏ��s");
							}

							this->HBitmap = CreateCompatibleBitmap(hdc , this->Width , this->Height);
							if( ! this->HBitmap )
							{
								throw RWin32Exception(EXCEPTIONTRACE + "CreateCompatibleBitmap �Ŏ��s");
							}

							HGDIOBJ hobj = SelectObject(this->MemoryDC, this->HBitmap);
							if(NULL == hobj) 
							{
								throw RWin32Exception(EXCEPTIONTRACE + "SelectObject�Ŏ��s");
							}

							if ( olePicture->Render(this->MemoryDC, 0, 0, this->Width,this->Height , 0, orignalHeight, orignalWidth, -orignalHeight, NULL) != S_OK)
							{
								throw RNotSupportException(EXCEPTIONTRACE + "�f�o�C�X�R���e�L�X�g�ɏ������߂܂���ł���");
							}
							::ReleaseDC(NULL,hdc);
						}
						catch(RException )
						{
							::ReleaseDC(NULL,hdc);
							throw ;
						}

						olePicture->Release();
						::GlobalFree(globalMemory);
				}
				catch(RException )
				{
					if (olePicture)
					{
						olePicture->Release();
					}
					::GlobalFree(globalMemory);
					throw ;
				}
			}

			void RImage::ChangeScale(int inWidth, int inHegiht)
			{
				HDC hdc = GetDC(NULL);
				HDC memoryDC;
				HBITMAP hBitmap;
				try
				{
					memoryDC = CreateCompatibleDC( hdc );
					if( ! memoryDC  )
					{
						throw RWin32Exception(EXCEPTIONTRACE + "CreateCompatibleDC�Ŏ��s");
					}

					hBitmap = CreateCompatibleBitmap(hdc , inWidth , inHegiht );
					if( ! hBitmap )
					{
						throw RWin32Exception(EXCEPTIONTRACE + "CreateCompatibleBitmap �Ŏ��s");
					}

					HGDIOBJ hobj = SelectObject(memoryDC, hBitmap );
					if(NULL == hobj) 
					{
						throw RWin32Exception(EXCEPTIONTRACE + "SelectObject�Ŏ��s");
					}

					::StretchBlt(memoryDC , 0 , 0, inWidth , inHegiht , this->MemoryDC , 0 , 0 , this->Width , this->Height , SRCCOPY);
					
					//���݂̉摜�f�[�^��j��
					Release();

					//�����قǍ�����f�[�^�Ɠ���ւ�
					this->MemoryDC = memoryDC;
					this->HBitmap = hBitmap;
					this->Width = inWidth;
					this->Height = inHegiht;

					::ReleaseDC(NULL,hdc);
				}
				catch(RException )
				{
					if (memoryDC) DeleteObject(memoryDC);
					if (hBitmap) DeleteObject(hBitmap);

					::ReleaseDC(NULL,hdc);
					throw ;
				}
			}


#endif