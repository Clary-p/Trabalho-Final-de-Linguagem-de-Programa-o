#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif


typedef struct Compromisso {
    int id;
    char titulo[50];
    char data[11]; // DD/MM/AAAA
    char hora[6];  // HH:MM
    char descricao[100];
    struct Compromisso *prox;
} Compromisso;

// Função para desenhar o gatinho 
void desenhogatinho() {

    printf("        ^   ^    \ \n");
    printf("       <=OwO=>   /  \n");
    printf("     '-.v..v.-'_/ \n");
    printf("\n");
    printf("   < Agenda Fofa > \n\n");
}

//  Inserir compromisso 
Compromisso* inserir(Compromisso *lista) {
    Compromisso *novo = (Compromisso*) malloc(sizeof(Compromisso));
    if (!novo) {
        printf("Erro de memória!\n");
        return lista;
    }
    printf("Título: ");
    getchar(); // limpa buffer
    fgets(novo->titulo, 50, stdin);
    novo->titulo[strcspn(novo->titulo, "\n")] = 0;

    printf("Data (DD/MM/AAAA): ");
    scanf("%10s", novo->data);
    printf("Hora (HH:MM): ");
    scanf("%5s", novo->hora);
    printf("Descrição: ");
    getchar();
    fgets(novo->descricao, 100, stdin);
    novo->descricao[strcspn(novo->descricao, "\n")] = 0;

    novo->id = (lista == NULL) ? 1 : lista->id + 1;
    novo->prox = lista;
    printf("Compromisso adicionado!\n");
    return novo;
}

// Listar compromissos
void listar(Compromisso *lista) {
    if (!lista) {
        printf("Nenhum compromisso salvo.\n");
        return;
    }
    printf("\n Lista de compromissos:\n");
    for (Compromisso *p = lista; p != NULL; p = p->prox) {
        printf("ID: %d\nTítulo: %s\nData: %s\nHora: %s\nDescrição: %s\n\n",
               p->id, p->titulo, p->data, p->hora, p->descricao);
    }
}

// Buscar por data
void buscar(Compromisso *lista) {
    if (!lista) {
        printf("Nenhum compromisso salvo.\n");
        return;
    }
    char data[11];
    printf("Digite a data (DD/MM/AAAA): ");
    scanf("%10s", data);

    int achou = 0;
    for (Compromisso *p = lista; p != NULL; p = p->prox) {
        if (strcmp(p->data, data) == 0) {
            printf("\n %s — %s às %s\n", p->titulo, p->data, p->hora);
            printf("   %s\n", p->descricao);
            achou = 1;
        }
    }
    if (!achou)
        printf("Nenhum compromisso encontrado nessa data.\n");
}

// Remover compromisso
Compromisso* remover(Compromisso *lista) {
    if (!lista) {
        printf("Nenhum compromisso para remover.\n");
        return NULL;
    }
    int id;
    printf("ID do compromisso a remover: ");
    scanf("%d", &id);

    Compromisso *ant = NULL, *p = lista;
    while (p && p->id != id) {
        ant = p;
        p = p->prox;
    }

    if (!p) {
        printf("ID não encontrado.\n");
        return lista;
    }

    if (ant == NULL) lista = p->prox;
    else ant->prox = p->prox;

    free(p);
    printf("️Compromisso removido!\n");
    return lista;
}

// Salvar em arquivo 
void salvar(Compromisso *lista) {
    FILE *arq = fopen("agenda.txt", "w");
    if (!arq) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }
    for (Compromisso *p = lista; p != NULL; p = p->prox) {
        fprintf(arq, "%d;%s;%s;%s;%s\n", p->id, p->titulo, p->data, p->hora, p->descricao);
    }
    fclose(arq);
    printf("Dados salvos em 'agenda.txt'!\n");
}

// Carregar do arquivo 
Compromisso* carregar() {
    FILE *arq = fopen("agenda.txt", "r");
    if (!arq) return NULL;

    Compromisso *lista = NULL;
    while (!feof(arq)) {
        Compromisso *novo = (Compromisso*) malloc(sizeof(Compromisso));
        if (fscanf(arq, "%d;%49[^;];%10[^;];%5[^;];%99[^\n]\n",
                   &novo->id, novo->titulo, novo->data, novo->hora, novo->descricao) == 5) {
            novo->prox = lista;
            lista = novo;
        } else {
            free(novo);
            break;
        }
    }
    fclose(arq);
    return lista;
}

// Programa principal 
int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(65001); // pesquisei isso para conseguir colocar acentos, UTF-8 no output
    #endif

    Compromisso *lista = carregar();
    int opcao;

    do {
        system("cls || clear");
        desenhogatinho();
        printf("1. Adicionar compromisso\n");
        printf("2. Listar compromissos\n");
        printf("3. Buscar por data\n");
        printf("4. Remover compromisso\n");
        printf("5. Salvar e sair\n");
        printf("\nEscolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: lista = inserir(lista); break;
            case 2: listar(lista); break;
            case 3: buscar(lista); break;
            case 4: lista = remover(lista); break;
            case 5: salvar(lista); break;
            default: printf("Opção inválida!\n");
        }
        if (opcao != 5) {
            printf("\nPressione ENTER para continuar...");
            getchar(); getchar();
        }
    } while (opcao != 5);

    while (lista) {
        Compromisso *tmp = lista;
        lista = lista->prox;
        free(tmp);
    }

    printf("\n Até mais!\n");
    return 0;
}

