#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <list>

using namespace std;

class NodeAVL{
    public:
    int data;
    struct NodeAVL *left;
    struct NodeAVL *right;
    int altura;
};

NodeAVL *insertarAVL(NodeAVL* node, int valor);
int maximo(int a, int b);
int altura(NodeAVL *nod);
NodeAVL *nuevoNodeAVL(int valor);
int getBalance(NodeAVL *nod);
NodeAVL *rotacionDerecha(NodeAVL *nodo1);
NodeAVL *rotacionIzquierda(NodeAVL *nodo1);
void InOrderAVL(struct NodeAVL* root);
void preOrder(NodeAVL *rootAVL);
NodeAVL *valorMinimo(NodeAVL* nodo);
NodeAVL *eliminarNodoAVL(NodeAVL* rootAVL, int data);
float mediana(NodeAVL *rootAVL, int contador);

int main()
{
    ifstream archivo;
    int numero, contador = 0;
    char delimitador = ' ';
    string texto, textoArchivo, letra;

    NodeAVL *rootAVL = NULL;

    archivo.open("entrada.txt", ios::in);

    if(archivo.fail())
    {
        cout<<"No se pudo abrir el archivo."<<endl;
        exit(1);
    }

    while(!archivo.eof())
    {
        getline(archivo, textoArchivo);
        unsigned int posicion1 = textoArchivo.find(delimitador);
        letra = textoArchivo.substr(0, posicion1);

        unsigned int posicion2 = textoArchivo.find(delimitador, posicion1 + 1);
        texto = textoArchivo.substr(posicion1 + 1, posicion2 - posicion1 - 1);
        numero = stoi(texto);

        if(letra == "A")
            rootAVL = insertarAVL(rootAVL, numero);

        if(letra == "E")
            rootAVL = eliminarNodoAVL(rootAVL, numero);

        contador++; //Cu�ntos datos hay en total
    }

    cout<<"El valor de la mediana es: "<<mediana(rootAVL, contador)<<endl;

    archivo.close();


    return 0;
}

int maximo(int a, int b)
{
    if(a > b)
        return a;
    else if(b > a)
        return b;
}

//Funci�n que retorna el valor de la altura del �rbol.

int altura(NodeAVL *nod)
{
    if(nod == NULL) //Revisa si el nodo existe, si no existe se retorna el valor 0
        return 0;

    return nod->altura; //Si existe el nodo, retorna el valor almacenado en altura del nodo.
}

//Funci�n que crea un nuevo nodo con el valor recibido.

NodeAVL* nuevoNodeAVL(int valor)
{
    NodeAVL* nodo = new NodeAVL(); //Crea un nuevo nodo del tipo �rbol AVL.
    nodo->data = valor; //Guarda el valor dentro de la variable data del nodo.
    nodo->left = nullptr; //Inicializa los punteros a derecha e izquierda con 0.
    nodo->right = nullptr;
    nodo->altura = 1; //Inicializa la altura con 1.
    return (nodo);
};

//Calcula el factor del balance dentro del �rbol AVL. Est� definido como la diferencia entre la altura del sub�rbol izquierdo y la altura del sub�rbol derecho.

int getBalance(NodeAVL *nod)
{
    if(nod == NULL) //Revisa si el nodo esta vac�o y retorna 0 como el balance.
        return 0;
    return (altura(nod->left) - altura(nod->right)); //Retorna la diferencia entre las alturas.
}

//Realiza la rotaci�n a derecha dentro del �rbol AVL.

NodeAVL *rotacionDerecha(NodeAVL *nodo1) //La funci�n lleva el nodo a rotar.
{
    NodeAVL *nodo2 = nodo1->left; //Cea un nuevo puntero y lo establece en el hijo izquierdo del nodo a rotar. Esto debido a que se va a realizar una rotaci�n a la derecha, por lo que se requiere que este nodo sea la nueva ra�z del �rbol.
    NodeAVL *nodoAux = nodo2->right; //Se crea un nodo temporal y se establece como el hijo derecho del nodo a rotar. Esto debido a que el nodo temporal se va a convertir en el hijo izquierdo del nodo a rotar, ya que es m�s peque�o que la nueva ra�z del �rbol pero m�s grande que los dem�s nodos del sub�rbol izquierdo.

    nodo2->right = nodo1; //Se establece el hijo derecho de la nueva ra�z en el nodo a rotar.
    nodo1->left = nodoAux; //Se establece el hijo izquierdo del nodo a rotar en el nodo temporal.

    //Se actualizan las alturas de los nodos.

    nodo1->altura = maximo(altura(nodo1->left), altura(nodo1->right)) + 1;
    nodo2->altura = maximo(altura(nodo2->left), altura(nodo2->right)) + 1;

    return nodo2; //Se retorna el puntero al nuevo nodo ra�z.
};

