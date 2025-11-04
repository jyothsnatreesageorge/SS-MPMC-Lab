#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
   FILE *f1,*f2,*f3,*f4,*f5;
   f1=fopen("input.txt","r");
   f2=fopen("optab.txt","r");
   f3=fopen("intermediate.txt","w");
   f4=fopen("symtab.txt","w");
   f5=fopen("length.txt","w");
   if(!f1||!f2||!f3||!f4||!f5)
   {
      printf("Error in opening files!\n");
      return -1;
   }
   char label[10],opcode[10],operand[10],opcode1[10],operand1[10];
   int x,start,locctr;
   while(fscanf(f1,"%s%s%s",label,opcode,operand)!=EOF)
   {
      if(strcmp(opcode,"END")==0)
      {
         fprintf(f3,"%X\t%s\t%s\t%s\n",locctr,label,opcode,operand);
         break;
      }
      if(strcmp(opcode,"START")==0)
      {
         start=locctr=(int)strtol(operand,NULL,16);
         fprintf(f3,"%s\t%s\t%s\n",label,opcode,operand);
         continue;
      }
      if(strcmp(label,"**")!=0)
      {
         fprintf(f4,"%X\t%s\n",locctr,label);
      }
      x=0;
      rewind(f2);
      while(fscanf(f2,"%s%s",opcode1,operand1)!=EOF)
      {
         if(strcmp(opcode,opcode1)==0)
         {
            x=1;
            break;
         }
      }
      fprintf(f3,"%X\t%s\t%s\t%s\n",locctr,label,opcode,operand);
      if(x==1) locctr+=3;
      if(strcmp(opcode,"WORD")==0) locctr+=3;
      if(strcmp(opcode,"RESW")==0) locctr+=3*atoi(operand);
      if(strcmp(opcode,"RESB")==0) locctr+=atoi(operand);
      if(strcmp(opcode,"BYTE")==0)
      {
         if(operand[0]=='C') locctr+=strlen(operand)-3;
         else if(operand[0]=='X') locctr+=(strlen(operand)-3)/2;
      }
   }
   fprintf(f5,"%X",locctr-start);
   fclose(f1);
   fclose(f2);
   fclose(f3);
   fclose(f4);
   fclose(f5);
   return 0;
}
