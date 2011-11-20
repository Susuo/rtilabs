//hacked by rti
/*
 * Copyright (c) rti    ���[��炢�Ƃ肴�[��
 * 
 * $Id$
 */
using System;
using System.Xml;
using System.IO;
using System.Drawing;
using System.Diagnostics;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Security.Cryptography;

using Travis.Storage;

using System.Data;
using System.Data.OleDb;

namespace Zanetti.CodeExtendData{

    //���
    abstract class DataInterface : IComparable
    {
        public abstract int getDate();
        public abstract void Read(XmlNode inX);
        public abstract XmlNode Write(XmlDocument ioDocument);

        public int CompareTo(object obj)
        {
            if (this.getDate() > ((DataInterface)obj).getDate())
            {
                return 1;
            }
            else if (this.getDate() < ((DataInterface)obj).getDate())
            {
                    return -1;
            }
            return 0;
        }

        public virtual int Compare(object x, object y)
        {
            if (((DataInterface)x).getDate() < ((DataInterface)y).getDate())
            {
                return -1;
            }
            else if (((DataInterface)x).getDate() > ((DataInterface)y).getDate())
            {
                return 1;
            }
            return 0;
        }
    }

    //�t�@���_�����^���ȏ����܂Ƃ߂ĕԂ��܂��B
    class DataBasket
    {
        public int Kettusan = 0;       //���Z�� yyyymmdd
        public int Uriage = 0;         //����(�S���~)
        public int Keijyourieki = 0;   //�o�험�v(�S���~)
        public int Rieki = 0;          //���v(�S���~)
        public int Hitoikabueki = 0;   //�ꊔ�v(�K)
        public int Haitoukin = 0;      //�z����(�K)
        public int Hitokabujyunsisan = 0;   //�ꊔ�����Y
        public int ZenkiKettusan = 0;   //�O�����Z�� yyyymmdd
    };

    //���Z���
    class DataAccounts : DataInterface
    {
        public int Code;
        public int Date;
        public int Kubun;               //���Z�敪 0:1/4�l����    1:2/4���Ԍ��Z   2:3/4�l����     3:4/4�{���Z
        public int ZenkiKettusan;       //�O�����Z��
//        public int ZenkiUriage;         //�O������(�S���~)
//        public int ZenkiKeijyourieki;   //�O���o�험�v(�S���~)
//        public int Zenkirieki;          //�O�����v(�S���~)
//        public int ZenkiHitoikabueki;   //�O���ꊔ�v(�K)
//        public int ZenkiHaitoukin;      //�O���z����(�K)
        public int KonkiKettusan;       //�������Z
        public int KonkiUriage;         //��������(�S���~)
        public int KonkiKeijyourieki;   //�����o�험�v(�S���~)
        public int Konkirieki;          //�������v(�S���~)
        public int KonkiHitoikabueki;   //�����ꊔ�v(�K)
        public int KonkiHaitoukin;      //�����z����(�K)
        public int JikiKettusan;        //�������Z
        public int JikiUriage;          //��������(�S���~)
        public int JikiKeijyourieki;    //�����o�험�v(�S���~)
        public int Jikirieki;           //�������v(�S���~)
        public int JikiHitoikabueki;    //�����ꊔ�v(�K)
        public int JikiHaitoukin;       //�����z����(�K)
        public int Hitokabujyunsisan;   //�ꊔ�����Y



        public override int getDate()
        {
            return this.Date;
        }

