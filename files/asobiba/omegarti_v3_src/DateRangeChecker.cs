/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Diagnostics;

using Zanetti.Indicators;

namespace Zanetti.Arithmetic
{
	//����Expression���L���ɂȂ邽�߂ɕK�v�ȓ��t�͈͂�\������B
	//���Ƃ��� begin=-4, end=1�̂Ƃ��́A�ߋ��T���Ԃ��K�v�ł��邱�Ƃ��Ӗ�����B
	//����0�̂Ƃ��͏�ɗL���ɂȂ�B
	internal class DateRange {
		public int begin;
		public int end;

		public static DateRange Empty {
			get {
				DateRange r = new DateRange();
				r.begin = 0;
				r.end = 0;
				return r;
			}
		}
		public static DateRange Today {
			get {
				DateRange r = new DateRange();
				r.begin = 0;
				r.end = 1;
				return r;
			}
		}
		public DateRange Merge(DateRange r) {
			if(r==null || (r.begin==0 && r.end==0)) return this;
			if(begin==0 && end==0) {
				begin = r.begin;
				end = r.end;
			}
			else {
				begin = Math.Min(begin, r.begin);
				end = Math.Max(end, r.end);
			}
			return this;
		}
	}

	/// <summary>
	/// Expression��������t����̑��Βl�łǂ̓��t�͈͂�K�v�Ƃ��Ă��邩��]������
	/// </summary>
	internal class DateRangeChecker : BasicVisitor {
		private EvalResult[] _args;
		public DateRangeChecker(string fn, EvalResult[] args) : base(fn) {
			_args = args;
		}
		public EvalResult[] Args {
			get {
				return _args;
			}
		}
		protected virtual Evaluator CreateEvaluator(string name) {
			return new Evaluator(name);
		}
		
		public override object Primitive(PrimitiveExpression expr) {
			if(expr.Args==null || expr.Args.Length==0) {
				return DateRange.Today;
			}
			else {
				if(expr.Args.Length==1) {
					DateRange r = DateRange.Today;
					r.begin = -EvalPrimitiveExpressionArgs(expr.Args[0]) + 1; 
					return r;
				}
				else if(expr.Args.Length==2) {
					DateRange r = DateRange.Today;
					r.begin = -EvalPrimitiveExpressionArgs(expr.Args[0]) + 1; 
					r.end   = -EvalPrimitiveExpressionArgs(expr.Args[1]) + 1; 
					return r;
				}
				else {
					ThrowArithmeticException("argument count mismatch");
					return null;
				}
			}
		}
		private int EvalPrimitiveExpressionArgs(Expression ex) {
			Evaluator ev = CreateEvaluator(_functionName);
			ev.Args = _args;
			//BaseIndex, Farm�͕s�v�Ȃ͂�
			return (int)((EvalResult)ex.Apply(ev)).DoubleVal;
		}

		public override object Function(FunctionExpression expr) {
			int i;
			//������Indicator namespace���K�v�ɂȂ�͈̂Ӗ��I�ɂ�����Ƃ�������
			ExpressionIndicator ind = Env.CurrentIndicators.FindFromAscii(expr.Name) as ExpressionIndicator;
			if(ind!=null) {
				//�����O�̂Ƃ��̓f�t�H���g�̃p�����[�^���g���̂ň����̌��`�F�b�N�����Ȃ�
				if(expr.Args==null || expr.Args.Length==0) {
					return (DateRange)ind.Expression.Apply(new DateRangeChecker(_functionName, ind.Args));
				}
				else {
					if(ind.ArgCount!=Util.SafeArgLength(expr.Args))
						ThrowArithmeticException(String.Format("{0} �̈����� {1} �łȂ���΂Ȃ�܂��񂪁A{2} �ł��B", expr.Name, ind.ArgCount, Util.SafeArgLength(expr.Args)));

					EvalResult[] p = null;
					if(expr.Args!=null) {
						p = new EvalResult[expr.Args.Length];
						for(i=0; i<p.Length; i++)
							p[i] = new EvalResult((double)EvalPrimitiveExpressionArgs(expr.Args[i]));
					}
					return (DateRange)ind.Expression.Apply(new DateRangeChecker(_functionName, p));
				}
			}
			else if((i = Env.ArithmeticLibrary.FindLaneID(expr.Name))!=-1) {
				Evaluator ev = CreateEvaluator(_functionName);
				ev.Args = _args;
				return Env.ArithmeticLibrary.CalcDateRange(i, this, ev, expr.Args);	
			}
			else
				return null;
		}
		public override object Constant(ConstantExpression expr) {
			return DateRange.Empty;
		}

		public override object Parameter(ParameterExpression expr) {
			return DateRange.Empty;
		}
		public override object UnaryOp(UnaryOpExpression expr) {
			return expr.Content.Apply(this);
		}
		public override object BinaryOp(BinaryOpExpression expr) {
			DateRange r = (DateRange)expr.Left.Apply(this);
			if(r==null)
				r = (DateRange)expr.Right.Apply(this);
			else
				r.Merge((DateRange)expr.Right.Apply(this));
			return r;
		}

		//�����Evaluator�Ɠ���
		public void EnsureArgIndex(int index) {
			EvalResult[] newval = new EvalResult[Math.Max(index+1, _args==null? 0 : _args.Length)];
			if(_args!=null) {
				for(int i=0; i<_args.Length; i++)
					newval[i] = _args[i];
			}
			_args = newval;
		}

	}
}
