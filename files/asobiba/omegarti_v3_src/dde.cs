//DDE
//C++�p�ɍ�������������C#�ɂ��܂����B
//NDDE �� msdn �� C++�̃w�b�_�[���Q�l�ɍ쐬���܂����B
//���Ȃ݂� NDDE ����̃R�s�y�͈�؂���܂���B
//
//�\�z�ȏ�ɖʓ|�������A����Ȃ�f���� NDDE���g���΂悩���������m��ʁB
//
using System;
using System.Collections;
using System.Text;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace Zanetti.DDE
{
    //DDE�֌W�̃C���|�[�g
    public class DDESource
    {
        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static int DdeInitialize(
                  ref int pidInst,          // �C���X�^���X���ʎq�ւ̃|�C���^
                  IntPtr pfnCallback,  // �R�[���o�b�N�֐��ւ̃|�C���^ �g��Ȃ��̂� null
                  uint afCmd,              // �R�}���h�t���O�ƃt�B���^�t���O
                  int ulRes               // �\��ς�
            );
        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static int DdeUninitialize(
                  int idInst   // �C���X�^���XID
            );
        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static uint DdeGetLastError(
                int idInst   // �C���X�^���X���ʎq
           );

        //DdeInitialize�̈���
        public const uint APPCMD_CLIENTONLY = (0x00000010);
        public const uint APPCMD_FILTERINITS = (0x00000020);
        public const uint APPCMD_MASK = (0x00000FF0);

        public const uint CBF_SKIP_CONNECT_CONFIRMS = (0x00040000);
        public const uint CBF_SKIP_REGISTRATIONS = (0x00080000);
        public const uint CBF_SKIP_UNREGISTRATIONS = (0x00100000);
        public const uint CBF_SKIP_DISCONNECTS = (0x00200000);
        public const uint CBF_SKIP_ALLNOTIFICATIONS = (0x003c0000);


        //�߂�l(�����ƃG���[)
        public const int DMLERR_NO_ERROR = 0;
        public const uint DMLERR_FIRST = 0x4000;

        public const uint DMLERR_ADVACKTIMEOUT = 0x4000;
        public const uint DMLERR_BUSY = 0x4001;
        public const uint DMLERR_DATAACKTIMEOUT = 0x4002;
        public const uint DMLERR_DLL_NOT_INITIALIZED = 0x4003;
        public const uint DMLERR_DLL_USAGE = 0x4004;
        public const uint DMLERR_EXECACKTIMEOUT = 0x4005;
        public const uint DMLERR_INVALIDPARAMETER = 0x4006;
        public const uint DMLERR_LOW_MEMORY = 0x4007;
        public const uint DMLERR_MEMORY_ERROR = 0x4008;
        public const uint DMLERR_NOTPROCESSED = 0x4009;
        public const uint DMLERR_NO_CONV_ESTABLISHED = 0x400a;
        public const uint DMLERR_POKEACKTIMEOUT = 0x400b;
        public const uint DMLERR_POSTMSG_FAILED = 0x400c;
        public const uint DMLERR_REENTRANCY = 0x400d;
        public const uint DMLERR_SERVER_DIED = 0x400e;
        public const uint DMLERR_SYS_ERROR = 0x400f;
        public const uint DMLERR_UNADVACKTIMEOUT = 0x4010;
        public const uint DMLERR_UNFOUND_QUEUE_ID = 0x4011;

        public const uint DMLERR_LAST = 0x4011;

        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static IntPtr DdeCreateStringHandle(
                int idInst,  // �C���X�^���X���ʎq
                string psz,    // NULL �ŏI��镶����ւ̃|�C���^
                uint iCodePage  // �R�[�h�y�[�W���ʎq
            );
        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static bool DdeFreeStringHandle(
                int idInst,  // �C���X�^���X���ʎq
                IntPtr hsz        // ������̃n���h��
            );

        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static IntPtr DdeConnect(
                int idInst,      // �C���X�^���X���ʎq
                IntPtr hszService,    // �T�[�r�X���̕�����̃n���h��
                IntPtr hszTopic,      // �g�s�b�N���̕�����̃n���h��
                IntPtr pCC   // �R���e�L�X�g�f�[�^���������\���̂ւ̃|�C���^(����͗��p���Ȃ��̂�IntPtr null������悤��)
            );

        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static bool DdeDisconnect(
                IntPtr hConv   // �Θb�̃n���h��
            );

        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static IntPtr DdeClientTransaction(
                byte[] pData,       // �T�[�o�[�ɓn���f�[�^�ւ̃|�C���^
                int cbData,       // �f�[�^�̒���
                IntPtr hConv,        // �Θb�̃n���h��
                IntPtr hszItem,        // ���ږ��̕�����̃n���h��
                uint wFmt,          // �N���b�v�{�[�h�f�[�^�`��
                uint wType,         // �g�����U�N�V�����^
                int dwTimeout,    // �^�C���A�E�g����
                ref int pdwResult   // �g�����U�N�V�����̌��ʂւ̃|�C���^
            );

        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static int DdeGetData(
                IntPtr hData,  // DDE �I�u�W�F�N�g�̃n���h��
                StringBuilder pDst,     // �R�s�[��o�b�t�@�ւ̃|�C���^
                int cbMax,     // �R�s�[����f�[�^�̗�
                int cbOff      // �f�[�^�̐擪�ւ̃I�t�Z�b�g
           );
        [DllImport("user32.dll", CharSet = CharSet.Ansi)]
        public extern static bool DdeFreeDataHandle(
                IntPtr hData   // DDE �I�u�W�F�N�g�̃n���h��
           );

        public const uint CP_WINANSI = 1004;    /* default codepage for windows & old DDE convs. */
        public const uint CP_WINUNICODE = 1200;

        public const uint CF_TEXT = 1;

        public const uint XTYPF_NOBLOCK = 0x0002;  /* CBR_BLOCK will not work */
        public const uint XTYPF_NODATA = 0x0004;  /* DDE_FDEFERUPD */
        public const uint XTYPF_ACKREQ = 0x0008;  /* DDE_FACKREQ */

        public const uint XCLASS_MASK = 0xFC00;
        public const uint XCLASS_BOOL = 0x1000;
        public const uint XCLASS_DATA = 0x2000;
        public const uint XCLASS_FLAGS = 0x4000;
        public const uint XCLASS_NOTIFICATION = 0x8000;

        public const uint XTYP_ERROR = (0x0000 | XCLASS_NOTIFICATION | XTYPF_NOBLOCK);
        public const uint XTYP_ADVDATA = (0x0010 | XCLASS_FLAGS);
        public const uint XTYP_ADVREQ = (0x0020 | XCLASS_DATA | XTYPF_NOBLOCK);
        public const uint XTYP_ADVSTART = (0x0030 | XCLASS_BOOL);
        public const uint XTYP_ADVSTOP = (0x0040 | XCLASS_NOTIFICATION);
        public const uint XTYP_EXECUTE = (0x0050 | XCLASS_FLAGS);
        public const uint XTYP_CONNECT = (0x0060 | XCLASS_BOOL | XTYPF_NOBLOCK);
        public const uint XTYP_CONNECT_CONFIRM = (0x0070 | XCLASS_NOTIFICATION | XTYPF_NOBLOCK);
        public const uint XTYP_XACT_COMPLETE = (0x0080 | XCLASS_NOTIFICATION);
        public const uint XTYP_POKE = (0x0090 | XCLASS_FLAGS);
        public const uint XTYP_REGISTER = (0x00A0 | XCLASS_NOTIFICATION | XTYPF_NOBLOCK);
        public const uint XTYP_REQUEST = (0x00B0 | XCLASS_DATA);
        public const uint XTYP_DISCONNECT = (0x00C0 | XCLASS_NOTIFICATION | XTYPF_NOBLOCK);
        public const uint XTYP_UNREGISTER = (0x00D0 | XCLASS_NOTIFICATION | XTYPF_NOBLOCK);
        public const uint XTYP_WILDCONNECT = (0x00E0 | XCLASS_DATA | XTYPF_NOBLOCK);

        public const uint XTYP_MASK = 0x00F0;
        public const uint XTYP_SHIFT = 4;  /* shift to turn XTYP_ into an index */

    }

    //DDE�̏������n  ��x�������΂���
    public class DDEMasterInstance
    {
        public DDEMasterInstance()
	    {
            this.DDEInstance = 0;
	    }
	    ~DDEMasterInstance()
	    {
            Destroy();
	    }
        //�쐬
	    public void Create()
	    {
            Debug.Assert(this.DDEInstance == 0);

            if (DDESource.DdeInitialize(ref this.DDEInstance, IntPtr.Zero, DDESource.APPCMD_CLIENTONLY | DDESource.CBF_SKIP_ALLNOTIFICATIONS, 0) != DDESource.DMLERR_NO_ERROR) 
		    {
			    throw new ApplicationException ( GetDDEErrorMessage( ) );
		    }
	    }
        //�j��
        public void Destroy() 
	    {
            if (this.DDEInstance != 0)
		    {
                DDESource.DdeUninitialize(this.DDEInstance);
                this.DDEInstance = 0;
		    }
	    }
        //DDE�C���X�^���X�̎擾
	    public int getInstance()
	    {
		    return this.DDEInstance;
	    }
        //DDE�G���[�̕�����
	    public  string GetDDEErrorMessage() 
        {
            uint uiErrNo = DDESource.DdeGetLastError(this.DDEInstance);
	        switch(uiErrNo)
	        {
            case DDESource.DMLERR_NO_ERROR:
		        return "DMLERR_NO_ERROR �����ɐ������܂���";
            case DDESource.DMLERR_ADVACKTIMEOUT:
		        return "DMLERR_ADVACKTIMEOUT �����A�h�o�C�Y�g�����U�N�V�����̗v�����^�C���A�E�g�ɂȂ�܂����B ";
            case DDESource.DMLERR_BUSY: 
		        return "DMLERR_BUSY �g�����U�N�V�����ւ̉����ɂ�� DDE_FBUSY �t���O���Z�b�g����܂����B ";
            case DDESource.DMLERR_DATAACKTIMEOUT: 
		        return "DMLERR_DATAACKTIMEOUT �����f�[�^�g�����U�N�V�����̗v�����^�C���A�E�g�ɂȂ�܂����B ";
            case DDESource.DMLERR_DLL_NOT_INITIALIZED: 
		        return "DMLERR_DLL_NOT_INITIALIZED DdeInitialize �֐����Ăяo������ DDEML �֐����Ăяo�������ADDEML �֐��ɓn�����C���X�^���X���ʎq�������ł��B ";
            case DDESource.DMLERR_DLL_USAGE: 
		        return "DMLERR_DLL_USAGE APPCLASS_MONITOR�Ƃ��ď��������ꂽ�A�v���P�[�V������ DDE�iDynamic Data Exchange�j�g�����U�N�V���������݂����AAPPCMD_CLIENTONLY �Ƃ��ď��������ꂽ�A�v���P�[�V�������T�[�o�[�g�����U�N�V�����̎��s�����݂܂����B ";
            case DDESource.DMLERR_EXECACKTIMEOUT:
		        return "DMLERR_EXECACKTIMEOUT �������s�g�����U�N�V�����̗v�����^�C���A�E�g�ɂȂ�܂����B ";
            case DDESource.DMLERR_INVALIDPARAMETER:
		        return "DMLERR_INVALIDPARAMETER DDEML���p�����[�^��L���ɂł��܂���ł����B���̗��R���l�����܂��B \r\n" + 
				        "�g�����U�N�V�������v��������̂Ƃ͈قȂ鍀�ږ��̃n���h���ŏ����������f�[�^�n���h�����g�������߁B\r\n" + 
				        "�g�����U�N�V�������v��������̂Ƃ͈قȂ�N���b�v�{�[�h�f�[�^�`���ŏ����������f�[�^�n���h�����g�������߁B\r\n" + 
				        "�T�[�o�[���֐��ŃN���C�A���g���̑Θb�n���h�����g�������A�N���C�A���g���ŃT�[�o�[���̑Θb�n���h�����g�������߁B\r\n" + 
				        "������ꂽ�f�[�^�n���h���܂��͕�����n���h�����g�������߁B\r\n" +
				        "�A�v���P�[�V�����̕����̃C���X�^���X�������I�u�W�F�N�g���g�������߁B";
            case DDESource.DMLERR_LOW_MEMORY: 
		        return "DMLERR_LOW_MEMORY DDEML�A�v���P�[�V�����ɂ���ăT�[�o�[��s��ԁi �T�[�o�[�A�v���P�[�V�������N���C�A���g�̏����\�͈ȏ�ɃT�[�r�X��񋟂��Ă����ԁj�ɂȂ�A��ʂ̃������������܂����B ";
            case DDESource.DMLERR_MEMORY_ERROR:
		        return "DMLERR_MEMORY_ERROR ���������蓖�ĂɎ��s���܂����B ";
            case DDESource.DMLERR_NO_CONV_ESTABLISHED:
		        return "DMLERR_NO_CONV_ESTABLISHED �N���C�A���g���Θb���m�����悤�Ƃ��Ď��s���܂����B ";
            case DDESource.DMLERR_NOTPROCESSED:
		        return "DMLERR_NOTPROCESSED �g�����U�N�V�����Ɏ��s���܂����B ";
            case DDESource.DMLERR_POKEACKTIMEOUT:
		        return "DMLERR_POKEACKTIMEOUT �����|�[�N�g�����U�N�V�����̗v�����^�C���A�E�g�ɂȂ�܂����B ";
            case DDESource.DMLERR_POSTMSG_FAILED:
		        return "DMLERR_POSTMSG_FAILED PostMessage �֐��̓����Ăяo���Ɏ��s���܂����B ";
            case DDESource.DMLERR_REENTRANCY:
		        return "DMLERR_REENTRANCY �����g�����U�N�V���������łɐi�s���̃A�v���P�[�V�����C���X�^���X�ŕʂ̓����g�����U�N�V�������J�n���悤�Ƃ������ADdeEnableCallback �֐��� DDEML �R�[���o�b�N�֐����ŌĂяo����܂����B ";
            case DDESource.DMLERR_SERVER_DIED:
		        return "DMLERR_SERVER_DIED �N���C�A���g�����I�������Θb�ŃT�[�o�[���g�����U�N�V���������݂�ꂽ���A�g�����U�N�V��������������O�ɃT�[�o�[���I�����܂����B ";
            case DDESource.DMLERR_SYS_ERROR:
		        return "DMLERR_SYS_ERROR DDEML�œ����G���[���������܂����B ";
            case DDESource.DMLERR_UNADVACKTIMEOUT:
		        return "DMLERR_UNADVACKTIMEOUT �A�h�o�C�Y�g�����U�N�V�����̏I���v�����^�C���A�E�g�ɂȂ�܂����B ";
            case DDESource.DMLERR_UNFOUND_QUEUE_ID:
		        return "DMLERR_UNFOUND_QUEUE_ID DDEML �֐��ɓn���ꂽ�g�����U�N�V�������ʎq�������ł��B�A�v���P�[�V������ XTYP_XACT_COMPLETE �R�[���o�b�N����߂�ƁA���̃R�[���o�b�N�֐��̃g�����U�N�V�������ʎq�͖����ɂȂ�܂��B";
	        }

            return String.Format("���m�̃G���[{0}���������܂���" , uiErrNo);
        }

        private int DDEInstance;
    };

    //DDE�ʐM�ɗ��p���镶����
    public class DDEString
    {
        public DDEString()
        {
            this.PoolDDEInstance = null;
            this.ItemDDEStringHandle = IntPtr.Zero;
        }
        ~DDEString()
        {
            Destroy();
        }
        //������̍쐬
        public IntPtr Create(DDEMasterInstance poolDDEInstance, string inItem)
        {
            Debug.Assert(this.PoolDDEInstance == null);
            Debug.Assert(this.ItemDDEStringHandle == IntPtr.Zero);
            Debug.Assert(poolDDEInstance != null);
            Debug.Assert(inItem.Length != 0);

            this.PoolDDEInstance = poolDDEInstance;

            //�ʐM���镶����̍쐬
            //�Ɍ��܂ő����������Ȃ�A�L���b�V������Α����Ȃ邩���B
            this.ItemDDEStringHandle = DDESource.DdeCreateStringHandle(
                this.PoolDDEInstance.getInstance(), inItem, DDESource.CP_WINANSI);
            if (this.ItemDDEStringHandle == IntPtr.Zero)
            {
                throw new ApplicationException(this.PoolDDEInstance.GetDDEErrorMessage());
            }
            return this.ItemDDEStringHandle;
        }
        //�J��
        public  void Destroy()
        {
            if (this.ItemDDEStringHandle != IntPtr.Zero)
            {
                Debug.Assert(this.PoolDDEInstance != null);

                DDESource.DdeFreeStringHandle(this.PoolDDEInstance.getInstance(), this.ItemDDEStringHandle);
                this.ItemDDEStringHandle = IntPtr.Zero;
            }
            this.PoolDDEInstance = null;
        }
        //�n���h���̎擾
        //�O�ŏ���ɊJ�������肷��ƃ_����
        public IntPtr Value
        {
            get
            {
                Debug.Assert(this.ItemDDEStringHandle != IntPtr.Zero);
                return this.ItemDDEStringHandle;
            }
        }

        private DDEMasterInstance PoolDDEInstance;
        private IntPtr ItemDDEStringHandle;
    };

    //DDE �N���C�A���g
    public class DDEClient  
    {
        public DDEClient()
	    {
            this.PoolDDEInstance = null;
            this.DDEConnection = IntPtr.Zero;
            this.ServiceNameStringHandle = new DDEString();
            this.TopicNameStringHandle = new DDEString();
            this.TimeOut = 6000;
	    }
	    ~DDEClient()
	    {
		    DisConnect();
	    }

        //�ڑ�
        public void Connect(DDEMasterInstance poolDDEInstance,
		    string inServiceName ,string inTopicName )
        {
            Debug.Assert(this.PoolDDEInstance == null);
            Debug.Assert(this.DDEConnection == IntPtr.Zero);
            Debug.Assert(poolDDEInstance != null);

            this.PoolDDEInstance = poolDDEInstance;

            ServiceNameStringHandle.Create(this.PoolDDEInstance, inServiceName);
            TopicNameStringHandle.Create(this.PoolDDEInstance, inTopicName);

            this.DDEConnection = DDESource.DdeConnect(
                this.PoolDDEInstance.getInstance(), ServiceNameStringHandle.Value, TopicNameStringHandle.Value, IntPtr.Zero);
            if (this.DDEConnection == IntPtr.Zero)
            {
                throw new ApplicationException(this.PoolDDEInstance.GetDDEErrorMessage());
            }
        }

        //�ؒf
	    public  void DisConnect()
        {
            if (this.DDEConnection != IntPtr.Zero)
            {
                DDESource.DdeDisconnect(this.DDEConnection);
                this.DDEConnection = IntPtr.Zero;
            }
            ServiceNameStringHandle.Destroy();
            TopicNameStringHandle.Destroy();
            this.PoolDDEInstance = null;
        }

        //���̎擾
        public string Trans(DDEString inItem)
        {
            Debug.Assert(this.PoolDDEInstance != null);
            Debug.Assert(this.DDEConnection != IntPtr.Zero);
            Debug.Assert(inItem != null);

            int dummyResult = 0;
            IntPtr ddeData =
                DDESource.DdeClientTransaction(null, 0, this.DDEConnection, inItem.Value, DDESource.CF_TEXT, DDESource.XTYP_REQUEST, this.TimeOutValue, ref dummyResult);
            if (ddeData == IntPtr.Zero)
            {
                //���肤��! �̂ŁA��O�ɂ͂��Ȃ�
                return "";
            }

            int length = DDESource.DdeGetData(ddeData, null, 0, 0);

            StringBuilder buffer = new StringBuilder(length);
            DDESource.DdeGetData(ddeData, buffer, length, 0);

            DDESource.DdeFreeDataHandle(ddeData);

            return buffer.ToString();
        }

        //���̎擾
	    public  string Trans(string  inItem )
        {
            Debug.Assert( this.PoolDDEInstance != null );
            Debug.Assert( inItem.Length != 0 );

            //�ʐM���镶����̍쐬
            DDEString ddeString = new DDEString();
            ddeString.Create(this.PoolDDEInstance , inItem);

            return this.Trans(ddeString);
        }
	    public  int TransINT(string  inItem )
	    {
            string str = Trans(inItem);

            int ret;
            if (! int.TryParse(str, out ret))
            {
                return 0;
            }
            return ret;
	    }
        public double TransDouble(string inItem)
        {
            string str = Trans(inItem);

            double ret;
            if (!Double.TryParse(str, out ret))
            {
                return 0;
            }
            return ret;
        }
        public int TimeOut
        {
            get
            {
                return this.TimeOutValue;
            }
            set
            {
                this.TimeOutValue = value;
            }
        }

	    private DDEMasterInstance	PoolDDEInstance;
        private IntPtr DDEConnection;
        private DDEString ServiceNameStringHandle;
        private DDEString TopicNameStringHandle;
        private int TimeOutValue;
    };

    //DDE�N���C�A���g + DDE������L���b�V��
    //DDE�ŕ�������L���b�V�����郄�c.
    //�p���������Ă����񂾂��ǁA�A�Ϗ����D���Ȃ�ŁB
    public class DDEClientWithCache
    {
        public DDEClientWithCache()
        {
            this.PoolDDEInstance = null;
            this.Client = new DDEClient();
            this.DDEStringCache = new Hashtable();
        }
        ~DDEClientWithCache()
        {
            DisConnect();
        }

        //�ڑ�
        public void Connect(DDEMasterInstance poolDDEInstance,
            string inServiceName, string inTopicName)
        {
            Debug.Assert(this.PoolDDEInstance == null);
            Debug.Assert(poolDDEInstance != null);

            this.PoolDDEInstance = poolDDEInstance;
            this.Client.Connect(this.PoolDDEInstance, inServiceName, inTopicName);
        }

        //�ؒf
        public void DisConnect()
        {
            this.Client.DisConnect();
            this.PoolDDEInstance = null;
        }

        //�L���b�V�����N���A �g�������邩��...
        public void CacheClear()
        {
            this.DDEStringCache = new Hashtable();
        }

        //���̎擾
        public string Trans(string inItem)
        {
            Debug.Assert(this.PoolDDEInstance != null);

            DDEString ddeString = (DDEString)this.DDEStringCache[inItem];
            if (ddeString == null)
            {
                //�L���b�V���ɂȂ��̂ō쐬����.
                ddeString = new DDEString();
                ddeString.Create(this.PoolDDEInstance , inItem);
                this.DDEStringCache[inItem] = ddeString;
            }

            return this.Client.Trans(ddeString);
        }
        public int TransINT(string inItem)
        {
            string str = Trans(inItem);

            int ret;
            if (!int.TryParse(str, out ret))
            {
                return 0;
            }
            return ret;
        }
        public double TransDouble(string inItem)
        {
            string str = Trans(inItem);

            double ret;
            if (!Double.TryParse(str, out ret))
            {
                return 0;
            }
            return ret;
        }
        public int TimeOut
        {
            get
            {
                return this.Client.TimeOut;
            }
            set
            {
                this.Client.TimeOut = value;
            }
        }
        private DDEMasterInstance PoolDDEInstance;
        private DDEClient Client;
        private Hashtable DDEStringCache;
    };
    /*
    public class DDETest
    {
        public static void test()
        {
            DDEMasterInstance master = new DDEMasterInstance();
            master.Create();

            DDEClientWithCache client = new DDEClientWithCache();
            client.Connect( master , "RSS" , "5401.T");
            int a = client.TransINT("�o����");

            int b = client.TransINT("�o����");

        }
    }
    */
}
