#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include<glob.h>
#include"grep.h"

int main(int argc, char* argv[]) {
  if (argc < 3) {                                                               // if loop to check if arguments are less than 3 then throw an error
    fprintf(stderr, "Usage: ./edgrep search_string filename\n");
    printf("%d\n", argc );
    exit(1);
  }
else if ((argc > 3) || (argc == 3)) {                                           // if loop to check if its a single file or a directory
  zero = (unsigned *)malloc(nlall * sizeof(unsigned));                          // init zero
  tfname = mkdtemp(tmpXXXXX);  init();
  const char* search_for = argv[1];                                             // array of file name string
  for (size_t i = 2; i<argc; i++){ match = 0;
    process_dir(argv[i], search_for, search_file);
    if (match == 1) {printf("\tno match found....\n");}}                          // for loop to process multiple files
    }
    printf("\nquitting....\n");
 return 0;
}
///////////////////////////////functions///////////////////////////////////////
int advance(char *lp, char *ep) {  char *curlp;  int i;                         // Advance func check for the patteren in buffer
  for (;;) {                                                                    // cases for the line numbers
    switch (*ep++) {
      case CCHR:  if (*ep++ == *lp++) { continue; } return(0);
      case CDOT:  if (*lp++) { continue; }    return(0);
      case CDOL:  if (*lp==0) { continue; }  return(0);
      case CEOF:  loc2 = lp;  return(1);
      case CCL:   if (cclass(ep, *lp++, 1)) {  ep += *ep;  continue; }  return(0);
      case NCCL:  if (cclass(ep, *lp++, 0)) { ep += *ep;  continue; }  return(0);
      case CBRA:  braslist[*ep++] = lp;  continue;
      case CKET:  braelist[*ep++] = lp;  continue;
      case CBACK:
        if (braelist[i = *ep++] == 0) { error(Q); }
        if (backref(i, lp)) { lp += braelist[i] - braslist[i];  continue; }  return(0);
      case CBACK|STAR:
        if (braelist[i = *ep++] == 0) { error(Q); }  curlp = lp;
        while (backref(i, lp)) { lp += braelist[i] - braslist[i]; }
        while (lp >= curlp) {  if (advance(lp, ep)) { return(1); }
          lp -= braelist[i] - braslist[i];  }  continue;
      case CDOT|STAR:  curlp = lp;  while (*lp++) { } do {  lp--;
        if (advance(lp, ep)) { return(1); } } while (lp > curlp);  return(0);
      case CCHR|STAR:  curlp = lp;  while (*lp++ == *ep) { }  ++ep; do {  lp--;
         if (advance(lp, ep)) { return(1); } } while (lp > curlp);
      return(0);
      case CCL|STAR: case NCCL|STAR:  curlp = lp;  while (cclass(ep, *lp++, ep[-1] == (CCL|STAR))) { }  ep += *ep;
      do {  lp--;  if (advance(lp, ep)) { return(1); } } while (lp > curlp);  return(0);
      default: error(Q);
    }
  }
}

int append(int (*f)(void), unsigned int *a) {                                   // append func to joing two strings and and make a new buffer
  unsigned int *a1, *a2, *rdot;  int nline, tl;  nline = 0;  dot = a;
  while ((*f)() == 0) {
    if ((dol-zero)+1 >= nlall) {  unsigned *ozero = zero;  nlall += 1024;
      if ((zero = (unsigned *)realloc((char *)zero, nlall*sizeof(unsigned)))==NULL){
        error("MEM?");  onhup(0);  }
      dot += zero - ozero;  dol += zero - ozero;
    }
    tl = putline();  nline++; a1 = ++dol;
    a2 = a1+1;  rdot = ++dot;
    while (a1 > rdot) { *--a2 = *--a1; }  *rdot = tl;
  }
  return(nline);}

int backref(int i, char *lp) {
  char *bp;  bp = braslist[i];                                                  // fun for back refrence used in advance
  while (*bp++ == *lp++) { if (bp >= braelist[i])   { return(1); } }
    return(0);
}

