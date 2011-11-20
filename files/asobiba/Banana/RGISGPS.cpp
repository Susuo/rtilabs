// RGISGPS.cpp: RGISGPS �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISGPS.h"
#include "RDebug.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////


//���s�R�[�h�܂�
class StreamCheckerForReturnCodeWithThread : public StreamChecker
{
public:
	StreamCheckerForReturnCodeWithThread(const RThread* inThread)
	{
		Separater = 0;
		ConectStart = 0;
		Thread = inThread;
	}
	~StreamCheckerForReturnCodeWithThread()
	{
	}

	virtual bool Check(const char* inBuffer , int inAllReadyRead , int inNowRead)
	{
		for (int i = inAllReadyRead; i < inNowRead ; i ++) 
		{
			//�X���b�h���I���������ꍇ�͂Ƃɂ����I��.
			if ( !Thread->IsHoldUp()) return false;

			if ( inBuffer[i] == '\n' )
			{
				Separater = i - 1;
				ConectStart = i;
				return false;
			}
		}
		return true;
	}

	virtual int getSeparater() const
	{
		return Separater;
	}
	virtual int getNext() const 
	{
		return ConectStart;
	}
private:
	int Separater;
	int ConectStart;
	const RThread* Thread;
};


RGISGPS::RGISGPS()
{
}

RGISGPS::~RGISGPS()
{

}

void RGISGPS::Create(int port ,int speed ,int type ,int logType ,const string & logDir )
{
	Stop();

	NMEA0183LogRingBuffer.Create(NMEA0183_RESERVE_BUFFER_SIZE);
	LogFile.Create( (RGISGPSLog::GPS_LOGTYPE)logType , logDir );

	try
	{
		NMEA0183.Create(100);					//���Ƃ�
		SerialPort.Open(port , speed   );
	}
	catch(RException e)
	{
		throw ;
	}
}


//GPS�𐧌䂷�邽�߂̃��b�Z�[�W�𑗐M���܂�.
void RGISGPS::SendGPSControlMessage(const string & inMessage) 
{
	RAutoLock al(&Lock);
	SerialPort.Write(inMessage.c_str() , inMessage.size() );
}


bool RGISGPS::getParam(LPOINT * xy, int * outSpeed , int * outDirection ,
					   SatellitesState outSatellitesState[SATELLITES_MAX]) const
{
	RAutoLock al(&Lock);

	return 
		NMEA0183.getParam(xy , outSpeed , outDirection ) &&	NMEA0183.getSatellites(outSatellitesState);
}

void RGISGPS::HoldUp()
{
	SerialPort.Close();

	RThread::HoldUp();
}

//���O���擾.
const string RGISGPS::getTailLog() const
{
	RAutoLock al(&Lock);

	return this->NMEA0183LogRingBuffer.getBuffer();
}



unsigned int RGISGPS::Run()
{
	char oneBuffer[NMEA0183_RESERVE_BUFFER_SIZE + 1];

	

	//��s�����o���܂�.
	RStream stream;
	stream.Create(&SerialPort, NMEA0183_RESERVE_BUFFER_SIZE * 2);

	//��s�̏I���̌��o.
	StreamCheckerForReturnCodeWithThread lineEnd(this);

	try
	{
		while( IsHoldUp() )
		{
			stream.Read(oneBuffer , NMEA0183_RESERVE_BUFFER_SIZE , &lineEnd);
			if (!IsHoldUp()) break;

			//�I�[������.
			oneBuffer[lineEnd.getNext() + 1] = '\0';
			LogFile.Logout(oneBuffer);

			try
			{
				//���.
				RAutoLock al(&Lock);

				NMEA0183LogRingBuffer.Push(oneBuffer , lineEnd.getNext() );
				NMEA0183.Analize(oneBuffer);
			}
			catch(RException e)
			{
				TRACE( e.getMessage() );
			}
		}
	}
	catch(RException e)
	{
	}

	SerialPort.Close();

	return 1;
}

