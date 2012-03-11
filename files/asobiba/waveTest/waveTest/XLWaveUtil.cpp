#include "XLWaveUtil.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>
#include <windows.h>

//��������
//���̃v���O�����́A�uC����ł͂��߂鉹�̃v���O���~���O�v���x�[�X���Ă��܂��B
//�����{�Ȃ̂�amazon �Ŕ����܂��傤�B
//http://www.amazon.co.jp/dp/4274206505
class XLWave
{
private:
	unsigned int fs;		// �W�{�����g��
	unsigned int bits;	// �ʎq�����x 
	unsigned int length; // ���f�[�^�̒���
	double *l;  // ���f�[�^
	double *r;  // ���f�[�^(���m�����̏ꍇ�ANULL�ɂȂ�.)
public:
	// �W�{�����g��
	unsigned int getSamplesPerSec() const
	{
		return this->fs;
	}
	// �ʎq�����x
	unsigned int getBitsPerSample() const
	{
		return this->bits;
	}
	// ���f�[�^�̒���
	unsigned int getLength() const
	{
		return this->length;
	}
	unsigned int getChannel() const
	{
		return IsStereo() ? 2 : 1;
	}
	// ���f�[�^ ��
	const double* getDataL() const
	{
		this->l;
	}
	// ���f�[�^ �E
	const double* getDataR() const
	{
		this->r;
	}
	// ���f�[�^ �� const�Ȃ�
	double* getLowDataL() 
	{
		this->l;
	}
	// ���f�[�^  const�Ȃ�
	double* getLowDataR() 
	{
		this->r;
	}
	bool IsStereo() const
	{
		return this->r != NULL;
	}
	bool IsEnable() const
	{
		return this->l != NULL;
	}

	XLWave()
	{
		this->fs = 0;
		this->bits = 0;
		this->length = 0;
		this->l = NULL;
		this->r = NULL;
	}
	XLWave(const XLWave& b)
	{
		this->fs = b.fs;
		this->bits = b.bits;
		this->length = b.length;
		if (b.l == NULL)
		{
			this->l = NULL;
		}
		else
		{
			this->l = (double*)calloc(b.length, sizeof(double));
			memcpy(this->l,b.l,b.length * sizeof(double));
		}
		if (b.r == NULL)
		{
			this->r = NULL;
		}
		else
		{
			this->r = (double*)calloc(b.length, sizeof(double));
			memcpy(this->r,b.r,b.length * sizeof(double));
		}
	}

	XLWave(const std::string& filename)
	{
		Open(filename);
	}
	XLWave(const char* wavedata ,unsigned  int len)
	{
		Open(wavedata,len);
	}
	XLWave(const char* wavedata ,unsigned  int len,unsigned int samplesPerSec,unsigned int bitsPerSample,unsigned  int channel)
	{
		Open(wavedata ,  len, samplesPerSec, bitsPerSample,  channel);
	}
	XLWave(unsigned int len,unsigned int samplesPerSec,unsigned int bitsPerSample,unsigned  int channel)
	{
		MakeNullWave( len, samplesPerSec, bitsPerSample,  channel);
	}

	virtual ~XLWave()
	{
		if (this->l)
		{
			free(this->l);
			this->l = NULL;
		}
		if (this->r)
		{
			free(this->r);
			this->r = NULL;
		}
	}

	bool XLWave::Open(const char* wavedata ,unsigned  int dataChunkSize,int samplesPerSec,int bitsPerSample, int channel)
	{
		unsigned data;
		const char* now = wavedata;
		this->fs = samplesPerSec;		// �W�{�����g��
		this->bits = bitsPerSample;	// �ʎq�����x 
		if (channel == 1)
		{//���m����
			this->length = dataChunkSize / 2; // ���f�[�^�̒���
			this->l = (double*) calloc(this->length, sizeof(double)); // �������̊m��
			this->r = NULL;
  
			for (unsigned int n = 0; n < this->length; n++)
			{
				memcpy(&data , now , 2 ); now += 2;	//���f�[�^�̓ǂݎ��
				this->l[n] = (double)data / 32768.0; // ���f�[�^��-1�ȏ�1�����͈̔͂ɐ��K������
			}
		}
		else
		{//�X�e���I
			this->length = dataChunkSize / 4; // ���f�[�^�̒���
			this->l = (double*) calloc(this->length, sizeof(double)); //
			this->r = (double*) calloc(this->length, sizeof(double)); //
  
			for (unsigned int n = 0; n < this->length; n++)
			{
				memcpy(&data , now , 2 ); now += 2;	//���f�[�^�̓ǂݎ��
				this->l[n] = (double)data / 32768.0; // ���f�[�^��-1�ȏ�1�����͈̔͂ɐ��K������

				memcpy(&data , now , 2 ); now += 2;	//���f�[�^�̓ǂݎ��
				this->r[n] = (double)data / 32768.0; // ���f�[�^��-1�ȏ�1�����͈̔͂ɐ��K������
			}
		}
		return false;
	}

