#include <stdio.h>

#define CELL_SIZE 512
#define CELL_COUNT 4
#define SEPARATOR ';'
#define CONTROL_COL 3
#define ROW_STYLE "background-color:rgb(191,244,174);"
#define CELL_STYLE "border: 1px solid silver;"
  
  FILE *in = NULL,*out = NULL;
  char **Cells;
  /*
   * Yeah, no buffer overflow checks, screw me
   */

void initCells(){
  int i;
  Cells = (char **)malloc(CELL_COUNT*sizeof(char *));
  for (i=0;i<CELL_COUNT;i++){
    Cells[i] = (char *)malloc(CELL_SIZE);
  }
}

void freeCells(){
  int i;
  for(i=0;i<CELL_COUNT;i++)
     free(Cells[i]);
  free(Cells);
}
  
void loadText(char *Buf,char TheEnd){
int i=0;
do{
  Buf[i++] = fgetc(in);
}while(Buf[i-1]!=TheEnd && !feof(in));
Buf[i-1]='\x0';
#ifdef DEBUG
printf("i=%d\n",i);
puts(Buf);
getch();
#endif
}

void yieldRow(){
  int i;
  for(i=0;i<CELL_COUNT-1;i++) 
    loadText(Cells[i],SEPARATOR);
  loadText(Cells[CELL_COUNT-1],'\n');
  if (Cells[CONTROL_COL][0]=='*')
    fprintf(out,"[[row style=\"%s\"]]",ROW_STYLE);
  else
    fprintf(out,"[[row]]");
  for(i=0;i<CELL_COUNT;i++){ 
    if (i!=CONTROL_COL)
    fprintf(out,"[[cell style=\"%s\"]]%s[[/cell]]",CELL_STYLE,Cells[i]);
  }
  fprintf(out,"[[/row]]\n");
}


int main(int argc, char *argv[]){
  if (argc==2 && argv[1][0]=='-' && argv[1][1]=='h'){
    puts("Usage: conv <file name>");
    return 0;
  }
  if (argc>1){
    in = fopen(argv[1],"rt");
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