        //�ǂݍ���
        public override void Read(XmlNode inX)
        {
            if (inX.Name != "���Z") return;

            this.Date = int.Parse(inX.Attributes["���t"].InnerText);
            this.Code = int.Parse(inX.Attributes["�R�[�h"].InnerText);
            this.Kubun = int.Parse(inX["�敪"].InnerText);
            this.ZenkiKettusan = int.Parse(inX["�O�����Z��"].InnerText);
//            this.ZenkiUriage = int.Parse(inX["�O������"].InnerText);
//            this.ZenkiKeijyourieki = int.Parse(inX["�O���o�험�v"].InnerText);
//            this.Zenkirieki = int.Parse(inX["�O�����v"].InnerText);
//            this.ZenkiHitoikabueki = int.Parse(inX["�O���ꊔ�v"].InnerText);
//            this.ZenkiHaitoukin = int.Parse(inX["�O���z����"].InnerText);
            this.KonkiKettusan = int.Parse(inX["�������Z��"].InnerText);
            this.KonkiUriage = int.Parse(inX["��������"].InnerText);
            this.KonkiKeijyourieki = int.Parse(inX["�����o�험�v"].InnerText);
            this.Konkirieki = int.Parse(inX["�������v"].InnerText);
            this.KonkiHitoikabueki = int.Parse(inX["�����ꊔ�v"].InnerText);
            this.KonkiHaitoukin = int.Parse(inX["�����z����"].InnerText);
            this.JikiKettusan = int.Parse(inX["�������Z��"].InnerText);
            this.JikiUriage = int.Parse(inX["��������"].InnerText);
            this.JikiKeijyourieki = int.Parse(inX["�����o�험�v"].InnerText);
            this.Jikirieki = int.Parse(inX["�������v"].InnerText);
            this.JikiHitoikabueki = int.Parse(inX["�����ꊔ�v"].InnerText);
            this.JikiHaitoukin = int.Parse(inX["�����z����"].InnerText);
            this.Hitokabujyunsisan = int.Parse(inX["�ꊔ�����Y"].InnerText);
        }


        public override XmlNode Write(XmlDocument ioDocument)
        {
            XmlNode element = ioDocument.CreateElement("���Z");
            element.Attributes.Append(UtilX.CreateAttribute(ioDocument, "���t", this.Date.ToString()));
            element.Attributes.Append(UtilX.CreateAttribute(ioDocument, "�R�[�h", this.Code.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�敪", this.Kubun.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�O�����Z��", this.ZenkiKettusan.ToString()));
//            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�O������", this.ZenkiUriage.ToString()));
//            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�O���o�험�v", this.ZenkiKeijyourieki.ToString()));
//            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�O�����v", this.Zenkirieki.ToString()));
//            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�O���ꊔ�v", this.ZenkiHitoikabueki.ToString()));
//            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�O���z����", this.ZenkiHaitoukin.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�������Z��", this.KonkiKettusan.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "��������", this.KonkiUriage.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�����o�험�v", this.KonkiKeijyourieki.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�������v", this.Konkirieki.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�����ꊔ�v", this.KonkiHitoikabueki.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�����z����", this.KonkiHaitoukin.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�������Z��", this.JikiKettusan.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "��������", this.JikiUriage.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�����o�험�v", this.JikiKeijyourieki.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�������v", this.Jikirieki.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�����ꊔ�v", this.JikiHitoikabueki.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�����z����", this.JikiHaitoukin.ToString()));
            element.AppendChild(UtilX.CreateElemnt(ioDocument, "�ꊔ�����Y", this.Hitokabujyunsisan.ToString()));

            return element;
        }
    };


     //�ƐяC��
     class DataRevised : DataInterface
     {
         public int Code;
         public int Date;
         public int SyuseimaeUriage;    //�C���O����
         public int Uriage;             //����
         public int SyuseimaeKeijyourieki;    //�C���O�o�험�v
         public int Keijyourieki;             //�o�험�v
         public int SyuseimaeZeibikieki;    //�C���O�ň��v
         public int Zeibikieki;             //�ň��v
         public int SyuseimaeHaitou;    //�C���O�z��
         public int Haitou;             //�z��

         public override int getDate()
         {
             return this.Date;
         }

         //�ǂݍ���
         public override void Read(XmlNode inX)
         {
             if (inX.Name != "�ƐяC��") return;

             this.Date = int.Parse(inX.Attributes["���t"].InnerText);
             this.Code = int.Parse(inX.Attributes["�R�[�h"].InnerText);
             this.SyuseimaeUriage = int.Parse(inX["�C���O����"].InnerText);
             this.Uriage = int.Parse(inX["����"].InnerText);
             this.SyuseimaeKeijyourieki = int.Parse(inX["�C���O�o�험�v"].InnerText);
             this.Keijyourieki = int.Parse(inX["�o�험�v"].InnerText);
             this.SyuseimaeZeibikieki = int.Parse(inX["�C���O�ň��v"].InnerText);
             this.Zeibikieki = int.Parse(inX["�ň��v"].InnerText);
             this.SyuseimaeHaitou = int.Parse(inX["�C���O�z��"].InnerText);
             this.Haitou = int.Parse(inX["�z��"].InnerText);
         }

