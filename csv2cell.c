#include <stdio.h>

#define BUF_SIZE 512
  FILE *in = NULL,*out = NULL;

  /*
   * Yeah, no buffer overflow checks, screw me
   */
void loadText(char *Buf,char TheEnd){
int i=0;
do{
  Buf[i++] = fgetc(in);
}while(Buf[i-1]!=TheEnd && Buf[i-1]!=EOF);
Buf[i-1]='\x0';
#ifdef DEBUG
printf("i=%d\n",i);
puts(Buf);
getch();
#endif
}

#define CELL_SIZE 512
void yieldRow(){
  char Cell1[CELL_SIZE];
  char Cell2[CELL_SIZE];
  char Cell3[CELL_SIZE];
  char CellX[8];
  loadText(Cell1,';');
  loadText(Cell2,';');
  loadText(Cell3,';');
  loadText(CellX,'\n');
  if (CellX[0]=='*')
    fprintf(out,"[[row style=\"background-color:rgb(191,244,174);\"]]");
  else
    fprintf(out,"[[row]]");
  fprintf(out,"[[cell style=\"border: 1px solid silver;\"]]%s[[/cell]]",Cell1);
  fprintf(out,"[[cell style=\"border: 1px solid silver;\"]]%s[[/cell]]",Cell2);
  fprintf(out,"[[cell style=\"border: 1px solid silver;\"]]%s[[/cell]]",Cell3);
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
  do{
    yieldRow();
  }while(!feof(in));
  fclose(in);
  fclose(out);

return 0;
}
