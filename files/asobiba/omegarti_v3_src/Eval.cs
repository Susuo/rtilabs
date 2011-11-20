/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.Diagnostics;

using Zanetti.Data;
using Zanetti.Indicators;
using Zanetti.Arithmetic.Series;

namespace Zanetti.Arithmetic
{
	internal class EvalResult {
		private enum T {
			Double,
			TimeSeries,
			Bool
		}
		private T _type;
		private double _doubleVal;
		private TimeSeries _timeSeries;
		private bool _boolVal;

		public EvalResult(double value) {
			_doubleVal = value;
			_type = T.Double;
		}
		public EvalResult(TimeSeries s) {
			_timeSeries = s;
			_type = T.TimeSeries;
		}
		public EvalResult(bool value) {
			_boolVal = value;
			_type = T.Bool;
		}
		public EvalResult(EvalResult s) {
			_type = s._type;
			_doubleVal = s._doubleVal;
			_timeSeries = s._timeSeries;
			_boolVal = s._boolVal;
		}

		public double DoubleVal {
			get {
				return _doubleVal;
			}
			set {
				_doubleVal = value;
				_type = T.Double;
			}
		}
		public TimeSeries TimeSeriesVal {
			get {
				return _timeSeries;
			}
		}
		public bool BoolVal {
			get {
				return _boolVal;
			}
			set {
				_boolVal = value;
				_type = T.Bool;
			}
		}

		public bool IsDouble {
			get {
				return _type==T.Double;
			}
		}
		public bool IsTimeSeries {
			get {
				return _type==T.TimeSeries;
			}
		}
		public bool IsBool {
			get {
				return _type==T.Bool;
			}
		}

		public string ToString(string fmt) {
			switch(_type) {
				case T.Bool:
					return _boolVal.ToString();
				case T.TimeSeries:
					return "timeseries";
				default:
					return _doubleVal.ToString(fmt);
			}
		}

	}

	internal class Evaluator : BasicVisitor {
		protected DataFarm _farm;
		protected int _baseIndex;
		protected EvalResult[] _args;

		public Evaluator(string fn)  : base(fn) {
		}
		public DataFarm Farm {
			get {
				return _farm;
			}
			set {
				_farm = value;
			}
		}
		public int BaseIndex {
			get {
				return _baseIndex;
			}
			set {
				_baseIndex = value;
			}
		}
		public EvalResult[] Args {
			get {
				return _args;
			}
			set {
				_args = value;
			}
		}
		
		public EvalResult Eval(Expression expr) {
			return (EvalResult)expr.Apply(this);
		}
		public EvalResult[] Eval(Expression[] exprs) {
			if(exprs==null) return null;
			EvalResult[] r = new EvalResult[exprs.Length];
			for(int i=0; i<r.Length; i++) r[i] = (EvalResult)exprs[i].Apply(this);
			return r;
		}

		public void EnsureArgIndex(int index) {
			EvalResult[] newval = new EvalResult[Math.Max(index+1, _args==null? 0 : _args.Length)];
			if(_args!=null) {
				for(int i=0; i<_args.Length; i++)
					newval[i] = _args[i];
			}
			_args = newval;
		}

		public override object Constant(ConstantExpression expr) {
			return new EvalResult(expr.Value);
		}
		public override object Parameter(ParameterExpression expr) {
			if(_args==null || _args.Length<=expr.Index)
				ThrowArithmeticException(String.Format("�p�����[�^�̃C���f�b�N�X {0} ���������͈͂ɂ���܂���B", expr.Index));
			return new EvalResult(_args[expr.Index]);
		}

		public override object Primitive(PrimitiveExpression expr) {
			if(expr.Args==null || expr.Args.Length==0) { //�����Ȃ��v���~�e�B�u�@_baseIndex�̓��t�̒l��Ԃ�
				TradeData td = _farm.GetByIndex(_baseIndex);
				double r = 0;
				switch(expr.Type) {
					case PrimitiveIndicator.Open:
						r = td.Open;
						break;
					case PrimitiveIndicator.Close:
						r = td.Close;
						break;
					case PrimitiveIndicator.High:
						r = td.High;
						break;
					case PrimitiveIndicator.Low:
						r = td.Low;
						break;
					case PrimitiveIndicator.Volume:
						r = td.Volume;
						break;
					case PrimitiveIndicator.CreditLong:
						r = td.CreditLong;
						break;
					case PrimitiveIndicator.CreditShort:
						r = td.CreditShort;
						break;
				}
				return new EvalResult(r);
			}
			else { //��������F���Ԃ�TimeSeries��Ԃ�
				int start, end;
				if(expr.Args.Length==1) {
					EvalResult r1 = (EvalResult)expr.Args[0].Apply(this);
					start = _baseIndex - (int)r1.DoubleVal + 1; //!!doubleVal�ł��邱�Ƃ̃`�F�b�N
					end = _baseIndex + 1;
				}
				else if(expr.Args.Length==2) {
					EvalResult r1 = (EvalResult)expr.Args[0].Apply(this);
					EvalResult r2 = (EvalResult)expr.Args[1].Apply(this);
					start = _baseIndex+1 - (int)r1.DoubleVal; //!!doubleVal�ł��邱�Ƃ̃`�F�b�N�A�͈̓`�F�b�N �Q�����̂Ƃ��͕��̒l���A���ł�����Ƃ���炵������
					end = _baseIndex+1 - (int)r2.DoubleVal;
				}
				else {
					ThrowArithmeticException("argument count mismatch");
					return null;
				}

				return new EvalResult(new IndicatorTimeSeries(_farm, Env.CurrentIndicators.GetPrimitive(expr.Type), start, end));
			}
		}