	bool XLWave::Open(const char* wavedata ,unsigned  int len) 
	{
		char riff_chunk_ID[4];
		long riff_chunk_size;
		char riff_form_type[4];
		char fmt_chunk_ID[4];
		long fmt_chunk_size;
		short fmt_wave_format_type;
		short fmt_channel;
		long fmt_samples_per_sec;
		long fmt_bytes_per_sec;
		short fmt_block_size;
		short fmt_bits_per_sample;
		char data_chunk_ID[4];
		unsigned long data_chunk_size;
		short data;

		if (len < 44)
		{
			return false;
		}
		const char* now = wavedata;
		memcpy(&riff_chunk_ID , now , 4 ); now += 4;
		memcpy(&riff_chunk_size , now , 4 ); now += 4;
		memcpy(&riff_form_type , now , 4 ); now += 4;
		memcpy(&fmt_chunk_ID , now , 4 ); now += 4;
		memcpy(&fmt_chunk_size , now , 4 ); now += 4;
		memcpy(&fmt_wave_format_type , now , 2 ); now += 2;
		memcpy(&fmt_channel , now , 2 ); now += 2;
		memcpy(&fmt_samples_per_sec , now , 4 ); now += 4;
		memcpy(&fmt_bytes_per_sec , now , 4 ); now += 4;
		memcpy(&fmt_block_size , now , 2 ); now += 2;
		memcpy(&fmt_bits_per_sample , now , 2 ); now += 2;
		memcpy(&data_chunk_ID , now , 4 ); now += 4;
		memcpy(&data_chunk_size , now , 4 ); now += 4;
		if (memcmp(riff_chunk_ID, "RIFF",4) != 0 )
		{
			return false;
		}
		if (memcmp(riff_form_type, "WAVE",4) != 0 )
		{
			return false;
		}
		if (memcmp(fmt_chunk_ID, "fmt ",4) != 0 )
		{
			return false;
		}
		if ( len - 44 < data_chunk_size)
		{
			return false;
		}
		if (fmt_channel >= 3)
		{
			return false;
		}

		this->fs = fmt_samples_per_sec;		// �W�{�����g��
		this->bits = fmt_bits_per_sample;	// �ʎq�����x 
		if (fmt_channel == 1)
		{//���m����
			this->length = data_chunk_size / 2; // ���f�[�^�̒���
			this->l = (double*) calloc(this->length, sizeof(double)); // �������̊m��
			this->r = NULL;
  
			for (unsigned int n = 0; n < this->length; n++)
			{
				memcpy(&data , now , 2 ); now += 2;	//���f�[�^�̓ǂݎ��
				this->l[n] = (double)data / 32768.0; // ���f�[�^��-1�ȏ�1�����͈̔͂ɐ��K������
			}
		}
		else
		{//�X�e���I
			this->length = data_chunk_size / 4; // ���f�[�^�̒���
			this->l = (double*) calloc(this->length, sizeof(double)); //
			this->r = (double*) calloc(this->length, sizeof(double)); //
  
			for (unsigned int n = 0; n < this->length; n++)
			{
				memcpy(&data , now , 2 ); now += 2;	//���f�[�^�̓ǂݎ��
				this->l[n] = (double)data / 32768.0; // ���f�[�^��-1�ȏ�1�����͈̔͂ɐ��K������

				memcpy(&data , now , 2 ); now += 2;	//���f�[�^�̓ǂݎ��
				this->r[n] = (double)data / 32768.0; // ���f�[�^��-1�ȏ�1�����͈̔͂ɐ��K������
			}
		}
		return true;
	}
	bool XLWave::Open(const std::string& wavFilename) 
	{
		FILE * fp = fopen(wavFilename.c_str() , "rb");
		//���݂��Ȃ��ꍇ�͋�
		if (fp == NULL) return false;

		//�P�c�Ɏ����Ă�����.
		fseek(fp , 0 ,SEEK_END);

		//����ŃT�C�Y���킩��.
		unsigned long size = ftell(fp);

		//�擪�ɖ߂�.
		fseek(fp , 0 ,SEEK_SET);

		char* buffer = new char[size];
		fread(buffer , 1 , size , fp);
		bool r = Open(buffer,size);

		delete [] buffer;
		return r;
	}
	bool XLWave::MakeNullWave(unsigned int len,unsigned int samplesPerSec,unsigned int bitsPerSample,unsigned int channel)
	{
		assert(channel == 1 || channel == 2);

		this->fs = samplesPerSec;		// �W�{�����g��
		this->bits = bitsPerSample;	// �ʎq�����x 
		if (channel == 1)
		{//���m����
			this->length = len; // ���f�[�^�̒���
			this->l = (double*) calloc(this->length, sizeof(double)); // �������̊m��
			this->r = NULL;
		}
		else
		{//�X�e���I
			this->length = len; // ���f�[�^�̒���
			this->l = (double*) calloc(this->length, sizeof(double)); // �������̊m��
			this->r = (double*) calloc(this->length, sizeof(double)); // �������̊m��
		}
		return true;
	}

