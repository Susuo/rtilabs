#include <StartWithExeFile.h>
#ifdef DEBUG_LIFE

#ifndef ___DEBUGH
#define ___DEBUGH

void DEBUG(char *str);
void DEBUG2(int i);
void DEBUG3(char* Format,...);

//���Ԍv�����s���ꍇ.
#ifdef TIME_CHECK

void NewRecordFPS(unsigned long rec);
void WriteRecordFPS(unsigned long DesignatedTime);
#define NOW_TIME(rec) NewRecordFPS(rec)
#define END_TIME(rec) WriteRecordFPS(rec)

#else

#define NOW_TIME(rec) /* */
#define END_TIME(rec) /* */

#endif	//���Ԍv�� END



#endif	//DEBUG END

#endif //LIEF END
