#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "afd.h"

PtrAutomata CargarAutomata (FILE* PtrArchivo, int* nd_inicial, int* nd_finales, int* num_fns)
{
    int i=0,j=0; //Contador de la cantidad de números de estados finales
    int org_aux, lleg_aux;
    char trans_aux;
    
    PtrAutomata afd=NULL;
    
    puts("Cargando automata en memoria...");
    puts("");
    
    while(!feof(PtrArchivo)) //Se recorre el archivo de principio a fin para cargar los datos
    {
        //sleep(700);
        
        if(i>2)
        {
            puts("Error. El autómata puede tener, a lo sumo, tres estados finales.");
            exit -1;
        }
        
        fscanf(PtrArchivo,"%d",&org_aux);
        fscanf(PtrArchivo,"%c",&trans_aux);
        
        if(trans_aux=='!')
        {
            *nd_inicial=org_aux;
            fscanf(PtrArchivo,"\n");
            printf("%d%c\n",org_aux,trans_aux);
        }
        
        if(trans_aux=='#')
        {    
            nd_finales[i]=org_aux;
            i++;
            fscanf(PtrArchivo,"\n");
            printf("%d%c\n",org_aux,trans_aux);
        }
    }
    
    puts("");
    printf("Estado inicial (!): %d\n",*nd_inicial);
    printf("Cantidad de estados finales: %d\n",i);
    printf("Estado(s) finales (#): ");
    
    while(j<i)
    {
        printf("%d ",nd_finales[j]);
        j++;
    }
    
    
    
    printf("\n\n"); 
    
    rewind(PtrArchivo);
    
    while(!feof(PtrArchivo))
    {   
        fscanf(PtrArchivo,"%d",&org_aux);
        fscanf(PtrArchivo,"%c",&trans_aux);
        
        if(trans_aux!='#' && trans_aux!='!')
        {
            fscanf(PtrArchivo,"%d",&lleg_aux);
            afd=InsertarNodoLista (afd,org_aux,trans_aux,lleg_aux);
        }
        
        fscanf(PtrArchivo,"\n");      
    }
    
    printf("\n\n"); 
    
    *num_fns=i;
    
    return afd;
}

PtrAutomata InsertarNodoLista (PtrAutomata afd, int ent_origen, char ent_transicion, int ent_llegada)
{
    PtrAutomata nodo;
    
    nodo=(PtrAutomata)malloc(sizeof(PtrAutomata));
    
    if (nodo==NULL)
        exit -1;
        
    nodo->origen=ent_origen;
    nodo->transicion=ent_transicion;
    nodo->llegada=ent_llegada;
    
    nodo->siguiente=afd;
    afd=nodo;
    
    return afd;
}

PtrAutomata InsertarCopiaLista (PtrAutomata afd, int ent_origen, char* ent_ereg, int ent_llegada)
{
    PtrAutomata nodo;
    char cad_aux[200];
    
    nodo=(PtrAutomata)malloc(sizeof(PtrAutomata));
    
    if (nodo==NULL)
        exit -1;
        
    strcpy(cad_aux,ent_ereg);
    
    nodo->origen=ent_origen;
    
    nodo->ereg=(char*)malloc(sizeof(char*)*(strlen(cad_aux)+1));
    strcpy(nodo->ereg,cad_aux);
    
    nodo->llegada=ent_llegada;
    
    nodo->siguiente=afd;
    afd=nodo;
    
    return afd;
}

short EsAFD (PtrAutomata afd)
{
    PtrAutomata actual,comparador;
    
    short comp_org;
    short comp_trans;
    short comp_lleg;
    
    short esafd=1;
    
    for(actual=afd;actual!=NULL;actual=actual->siguiente)
    {
        
        for(comparador=afd;comparador!=NULL;comparador=comparador->siguiente)
        {
            comp_org=actual->origen == comparador->origen;
            comp_trans=actual->transicion == comparador->transicion;
            comp_lleg=actual->llegada != comparador->llegada;
            
            if(comp_org && comp_trans && comp_lleg)
                esafd=0;
        }
    }
    return esafd;
}

void ProcesarLista(PtrAutomata afd)
{
    PtrAutomata actual;
    
    for(actual=afd;actual!=NULL;actual=actual->siguiente)
        ConvCharExpReg(actual);
}

