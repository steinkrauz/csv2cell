#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "cJSON.h"

#ifdef _MSC_VER
#define stat _stat
#define _CRT_SECURE_NO_WARNINGS
#endif

#define CFG(x) cJSON_GetObjectItem(cfg,x)
#define CFGi(x) cJSON_GetObjectItem(cfg,x)->valueint
#define CFGs(x) cJSON_GetObjectItem(cfg,x)->valuestring
#define CFG_FILE "csv2cell.json"

/*
#define CELL_SIZE 512
#define CELL_COUNT 4
#define SEPARATOR ';'
#define CONTROL_COL 3
#define ROW_STYLE "background-color:rgb(191,244,174);"
#define CELL_STYLE "border: 1px solid silver;"
*/

  struct cJSON *cfg;

  FILE *in = NULL,*out = NULL;
  char **Cells;
  /*
   * Yeah, no buffer overflow checks, screw me
   */
void initCells(){
  int i, columns, csize;
  columns = CFGi("cell_count");
  csize = CFGi("cell_size");
  Cells = (char **)malloc(columns*sizeof(char *));
  for (i=0;i<columns;i++){
    Cells[i] = (char *)malloc(csize);
  }
}

void freeCells(){
  int i, columns;
  columns = CFGi("cell_count");
  for(i=0;i<columns;i++)
     free(Cells[i]);
  free(Cells);
}

int getFileSize(char *filename){
  struct stat buf;
  if (stat(filename, &buf)==0){
     return buf.st_size;
  } else {
  return -1;
  }
}

int checkConfig(){
 int i,par_count = 6, res = 1;
 char *names[] = {"cell_size", "cell_count", "control_column", "separator", "cell_style", "row_style"};
 for (i=0;i<par_count;i++){
   if (CFG(names[i])==NULL){
      res = 0;
      printf("Parameter %s missed.\n",names[i]);
   }
 }
 return res;
}

int loadConfig(){
  int fsize;
  char *cfgText;
  fsize = getFileSize(CFG_FILE);

  in = fopen(CFG_FILE,"rt");
  cfgText = (char *)malloc(fsize);
  fread(cfgText,fsize,1,in);
  fclose(in);
  cfg = cJSON_Parse(cfgText);
  if (!cfg) {printf("Error before: [%s]\n",cJSON_GetErrorPtr()); return 0;}
  
  return checkConfig();
}
  
void loadText(char *Buf,char TheEnd){
int i=0; char ch;
do{
  Buf[i++] = fgetc(in);
}while(Buf[i-1]!=TheEnd && !feof(in));
Buf[i-1]='\x0';
}

void yieldRow(){
  int i,columns, ctrl_col;
  char separator;
  char *row_style, *cell_style;
  columns = CFGi("cell_count");
  ctrl_col = CFGi("control_column");
  separator = CFGs("separator")[0];
  row_style = CFGs("row_style");
  cell_style = CFGs("cell_style");
  for(i=0;i<columns-1;i++) 
    loadText(Cells[i],separator);
  loadText(Cells[columns-1],'\n');
  if (ctrl_col>=0 && Cells[ctrl_col][0]=='*')
    fprintf(out,"[[row style=\"%s\"]]",row_style);
  else
    fprintf(out,"[[row]]");
  for(i=0;i<columns;i++){ 
    if (i!=ctrl_col)
      fprintf(out,"[[cell style=\"%s\"]]%s[[/cell]]",cell_style,Cells[i]);
  }
  fprintf(out,"[[/row]]\n");
}


int main(int argc, char *argv[]){
  if (argc==2 && argv[1][0]=='-' && argv[1][1]=='h'){
    puts("Usage: conv <file name>");
    return 0;
  }
  
  if (!loadConfig()) return 2;

  if (argc>1){
    in = fopen(argv[1],"rt");
    puts("dd");
  }
  if (!in) in = stdin;
  if (argc>2){
    out= fopen(argv[2],"wt");
  }
  if (!out) out = stdout;

  initCells();
  do{
    yieldRow();
  }while(!feof(in));
  freeCells();
  fclose(in);
  fclose(out);

return 0;
}
