#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

int ilosc_plikow(const char *sciezka_nazwa) {
    int licznik = 0;
    struct dirent *plik;
    DIR *sciezka = opendir(sciezka_nazwa);
    if (!sciezka) return 0;
    while (readdir(sciezka)) licznik++;
    closedir(sciezka);
    return licznik;
}



char** lista_rzeczy( const char *nazwa_sciezki ) {
     int ilosc = ilosc_plikow(nazwa_sciezki);

     if(ilosc == 0) return NULL;

     char **t = (char**) malloc(ilosc * sizeof(char*));
     
     struct dirent *plik;
     DIR *sciezka = opendir(nazwa_sciezki);
     int i = 0;
     while(( plik = readdir( sciezka ) ) != NULL ){
          t[i] = (char*) malloc(strlen(plik->d_name) + 1);

          strcpy(t[i], plik->d_name);
          i++;
        }
     closedir( sciezka );
     return t;
     
}



int main( int argc, char ** argv ) {

     char **lista = lista_rzeczy(".");
     if(lista){
          for(int i = 0; i < ilosc_plikow(".");i++){
               printf("plik %d: %s\n",i,lista[i]);
               free(lista[i]);
          }
          free(lista);
     }
     return 0;
}