void blkio(int b, char *buf, long (*iofcn)(int, void*, unsigned long)) {
  lseek(tfile, (long)b*BLKSIZE, 0);
  if ((*iofcn)(tfile, buf, BLKSIZE) != BLKSIZE) {  error(T);  }
}

int cclass(char *set, int c, int af) {  int n;
  if (c == 0) { return(0); }
  n = *set++;
  while (--n) { if (*set++ == c) { return(af); } }
  return(!af);
}

void compile(int eof) {                                                         // Compile func used to make a  new compiled buffer and stores into expbuf
  int c, cclcnt;  char *ep = expbuf, *lastep, bracket[NBRA], *bracketp = bracket;
  if ((c = getchr()) == '\n') { peekc = c;  c = eof; }
  if (c == eof) {  if (*ep==0) { error(Q); }  return; }
  nbra = 0;  if (c=='^') { c = getchr();  *ep++ = CCIRC; }
  peekc = c;  lastep = 0;
  for (;;) {
    if (ep >= &expbuf[ESIZE]) {expbuf[0] = 0;  nbra = 0;  error(Q); } c = getchr();
    if (c == '\n') { peekc = c;  c = eof; }
    if (c==eof) { if (bracketp != bracket) { expbuf[0] = 0;  nbra = 0;  error(Q); }
      *ep++ = CEOF;  return;  }
    if (c!='*') { lastep = ep; }
    switch (c) {
      case '\\':
        if ((c = getchr())=='(') {
          if (nbra >= NBRA) { expbuf[0] = 0;  nbra = 0;  error(Q); }
            *bracketp++ = nbra;  *ep++ = CBRA;
              *ep++ = nbra++;  continue;
        }
        if (c == ')') {  if (bracketp <= bracket) { expbuf[0] = 0;  nbra = 0;  error(Q); }
        *ep++ = CKET; *ep++ = *--bracketp;
          continue; }
        if (c>='1' && c<'1'+NBRA) { *ep++ = CBACK;  *ep++ = c-'1';  continue; }
        *ep++ = CCHR;  if (c=='\n') { expbuf[0] = 0;  nbra = 0;  error(Q); }
        *ep++ = c;  continue;
      case '.': *ep++ = CDOT;  continue;                                        // cases for the regexp
      case '*':  if (lastep==0 || *lastep==CBRA || *lastep==CKET) { *ep++ = CCHR;  *ep++ = c; }
      *lastep |= STAR; continue;
      case '$':  if ((peekc=getchr()) != eof && peekc!='\n') { *ep++ = CCHR;  *ep++ = c; }
        *ep++ = CDOL;  continue;
      case '[':  *ep++ = CCL; *ep++ = 0;  cclcnt = 1;
        if ((c=getchr()) == '^') {  c = getchr();  ep[-2] = NCCL; }
        do {
          if (c=='\n') { expbuf[0] = 0;  nbra = 0;  error(Q); }
          if (c=='-' && ep[-1]!=0) {
            if ((c=getchr())==']') { *ep++ = '-';  cclcnt++;  break; }
            while (ep[-1] < c) {  *ep = ep[-1] + 1;  ep++;  cclcnt++;
                if (ep >= &expbuf[ESIZE]) { expbuf[0] = 0;  nbra = 0;  error(Q); } }
              } *ep++ = c;  cclcnt++;
              if (ep >= &expbuf[ESIZE]) { expbuf[0] = 0;  nbra = 0;
              error(Q); }
        } while ((c = getchr()) != ']');
        lastep[1] = cclcnt;  continue;
       default:  *ep++ = CCHR;  *ep++ = c; }
  }
}

void error(char *s) {
  int c;  wrapp = 0;  listf = 0; listn = 0;
  putchar('?'); puts_(s);                                                       // Error func used to throw an error
  count = 0;  lseek(0, (long)0, 2);  pflag = 0;
  if (globp) { lastc = '\n'; }
  globp = 0; peekc = lastc;
  if(lastc) { while ((c = getchr()) != '\n' && c != EOF) { } }
  if (io > 0) { close(io);  io = -1; }
}

