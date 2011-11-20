using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using Zanetti.DDE;
using Zanetti.Data;

namespace Zanetti.RealTimeUpdate
{
    //���C��
    class Main
    {
        private RSSThread ThreadProc;
        private Thread Thread;
        public Main(IntPtr inNotifyWindowHandle, int inNotifyWindowMessage)
        {
            this.ThreadProc = new RSSThread();
            this.ThreadProc.DMZ.SetNotify(inNotifyWindowHandle, inNotifyWindowMessage);

            this.Thread = null;
        }
        ~Main()
        {
            Stop();
        }
        //�X���b�h�̊J�n
        public void Start()
        {
            if (this.Thread != null)
            {
                return; //���łɓ��쒆
            }
            this.Thread = new Thread(new ThreadStart(this.ThreadProc.Run));
            this.Thread.Priority = ThreadPriority.Lowest;   //�D��x�͒��
            this.Thread.IsBackground = true;
            this.Thread.Start();
        }
        //�X���b�h�̒�~
        public void Stop()
        {
            if (this.Thread == null)
            {
                return; //�X���b�h�͓����Ă��܂���!!
            }
            if (!this.Thread.IsAlive)
            {
                return; //�X���b�h�͂��łɎ���ł��܂�
            }
            //�X���b�h���~�����܂���.
            //this.Thread.Abort(); //Interrupt �ŏI��点��̂�����? Abort()�������Ă���y�[�W�����\����̂͂Ȃ�?
            this.Thread.Interrupt();
            this.Thread.Join();
            this.Thread = null;
        }
        //�X���b�h�͐����Ă܂���?
        public bool IsAlive()
        {
            return this.Thread != null;
        }
        //�������̎擾
        public void GetKabuka(out int outCode, out int outDate, out NewDailyData outData)
        {
            this.ThreadProc.DMZ.GetKabuka(out outCode, out  outDate, out  outData);
        }
        //�������̎擾
        public bool SetNextCode(int inCode , MarketType inType)
        {
            string codeString = inCode + ".";
            switch (inType)
            {
                case MarketType.T1:
                case MarketType.T2:
                    codeString += "T";
                    break;
                case MarketType.O1:
                case MarketType.O2:
                    codeString += "OS";
                    break;
                case MarketType.J:
                    codeString += "Q";
                    break;
                case MarketType.M:
                    codeString += "T";
                    break;
                case MarketType.H:
                    codeString += "OJ";
                    break;
                /* �}�P�X�s���o�������擾�ł��Ȃ��̂Ŏ~�߂�
                case MarketType.B:
                    switch (inCode)
                    {
                        case 101:   //���o����
                            codeString = "N225";
                            break;
                        case 102:   //TOPIX
                            codeString = "TOPX";
                            break;
                        case 106:   //JASDAQ
                            codeString = "JSD";
                            break;
                        case 151:   //���o���ϐ敨
                            codeString = "N225.FUT01.OS";
                            break;
                        default:
                            //���Ή�
                            this.ThreadProc.DMZ.SetNextCode(0, "");
                            return false;
                    }
                    break;
                 */
                default:
                    //���Ή�
                    this.ThreadProc.DMZ.SetNextCode(0, "");
                    return false;
            }
            this.ThreadProc.DMZ.SetNextCode(inCode, codeString);
            return true;
        }

    };
    internal class DMZ
    {
        private volatile int NextCode;
        private volatile string NextCodeString;
        private volatile int Code;
        private volatile int Date;
        private volatile NewDailyData Data;
        private volatile string ErrorMessage;
        //�f�[�^�ɍX�V���������ꍇ�̒ʒm��
        private volatile IntPtr NotifyWindowHandle;
        private volatile int NotifyWindowMessage;

        public void SetNextCode(int inCode,string inCodeString)
        {
            lock (this) ;
            this.NextCode = inCode;
            this.NextCodeString = inCodeString; 
        }

