#define BUFSIZE 100
typedef void (*SIG_TYP)(int);
                                                                                // all the varibales declared and initilized here
const int BLKSIZE = 4096 ,NBLK = 2047 , FNSIZE = 128, LBSIZE = 4096, ESIZE = 256
,GBSIZE = 256, NBRA = 5, KSIZE = 9, CBRA = 1, CCHR = 2, CDOT = 4, CCL =6, NCCL = 8,
CDOL = 10, CEOF = 11, CKET = 12, CBACK = 14, CCIRC = 15, STAR = 01, READ = 0,
WRITE = 1;

int bufp = 0, peekc, lastc, given, ninbuf, io, pflag, match;
int vflag = 1, oflag, listf, listn, col, tfile = -1, tline, iblock = -1,
    oblock = -1, ichanged, nleft;
int names[26], anymarks, nbra, subnewa, subolda, fchange, wrapp, bpagesize = 20;
unsigned nlall = 128;
unsigned int *addr1, *addr2, *dot, *dol, *zero;

long count;
char Q[] = "", T[] = "TMP", savedfile[FNSIZE], file[FNSIZE], linebuf[LBSIZE],
     rhsbuf[LBSIZE / 2], expbuf[ESIZE + 4];
char genbuf[LBSIZE], *nextip, *linebp, *globp, *mkdtemp(char *),
    tmpXXXXX[50] = "/tmp/eXXXXX";
char *tfname, *loc1, *loc2, ibuff[BLKSIZE], obuff[BLKSIZE],
    WRERR[] = "WRITE ERROR", *braslist[NBRA], *braelist[NBRA];
char line[70], buf[BUFSIZE], inputbuf[GBSIZE];
                                                                                // all the function names are defined here
void printcommand(void);
void add(int i);
int advance(char* lp, char* ep);
int append(int (*f)(void), unsigned int* a);
int backref(int i, char* lp);
void blkio(int b, char* buf, long (*iofcn)(int, void*, unsigned long));
int cclass(char* set, int c, int af);
void compile(int eof);
void error(char* s);
int execute(unsigned int* addr);
void exfile(void);
char* getblock(unsigned int atl, int iof);
int getchr(void);
int getfile(void);
char* getline_blk(unsigned int tl);
char nomatch();
void global(int k);
void init(void);
void newline(void);
void nonzero(void);
void onhup(int n);
void onintr(int n);
void print(void);
void putfile(void);
int putline(void);
void puts_(char* sp);
void quit(int n);
void setwide(void);
void setnoaddr(void);
void squeeze(int);
void readfile_(const char* c);
void searchfile_(const char* c);
void global(int k);
int getch_(void);
void ungetch_(int c);
void search_file(const char* filename, const char* searchfor);
void process_dir(const char* dir, const char* searchfor,
                 void (*fp)(const char*, const char*));
