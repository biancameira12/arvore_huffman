#include <bits/stdc++.h>

using namespace std;

//////variaveis globais
map<char, int> tab_freq;

//no da arvore de Huffman
struct no{
    int id;
    int freq;
    char chave;
    struct no *esq;
    struct no *dir;
    no(int id, char chave, int freq, struct no *esq=NULL, struct no *dir=NULL):
        freq(freq), chave(chave), dir(dir),esq(esq), id(id){}
    ~no(){
        delete esq, delete dir;
    }
    class Compare {
    public:
        bool operator()(no *a, no *b) {
            return a->freq > b->freq;
        }
    };
};

////tabela de frequencia gerada com o arquivo 'input.txt'
void tabela_frequencia(){
    FILE *fp;
    //lendo o arquivo
    fp=fopen("input.txt", "r");
    if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }
   char c;
   //adicionando elementos na tabela
   fscanf(fp, "%c", &c);
   while(!feof(fp)){
        tab_freq[c]++;
        fscanf(fp, "%c", &c);
   }
   fclose(fp);
   /*fp=fopen("tabela.txt", "w");
    if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }
   for(map<char, int> ::iterator it = tab_freq.begin(); it != tab_freq.end(); ++it){
        fprintf(fp, "%c, %d\n", it->first, it->second);
   }
   fclose(fp);*/
}

//construcao arvore de huffman
no *contruir_arvore(map<char, int> tab_freq){
    // contruir uma pilha
    // construir os nos das folhas primeiramente
    priority_queue<no *, vector<no *>, no::Compare > alph_heap;
    int id=1;
    for (map<char, int> ::iterator it = tab_freq.begin(); it != tab_freq.end(); ++it) {
        no *folha = new no(id, it->first, it->second);
        id++;
        alph_heap.push(folha);
        id++;
    }
     // HuffmanTree algorithm: Merge two lowest weight leaf nodes until
    // only one node is left (root).
    no *root = NULL;
    while (alph_heap.size() > 1) {
        no *l, *r;
        l = alph_heap.top();
        alph_heap.pop();
        r = alph_heap.top();
        alph_heap.pop();
        id++;
        root = new no(id, 0, l->freq + r->freq, l, r);
        id++;
        alph_heap.push(root);
    }

    return root;

}

typedef vector<bool> code_t;
typedef map<char, code_t> codetable;

//construcao da tabela de codificacao
map<char, code_t> const_tabela_codificacao(no *htree) {
    codetable lookup;
    deque< pair<no *, code_t> > q;

    q.push_back(make_pair(htree, code_t()));
    while (!q.empty()) {
        no *node, *lc, *rc;
        code_t code;
        node = q.front().first;
        code = q.front().second;
        q.pop_front();
        lc = node->esq;
        rc = node->dir;
        if (lc) {
            // HuffmanTree is always full (either no children or two children)
            // Left child is appended a 0 and right child a 1.
            code_t code_cp(code);
            q.push_back(make_pair(lc, (code.push_back(0), code)));
            q.push_back(make_pair(rc, (code_cp.push_back(1), code_cp)));
        } else {
            // Leaf node: contains the character
            lookup.insert(make_pair(node->chave, code));

        }
    }

    return lookup;
}

//fprinta pre-ordem
void fprint_pre_ordem(no *harvore, FILE *fp){
    no *pre=harvore;
    fprintf(fp, "%d %d\n", (int) pre->chave, pre->id);
    if(pre -> esq != NULL){         //arvore de huffman ou tem 0 ou 2 filhos
            fprint_pre_ordem(pre -> esq, fp);
            fprint_pre_ordem(pre -> dir, fp);
    }
}

//fprinta ordem simetrica
void fprint_sim_ordem(no *harvore, FILE *fp){
    no *sim=harvore;
    if(sim -> esq != NULL) fprint_sim_ordem(sim -> esq, fp);
    fprintf(fp, "%d %d\n", (int) sim->chave, sim->id);
    if(sim -> dir != NULL) fprint_sim_ordem(sim -> dir, fp);
}

//faz arquivo 'arvhuf.txt' e chame funcao fprint_pre_ordem e fprint_sim_ordem
void fprint_arvore(no *harvore, FILE *fp){
    fprint_pre_ordem(harvore, fp);
    fprintf(fp, "0 0\n");
    fprint_sim_ordem(harvore, fp);
}

//codificando texto
void codificacao(map<char, code_t> cod_huf){
    FILE *entrada, *saida;
    entrada=fopen("input.txt", "r");
    if (entrada == NULL)
    {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
    }
    saida=fopen("texto.hfm", "w");
    if (saida == NULL)
    {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
    }
    char c;
    fscanf(entrada, "%c", &c);
    while(!feof(entrada)){
        for(int i=0; i<cod_huf[c].size(); i++){
                if(cod_huf[c][i])fprintf(saida, "1");
                else fprintf(saida, "0");
        }
        fscanf(entrada, "%c", &c);
    }
    fclose(entrada);
    fclose(saida);
}


int main(){
    tabela_frequencia();
    no *harvore=contruir_arvore(tab_freq);
    map<char, code_t> cod_huf=const_tabela_codificacao(harvore);
    codificacao(cod_huf);
    FILE *fp;
    fp=fopen("arvhuf.txt", "w");
    if (fp == NULL)
    {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
    }
    fprint_arvore(harvore, fp);
    fclose(fp);
}

