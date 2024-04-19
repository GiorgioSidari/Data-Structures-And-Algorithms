#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>

#define NUMCARMAX 512
typedef struct AlberoAutonomia *alberoAuto;
typedef struct BinarySearchTree *stazione;
typedef struct PercorsoInOrder *percorsoInOrder;


struct PercorsoInOrder {
    int distancefromstart;
    int maxAutonomia;
    percorsoInOrder next;

};

struct AlberoAutonomia{
    int autonomia;
    int numAutoUgualeAutonomia;
    alberoAuto left;
    alberoAuto right;
};

struct BinarySearchTree {
    int distanceFromStart;
    stazione left;
    stazione right;
    int numAuto;
    alberoAuto alberoAutonomie;
};

alberoAuto AutoSuccessiva(alberoAuto radice, alberoAuto T);

void CancellaLista(percorsoInOrder lista);

alberoAuto CopiaAlbero(alberoAuto vecchio);

void Stampa(percorsoInOrder lista);


void PianificaPercorso(stazione radice, int partenza, int arrivo);

void cancellaAlberoAutonomie(alberoAuto radice);

bool EsisteAutonomia(alberoAuto radice, int autonomia);

int TappaSuccessiva(percorsoInOrder lista);

percorsoInOrder InserisciInTestaPIO(percorsoInOrder lista, int distance, int maxAuto);

percorsoInOrder InserisciInCoda(percorsoInOrder lista, int distanza, int max);

alberoAuto DemolisciAuto(alberoAuto radice, int autonomia);

alberoAuto AutoPrecedenteAlbero(alberoAuto radice,int autonomia);

alberoAuto CercaAutonomia(alberoAuto radice, int autonomia);

percorsoInOrder PianificaPercorsoAndata(stazione radice, int partenza, int arrivo, percorsoInOrder list);

percorsoInOrder PianificaPercorsoRitorno(stazione radice, int partenza, int arrivo, percorsoInOrder list);

stazione StazionePrecedente(stazione radice, int distance); // ritorna il nodo precedente a quello con distanza distance

stazione StazioneSuccessiva(stazione radice, stazione T);


stazione AggiungiStazione(stazione T, int distance, int numAuto, int autonomie[]);

stazione CercaStazione(stazione radice, int distance); // partendo dalla radice restituisce la stazione a distanza distance

stazione DemolisciStazione(stazione radice, int distance);

bool EsisteStazione(stazione T, int distance);


void AggiungiAuto(stazione radice, int distance, int autonomia);

void RottamaAuto(stazione radice, int distance, int autonomia);

int maxAlbero(alberoAuto radice);

void cancellaAlbero(stazione radice);

void StampaAlContrario(percorsoInOrder listaD, int inizio);

