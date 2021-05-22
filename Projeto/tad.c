#include "tad.h"

struct remedio {
    char nome_med[max+1];
    char preco[10+1];
    int quant_estoque;
    int exigencia_receita; // 0 para não, 1 para sim
    med* next;
    char tipoMed[max+1];
};

struct tipo_remedio {
    char tip[max+1];
    tipo* next;
    med* inicio_lista_med;
    int numElem_Meds;
};

struct lista{
    tipo* inicio_listaTipo;
    int numElem_Tipos;
};

// FUNÇÕES LOCAIS - ENCAPSULADAS ~ PRIVATE
lista* criaLista();
void criaEstoquePadraoTipoRemedio(lista* l);
void criaEstoquePadraoRemedio(lista* l);
void inserirTipo(lista* l, char tip[max+1]);
void inserirRemedio(lista* l, char nome_med[max+1], char preco[10+1], int quant_estoque, int exigencia_receita, char tipoMed[max+1]);
tipo* ultimoElemTipo(lista* l);
tipo* NoTipoEspecifico(lista* l, char tipoMed[max+1]);
int tipoExiste(lista* l, char tipoMed[max+1]);
void inserirMed_na_lista(tipo* tipo_de_remedio, med* medicacao);
med* ultimoElemMed(tipo* tipo_de_remedio, med* medicacao);
void liberaLista(lista* l);
void menuCascata(lista* l, int contMenuPrincipal);
void submenu(lista* l, tipo* tipo_de_remedio, int contMenuPrincipal);
void menuInfo(lista* l, med* medicacao, int contMenuPrincipal);

void menuPrincipal(int contMenuPrincipal){
    contMenuPrincipal++;
    system("clear");
    printf("=========================================================================================\n");
    printf("\n\t\tSeja bem vindo ao Sistema de Controle de Estoque!\n\n");
    printf("Por favor, selecione a opção desejada:\n\n");
    printf("1 - Buscar Medicamento.\n");
    printf("2 - Pesquisar Medicamento.\n");
    printf("3 - Sair.\n");
    printf("=========================================================================================\n");

    int input;
    scanf("%d", &input);

    lista* l = criaLista();

    if(input == 1){
        criaEstoquePadraoTipoRemedio(l);
        criaEstoquePadraoRemedio(l);
        menuCascata(l, contMenuPrincipal);
    }
    else if(input == 2){
        // chama a função pesquisar produto via string de busca
    }
    else if(input == 3){
        liberaLista(l);
        printf("\n\tSISTEMA ENCERRADO!\n");
        return;
    }
}

lista* criaLista(){
    lista* l = (lista*) malloc(sizeof(lista));
    l->inicio_listaTipo = NULL;
    l->numElem_Tipos = 0;
    
    return l;
}

// Função Local para criar nosso "Estoque Atual" ou "Estoque existente"
void criaEstoquePadraoTipoRemedio(lista* l){
    inserirTipo(l, "analgesico");
    inserirTipo(l, "anfetamina");
    inserirTipo(l, "antiacido");
}

// Função Local para criar nosso "Estoque Atual" ou "Estoque existente"
void criaEstoquePadraoRemedio(lista* l){
    inserirRemedio(l, "Paracetamol", "12", 80, 0, "analgesico");
    inserirRemedio(l, "Dipirona", "4,50", 15, 0, "analgesico");

    inserirRemedio(l, "Venvanse", "257", 94, 1, "anfetamina");
    inserirRemedio(l, "Ritalina", "40", 50, 1, "anfetamina");

    inserirRemedio(l, "Engov", "5,35", 320, 0, "antiacido");
}

// Função local para inserir um nó na lista de Tipos de medicamentos
void inserirTipo(lista* l, char tip[max+1]){
    tipo* novo_tipo = (tipo*) malloc(sizeof(tipo));
    strcpy(novo_tipo->tip, tip);
    novo_tipo->inicio_lista_med = NULL;
    novo_tipo->numElem_Meds = 0;
    novo_tipo->next = NULL;
    
    if(l->numElem_Tipos == 0){
        l->inicio_listaTipo = novo_tipo;
        l->numElem_Tipos++;
    }
    else{
        tipo* ultimo = ultimoElemTipo(l);
        ultimo->next = novo_tipo;
        l->numElem_Tipos++;
    }
}

// Função local para retornar último elemento da lista de tipos de remédio
tipo* ultimoElemTipo(lista* l){
    tipo* tempTipo = l->inicio_listaTipo;
    while(tempTipo->next != NULL){
        tempTipo = tempTipo->next;
    }
    return tempTipo;
}

// Função local para inserir um Remedio em sua lista especifica de acordo com seu Tipo
void inserirRemedio(lista* l, char nome_med[max+1], char preco[10+1], int quant_estoque, int exigencia_receita, char tipoMed[max+1]){
    med* novo_med = (med*) malloc(sizeof(med));
    strcpy(novo_med->nome_med, nome_med);
    strcpy(novo_med->preco, preco);
    novo_med->quant_estoque = quant_estoque;
    novo_med->exigencia_receita = exigencia_receita;
    strcpy(novo_med->tipoMed, tipoMed);

    int check = tipoExiste(l, tipoMed);
    if(check == 0){
        inserirTipo(l, tipoMed);
    }

    tipo* tempTipo = NoTipoEspecifico(l, tipoMed);
    inserirMed_na_lista(tempTipo, novo_med);
    tempTipo->numElem_Meds++;
}

