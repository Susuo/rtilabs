/*
 * Copyright (c) Daisuke OKAJIMA    All rights reserved.
 * 
 * $Id$
 */
using System;
using System.IO;
using System.Diagnostics;
using System.Drawing;
using System.Text;

using Travis.Storage;
using Travis.Util;

using Zanetti.Indicators;

namespace Zanetti.UI
{
	internal class ChartFormatSpecificValue {
		private int[] _data;
		private const int LENGTH = 3;

		public ChartFormatSpecificValue(int[] t) {
			if(t.Length==LENGTH)
				_data = t; //�����͂���
			else {
				_data = new int[LENGTH];
				for(int i=0; i<LENGTH; i++) _data[i] = t.Length>i? t[i] : 0;
			}
		}
		public ChartFormatSpecificValue(string value) {
			string[] t = value.Split(',');
			_data = new int[LENGTH];
			for(int i=0; i<LENGTH; i++)
				_data[i] = t.Length>i? Int32.Parse(t[i]) : 0;
		}

		public int GetValue(ChartFormat fmt) {
#if DOJIMA
			if(fmt==ChartFormat.HalfDaily) fmt = ChartFormat.Daily;
#endif
			int i = (int)fmt;
			return i<_data.Length? _data[i] : 0;
		}
		public void Update(int[] t) {
			Debug.Assert(t.Length==LENGTH);
			_data = t;
		}

		public override string ToString() {
			StringBuilder b = new StringBuilder();
			for(int i=0; i<LENGTH; i++) {
				if(i>0) b.Append(",");
				b.Append(_data[i].ToString());
			}
			return b.ToString();
		}
	}

	internal class OscillatorPreference {
		//�ő�R��
		public const int LENGTH = 3;

		//Group�����݂��Ă��Ă��ꎞ�I�ɕ\������Ȃ��Ƃ����`�Ԃ�z�肵��_visible�𓱓�
		private OscillatorGroup _group;
		private string _id;
		private HeightConfig _config;
		private double[] _scaleValues;
		private Trans _trans;

		public OscillatorPreference(string id) {
			string[] t = id.Split(':');
			_config = Util.ParseHeightConfig(t[0], HeightConfig.None);
			_id = t.Length>1? t[1] : "none";
			_scaleValues = new double[3];
		}

		public OscillatorGroup OscillatorGroup {
			get {
				if(_group==null) Bind();
				return _group;
			}
			set {
				_group = value;
				if(_group!=null) _id = value.Name;
			}
		}
		public HeightConfig Config {
			get {
				if(_group==null) Bind();
				if(_group==null)
					return HeightConfig.None;
				else
					return _config; //�N������ɂ�Group�����݂��Ȃ����Ƃ�����
			}
			set {
				_config = value;
			}
		}
		private void Bind() {
			_group = Env.CurrentIndicators.FindOscillatorGroup(_id);
		}
		public string Format() {
			return String.Format("{0}:{1}", _config.ToString(), _group==null? "none" : _id);
		}

		public double[] ScaleValues {
			get {
				return _scaleValues;
			}
		}
		public void SetScaleValues(double v0, double v1, double v2) {
			_scaleValues[0] = v0;
			_scaleValues[1] = v1;
			_scaleValues[2] = v2;
		}
		public Trans Trans {
			get {
				return _trans;
			}
			set {
				_trans = value;
			}
		}

		//�X�L�[�}�̃����[�h�ȂǁA�ǂݍ��݂Ȃ������Ƃ��͎Q�Ƃ�؂��Ă����K�v������
		public void Refresh() {
			_group = null;
		}
	}


	[EnumDesc(typeof(MouseTrackingLineMode))]
	internal enum MouseTrackingLineMode {
		[EnumValue(Description="�Ȃ�")] None,
		[EnumValue(Description="���t�̂�")] Date,
		[EnumValue(Description="���t�Ɖ��i")]Full
	}

	//�o�����ƃI�V���[�^�̍����ݒ�
	internal enum HeightConfig {
		None,
		Large,
		Middle,
		Small
	}