int main() {


    stazione T= NULL;

    FILE * file =stdin;


        char input[20];
        int distanza, numAuto, autonomia;
        int distanza2;
        int autonomie[NUMCARMAX];
        while (fscanf(file, "%s", input) != EOF) {
            if (strcmp(input, "aggiungi-stazione") == 0) {
                if (fscanf(file, "%d %d", &distanza, &numAuto) != EOF) {
                    for (int i = 0; i < numAuto; i++) {
                        if (fscanf(file, "%d", &autonomie[i]) != EOF);
                    }
                    T = AggiungiStazione(T, distanza, numAuto, autonomie);
                }
            } else if (strcmp(input, "demolisci-stazione") == 0) {
                if (fscanf(file, "%d", &distanza) != EOF) {
                    T = DemolisciStazione(T, distanza);
                }
            } else if (strcmp(input, "aggiungi-auto") == 0) {
                if (fscanf(file, "%d %d", &distanza, &autonomia)) {
                    AggiungiAuto(T, distanza, autonomia);
                }
            } else if (strcmp(input, "rottama-auto") == 0) {
                if (fscanf(file, "%d %d", &distanza, &autonomia)) {
                    RottamaAuto(T, distanza, autonomia);
                }
            } else if (strcmp(input, "pianifica-percorso") == 0) {
                if (fscanf(file, "%d %d", &distanza, &distanza2)) {
                    PianificaPercorso(T, distanza, distanza2);
                }
            }

        }
        cancellaAlbero(T);
    return 0;
}
// Crea nodo nell'albero con la nuova stazione, se non esiste già
stazione AggiungiStazione(stazione T, int distance, int numAuto, int autonomie[]) {
    if (T != NULL) {
        if (distance > T->distanceFromStart) {
            T->right = AggiungiStazione(T->right, distance, numAuto, autonomie);

        } else if (distance < T->distanceFromStart) { // stessa cosa per i nodi a sinistra solo che il controllo è opposto

            T->left = AggiungiStazione(T->left, distance, numAuto, autonomie);
        } else {
            printf("non aggiunta\n");
            //la stazione già esiste
        }
    } else { // se arrivo qui sono in un nodo nullo, allora creo con una malloc
        T = (stazione) malloc(sizeof(struct BinarySearchTree));
        T->distanceFromStart = distance;
        T->alberoAutonomie = NULL;
        T->left = NULL;
        T->right = NULL;
        T->numAuto = numAuto;
        for (int i = 0; i < numAuto; i++) {
            //aggiungo al suo interno un BST con le autonomie delle macchine
                alberoAuto alberoAuto1 = T->alberoAutonomie;
                alberoAuto temp = T->alberoAutonomie;
                if(T->alberoAutonomie==NULL){
                    alberoAuto1 = (alberoAuto) malloc(sizeof (struct AlberoAutonomia));
                    alberoAuto1->autonomia= autonomie[i];
                    alberoAuto1->left = NULL;
                    alberoAuto1->right = NULL;
                    alberoAuto1->numAutoUgualeAutonomia = 1;
                    T->alberoAutonomie = alberoAuto1;
                }else{
                    alberoAuto1 = CercaAutonomia(T->alberoAutonomie, autonomie[i]);
                    if(alberoAuto1==NULL){
                        alberoAuto1 = (alberoAuto) malloc(sizeof (struct AlberoAutonomia));
                        alberoAuto1->autonomia= autonomie[i];
                        alberoAuto1->left = NULL;
                        alberoAuto1->right = NULL;
                        alberoAuto1->numAutoUgualeAutonomia = 1;
                        while(temp!=NULL){
                            if(temp->left== NULL && temp->autonomia>alberoAuto1->autonomia){
                                temp->left = alberoAuto1;
                                break;
                            }else if(temp->right==NULL && temp->autonomia< alberoAuto1->autonomia){
                                temp->right = alberoAuto1;
                                break;
                            }else if(temp->autonomia>alberoAuto1->autonomia){
                                temp = temp->left;
                            }else if(temp->autonomia< alberoAuto1->autonomia){
                                temp = temp->right;
                            }
                        }

                    }else{
                        //se l'autonomia all'interno del nodo esiste già, aumento il contatore, non aggiungo
                        // un nuovo nodo
                        alberoAuto1->numAutoUgualeAutonomia++;
                    }
                }

        }
        printf("aggiunta\n");
        return T;
    }
    return T;

}
//cerca autonomia macchina all'interno di un nodo stazione
alberoAuto CercaAutonomia(alberoAuto radice, int autonomia){
    if (radice == NULL)
        return NULL;
    else {
        if (radice->autonomia > autonomia)
            return CercaAutonomia(radice->left, autonomia);
        else if (radice->autonomia < autonomia)
            return CercaAutonomia(radice->right, autonomia);
        else if (radice->autonomia == autonomia)
            return radice;
        else {
            return NULL;
        }

    }
}


