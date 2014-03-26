/* deltree.c:
 * delete diretory
 * written for OS/2 Visual Age C++
 *
 * History:
 *  8 Sep 00 MT: Allow multiple directories/files.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <direct.h>
#include <sys\types.h>
#include <sys\stat.h>

#include "config.h"
#include "file.h"

static long	verbose = 0;
static long	printonly = 0;
static long	fullpath = 0;
static long	noyes = 0;
static long	ignore_timestamp = 0;

#define DELTREE_DEBUG	0

static void
usage (char *prog)
{
	printf ("OS/2 Deltree\n");
	printf ("By Michael Taylor  miket@interact.net.au\n");
	printf ("Usage: %s directory\n", prog);
	printf ("Options:\n");
	printf ("\t-y\tdon't ask\n");
	printf ("\t-v\tverbose\n");
	printf ("\t-q\tquiet\n");
	printf ("\t-p\tprint only - no execute\n");
}

static void
process_file (char *srcfile)
{
	struct stat	stat_buf1;

	if (verbose > 1)
		printf ("deleting file %s\n", srcfile);

	if (0 != stat (srcfile, &stat_buf1))
		return;

	if (!printonly) {
		if (stat_buf1.st_mode & S_IFREG) {
			if (remove (srcfile))	 /* file */
				printf (
"could not delete file %s\n", srcfile);
		} else {
			if (rmdir (srcfile))	 /* directory */
				printf (
"could not delete directory %s\n", srcfile);
		}
	} else
		printf ("would delete file/dir %s\n", srcfile);

	return;
}

static void
do_dir (char *srcpath)
{
	char		dir[256], f_spec[256], file[256], newname[256];
	char		testname[256];
	char		outdir[256];
	long		dirh;
	struct stat	stat_buf;
	char		*p;

	strcpy (dir, srcpath);
	strcat (dir, DIRSEP);
	strcat (dir, "*.*");
	if (-1 == (dirh = open_dir (dir, file))) {
	        printf ("Directory empty or does not exist: %s\n", dir);
		return;
	}

	while (next_dir (dirh, file)) {
		strcpy (f_spec, srcpath);
		strcat (f_spec, DIRSEP);
		strcat (f_spec, file);
		stat (f_spec, &stat_buf);
		if (stat_buf.st_mode & S_IFREG) { /* file */
			process_file (f_spec);
		} else if (stat_buf.st_mode & S_IFDIR) { /* directory */
			if (!strcmp (file, ".") || !strcmp (file, ".."))
				continue;
			do_dir (f_spec);
			if (printonly)
				printf ("would delete dir %s\n", f_spec);
			process_file (f_spec);
		}
	}
	close_dir (dirh);

	return;
}

int
main (int argc, char **argv)
{
	char		*rdir, f_spec[256];
	struct stat	stat_buf;
	int		i;
	int		rind;
	int		nparms;

	rdir = NULL;
	rind = -1;

	for (nparms = 0, i = 1; i < argc; ++i) {
		if (argv[i][0] == '?' || argv[i][1] == '?') {
			usage (argv[0]);
			exit (1);
		}

		if (!(argv[i][0] == '-') && !(argv[i][0] == '/')) {
			++nparms;
			continue;
		}
		switch (tolower (argv[i][1])) {
		case 'y':
			++noyes;
			break;
		case 'p':
			++printonly;
			break;
		case 'q':
			verbose = 0;
			break;
		case 'v':
			++verbose;
			break;
		default:
			fprintf (stderr, "Unknown option \"%s\"",
				argv[i]);
			usage (argv[0]);
			exit (1);
			break;
		}
	}
	if (!nparms) {
		usage (argv[0]);
		exit (1);
	}
	for (i = 1; i < argc; ++i) {
		if (!(argv[i][0] == '-') && !(argv[i][0] == '/')) {
			rdir = argv[i];

			if (verbose) {
				printf ("delete directory is     %s\n", rdir);
			}

			if (!noyes && !printonly) {
				int x;

				printf ("Are you sure? Y/N\n");
				x = _getche ();
				if (toupper(x) != 'Y')
					exit (0);
				printf ("\n");
			}

			do_dir (rdir);
			if (printonly)
				printf ("would delete dir %s\n", f_spec);
			process_file (rdir);

		}
	}
	if (NULL == rdir) {
		usage (argv[0]);
		exit (1);
	}

	exit (0);
	return (0);
}