	bool XLWave::Save(const std::string& filename) 
	{
		assert(this->l);

		char riff_chunk_ID[4];
		long riff_chunk_size;
		char riff_form_type[4];
		char fmt_chunk_ID[4];
		long fmt_chunk_size;
		short fmt_wave_format_type;
		short fmt_channel;
		long fmt_samples_per_sec;
		long fmt_bytes_per_sec;
		short fmt_block_size;
		short fmt_bits_per_sample;
		char data_chunk_ID[4];
		long data_chunk_size;
		short data;
  
		riff_chunk_ID[0] = 'R';
		riff_chunk_ID[1] = 'I';
		riff_chunk_ID[2] = 'F';
		riff_chunk_ID[3] = 'F';
		riff_chunk_size = 36 + this->length * 2 * this->getChannel();
		riff_form_type[0] = 'W';
		riff_form_type[1] = 'A';
		riff_form_type[2] = 'V';
		riff_form_type[3] = 'E';
  
		fmt_chunk_ID[0] = 'f';
		fmt_chunk_ID[1] = 'm';
		fmt_chunk_ID[2] = 't';
		fmt_chunk_ID[3] = ' ';
		fmt_chunk_size = 16;
		fmt_wave_format_type = 1;
		fmt_channel = this->getChannel(); //�X�e���I2 ���m����1�H
		fmt_samples_per_sec = this->fs; // �W�{�����g�� 
		fmt_bytes_per_sec = this->fs * this->bits / 8 * this->getChannel();
		fmt_block_size = this->bits / 8 * this->getChannel();
		fmt_bits_per_sample = this->bits; // �ʎq�����x 
  
		data_chunk_ID[0] = 'd';
		data_chunk_ID[1] = 'a';
		data_chunk_ID[2] = 't';
		data_chunk_ID[3] = 'a';
		data_chunk_size = this->length * 2 * this->getChannel();
  
		FILE * fp = fopen(filename.c_str(), "wb");
		if (!fp)
		{
			return false;
		}
  
		fwrite(riff_chunk_ID, 1, 4, fp);
		fwrite(&riff_chunk_size, 4, 1, fp);
		fwrite(riff_form_type, 1, 4, fp);
		fwrite(fmt_chunk_ID, 1, 4, fp);
		fwrite(&fmt_chunk_size, 4, 1, fp);
		fwrite(&fmt_wave_format_type, 2, 1, fp);
		fwrite(&fmt_channel, 2, 1, fp);
		fwrite(&fmt_samples_per_sec, 4, 1, fp);
		fwrite(&fmt_bytes_per_sec, 4, 1, fp);
		fwrite(&fmt_block_size, 2, 1, fp);
		fwrite(&fmt_bits_per_sample, 2, 1, fp);
		fwrite(data_chunk_ID, 1, 4, fp);
		fwrite(&data_chunk_size, 4, 1, fp);
  
		if (this->IsStereo() )
		{//�X�e���I
			for (unsigned int n = 0; n < this->length; n++)
			{
				double s = (this->l[n] + 1.0) / 2.0 * 65536.0;
				if (s > 65535.0)
				{
					s = 65535.0; // �N���b�s���O
				}
				else if (s < 0.0)
				{
					s = 0.0;    // �N���b�s���O
				}
    			data = (short)(s + 0.5) - 32768; // �l�̌ܓ��ƃI�t�Z�b�g�̒���
				fwrite(&data, 2, 1, fp);         // ���f�[�^�̏����o��

				s = (this->r[n] + 1.0) / 2.0 * 65536.0;
				if (s > 65535.0)
				{
					s = 65535.0; // �N���b�s���O
				}
				else if (s < 0.0)
				{
					s = 0.0;    // �N���b�s���O
				}
    			data = (short)(s + 0.5) - 32768; // �l�̌ܓ��ƃI�t�Z�b�g�̒���
				fwrite(&data, 2, 1, fp);         // ���f�[�^�̏����o��
			}
		}
		else
		{//���m����
			for (unsigned int n = 0; n < this->length; n++)
			{
				double s = (this->l[n] + 1.0) / 2.0 * 65536.0;

				if (s > 65535.0)
				{
					s = 65535.0; // �N���b�s���O
				}
				else if (s < 0.0)
				{
					s = 0.0;    // �N���b�s���O
				}
    
				data = ((short)(s + 0.5)) - 32768; // �l�̌ܓ��ƃI�t�Z�b�g�̒���
				fwrite(&data, 2, 1, fp);         // ���f�[�^�̏����o��
			}
		}
		fclose(fp);
		return true;
	}