//cerca nodo all'interno dell'albero delle stazioni
stazione CercaStazione(stazione radice, int distance) {
    if (radice == NULL)
        return NULL;
    else {
        if (radice->distanceFromStart > distance)
            return CercaStazione(radice->left, distance);
        else if (radice->distanceFromStart < distance)
            return CercaStazione(radice->right, distance);
        else if (radice->distanceFromStart == distance)
            return radice;
        else {
            return NULL;
        }

    }

}
// cancella nodo dal BST coprendo i tre diversi casi: il nodo è una foglia, il nodo ha un solo figlio,
// il nodo ha due figli
stazione DemolisciStazione(stazione radice, int distance){
    if(radice==NULL)
        return NULL;
    if(!EsisteStazione(radice,distance)){
        printf("non demolita\n");
        return radice;
    }else{
        if(radice->distanceFromStart>distance){
            radice->left = DemolisciStazione(radice->left,distance);

            return radice;
        }
        else if(radice->distanceFromStart<distance){
            radice->right = DemolisciStazione(radice->right,distance);

            return radice;
        }
        if (radice->left == NULL) {
            stazione temp = radice->right;
            free(radice);
            printf("demolita\n");
            return temp;
        } else if (radice->right == NULL) {
            stazione temp = radice->left;
            free(radice);
            printf("demolita\n");
            return temp;
        }else{
            stazione prev = radice;
            stazione succ = radice->right;
            while (succ->left != NULL) {
                prev = succ;
                succ = succ->left;
            }
            if (prev != radice)
                prev->left = succ->right;
            else
                prev->right = succ->right;


            radice->distanceFromStart = succ->distanceFromStart;
            radice->numAuto = succ->numAuto;
            cancellaAlberoAutonomie(radice->alberoAutonomie);
            radice->alberoAutonomie=NULL;
            radice->alberoAutonomie = CopiaAlbero(succ->alberoAutonomie);
            cancellaAlberoAutonomie(succ->alberoAutonomie);

            free(succ);
            printf("demolita\n");
            return radice;
        }
    }
}
// controlla se la stazione esiste
bool EsisteStazione(stazione T, int distance) {
    if (T == NULL)
        return false;
    if (T->distanceFromStart == distance)
        return true;
    else if (T->distanceFromStart > distance)
        return EsisteStazione(T->left, distance);
    else if (T->distanceFromStart < distance)
        return EsisteStazione(T->right, distance);
    else
        return false;

}
//controlla se esiste l'autonomia in una determinata stazione
bool EsisteAutonomia(alberoAuto radice, int autonomia) {
    if (radice == NULL)
        return false;
    if (radice->autonomia == autonomia)
        return true;
    else if (radice->autonomia > autonomia)
        return EsisteAutonomia(radice->left, autonomia);
    else if (radice->autonomia < autonomia)
        return EsisteAutonomia(radice->right, autonomia);
    else
        return false;

}
//trova la stazione successiva (successore del BST)
stazione StazioneSuccessiva(stazione radice, stazione T) {
    int distance = T->distanceFromStart;
    if (T->right != NULL) {
        T = T->right;
        while (T->left != NULL) {
            T = T->left;
        }
    } else {
        if (radice->distanceFromStart > distance) {
            T = radice;
            StazioneSuccessiva(radice->left, T);
        }
    }
    return T;
}
//trova l'auto successiva (successore del BST)
alberoAuto AutoSuccessiva(alberoAuto radice, alberoAuto T) {
    int autonomia = T->autonomia;
    if (T->right != NULL) {
        T = T->right;
        while (T->left != NULL) {
            T = T->left;
        }
    } else {
        if (radice->autonomia > autonomia) {
            T = radice;
            AutoSuccessiva(radice->left, T);
        }
    }
    return T;
}
//aggiunge auto al BST
void AggiungiAuto(stazione radice, int distance, int autonomia) {
    stazione T = CercaStazione(radice, distance);
    if (radice == NULL || T==NULL || autonomia<0){
        printf("non aggiunta\n");
        return;
    }
    else {
        if (T->numAuto < NUMCARMAX) {
            alberoAuto curr = CercaAutonomia(T->alberoAutonomie,autonomia);
            alberoAuto temp = T->alberoAutonomie;
            T->numAuto++;
            if(curr== NULL){
                alberoAuto nuovoNodo = malloc(sizeof (struct AlberoAutonomia));
                nuovoNodo->autonomia = autonomia;
                nuovoNodo->numAutoUgualeAutonomia = 1;
                nuovoNodo->right = NULL;
                nuovoNodo->left = NULL;
                if(temp==NULL){
                    T->alberoAutonomie = nuovoNodo;
                }else{
                    while(temp!=NULL){
                        if(temp->left== NULL && temp->autonomia>nuovoNodo->autonomia){
                            temp->left = nuovoNodo;
                            break;
                        }else if(temp->right==NULL && temp->autonomia< nuovoNodo->autonomia){
                            temp->right = nuovoNodo;
                            break;
                        }else if(temp->autonomia>nuovoNodo->autonomia){
                            temp = temp->left;
                        }else if(temp->autonomia< nuovoNodo->autonomia){
                            temp = temp->right;
                        }
                    }
                }
            }else{
                //se il nodo già esiste, aumento il contatore
                curr->numAutoUgualeAutonomia++;
            }
            printf("aggiunta\n");
        }
        else {
            printf("non aggiunta\n");
        }

    }

}
//rimuovo il nodo autonomia se presente solo una macchina
void RottamaAuto(stazione radice, int distance, int autonomia) {
    if (radice == NULL){
        printf("non rottamata\n");
        return;
    }
    stazione T = CercaStazione(radice, distance);
    if (T == NULL || T->alberoAutonomie == NULL) {
        printf("non rottamata\n");
    } else {
            T->alberoAutonomie = DemolisciAuto(T->alberoAutonomie, autonomia);
            T->numAuto--;
    }
}
//quattro casi di rimozione auto: il nodo contiene più di un'auto con uguale autonomia decremento il contatore,
// il nodo è una foglia, il nodo ha un solo figlio, il nodo ha entrambi i figli
alberoAuto DemolisciAuto(alberoAuto radice, int autonomia){
    if(radice==NULL)
        return NULL;
    if(!EsisteAutonomia(radice,autonomia)){
        printf("non rottamata\n");
        return radice;
    }else {
        if (radice->autonomia > autonomia) {
            radice->left = DemolisciAuto(radice->left, autonomia);
            return radice;
        } else if (radice->autonomia < autonomia) {
            radice->right = DemolisciAuto(radice->right, autonomia);
            return radice;
        }
        if (radice->numAutoUgualeAutonomia > 1) {
            radice->numAutoUgualeAutonomia--;
            printf("rottamata\n");
            return radice;
        } else {
            if (radice->left == NULL) {
                alberoAuto temp = radice->right;
                free(radice);
                printf("rottamata\n");
                return temp;
            } else if (radice->right == NULL) {
                alberoAuto temp = radice->left;
                free(radice);
                printf("rottamata\n");
                return temp;
            } else {
                alberoAuto prev = radice;

                alberoAuto succ = radice->right;
                while (succ->left != NULL) {
                    prev = succ;
                    succ = succ->left;
                }
                if (prev != radice)
                    prev->left = succ->right;
                else
                    prev->right = succ->right;

                radice->autonomia = succ->autonomia;
                radice->numAutoUgualeAutonomia = succ->numAutoUgualeAutonomia;

                free(succ);
                printf("rottamata\n");
                return radice;
            }
        }
    }
}