         public override XmlNode Write(XmlDocument ioDocument)
         {
             XmlNode element = ioDocument.CreateElement("�ƐяC��");
             element.Attributes.Append(UtilX.CreateAttribute(ioDocument, "���t", this.Date.ToString()));
             element.Attributes.Append(UtilX.CreateAttribute(ioDocument, "�R�[�h", this.Code.ToString()));
             element.AppendChild(UtilX.CreateElemnt(ioDocument, "�C���O����", this.SyuseimaeUriage.ToString()));
             element.AppendChild(UtilX.CreateElemnt(ioDocument, "����", this.Uriage.ToString()));
             element.AppendChild(UtilX.CreateElemnt(ioDocument, "�C���O�o�험�v", this.SyuseimaeKeijyourieki.ToString()));
             element.AppendChild(UtilX.CreateElemnt(ioDocument, "�o�험�v", this.Keijyourieki.ToString()));
             element.AppendChild(UtilX.CreateElemnt(ioDocument, "�C���O�ň��v", this.SyuseimaeZeibikieki.ToString()));
             element.AppendChild(UtilX.CreateElemnt(ioDocument, "�ň��v", this.Zeibikieki.ToString()));
             element.AppendChild(UtilX.CreateElemnt(ioDocument, "�C���O�z��", this.SyuseimaeHaitou.ToString()));
             element.AppendChild(UtilX.CreateElemnt(ioDocument, "�z��", this.Haitou.ToString()));

             return element;
         }
     };


    //�R�[�h�g���𑩂˂�{�X
    //�q���Ƃ��b�������Ƃ��́A�{�X�ɘb��ʂ��Ƃ����̂��X�W�Ƃ�������
    //�R�C�c�̓R�[�h(����)�P�ʂő��݂���.
    class Data
    {
        private ArrayList AccountsHash; //���Z���
        private ArrayList RevisedHash;  //�ƐяC��

        public Data()
        {
            this.AccountsHash = new ArrayList();
            this.RevisedHash = new ArrayList();
        }

        //�ǂݍ���
        public void Read(int inCode)
        {
            string filename = Util.GetExtDataFileName(inCode);

            if (!File.Exists(filename))
            {
                return;
            }
            XmlDocument document = new XmlDocument();
            document.Load(filename);
            XmlNodeList nodeList = document.DocumentElement.ChildNodes;
            foreach (XmlNode nd in nodeList)
            {
                if (nd.Name == "���Z")
                {
                    DataAccounts p = new DataAccounts();
                    p.Read(nd);
                    this.AccountsHash.Add(p);
                }
                else if (nd.Name == "�ƐяC��")
                {
                    DataRevised p = new DataRevised();
                    p.Read(nd);
                    this.RevisedHash.Add(p);
                }
            }

        }
        //��������
        public void Write(int inCode)
        {
            bool change = false;

            string filename = Util.GetExtDataFileName(inCode);
            XmlDocument document = new XmlDocument();
            document.AppendChild(document.CreateXmlDeclaration("1.0", "UTF-8", null));
            document.AppendChild(document.CreateElement("�R�[�h�g��"));

            foreach (DataAccounts p in this.AccountsHash)
            {
                XmlNode element = p.Write(document);
                document.DocumentElement.AppendChild(element);
                change = true;
            }
            foreach (DataRevised p in this.RevisedHash)
            {
                XmlNode element = p.Write(document);
                document.DocumentElement.AppendChild(element);
                change = true;
            }

            if (change)
            {
                document.Save(filename);
            }
        }

        //���Z���̏C��
        public void Update( DataAccounts inAccounts)
        {
            UpdateLaw(inAccounts, this.AccountsHash);
        }
        //�ƐяC��
        public void Update(DataRevised inRevised)
        {
            UpdateLaw(inRevised, this.RevisedHash);
        }

