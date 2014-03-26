/* Portable file/dir functions */
/* OS/2 and DOS */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef OS2
/* OS/2 and VisAge C++ */
#define INCL_DOSFILEMGR   /* File Manager values */
#define INCL_DOSERRORS    /* DOS error values */
#include <os2.h>
#else
/* MSDOS and MS VC++ */
#include <dos.h>

#endif

#include "config.h"

#define MAX_DIR_OPENS	50

#ifdef	OS2
static	FILEFINDBUF3  FindBuffer[MAX_DIR_OPENS]     = {0};
static	HDIR          hdirFindHandle[MAX_DIR_OPENS] =
{
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};
static	ULONG         ulFindCount[MAX_DIR_OPENS];
static	APIRET        rc             = NO_ERROR; /* Return code */
#define FNAME	FindBuffer[dh].achName
#else
static	struct _find_t  file;
static 	int	rc;
#define FNAME	file.name
#endif

long open_dir (char * f_spec, char * fname) {
	long	dh;
#ifdef	OS2
	for (dh = 0; dh < MAX_DIR_OPENS && hdirFindHandle[dh] != -1; ++dh);
	if (dh >= MAX_DIR_OPENS)
		return (-1);

	hdirFindHandle[dh] = -1;
	ulFindCount[dh] = 1;
	rc = DosFindFirst( (PSZ)f_spec, &hdirFindHandle[dh],
		(ULONG)FILE_DIRECTORY, &FindBuffer[dh],
		(ULONG)sizeof(FILEFINDBUF3),
		&ulFindCount[dh], FIL_STANDARD);
	if (rc) {
		hdirFindHandle[dh] = -1;
		return (-1);
	}

        strcpy (fname, FNAME);

	return dh;
#else
	rc = _dos_findfirst (f_spec, _A_NORMAL, &file );
	if (!rc) dh = (void *)file;
	else dh = -1;

	if (dh >= 0)
	        strcpy (fname, FNAME);

	return dh;
#endif
}

int	next_dir (long dh, char * fname) {
#ifdef	OS2
	rc = !DosFindNext ((HDIR)hdirFindHandle[dh], &FindBuffer[dh],
		(ULONG)sizeof(FILEFINDBUF3), &ulFindCount[dh]);
#else
	rc = !_dos_findnext ((struct _find_t *)dh);
#endif
	if (rc)
	        strcpy (fname, FNAME);

	return rc;
}

void	close_dir (long dh) {
#ifdef	OS2
	DosFindClose((HDIR)hdirFindHandle[dh]);
	hdirFindHandle[dh] = 0xFFFFFFFF;
#endif
}