        public string GetNextCodeString()
        {
            lock (this) ;
            return this.NextCodeString;
        }
        public int GetNextCode()
        {
            lock (this) ;
            return this.NextCode;
        }
        public void SetKabuka(int inCode,int inDate ,NewDailyData inData)
        {
            lock (this) ;
            this.Code = inCode;
            this.Date = inDate;
            this.Data = inData;
        }
        public void GetKabuka(out int outCode, out int outDate, out NewDailyData outData)
        {
            lock (this) ;
            outCode = this.Code;
            outDate = this.Date;
            outData = this.Data;
        }
        public void SetError(string inErrorMessage)
        {
            lock (this) ;
            this.ErrorMessage = inErrorMessage;
        }
        public void SetNotify(IntPtr inNotifyWindowHandle,int inNotifyWindowMessage)
        {
            lock (this) ;
            this.NotifyWindowHandle = inNotifyWindowHandle;
            this.NotifyWindowMessage = inNotifyWindowMessage;
        }
        public void Notify()
        {
            IntPtr notifyWindowHandle;
            int notifyWindowMessage;
            {
                lock (this) ;
                notifyWindowHandle = this.NotifyWindowHandle;
                notifyWindowMessage = this.NotifyWindowMessage;
            }
            Win32.PostMessage(notifyWindowHandle, notifyWindowMessage, IntPtr.Zero, IntPtr.Zero);
        }
    };
    //�X���b�h
    internal class RSSThread
    {
        //�������Ă���،��R�[�h
        private int  Code;
        //���ݒl�L���b�V��
        private int GenzaineDekidaka;

        //�f�[�^�̎󂯓n���ꏊ
        public DMZ DMZ;

        //DDE�e��
        DDE.DDEMasterInstance DDEMaster;
        DDE.DDEClientWithCache DDEClient;



        public RSSThread()
        {
            this.DMZ = new DMZ();
        }


        //�X���b�h���C��
        public void Run()
        {
            this.Code = 0;
            this.GenzaineDekidaka = 0;
            this.DDEMaster = new DDE.DDEMasterInstance();
            this.DDEClient = new DDE.DDEClientWithCache();
            this.DDEMaster.Create();

            try
            {
                while (true)
                {
                    try
                    {
                        if (RSS())
                        {//���܂��擾�ł����̂łق�̏����x�݂܂�
                            Thread.Sleep(1000);
                        }
                        else
                        {//���܂��擾�ł��Ȃ������̂Ō��\�x�݂܂�
                            Thread.Sleep(3000);    //3�b���炢
                        }
                    }
                    catch (ApplicationException ex)
                    {//���܂��擾�ł��Ȃ������̂Ō��\�x�݂܂�
                        this.DMZ.SetError(ex.Message);
                        Thread.Sleep(3000);    //3�b���炢
                    }
                }
            }
            catch (ThreadInterruptedException e)
            {
                //�����܂�.
            }
        }
        //�X���b�h���C��
        private bool RSS()
        {
            //���ݐڑ����Ă�������̊m�F
            int nextCode = this.DMZ.GetNextCode();
            if (nextCode == 0)
            {
                return false;
            }
            if (this.Code != nextCode)
            {//���[�U�[�͕ʂ̖��������w��
                this.Code = nextCode;
                this.DDEClient.DisConnect();
                this.DDEClient.Connect(this.DDEMaster, "RSS", this.DMZ.GetNextCodeString());
            }

            //������?
            string genzaihizuke = this.DDEClient.Trans("���ݓ��t");
            if (genzaihizuke == "")
            {
                //�ؒf���ꂽ�炵���A�Đڑ�����.
                this.DDEClient.DisConnect();
                this.DDEClient.Connect(this.DDEMaster, "RSS", this.DMZ.GetNextCodeString());

                //���g���C
                genzaihizuke = this.DDEClient.Trans("���ݓ��t");
                if (genzaihizuke == "")
                {//����ς肾��
                    throw new  ApplicationException ( "RSS���ؒf����܂���" );
                }
            }

            int dekidaka = this.DDEClient.TransINT("�o����");
            //�o�����������Ă��Ȃ�����A�X�V�͂Ȃ���
            if (this.GenzaineDekidaka == dekidaka)
            {
                return true;
            }
            this.GenzaineDekidaka = dekidaka;
            //���t���Ă��Ȃ�
            if (dekidaka == 0)
            {
                return true;
            }

            int genzaine = (int)this.DDEClient.TransDouble("���ݒl");
            int hatune = (int)this.DDEClient.TransDouble("�n�l");
            int takane = (int)this.DDEClient.TransDouble("���l");
            int yasune = (int)this.DDEClient.TransDouble("���l");

            //�ُ�ȃf�[�^?
            if (hatune == 0 || takane == 0 || yasune == 0 || genzaine == 0)
            {
                return false;
            }
            //�f�[�^�̐��`
            int date = Util.StringDateToInt(genzaihizuke);

            NewDailyData data = new NewDailyData();
            data.open = hatune;
            data.high = takane;
            data.low = yasune;
            data.close = genzaine;
            data.volume = dekidaka;
            this.DMZ.SetKabuka(this.Code, date , data);
            this.DMZ.Notify();

            return true;
        }
    };
}