	/// <summary>
	/// Preference�́A���[�U���J�X�^�}�C�Y�\�ȕ\���Ɋւ���ݒ�����^����BOption�̈ꕔ�Ƃ������邪�A
	/// ���C�g�o�C���f�B���O����GDI�I�u�W�F�N�g�Ȃǂ�����
	/// </summary>
	internal class Preference
	{
		//�F�֌W
		private ZBrush _defaultBrush;
		private ZPen _defaultPen;

		private ZBrush _backBrush;
		private ZBrush _volumeBrush;
		private ZCandlePen _candlePen;
		private ZBrush _insenBrush;
		private ZPen  _monthDivPen;
		private ZPen  _mouseTrackingLinePen;

        //hacked by rti
        //���������ŊJ�n�ƏI���ɐ���������̃y��
		private ZPen  _autoTradingStartLinePen;
		private ZPen  _autoTradingEndLinePen;
        //������������ �������z
        private Int64 _InitCach;
        //���ԕ��ώ��v�������߂����
        private int _RTISisuuHeikinKikan;
        //�{���������ɉ񂳂�����z
        private Int64 _TodayBuyCach;
        //--------------------


		private MouseTrackingLineMode _mouseTrackingLineMode;
		private ZPen  _priceScalePen;
		private ZPen  _volumeScalePen;
		private ZPen  _oscillatorScalePen;
		private Color _fushiColor;
		private Color _freeLineColor;

		private IndicatorAppearance _creditLongAppearance;
		private IndicatorAppearance _creditShortAppearance;
		
		// ���[�\�N��
		private int _candleWidth;

		//�\���ݒ�n
		private bool _useCandleEffect;
		private bool _inverseChart;	// �㉺�t
		private bool _logScale;     // �ΐ��\��
		private bool _showPrice;
		private HeightConfig _showVolume;
		private bool _showAccumulativeVolume; //���i�ѕʏo����
		private bool _adjustSplit; //�����𒲐����邩�ǂ���
		private bool _scaleLock;

		//font
		private string _fontName;
		private float  _fontSize;
		private FontStyle _fontStyle;
		
		//���C�g�o�C���h���镨
		private bool _fontDirty;
		private SizeF _defaultFontPitch;
		private SizeF _headerFontPitch;
		private Font _defaultFont;
		private Font _headerFont;
		private IntPtr _defaultHFont;
		private IntPtr _headerHFont;
		private ZPen _freeLinePen;
		private ZPen _freeLineDottedPen;
		private ZPen _freeLineBoldPen;

		//�����Ԃ̍��l�E���l��߂Ƃ��邩
		private int _fushiRange;

		//���i�ѕʏo����
		private ChartFormatSpecificValue _accumulativeVolumePeriod;

		//�I�V���[�^
		private OscillatorPreference[] _oscillatorPreferences;

		public Color FushiColor {
			get {
				return _fushiColor;
			}
			set {
				_fushiColor = value;
			}
		}
		public string FontName {
			get {
				return _fontName;
			}
			set {
				_fontName = value;
				_fontDirty = true;
			}
		}
		public float FontSize {
			get {
				return _fontSize;
			}
			set {
				_fontSize = value;
				_fontDirty = true;
			}
		}
		public FontStyle FontStyle {
			get {
				return _fontStyle;
			}
			set {
				_fontStyle = value;
				_fontDirty = true;
			}
		}		
		public Color TextColor 
		{
			get {
				return _defaultBrush.Color;
			}
		}

		public ZBrush DefaultBrush {
			get {
				return _defaultBrush;
			}
		}
		public ZPen DefaultPen {
			get {
				return _defaultPen;
			}
		}
		public ZBrush BackBrush {
			get {
				return _backBrush;
			}
		}
		public ZBrush InsenBrush {
			get {
				return _insenBrush;
			}
		}
		public ZCandlePen CandlePen {
			get {
				return _candlePen;
			}
		}
		public ZBrush VolumeBrush {
			get {
				return _volumeBrush;
			}
		}
		public ZPen MonthDivPen {
			get {
				return _monthDivPen;
			}
		}
		public ZPen MouseTrackingLinePen {
			get {
				return _mouseTrackingLinePen;
			}
		}