void ConvCharExpReg (PtrAutomata afd)
{
    char cad_aux[2];
    
    cad_aux[0]=afd->transicion;
    cad_aux[1]='\0';
    
    afd->ereg=(char*)malloc(sizeof(cad_aux)+1);
    
    strcpy(afd->ereg,cad_aux);
}

void BorrarNodoLista (PtrAutomata afd, int org_buscado, char* ereg_buscada, int lleg_buscada)
{
    PtrAutomata actual;
    PtrAutomata anterior;
    char cad_aux[200];
    
    //Los comparadores almacenarán el valor lógico de la comparación entre los orígenes, transiciones y llegadas.
    
    short comp_org;
    short comp_trans;
    short comp_lleg;
    
    short encontrado=1;
    
    actual=afd;
    
    while(actual!=NULL && encontrado)   
    {
        comp_org = org_buscado==actual->origen; 
        comp_trans = !(strcmp(ereg_buscada,actual->ereg)); 
        comp_lleg = lleg_buscada==actual->llegada; 
        
        if(comp_org && comp_trans && comp_lleg)
        {
            encontrado=0;
            
            if(actual==afd)
                afd=actual->siguiente;
            
            else
                anterior->siguiente=actual->siguiente;
            
            free(actual);
        }
        anterior=actual;
        actual=actual->siguiente;
    }
    
}

void BuscarUniones (PtrAutomata afd)
{
    PtrAutomata actual,comparador,tercer_cadena;
    
    int ocurrencia; //Determina si hay ocurrencias de uniones.
    int tercercadena;
    
    int comp_org;   //Comparador de origen
    int comp_trans; //Comparador de transiciones
    int comp_lleg;  //Comparador de llegadas
    
    actual=afd;
    
    while(actual!=NULL)
    {   
        ocurrencia=0; //Si hay una ocurrencia de unión al recorrer mediante actual, cambia a 1.
        tercercadena=0;
        
        comparador=afd;
        
        while(comparador!=NULL)
        {            
            if(actual->origen==comparador->origen && strcmp(actual->ereg,comparador->ereg) && actual->llegada==comparador->llegada) //Compara orígenes y llegadas y que la transición sea distinta (strcmp != 0)
            {                
                ocurrencia=1;
                
                tercer_cadena=afd;  
                
                while(tercer_cadena!=NULL)
                {
                    if(tercer_cadena->origen==comparador->origen && tercer_cadena->llegada==comparador->llegada && tercer_cadena->ereg!=comparador->ereg && tercer_cadena->ereg!=actual->ereg)
                    {
                        tercercadena=1;
                        
                        InsertarUniones(actual,actual->ereg,comparador->ereg,tercer_cadena->ereg,comparador->origen,comparador->llegada);
                        BorrarNodoLista(afd,tercer_cadena->origen,tercer_cadena->ereg,tercer_cadena->llegada);
                        BorrarNodoLista(afd,comparador->origen,comparador->ereg,comparador->llegada);
                        
                        break;
                    }
              
                    tercer_cadena=tercer_cadena->siguiente;
                }
                
                if(!tercercadena)
                {
                    InsertarUniones(actual,actual->ereg,comparador->ereg,NULL,comparador->origen,comparador->llegada);  
                    BorrarNodoLista(afd,comparador->origen,comparador->ereg,comparador->llegada);
                }
                else
                    break;    
            }   
                
            comparador=comparador->siguiente;
        }
        
        actual=actual->siguiente;
    }
}

void InsertarUniones(PtrAutomata entrada, char* ereg1, char* ereg2, char* ereg3,int ent_org, int ent_lleg)
{      
    char cad_aux1[200]=""; 
    char cad_aux2[200]=""; 
    char cad_aux3[200]="";
    
    char cad_union[200]="(";
    
    strcpy(cad_aux1,ereg1);
    strcpy(cad_aux2,ereg2);
    
    cad_aux1[strlen(cad_aux1)]='+';
    cad_aux1[strlen(cad_aux1)+1]='\0';
    
    strcat(cad_aux1,cad_aux2);
    strcat(cad_union,cad_aux1);
    
    if(ereg3!=NULL)
    {
        strcpy(cad_aux3,ereg3);
        cad_union[strlen(cad_union)]='+';
        cad_union[strlen(cad_union)+1]='\0';
        strcat(cad_union,cad_aux3);
    }
        
    cad_union[strlen(cad_union)]=')';
    cad_union[strlen(cad_union)+1]='\0';
    
    entrada->ereg=(char*)malloc(sizeof(char*)*(strlen(cad_union)));
    strcpy(entrada->ereg,cad_union);
       
    //printf("%d%s%d\n",entrada->origen,entrada->ereg,entrada->llegada);   
}