stazione StazionePrecedente(stazione radice,int distance) { //ritorna stazione precedente come nodo dell'albero, non come distanza da 0
    if (radice == NULL || (radice->left== NULL && radice->right==NULL)) {
        return NULL;
    }
    if ( radice->right!= NULL && radice->right->distanceFromStart == distance)
        return radice;
    if(radice->left!=NULL && radice->left->distanceFromStart == distance)
        return radice;
    if (radice->right != NULL && radice->distanceFromStart<distance)
        return StazionePrecedente(radice->right, distance);
    if (radice->left != NULL && radice->distanceFromStart>distance)
       return StazionePrecedente(radice->left, distance);
    return NULL;


}
alberoAuto AutoPrecedenteAlbero(alberoAuto radice,int autonomia){//ritorna auto precedente come nodo dell'albero
    if (radice == NULL || (radice->left== NULL && radice->right==NULL)) {
        return NULL;
    }
    if (radice->right!=NULL && radice->right->autonomia == autonomia)
        return radice;
    if (radice->left!=NULL && radice->left->autonomia == autonomia)
        return radice;
    if (radice->right != NULL)
        return AutoPrecedenteAlbero(radice->right, autonomia);
    if (radice->left != NULL)
        return AutoPrecedenteAlbero(radice->left, autonomia);
    return NULL;
}


percorsoInOrder PianificaPercorsoAndata(stazione radice, int partenza, int arrivo, percorsoInOrder list) {
    if (radice == NULL)
        return NULL;

    if (radice->distanceFromStart >= partenza && radice->distanceFromStart <= arrivo) {
        if (radice->left != NULL)
            list = PianificaPercorsoAndata(radice->left, partenza, arrivo, list);
        int max = maxAlbero(radice->alberoAutonomie);
        list = InserisciInCoda(list, radice->distanceFromStart,max);
        if (radice->right != NULL)
            list = PianificaPercorsoAndata(radice->right, partenza, arrivo, list);
    } else {
        if (radice->left != NULL && radice->distanceFromStart > arrivo)
            list = PianificaPercorsoAndata(radice->left, partenza, arrivo, list);
        else if (radice->right != NULL && radice->distanceFromStart < partenza)
            list = PianificaPercorsoAndata(radice->right, partenza, arrivo, list);
    }
    return list;
}

