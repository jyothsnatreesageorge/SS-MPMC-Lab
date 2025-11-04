#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
   FILE *f1,*f2,*f3,*f4,*f5,*f6;
   f1=fopen("inputs.txt","r");
   f2=fopen("optabs.txt","r");
   f3=fopen("inters.txt","w");
   f4=fopen("symtabs.txt","w");
   f5=fopen("lengths.txt","w");
   if(!f1||!f2||!f3||!f4||!f5)
   {
      printf("Error in opening files!\n");
      return -1;
   }
   char label[10],opcode[10],operand[10],opcode1[10],operand1[10];
   int x,locctr,start;
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
         if(strcmp(opcode1,opcode)==0)
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

   f1=fopen("inters.txt","r");
   f2=fopen("lengths.txt","r");
   f3=fopen("optabs.txt","r");
   f4=fopen("symtabs.txt","r");
   f5=fopen("lists.txt","w");
   f6=fopen("objs.txt","w");
   if(!f1||!f2||!f3||!f4||!f5||!f6)
   {
      printf("Error in opening the files!\n");
      return -1;
   }
   char address[10],add[10],sym[10],t_start[10],objcode[1024]="",l1[10],s1[10];
   int tlen,found;
   fscanf(f1,"%s%s%s",label,opcode,operand);
   if(strcmp(opcode,"START")==0)
   {
      strcpy(s1,operand);
      fscanf(f2,"%s",l1);
      fprintf(f6,"H^%-6s^%s^%s\n",label,s1,l1);
      fprintf(f5,"%s\t%s\t%s\n",label,opcode,operand);
      fscanf(f1,"%s%s%s%s",address,label,opcode,operand);
   }
   tlen=0;
   strcpy(t_start,address);
   strcpy(objcode,"");
   while(strcmp(opcode,"END")!=0)
   {
      found=0;
      char temp[100]="";
      rewind(f3);
      while(fscanf(f3,"%s%s",opcode1,operand1)!=EOF)
      {
         if(strcmp(opcode1,opcode)==0)
         {
            rewind(f4);
            while(fscanf(f4,"%s%s",add,sym)!=EOF)
            {
               if(strcmp(operand,sym)==0)
               {
                  sprintf(temp,"%s%s",operand1,add);
                  found=1;
                  fprintf(f5,"%s\t%s\t%s\t%s\t%s\n",address,label,opcode,operand,temp);
                  break;
               }
            }
            break;
         }
      }
      if(!found)
      {
         if(strcmp(opcode,"WORD")==0)
         {
            sprintf(temp,"%06X",atoi(operand));
            fprintf(f5,"%s\t%s\t%s\t%s\t%s\n",address,label,opcode,operand,temp);
         }
         if(strcmp(opcode,"BYTE")==0)
         {
            if(operand[0]=='C')
            {
               char hex[5];
               for(int i=2;i<strlen(operand)-1;i++)
               {
                  sprintf(hex,"%02X",operand[i]);
                  strcat(temp,hex);
               }
            }
            else if(operand[0]=='X')
            {
               strncpy(temp,operand+2,strlen(operand)-3);
               temp[strlen(operand)-3]='\0';
            }
            fprintf(f5,"%s\t%s\t%s\t%s\t%s\n",address,label,opcode,operand,temp);
         }
         if((strcmp(opcode,"RESW")==0)||(strcmp(opcode,"RESB")==0))
         {
            if(tlen>0)
            {
               fprintf(f6,"T^%s^%02X^%s\n",t_start,tlen,objcode);
               tlen=0;
               strcpy(objcode,"");
            }
            fprintf(f5,"%s\t%s\t%s\t%s\t%s\n",address,label,opcode,operand,objcode);
            fscanf(f1,"%s%s%s%s",address,label,opcode,operand);
            strcpy(t_start,address);
            continue;
         }
      }
      if(strlen(temp)>0)
      {
         if(tlen+strlen(temp)/2>30)
         {
            fprintf(f6,"T^%s^%02X^%s\n",t_start,tlen,objcode);
            tlen=0;
            strcpy(objcode,"");
            strcpy(t_start,address);
         }
         strcat(objcode,temp);
         strcat(objcode,"^");
         tlen+=strlen(temp)/2;
      }
      fscanf(f1,"%s%s%s%s",address,label,opcode,operand);
   }
   if(tlen>0)
   {
      fprintf(f6,"T^%s^%02X6%s\n",t_start,tlen,objcode);
   }
   fprintf(f5,"%s\t%s\t%s\t%s\t%s\n",address,label,opcode,operand,objcode);
   fprintf(f6,"E^%s",s1);
   fclose(f1);
   fclose(f2);
   fclose(f3);
   fclose(f4);
   fclose(f5);
   fclose(f6);
   return 0;
}

