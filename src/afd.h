/*Archivo de definición para el tipo de dato AUTÓMATA FINITO DETERMINÍSTICO*/

struct AutomataFinito
{
    int origen;
    int llegada;
    char transicion;
    char* ereg;
    struct AutomataFinito* siguiente;
};

typedef struct AutomataFinito Automata;
typedef Automata* PtrAutomata;

PtrAutomata CargarAutomata (FILE* PtrArchivo, int* nd_inicial, int* nd_finales, int* cant_fns); //Carga el autómata desde el archivo de texto.
short EsAFD (Automata* afd); //Toma como entrada la cabeza de la lista enlazada con el autómata y valida que el mismo sea un AFD (retorna 1) y no AFN (retorna 0).

void ProcesarLista(PtrAutomata afd); //Recorre la lista enlazada para procesarla y modificarla mediante otras funciones

PtrAutomata InsertarNodoLista (PtrAutomata afd, int origen, char transicion, int llegada); //Pasa los datos del autómata desde el archivo a la lista enlazada.
void BorrarNodoLista (PtrAutomata afd, int org_buscado, char* ereg_buscada, int lleg_buscada); //Borra un nodo de la lista enlazada que contiene al autómata.

void ConvCharExpReg (PtrAutomata afd); //Convierte la transición inicial del autómata a una expresión regular de símbolo único.
void BuscarUniones (PtrAutomata afd); //Busca en la lista del autómata aquellas transiciones que coincidan en origen y llegada.
void BuscarClausuras (PtrAutomata afd); //Busca en la lista del autómata aquellas transiciones que coincidan en origen y llegada y además origen==llegada.
void InsertarUniones (PtrAutomata afd, char* ereg1, char* ereg2, char* ereg3, int ent_org, int ent_lleg); //Inserta las uniones encontradas en la lista enlazada y elimina los nodos redundantes.
char* InsertarClausuras (char* ereg); //Inserta las clausuras encontradas en la lista enlazada (agrega * a los nodos que tengan origen==llegada)

void EliminarEstados (PtrAutomata afd, int* est_in, int* est_fls, int* cant_fns, FILE* PtrArchivo); //Inicia el proceso de eliminación de estados, tomando entre opciones de procesamiento acorde a la cantiddad de estados finales.
PtrAutomata InsertarCopiaLista (PtrAutomata afd, int ent_origen, char* ent_ereg, int ent_llegada); //Se encarga de hacer copias exactas de la lista del AFD ya procesado.
void BorrarEstados (PtrAutomata afd, int est_ini, int est_fl, FILE*PtrArchivo); //Borra los estados respetando el estado inicial y el estado final del AFD que se de como entrada.

int* ListarEstados (PtrAutomata afd, int* lista_estados); //Genera la lista de estados de la que consta el autómata.

void Obtener_G3(PtrAutomata cabeza_afd, int* nodos_finales, int* cant_finales); //Obtiene la G3 a partir de la lista del AFD.
void leer_G3(void); //Lee la G3 a partir del archivo e imprime las reglas de produccion en pantalla.
