/*****************************************************************/
/*****************************************************************/
// �O�����h�ȃf�o�b�O���[�`��
/*****************************************************************/
/*****************************************************************/
#include <StartWithExeFile.h>
#ifdef DEBUG_LIFE

#include <windows.h>
#include <stdio.h>
#include <.\Base\rti_debug.h>

#define DEBUGFILE "DEBUG.TXT"


void DEBUG(char *str)
{
	FILE *fp;
	fp = fopen(DEBUGFILE,"a+");
	if (fp==NULL) return;
	fprintf(fp,"%s\r\n",str);
	fclose(fp);
}


void DEBUG2(int i)
{
	FILE *fp;
	fp = fopen(DEBUGFILE,"a+");
	if (fp==NULL) return;
	fprintf(fp,"%d\r\n",i);
	fclose(fp);
}

void DEBUG3(char* Format,...)
{

	FILE *fp;
	fp = fopen(DEBUGFILE,"a+");
	if (fp==NULL) return;
	vfprintf(fp,Format,(char*)(&Format+1));
	fputs("\r\n",fp);
	fclose(fp);

}

//���Ԍv�����s���ꍇ.
static unsigned long Max = 0;			//�ő僌�[�g(�ň��̌���)
static unsigned long Min = 0xffff;		//�Œ჌�[�g(�ō��̌���)
static unsigned long Avg = 100;			//���σ��[�g
void NewRecordFPS(unsigned long rec)
{
	Avg = (Avg + rec) /2;
	if (Max < rec) Max = rec;
	if (Min > rec) Min = rec;
}

void WriteRecordFPS(unsigned long DesignatedTime)
{
	DEBUG3(" �w�背�[�g     %ld \n �u�ԍő僌�[�g %ld "
				"\n �u�ԍŒ჌�[�g %ld \n ���σ��[�g     %ld",DesignatedTime,Max,Min,Avg);
}


#endif // LIFE END
