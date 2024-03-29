#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>

typedef struct Symbol
{
    char Sym [MAX_PATH];
    struct Symbol * Next;



}Symbol;

typedef struct Tag
{
    char T[MAX_PATH];
}Tag;

int SortingFiles(char *Path,char * Temp);
int BrowsingDirectories (int found );
void ParsingProjectFiles (char *BasePath,char * Temp);
int DeleteTemp(char *Temp);
void GeneratingProjectFile(Symbol *Symbols_List,char *Temp,char *Output_File);
Symbol* DefiningSymbols(Symbol *Symbols_List);
void InitSetupFile(char *Temp);
void Init_MainLoopFile(char * Temp);
void AddEndBracketToFile(char *Temp);
void Interface();
void clrscr();
void ParsingProjectFilesCustomTags (char *BasePath,char * Temp,int NmbTagsInFamily[],Tag *PPCT , Tag *LIBCT ,Tag *PINCT,Tag *GVDCT,Tag *OBINITCT,Tag *TMVCT,Tag *SETCT,Tag *MLCT,Tag *FDCT);
int SortingFilesCustomTags(char *Path,char * Temp,int NmbTagsInFamily[],Tag *PPCT , Tag *LIBCT ,Tag *PINCT,Tag *GVDCT,Tag *OBINITCT,Tag *TMVCT,Tag *SETCT,Tag *MLCT,Tag *FDCT);
void RemoveExistingFile(char * FilePath);

int menu (int *CustomTags);
int MenuDepth1(int *CustomTags);
int MenuDepth2 (int *CustomTags);

Tag * RetrieveTags(int *NmbTagsInFamily,char * Path , Tag * TagArray );
void CheckEmptyDir(int * CustomTags);

int BrowsingDirectories (int found )
{
    struct dirent *de;
    DIR *dr= opendir(".");
    while ((de = readdir(dr)) != NULL && !found)
            {
            if (strcmp(de->d_name,"ProjectFiles")==0)
            {   found=1;
                closedir(dr);
                dr=opendir(de->d_name);
                if ((de=readdir(dr)) == NULL)
                { printf("ERROR DIRECTORY IS EMPTY\n");}
                }
}closedir(dr);
return found ;
}


void ParsingProjectFiles (char * BasePath,char * Temp)
{
    struct dirent *de;
    char Path[PATH_MAX];


    DIR * dr=opendir(BasePath);

    if (!dr)
        return;


    while ((de=readdir(dr))!=NULL)
    {
        if ((strcmp(de->d_name,"."))!=0 && (strcmp(de->d_name,".."))!=0)
        {

            strcpy(Path,BasePath);
            strcat(Path,"/");
            strcat(Path,de->d_name);
            SortingFiles(Path,Temp);
            ParsingProjectFiles(Path,Temp);
        }
    }
    closedir(dr);
}