        //hacked by rti----------------
        //���������ŊJ�n�Ɉ������C���̃y��
        public ZPen AutoTradingStartLinePen
        {
            get
            {
                return _autoTradingStartLinePen;
            }
        }
        //���������ŏI���Ɉ������C���̃y��
        public ZPen AutoTradingEndLinePen
        {
            get
            {
                return _autoTradingEndLinePen;
            }
        }
        //���������������z
        public Int64 InitCach
        {
            get
            {
                return _InitCach;
            }
            set
            {
                _InitCach = value;
            }
        }
        //���ԕ��ώ��v�����߂����
        public int RTISisuuHeikinKikan
        {
            get
            {
                return _RTISisuuHeikinKikan;
            }
            set
            {
                _RTISisuuHeikinKikan = value;
            }
        }
        //�{���������ɉ񂳂�����z
        public Int64 TodayBuyCach
        {
            get
            {
                return _TodayBuyCach;
            }
            set
            {
                _TodayBuyCach = value;
            }
        }
        //-------------------------------------


		public ZPen PriceScalePen {
			get {
				return _priceScalePen;
			}
		}
		public ZPen VolumeScalePen {
			get {
				return _volumeScalePen;
			}
		}
		public ZPen OscillatorScalePen {
			get {
				return _oscillatorScalePen;
			}
		}

		public int FushiRange {
			get {
				return _fushiRange;
			}
		}
		public IndicatorAppearance CreditLongAppearance {
			get {
				return _creditLongAppearance;
			}
		}
		public IndicatorAppearance CreditShortAppearance {
			get {
				return _creditShortAppearance;
			}
		}
		public MouseTrackingLineMode MouseTrackingLineMode {
			get {
				return _mouseTrackingLineMode;
			}
		}

		// ���[�\�N����
		public bool InverseChart {
			get {
				return _inverseChart;
			}
			set {
				_inverseChart = value;
			}		
		}
		public bool LogScale{
			get {
				return _logScale;
			}
			set {
				_logScale = value;
			}		
		}
		public bool AdjustSplit {
			get {
				return _adjustSplit;
			}
			set {
				_adjustSplit = value;
			}
		}
		public bool ShowPrice {
			get {
				return _showPrice;
			}
			set {
				_showPrice = value;
			}
		}
		public HeightConfig ShowVolume {
			get {
				return _showVolume;
			}
			set {
				_showVolume = value;
			}
		}
		public bool ShowAccumulativeVolume {
			get {
				return _showAccumulativeVolume;
			}
			set {
				_showAccumulativeVolume = value;
			}
		}

		public bool ScaleLock {
			get {
				return _scaleLock;
			}
			set {
				_scaleLock = value;
			}
		}
		public bool UseCandleEffect {
			get {
				return _useCandleEffect;
			}
			set {
				_useCandleEffect = value;
			}
		}

		//���E�\�N���B��łȂ��Ƃ���
		public int CandleWidth {
			get {
				return _candleWidth;
			}
			set {
				Debug.Assert((_candleWidth & 1)==1);
				_candleWidth = value;
			}
		}
		//CandleWidth����̓��o
		public int DatePitch {
			get {
				return _candleWidth * 5 / 3; //���E�\�N�̌��Ԃ��󂯂邽�߂��ꂭ�炢
			}
		}
		public int HalfCandleWidth {
			get {
				return (_candleWidth+1)>>1;
			}
		}