percorsoInOrder PianificaPercorsoRitorno(stazione radice, int partenza, int arrivo, percorsoInOrder list) {
    if (radice == NULL)
        return NULL;

    if (radice->distanceFromStart <= partenza && radice->distanceFromStart >= arrivo) {
        if (radice->right != NULL)
            list = PianificaPercorsoRitorno(radice->right, partenza, arrivo, list);
        int max = maxAlbero(radice->alberoAutonomie);
        list = InserisciInCoda(list, radice->distanceFromStart,max);

        if (radice->left != NULL)
            list = PianificaPercorsoRitorno(radice->left, partenza, arrivo, list);
    } else {
        if (radice->left != NULL && radice->distanceFromStart > partenza)
            list = PianificaPercorsoRitorno(radice->left, partenza, arrivo, list);
        else if (radice->right != NULL && radice->distanceFromStart < arrivo)
            list = PianificaPercorsoRitorno(radice->right, partenza, arrivo, list);
    }
    return list;

}


int maxAlbero(alberoAuto radice) {
    if(radice==NULL)
        return 0;
  while(radice->right!=NULL){
      radice = radice->right;
  }
  return radice->autonomia;
}


percorsoInOrder InserisciInCoda(percorsoInOrder lista, int distanza, int max) {
    if (lista == NULL) {
        lista = malloc(sizeof(struct PercorsoInOrder));
        lista->distancefromstart = distanza;
        lista->maxAutonomia = max;
        lista->next = NULL;
        return lista;
    } else {
        percorsoInOrder l = lista;
        while (l->next != NULL) {
            l = l->next;
        }
        percorsoInOrder temp =  malloc(sizeof(struct PercorsoInOrder));
        temp->distancefromstart = distanza;
        temp->maxAutonomia =max;
        temp->next = NULL;
        l->next = temp;
        return lista;
    }

}

