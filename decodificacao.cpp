#include <bits/stdc++.h>
#include <iostream>
#include <stdio.h>

using namespace std;

//variaveis globais
vector< pair<int,char> > pre, sim;

struct no{
    int id;
    char chave;
    struct no *esq;
    struct no *dir;
};
struct no *raiz;

void gets_ordem(){
    FILE *fp;
    //lendo o arquivo
    fp=fopen("arvhuf.txt", "r");
    if (fp == NULL)
    {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
    }
    int aux1, aux2, contador=0;
    fscanf(fp, "%d %d\n", &aux2, &aux1);
    vector< pair<int,char> > aux;
    while(!feof(fp)) {
        aux.push_back(make_pair(aux1, (char) aux2));
        fscanf(fp, "%d %d\n", &aux2, &aux1);

        contador++;
    }
    aux.push_back(make_pair(aux1, (char) aux2));
    fclose(fp);
    for(pair<char, int> p : aux){
            if(contador>aux.size()/2){
                pre.push_back(make_pair(p.first,p.second));
                contador--;

            }
            else if(contador==aux.size()/2)contador--;
            else sim.push_back(make_pair(p.first,p.second));
    }

}

void recupera_arvore(int ip, int fp, int is, int fs, struct no **raiz){
    int nre=0;

    if (ip == fp){
        *raiz = (no *)malloc (sizeof(struct no));
        (*raiz)->chave = pre[ip].second;
        (*raiz)->id = pre[ip].first;
        (*raiz)->dir =NULL;
        (*raiz)->esq =NULL;
    }
    else{
        recupera_arvore (ip,ip,0,0,raiz);
        while (sim[is+nre].first != pre[ip].first) {
                nre++;
        }
        if (nre != 0) {
                recupera_arvore (ip+1,ip+nre,is,is+nre-1,&((*raiz)->esq));
        }
        if (is+nre < fs) {
                recupera_arvore (ip+nre+1,fp,is+nre+1,fs,&((*raiz)->dir));
        }
    }
}

void decode(no *raiz){
    FILE *entrada, *saida;
    //lendo o arquivo
    entrada=fopen("texto.hfm", "r");
    if (entrada == NULL){
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    saida=fopen("saida.txt", "w");
    if (saida == NULL){
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    no *procura_fim=raiz;
    char aux;
    int aux2=0;
    while(!feof(entrada)){
        if(aux2==1){
                if(aux=='1')procura_fim=raiz->dir;
                else procura_fim=raiz->esq;
                aux2=0;
        }
        fscanf(entrada, "%c", &aux);
        if(aux=='1'){
            if(procura_fim->dir!=NULL)procura_fim=procura_fim->dir;
            else {
                    fprintf(saida, "%c", procura_fim->chave);
                    aux2=1;
            }

        }
        else{
            if(procura_fim->esq!=NULL)procura_fim=procura_fim->esq;
            else {
                    fprintf(saida, "%c", procura_fim->chave);
                    aux2=1;
            }
        }
    }
    fclose(entrada);
    fclose(saida);
}


int main(int argc, const char * argv[]){
    gets_ordem();

    recupera_arvore(0, pre.size()-1, 0, sim.size()-1, &raiz);

    decode(raiz);
}