		//���R�����n
		public Color FreeLineColor {
			get {
				return _freeLineColor;
			}
			set {
				_freeLineColor = value;
				_freeLinePen = null;
				_freeLineDottedPen = null;
			}
		}
		public ZPen FreeLinePen {
			get {
				if(_freeLinePen==null)
					_freeLinePen = new ZPen(_freeLineColor, ZPen.PenStyle.Bold);
				return _freeLinePen;
			}
		}
		public ZPen FreeLineDottedPen {
			get {
				if(_freeLineDottedPen==null)
					_freeLineDottedPen = new ZPen(_freeLineColor, ZPen.PenStyle.Dotted);
				return _freeLineDottedPen;
			}
		}
		public ZPen FreeLineBoldPen {
			get {
				if(_freeLineBoldPen==null)
					_freeLineBoldPen = new ZPen(_freeLineColor, ZPen.PenStyle.Bold);
				return _freeLineBoldPen;
			}
		}
		public ChartFormatSpecificValue AccumulativeVolumePeriod {
			get {
				return _accumulativeVolumePeriod;
			}
			set {
				_accumulativeVolumePeriod = value;
			}
		}
		public OscillatorPreference[] OscillatorPreferences {
			get {
				return _oscillatorPreferences;
			}
		}


		public Preference(StorageNode config) {
			_fontName = LoadString(config, "font-name", "�l�r ����");
			_fontSize = LoadFloat(config, "font-size", 9);
			_fontStyle = LoadFontStyle(config, "font-style", FontStyle.Regular);

			_defaultBrush = new ZBrush(LoadColor(config, "text-color", Color.White));
			_backBrush = new ZBrush(LoadColor(config, "back-color", Color.Black));
			_insenBrush = new ZBrush(LoadColor(config, "insen-color", Color.White));
			_fushiColor = LoadColor(config, "fushi-color", Color.DarkGray);
			_freeLineColor = LoadColor(config, "free-line-color", Color.Pink);
			_defaultPen = new ZPen(_defaultBrush.Color, ZPen.PenStyle.Normal);
			
			_useCandleEffect = LoadBool(config, "candle-effect", true);
			_logScale = LoadBool(config, "log-scale", false);
			_inverseChart = LoadBool(config, "inverse-chart", false);
			_adjustSplit = LoadBool(config, "adjust-split", true);
			_candleWidth = LoadInt(config, "candle-width", 9);
			_candleWidth |= 1; //��ɂ���
			if(_candleWidth<Env.Constants.MIN_CANDLE_WIDTH) _candleWidth = Env.Constants.MIN_CANDLE_WIDTH; //min
			if(_candleWidth>Env.Constants.MAX_CANDLE_WIDTH) _candleWidth = Env.Constants.MAX_CANDLE_WIDTH;
			_showPrice = LoadBool(config, "show-price", true);
			_showVolume = Util.ParseHeightConfig(LoadString(config, "show-volume", "Large"), HeightConfig.Large);
			_showAccumulativeVolume = LoadBool(config, "show-accumulative-volume", true);
			
			_candlePen = new ZCandlePen(LoadColor(config, "candle-color", Color.White), _backBrush.Color);
			_monthDivPen = new ZPen(LoadColor(config, "month-div-color", Color.OliveDrab), LoadStyle(config, "month-div-style", ZPen.PenStyle.Normal));
			_priceScalePen = new ZPen(LoadColor(config, "price-scale-color", Color.MediumSeaGreen), LoadStyle(config, "price-scale-style", ZPen.PenStyle.Dotted));
			_volumeScalePen = new ZPen(LoadColor(config, "volume-scale-color", Color.WhiteSmoke), LoadStyle(config, "volume-scale-style", ZPen.PenStyle.Dotted));
			_oscillatorScalePen = new ZPen(LoadColor(config, "oscillator-scale-color", Color.DarkRed), LoadStyle(config, "oscillator-scale-style", ZPen.PenStyle.Dotted));
			_volumeBrush = new ZBrush(LoadColor(config, "volume-color", Color.RoyalBlue));
			_mouseTrackingLineMode = (MouseTrackingLineMode)Enum.Parse(typeof(MouseTrackingLineMode), LoadString(config, "mouse-tracking-mode", "Full"));
			_mouseTrackingLinePen = new ZPen(LoadColor(config, "mouse-tracking-color", Color.LightGray), LoadStyle(config, "mouse-tracking-style", ZPen.PenStyle.Dotted));

            //hacked by rti
            //���������̊J�n�ƏI�����ɐ����������̃y���̐F
            //�f�B�t�H���g�͗����Ƃ��ɂ��Ă�����
            _autoTradingStartLinePen = new ZPen(LoadColor(config, "autotrading-startline-color", Color.Blue), LoadStyle(config, "autotrading-startline-style", ZPen.PenStyle.Dotted));
            _autoTradingEndLinePen = new ZPen(LoadColor(config, "autotrading-endline-color", Color.Blue), LoadStyle(config, "autotrading-endline-style", ZPen.PenStyle.Dotted));
            //������������ ������z
            _InitCach = LoadInt64(config, "init-cash", 10000000);
            _RTISisuuHeikinKikan = LoadInt(config, "rtisisuu-heikin-kikan", 180);
            _TodayBuyCach = LoadInt64(config, "today-buy-cach", 10000000);
            //--------------------------------------------

			_creditLongAppearance  = new IndicatorAppearance(LoadStyle(config, "creditlong-style", IndicatorStyle.Line), LoadColor(config, "creditlong-color", Color.Blue));
			_creditShortAppearance = new IndicatorAppearance(LoadStyle(config, "creditshort-style", IndicatorStyle.Line), LoadColor(config, "creditshort-color", Color.Red));

			_fushiRange = LoadInt(config, "fushi-range", 5);
			_fontDirty = true;
			
			_accumulativeVolumePeriod = new ChartFormatSpecificValue(LoadString(config, "accumulative-volume-period", "60,52,24"));
			_oscillatorPreferences = new OscillatorPreference[OscillatorPreference.LENGTH];
			for(int i=0; i<_oscillatorPreferences.Length; i++)
				_oscillatorPreferences[i] = new OscillatorPreference(LoadString(config, "oscillator-"+i, "none")); 
		}
		private string LoadString(StorageNode config, string name, string def) {
			if(config==null) return def;
			return config.GetValue(name, def);
		}
		private Color LoadColor(StorageNode config, string name, Color def) {
			if(config==null) return def;
			string cn = config.GetValue(name);
			if(cn==null)
				return def;
			else
				return Util.ParseColor(cn, def);
		}
		private FontStyle LoadFontStyle(StorageNode config, string name, FontStyle def) {
			if(config==null) return def;
			string cn = config.GetValue(name);
			if(cn==null)
				return def;
			else
				return (FontStyle)Enum.Parse(typeof(FontStyle), cn);
		}