void cancellaAlbero(stazione radice) {
    if (radice == NULL)
        return;
    cancellaAlberoAutonomie(radice->alberoAutonomie);
    cancellaAlbero(radice->right);
    cancellaAlbero(radice->left);
    free(radice);
    radice = NULL;
}
void cancellaAlberoAutonomie(alberoAuto radice) {
    if (radice == NULL)
        return;
    cancellaAlberoAutonomie(radice->right);
    cancellaAlberoAutonomie(radice->left);
    free(radice);
    radice = NULL;
}
// visita in order dell'albero, considerando i tre casi: partenza e arrivo sono uguali, stampo il nodo;
// partenza minore di arrivo, pianifico percorso andata; partenza maggiore di arrivo, pianifico percorso ritorno
void PianificaPercorso(stazione radice, int partenza, int arrivo) {
    percorsoInOrder lista = NULL;
    if (radice == NULL)
        return;
    // creo la lista con tutti i nodi presenti nell'albero, che vanno dal nodo di partenza al nodo di arrivo, con una visita in order
    if (partenza < arrivo) {
        lista = PianificaPercorsoRitorno(radice, arrivo, partenza, lista);
    } else if (partenza > arrivo) {
        lista = PianificaPercorsoRitorno(radice, partenza,arrivo, lista);
    } else {//partenza =arrivo
        printf("%d", partenza);
    }
    if (lista == NULL)
        return;
    percorsoInOrder temp = lista;
    if (temp->distancefromstart != arrivo && temp->distancefromstart!=partenza) {
        printf("nessun percorso\n");
        CancellaLista(lista);
        return;
    }
    if (temp->distancefromstart == arrivo && temp->next->distancefromstart == partenza) {
        if (temp->next->maxAutonomia < temp->distancefromstart - temp->next->distancefromstart) {
            printf("nessun percorso\n");
        }else{
            printf("%d %d\n", temp->next->distancefromstart,temp->distancefromstart);
        }
        CancellaLista(lista);
        return;
    }
    if(temp->distancefromstart== partenza && temp->next->distancefromstart==arrivo){
        if(temp->maxAutonomia< temp->distancefromstart - temp->next->distancefromstart){
            printf("nessun percorso\n");
        }else{
            printf("%d %d\n", temp->distancefromstart,temp->next->distancefromstart);
        }
        CancellaLista(lista);
        return;
    }

    int miglioreDistanza = 0;
    int miglioreAutoDistanza = 0;
    int miglioreVecchiaTappaSuccessiva =temp->distancefromstart;
    percorsoInOrder listaD = NULL;
    percorsoInOrder prev = temp;
    while (temp->next != NULL) {
        if (partenza < arrivo && temp->distancefromstart!=arrivo && temp->next->maxAutonomia < temp->distancefromstart - temp->next->distancefromstart) {
            percorsoInOrder succ = temp->next;
            while(succ!=NULL){
                if(succ->maxAutonomia<temp->distancefromstart- succ->distancefromstart){
                    succ = succ->next;
                }else{
                    break;
                }
            }
            if(succ == NULL){
                printf("nessun percorso\n");
                CancellaLista(lista);
                CancellaLista(listaD);
                return;
            }else{
                temp = succ;
            }
        } else if (partenza > arrivo && temp->maxAutonomia < temp->distancefromstart - temp->next->distancefromstart) {

            printf("nessun percorso\n");
            CancellaLista(lista);
            return;
        } else {
            if (listaD == NULL) {
                listaD = InserisciInTestaPIO(listaD, temp->distancefromstart, temp->maxAutonomia);
                if(partenza>arrivo){
                    percorsoInOrder aux = temp->next;
                    percorsoInOrder succ = temp->next;

                    while(aux!= NULL && temp->maxAutonomia>= temp->distancefromstart - aux->distancefromstart){
                        int miglioreTappaSuccessiva = TappaSuccessiva(aux);
                        if(miglioreTappaSuccessiva<= miglioreVecchiaTappaSuccessiva){
                            miglioreDistanza = aux->distancefromstart;
                            miglioreAutoDistanza = aux->maxAutonomia;
                            miglioreVecchiaTappaSuccessiva = miglioreTappaSuccessiva;
                        }
                        aux = aux->next;
                    }

                    if(prev!=temp){
                        while(succ!=NULL && prev->maxAutonomia>= prev->distancefromstart - succ->distancefromstart){
                                int best = TappaSuccessiva(succ);
                                if (listaD != NULL && best <= miglioreDistanza && succ->distancefromstart < listaD->distancefromstart) {
                                    percorsoInOrder destroy = listaD;
                                    listaD = listaD->next;
                                    free(destroy);
                                    destroy = NULL;
                                    listaD = InserisciInTestaPIO(listaD, succ->distancefromstart, succ->maxAutonomia);
                                }
                            succ = succ->next;
                        }
                    }
                   listaD = InserisciInTestaPIO(listaD, miglioreDistanza, miglioreAutoDistanza);
                    if(miglioreDistanza== arrivo){
                        temp->distancefromstart= arrivo;
                        break;
                    }
                    prev = temp;
                    if(temp!=aux){
                        while(temp !=NULL && temp->distancefromstart!=miglioreDistanza){
                            temp = temp->next;
                        }
                    }else{
                        temp = temp->next;
                    }
                }else{
                    if (partenza < arrivo  && temp->next->maxAutonomia < listaD->distancefromstart - temp->next->distancefromstart) {
                        percorsoInOrder succ = temp->next;
                        while(succ!=NULL){
                            if(succ->maxAutonomia<listaD->distancefromstart- succ->distancefromstart){
                                succ = succ->next;
                            }else{
                                break;
                            }
                        }
                        if(succ == NULL){
                            printf("nessun percorso\n");
                            CancellaLista(lista);
                            CancellaLista(listaD);
                            return;
                        }else{
                            temp = succ;
                        }
                    }

                }
            } else {
                if (partenza < arrivo) {
                    while (listaD->next!=NULL && temp->maxAutonomia >= listaD->next->distancefromstart - temp->distancefromstart) {
                        percorsoInOrder destroy = listaD;
                        listaD = listaD->next;
                        free(destroy);
                        destroy = NULL;
                    }
                    listaD = InserisciInTestaPIO(listaD, temp->distancefromstart, temp->maxAutonomia);
                    temp = temp->next;
                } else {

                        percorsoInOrder aux = temp->next;
                        percorsoInOrder succ = temp->next;

                        while(aux!= NULL && temp->maxAutonomia>= temp->distancefromstart - aux->distancefromstart){
                            int miglioreTappaSuccessiva = TappaSuccessiva(aux);

                            if(miglioreTappaSuccessiva<= miglioreVecchiaTappaSuccessiva){
                                miglioreDistanza = aux->distancefromstart;
                                miglioreAutoDistanza = aux->maxAutonomia;
                                miglioreVecchiaTappaSuccessiva = miglioreTappaSuccessiva;

                            }
                            aux = aux->next;
                        }

                        if(prev!=temp){
                            while(succ!=NULL && prev->maxAutonomia>= prev->distancefromstart - succ->distancefromstart){
                                    int best = TappaSuccessiva(succ);

                                    if (listaD != NULL && best <= miglioreDistanza &&  succ->distancefromstart < listaD->distancefromstart) {
                                        percorsoInOrder destroy = listaD;
                                        listaD = listaD->next;
                                        free(destroy);
                                        destroy = NULL;
                                        if(prev->distancefromstart!=listaD->distancefromstart && listaD->maxAutonomia< listaD->distancefromstart - succ->distancefromstart){
                                            if(prev->maxAutonomia>= prev->distancefromstart-succ->distancefromstart){
                                                percorsoInOrder destroy2 = listaD;
                                                listaD = listaD->next;
                                                free(destroy2);
                                                destroy2 = NULL;
                                                listaD = InserisciInTestaPIO(listaD,prev->distancefromstart,prev->maxAutonomia);
                                            }
                                        }

                                        listaD = InserisciInTestaPIO(listaD, succ->distancefromstart,
                                                                     succ->maxAutonomia);
                                    }
                                succ = succ->next;
                            }
                        }

                        listaD = InserisciInTestaPIO(listaD, miglioreDistanza, miglioreAutoDistanza);
                        if(miglioreDistanza== arrivo){
                            temp->distancefromstart= arrivo;
                            break;
                        }
                        prev = temp;
                        if(temp!=aux){
                            while(temp !=NULL && temp->distancefromstart!=miglioreDistanza){
                                temp = temp->next;
                            }
                        }else{
                            temp = temp->next;
                        }
                }
            }
        }
    }
    if(partenza<arrivo){
        if(listaD->distancefromstart!=partenza){
            while (listaD->next != NULL && temp->maxAutonomia >= listaD->next->distancefromstart - temp->distancefromstart) {
                percorsoInOrder destroy = listaD;
                listaD = listaD->next;
                free(destroy);
                destroy = NULL;
            }
            printf("%d ", partenza);
        }
        percorsoInOrder distruggi = listaD;
        Stampa(listaD);
        printf("\n");
        CancellaLista(lista);
        CancellaLista(distruggi);

    }else{
        percorsoInOrder distruggi = listaD;
        StampaAlContrario(listaD, listaD->distancefromstart);
        printf("\n");
        CancellaLista(lista);
        CancellaLista(distruggi);
    }

}





