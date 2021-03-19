#include <stdio.h>
#include <stdlib.h>
#include "afd.h"

int main(int argc, char *argv[])
{
    FILE* PunteroENTRADA;
    PunteroENTRADA=fopen("AFD.txt","r");
    FILE* PunteroSALIDA;
    PunteroSALIDA=fopen("SALIDA.txt","w+");
    
    int* nodo_inicial;
    int nodos_finales[3];
    int* cant_finales;
    
    int opcion;
    int ing_correcto=1;
    
    PtrAutomata cabeza_afd=NULL,recorrer;
    
    nodo_inicial=(int*)malloc(sizeof(int*));
    cant_finales=(int*)malloc(sizeof(int*));
  
    if(PunteroENTRADA==NULL || PunteroSALIDA==NULL)
    {
        puts("Error de apertura de archivos. El programa se detendrá.");
        system("PAUSE");
        exit(-1);
    }
        
    cabeza_afd=CargarAutomata(PunteroENTRADA,nodo_inicial,nodos_finales,cant_finales);
    
    if(!EsAFD(cabeza_afd))
    {
        printf("El automata ingresado no es un AFD. El programa terminara su ejecucion");
        system("PAUSE");
        exit -1;
    }
	
	puts("1. Generar ER a partir de un AFD");
	puts("2. Generar G3 a partir de un AFD");
	puts("");
	printf("Ingrese una opcion: ");
	scanf("%d",&opcion);
	puts("");
	
    switch(opcion)
    {
        case 1:
        {
            ProcesarLista(cabeza_afd);
            BuscarUniones(cabeza_afd);
            BuscarClausuras(cabeza_afd);
            EliminarEstados (cabeza_afd,nodo_inicial,nodos_finales,cant_finales,PunteroSALIDA);
            break;
        }
        case 2:
        {
            ing_correcto=1;
            Obtener_G3(cabeza_afd, nodos_finales, cant_finales);
            leer_G3();
            break;
        }
        default: 
        {
            printf("Error en el ingreso, el programa terminara su ejecucion.\n");
            exit -1;
            break;
        }  
    }
    
    system("PAUSE");
	
	fclose(PunteroENTRADA);
	fclose(PunteroSALIDA);
    
    return 0;
}