		public override object BinaryOp(BinaryOpExpression expr) {
			if(expr.Type==BinaryOpExpressionType.And || expr.Type==BinaryOpExpressionType.Or || expr.Type==BinaryOpExpressionType.Predication) {
				//�V���[�g�J�b�g�]���\
				return BinaryOpLogical(expr);
			}
			else {
				//�V���[�g�J�b�g��r�͕s��
				EvalResult r1 = (EvalResult)expr.Left.Apply(this);
				EvalResult r2 = (EvalResult)expr.Right.Apply(this);
				if(expr.Type==BinaryOpExpressionType.Plus || expr.Type==BinaryOpExpressionType.Minus)
					return BinaryOpAddSub(expr, r1, r2);
				else if(expr.Type==BinaryOpExpressionType.Multiply || expr.Type==BinaryOpExpressionType.Divide)
					return BinaryOpMulDiv(expr, r1, r2);
				else //����ȊO�͔�r���Z
					return BinaryOpComp(expr, r1, r2);
			}
		}
		private EvalResult BinaryOpLogical(BinaryOpExpression expr) {
			EvalResult r1 = (EvalResult)expr.Left.Apply(this);
			Debug.Assert(r1.IsBool);
			bool v1 = r1.BoolVal;
			if(expr.Type==BinaryOpExpressionType.And) {
				if(!v1) {
					r1.BoolVal = false; //�I�u�W�F�N�g�����ʍ팸�e�N
					return r1;
				}
				else
					return (EvalResult)expr.Right.Apply(this);
			}
			else if(expr.Type==BinaryOpExpressionType.Or) {
				if(v1) {
					r1.BoolVal = true;
					return r1;
				}
				else
					return (EvalResult)expr.Right.Apply(this);
			}
			else {
				Debug.Assert(expr.Type==BinaryOpExpressionType.Predication);
				if(!v1) {
					r1.BoolVal = true;
					return r1;
				}
				else
					return (EvalResult)expr.Right.Apply(this);
			}
		}
		private EvalResult BinaryOpAddSub(BinaryOpExpression expr, EvalResult r1, EvalResult r2) {
			//���l���m�A�T�C�Y�̓�����TimeSeries���m�ŉ��Z�\
			if(r1.IsDouble && r2.IsDouble) {
				if(expr.Type==BinaryOpExpressionType.Plus)
					r1.DoubleVal += r2.DoubleVal;
				else
					r1.DoubleVal -= r2.DoubleVal;
				return r1;
			}
			else if(r1.IsTimeSeries && r2.IsTimeSeries) {
				if(expr.Type==BinaryOpExpressionType.Plus)
					r1 = new EvalResult(new BinOpTimeSeries(r1.TimeSeriesVal, r2.TimeSeriesVal, BinOpTimeSeries.ADD));
				else
					r1 = new EvalResult(new BinOpTimeSeries(r1.TimeSeriesVal, r2.TimeSeriesVal, BinOpTimeSeries.SUB));
				return r1;
			}
			else if(r1.IsTimeSeries && r2.IsDouble) {
				r1 = new EvalResult(new SingleOpTimeSeries(r1.TimeSeriesVal, SingleOpTimeSeries.CreateSlided(expr.Type==BinaryOpExpressionType.Plus? r2.DoubleVal : -r2.DoubleVal)));
				return r1;
			}
			else if(r2.IsTimeSeries && r1.IsDouble) {
				r2 = new EvalResult(new SingleOpTimeSeries(r2.TimeSeriesVal, SingleOpTimeSeries.CreateSlided(expr.Type==BinaryOpExpressionType.Plus? r1.DoubleVal : -r1.DoubleVal)));
				return r2;
			}
			else
				throw new Exception("type mismatch(add/sub)");
		}
		private EvalResult BinaryOpMulDiv(BinaryOpExpression expr, EvalResult r1, EvalResult r2) {
			//���l���m�ATimeSeries�Ɛ��l�ŉ��Z�\
			if(r1.IsDouble && r2.IsDouble) {
				if(expr.Type==BinaryOpExpressionType.Multiply)
					r1.DoubleVal *= r2.DoubleVal;
				else
					r1.DoubleVal /= r2.DoubleVal;
				return r1;
			}
			else if(r1.IsTimeSeries && r2.IsDouble) {
				r1 = new EvalResult(new SingleOpTimeSeries(r1.TimeSeriesVal,
					SingleOpTimeSeries.CreateScalar(expr.Type==BinaryOpExpressionType.Multiply? r2.DoubleVal : 1/r2.DoubleVal)));
				return r1;
			}
			else if(r1.IsTimeSeries && r2.IsTimeSeries) {
				if(r1.TimeSeriesVal.Count!=r2.TimeSeriesVal.Count) throw new Exception("timeseries length mismatch");
				return new EvalResult(new BinOpTimeSeries(r1.TimeSeriesVal, r2.TimeSeriesVal, expr.Type==BinaryOpExpressionType.Multiply? BinOpTimeSeries.MUL : BinOpTimeSeries.DIV));
			}
			else
				throw new Exception("type mismatch(mul/div)");
		}