void CancellaLista(percorsoInOrder lista){
    while (lista != NULL) {
        percorsoInOrder temp = lista;
        lista = lista->next;
        free(temp);
        temp=NULL;
    }
}

percorsoInOrder InserisciInTestaPIO(percorsoInOrder lista, int distance, int maxAuto){
    percorsoInOrder lista1 = malloc(sizeof (struct PercorsoInOrder));
    lista1->maxAutonomia = maxAuto;
    lista1->distancefromstart =distance;
    lista1->next = lista;
    lista = lista1;
    return lista;
}

void StampaAlContrario(percorsoInOrder lista, int inizio){
    if(lista== NULL)
        return;
    StampaAlContrario(lista->next, inizio);
    printf("%d", lista->distancefromstart);
    if(lista->distancefromstart!=inizio)
        printf(" ");


}
int TappaSuccessiva(percorsoInOrder lista){

    percorsoInOrder curr = lista;
    while(curr->next!=NULL && lista->maxAutonomia>=lista->distancefromstart - curr->next->distancefromstart){
        curr = curr->next;
    }
    return curr->distancefromstart;
}
void Stampa(percorsoInOrder lista){
    percorsoInOrder curr = lista;
    while (curr->next != NULL) {
    printf("%d ", curr->distancefromstart);
    curr = curr->next;
    }
    printf("%d", curr->distancefromstart);
}