int execute(unsigned int *addr) {
  char *p1, *p2 = expbuf;  int c;                                                // execute func to find the pattren in the string line

  if (addr == (unsigned *)0) {
    if (*p2 == CCIRC) { return(0); }  p1 = loc2; }
     else if (addr == zero) { return(0); }
  else { p1 = getline_blk(*addr); }
  if (*p2 == CCIRC) {  loc1 = p1;  return(advance(p1, p2+1)); }
  if (*p2 == CCHR) {  c = p2[1];                                                 //fast check for first character
    do {  if (*p1 != c) {continue;}
      if (advance(p1, p2)) {  loc1 = p1;  return(1); }
    } while (*p1++);
    return(0);
  }
  do { if (advance(p1, p2)) {  loc1 = p1;  return(1);  }
} while (*p1++);
    return(0);
}

void exfile(void) {close(io); io = -1;}

char * getblock(unsigned int atl, int iof) {
  int off, bno = (atl/(BLKSIZE/2));  off = (atl<<1) & (BLKSIZE-1) & ~03;
  if (bno >= NBLK) {  lastc = '\n';  error(T);  }  nleft = BLKSIZE - off;
  if (bno==iblock) {  ichanged |= iof;  return(ibuff+off);  }
  if (bno==oblock)  { return(obuff+off);  }
  if (iof==READ) {
    if (ichanged) { blkio(iblock, ibuff, (long (*)(int, void*, unsigned long))write); }
    ichanged = 0;
    iblock = bno;
    blkio(bno, ibuff, read);  return(ibuff+off);
  }
  if (oblock>=0) {
    blkio(oblock, obuff, (long (*)(int, void*, unsigned long))write); }
  oblock = bno;
    return(obuff+off);
}

int getchr(void) {  char c;                                                     // getch_ func is used in getchr func to read a char from a shared buf throw ungetch_ func
  if ((lastc=peekc)) {  peekc = 0;  return(lastc); }
  if (globp) {  if ((lastc = *globp++) != 0) { return(lastc); }
  globp = 0;
    return(EOF); }
  if ((c =getch_()) < 0) { return(lastc = EOF); }
  return lastc = c & 0177;                                                      // if loop to see if its the end of the file
}

int getfile(void) {
  int c;  char *lp = linebuf, *fp = nextip;                                     // getfile func to read char from the file.txt and save that text in genbnuf[]
  do {
    if (--ninbuf < 0) {
      if ((ninbuf = (int)read(io, genbuf, LBSIZE)-1) < 0) {
        if (lp>linebuf) { *genbuf = '\n';  }
        else { return(EOF); }
      }
      fp = genbuf;  while(fp < &genbuf[ninbuf]) {
          if (*fp++ & 0200) { break; }}
      fp = genbuf;
    }
    c = *fp++;
    if (c=='\0') { continue; }
    if (c&0200 || lp >= &linebuf[LBSIZE]) {  lastc = '\n';  error(Q);  }
    *lp++ = c;  count++;
  } while (c != '\n');
  *--lp = 0;  nextip = fp;
    return(0);
}

char* getline_blk(unsigned int tl) {
  char *bp, *lp;  int nl;  lp = linebuf;
  bp = getblock(tl, READ);
  nl = nleft;
  tl &= ~((BLKSIZE/2)-1);                                                       // func to get a newline
  while ((*lp++ = *bp++)) {
    if (--nl == 0) {  bp = getblock(tl+=(BLKSIZE/2), READ);
      nl = nleft;  } }
  return(linebuf);
}

int getch_(void) {
   char c = (bufp > 0) ? buf[--bufp] : nomatch();                               // getch_ func to read character from a shared buf with ungetch_ func
   return lastc = c & 0177;
}

