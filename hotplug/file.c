#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>   
unsigned long get_file_size(const char *path)  
{  
    unsigned long filesize = -1;      
    struct stat statbuff;  
    if(stat(path, &statbuff) < 0){  
        return filesize;  
    }else{  
        filesize = statbuff.st_size;  
    }  
    return filesize;  
}
void main()
{
   //unsigned long i;
   int i;
   char s[9];
   int j = 0;
   int ret;
   char buf[10000];
   FILE *fp1;
   FILE *fp2;
   fp1 = fopen("gif.dat","ab+");
   while(j < 100)
   {
   if(j < 10)
   sprintf(s,"0%d.jpg",j);
   else sprintf(s,"%d.jpg",j);
   i = get_file_size(s);
   if(i < 0)
   printf("read error\n");
   else printf("filesize is %d\n",i);
   fp2 = fopen(s,"rb");
   fread(buf,i,1,fp2);
   fwrite(&i,1,sizeof(i),fp1);
   ret = fwrite(buf,1,i,fp1);
   printf("ret is %d\n",ret);
   if(ret!=i)
   printf("write error!\n");
   memset(buf,0,10000);
   fclose(fp2);
   j++;
   }
   fclose(fp1);
}
   