        //�C�����s�����ۂ̕���
        private void UpdateLaw(DataInterface inData, ArrayList inArrayList)
        {
            foreach (DataInterface p in inArrayList)
            {
                if (p.getDate() == inData.getDate() )
                {
                    inArrayList.Remove(p);
                    inArrayList.Add(inData);
                    inArrayList.Sort();
                    return;
                }
            }
            //�Ȃ��ꍇ�͒ǉ�����.
            inArrayList.Add(inData);
            inArrayList.Sort();
        }

        //�߂��f�[�^�������o��
        public DataAccounts FindAccounts(int inDate )
        {
            return (DataAccounts)FindLaw(inDate, this.AccountsHash);
        }
        //�߂��f�[�^�������o��
        public DataRevised FindRevised(int inDate )
        {
            return (DataRevised)FindLaw(inDate,this.RevisedHash);
        }

        //��ԍ����I���Ǝv���錈�Z�A�ƐяC���������o��
        private DataBasket FindFundamentalSub(int inDate)
        {
            //�ߏ�̌��Z�Ƌߏ�̋ƐяC�������߂�
            DataAccounts accounts = FindAccounts(inDate);
            DataRevised revised = FindRevised(inDate);

            DataBasket  ret = new DataBasket();

            if (accounts == null && revised == null)
            {
                return ret; //��񂪂Ȃ�!!
            }
            if (accounts == null)
            {//�ƐяC����Ԃ�
                ret.Kettusan = revised.Date;
                ret.Uriage = revised.Uriage;
                ret.Keijyourieki = revised.Keijyourieki;
                ret.Rieki = revised.Zeibikieki;
                ret.Hitoikabueki = 0;
                ret.Haitoukin = revised.Haitou;
                ret.Hitokabujyunsisan = 0;
                ret.ZenkiKettusan = 0;
                return ret;
            }
            if (revised == null)
            {//���Z����Ԃ�
                ret.Kettusan = accounts.JikiKettusan;
                ret.Uriage = accounts.JikiUriage;
                ret.Keijyourieki = accounts.JikiKeijyourieki;
                ret.Rieki = accounts.Jikirieki;
                ret.Hitoikabueki = accounts.JikiHitoikabueki;
                ret.Haitoukin = accounts.JikiHaitoukin;
                ret.Hitokabujyunsisan = accounts.Hitokabujyunsisan;
                ret.ZenkiKettusan = accounts.ZenkiKettusan;
                return ret;
            }

            if (accounts.getDate() >= revised.getDate() )
            {
                if (accounts.JikiUriage == 0 && accounts.JikiKeijyourieki == 0 && accounts.Jikirieki == 0)
                {
                    //�������Z��񂪔���!!!
                    //�d���Ȃ��̂ŁA�ƐяC����Ԃ�
                }
                else
                {
                    //���Z����Ԃ�
                    ret.Kettusan = accounts.JikiKettusan;
                    ret.Uriage = accounts.JikiUriage;
                    ret.Keijyourieki = accounts.JikiKeijyourieki;
                    ret.Rieki = accounts.Jikirieki;
                    ret.Hitoikabueki = accounts.JikiHitoikabueki;
                    ret.Haitoukin = accounts.JikiHaitoukin;
                    ret.Hitokabujyunsisan = accounts.Hitokabujyunsisan;
                    ret.ZenkiKettusan = accounts.ZenkiKettusan;

                    return ret;
                }
            }
            //�ƐяC����Ԃ�
            ret.Kettusan = revised.Date;
            ret.Uriage = revised.Uriage;
            ret.Keijyourieki = revised.Keijyourieki;
            ret.Rieki = revised.Zeibikieki;
            ret.Hitoikabueki = (accounts.JikiHitoikabueki * revised.Zeibikieki) / accounts.Konkirieki;
            ret.Haitoukin = revised.Haitou;
            //�ꊔ���Y���ƐяC���ɂȂ��̂ŁA���Z��񂩂�t�Z���ċ��߂܂��B
            //�r���ő��������ꂽ�ꍇ�A���������������Ȃ�܂��B
            ret.Hitokabujyunsisan = (accounts.Hitokabujyunsisan * revised.Zeibikieki) / accounts.Konkirieki;
            ret.ZenkiKettusan = accounts.ZenkiKettusan;

            return ret;
        }
        //�w��N���O�̈�ԍ����I���Ǝv���錈�Z�A�ƐяC���������o��
        public DataBasket FindFundamental(int inDate,int inSkip )
        {
            DataBasket NullBasket = new DataBasket();
            DataBasket basket = FindFundamentalSub(inDate);

            for (int i = 0; i < inSkip; i++)
            {
                if (basket.ZenkiKettusan == 0)
                {
                    return NullBasket;  //�������Ȃ��̂ŋ��Ԃ��܂�
                }
                basket = FindFundamentalSub( basket.ZenkiKettusan );
            }
            return basket;
        }

