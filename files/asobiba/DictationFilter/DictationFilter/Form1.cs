﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace DictationFilter
{
    public partial class Form1 : Form
    {
        DictationFilter.SpeechRecognizer Reco = null;
        private string MustMatchString = "コンピュータ"; //絶対入っていなければいけない文字列

        public Form1()
        {
            InitializeComponent();
            this.Reco = new DictationFilter.SpeechRecognizer();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.Reco.Hypothesis +=
                delegate(int streamNumber, object streamPosition, SpeechLib.ISpeechRecoResult result)
                {
                    string strText = result.PhraseInfo.GetText(0, -1, true);
                    this.HypothesisTextBox.Text = strText;
                };
            this.Reco.Recognition +=
                delegate(int streamNumber, object streamPosition, SpeechLib.SpeechRecognitionType srt, SpeechLib.ISpeechRecoResult isrr)
                {
                    string strText = isrr.PhraseInfo.GetText(0, -1, true);
                    this.RecognitionTextBox.Text = strText;
                };
            this.Reco.StartStream +=
                delegate(int streamNumber, object streamPosition)
                {
                    this.HypothesisTextBox.Text = "";
                    this.RecognitionTextBox.Text = "";
                };
            this.Reco.FalseRecognition +=
                delegate(int streamNumber, object streamPosition, SpeechLib.ISpeechRecoResult isrr)
                {
                    this.RecognitionTextBox.Text = "--ERROR!--";
                };

            this.Reco.DictationRecognition +=
                delegate(int streamNumber, object streamPosition, SpeechLib.SpeechRecognitionType srt, SpeechLib.ISpeechRecoResult isrr)
                {
                    string strText = isrr.PhraseInfo.GetText(0, -1, true);
                    this.DictationRecognitioTextBox.Text = strText;
                };

            string str = "あしたのてんきは"; //コンピュータあしたのてんきは
            this.Reco.SetMustMatchString(this.MustMatchString);
            this.Reco.SetString(str);
            this.StringTextBox.Text = str;
        }

        private void RuleRebuildButton_Click(object sender, EventArgs e)
        {
            try
            {
                this.Reco.SetMustMatchString(this.MustMatchString);
                this.Reco.SetString(this.StringTextBox.Text);
            }
            catch (Exception ex)
            {
                MessageBox.Show("エラー:" + ex.Message);
            }
        }

        private void DictationFilterCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (this.DictationFilterCheckBox.Checked)
            {
                this.Reco.SetMustMatchString(this.MustMatchString);
            }
            else
            {
                this.Reco.SetMustMatchString(""); //無効
            }
        }
    }
}
