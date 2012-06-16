#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "cJSON.h"

#ifdef _MSC_VER
#define stat _stat
#ifndef _CRT_SECURE_NO_WARNINGS
  #define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#define CFG(x) cJSON_GetObjectItem(cfg,x)
#define CFGi(x) cJSON_GetObjectItem(cfg,x)->valueint
#define CFGs(x) cJSON_GetObjectItem(cfg,x)->valuestring
#define CFG_FILE "csv2cell.json"

#define CFG_COUNT 6
#define CFG_FIELDS {"cell_size", "column_count", "control_column", "separator", "cell_style", "row_style"}

  struct cJSON *cfg;
  struct {
    int cell_size;
    int column_count;
    int ctrl_column;
    char separator;
    char* row_style;
    char* cell_style;    
  } cf;

  FILE *in = NULL,*out = NULL;
  char **Cells;
  /*
   * Yeah, no buffer overflow checks, screw me
   */
void initCells(){
  int i;
  Cells = (char **)malloc(cf.column_count*sizeof(char *));
  for (i=0;i<cf.column_count;i++){
    Cells[i] = (char *)malloc(cf.cell_size);
  }
}

void freeCells(){
  int i;
  for(i=0;i<cf.column_count;i++)
     free(Cells[i]);
  free(Cells);
}

void setConfig(){
  cf.cell_size = CFGi("cell_size");
  cf.column_count = CFGi("column_count");
  cf.ctrl_column = CFGi("control_column")-1;
  cf.separator = CFGs("separator")[0];
  cf.row_style = CFGs("row_style");
  cf.cell_style = CFGs("cell_style");
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
 int i, res = 1;
 char *names[] = CFG_FIELDS;
 for (i=0;i<CFG_COUNT;i++){
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
int i=0; 
do{
  Buf[i++] = fgetc(in);
}while(i<cf.cell_size && Buf[i-1]!=TheEnd && !feof(in));
Buf[i-1]='\x0';
}

void yieldRow(){
  int i;
  for(i=0;i<cf.column_count-1;i++) 
    loadText(Cells[i],cf.separator);
  loadText(Cells[cf.column_count-1],'\n');
  if (cf.ctrl_column>=0 && Cells[cf.ctrl_column][0]=='*')
    fprintf(out,"[[row style=\"%s\"]]",cf.row_style);
  else
    fprintf(out,"[[row]]");
  for(i=0;i<cf.column_count;i++){ 
    if (i!=cf.ctrl_column)
      fprintf(out,"[[cell style=\"%s\"]]%s[[/cell]]",cf.cell_style,Cells[i]);
  }
  fprintf(out,"[[/row]]\n");
}


int main(int argc, char *argv[]){
  if (argc==2 && argv[1][0]=='-' && argv[1][1]=='h'){
    puts("Usage: conv <file name>");
    return 0;
  }
  
  if (!loadConfig()) return 2;
  setConfig();

  in = stdin; out = stdout;

  if (argc>1){
    in = fopen(argv[1],"rt");
    if (!in) return 2;
  }
  if (argc>2){
    out= fopen(argv[2],"wt");
    if (!out) return 2;
  }

  initCells();
  fprintf(out,"[[table style=\"border-collapse:collapse\"]]\n");
  do{
    yieldRow();
  }while(!feof(in));
  fprintf(out,"[[/table]]\n");
  freeCells();
  fclose(in);
  fclose(out);

return 0;
}