int main(void)
{
    int found=0;
    int  CustomTags =1;
    char cwd[PATH_MAX];
    char Temp[PATH_MAX];
    char BasePath[PATH_MAX];
    char Output_File[PATH_MAX];
    char Path[PATH_MAX];
    Symbol* Symbols_List;
    Symbols_List=(Symbol*)malloc(sizeof(Symbol));
    Symbols_List=DefiningSymbols(Symbols_List);

    /******************************/
    int NmbTagsInFamily[9]={1,1,1,1,1,1,1,1,1};
    Tag *PPCT;
    PPCT=malloc(sizeof(Tag));
    strcpy(PPCT[0].T,"//pp");

    Tag *LIBCT;
    LIBCT=malloc(sizeof(Tag));
    strcpy(LIBCT[0].T,"//lib");

    Tag *PINCT;
    PINCT=malloc(sizeof(Tag));
    strcpy(PINCT[0].T,"//pin");

    Tag *GVDCT;
    GVDCT=malloc(sizeof(Tag));
    strcpy(GVDCT[0].T,"//gvd");

    Tag *OBINITCT;
    OBINITCT=malloc(sizeof(Tag));
    strcpy(OBINITCT[0].T,"//obinit");

    Tag *TMVCT;
    TMVCT=malloc(sizeof(Tag));
    strcpy(TMVCT[0].T,"//tmv");

    Tag *SETCT;
    SETCT=malloc(sizeof(Tag));
    strcpy(SETCT[0].T,"//set");

    Tag *MLCT;
    MLCT=malloc(sizeof(Tag));
    strcpy(MLCT[0].T,"//ml");

    Tag *FDCT;
    FDCT=malloc(sizeof(Tag));
    strcpy(FDCT[0].T,"//fd");
    /******************************/


    Interface();
    switch(menu (&CustomTags))
{
 case 2  :


        {     clrscr() ;
                Interface();
                DIR *dr= opendir(".");

            if (dr == NULL)
            {
                printf("Could not open current directory" );
                closedir(dr);
                return 0;
            }

                    found=BrowsingDirectories(found);
                    if (!found)
                    {mkdir("ProjectFiles");}


                if (getcwd(cwd, sizeof(cwd)) == NULL)
                { perror("getcwd() error");
               return 1;}



           strcpy(Temp,cwd);
           strcat(Temp,"/Temp");
           DeleteTemp(Temp);
           mkdir("Temp");

            strcpy(BasePath,cwd);
            strcat(BasePath,"/ProjectFiles");
            //strcat(cwd,"/CustomTags.txt");

                /********Default Tags********/

                if (CustomTags)
                {CheckEmptyDir(&CustomTags);}


                if (!CustomTags)
                {   printf("Using Default Tags\n");


                        memset(cwd,'\0',sizeof(char)*strlen(cwd));
                        if (getcwd(cwd, sizeof(cwd)) == NULL)
                { perror("getcwd() error");
                        return 1;}

                    InitSetupFile(Temp);
                    Init_MainLoopFile(Temp);

                    ParsingProjectFiles (BasePath,Temp);
                    AddEndBracketToFile(Temp);


                    printf("Enter output file  name \n");
                    fflush(stdin);
                    gets(Output_File);

                    strcat(cwd,"/Output/");
                    strcat(cwd,Output_File);
                    mkdir("Output");
                    strcat(cwd,"/");
                    mkdir(cwd);
                    strcat(Output_File,".INO");
                    strcat(cwd,Output_File);

                    GeneratingProjectFile(Symbols_List,Temp,cwd);
                    DeleteTemp(Temp);}
                    /********Default Tags********/

                else {



              /***Loading Custom Tags to relative Family***/
             if (getcwd(Path, sizeof(Path)) == NULL)
                { perror("getcwd() error");
               return 1;}
            strcat(Path,"/CustomTags/pp.txt");
            PPCT=RetrieveTags(&NmbTagsInFamily[0],Path ,PPCT);

            /***************************************************/

            printf("\n");
            memset(Path,'\0',sizeof(char)*MAX_PATH);
            if (getcwd(Path, sizeof(Path)) == NULL)
                { perror("getcwd() error");
               return 1;}
            strcat(Path,"/CustomTags/lib.txt");
            LIBCT=RetrieveTags(&NmbTagsInFamily[1],Path ,LIBCT);

           /***************************************************/
            printf("\n");
             memset(Path,'\0',sizeof(char)*MAX_PATH);
            if (getcwd(Path, sizeof(Path)) == NULL)
                { perror("getcwd() error");
               return 1;}
            strcat(Path,"/CustomTags/pin.txt");
            PINCT=RetrieveTags(&NmbTagsInFamily[2],Path ,PINCT);

           /***************************************************/
            printf("\n");
             memset(Path,'\0',sizeof(char)*MAX_PATH);
            if (getcwd(Path, sizeof(Path)) == NULL)
                { perror("getcwd() error");
               return 1;}
            strcat(Path,"/CustomTags/gvd.txt");
            GVDCT=RetrieveTags(&NmbTagsInFamily[3],Path ,GVDCT);

            /***************************************************/
            printf("\n");
             memset(Path,'\0',sizeof(char)*MAX_PATH);
            if (getcwd(Path, sizeof(Path)) == NULL)
                { perror("getcwd() error");
               return 1;}
            strcat(Path,"/CustomTags/obinit.txt");
            OBINITCT=RetrieveTags(&NmbTagsInFamily[4],Path ,OBINITCT);

            /***************************************************/
            printf("\n");
             memset(Path,'\0',sizeof(char)*MAX_PATH);
            if (getcwd(Path, sizeof(Path)) == NULL)
                { perror("getcwd() error");
               return 1;}
            strcat(Path,"/CustomTags/tmv.txt");
            TMVCT=RetrieveTags(&NmbTagsInFamily[5],Path ,TMVCT);

            /***************************************************/
            printf("\n");
            memset(Path,'\0',sizeof(char)*MAX_PATH);
             if (getcwd(Path, sizeof(Path)) == NULL)
                { perror("getcwd() error");
               return 1;}
            strcat(Path,"/CustomTags/set.txt");
            SETCT=RetrieveTags(&NmbTagsInFamily[6],Path ,SETCT);

             /**********************************************/
                printf("\n");
              memset(Path,'\0',sizeof(char)*MAX_PATH);
             if (getcwd(Path, sizeof(Path)) == NULL)
                { perror("getcwd() error");
               return 1;}
            strcat(Path,"/CustomTags/ml.txt");
            MLCT=RetrieveTags(&NmbTagsInFamily[7],Path ,MLCT);

             /**********************************************/
              printf("\n");
              memset(Path,'\0',sizeof(char)*MAX_PATH);
             if (getcwd(Path, sizeof(Path)) == NULL)
                { perror("getcwd() error");
               return 1;}
            strcat(Path,"/CustomTags/fd.txt");
            FDCT=RetrieveTags(&NmbTagsInFamily[8],Path ,FDCT);

             /**********************************************/

                        clrscr();
                        Interface();
                        printf("Using Custom Tags\n");
                        memset(cwd,'\0',sizeof(char)*strlen(cwd));
                        if (getcwd(cwd, sizeof(cwd)) == NULL)
                        { perror("getcwd() error"); printf("HTRH\n");
                        return 1;}


                        InitSetupFile(Temp);
                        Init_MainLoopFile(Temp);

                        ParsingProjectFilesCustomTags(BasePath,Temp,NmbTagsInFamily,PPCT ,LIBCT,PINCT,GVDCT,OBINITCT,TMVCT,SETCT,MLCT,FDCT);
                        AddEndBracketToFile(Temp);
                        printf("\nEnter output file  name \n");
                        fflush(stdin);
                        gets(Output_File);

                        strcat(cwd,"/Output/");
                        strcat(cwd,Output_File);
                        mkdir("Output");
                        strcat(cwd,"/");
                        mkdir(cwd);
                        strcat(Output_File,".INO");
                        strcat(cwd,Output_File);

                        RemoveExistingFile(cwd);

                        GeneratingProjectFile(Symbols_List,Temp,cwd);
                        DeleteTemp(Temp);







                        }

 }



                    break;

    case 0 :
                {clrscr();
                Interface();
                return 0;}break;


}

    return 0;
}




