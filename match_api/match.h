#define TRUE 1
#define FALSE 0
#define HEADER_NUM 100
void match_init(); //  read ini file
int match(const char * pload,int len,char ** source_path); 
// match the pload whose length is len,if matched return 1(TRUE), 
// and source_path will point to a response file path.
// else return 0(FALSE) 
// ** len not include '\0'
