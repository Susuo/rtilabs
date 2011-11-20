/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.IO;
using System.Security.Cryptography;
using System.Diagnostics;

namespace Zanetti
{
/*
 * �C�ӂ̃e�L�X�g�f�[�^�i�T�^�I�ɂ�XML�j�ɊȈՏ����@�\��ǉ�����B
 * XML Signature�݂����Ȃ̂��}�W�Ŏ�������̂͑�ςȂ̂ŁA�����̃R�����g��
 * <!-- sig:??????? -->
 * �̌`���̍s������B�����Ώۂ̓t�@�C���̐擪���炱�̍s�̒��O�̍s���܂łƂ���
 */

	internal class StreamWithSignature {

		private byte[] _rawData;

		public StreamWithSignature(string filename) {
			_rawData = new byte[(int)new FileInfo(filename).Length];
			FileStream s = new FileStream(filename, FileMode.Open, FileAccess.Read);
			s.Read(_rawData, 0, _rawData.Length);
			s.Close();
		}
		public MemoryStream AsStream() {
			return new MemoryStream(_rawData, false);
		}
		public byte[] SHA1Hash {
			get {
				int e = FindEndOfBody();
				//Debug.WriteLine("Signature Offset " + e);
				if(e==-1)
					return null;
				else
					return new SHA1CryptoServiceProvider().ComputeHash(_rawData, 0, e);
			}
		}

		private int FindEndOfBody() {
			//utf16���Ƃ���ł͂��߂�����
			byte[] targets = System.Text.Encoding.ASCII.GetBytes("<!-- sig:");
			int c = targets.Length-1;
			for(int i=_rawData.Length-1; i>=0; i--) {
				byte d = _rawData[i];
				if(d==targets[c]) {
					if(c==0) return i;
					else c--;
				}
				else
					c = targets.Length-1;
			}
			return -1;

		}

	}
}