// Função local auxiliar da função inserirRemedio usada para inserir remedio na sua respectiva lista
void inserirMed_na_lista(tipo* tipo_de_remedio, med* medicacao){
    if(tipo_de_remedio->inicio_lista_med == NULL){
        tipo_de_remedio->inicio_lista_med = medicacao;
    }
    else{
        med* tempMed = ultimoElemMed(tipo_de_remedio, medicacao);
        tempMed->next = medicacao;
    }
}

// Função local auxiliar da função inserirMed_na_lista
med* ultimoElemMed(tipo* tipo_de_remedio, med* medicacao){
    med* tempMed = tipo_de_remedio->inicio_lista_med;
    while(tempMed->next != NULL){
        tempMed = tempMed->next;
    }
    return tempMed;
}

// Função local para retornar 1 se o tipo especificado de medicamento existe e 0 caso não exista no sistema.
int tipoExiste(lista* l, char tipoMed[max+1]){
    tipo* tempTipo = l->inicio_listaTipo;
    for(int i = 0;strcmp(tempTipo->tip, tipoMed) != 0 && i < l->numElem_Tipos;i++){
        tempTipo = tempTipo->next;
    }

    if(strcmp(tempTipo->tip, tipoMed) == 0)
        return 1;
    else
        return 0;
}

// Função local para retornar o nó do tipo de medicamento especificado
tipo* NoTipoEspecifico(lista* l, char tipoMed[max+1]){
    tipo* tempTipo = l->inicio_listaTipo;
    for(int i = 0;strcmp(tempTipo->tip, tipoMed) != 0 && i < l->numElem_Tipos;i++){
        tempTipo = tempTipo->next;
    }
    if(strcmp(tempTipo->tip, tipoMed) == 0){
        return tempTipo;
    }
    else{
        printf("Tipo de medicamento especificado não existe em estoque!\n");
        return NULL;
    }
}

void menuCascata(lista* l, int contMenuPrincipal){
    system("clear");
    tipo* tempTipo = l->inicio_listaTipo;
    printf("Selecione o tipo de medicação desejado:\n");
    int i;
    for(i = 1;tempTipo != NULL;i++){
        printf("%d - %s\n", i, tempTipo->tip);
        tempTipo = tempTipo->next;
    }
    printf("0 - Voltar ao Menu Principal\n");

    int input;
    scanf("%d", &input);

    tempTipo = l->inicio_listaTipo;
    for(i = 1;tempTipo != NULL;i++){
        if(i == input){
            submenu(l, tempTipo, contMenuPrincipal);
        }
        tempTipo = tempTipo->next;
    }
}

void submenu(lista* l, tipo* tipo_de_remedio, int contMenuPrincipal){
    system("clear");
    med* tempMed = tipo_de_remedio->inicio_lista_med;
    printf("Selecione o %s desejado:\n", tipo_de_remedio->tip);
    int i;
    for(i = 1;tempMed != NULL;i++){
        printf("%d - %s\n", i, tempMed->nome_med);
        tempMed = tempMed->next;
    }
    printf("0 - Voltar ao Menu Principal\n");

    int input;
    scanf("%d", &input);

    if(input == 0){
        menuPrincipal(contMenuPrincipal);
    }
    tempMed = tipo_de_remedio->inicio_lista_med;
    for(i = 1;tempMed != NULL;i++){
        if(i == input){
            menuInfo(l, tempMed, contMenuPrincipal);
        }
        tempMed = tempMed->next;
    }
}

void menuInfo(lista* l, med* medicacao, int contMenuPrincipal){
    system("clear");
    printf("\t\tInformações e Opções do medicamento:\n\n");
    printf("Medicamento: %s\n", medicacao->nome_med);
    printf("Preço: R$ %s\n", medicacao->preco);
    printf("Quantidade em Estoque: %d\n", medicacao->quant_estoque);
    printf("Tipo de medicamento: %s\n", medicacao->tipoMed);
    if(medicacao->exigencia_receita == 1){
        printf("Exige Receita Médica? - SIM\n");
    }
    else{
        printf("Exige Receita Médica? - NÃO\n");
    }
    printf("\n");
    printf("1 - Solicitar Reposição de Estoque\n");
    printf("0 - Voltar ao Menu Principal\n");

    int input;
    scanf("%d", &input);

    if(input == 0){
        menuPrincipal(contMenuPrincipal);
    }
    if(input == 1){
        if(medicacao->quant_estoque > 10){
            printf("\nREPOSIÇÃO NÃO PERMITIDA!\n");
            printf("A resposição de estoque só é permitida caso a quantidade do medicamento em estoque seja 10 ou menos!\n");
        }
        else{
            printf("Por favor, digite quantos medicamentos deseja reestocar:\n");
            scanf("%d", &input);
            medicacao->quant_estoque = medicacao->quant_estoque + input;
            printf("MEDICAMENTO REESTOCADO!\n\nNova quantidade em estoque: %d\n", medicacao->quant_estoque);
        }
    }
}

void liberaLista(lista* l){
    if(l->inicio_listaTipo == NULL){
        return;
    }

    tipo* tempTipo = l->inicio_listaTipo;
    med* tempMed = tempTipo->inicio_lista_med;

    for(int j = 0;j < l->numElem_Tipos;j++){
        for(int i = 0;i < tempTipo->numElem_Meds;i++){
            med* apagarMed = tempMed;
            tempMed = tempMed->next;
            free(apagarMed);
        }
        tipo* apagarTipo = tempTipo;
        tempTipo = tempTipo->next;
        free(apagarTipo);
    }

    free(l);
}