int SortingFiles(char *Path,char *Temp )
{
  char Temp_Path[PATH_MAX];
  char c;
  int Pos;
  int i =0;
  int cpy =0;
  FILE *pp;
  FILE *f;
  FILE *Set;
  FILE *Pins;
  FILE *GVD;
  FILE *TMV;
  FILE *ObInit;
  FILE *ML;
  FILE *Lib;
  FILE *FD;


    f=fopen(Path,"r");
    if (!f)
       { return 0;}

    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/PersonalProperty.txt");
    pp=fopen(Temp_Path,"a+"); //pp

    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/Setup.txt");
    Set=fopen(Temp_Path,"a+"); //set

    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/Pins.txt");
    Pins=fopen(Temp_Path,"a+"); //pin

    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/GlobalVariablesAndDefines.txt");
    GVD=fopen(Temp_Path,"a+"); //gvd

    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/TestingMenuVariables.txt");
    TMV=fopen(Temp_Path,"a+");//tmv

    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/Libraries.txt");
    Lib=fopen(Temp_Path,"a+");//lib

    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/ObjectInitialization.txt");
    ObInit=fopen(Temp_Path,"a+");//obinit

    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/MainLoop.txt");
    ML=fopen(Temp_Path,"a+");//ml

    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/FunctionDevelopment.txt");
    FD=fopen(Temp_Path,"a+");//fd


  char *Buf ;
  Buf=malloc(sizeof(char)*6);


  while (!feof(f))
  {
                    c=fgetc(f);
                    if (c=='/')
                           {

                                //***********************************************************************************//

                                 memset(Buf,'\0',sizeof(char)*(strlen("//pp")+1));
                                                    fseek(f,0,SEEK_CUR);
                                                    Pos=ftell(f);

                                        for (i=0;i<strlen("//pp") ;i++)
                                                {   Buf[i]=c;
                                                  c=fgetc(f);
                                                    }
                                                    Buf[strlen("//pp")]='\0';

                                                    if (strcmp(Buf,"//pp")==0)
                                                    {   cpy=1;
                                                        fseek(f,0,SEEK_CUR);

                                                        while (cpy)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);
                                                                for (i=0;i<strlen("//pp") ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}


                                                                if (strcmp(Buf,"//pp")==0)
                                                                    {   cpy=0;
                                                                    fseek(f,0,SEEK_CUR);
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),pp);c=fgetc(f);}}
                                                        else {fputc(c,pp);c=fgetc(f);}

                                                        }
                                                       }
                                               else {fseek(f,-(strlen("//pp")+1),SEEK_CUR);c=fgetc(f); fseek(f,Pos,SEEK_SET);}


                                       //***********************************************************************************//
                                        memset(Buf,'\0',sizeof(char)*6);
                                        fseek(f,0,SEEK_CUR);
                                        Pos=ftell(f);
                                         for (i=0;i<strlen("//set") ;i++)
                                                {   Buf[i]=c;
                                                    c=fgetc(f);}Buf[5]='\0';

                                            if (strcmp(Buf,"//set")==0)
                                                    {   cpy=1;
                                                         fseek(f,0,SEEK_CUR);

                                                        while (cpy)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);

                                                                        for (i=0;i<strlen("//set") && c!=EOF ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}

                                                                if (strcmp(Buf,"//set")==0)
                                                                    {   cpy=0;
                                                                    fseek(f,0,SEEK_CUR);
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),Set);c=fgetc(f);}}
                                                        else {fputc(c,Set);c=fgetc(f);}

                                                        }
                                                        }
                                                  else {fseek(f,-(strlen("//set")+1),SEEK_CUR);c=fgetc(f); fseek(f,Pos,SEEK_SET);}

                              //******************************************************************************************//
                                                    memset(Buf,'\0',sizeof(char)*6);
                                                    fseek(f,0,SEEK_CUR);
                                                    Pos=ftell(f);

                                                 for (i=0;i<strlen("//pin")  ;i++)
                                                {   Buf[i]=c;
                                                   c=fgetc(f);
                                                    }
                                                    Buf[5]='\0';



                                            if (strcmp(Buf,"//pin")==0)
                                                    {   cpy=1;
                                                        fseek(f,0,SEEK_CUR);

                                                        while (cpy)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);
                                                                for (i=0;i<strlen("//pin") ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}


                                                                if (strcmp(Buf,"//pin")==0)
                                                                    {   cpy=0;
                                                                    fseek(f,0,SEEK_CUR);
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),Pins);c=fgetc(f);}}
                                                        else {fputc(c,Pins);c=fgetc(f);}

                                                        }
                                                       }
                                                else {fseek(f,-(strlen("//pin")+1),SEEK_CUR);c=fgetc(f); fseek(f,Pos,SEEK_SET);}

                                //***********************************************************************************//

                                                memset(Buf,'\0',sizeof(char)*6);
                                                    fseek(f,0,SEEK_CUR);
                                                    Pos=ftell(f);

                                        for (i=0;i<strlen("//gvd")  ;i++)
                                                {   Buf[i]=c;
                                                  c=fgetc(f);
                                                    }
                                                    Buf[5]='\0';



                                            if (strcmp(Buf,"//gvd")==0)
                                                    {   cpy=1;
                                                        fseek(f,0,SEEK_CUR);

                                                        while (cpy)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);
                                                                for (i=0;i<strlen("//gvd") ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}


                                                                if (strcmp(Buf,"//gvd")==0)
                                                                    {   cpy=0;
                                                                    fseek(f,0,SEEK_CUR);
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),GVD);c=fgetc(f);}}
                                                        else {fputc(c,GVD);c=fgetc(f);}

                                                        }
                                                       }
                                                else {fseek(f,-(strlen("//gvd")+1),SEEK_CUR);c=fgetc(f); fseek(f,Pos,SEEK_SET);}

                                         //******************************************************************************************//
                                                    memset(Buf,'\0',sizeof(char)*6);
                                                    fseek(f,0,SEEK_CUR);
                                                    Pos=ftell(f);

                                                 for (i=0;i<strlen("//tmv")  ;i++)
                                                {   Buf[i]=c;
                                                   c=fgetc(f);
                                                    }
                                                    Buf[5]='\0';



                                            if (strcmp(Buf,"//tmv")==0)
                                                    {   cpy=1;
                                                        fseek(f,0,SEEK_CUR);

                                                        while (cpy)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);
                                                                for (i=0;i<strlen("//tmv") ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}


                                                                if (strcmp(Buf,"//tmv")==0)
                                                                    {   cpy=0;
                                                                    fseek(f,0,SEEK_CUR);
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),TMV);c=fgetc(f);}}
                                                        else {fputc(c,TMV);c=fgetc(f);}

                                                        }
                                                       }
                                                else {fseek(f,-(strlen("//tmv")+1),SEEK_CUR);c=fgetc(f); fseek(f,Pos,SEEK_SET);}

                                //***********************************************************************************//

                                                memset(Buf,'\0',sizeof(char)*6);
                                                    fseek(f,0,SEEK_CUR);
                                                    Pos=ftell(f);

                                        for (i=0;i<strlen("//lib")  ;i++)
                                                {   Buf[i]=c;
                                                  c=fgetc(f);
                                                    }
                                                    Buf[5]='\0';



                                            if (strcmp(Buf,"//lib")==0)
                                                    {   cpy=1;
                                                        fseek(f,0,SEEK_CUR);

                                                        while (cpy)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);
                                                                for (i=0;i<strlen("//lib") ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}


                                                                if (strcmp(Buf,"//lib")==0)
                                                                    {   cpy=0;
                                                                    fseek(f,0,SEEK_CUR);
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),Lib);c=fgetc(f);}}
                                                        else {fputc(c,Lib);c=fgetc(f);}

                                                        }
                                                       }
                                                else {fseek(f,-(strlen("//lib")+1),SEEK_CUR);c=fgetc(f); fseek(f,Pos,SEEK_SET);}


                                                //***********************************************************************************//

                                                memset(Buf,'\0',sizeof(char)*(strlen("//obinit")+1));
                                                    fseek(f,0,SEEK_CUR);
                                                    Pos=ftell(f);

                                        for (i=0;i<strlen("//obinit")  ;i++)
                                                {   Buf[i]=c;
                                                  c=fgetc(f);
                                                    }
                                                    Buf[strlen("//obinit")]='\0';



                                            if (strcmp(Buf,"//obinit")==0)
                                                    {   cpy=1;
                                                        fseek(f,0,SEEK_CUR);

                                                        while (cpy)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);
                                                                for (i=0;i<strlen("//obinit") ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}


                                                                if (strcmp(Buf,"//obinit")==0)
                                                                    {   cpy=0;
                                                                    fseek(f,0,SEEK_CUR);
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),ObInit);c=fgetc(f);}}
                                                        else {fputc(c,ObInit);c=fgetc(f);}

                                                        }
                                                       }
                                                else {fseek(f,-(strlen("//obinit")+1),SEEK_CUR);c=fgetc(f); fseek(f,Pos,SEEK_SET);}


                                                 //***********************************************************************************//

                                                memset(Buf,'\0',sizeof(char)*(strlen("//ml")+1));
                                                    fseek(f,0,SEEK_CUR);
                                                    Pos=ftell(f);

                                        for (i=0;i<strlen("//ml")  ;i++)
                                                {   Buf[i]=c;
                                                  c=fgetc(f);
                                                    }
                                                    Buf[strlen("//ml")]='\0';

                                                    if (strcmp(Buf,"//ml")==0)
                                                    {   cpy=1;
                                                        fseek(f,0,SEEK_CUR);

                                                        while (cpy)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);
                                                                for (i=0;i<strlen("//ml") ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}


                                                                if (strcmp(Buf,"//ml")==0)
                                                                    {   cpy=0;
                                                                    fseek(f,0,SEEK_CUR);
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),ML);c=fgetc(f);}}
                                                        else {fputc(c,ML);c=fgetc(f);}

                                                        }
                                                       }
                                                else {fseek(f,-(strlen("//ml")+1),SEEK_CUR);c=fgetc(f); fseek(f,Pos,SEEK_SET);}


                                                 //***********************************************************************************//


                                                   memset(Buf,'\0',sizeof(char)*(strlen("//fd")+1));
                                                    fseek(f,0,SEEK_CUR);
                                                    Pos=ftell(f);

                                        for (i=0;i<strlen("//fd")  ;i++)
                                                {   Buf[i]=c;
                                                  c=fgetc(f);
                                                    }
                                                    Buf[strlen("//fd")]='\0';

                                                    if (strcmp(Buf,"//fd")==0)
                                                    {   cpy=1;
                                                        fseek(f,0,SEEK_CUR);

                                                        while (cpy)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);
                                                                for (i=0;i<strlen("//fd") ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}


                                                                if (strcmp(Buf,"//fd")==0)
                                                                    {   cpy=0;
                                                                    fseek(f,0,SEEK_CUR);
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),FD);c=fgetc(f);}}
                                                        else {fputc(c,FD);c=fgetc(f);}

                                                        }
                                                       }
                                                else {fseek(f,Pos,SEEK_SET);}


                                                 //***********************************************************************************//



                                                        }
  }




    free(Buf);
    fclose(f);
    fclose(Set);
    fclose(Pins);
    fclose(GVD);
    fclose(Lib);
    fclose(TMV);
    fclose(ObInit);
    fclose(ML);
    fclose(pp);
    fclose(FD);


    return 0;
}

