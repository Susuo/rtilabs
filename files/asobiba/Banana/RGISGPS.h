// RGISGPS.h: RGISGPS �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGISGPS_H__3923DE27_D898_4504_9D2B_0B93055782D9__INCLUDED_)
#define AFX_RGISGPS_H__3923DE27_D898_4504_9D2B_0B93055782D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RGISNMEA0183.h"
#include "RStdioSerialPort.h"
#include "RThread.h"
#include "RLock.h"
#include "RStdioFile.h"
#include "RListenner.h"
#include "RGISGPSLog.h"
#include "RStringRingBuffer.h"

const NMEA0183_RESERVE_BUFFER_SIZE	=	128;

class RGISGPS  : public RThread
{
public:
	RGISGPS();
	virtual ~RGISGPS();

	//�쐬.
	void Create(int port ,int speed ,int type  ,int logType ,const string & logDir );
	//�X���b�h���C��.
	virtual unsigned int Run();

	//GPS����̐M������͂�������.
	bool getParam(LPOINT * xy, int * outSpeed , int * outDirection ,
					SatellitesState outSatellitesState[SATELLITES_MAX]) const;

	//���O���擾.
	const string getTailLog() const;

	//GPS�𐧌䂷�邽�߂̃��b�Z�[�W�𑗐M���܂�.
	void SendGPSControlMessage(const string & inMessage) ;

	virtual void HoldUp();

private:
	RStdioSerialPort	SerialPort;
	RGISNMEA0183		NMEA0183;
	mutable	RLock		Lock;
	RGISGPSLog			LogFile;

	RStringRingBuffer	NMEA0183LogRingBuffer;
};

#endif // !defined(AFX_RGISGPS_H__3923DE27_D898_4504_9D2B_0B93055782D9__INCLUDED_)
