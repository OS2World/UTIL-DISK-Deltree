/* config.h */

#ifdef OS2
#define RT	"r"
#define WT	"w"
#define RB	"r"
#define WB	"wb"
#define DIRSEP	"\\"

#elif UNIX
#define RT	"rt"
#define WT	"wt"
#define RB	"rb"
#define WB	"wb"
#define DIRSEP	"/"

#else
#define RT	"rt"
#define WT	"wt"
#define RB	"rb"
#define WB	"wb"
#define DIRSEP	"\\"

#endif