int DeleteTemp(char *Temp)
{   struct dirent *de;
    DIR *dr =opendir(Temp);
    char File_Path[MAX_PATH];
    strcat(Temp,"/");


        while ((de=readdir(dr))!=NULL)
        {


            if ((strcmp(de->d_name,".")!=0) && (strcmp(de->d_name,"..")!=0))
            {
                strcpy(File_Path,Temp);
                strcat(File_Path,de->d_name);
                remove(File_Path);


            }
        }
closedir(dr);
return 0;
}

void GeneratingProjectFile(Symbol *Symbols_List,char *Temp,char *Output_File)
{

    struct dirent *de;
    DIR *dr=opendir(Temp);
    char File_Path[MAX_PATH];
    FILE *f;
    FILE *f1;
    char Buf[MAX_PATH];

    if (Symbols_List!=NULL)
    {strcpy(File_Path,Temp);
    strcat(File_Path,"/");
    strcat(File_Path,Symbols_List->Sym);

    while ((de=readdir(dr))!=NULL )
    {
        if (strcmp(Symbols_List->Sym,de->d_name)==0)
        {   de=NULL;
            f=fopen(Output_File,"a");
            fputs("\n/**/\n",f);
            f1=fopen(File_Path,"r");
            {
                if (!f1)
                {
                    perror("ERROR GENERATING FILE\n");
                }
            }

                while (fgets(Buf,259,f1)!=NULL)
                    {

                        fputs(Buf,f);


                    }

            fclose(f);
            fclose(f1);

        }


    }
        GeneratingProjectFile(Symbols_List->Next,Temp,Output_File);
 }


}





Symbol* DefiningSymbols(Symbol *Symbols_List)
{
     Symbol *S;
     Symbol *S1;
    S=malloc(sizeof(Symbol));
    Symbols_List=S;
    strcpy(S->Sym,"PersonalProperty.txt");
    S1=malloc(sizeof(Symbol));
    S->Next=S1;
    strcpy(S1->Sym,"Libraries.txt");
    S=malloc(sizeof(Symbol));
    S1->Next=S;
    strcpy(S->Sym,"Pins.txt");
    S1=malloc(sizeof(Symbol));
    S->Next=S1;
    strcpy(S1->Sym,"GlobalVariablesAndDefines.txt");
    S=malloc(sizeof(Symbol));
    S1->Next=S;
    strcpy(S->Sym,"ObjectInitialization.txt");
    S1=malloc(sizeof(Symbol));
    S->Next=S1;
    strcpy(S1->Sym,"TestingMenuVariables.txt");
    S=malloc(sizeof(Symbol));
    S1->Next=S;
    strcpy(S->Sym,"Setup.txt");
    S1=malloc(sizeof(Symbol));
    S->Next=S1;
    strcpy(S1->Sym,"MainLoop.txt");
    S=malloc(sizeof(Symbol));
    S1->Next=S;
    strcpy(S->Sym,"FunctionDevelopment.txt");
    S->Next=NULL;








return Symbols_List;

}

void Init_MainLoopFile(char * Temp)
{   FILE *f;
    char File_Path [MAX_PATH];
    strcpy(File_Path,Temp);
    strcat(File_Path,"MainLoop.txt");
    f=fopen(File_Path,"a");
    fputs("void loop ()\n{",f);
    fclose(f);
}