        //hacked by rti
        private Int64 LoadInt64(StorageNode config, string name, Int64 def)
        {
            if (config == null) return def;
            return Util.ParseInt64(config.GetValue(name, ""), def);
        }
        //--------------

		private int LoadInt(StorageNode config, string name, int def) {
			if(config==null) return def;
			return Util.ParseInt(config.GetValue(name, ""), def);
		}
		private bool LoadBool(StorageNode config, string name, bool def) {
			if(config==null) return def;
			return Util.ParseBool(config.GetValue(name, ""), def);
		}
		private float LoadFloat(StorageNode config, string name, float def) {
			if(config==null) return def;
			return Util.ParseFloat(config.GetValue(name, ""), def);
		}
		private IndicatorStyle LoadStyle(StorageNode config, string name, IndicatorStyle def) {
			if(config==null) return def;
			return IndicatorAppearance.ParseStyle(config.GetValue(name, ""), def);
		}
		private ZPen.PenStyle LoadStyle(StorageNode config, string name, ZPen.PenStyle def) {
			if(config==null) return def;
			string t = config.GetValue(name, "");
			if(t=="Normal")
				return ZPen.PenStyle.Normal;
			else if(t=="Dotted")
				return ZPen.PenStyle.Dotted;
			else if(t=="Bold")
				return ZPen.PenStyle.Bold;
			else
				return def;
		}
								 

