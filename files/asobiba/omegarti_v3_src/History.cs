using System;
using System.Collections;
using System.Diagnostics;

using Zanetti.Data;

namespace Zanetti.Commands
{
	//�u���E�U���̐i�ށE�߂�̗�����\������ Undo/Redo�̗����Ƃ�������
	internal class BrandHistory {
		private ArrayList _back;
		private ArrayList _front;
		private const int LIMIT_COUNT = 20;

		public BrandHistory() {
			_back = new ArrayList();
			_front = new ArrayList();
		}
		public bool HasBack {
			get {
				return _back.Count>0;
			}
		}
		public bool HasFront {
			get {
				return _front.Count>0;
			}
		}
		public IEnumerable BackBrands {
			get {
				return _back;
			}
		}
		public IEnumerable FrontBrands {
			get {
				return _front;
			}
		}

		//������\��
		public void Update(AbstractBrand br) {
			AbstractBrand top = _back.Count==0? null : (AbstractBrand)_back[0];
			if(top==br) return; //���������\�����Ă��X�V���Ȃ�
			_back.Insert(0, br); //��������������͏��Ȃ��̂�OK�Ƃ���
			
			if(_back.Count>LIMIT_COUNT) _back.RemoveAt(_back.Count-1);
			_front.Clear(); //Redo�o�b�t�@�̓N���A�����
		}

		//step�߂���������Ԃ��B���ʂ͂P�����A���j���[����I�������Ƃ��͂Q�ȏ�̂��Ƃ�����
		public AbstractBrand Back(AbstractBrand current, int step) {
			for(int i=0; i<step; i++) {
				Debug.Assert(_back.Count>0);
				_front.Insert(0, current);
				current = (AbstractBrand)_back[0];
				_back.RemoveAt(0);
			}

			while(_front.Count>LIMIT_COUNT) _front.RemoveAt(_back.Count-1);
			return current;
		}
		public AbstractBrand Redo(AbstractBrand current, int step) {
			for(int i=0; i<step; i++) {
				Debug.Assert(_front.Count>0);
				_back.Insert(0, current);
				current = (AbstractBrand)_front[0];
				_front.RemoveAt(0);
			}

			while(_back.Count>LIMIT_COUNT) _back.RemoveAt(_back.Count-1);
			return current;
		}
	}
}