void InitSetupFile(char *Temp)
{   FILE *f;
    char File_Path [MAX_PATH];
    strcpy(File_Path,Temp);
    strcat(File_Path,"Setup.txt");
    f=fopen(File_Path,"a");
    fputs("void setup()\n{",f);
    fclose(f);
}
void AddEndBracketToFile(char *Temp)
{

    FILE *f ;
    char  File_Path [MAX_PATH];
    strcpy(File_Path,Temp);
    strcat(File_Path,"Setup.txt");
    f=fopen(File_Path,"a");
    fseek(f,0,SEEK_END);
    fputs("\n}",f);
    fclose(f);
    memset(File_Path,'\0',MAX_PATH);
    strcpy(File_Path,Temp);
    strcat(File_Path,"/MainLoop.txt");
    f=fopen(File_Path,"a");
    fseek(f,0,SEEK_END);
    fputs("\n}",f);
    fclose(f);
}



void Interface()
{
printf("*************************************************************************\n");
printf("*                                                                       *\n");
printf("*     Thank You For Using Simple Arduino Integrator                     *\n");
printf("*                                                                       *\n");
printf("*                                                           by Sumpark  *\n");
printf("*************************************************************************\n");
printf("\n\n");



}


int menu (int * CustomTags )
{
    int choice =0;
    char  cwd[MAX_PATH];


      if (getcwd(cwd, sizeof(cwd)) == NULL)
                { perror("getcwd() error");
               return 1;}

            strcat(cwd,"/CustomTags.txt");
            clrscr();
            Interface();
    do
    {   clrscr();
        Interface();
        printf("1: Tags Settings\n");
        printf("2: Start integrating\n");
        printf("0: Exit\n");
        scanf("%d",&choice);

    }while (choice<0 || choice >2);

    if (choice ==1 )
    {choice= MenuDepth1(CustomTags); }

   else  {return choice;}
return choice;
}

int MenuDepth1(int * CustomTags)
{
    int choice;

        clrscr();
        Interface();
        printf("1: Define New Tags \n");
        printf("2: Load Default Tags\n");
        printf("0: Return To Main Menu\n");
        scanf("%d",&choice);


        if (choice==1)
        {
            choice=MenuDepth2(CustomTags);
        }
        else if (choice==2)
        {
            *CustomTags=0;
            choice=menu(CustomTags);
        }
        else
            {  choice=menu(CustomTags);}

    printf("\n%d\n",choice);
    return choice;
}


int MenuDepth2 ( int *CustomTags)
{   int choice ;
    char Buf [MAX_PATH] ;
    char FilePatch [MAX_PATH];


    if (getcwd(FilePatch, sizeof(FilePatch)) == NULL)
                { perror("getcwd() error");
               return 1;}


    strcat(FilePatch,"/CustomTags");
    FILE  *f;
    mkdir(FilePatch);
              do {
                clrscr();
                Interface();
                printf("1: Add new tags to //pp\n");
                printf("2: Add new tags to //lib\n");
                printf("3: Add new tags to //pin\n");
                printf("4: Add new tags to //gvd\n");
                printf("5: Add new tags to //obinit\n");
                printf("6: Add new tags to //tmv\n");
                printf("7: Add new tags to //set\n");
                printf("8: Add new tags to //ml\n");
                printf("9; Add new tags to //fd\n");
                printf("0 :Return \n");
                scanf("%d",&choice);}while( choice <0 || choice >9);


                if (choice==1)
                {
                    clrscr();
                    Interface();
                    printf("0: return \n");
                    strcat(FilePatch,"/pp.txt");
                    f=fopen(FilePatch,"a");
                    do
                    {
                        printf("//");
                        fflush(stdin);
                        gets(Buf);
                        *CustomTags=1;
                        if (strlen(Buf)!=1 || Buf [0]!='0')
                        {   fprintf(f,"//%s\n",Buf);
                        }}
                        while (strlen(Buf)!=1 || Buf[0]!='0' );
                       fclose(f);
                       return MenuDepth2(CustomTags);
                }

                 if (choice==2)
                {
                    clrscr();
                    Interface();
                    printf("0: return \n");
                    strcat(FilePatch,"/lib.txt");
                    f=fopen(FilePatch,"a");
                    do
                    {
                        printf("//");
                        fflush(stdin);
                        gets(Buf);
                        *CustomTags=1;
                        if (strlen(Buf)!=1 || Buf [0]!='0')
                        {   fprintf(f,"//%s\n",Buf);
                        }}
                        while (strlen(Buf)!=1 || Buf[0]!='0' );
                       fclose(f);
                       return MenuDepth2(CustomTags);
                }
                 if (choice==3)
                {
                    clrscr();
                    Interface();
                    printf("0: return \n");
                    strcat(FilePatch,"/pin.txt");
                    f=fopen(FilePatch,"a");
                    do
                    {
                        printf("//");
                        fflush(stdin);
                        gets(Buf);
                        *CustomTags=1;
                        if (strlen(Buf)!=1 || Buf [0]!='0')
                        {   fprintf(f,"//%s\n",Buf);
                        }}
                        while (strlen(Buf)!=1 || Buf[0]!='0' );
                       fclose(f);
                       return MenuDepth2(CustomTags);
                }
                 if (choice==4)
                {
                    clrscr();
                    Interface();
                    printf("0: return \n");
                    strcat(FilePatch,"//gvd.txt");
                    f=fopen(FilePatch,"a");
                    do
                    {
                        printf("//");
                        fflush(stdin);
                        gets(Buf);
                        *CustomTags=1;
                        if (strlen(Buf)!=1 || Buf [0]!='0')
                        {   fprintf(f,"//%s\n",Buf);
                        }}
                        while (strlen(Buf)!=1 || Buf[0]!='0' );
                       fclose(f);
                       MenuDepth2(CustomTags);
                }
                 if (choice==5)
                {
                    clrscr();
                    Interface();
                    printf("0: return \n");
                    strcat(FilePatch,"/obinit.txt");
                    f=fopen(FilePatch,"a");
                    do
                    {
                        printf("//");
                        fflush(stdin);
                        gets(Buf);
                        *CustomTags=1;
                        if (strlen(Buf)!=1 || Buf [0]!='0')
                        {   fprintf(f,"//%s\n",Buf);
                        }}
                        while (strlen(Buf)!=1 || Buf[0]!='0' );
                       fclose(f);
                      return  MenuDepth2(CustomTags);
                }
                 if (choice==6)
                {
                    clrscr();
                    Interface();
                    printf("0: return \n");
                    strcat(FilePatch,"/tmv.txt");
                    f=fopen(FilePatch,"a");
                    do
                    {
                        printf("//");
                        fflush(stdin);
                        gets(Buf);
                        *CustomTags=1;
                        if (strlen(Buf)!=1 || Buf [0]!='0')
                        {   fprintf(f,"//%s\n",Buf);
                        }}
                        while (strlen(Buf)!=1 || Buf[0]!='0' );
                       fclose(f);
                     return   MenuDepth2(CustomTags);
                }
                 if (choice==7)
                {
                    clrscr();
                    Interface();
                    printf("0: return \n");
                    strcat(FilePatch,"/set.txt");
                    f=fopen(FilePatch,"a");
                    do
                    {
                        printf("//");
                        fflush(stdin);
                        gets(Buf);
                        *CustomTags=1;
                        if (strlen(Buf)!=1 || Buf [0]!='0')
                        {   fprintf(f,"//%s\n",Buf);
                        }}
                        while (strlen(Buf)!=1 || Buf[0]!='0' );
                       fclose(f);
                      return  MenuDepth2(CustomTags);
                }
                 if (choice==8)
                {
                    clrscr();
                    Interface();
                    printf("0: return \n");
                    strcat(FilePatch,"/ml.txt");
                    f=fopen(FilePatch,"a");
                    do
                    {
                        printf("//");
                        fflush(stdin);
                        gets(Buf);
                        *CustomTags=1;
                        if (strlen(Buf)!=1 || Buf [0]!='0')
                        {   fprintf(f,"//%s\n",Buf);
                        }}
                        while (strlen(Buf)!=1 || Buf[0]!='0' );
                       fclose(f);
                      return  MenuDepth2(CustomTags);
                }
                 if (choice==9)
                {
                    clrscr();
                    Interface();
                    printf("0: return \n");
                    strcat(FilePatch,"/fd.txt");
                    f=fopen(FilePatch,"a");
                    do
                    {
                        printf("//");
                        fflush(stdin);
                        gets(Buf);
                        *CustomTags=1;
                        if (strlen(Buf)!=1 || Buf [0]!='0')
                        {   fprintf(f,"//%s\n",Buf);
                        }}
                        while (strlen(Buf)!=1 || Buf[0]!='0' );
                       fclose(f);
                      return  MenuDepth2(CustomTags);
                }
               else if (!choice)
                {
                   return  MenuDepth1(CustomTags);
                }
                 return choice ;




}