void BuscarClausuras (PtrAutomata afd)
{
    PtrAutomata comparador;
    
    for(comparador=afd;comparador!=NULL;comparador=comparador->siguiente)
    {   
        if(comparador->origen==comparador->llegada)
        {
            comparador->ereg=InsertarClausuras(comparador->ereg);
        }
    }
}

char* InsertarClausuras (char* ereg)
{
    char cad_aux[200];
    char clausura[]="*";
    
    strcpy(cad_aux,ereg);
    strcat(cad_aux,clausura);
    
    ereg=(char*)malloc(sizeof(char*)*(strlen(cad_aux)+1));
    
    strcpy(ereg,cad_aux);
    
    return ereg;
}

void EliminarEstados (PtrAutomata afd, int* est_in, int* est_fls, int* cant_fls, FILE* PtrSALIDA)
{
    PtrAutomata recorrer;
    
    PtrAutomata lista1=NULL;
    PtrAutomata lista2=NULL;
    PtrAutomata lista3=NULL;
    
    int inicial=*est_in;
    int cant_finales=*cant_fls;
    
    char cad_lista1[200];
    char cad_lista2[200];
    char cad_lista3[200];
    
    for(recorrer=afd;recorrer!=NULL;recorrer=recorrer->siguiente)
    {
        lista1=InsertarCopiaLista(lista1,recorrer->origen,recorrer->ereg,recorrer->llegada);
        lista2=InsertarCopiaLista(lista2,recorrer->origen,recorrer->ereg,recorrer->llegada);
        lista3=InsertarCopiaLista(lista3,recorrer->origen,recorrer->ereg,recorrer->llegada);
    }
    
    puts("Expresion regular:\n");
    
    switch(cant_finales)
    {
        case(1):
            {
                BorrarEstados(lista1,inicial,est_fls[0],PtrSALIDA);   
                printf("\n\n");
            } break;
        case(2):
            {
                BorrarEstados(lista1,inicial,est_fls[0],PtrSALIDA);
                printf(" + ");
                BorrarEstados(lista2,inicial,est_fls[1],PtrSALIDA);
                printf("\n\n");
            }; break;
        case(3):
            {
                BorrarEstados(lista1,inicial,est_fls[0],PtrSALIDA);
                printf(" + ");
                BorrarEstados(lista2,inicial,est_fls[1],PtrSALIDA);
                printf(" + ");
                BorrarEstados(lista3,inicial,est_fls[2],PtrSALIDA);
                printf("\n\n");
            }; break;
    }   
}