//Realiza la rotaci�n a izqueirda dentro del �rbol AVL. Sigue los mismos pasos que la rotaci�n a derecha.

NodeAVL *rotacionIzquierda(NodeAVL *nodo1)
{
    NodeAVL *nodo2 = nodo1->right;
    NodeAVL *nodoAux = nodo2->left;

    nodo2->left = nodo1;
    nodo1->right = nodoAux;

    nodo1->altura = maximo(altura(nodo1->left), altura(nodo1->right)) + 1;
    nodo2->altura = maximo(altura(nodo2->left), altura(nodo2->right)) + 1;

    return nodo2;
};

//Funci�n para imprimir el �rbol en PreOrder

void preOrder(NodeAVL *rootAVL)
{
    if(rootAVL != nullptr) //Si el �rbol es distinto a vac�o que lo imprima.
    {
        cout << rootAVL->data << " -> ";
        preOrder(rootAVL->left);
        preOrder(rootAVL->right);
    }
}

//Funci�n para insertar dentro del �rbol AVL.

NodeAVL* insertarAVL(NodeAVL* nodo, int data)
{
    //Se usa el mismo tipo de inserci�n que en el �rbol BST pero realiza rotaciones para mantener el factor de balance entre -1 y 1

    if(nodo == nullptr) //Si el �rbol esta vac�o, crea un nuevo nodo y lo retorna.
        return(nuevoNodeAVL(data));

    if(data < nodo->data) //Si el valor es menor que el nodo actual, lo inserta en el sub�rbol izquierdo.
        nodo->left = insertarAVL(nodo->left, data);
    else if(data > nodo->data) //Si el valor es mayor que el nodo actual, lo inserta en el sub�rbol derecho.
        nodo->right = insertarAVL(nodo->right, data);
    else //Si los valores son iguales retorna el nodo para evitar duplicados.
        return nodo;

    //Actualiza la altura del nodo actual.
    nodo->altura = 1 + maximo(altura(nodo->left), altura(nodo->right));

    //Calcula el factor de balance del �rbol haciendo uso de la funci�n.
    int balanceArbol = getBalance(nodo);

    if((balanceArbol > 1) && (data < nodo->left->data)) //Si el balance es mayor a 1 y el nuevo valor es menor al del hijo izquierdo, realiza una rotaci�n a derecha.
        return rotacionDerecha(nodo);

    if((balanceArbol < -1) && (data > nodo->right->data)) //Si el balance es menor a -1 y el nuevo valor es mayor al del hijo derecho, realiza una rotaci�n a izquierda.
        return rotacionIzquierda(nodo);

    if((balanceArbol > 1) && (data > nodo->left->data)) //Si el balance es mayor a 1 y el nuevo valor es mayor al hijo izquierdo, se realiza una rotaci�n a izquierda y a derecha.
    {
        nodo->left = rotacionIzquierda(nodo->left);
        return rotacionDerecha(nodo);
    }

    if((balanceArbol < -1) && (data < nodo->right->data)) //Si el balance es menor a -1 y el nuevo valor es menor al hijo derecho, se realiza una rotaci�n a derecha y a izquierda.
    {
        nodo->right = rotacionDerecha(nodo->right);
        return rotacionIzquierda(nodo);
    }

    return nodo;
}

//Funci�n que retorna el valor m�nimo dentro del �rbol.

NodeAVL *valorMinimo(NodeAVL* nodo)
{
    NodeAVL *minimo = nodo; //Crea un nuevo nodo inicializado en el nodo recibido.

    while (minimo->left != nullptr) //El ciclo continua mientras no haya encontrado en hijo null en el puntero a izquierda.
        minimo = minimo->left;

    return minimo; //Retorna el valor m�nimo.
}

//Funci�n para eliminar datos realizando un proceso de rotaciones.