void clrscr(){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}



void ParsingProjectFilesCustomTags (char *BasePath,char * Temp,int NmbTagsInFamily[],Tag *PPCT , Tag *LIBCT ,Tag *PINCT,Tag *GVDCT,Tag *OBINITCT,Tag *TMVCT,Tag *SETCT,Tag *MLCT,Tag *FDCT)
{

    struct dirent *de;
    char Path[PATH_MAX];


    DIR * dr=opendir(BasePath);

    if (!dr)
         return;


    while ((de=readdir(dr))!=NULL)
    {
        if ((strcmp(de->d_name,"."))!=0 && (strcmp(de->d_name,".."))!=0)
        {

            strcpy(Path,BasePath);
            strcat(Path,"/");
            strcat(Path,de->d_name);
            SortingFilesCustomTags(Path,Temp,NmbTagsInFamily,PPCT,LIBCT,PINCT,GVDCT,OBINITCT,TMVCT,SETCT,MLCT,FDCT);
            ParsingProjectFilesCustomTags(Path,Temp,NmbTagsInFamily,PPCT ,LIBCT,PINCT,GVDCT,OBINITCT,TMVCT,SETCT,MLCT,FDCT);

        }
    }
    closedir(dr);



}
int SortingFilesCustomTags(char *Path,char * Temp,int NmbTagsInFamily[],Tag *PPCT , Tag *LIBCT ,Tag *PINCT,Tag *GVDCT,Tag *OBINITCT,Tag *TMVCT,Tag *SETCT,Tag *MLCT,Tag *FDCT)
{
  char Temp_Path[PATH_MAX];
  char c;
  int Pos;
  int i =0,j=0;
  int cpy =0;
  FILE *f;
  FILE *Set;
  FILE *Pins;
  FILE *GVD;
  FILE *TMV;
  FILE *ObInit;
  FILE *ML;
  FILE *Lib;
  FILE *PP;
  FILE *FD;

    f=fopen(Path,"r");
    if (!f)
       { return 0;}


    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/PersonalProperty.txt");
    PP=fopen(Temp_Path,"a+"); //pp

    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/Setup.txt");
    Set=fopen(Temp_Path,"a+"); //set

    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/Pins.txt");
    Pins=fopen(Temp_Path,"a+"); //pin

    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/GlobalVariablesAndDefines.txt");
    GVD=fopen(Temp_Path,"a+"); //gvd

    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/TestingMenuVariables.txt");
    TMV=fopen(Temp_Path,"a+");//tmv

    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/Libraries.txt");
    Lib=fopen(Temp_Path,"a+");//lib

    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/ObjectInitialization.txt");
    ObInit=fopen(Temp_Path,"a+");//obinit

    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/MainLoop.txt");
    ML=fopen(Temp_Path,"a+");//ml

    strcpy(Temp_Path,Temp);
    strcat(Temp_Path,"/FunctionDevelopment.txt");
    FD=fopen(Temp_Path,"a+");//fd



  char *Buf ;
  Buf=malloc(sizeof(char)*MAX_PATH);


  while (!feof(f))
  {
                    c=fgetc(f);
                    if (c=='/')
                           {

                                //***********************************************************************************//
                                     for (j=0;j<NmbTagsInFamily[0];j++)
                                            {       memset(Buf,'\0',sizeof(char)*(strlen(PPCT[j].T)+1));
                                                    fseek(f,0,SEEK_CUR);
                                                    Pos=ftell(f);


                                        for (i=0;i<strlen(PPCT[j].T) ;i++)
                                                {   Buf[i]=c;
                                                  c=fgetc(f);
                                                    }
                                                    Buf[strlen(PPCT[j].T)]='\0';

                                                    if (strcmp(Buf,PPCT[j].T)==0)
                                                    {   cpy=1;
                                                        fseek(f,0,SEEK_CUR);

                                                        while (cpy&& c!=EOF)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);
                                                                    memset(Buf,'\0',sizeof(char)*(strlen(PPCT[j].T)+1));
                                                                for (i=0;i<strlen(PPCT[j].T) ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}


                                                                if (strcmp(Buf,PPCT[j].T)==0)
                                                                    {   cpy=0;
                                                                    //fseek(f,0,SEEK_CUR);
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),PP);c=fgetc(f);}}
                                                        else {fputc(c,PP);c=fgetc(f);}

                                                        }
                                                       }
                                               else {fseek(f,Pos-1,SEEK_SET);c=fgetc(f);}
                                               }


                                       //***********************************************************************************//
                                        for (j=0;j<NmbTagsInFamily[1];j++)
                                            {       memset(Buf,'\0',sizeof(char)*(strlen(LIBCT[j].T)+1));
                                                    fseek(f,0,SEEK_CUR);
                                                    Pos=ftell(f);


                                        for (i=0;i<strlen(LIBCT[j].T) ;i++)
                                                {   Buf[i]=c;
                                                  c=fgetc(f);
                                                    }
                                                    Buf[strlen(LIBCT[j].T)]='\0';

                                                    if (strcmp(Buf,LIBCT[j].T)==0)
                                                    {   cpy=1;
                                                        fseek(f,0,SEEK_CUR);

                                                        while (cpy&& c!=EOF)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);
                                                                    memset(Buf,'\0',sizeof(char)*(strlen(LIBCT[j].T)+1));
                                                                for (i=0;i<strlen(LIBCT[j].T) ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}


                                                                if (strcmp(Buf,LIBCT[j].T)==0)
                                                                    {   cpy=0;
                                                                    //fseek(f,0,SEEK_CUR);
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),Lib);c=fgetc(f);}}
                                                        else {fputc(c,Lib);c=fgetc(f);}

                                                        }
                                                       }
                                               else {fseek(f,Pos-1,SEEK_SET);c=fgetc(f);}
                                               }
                              //******************************************************************************************//
                                                   for (j=0;j<NmbTagsInFamily[2];j++)
                                            {       memset(Buf,'\0',sizeof(char)*(strlen(PINCT[j].T)+1));
                                                    fseek(f,0,SEEK_CUR);
                                                    Pos=ftell(f);


                                        for (i=0;i<strlen(PINCT[j].T) ;i++)
                                                {   Buf[i]=c;
                                                  c=fgetc(f);
                                                    }
                                                    Buf[strlen(PINCT[j].T)]='\0';

                                                    if (strcmp(Buf,PINCT[j].T)==0)
                                                    {   cpy=1;
                                                        fseek(f,0,SEEK_CUR);

                                                        while (cpy&& c!=EOF)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);
                                                                    memset(Buf,'\0',sizeof(char)*(strlen(PINCT[j].T)+1));
                                                                for (i=0;i<strlen(PINCT[j].T) ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}


                                                                if (strcmp(Buf,PINCT[j].T)==0)
                                                                    {   cpy=0;
                                                                    //fseek(f,0,SEEK_CUR);
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),Pins);c=fgetc(f);}}
                                                        else {fputc(c,Pins);c=fgetc(f);}

                                                        }
                                                       }
                                               else {fseek(f,Pos-1,SEEK_SET);c=fgetc(f);}
                                               }

                                //***********************************************************************************//

                                                for (j=0;j<NmbTagsInFamily[3];j++)
                                            {       memset(Buf,'\0',sizeof(char)*(strlen(GVDCT[j].T)+1));
                                                    fseek(f,0,SEEK_CUR);
                                                    Pos=ftell(f);


                                        for (i=0;i<strlen(GVDCT[j].T) ;i++)
                                                {   Buf[i]=c;
                                                  c=fgetc(f);
                                                    }
                                                    Buf[strlen(GVDCT[j].T)]='\0';

                                                    if (strcmp(Buf,GVDCT[j].T)==0)
                                                    {   cpy=1;
                                                        fseek(f,0,SEEK_CUR);

                                                        while (cpy&& c!=EOF)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);
                                                                    memset(Buf,'\0',sizeof(char)*(strlen(GVDCT[j].T)+1));
                                                                for (i=0;i<strlen(GVDCT[j].T) ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}


                                                                if (strcmp(Buf,GVDCT[j].T)==0)
                                                                    {   cpy=0;
                                                                    //fseek(f,0,SEEK_CUR);
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),GVD);c=fgetc(f);}}
                                                        else {fputc(c,GVD);c=fgetc(f);}

                                                        }
                                                       }
                                               else {fseek(f,Pos-1,SEEK_SET);c=fgetc(f);}
                                               }

                                         //******************************************************************************************//
                                                   for (j=0;j<NmbTagsInFamily[4];j++)
                                            {       memset(Buf,'\0',sizeof(char)*(strlen(OBINITCT[j].T)+1));
                                                    fseek(f,0,SEEK_CUR);
                                                    Pos=ftell(f);


                                        for (i=0;i<strlen(OBINITCT[j].T) ;i++)
                                                {   Buf[i]=c;
                                                  c=fgetc(f);
                                                    }
                                                    Buf[strlen(OBINITCT[j].T)]='\0';

                                                    if (strcmp(Buf,OBINITCT[j].T)==0)
                                                    {   cpy=1;
                                                        fseek(f,0,SEEK_CUR);

                                                        while (cpy && c!=EOF)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);
                                                                    memset(Buf,'\0',sizeof(char)*(strlen(OBINITCT[j].T)+1));
                                                                for (i=0;i<strlen(OBINITCT[j].T) ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}


                                                                if (strcmp(Buf,OBINITCT[j].T)==0)
                                                                    {   cpy=0;
                                                                    //fseek(f,0,SEEK_CUR);
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),ObInit);c=fgetc(f);}}
                                                        else {fputc(c,ObInit);c=fgetc(f);}

                                                        }
                                                       }
                                               else {fseek(f,Pos-1,SEEK_SET);c=fgetc(f);}
                                               }
                                //***********************************************************************************//

                                                for (j=0;j<NmbTagsInFamily[5];j++)
                                            {       memset(Buf,'\0',sizeof(char)*(strlen(TMVCT[j].T)+1));
                                                    fseek(f,0,SEEK_CUR);
                                                    Pos=ftell(f);


                                        for (i=0;i<strlen(TMVCT[j].T) ;i++)
                                                {   Buf[i]=c;
                                                  c=fgetc(f);
                                                    }
                                                    Buf[strlen(TMVCT[j].T)]='\0';

                                                    if (strcmp(Buf,TMVCT[j].T)==0)
                                                    {   cpy=1;
                                                        fseek(f,0,SEEK_CUR);

                                                        while (cpy && c!=EOF)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);
                                                                    memset(Buf,'\0',sizeof(char)*(strlen(TMVCT[j].T)+1));
                                                                for (i=0;i<strlen(TMVCT[j].T) ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}


                                                                if (strcmp(Buf,TMVCT[j].T)==0)
                                                                    {   cpy=0;
                                                                    //fseek(f,0,SEEK_CUR);
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),TMV);c=fgetc(f);}}
                                                        else {fputc(c,TMV);c=fgetc(f);}

                                                        }
                                                       }
                                               else {fseek(f,Pos-1,SEEK_SET);c=fgetc(f);}
                                               }

                                                //***********************************************************************************//

                                                 for (j=0;j<NmbTagsInFamily[6];j++)
                                            {       memset(Buf,'\0',sizeof(char)*(strlen(SETCT[j].T)+1));
                                                    fseek(f,0,SEEK_CUR);
                                                    Pos=ftell(f);


                                        for (i=0;i<strlen(SETCT[j].T) ;i++)
                                                {   Buf[i]=c;
                                                  c=fgetc(f);
                                                    }
                                                    Buf[strlen(SETCT[j].T)]='\0';

                                                    if (strcmp(Buf,SETCT[j].T)==0)
                                                    {   cpy=1;
                                                        fseek(f,0,SEEK_CUR);

                                                        while (cpy && c!=EOF)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);
                                                                    memset(Buf,'\0',sizeof(char)*(strlen(SETCT[j].T)+1));
                                                                for (i=0;i<strlen(SETCT[j].T) ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}


                                                                if (strcmp(Buf,SETCT[j].T)==0)
                                                                    {   cpy=0;
                                                                    //fseek(f,0,SEEK_CUR);
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),Set);c=fgetc(f);}}
                                                        else {fputc(c,Set);c=fgetc(f);}

                                                        }
                                                       }
                                               else {fseek(f,Pos-1,SEEK_SET);c=fgetc(f);}
                                               }



                                                 //***********************************************************************************//

                                              for (j=0;j<NmbTagsInFamily[7];j++)
                                            {       memset(Buf,'\0',sizeof(char)*(strlen(MLCT[j].T)+1));
                                                    fseek(f,0,SEEK_CUR);
                                                    Pos=ftell(f);


                                        for (i=0;i<strlen(MLCT[j].T) ;i++)
                                                {   Buf[i]=c;
                                                  c=fgetc(f);
                                                    }
                                                    Buf[strlen(MLCT[j].T)]='\0';

                                                    if (strcmp(Buf,MLCT[j].T)==0)
                                                    {   cpy=1;
                                                        fseek(f,0,SEEK_CUR);

                                                        while (cpy && c!=EOF)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);
                                                                    memset(Buf,'\0',sizeof(char)*(strlen(MLCT[j].T)+1));
                                                                for (i=0;i<strlen(MLCT[j].T) ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}


                                                                if (strcmp(Buf,MLCT[j].T)==0)
                                                                    {   cpy=0;
                                                                    //fseek(f,0,SEEK_CUR);
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),ML);c=fgetc(f);}}
                                                        else {fputc(c,ML);c=fgetc(f);}

                                                        }
                                                       }
                                               else {fseek(f,Pos-1,SEEK_SET);c=fgetc(f);}
                                               }


                                                 //***********************************************************************************//

                                                 for (j=0;j<NmbTagsInFamily[8];j++)
                                            {       memset(Buf,'\0',sizeof(char)*(strlen(FDCT[j].T)+1));
                                                    fseek(f,0,SEEK_CUR);
                                                    Pos=ftell(f);


                                        for (i=0;i<strlen(FDCT[j].T) ;i++)
                                                {   Buf[i]=c;
                                                  c=fgetc(f);
                                                    }
                                                    Buf[strlen(FDCT[j].T)]='\0';

                                                    if (strcmp(Buf,FDCT[j].T)==0)
                                                    {   cpy=1;
                                                        fseek(f,0,SEEK_CUR);

                                                        while (cpy && c!=EOF)
                                                        {
                                                            if (c=='/')
                                                                    {  fseek(f,0,SEEK_CUR);
                                                                    Pos=ftell(f);
                                                                    memset(Buf,'\0',sizeof(char)*(strlen(FDCT[j].T)+1));
                                                                for (i=0;i<strlen(FDCT[j].T) ;i++)
                                                                    { Buf[i]=c;
                                                                        c=fgetc(f);}


                                                                if (strcmp(Buf,FDCT[j].T)==0)
                                                                    {   cpy=0;
                                                                    }
                                                                else {fseek(f,Pos-1,SEEK_SET);
                                                                    fputc(c=fgetc(f),FD);c=fgetc(f);}}
                                                        else {fputc(c,FD);c=fgetc(f);}

                                                        }
                                                       }
                                               else {fseek(f,Pos-1,SEEK_SET);c=fgetc(f);}
                                               }


                                                 //***********************************************************************************//



                                                        }
  }


    free(Buf);
    fclose(f);
    fclose(Set);
    fclose(Pins);
    fclose(GVD);
    fclose(Lib);
    fclose(TMV);
    fclose(ObInit);
    fclose(ML);
    fclose(PP);
    fclose(FD);

    return 0;

}
void RemoveExistingFile(char * FilePath)
{
    FILE *f;
    f=fopen(FilePath,"r");
        if (f)
            {  fclose(f);
            remove(FilePath) ;}
}