	//���̈ꕔ������Ƃ� substr �݂����ȁB
	XLWave SubWave(unsigned int startFream,unsigned int step = 0) const
	{
		if (startFream > this->length)
		{
			return XLWave();
		}
		if (step == 0 || startFream + step > this->length)
		{//�Ō�܂�
			step = this->length - startFream;
		}
		XLWave ret( step , this->fs,this->bits , this->getChannel() );
		if (this->getChannel() == 2)
		{//�X�e���I
			for(unsigned int i = 0 ; i < step ; ++i)
			{
				ret.l[i + startFream] = this->l[i];
				ret.r[i + startFream] = this->r[i];
			}
		}
		else
		{//���m����
			for(unsigned int i = 0 ; i < step ; ++i)
			{
				ret.l[i + startFream] = this->l[i];
			}
		}
		return std::move(ret);
	}
	//�Ō�̕����ɉ���������.

	//�f�B���C
	XLWave DelayEffect(double attenuationRate = 0.5,double delayTime = 0.385,int repeat = 2) const
	{
		//�`�����l�����킩��Ă��܂��̂ŁA���ʎ�������āA�`�����l�����ƂɓK������.
		auto func = [](double* ss,const double* s,int len,double attenuationRate,double delayTime ,int repeat){
			for (int n = 0; n < len; n++)
			{
				ss[n] = s[n]; // ���݂̎����̉��f�[�^
				for (int i = 1; i <= repeat; i++)
				{
					int m = (int)((double)n - ((double)i * delayTime));
					if (m >= 0)
					{
						ss[n] += pow(attenuationRate, (double)i) * s[m]; // �ߋ��̉��f�[�^���~�b�N�X����
					}
				}
			}
		};

		delayTime *= this->fs; //�x������ * �W�{�����g��

		//���ʂ��i�[����N���X���쐬����
		XLWave ret( this->length , this->fs,this->bits , this->getChannel() );
		if (this->IsStereo())
		{
			func(ret.l , this->l,this->length,attenuationRate,delayTime , repeat);
			func(ret.r , this->r,this->length,attenuationRate,delayTime , repeat);
		}
		else
		{
			func(ret.l , this->l,this->length,attenuationRate,delayTime , repeat);
		}

		return ret;
	}