		public void SaveTo(StorageNode parent) {
			StorageNode node = new StorageNode();
			node.Name = "preference";
			node["font-name"] = _fontName;
			node["font-size"] = _fontSize.ToString();
			node["font-style"] = _fontStyle.ToString();
			node["text-color"] = Util.FormatColor(_defaultBrush.Color);
			node["back-color"] = Util.FormatColor(_backBrush.Color);
			node["insen-color"] = Util.FormatColor(_insenBrush.Color);
			node["fushi-color"] = Util.FormatColor(_fushiColor);
			node["free-line-color"] = Util.FormatColor(_freeLineColor);
			node["candle-color"] = Util.FormatColor(_candlePen.Color);
			node["month-div-color"] = Util.FormatColor(_monthDivPen.Color);
			node["month-div-style"] = _monthDivPen.Style.ToString();
			node["price-scale-color"] = Util.FormatColor(_priceScalePen.Color);
			node["price-scale-style"] = _priceScalePen.Style.ToString();
			node["volume-scale-color"] = Util.FormatColor(_volumeScalePen.Color);
			node["volume-scale-style"] = _volumeScalePen.Style.ToString();
			node["oscillator-scale-color"] = Util.FormatColor(_oscillatorScalePen.Color);
			node["oscillator-scale-style"] = _oscillatorScalePen.Style.ToString();
			node["volume-color"] = Util.FormatColor(_volumeBrush.Color);
			node["mouse-tracking-color"] = Util.FormatColor(_mouseTrackingLinePen.Color);
			node["mouse-tracking-style"] = _mouseTrackingLinePen.Style.ToString();

            //hacked by rti
            //���������̊J�n�ƏI���Ɉ����y���̐F�Ƃ���ԂƂ��̕ۑ�
            node["autotrading-startline-color"] = Util.FormatColor(_autoTradingStartLinePen.Color);
            node["autotrading-startline-style"] = _autoTradingStartLinePen.Style.ToString();
            node["autotrading-endline-color"] = Util.FormatColor(_autoTradingEndLinePen.Color);
            node["autotrading-endline-style"] = _autoTradingEndLinePen.Style.ToString();
            //�������z ��������
            node["init-cash"] = _InitCach.ToString();
            node["rtisisuu-heikin-kikan"] = _RTISisuuHeikinKikan.ToString();
            node["today-buy-cach"] = _TodayBuyCach.ToString();
            //---------------------------

			node["mouse-tracking-mode"] = _mouseTrackingLineMode.ToString();
			node["fushi-range"] = _fushiRange.ToString();
			node["creditlong-style"] = _creditLongAppearance.Pen.Style.ToString();
			node["creditlong-color"] = Util.FormatColor(_creditLongAppearance.Pen.Color);
			node["creditshort-style"] = _creditShortAppearance.Pen.Style.ToString();
			node["creditshort-color"] = Util.FormatColor(_creditShortAppearance.Pen.Color);
			node["candle-width"] = _candleWidth.ToString();
			node["candle-effect"] = _useCandleEffect.ToString();
			node["log-scale"] = _logScale.ToString();
			node["inverse-chart"] = _inverseChart.ToString();
			node["adjust-split"] = _adjustSplit.ToString();
			node["show-price"] = _showPrice.ToString();
			node["show-volume"] = _showVolume.ToString();
			node["show-accumulative-volume"] = _showAccumulativeVolume.ToString();
			node["accumulative-volume-period"] = _accumulativeVolumePeriod.ToString();
			for(int i=0; i<_oscillatorPreferences.Length; i++)
				node["oscillator-"+i] = _oscillatorPreferences[i].Format();
			parent.AddChild(node);
		}

		public void Refresh() {
			for(int i=0; i<_oscillatorPreferences.Length; i++)
				_oscillatorPreferences[i].Refresh();
		}


		/*
#define PS_SOLID            0
#define PS_DASH             1       /* -------  
#define PS_DOT              2       /* .......  
#define PS_DASHDOT          3       /* _._._._  
*/