void BorrarEstados (PtrAutomata afd, int est_ini, int est_fl, FILE* PtrENTRADA)
{
    PtrAutomata actual,recorrer;
    PtrAutomata parten=NULL,llegan=NULL,rcr_parten,rcr_llegan;
    PtrAutomata nuevo;
    
    FILE* PtrArchivo;
    
    //PtrArchivo=fopen("SALIDA.txt","w+");
    
    int i=0,j=0,k,coincide=0;
    int bucle;
    int estado_mayor=0;
    
    int* conj_estados=ListarEstados(afd,conj_estados);
    int est_borrados[200];
    
    char bucle_cad[200];
    char cad_aux[200];
    
    int long_cadaux;
    
    char R[100]=""; //Cadenas para generar la forma
    char S[100]="";
    char T[100]="";
    char U[100]="";
    
    for(recorrer=afd;recorrer!=NULL;recorrer=recorrer->siguiente)  //Busca el mayor número de estado.
    {
        if((recorrer->origen)>=estado_mayor || (recorrer->llegada)>=estado_mayor)
        {
            if((recorrer->origen)>(recorrer->llegada))
                estado_mayor=recorrer->origen;
            else
                estado_mayor=recorrer->llegada;
        }    
    }
    
    for(actual=afd;actual!=NULL && i<=estado_mayor;actual=actual->siguiente)
    {
        
        bucle=j=0;
        
        for(recorrer=afd;recorrer!=NULL;recorrer=recorrer->siguiente) //Busca si existe un bucle en el nodo i del autómata.
        {
            if(recorrer->origen==recorrer->llegada && recorrer->origen==recorrer->llegada==i)
            {
                if(recorrer->origen==est_ini)
                {
                    strcpy(R,recorrer->ereg);
                }
                    
                if(recorrer->origen==est_fl)
                {
                    strcpy(U,recorrer->ereg);
                }
                    
                strcpy(bucle_cad,recorrer->ereg);
                bucle=1;
                
                break;
            }
        }
        
        for(recorrer=afd;recorrer!=NULL;recorrer=recorrer->siguiente) //Recorre para buscar coincidencias en las llegadas y los orígenes al estado a borrar.
        {   
            if(i!=est_ini && i!=est_fl)
            {
                
                if(i==recorrer->llegada && recorrer->origen!=recorrer->llegada)
                {
                    llegan=InsertarCopiaLista(llegan,recorrer->origen,recorrer->ereg,recorrer->llegada);
                }
                
                if(i==recorrer->origen && recorrer->origen!=recorrer->llegada)
                {
                    parten=InsertarCopiaLista(parten,recorrer->origen,recorrer->ereg,recorrer->llegada);;
                }   
            }        
        }
        
        for(rcr_llegan=llegan;rcr_llegan!=NULL;rcr_llegan=rcr_llegan->siguiente)
        {   
            //printf("LLEGA A  i: %d %s %d\n",rcr_llegan->origen,rcr_llegan->ereg,rcr_llegan->llegada);
            
            for(rcr_parten=parten;rcr_parten!=NULL;rcr_parten=rcr_parten->siguiente)
            {
                cad_aux[0]='\0';
                
                //printf("PARTE DE i: %d %s %d\n",rcr_parten->origen,rcr_parten->ereg,rcr_parten->llegada);
                
                nuevo=(PtrAutomata)malloc(sizeof(PtrAutomata));
                
                nuevo->origen=rcr_llegan->origen;
                nuevo->llegada=rcr_parten->llegada;
                
                if(nuevo->origen==nuevo->llegada)
                {   
                    cad_aux[0]='(';
                    cad_aux[1]='\0';
                    
                    if(nuevo->origen==est_ini)
                    {
                        strcat(cad_aux,R);
                    }
                    if(nuevo->origen==est_fl)
                    {
                        strcat(cad_aux,U);
                    }
                }
                
                strcat(cad_aux,rcr_llegan->ereg);
                
                if(bucle)
                    strcat(cad_aux,bucle_cad);
                    
                strcat(cad_aux,rcr_parten->ereg);
                
                if(nuevo->origen==nuevo->llegada)
                {
                    long_cadaux=strlen(cad_aux);
                    
                    cad_aux[long_cadaux]=')';
                    cad_aux[long_cadaux+1]='\0';
                    
                    nuevo->ereg=InsertarClausuras(cad_aux);
                }
                else
                {
                    nuevo->ereg=(char*)malloc(sizeof(char*)*(strlen(cad_aux)+1));
                    strcpy(nuevo->ereg,cad_aux);
                }
              
                //printf("%d%s%d\n",nuevo->origen,nuevo->ereg,nuevo->llegada);
                
                if(nuevo->origen==est_ini && nuevo->llegada==est_ini)
                {
                    strcpy(R,nuevo->ereg);
                }
                
                if(nuevo->origen==est_ini && nuevo->llegada==est_fl)
                {
                    strcpy(S,nuevo->ereg);
                }
                
                if(nuevo->origen==est_fl && nuevo->llegada==est_ini)
                {
                    strcpy(T,nuevo->ereg);
                }
                
                if(nuevo->origen==est_fl && nuevo->llegada==est_fl)
                {
                    strcpy(U,nuevo->ereg);
                }
                
                nuevo->siguiente=afd;
                afd=nuevo;
                
                BorrarNodoLista(afd,rcr_parten->origen,rcr_parten->ereg,rcr_parten->llegada);
            }
            BorrarNodoLista(afd,rcr_llegan->origen,rcr_llegan->ereg,rcr_llegan->llegada);
            
        }
            
        parten=llegan=NULL;
        
        est_borrados[i]=i;
        est_borrados[i+1]=-1;
        
        i++;
        
        fprintf(PtrENTRADA,"%c",'(');
        
        while(conj_estados[j]!=-1)
        {
            if(conj_estados[j]==est_ini || conj_estados[j]==est_fl)
            {
                fprintf(PtrENTRADA,"%d",conj_estados[j]);
            }
            else
            {
                k=coincide=0;
                
                while(est_borrados[k]!=-1)
                {
                    if(conj_estados[j]==est_borrados[k])
                        coincide=1;
                    k++;
                }
                
                if(!coincide)
                    fprintf(PtrENTRADA,"%d",conj_estados[j]);
            }
            
            j++;    
        }
        
        fprintf(PtrENTRADA,"%c\n",')');
    }
    fprintf(PtrENTRADA,"\n",')');
    
    if(est_ini!=est_fl)
        printf("(%s+%s%s%s)*%s%s",R,S,U,T,S,U);
    else
        printf("%s",R);
    
    return;
}

