#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int ilosc_plikow;
    int ilosc_folderow;
} Ilosc;

typedef struct {
    char **pliki;
    int ilosc_plikow;
    char **foldery;
    int ilosc_folderow;
} ZawartoscFolderu;

long long rozmiar(const char *nazwa_sciezki){
     
     struct stat st;
     if(stat(nazwa_sciezki, &st)==0){

          long long rozmiar = st.st_size;
          return rozmiar;
     }
     else{
          return -1;
     }
}

Ilosc liczba(const char *nazwa_sciezki) {
    Ilosc licznik = {0,0};
    struct dirent *plik;
    DIR *sciezka = opendir(nazwa_sciezki);

    //if (!sciezka) return NULL;
    while ((plik = readdir(sciezka)) != NULL){
     if (strcmp(plik->d_name, ".") == 0 || strcmp(plik->d_name, "..") == 0) continue;

     if(plik->d_type == DT_DIR){
               licznik.ilosc_folderow++;
          }
     else if(plik->d_type == DT_REG){
               licznik.ilosc_plikow++;
          }


    };
    closedir(sciezka);
    return licznik;
}

ZawartoscFolderu lista_rzeczy( const char *nazwa_sciezki ) {
     Ilosc ilosc = liczba(nazwa_sciezki);

     //if((ilosc.ilosc_plikow+ilosc_folderow) == 0) return NULL;

     struct dirent *plik;
     DIR *sciezka = opendir(nazwa_sciezki);
      
      

     ZawartoscFolderu zwracak;


     zwracak.ilosc_plikow = ilosc.ilosc_plikow;
     zwracak.ilosc_folderow = ilosc.ilosc_folderow;
     
     zwracak.pliki = (char**) malloc(ilosc.ilosc_plikow * sizeof(char*));
     zwracak.foldery = (char**) malloc(ilosc.ilosc_folderow * sizeof(char*));
     char **pliki = zwracak.pliki;
     char **foldery = zwracak.foldery;
     int i = 0;
     int j = 0;
     while(( plik = readdir( sciezka ) ) != NULL ){
          if (strcmp(plik->d_name, ".") == 0 || strcmp(plik->d_name, "..") == 0) continue;
          
          if(plik->d_type == DT_DIR){
               foldery[i] = (char*) malloc(strlen(plik->d_name) + 1);
               strcpy(foldery[i], plik->d_name);
               i++;
          }
          else if(plik->d_type == DT_REG){
               pliki[j] = (char*) malloc(strlen(plik->d_name) + 1);
               strcpy(pliki[j], plik->d_name);
               j++;
          }

          
        }
     closedir( sciezka );
     return zwracak;
     
}


/*
void zapis(const char *nazwa_sciezki){
     FILE *fptr;
     fptr = fopen("zapis.json","a");
     
     ZawartoscFolderu lista = lista_rzeczy(nazwa_sciezki);

     Ilosc ilosc = liczba(nazwa_sciezki);

     
     //zapis 
     fprintf(fptr, "{\n"); // Otwierasz obiekt folderu
     fprintf(fptr, "\"sciezka\": \"%s\",\n", nazwa_sciezki);
     fprintf(fptr, "  \"pliki\": \n[\n");
     if(lista.pliki){
          for(int i = 0; i < ilosc.ilosc_plikow;i++){
               if(i == ilosc.ilosc_plikow-1){
                    fprintf(fptr, "\"%s\",\n", lista.pliki[i]);
                    fprintf(fptr, "\"%s\"\n", rozmiar(nazwa_sciezki));
                    continue;
               }
               fprintf(fptr, "\"%s\",\n", lista.pliki[i]);
               fprintf(fptr, "\"%s\",\n", rozmiar(nazwa_sciezki));
               free(lista.pliki[i]);
          }
          free(lista.pliki);
     }
     fprintf(fptr, "],\n");

     fprintf(fptr, "  \"foldery\": \n[\n");
     if(lista.foldery){
          for(int i = 0; i < ilosc.ilosc_folderow;i++){
               if(i == ilosc.ilosc_folderow-1){
                    fprintf(fptr, "\"%s\"\n", lista.foldery[i]);
                    continue;
               }

               fprintf(fptr, "\"%s\",\n", lista.foldery[i]);
               free(lista.foldery[i]);
          }
          free(lista.foldery);
     }
     fprintf(fptr, "]\n");

     fprintf(fptr, "}\n");


     fclose(fptr);
}
*/

/*
int main( int argc, char ** argv ) {
     /*
     ZawartoscFolderu tablica = lista_rzecze(".");
     char **lista = tablica.foldery;
     Ilosc cos = liczba(".");
     if(tablica.foldery){
          for(int i = 0; i < cos.ilosc_folderow;i++){
               printf("plik %d: %s\n",i,tablica.foldery[i]);
               free(tablica.foldery[i]);
          }
          free(tablica.foldery);
     }
     
     zapis("/");


     return 0;
}
*/