Tag * RetrieveTags(int *NmbTagsInFamily,char * Path , Tag * TagArray )
{   int i =1;
    char Buf [MAX_PATH];
    FILE  *f;

   if ((f=fopen(Path,"r"))!=NULL)
    {while (fscanf(f,"%s\n",Buf)!=EOF)
    {   TagArray=realloc(TagArray,sizeof(Tag)*(i+1));
        strcpy(TagArray[i].T,Buf);
        i++;

    }
    (*NmbTagsInFamily)=i;
    fclose(f);}

    return TagArray;
}

void  CheckEmptyDir(int * CustomTags)
{   char Path[MAX_PATH];
    int n = 0;
    struct dirent *d;
    DIR *dir;

  if (getcwd(Path, sizeof(Path)) == NULL)
        { perror("getcwd() error"); *CustomTags=0;return ;}
            strcat(Path,"/CustomTags");
    dir = opendir(Path);
  if (dir == NULL) //Not a directory or doesn't exist
    {*CustomTags=0;
    printf("Custom Tags Directory Empty or Doesn't Exist\n");}
   else {while ((d = readdir(dir)) != NULL)
    {
    if(++n > 2)
      {*CustomTags=1;break;}
  }
  closedir(dir);
  if (n <= 2) //Directory Empty
   {
        printf("Custom Tags Directory Empty or Doesn't Exist\n");
       *CustomTags=0; }
  else
     *CustomTags=1;


}
}