void global(int k)                                                              // global func to compile and print printcommand
 {  char *gp;  int c;  unsigned int *a1;  char globuf[GBSIZE];
  if (globp) { error(Q); }  setwide();
    squeeze(dol > zero);
  if ((c = getchr()) == '\n') { error(Q);}
   compile(c);  gp = globuf;
  while ((c = getchr()) != '\n') {  if (c == EOF) { error(Q); }
    if (c == '\\') {  c = getchr();
      if (c != '\n') { *gp++ = '\\'; }  }
    *gp++ = c;
    if (gp >= &globuf[GBSIZE-2]) { error(Q); }
  }
  if (gp == globuf) { *gp++ = 'p'; }
   *gp++ = '\n';
   *gp++ = 0;
  for (a1 = zero; a1 <= dol; a1++) {
      *a1 &= ~01;  if (a1>=addr1 && a1<=addr2 && execute(a1)==k) { *a1 |= 01; } }
  for (a1 = zero; a1 <= dol; a1++) {
    if (*a1 & 01) {  *a1 &= ~01;  dot = a1;
        globp = globuf;  printcommand();
          a1 = zero; }
  }
}

void init(void) {
  int *markp;  close(tfile);  tline = 2;                                        // init func to initilize
  for (markp = names; markp < &names[26]; )  {
      *markp++ = 0;  }
  subnewa = 0;  anymarks = 0;
  iblock = -1;  oblock = -1;
  ichanged = 0;
  close(creat(tfname, 0600));
  tfile = open(tfname, 2);
  dot = dol = zero;
  memset(inputbuf, 0, sizeof(inputbuf));
}

void newline(void) {  int c;
  if ((c = getchr()) == '\n' || c == EOF) { return;}                            // newline func for adding a \n when its called
  if (c == 'p' || c == 'l' || c == 'n') {  pflag++;
  if (c == 'l') { listf++;  }
  else if (c == 'n') { listn++; }
  if ((c = getchr()) == '\n') { return; }
  }  error(Q);
}

void nonzero(void) { squeeze(1); }

void onhup(int n) {
  signal(SIGINT, SIG_IGN);  signal(SIGHUP, SIG_IGN);
  if (dol > zero) {  addr1 = zero+1; addr2 = dol;
      io = creat("ed.hup", 0600);
    if (io > 0) { putfile(); } }
  fchange = 0;
  quit(0);
}

char nomatch() {                                                                // func to set a flag for the match if not found
  match =1;
  return putchar(' ');
}

void onintr(int n) {
  signal(SIGINT, onintr);  putchar('\n');
  lastc = '\n';
    error(Q);
}

void process_dir(const char* dir, const char* searchfor,                        // func to process directory and the multiple files
                 void (*fp)(const char*, const char*)) {
  if (strchr(dir, '*') == NULL) {                                               // strchr search for * in the filename to see if its a directory
    search_file(dir, searchfor);
    return;
  }                                                                             // or search a directory of files using glob()
  glob_t results;
  memset(&results, 0, sizeof(results));
  glob(dir, 0, NULL, &results);

  printf("\nfilename: %s\n", dir);
  for (int i = 0; i < results.gl_pathc; ++i) {                                  // if its a directory for loop to get all the names.txt to read
    const char* filename = results.gl_pathv[i];
    fp(filename,searchfor);                                                     // function ptr to function that reads and searches a file
  }
  globfree(&results);
}

void print(void) {
  unsigned int *a1 = addr1;  nonzero();                                         // func to print the characters
  do {  if (listn) {  count = a1 - zero; }
  puts_(getline_blk(*a1++));  } while (a1 <= addr2);
  dot = addr2;
  listf = 0;
  listn = 0;
  pflag = 0;
}

void printcommand(void) {                                                       // printcommand func to print if the regexp pattren is found and then a EOF case to close the func
  int c;  char lastsep;
   for (;;) {  unsigned int* a1;
     if (pflag) { pflag = 0;  addr1 = addr2 = dot;  print(); }
       c = '\n';
     for (addr1 = 0;;) {  lastsep = c;
        a1 = 0;
        c = getchr();
       if (c != ',' && c != ';') { break; }
       if (lastsep==',') { error(Q); }
       if (a1==0) {  a1 = zero+1;
           if (a1 > dol) { a1--; }  }
       addr1 = a1;  if (c == ';') { dot = a1; }
     }
     if (lastsep != '\n' && a1 == 0) { a1 = dol; }
     if ((addr2 = a1)==0) { given = 0;  addr2 = dot;  }
      else { given = 1; }
     if (addr1==0) { addr1 = addr2; }
     switch(c) {                                                                // switch statements for p case aka print and EOF case aka end of file
       case 'p':  case 'P': printf("\t"); print(); newline();
       case EOF:  default:  return; }
   }
}
                                                                                // putfile function to put a file into a buffer