NodeAVL *eliminarNodoAVL(NodeAVL* rootAVL, int data)
{
    if(rootAVL == nullptr) //Revisa si el �rbol esta vac�o, si lo est� retorna el �rbol.
        return rootAVL;

    if(data < rootAVL->data) //Si el valor a eliminar es menor al dato en el �rbol.
        rootAVL->left = eliminarNodoAVL(rootAVL->left, data);

    else if(data > rootAVL->data) //Si el valor a eliminar es mayor al dato en el �rbol.
        rootAVL->right = eliminarNodoAVL(rootAVL->right, data);

    else //Si el valor del dato es igual a la ra�z del nodo.
    {
        if((rootAVL->left == nullptr) || (rootAVL->right == nullptr)) //Revisa si el nodo tiene 0 o un hijo.
        {

            NodeAVL *temporal;

            //Inicializa el puntero de la variable temporal ya sea al hijo izquierdo o derecho.
            if(rootAVL->left)
                temporal = rootAVL->left;
            else
                temporal = rootAVL->right;

            //Si no tiene hijos.
            if(temporal == nullptr)
            {
                temporal = rootAVL;
                rootAVL = nullptr;
            }
            else //Si tiene un hijo
                *rootAVL = *temporal; //Copia el contenido del hijo que no esta vac�o.

            free(temporal);
        }
        else
        {
            //Nodo que tiene dos hijos, obtiene el valor m�nimo para ser el sucesor.
            NodeAVL *temporal = valorMinimo(rootAVL->right);
            //Copia el valor m�nimo del sucesor.
            rootAVL->data = temporal->data;
            //Elimina el sucesor para evitar duplicaciones.
            rootAVL->right = eliminarNodoAVL(rootAVL->right, temporal->data);
        }
    }

    if(rootAVL == nullptr)
        return rootAVL;

    //Se actualiza el valor de la altura.
    rootAVL->altura = 1 + maximo(altura(rootAVL->left), altura(rootAVL->right));

    //Se obtiene el valor del balanceo del �rbol.
    int balanceo = getBalance(rootAVL);

    //En caso de que no este balanceado se realiza un proceso de rotaci�n (explicado en inserci�n).
    if((balanceo > 1) && (getBalance(rootAVL->left) >= 0))
        return rotacionDerecha(rootAVL);

    if((balanceo > 1) && (getBalance(rootAVL->left) < 0))
    {
        rootAVL->left = rotacionIzquierda(rootAVL->left);
        return rotacionDerecha(rootAVL);
    }

    if((balanceo < -1) && getBalance(rootAVL->right) <= 0)
        return rotacionIzquierda(rootAVL);

    if((balanceo < -1) && (getBalance(rootAVL->right) > 0))
    {
        rootAVL->right = rotacionDerecha(rootAVL->right);
        return rotacionIzquierda(rootAVL);
    }

    return rootAVL;

}

float mediana(NodeAVL* rootAVL, int contador)
{
    if(rootAVL == nullptr)
        return 0;

    int contadorActual = 0;
    NodeAVL *actual = rootAVL, *pre, *prev;

    while(actual != NULL)
    {
        if(actual->left == nullptr)
        {
            contadorActual++; //Cuanta el nodo actual

            //En caso de que el nodo actual sea la media, caso impar
            if(contador % 2 != 0 && contadorActual == (contador+1)/2)
                return actual->data;

            //Caso par
            else if(contador % 2 == 0 && contadorActual == (contador/2) + 1)
                return (prev->data + actual->data)/2;

            //Actualizar prev con el n�mero par de los nodos

            prev = actual;

            //Mover a la derecha

            actual = actual->right;
        }
        else
        {
            //Encontrar predecesor
            pre = actual->left;

            while(pre->right != NULL && pre->right != actual)
                pre = pre->right;

            //Actual como el hijo derecho del predecesor
            if(pre->right == NULL)
            {
                pre->right = actual;
                actual = actual->left;
            }
            //Revertir los cambios
            else
            {
                pre->right = NULL;

                prev = pre;

                //Aumentar el contador actual
                contadorActual++;

                //Revisar si el nodo actual es la mediana
                if(contador % 2 != 0 && contadorActual == (contador+1)/2)
                    return actual->data;

                else if(contador % 2 == 0 && contadorActual == (contador/2)+1)
                    return(prev->data + actual->data)/2;

                //Actualizar nodo prev en caso de n�mero par de nodos

                prev = actual;
                actual = actual->right;
            }
        }
    }
}