        //���łɊm�肵�Ă����ԍ����I���Ǝv���錈�Z�������o��
        private DataBasket FindFixedFundamentalSub(int inDate)
        {
            //�ߏ�̌��Z�Ƌߏ�̋ƐяC�������߂�
            DataAccounts accounts = FindAccounts(inDate);

            DataBasket ret = new DataBasket();

            if (accounts == null)
            {
                return ret; //��񂪂Ȃ�!!
            }
            //���Z����Ԃ�
            ret.Kettusan = accounts.KonkiKettusan;
            ret.Uriage = accounts.KonkiUriage;
            ret.Keijyourieki = accounts.KonkiKeijyourieki;
            ret.Rieki = accounts.Konkirieki;
            ret.Hitoikabueki = accounts.KonkiHitoikabueki;
            ret.Haitoukin = accounts.KonkiHaitoukin;
            ret.Hitokabujyunsisan = accounts.Hitokabujyunsisan;
            ret.ZenkiKettusan = accounts.ZenkiKettusan;

            return ret;
        }

        //���łɊm�肵�Ă���w��N���O�̈�ԍ����I���Ǝv���錈�Z�������o��
        public DataBasket FindFixedFundamental(int inDate, int inSkip)
        {
            DataBasket NullBasket = new DataBasket();
            DataBasket basket = FindFixedFundamentalSub(inDate);

            for (int i = 0; i < inSkip; i++)
            {
                if (basket.ZenkiKettusan == 0)
                {
                    return NullBasket;  //�������Ȃ��̂ŋ��Ԃ��܂�
                }
                basket = FindFixedFundamentalSub(basket.ZenkiKettusan);
            }
            return basket;
        }

        //�߂��f�[�^�������o��
        private DataInterface FindLaw(int inDate, ArrayList inArrayList)
        {
            DataInterface last = null;
            //�z��͏����Ƀ\�[�g����Ă��邽�߁A�������傫�����̂�������܂Ŕ�r����.
            foreach (DataInterface p in inArrayList)
            {
                if (inDate <= p.getDate() )
                {
                    return last;
                }
                last = p;
            }
            //�߂��f�[�^�͂Ȃ�
            return last;
        }
    };

    //���ʕ������ȁA����
    public class UtilX
    {
        public static XmlElement CreateElemnt(XmlDocument ioDocument, string inName, string inValue)
        {
            XmlElement e = ioDocument.CreateElement(inName);
            XmlText text = ioDocument.CreateTextNode(inValue);
            e.AppendChild(text);
            return e;
        }
        public static XmlAttribute CreateAttribute(XmlDocument ioDocument, string inName, string inValue)
        {
            XmlAttribute e = ioDocument.CreateAttribute(inName);
            e.Value = inValue;
            return e;
        }
        