	//�g������
	XLWave TremoloEffect(double depth, double rate) const
	{
		//�`�����l�����킩��Ă��܂��̂ŁA���ʎ�������āA�`�����l�����ƂɓK������.
		auto func = [](double* ss,const double* s,int len,double depth,double rate){
			for (int n = 0; n < len; n++)
			{
//				double a = 1.0 + depth * sin(2.0 * M_PI * rate * n / fs);
				double a = 1.0 + depth * sin( rate * n); //2.0 * M_PI *rate/fs���ɂ���Ă��܂�.
				ss[n] = a * s[n];
			}
		};

		rate = 2.0 * M_PI * rate / this->fs;

		//���ʂ��i�[����N���X���쐬����
		XLWave ret( this->length , this->fs,this->bits , this->getChannel() );
		if (this->IsStereo())
		{
			func(ret.l , this->l,this->length,depth,rate);
			func(ret.r , this->r,this->length,depth,rate);
		}
		else
		{
			func(ret.l , this->l,this->length,depth,rate);
		}

		return ret;
	}
	//�R�[���X
	XLWave ChorusEffect(double delayTime,double depth, double rate) const
	{
		//�`�����l�����킩��Ă��܂��̂ŁA���ʎ�������āA�`�����l�����ƂɓK������.
		auto func = [](double* ss,const double* s,int len,double delayTime,double depth, double rate){
			for (int n = 0; n < len; n++)
			{
				ss[n] = s[n]; // ���݂̎����̉��f�[�^

//				double tau = delayTime + depth * sin(2.0 * M_PI * rate * n / this->fs);
				double tau = delayTime + depth * sin(rate * n );  //2.0 * M_PI *rate/fs���ɂ���Ă��܂�.
				double t = (double)n - tau;
				int m = (int)t;
				double delta = t - (double)m;
				if (m >= 0 && m + 1 < len)
				{
					ss[n] += delta * s[m + 1] + (1.0 - delta) * s[m]; 
				}
			}
		};

		delayTime *= this->fs; //�x������ * �W�{�����g��
		rate = 2.0 * M_PI * rate / this->fs;

		XLWave ret( this->length , this->fs,this->bits , this->getChannel() );
		if (this->IsStereo())
		{
			func(ret.l , this->l,this->length,delayTime, depth,  rate);
			func(ret.r , this->r,this->length,delayTime, depth,  rate);
		}
		else
		{
			func(ret.l , this->l,this->length,delayTime, depth,  rate);
		}
		return ret;
	}