void putfile(void) {
  unsigned int *a1;  char *fp, *lp;  int n, nib = BLKSIZE;
  fp = genbuf;  a1 = addr1;
  do {
    lp = getline_blk(*a1++);
    for (;;) {
      if (--nib < 0) {
        n = (int)(fp-genbuf);
        if (write(io, genbuf, n) != n) {  puts_(WRERR);  error(Q);  }
        nib = BLKSIZE-1;
        fp = genbuf;
      }
      count++;
      if ((*fp++ = *lp++) == 0) {  fp[-1] = '\n';  break; }
    }
  } while (a1 <= addr2);
  n = (int)(fp-genbuf);
  if (write(io, genbuf, n) != n) {  puts_(WRERR);  error(Q); }
}
                                                                                // putline func to put a line
int putline(void) {
  char *bp, *lp;  int nl;  unsigned int tl;
  fchange = 1;
  lp = linebuf;
  tl = tline;
  bp = getblock(tl, WRITE);
  nl = nleft;
  tl &= ~((BLKSIZE/2)-1);
  while ((*bp = *lp++)) {
    if (*bp++ == '\n') {  *--bp = 0;  linebp = lp;  break;  }
    if (--nl == 0) {  bp = getblock(tl += (BLKSIZE/2), WRITE);
       nl = nleft;}
  }
  nl = tline;
  tline += (((lp - linebuf) + 03) >> 1) & 077776;
  return(nl);
}                                                                               // puts_ func to print a character and then put a \n at the end
void puts_(char *sp) {
   col = 0;
   while (*sp) { putchar(*sp++); }
   putchar('\n');
  }
void quit(int n) {
  if (vflag && fchange && dol!=zero) {  fchange = 0;  error(Q);  }
    unlink(tfname);
     exit(0);
    }                                                                            // func to quit the procedure

void searchfile_(const char * c){                                               // search file func to search for the regexp into the stored txt file
  char buf[GBSIZE];
  snprintf(buf, sizeof(buf), "/%s\n", c);
  const char* p = buf + strlen(buf) - 1;
  while (p >= buf) { ungetch_(*p--); }                                          // ungetch_ to store the the characters into a buf and the using getch to read them
  global(1);                                                                    // caling global
}

void search_file(const char* filename, const char* searchfor) {                 //search file fun called in directory if argc is =3 or >3 and the filename string didnt had any *.txt
  printf("\nfilename: %s\n", filename);
  readfile_(filename);
  searchfile_(searchfor);
  printcommand();
}

void setnoaddr(void) {
   if (given) { error(Q); }
 }                                                                              // set no address func to throw error if no address is found

void setwide(void) {
  if (!given) { addr1 = zero + (dol>zero);  addr2 = dol; }
 }

void squeeze(int i) {
  if (addr1 < zero+i || addr2 > dol || addr1 > addr2) { error(Q); }
 }

void readfile_ (const char* c){                                                 // one of the main func to read the filename string from the buf argv[]
  setnoaddr();
  strcpy(file, c);
  strcpy(savedfile, c);
  init(); addr2 = zero;
  if ((io = open((const char*)file, 0)) < 0) { lastc = '\n';  error(file);}     // open the file
  setwide(); squeeze(0); ninbuf = 0;
  append(getfile, addr2); exfile(); fchange = 'e';                              // getfile func called in appened , which get the text from the open file
}

void ungetch_(int c) {                                                          // ungetch fucn to store the characters of txt file into buf[], which are used by getch later
  if (bufp >= BUFSIZE) { printf("ungetch: overflow\n");}
  else { buf[bufp++] = c;}
}