        /*
         * //ADO ��
        //Hashtable[ArrayList(ArrayList)]���č\���ŕԂ��Ă���(w
        //Hashtable[�\�̖��O][�s][��] �Ƃ����\�����ȁA�A�A
        //�Ɉ����ȁB
        public static Hashtable XLSReader(string inFilename)
        {
            //ADO �I�u�W�F�N�g������쐬���܂��B
            ADODB.Connection connect = new ADODB.Connection();
            ADOX.Catalog catalog = new ADOX.Catalog();
            //�G�N�Z���t�@�C���ɐڑ����܂��B
            connect.Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + inFilename + ";" + " Extended Properties=Excel 8.0;","","",0);

            catalog.ActiveConnection = connect;
            Hashtable returnValue = new Hashtable();

            string tablename ; 
            for(int t = 0 ; t < catalog.Tables.Count ; t ++)
            {
                //�e�[�u��(�\)
                if ( catalog.Tables[t].Type != "TABLE" )
                {
                    continue;   //�e�[�u������Ȃ���B
                }
                //�e�[�u�������������̂ŁA�����ɃN�G��������܂��B
                tablename = catalog.Tables[t].Name;

			    //SQL�̂悤�Ȃ��� ���\�z.
			    string sql = "SELECT * FROM `" + tablename + "`";

			    //SQL�́@������ ���s
                object temp;    //�����Ԃ������炵���A�K���ɓ����
                ADODB.Recordset recordset = connect.Execute(sql, out temp, (int)ADODB.ExecuteOptionEnum.adOptionUnspecified);
    			
                //�s
                ArrayList arrayY = new ArrayList();
                for(int line = 1 ;  ! recordset.EOF ; line ++ )
                {
                    //��
                    ArrayList arrayX = new ArrayList();
                    for (int fields = 0; fields < recordset.Fields.Count; fields++)
                    {
                        arrayX.Add( recordset.Fields[fields].Value.ToString()   );
                    }
                    
                    arrayY.Add(arrayX);

                    //���̍s��
                    recordset.MoveNext();
                }

                returnValue[tablename] = arrayY;
                //�J��
                recordset.Close();
                recordset = null;
                temp = null;
            }
            connect.Close();
            connect = null;

            catalog = null;


            return returnValue;
        }
        */
        //ADO.NET��
        //Hashtable[ArrayList(ArrayList)]���č\���ŕԂ��Ă���(w
        //Hashtable[�\�̖��O][�s][��] �Ƃ����\�����ȁA�A�A
        //�Ɉ����ȁB
        public static Hashtable XLSReader(string inFilename)
        {
            //����Ɍ��ʂ������܂�.
            Hashtable returnValue = new Hashtable();

            OleDbConnection connect = new OleDbConnection("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + inFilename + ";" + " Extended Properties=Excel 8.0;");
            //�ڑ��J�n
            connect.Open();

            //�e�[�u���������߂܂�
            string tablename;
            DataTable schemaTable = connect.GetOleDbSchemaTable(OleDbSchemaGuid.Tables,
                          new Object[] { null, null, null, "TABLE" });
            foreach (DataRow schemaTableRow in schemaTable.Rows)
            {
                //�e�[�u���̖��O
                tablename = schemaTableRow["TABLE_NAME"].ToString();

                //SQL�̂悤�Ȃ��� ���\�z���܂�.
                string sql = "SELECT * FROM `" + tablename + "`";
                OleDbCommand command = connect.CreateCommand();
                command.CommandText = sql;

                OleDbDataAdapter adapter = new OleDbDataAdapter(command);
                DataSet dts = new DataSet();
                adapter.Fill(dts);

                DataTable table = dts.Tables[0];

                //�s
                ArrayList arrayY = new ArrayList();

                //ADO�Ƃ̌݊�������邽�߂Ƀw�b�_�[�����ʂɊ܂߂܂��B
                {
                    ArrayList arrayX = new ArrayList();
                    foreach (DataColumn column in table.Columns)
                    {
                        arrayX.Add(column.ToString());
                    }
                    arrayY.Add(arrayX);
                }

                foreach (DataRow row in table.Rows)
                {
                    //��
                    ArrayList arrayX = new ArrayList();
                    foreach (DataColumn column in table.Columns)
                    {
                        arrayX.Add(row[column].ToString());
                    }
                    arrayY.Add(arrayX);
                }
                returnValue[tablename] = arrayY;
            }

            connect.Close();
            connect = null; //�J���`

            return returnValue;
        }
    };
}

//------------------