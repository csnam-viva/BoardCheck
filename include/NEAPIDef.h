#ifndef _NEAPIDEF
#define _NEAPIDEF

#define API_ST_NOTHING			1
#define API_ST_LOADED			2
#define API_ST_READY			3
#define API_ST_RUN				4

#define MAX_SPLIT_CONF			36

#define CAPBOARDSN				0
#define AUDIOBOARDSN			1
#define OVERLAYBOARDSN			2

#define HTTP_GET				1
#define HTTP_POST				2
#define HTTP_SET				3

/////////////////////////////////////////////////////////////////////////////////////////
// device dependent library interfaces

//	UDA5 interface
//#include <Initguid.h>
#include "Cmn5BoardLibEx.h"
#include "Net5BoardLibEx.h"
#include "Aud5BoardLibEx.h"
#include "Cod5BoardLibEx.h"
#include "Cap5BoardLibEx.h"
#include "Ovr5BoardLibEx.h"

typedef BOOL (__stdcall *FUDACmn5CreateInstance)(PVOID *pUnknown);
typedef struct {
	INet5		*iNet5;
	IAud5		*iAud5;
	ICod5		*iCod5;
	ICap5		*iCap5;
	IOvr5		*iOvr5;
} LIBUDAIF, *LPLIBUDAIF;

// end of device dependent library interfaces
/////////////////////////////////////////////////////////////////////////////////////////

#endif // _NEAPIDEF