		private EvalResult BinaryOpComp(BinaryOpExpression expr, EvalResult r1, EvalResult r2) {
			//���l���m�̂�
			if(r1.IsDouble && r2.IsDouble) {
				double v1 = r1.DoubleVal;
				double v2 = r2.DoubleVal;
				bool result = false;
				switch(expr.Type) {
					case BinaryOpExpressionType.Eq:
						result = (v1==v2);
						break;
					case BinaryOpExpressionType.NotEq:
						result = (v1!=v2);
						break;
					case BinaryOpExpressionType.Smaller:
						result = (v1<v2);
						break;
					case BinaryOpExpressionType.SmallerEq:
						result = (v1<=v2);
						break;
					case BinaryOpExpressionType.Greater:
						result = (v1>v2);
						break;
					case BinaryOpExpressionType.GreaterEq:
						result = (v1>=v2);
						break;
				}
				r1.BoolVal = result;
				return r1;
			}
			else
				throw new Exception("type mismatch(bin-op)");
		}

		public override object UnaryOp(UnaryOpExpression expr) {
			Debug.Assert(expr.Type==UnaryOpExpressionType.Inverse);
			EvalResult e = (EvalResult)expr.Content.Apply(this);
			if(e.IsBool)
				e.BoolVal = !e.BoolVal;
			else if(e.IsDouble)
				e.DoubleVal = -e.DoubleVal;
			else
				e = new EvalResult(new SingleOpTimeSeries(e.TimeSeriesVal, SingleOpTimeSeries.NEG));

			return e;
		}

		public override object Function(FunctionExpression expr) {
			if(expr.Category==FunctionExpression.FECategory.Unknown) { //���O�̉�������B�g�ݍ��݃��C�u�����ƒ�`�ς�Indicator�̂ǂ��炩�łȂ��Ƃ����Ȃ�
				int i = Env.ArithmeticLibrary.FindLaneID(expr.Name);
				if(i!=-1) {
					expr.Category = FunctionExpression.FECategory.Library;
					expr.LaneID = i;
				}
				else {
					Indicator ind = Env.CurrentIndicators.FindFromAscii(expr.Name);
					if(ind!=null) {
						expr.Category = FunctionExpression.FECategory.Indicator;
						expr.LaneID = ind.LaneID;
					}
					else
						ThrowArithmeticException(expr.Name + " �͒�`����Ă��܂���B");
				}
			}

			if(expr.Category==FunctionExpression.FECategory.Library) 
				return Env.ArithmeticLibrary.Calc(expr.LaneID, this, expr.Args);
			else {
				ExpressionIndicator ei = (ExpressionIndicator)Env.CurrentIndicators.GetIndicator(expr.LaneID);
				if(ei.ArgCount==0)
					return new EvalResult(_farm.GetByIndex(_baseIndex).GetValue(ei));
				else { //�����O�̏ꍇ�̓L���b�V�����L���ɂȂ邪�A�����łȂ��Ƃ��͍Čv�Z�B�ꉞ�����̒��g���`�F�b�N���邱�ƂŃL���b�V�����쐬���邱�Ƃ��ł��邪�ʓ|�Ȃ̂ł�߂�
					EvalResult[] t = new EvalResult[ei.ArgCount];
					//�����O�̂Ƃ��͎Q�Ɛ�̃p�����[�^�����̂܂܎g��
					if(expr.Args==null || expr.Args.Length==0)
						t = ei.Args;
					else {
						if(ei.ArgCount!=expr.Args.Length)
							ThrowArithmeticException(String.Format("{0} �̈����� {1} �łȂ���΂Ȃ�܂��񂪁A{2} �ł��B", expr.Name, ei.Args.Length, expr.Args.Length));
						for(int i=0; i<t.Length; i++)
							t[i] = (EvalResult)expr.Args[i].Apply(this);
					}

					//�p�����[�^��u�����čĎ��s
					EvalResult[] saved = _args;
					_args = t;
					EvalResult r = (EvalResult)ei.Expression.Apply(this);
					_args = saved;
					return r;
				}
			}
		}

	}
}
