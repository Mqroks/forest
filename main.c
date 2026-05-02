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
Ilosc liczba(const char *sciezka_nazwa) {
    Ilosc licznik = {0,0};
    struct dirent *plik;
    DIR *sciezka = opendir(sciezka_nazwa);
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



int main( int argc, char ** argv ) {

     ZawartoscFolderu tablica = lista_rzeczy(".");
     char **lista = tablica.foldery;
     Ilosc cos = liczba(".");
     if(lista){
          for(int i = 0; i < cos.ilosc_folderow;i++){
               printf("plik %d: %s\n",i,lista[i]);
               free(lista[i]);
          }
          free(lista);
     }
     return 0;
}