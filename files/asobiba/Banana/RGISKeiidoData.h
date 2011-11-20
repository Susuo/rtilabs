// RGISKeiidoData.h: RGISKeiidoData �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISKEIIDODATA_H__FD9E1E43_A5A6_4405_87C0_9CA6C7E20142__INCLUDED_)
#define AFX_RGISKEIIDODATA_H__FD9E1E43_A5A6_4405_87C0_9CA6C7E20142__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"

class RGISKeiidoData
{
public:
	int		Do;		//�x
	int		Fun;	//��
	int		Byou;	//�b
	int		M;		//�~���b

	RGISKeiidoData()
	{
	}

	RGISKeiidoData(int inDo , int inFun , int inByou , int inM)
	{
		setInt(inDo , inFun , inByou , inM);
	}
	void setInt(int inDo , int inFun , int inByou , int inM)
	{
		Do = inDo ;
		Fun = inFun ;
		Byou = inByou ;
		M = inM;
		ASSERT(Fun >= 0 && Fun <= 60 );
		ASSERT(Byou >= 0 && Byou <= 60 );
	}
	void setDouble(double x)
	{
		ASSERT(x < 360 && x >= 0);
		double ans = x;
		Do = (int)(ans);
		ans = (ans - Do) * 60;

		Fun = (int)(ans);
		ans = (ans - Fun) * 60;

  		Byou = (int)(ans);
		ans = (ans - Byou) * 10;

  		M = (int)(ans +0.5);

		ASSERT(Fun >= 0 && Fun <= 60 );
		ASSERT(Byou >= 0 && Byou <= 60 );
	}
	void setInt(unsigned long x)
	{
        double ans = x / (double)360000;
//        s = s + " �k�� " + (int)ans + "�x";
		Do = (int) ans;

        ans -= (int)ans;
        ans *= (double)60;
//        s = s + (int)ans + "��";
		Fun = (int)ans;

        ans -= (int)ans;
        ans *= (double)60;
//        s = s + (int)ans + ".";
		Byou = (int)ans;

        ans -= (int)ans;
        ans = (int)(ans * (double)10);
//        s = s + (int)ans + "�b";
		M = (int)ans;
	}

	double toDouble() const
	{
//		return (double)Do *3600 + Fun *60 + Byou + (M / 10);
//		return Do + Fun / 60.0 + Byou / 3600.0 + M / 360000.0;
		return Do + Fun / 60.0 + Byou / 3600.0 + M / 36000.0;
	}

	const string toString() const
	{
		return PrintFormat("%d/%d/%d.%d" , Do , Fun , Byou , M);
	}

	bool operator==(const RGISKeiidoData & inTarget) const
	{
		return inTarget.Do == this->Do && inTarget.Fun == this->Fun && inTarget.Byou == this->Byou && inTarget.M == this->M;
	}

	static void test()
	{
		{
			RGISKeiidoData kei(36,3,4,5);
			double k = kei.toDouble();

			RGISKeiidoData kei2;
			kei2.setDouble(k);

			ASSERT(kei == kei2);
		}
		{
			RGISKeiidoData kei(43,02,37,7);
			double k = kei.toDouble();

			RGISKeiidoData kei2;
			kei2.setDouble(43.043819);

			ASSERT(kei == kei2);
		}
	}
};

#endif // !defined(AFX_RGISKEIIDODATA_H__FD9E1E43_A5A6_4405_87C0_9CA6C7E20142__INCLUDED_)
