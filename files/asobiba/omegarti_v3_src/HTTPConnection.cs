/*
 * Copyright (c) Tasuku SUENAGA, Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.IO;
using System.Text;
using System.Runtime.InteropServices;

namespace Travis.Http {

	[StructLayout(LayoutKind.Sequential, CharSet=CharSet.Unicode,Pack=4)]
	struct WINHTTP_AUTOPROXY_OPTIONS {
		public uint dwFlags;
		public uint dwAutoDetectFlags;
		public IntPtr lpszAutoConfigUrl;
		public System.IntPtr lpvReserved;
		public uint dwReserved;
		public bool fAutoLogonIfChallenged;
	};
	[StructLayout(LayoutKind.Sequential,Pack=4)]
	struct WINHTTP_PROXY_INFO {
		public uint dwAccessType;
		public IntPtr lpszProxy;
		public IntPtr lpszProxyBypass;
	};

	[StructLayout(LayoutKind.Sequential,Pack=4)]
	struct WINHTTP_CURRENT_USER_IE_PROXY_CONFIG {
		public bool fAutoDetect;
		public IntPtr lpszAutoConfigUrl;
		public IntPtr lpszProxy;
		public IntPtr lpszProxyBypass;
	};

	[StructLayout(LayoutKind.Sequential,Pack=4)]
	struct URL_COMPONENTS {
		public int dwStructSize;
		public IntPtr lpszScheme;
		public int dwSchemeLength;
		public int nScheme;
		public IntPtr lpszHostName;
		public int dwHostNameLength;
		public ushort nPort;
		public IntPtr lpszUserName;
		public uint dwUserNameLength;
		public IntPtr lpszPassword;
		public uint dwPasswordLength;
		public IntPtr lpszUrlPath;
		public int dwUrlPathLength;
		public IntPtr lpszExtraInfo;
		public int dwExtraInfoLength;
	};

	public class HttpException : ApplicationException {
		public HttpException(string msg) : base(msg) {
		}
	}

	public class HTTPConnection : IDisposable {

		[DllImport("winhttp", CharSet=CharSet.Unicode, SetLastError=true)]
		private extern static System.IntPtr WinHttpOpen(string pwszUserAgent,uint dwAccessType,string pwszProxyName,string pwszProxyBypass,uint dwFlags);
		[DllImport("winhttp", CharSet=CharSet.Unicode, SetLastError=true)]
		private extern static bool WinHttpGetProxyForUrl(System.IntPtr hSession,string lpcwszUrl,ref WINHTTP_AUTOPROXY_OPTIONS pAutoProxyOptions,ref WINHTTP_PROXY_INFO pProxyInfo);
		[DllImport("winhttp")]
		private extern static bool WinHttpCloseHandle(System.IntPtr hInternet);
		[DllImport("winhttp")]
		private extern static bool WinHttpGetIEProxyConfigForCurrentUser(ref WINHTTP_CURRENT_USER_IE_PROXY_CONFIG pProxyConfig);
		[DllImport("winhttp", CharSet=CharSet.Unicode, SetLastError=true)]
		private extern static System.IntPtr WinHttpConnect(IntPtr hSession,string pswzServerName,ushort nServerPort,uint dwReserved);
		[DllImport("winhttp", CharSet=CharSet.Unicode, SetLastError=true)]
		private extern static bool WinHttpCrackUrl(string pwszUrl,int dwUrlLength,uint dwFlags,ref URL_COMPONENTS lpUrlComponents);
		[DllImport("winhttp", CharSet=CharSet.Unicode, SetLastError=true)]
		private extern static System.IntPtr WinHttpOpenRequest
			(IntPtr hConnect,string pwszVerb,IntPtr pwszObjectName,string pwszVersion,string pwszReferrer,IntPtr ppwszAcceptTypes,uint dwFlags);
		[DllImport("winhttp", SetLastError=true)]
		private extern static bool WinHttpSetOption(IntPtr hInternet,uint dwOption,ref WINHTTP_PROXY_INFO lpBuffer,int dwBufferLength);
		[DllImport("winhttp", CharSet=CharSet.Unicode, SetLastError=true)]
		private extern static bool WinHttpSendRequest(IntPtr hRequest,string pwszHeaders,uint dwHeadersLength,IntPtr lpOptional,uint dwOptionalLength,uint dwTotalLength,ref uint dwContext);
		[DllImport("winhttp", SetLastError=true)]
		private extern static bool WinHttpReceiveResponse(IntPtr hRequest,IntPtr lpReserved);
		[DllImport("winhttp", SetLastError=true)]
		private extern static bool WinHttpQueryDataAvailable(IntPtr hRequest,ref uint lpdwNumberOfBytesAvailable);
		[DllImport("winhttp", SetLastError=true)]
		unsafe private extern static bool WinHttpReadData(IntPtr hRequest,void* lpBuffer,uint dwNumberOfBytesToRead,ref uint lpdwNumberOfBytesRead);

		[DllImport("kernel32")]
		private extern static IntPtr GlobalFree(IntPtr hMem);
	
		//WinHTTP�����p�\���ǂ����𒲂ׂ�B���Ƃ���Win9x�ł͗��p�s�B
		private static bool _examined;
		private static bool _available;
		public static bool IsWinHTTPAvailable {
			get {
				if(!_examined) { //��x���ׂ��炻����g��
					try {
						WinHttpCloseHandle(IntPtr.Zero);
						_available = true;
					}
					catch(Exception) {
						_available = false;
					}
					_examined = true;
				}
				return _available;
			}
		}

		private IntPtr _hSession;
		private URL_COMPONENTS _urlc;
		private string _url;

		public HTTPConnection(string url) {
			_url = url;
			_hSession = WinHttpOpen(	"Omega Chart/1.0",
				0,	// WINHTTP_ACCESS_TYPE_DEFAULT_PROXY
				string.Empty, // WINHTTP_NO_PROXY_NAME
				string.Empty, // WINHTTP_NO_PROXY_BYPASS
				0);

			if(_hSession==IntPtr.Zero) {
				// �G���[�FHTTP�Z�b�V�������J�n�ł��Ȃ�
				throw new HttpException("HTTP�Z�b�V�������J�n�ł��܂���B");
			}

			// url����
			_urlc = new URL_COMPONENTS();

			_urlc.dwStructSize = Marshal.SizeOf(_urlc);
			_urlc.dwSchemeLength = -1;
			_urlc.dwHostNameLength = -1;
			_urlc.dwUrlPathLength = -1;
			_urlc.dwExtraInfoLength = -1;

			int avd = url.Length;

			if(!WinHttpCrackUrl(url, url.Length, 0, ref _urlc)) {
				// �G���[�FURL���s��
				// #define ERROR_WINHTTP_INVALID_URL              (WINHTTP_ERROR_BASE + 5)
				// #define ERROR_WINHTTP_UNRECOGNIZED_SCHEME      (WINHTTP_ERROR_BASE + 6)
				throw new HttpException("�f�[�^�̎擾��URL���s���ł��B");
			}
		}
	
		public MemoryStream Open() {
			MemoryStream ms = new MemoryStream();

			IntPtr hConnect = IntPtr.Zero;
			IntPtr hRequest = IntPtr.Zero;
			try {
				// �z�X�g���擾�Econnect
				string hostname = Marshal.PtrToStringUni(_urlc.lpszHostName);
				hostname = hostname.Substring(0,hostname.IndexOf(Marshal.PtrToStringUni(_urlc.lpszUrlPath)));
			
				hConnect = WinHttpConnect(_hSession,hostname,_urlc.nPort,0);

				if(hConnect==IntPtr.Zero) {
					// �G���[�F�ڑ���񂪕s��
					throw new HttpException("�����G���[�F�ڑ���񂪕s���ł��B");
				}

				// ���N�G�X�g�쐬
				hRequest = WinHttpOpenRequest(hConnect,"GET",_urlc.lpszUrlPath,"HTTP/1.1",string.Empty,IntPtr.Zero,0x100); //�Ō�̒萔��WINHTTP_FLAG_REFRESH

				if(hRequest==IntPtr.Zero) {
					// �G���[�F���N�G�X�g���s��
					throw new HttpException("�����G���[�F���N�G�X�g���s���ł��B");
				}

				AdjustProxy(hConnect, hRequest);

				// ���N�G�X�g����M
				uint dwContext = 0;
				if(!WinHttpSendRequest(hRequest,string.Empty,0,IntPtr.Zero,0,0,ref dwContext)) {
					// �G���[�F���N�G�X�g���M�Ɏ��s
					throw new HttpException("�f�[�^�̎擾�Ɏ��s���܂����B");
				}

				if(!WinHttpReceiveResponse(hRequest,IntPtr.Zero)) {
					// �G���[�F���N�G�X�g�ԓ���M�Ɏ��s
					throw new HttpException("�f�[�^�̎擾�Ɏ��s���܂����B");
				}

				// �f�[�^�擾

				unsafe {
					uint size,downsize = 0;
					byte[] buf = new byte[0x1000];

					while(true) {
						size = 0;
						if(!WinHttpQueryDataAvailable(hRequest,ref size)) {
							// �G���[�FHTTP�f�[�^�擾�Ɏ��s
							throw new HttpException("�f�[�^�̎擾(WinHttpQueryDataAvailable)�Ɏ��s���܂����B");
						}
						if(size == 0)
							break;

						if(buf.Length < size)
							buf = new byte[size];

						fixed (byte* pbuf = buf) {
							if(!WinHttpReadData(hRequest,pbuf,size,ref downsize)) {
								// �G���[�FHTTP�f�[�^�擾�Ɏ��s
								throw new HttpException("�f�[�^�̎擾(WinHttpReadData)�Ɏ��s���܂����B");
							}
							else {
								ms.Write(buf,0,(int)downsize);
							}
						}
					}
				}
			}
			finally {
				//�P�֐����Ŏg�p������������̂�try...finally, �����łȂ����̂�IDisposable��Finalize���g��
				if(hRequest!=IntPtr.Zero)
					WinHttpCloseHandle(hRequest);
				if(hConnect!=IntPtr.Zero)
					WinHttpCloseHandle(hConnect);
			}

			return ms;
		}

		private void AdjustProxy(IntPtr hConnect, IntPtr hRequest) {
			// IE�̐ݒ�擾
			WINHTTP_CURRENT_USER_IE_PROXY_CONFIG ieconfig = new WINHTTP_CURRENT_USER_IE_PROXY_CONFIG();
			WINHTTP_AUTOPROXY_OPTIONS apo = new WINHTTP_AUTOPROXY_OPTIONS();
			WINHTTP_PROXY_INFO pi = new WINHTTP_PROXY_INFO();
			try {
				WinHttpGetIEProxyConfigForCurrentUser(ref ieconfig);

				if(ieconfig.lpszAutoConfigUrl!=IntPtr.Zero) {
					// pac�t�@�C��url�w��
					apo.dwFlags = 2; // WINHTTP_AUTOPROXY_CONFIG_URL
					apo.lpszAutoConfigUrl = ieconfig.lpszAutoConfigUrl;
				}
				else if(ieconfig.fAutoDetect) {
					// WPAD
					apo.dwFlags = 1; // WINHTTP_AUTOPROXY_AUTO_DETECT
					apo.dwAutoDetectFlags = 3; // WINHTTP_AUTO_DETECT_TYPE_DHCP | WINHTTP_AUTO_DETECT_TYPE_DNS_A;
					apo.fAutoLogonIfChallenged = true;
				}

				// proxy�ݒ�̎擾�A�����͎��s���Ă�����
				if(WinHttpGetProxyForUrl(_hSession, _url, ref apo,ref pi)) {
					unsafe  {
						if(!WinHttpSetOption(hRequest,38/*WINHTTP_OPTION_PROXY*/, ref pi, sizeof(WINHTTP_PROXY_INFO))) {
							// �G���[�Fproxy���̐ݒ肪�ł��Ȃ�
							throw new HttpException("�����G���[�F�v���N�V��񂪐ݒ�ł��܂���B");
						}
					}
				}
			}
			finally {

				if(ieconfig.lpszAutoConfigUrl!=IntPtr.Zero)
					GlobalFree(ieconfig.lpszAutoConfigUrl);
				if(ieconfig.lpszProxy!=IntPtr.Zero)
					GlobalFree(ieconfig.lpszProxy);
				if(ieconfig.lpszProxyBypass!=IntPtr.Zero)
					GlobalFree(ieconfig.lpszProxyBypass);

				if(pi.lpszProxy!=IntPtr.Zero)
					GlobalFree(pi.lpszProxy);
				if(pi.lpszProxyBypass!=IntPtr.Zero)
					GlobalFree(pi.lpszProxyBypass);
			}
		}

		public void Dispose() {
			CleanUp();
		}
		~HTTPConnection() {
			CleanUp();
		}
		private void CleanUp() {
			if(_hSession!=IntPtr.Zero)
				WinHttpCloseHandle(_hSession);
			_hSession = IntPtr.Zero;
		}

	}
}