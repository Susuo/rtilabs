/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Security.Cryptography;
using Travis.PKI;
using Travis.Util;

namespace Zanetti
{
	/*
	 * ���݂̎d�l�́A
	 * (a) �o�[�W����(1byte)
	 * (b) ���[�U���̒���(1byte)
	 * (c) ���[�U��(utf-8, (b)�̒l�̒���)
	 * (d) (a)-(c)��MD5�n�b�V��-X(16byte)�̏�����(16byte)
	 * 
	 * ������128�r�b�g��RSA�ɂ��B�{��128�ł͑S�R�s�������A����قǌ��d�ɂ��킯�ł͂Ȃ��̂ł��̒��x�ŏ\��
	 * MD5�n�b�V��-X�͊Ȉ�mod�ŁA�擪�o�C�g��0x02�ɌŒ肵�����́B
	 */

	internal class SimpleUserKey
	{
		private string _userName;
		private bool _valid;

		public string UserName {
			get {
				return _userName;
			}
			set {
				_userName = value;
			}
		}
		public bool IsValid {
			get {
				return _valid;
			}
		}

		public SimpleUserKey() {
		}

		public string ComputeSignedString() {
			byte[] t = Encoding.UTF8.GetBytes(_userName);
			byte[] buf = new byte[t.Length+2+32];
			buf[0] = 1;
			buf[1] = (byte)t.Length;
			Array.Copy(t, 0, buf, 2, t.Length);
			byte[] hash = new MD5CryptoServiceProvider().ComputeHash(buf, 0, buf.Length-32);

			StreamReader re = new StreamReader(Env.GetAppDir()+"privatekey.txt");
			BigInteger d = new BigInteger(re.ReadLine(), 16);
			BigInteger p = new BigInteger(re.ReadLine(), 16);
			BigInteger q = new BigInteger(re.ReadLine(), 16);
			BigInteger u = new BigInteger(re.ReadLine(), 16);

			RSAPublicKey pub = ZPublicKey.PubKeyForExtensionKit;
			RSAKeyPair kp = new RSAKeyPair(pub.Exponent, d, pub.Modulus, u, p, q);
			hash[0] = 0x02;
			byte[] signature = kp.Sign(hash);
			Debug.Assert(signature.Length==32);
			kp.Verify(signature, hash);
			
			Array.Copy(signature, 0, buf, buf.Length-32, signature.Length);

			return Encoding.ASCII.GetString(Base64.Encode(buf));
		}

		public void Import(string code) {
			byte[] rawdata = Base64.Decode(Encoding.ASCII.GetBytes(code));
			if(rawdata.Length<35) throw new FormatException("signature string is too short");
			if(rawdata[0]!=1) throw new FormatException("unsupported signature version");
			
			byte[] hash = new MD5CryptoServiceProvider().ComputeHash(rawdata, 0, rawdata.Length-32);
			hash[0] = 2;
			RSAPublicKey pub = ZPublicKey.PubKeyForExtensionKit;
			byte[] sig = new byte[32];
			Array.Copy(rawdata, rawdata.Length-32, sig, 0, 32);
			pub.Verify(sig, hash);

			_userName = Encoding.UTF8.GetString(rawdata, 2, rawdata[1]);
			_valid = true;
		}
			
		public static SimpleUserKey Load(string code) {
			SimpleUserKey key = new SimpleUserKey();
			if(code!=null && code.Length>0) {
				try {
					key.Import(code);
				}
				catch(Exception ex) {
					Util.ReportCriticalError(ex);
				}
			}
			return key;
		}
	}
}