int* ListarEstados (PtrAutomata afd, int* lista_estados)
{
    PtrAutomata recorrer;
    int estados[200];
    int i=0;
    int estado_mayor=0;
    
    for(recorrer=afd;recorrer!=NULL;recorrer=recorrer->siguiente)  //Busca el mayor número de estado.
    {
        if((recorrer->origen)>estado_mayor || (recorrer->llegada)>estado_mayor)
        {
            if((recorrer->origen)>(recorrer->llegada))
                estado_mayor=recorrer->origen;
            else
                estado_mayor=recorrer->llegada;
        }    
    }
    
    while(i<=estado_mayor)
    {
        estados[i]=i;
        i++;
    }
    
    estados[i]=-1;
    
    lista_estados=(int*)malloc(i*(sizeof(int*)+1));
    
    i=0;
    
    while(estados[i]!=-1)
    {
        lista_estados[i]=estados[i];
        i++;
    }
    
    lista_estados[i]=-1;
    
    i=0;
    
    return lista_estados;
}

void Obtener_G3(PtrAutomata cabeza_afd, int* nodos_finales, int* cant_finales)
{
	PtrAutomata anterior;
	PtrAutomata actual;
	
	int i,flag;
	
	FILE* PunteroArchivo;
	PunteroArchivo=fopen("G3.txt","w");
	
	if(PunteroArchivo == NULL)
		printf("\nNo se puede abrir el archivo");
		
    else
	{
        actual=cabeza_afd;
        
        while(actual != NULL)
        {   
            i=flag=0;
            
            while(i<*cant_finales && !flag)
            {
                if(actual->llegada==nodos_finales[i]) //Si es estado final, se imprime sólo la transición, que es un solo símbolo terminal.
                {
                    fprintf(PunteroArchivo,"%i:%c\n",actual->origen,actual->transicion);
                    flag=1;
                }
                
                i++;
                
                if(i==*cant_finales && !flag) //Si i es igual a la cantidad de estados finales y la bandera no hizo switch de 0 a 1, no es estado final, y se imprime el terminal y el no terminal (número del estado llegada).
                {
                    fprintf(PunteroArchivo,"%i:%c%i\n",actual->origen,actual->transicion,actual->llegada);        
                }
            }
            
			actual=actual->siguiente;
		}
		
		fclose(PunteroArchivo);     
	}
}

void leer_G3(void)
{
	char transicion,origen,llegada;
	FILE* PtrArchivo=fopen("G3.txt","r");
	
	char aux[5];
	
	printf("Gramatica G3 generada:\n\n");
	
	while(!feof(PtrArchivo))
	{
        fscanf(PtrArchivo,"%c:%c%c",&origen,&transicion,&llegada);
        
        if(llegada=='\n') //En caso de que sea nodo final, y no haya símbolo no terminal a la izquierda de la transición.
            printf("%c:%c\n",origen,transicion);
        else //Nodo que no posee transición hacia el final.
        {
            printf("%c:%c%c\n",origen,transicion,llegada);
            fscanf(PtrArchivo,"\n");
        }
	}
	
	puts("");
	
	fclose(PtrArchivo);
}