		public IntPtr DefaultHFont {
			get {
				if(_fontDirty) CreateFont();
				return _defaultHFont;
			}
		}
		public Font DefaultFont {
			get {
				if(_fontDirty) CreateFont();
				return _defaultFont;
			}
		}
		public IntPtr HeaderHFont {
			get {
				if(_fontDirty) CreateFont();
				return _headerHFont;
			}
		}
		public Font HeaderFont {
			get {
				if(_fontDirty) CreateFont();
				return _headerFont;
			}
		}
		public SizeF DefaultCharPitch {
			get {
				if(_fontDirty) CreateFont();
				return _defaultFontPitch;
			}
		}
		public SizeF HeaderCharPitch {
			get {
				if(_fontDirty) CreateFont();
				return _headerFontPitch;
			}
		}


		private void CreateFont() {
			_defaultFont = new Font(_fontName, _fontSize, _fontStyle);
			_headerFont = new Font(_fontName, _fontSize, _fontStyle);
			
			Graphics g = Env.Frame.CreateGraphics();
			SizeF t1 = g.MeasureString("A",  _defaultFont);
			SizeF t2 = g.MeasureString("AA", _defaultFont);
			_defaultFontPitch = new SizeF(t2.Width-t1.Width, t1.Height);

			t1 = g.MeasureString("A",  _headerFont);
			t2 = g.MeasureString("AA", _headerFont);
			_headerFontPitch = new SizeF(t2.Width-t1.Width, t1.Height);
			g.Dispose();

			_defaultHFont = _defaultFont.ToHfont();
			_headerHFont = _headerFont.ToHfont();
			_fontDirty = false;
		}

	}

	/// <summary>
	/// LayoutInfo�́APreference�I�u�W�F�N�g�ƌ��݂̃E�B���h�E�T�C�Y�Ȃǂ����Ɍv�Z���郌�C�A�E�g�������^
	/// </summary>
	internal class LayoutInfo {
		//��ʍ\��
		private int _volumePaneHeight;     //�o�����n����\�����鍂��
		private int[] _oscillatorPaneHeights; //�I�V���[�^�̍���
		private int _oscillatorPaneHeightTotal; //���̍��v
		private int _headerHeight;         //�㕔�̖������Ȃǂ�\������̈�̍���
		private int _footerHeight;         //�����̓��t�Ȃǂ�\������̈�̍���
		private int _scaleAreaWidth;       //�ڐ���̐��l��\������̈�̕�
        private int _accumulativeVolumeWidth; //���i�яo�����p�̕�
        private int _remarkAreaWidth;      //���ݒl�ƒ���
		private int _defaultTextWidth;
		private int _defaultTextHeight;
		private int _brandInfoHeight;

		public void Init() {
			//�ꕔ��Preference����擾
			Preference pref = Env.Preference;
			_volumePaneHeight = CalcHeight(pref.ShowVolume, 100);
			_oscillatorPaneHeightTotal = 0;
			_oscillatorPaneHeights = new int[OscillatorPreference.LENGTH];
			for(int i=0; i<_oscillatorPaneHeights.Length; i++) {
				int h = CalcHeight(pref.OscillatorPreferences[i].Config, 100);
				_oscillatorPaneHeights[i] = h;
				_oscillatorPaneHeightTotal += h;
			}
			_headerHeight = 28; //(int)pref.HeaderCharPitch.Height+2;
			_footerHeight = (int)pref.DefaultCharPitch.Height+2;
			_brandInfoHeight = Math.Min(40, _footerHeight);
			_scaleAreaWidth = (int)(pref.DefaultCharPitch.Width * 7);
            _accumulativeVolumeWidth = pref.ShowAccumulativeVolume? (int)(pref.DefaultCharPitch.Width * 20) : 0;
            _remarkAreaWidth = pref.ShowPrice? (int)(pref.DefaultCharPitch.Width * 23) : 0;
			_defaultTextWidth = (int)Math.Ceiling(pref.DefaultCharPitch.Width);
			_defaultTextHeight = (int)Math.Ceiling(pref.DefaultCharPitch.Height);
		}
		