	//�X�s�[�h�ύX
	XLWave SpeedUpEffect( double rate = 1.5 ) const
	{
		//�`�����l�����킩��Ă��܂��̂ŁA���ʎ�������āA�`�����l�����ƂɓK������.
		auto func = [](double* ss,const double* s,int len,double rate,double fs){
  
			int template_size = (int)(fs * 0.01); // 10ms
			int pmin = (int)(fs * 0.005); // 5ms
			int pmax = (int)(fs * 0.02);  // 20ms
  
			double* x = (double*)calloc(template_size, sizeof(double)); // �������̊m��
			double* y = (double*)calloc(template_size, sizeof(double)); // �������̊m��
			double* r = (double*)calloc((pmax + 1), sizeof(double));    // �������̊m��
  
			int offset0 = 0;
			int offset1 = 0;
			bool isup = rate >= 1;
  
			while (offset0 + pmax * 2 < len)
			{
				for (int n = 0; n < template_size; n++)
				{
					x[n] = s[offset0 + n]; // �{���̉��f�[�^
				}
    
				double max_of_r = 0.0;
				int p = pmin;
				for (int m = pmin; m <= pmax; m++)
				{
					for (int n = 0; n < template_size; n++)
					{
						y[n] = s[offset0 + m + n]; // m�T���v�����炵�����f�[�^
					}
					r[m] = 0.0;
					for (int n = 0; n < template_size; n++)
					{
						r[m] += x[n] * y[n]; // ���֊֐�
					}
					if (r[m] > max_of_r)
					{
						max_of_r = r[m]; // ���֊֐��̃s�[�N
						p = m;			 // ���f�[�^�̊�{����
					}
				}
    
				if (isup)
				{//�X�s�[�h�A�b�v
					for (int n = 0; n < p; n++)
					{
						ss[offset1 + n] = s[offset0 + n] * (p - n) / p; // �P�������̏d�݂Â�
						ss[offset1 + n] += s[offset0 + p + n] * n / p;  // �P�������̏d�݂Â�
					}
    
					int q = (int)(p / (rate - 1.0) + 0.5);
					for (int n = p; n < q; n++)
					{
						if (offset0 + p + n >= len)
						{
							break;
						}
						ss[offset1 + n] = s[offset0 + p + n];
					}
    
					offset0 += p + q; /* offset0�̍X�V */
					offset1 += q; /* offset1�̍X�V */
				}
				else
				{//�X�s�[�h�_�E��
					for (int n = 0; n < p; n++)
					{
						ss[offset1 + n] = s[offset0 + n];
					}
					for (int n = 0; n < p; n++)
					{
						ss[offset1 + p + n] = s[offset0 + p + n] * (p - n) / p; /* �P�������̏d�݂Â� */
						ss[offset1 + p + n] += s[offset0 + n] * n / p; /* �P�������̏d�݂Â� */
					}
    
					int q = (int)(p * rate / (1.0 - rate) + 0.5);
					for (int n = p; n < q; n++)
					{
						if (offset0 + n >= len)
						{
						break;
						}
						ss[offset1 + p + n] = s[offset0 + n];
					}
    
					offset0 += q; /* offset0�̍X�V */
					offset1 += p + q; /* offset1�̍X�V */
				}
			}
  
			free(x); /* �������̉�� */
			free(y); /* �������̉�� */
			free(r); /* �������̉�� */
  		};

		XLWave ret( this->length / rate + 1, this->fs,this->bits , this->getChannel() );
		if (this->IsStereo())
		{
			func(ret.l , this->l,this->length,rate,this->fs);
			func(ret.r , this->r,this->length,rate,this->fs);
		}
		else
		{
			func(ret.l , this->l,this->length,rate,this->fs);
		}
		return ret;
	}
	//���ʕύX
	XLWave VolumeUpEffect( double rate = 1.5 ) const
	{
		//�`�����l�����킩��Ă��܂��̂ŁA���ʎ�������āA�`�����l�����ƂɓK������.
		auto func = [](double* ss,const double* s,int len,double rate){
			for (int n = 0; n < len; n++)
			{
				ss[n] = s[n] * rate;
			}
		};

		//���ʂ��i�[����N���X���쐬����
		XLWave ret( this->length , this->fs,this->bits , this->getChannel() );
		if (this->IsStereo())
		{
			func(ret.l , this->l,this->length,rate);
			func(ret.r , this->r,this->length,rate);
		}
		else
		{
			func(ret.l , this->l,this->length,rate);
		}

		return ret;
	}
	//�s�b�`�V�t�^
	XLWave PitchShiftEffect( double pitch = 1.5) const
	{
		//�`�����l�����킩��Ă��܂��̂ŁA���ʎ�������āA�`�����l�����ƂɓK������.
		auto func = [](double* ss,const double* s,int shortlen,int orignallen,double pitch){
			int J = 24;

			for (int n = 0; n < orignallen; n++)
			{
				double t = pitch * n;
				int offset = (int)t;
				for (int m = offset - J / 2; m <= offset + J / 2; m++)
				{
					if (m >= 0 && m < shortlen)
					{
						ss[n] += s[m] * sinc(M_PI * (t - m));
					}
				}
			}
		};

		//���ʂ��i�[����N���X���쐬����
		XLWave ret( this->length , this->fs,this->bits , this->getChannel() );

		//�܂����x��ύX���܂��B
//		double rate = 0.66 * pitch / 1.5; //rate��picth�ɂ͏�������t����H rate:pitch = 0.66:1.5
		double rate = 2.16 - pitch; //picth 0.66   rate 1.5
		                            //picth 1.5    rate 0.66
		XLWave tempWave = this->SpeedUpEffect(rate);
		
		//����ŉ��̍����͕ς��܂������A
		//��������/�X���[�Đ������̂ŉ��̒������ς���Ă��܂����̂ŁA
		//�����L�΂��A���k�����܂��B
		if (this->IsStereo())
		{
			func(ret.l , tempWave.l,tempWave.length,ret.length,pitch);
			func(ret.r , tempWave.r,tempWave.length,ret.length,pitch);
		}
		else
		{
			func(ret.l , tempWave.l,tempWave.length,ret.length,pitch);
		}

		return ret;
	}
private:
	static double XLWave::sinc(double x)
	{
		double y;
  
		if (x == 0.0)
		{
			y = 1.0;
		}
		else
		{
			y = sin(x) / x;
		}
  
		return y;
	}
};

int main()
{
	XLWave a("sample12.wav");
	XLWave b = a.PitchShiftEffect(1.5);
	b.Save("b.wav");
  
	return 0;
}