		public int OscillatorPaneHeightTotal {
			get {
				return _oscillatorPaneHeightTotal;
			}
		}
		public int VolumePaneHeight {
			get {
				return _volumePaneHeight;
			}
		}
		public int[] OscillatorPaneHeights {
			get {
				return _oscillatorPaneHeights;
			}
		}
		public int HeaderHeight {
			get {
				return _headerHeight;
			}
		}	  
		public int FooterHeight {
			get {
				return _footerHeight;
			}
		}	  
		public int ScaleAreaWidth {
			get {
				return _scaleAreaWidth;
			}
		}
        public int AccumulativeVolumeWidth {
            get {
                return _accumulativeVolumeWidth;
            }
        }
        public int RemarkAreaWidth {
			get {
				return _remarkAreaWidth;
			}
		}
		public int DefaultTextHeight {
			get {
				return _defaultTextHeight;
			}
		}
		public int DefaultTextWidth {
			get {
				return _defaultTextWidth;
			}
		}

		//���C�A�E�g�����
		public Rectangle ChartBodyRect {
			get {
				return new Rectangle(0, 0, Env.Frame.ChartCanvas.Width-_remarkAreaWidth-_accumulativeVolumeWidth-_scaleAreaWidth, Env.Frame.ChartCanvas.BodyHeight);
			}
		}
		public Rectangle BrandInformationRect {
			get {
				return new Rectangle(0, _headerHeight, Env.Frame.ChartCanvas.Width-_remarkAreaWidth-_accumulativeVolumeWidth, _brandInfoHeight);
			}
		}
        public Rectangle CurrentValueRect {
            get {
                return new Rectangle(Env.Frame.ChartCanvas.Width - _remarkAreaWidth, _headerHeight, _remarkAreaWidth, _defaultTextHeight * (GetDrawingEngine().MaximumValueWindowItemCount + 1));
            }
        }
        public Rectangle ExplanationRect {
            get {
                Rectangle r = this.CurrentValueRect;
                r.Y = r.Bottom;
                r.Height = Env.Frame.ChartCanvas.BodyHeight - r.Y;
                return r;
            }
        }
        public Rectangle AccumulativeVolumeRect {
            get {
                return new Rectangle(Env.Frame.ChartCanvas.Width - _remarkAreaWidth - _accumulativeVolumeWidth, _headerHeight, _accumulativeVolumeWidth, Env.Frame.ChartCanvas.BodyHeight-_oscillatorPaneHeightTotal-_volumePaneHeight-_footerHeight);
            }
        }

        public int ChartAreaHeight {
			get {
				return Env.Frame.ChartCanvas.BodyHeight-_headerHeight-_footerHeight;
			}
		}
		public int ChartAreaWidth {
			get {
				return Env.Frame.ChartCanvas.Width-_remarkAreaWidth-_accumulativeVolumeWidth-_scaleAreaWidth;
			}
		}
		public int ChartAreaBottom {
			get {
				return Env.Frame.ChartCanvas.BodyHeight-_footerHeight;
			}
		}

        public int DisplayColumnCount {
			get {
				int n = this.ChartAreaWidth/Env.Preference.DatePitch;
#if DOJIMA
				if(Env.CurrentIndicators.Format==ChartFormat.HalfDaily) n /= 2;
#endif
				if(n<=1) n = 1;
				return n;
			}
		}
		//Preference��DatePitch�ƈقȂ�AChartFormat���l�������l�ɂȂ�
		public int DatePitch {
			get {
				int n = Env.Preference.DatePitch;
#if DOJIMA
				if(Env.CurrentIndicators.Format==ChartFormat.HalfDaily) n *= 2;
#endif
				return n;
			}
		}
		public int CandleMiddleOffset {
			get {
				return DatePitch / 2;
			}
		}		

        private ChartDrawing GetDrawingEngine() {
			return Env.Frame.ChartCanvas.DrawingEngine;
		}

		private static int CalcHeight(HeightConfig value, int max) {
			switch(value) {
				case HeightConfig.None:
					return 0;
				case HeightConfig.Large:
					return max;
				case HeightConfig.Middle:
					return max * 3 / 4;
				default: //Small
					return max / 2;
			}
		}
	}
}
