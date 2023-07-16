#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <strings.h>

using namespace std;

int qtdl=0, qtdp=0, qtde=0;
int id_u=999; //id global do usuário para facilitar a lógica

typedef struct { //estrutura do livro
    int id;
    char titulo[40];
    char autor[40];
    int num_paginas;
    char genero[15];
    int qtd_total;
    int qtd_emprestada;
    int excluido;
} t_livro;


typedef struct { //estrutura dos emprestados
    int livro;
    int usuario_id;
    int devolvido;
} t_emprestimos;

typedef struct { //estrutura usuarios
    int id;
    char login[40];
    int senha;
    int excluido;
} t_usuario;


void gotoxy(int x, int y) { //função para mover cursor para xy do console
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x-1,y-1});
}
void continuar(){//função para auxiliar na visualização dos elementos presentes na tela
    int c;
    printf("\nPressione qualquer tecla para continuar...");
    c=getch();
    system("cls");
}
int abrir_arq(char *nome){ //abrir arquivo para retornar tamanho do arquivo e realizar o malloc
    char linha[130];
    int qtd_linhas=0, x=0;
    FILE *fp=fopen(nome,"a+");
    if(!fp) printf("ERRO NA ABERTURA DO ARQUIVO!");

        while(!feof(fp)){
            if(fgets(linha,sizeof(linha),fp)!=NULL){
                qtd_linhas++;
            }
        }
    return qtd_linhas+10;
    fclose(fp);
}
void login(t_usuario*ppl){ //funçaõ para login que contem cadastro
    char login[40];
    char laux[40];
    int senha;
    int id=1, aux;
    int cad=0, sc=0;
    int x=0;
    char linha[130];
    while(true){
        gotoxy(40,1);printf("LOGIN");gotoxy(35,3);printf("Caso nao tenha cadastro, digite 0 no login para se cadastrar.");gotoxy(35,4);printf("Digite 0 na senha para sair.");gotoxy(1,2);
        printf("Login: ");fflush(stdin);scanf("%[^\n]",login);fflush(stdin);

        if(strcmp(login, "0") == 0){ //função cadastro quando digita 0 no login
            system("cls");
            gotoxy(40,1);printf("CADASTRO");gotoxy(1,2);
            FILE*fp=fopen("pessoas.txt","a+");
            if(!fp) printf("ERRO NA ABERTURA DO ARQUIVO!");
            printf("Digite um nome de Usuario:");fflush(stdin);scanf("%[^\n]",login);fflush(stdin);
            while(!feof(fp)){
                if(fgets(linha,sizeof(linha),fp)!= NULL){
                    sscanf(linha,"ID:%d|Usuario:%[^|]",&aux,laux);
                    if(strcmp(login,laux)==0){
                            printf("Nome ja utilizado, redigite: ");fflush(stdin);scanf("%[^\n]",login);fflush(stdin);
                            rewind(fp);

                    }
                }
            }
            printf("Digite uma senha de ate 8 digitos: ");scanf("%d",&senha); //tamanho maximo de 8 digitos da senha e não pode ser 0
            while(senha>99999999||senha<1){
                printf("Senha Invalida, redigite");
                scanf("%d",&senha);
            }
            rewind(fp);
            while(!feof(fp)){ //consistencia para não haverem IDs de usuários repetidos, são atribuidos automaticamente
                if(fgets(linha,sizeof(linha),fp)!= NULL){
                    sscanf(linha,"ID:%d",&aux);
                    if(aux==id){
                        id++;
                    }
                }
            }
            fprintf(fp,"\nID:%d|Usuario:%s|Senha:%d|Excluido:0",id,login,senha); //salvar o novo usuario
            cad=1;
            qtdp++; //aumentar a quantidade de pessoas na memoria
            fclose(fp);
            login[40]=NULL;
            id_u=id;
            sc=1;
        }
        if(cad!=1){
            printf("Senha: ");scanf("%d",&senha); //parte da senha quando passa do login e cadastro com suas consistencias
            if(senha==0)
                exit(0);
            FILE*fp=fopen("pessoas.txt","a+");
            if(!fp) printf("ERRO NA ABERTURA DO ARQUIVO!");
            while(!feof(fp)){
                if(fgets(linha,sizeof(linha),fp)!= NULL){
                    sscanf(linha,"ID:%d|Usuario:%[^|]|Senha:%d|Excluido:%d",&ppl[x].id,ppl[x].login,&ppl[x].senha,&ppl[x].excluido);
                    if(strcmp(login, "admin")==0&&senha==999){ //login admin e senha 999 para acessar a aba de administração da biblioteca
                        id_u=0;
                        sc=1;
                    } else if(strcmp(login, ppl[x].login)==0&&senha==ppl[x].senha){
                        id_u=ppl[x].id;
                        sc=1;
                    }
                    x++;
                }
            }
            fclose(fp);

        }
        if(sc==1)
            break;
        system("cls");
        gotoxy(35,5);printf("Login e/ou Senha Incorreto(s).");
        x=0;
    }
    printf("\n\nEntrada bem sucedida!");
    continuar();

}
void mostrar_acervo(t_livro*str){ //funçaõ que mostra o acervo de livros adicionados no arquivo
    char linha[130];
    int x=0, y=3;
    FILE *fp=fopen("livros.txt","a+");
    if(!fp) printf("ERRO NA ABERTURA DO ARQUIVO!");

        printf("%c  ID  %c",219,219);
        printf("     TITULO     %c",219);
        printf("    AUTOR     %c",219);
        printf("  PAGINAS   %c",219);
        printf("    GENERO     %c",219);
        printf("  QUANTIDADE   %c",219);
        printf("  DISPONIVEIS   %c",219);
        printf("     EXCLUIDO     %c",219);
        gotoxy(1,2);
        for(int i=0;i<120;i++)
            printf("%c",219);
        while(!feof(fp)){
            if(fgets(linha,sizeof(linha),fp)!= NULL){
                sscanf(linha,"ID:%d|Titulo:%[^|]|Autor:%[^|]|Paginas:%d|Genero:%[^|]|Quantidade:%d|Emprestados:%d|Excluido:%d",&str[x].id,str[x].titulo,str[x].autor,&str[x].num_paginas,str[x].genero,&str[x].qtd_total, &str[x].qtd_emprestada,&str[x].excluido);
                if(str[x].excluido == 0) {
                    gotoxy(1,y);printf("%c",186);
                    gotoxy(3,y);printf("%d",str[x].id);gotoxy(8,y);printf("%c",186);
                    gotoxy(10,y);printf("%s",str[x].titulo);gotoxy(25,y);printf("%c",186);
                    gotoxy(27,y);printf("%s",str[x].autor);gotoxy(40,y);printf("%c",186);
                    gotoxy(42,y);printf("%d",str[x].num_paginas);gotoxy(53,y);printf("%c",186);
                    gotoxy(55,y);printf("%s",str[x].genero);gotoxy(69,y);printf("%c",186);
                    gotoxy(71,y);printf("%d",str[x].qtd_total);gotoxy(85,y);printf("%c",186);
                    gotoxy(87,y);printf("%d",str[x].qtd_total-str[x].qtd_emprestada);gotoxy(102,y);printf("%c",186);
                    gotoxy(109,y);printf("Nao");gotoxy(120,y);printf("%c",186);
                    y++;
                } else if(str[x].excluido == 1 && id_u == 0) {
                    gotoxy(1,y);printf("%c",186);
                    gotoxy(3,y);printf("%d",str[x].id);gotoxy(8,y);printf("%c",186);
                    gotoxy(10,y);printf("%s",str[x].titulo);gotoxy(25,y);printf("%c",186);
                    gotoxy(27,y);printf("%s",str[x].autor);gotoxy(40,y);printf("%c",186);
                    gotoxy(42,y);printf("%d",str[x].num_paginas);gotoxy(53,y);printf("%c",186);
                    gotoxy(55,y);printf("%s",str[x].genero);gotoxy(69,y);printf("%c",186);
                    gotoxy(71,y);printf("%d",str[x].qtd_total);gotoxy(85,y);printf("%c",186);
                    gotoxy(87,y);printf("%d",str[x].qtd_total-str[x].qtd_emprestada);gotoxy(102,y);printf("%c",186);
                    gotoxy(109,y);printf("Sim");gotoxy(120,y);printf("%c",186);
                    y++;
                }
                x++;
            }

        }
        printf("\n");
        for(int i=0;i<120;i++)
            printf("%c",219);
    fclose(fp);
}
void mostrar_emprestimos(t_emprestimos *emp){ //função para mostrar os livros que foram emprestados
    char linha[130];
    int x=0, y=3;
    FILE *fp=fopen("emprestimos.txt","a+");
    if(!fp) printf("ERRO NA ABERTURA DO ARQUIVO!");

        printf("%c  ID LIVRO  %c",219,219);
        printf("     ID USUARIO     %c",219);
        printf("    DEVOLVIDO     %c",219);
        gotoxy(1,2);
        for(int i=0;i<120;i++)
            printf("%c",219);
        while(!feof(fp)){
            if(fgets(linha,sizeof(linha),fp)!= NULL){
                sscanf(linha,"Livro:%d|Usuario:%d|Devolvido:%d",&emp[x].livro,&emp[x].usuario_id,&emp[x].devolvido);
                if(emp[x].devolvido == 0) {
                    gotoxy(1,y);printf("%c",186);
                    gotoxy(6,y);printf("%d",emp[x].livro);gotoxy(14,y);printf("%c",186);
                    gotoxy(24,y);printf("%d",emp[x].usuario_id);gotoxy(35,y);printf("%c",186);
                    gotoxy(43,y);printf("Nao");gotoxy(54,y);printf("%c",186);
                    y++;
                } else if(emp[x].devolvido == 1) {
                    gotoxy(1,y);printf("%c",186);
                    gotoxy(6,y);printf("%d",emp[x].livro);gotoxy(14,y);printf("%c",186);
                    gotoxy(24,y);printf("%d",emp[x].usuario_id);gotoxy(35,y);printf("%c",186);
                    gotoxy(43,y);printf("Sim");gotoxy(54,y);printf("%c",186);
                    y++;
                }
                x++;
            }

        }
        printf("\n");
        for(int i=0;i<120;i++)
            printf("%c",219);
    fclose(fp);
}
int adicionar_livro(t_livro*str){ //função para adicionar um livro
    int id, aux;
    char titulo[40];
    char autor[40];
    int num_paginas;
    char genero[15];
    int qtd_total;
    char linha[130];

    gotoxy(40,1);printf("ADICIONAR LIVRO");gotoxy(1,2);
    printf("ID: ");scanf("%d",&id);
    FILE *fp=fopen("livros.txt","a+");
    if(!fp) printf("ERRO NA ABERTURA DO ARQUIVO!");
        while(!feof(fp)){
            if(fgets(linha,sizeof(linha),fp)!= NULL){ //consistência do ID do livro
                sscanf(linha,"ID:%d",&aux);
                if(aux==id){
                    printf("ID ja utilizado, redigite: ");scanf("%d",&id);
                    rewind(fp);
                }
            }
        }
    system("cls");
    gotoxy(40,1);printf("ADICIONAR LIVRO");gotoxy(1,2);
    printf("ID: %d\n",id);
    fflush(stdin);
    printf("Titulo: "); scanf("%[^\n]",titulo);fflush(stdin);
    printf("Autor: ");  scanf("%[^\n]",autor);fflush(stdin);
    printf("Numero de Paginas: ");  scanf("%d",&num_paginas);fflush(stdin);
    printf("Genero: "); scanf("%[^\n]",genero);
    printf("Quantidade: ");scanf("%d",&qtd_total);
    fprintf(fp,"\nID:%d|Titulo:%s|Autor:%s|Paginas:%d|Genero:%s|Quantidade:%d|Emprestados:0|Excluido:0",id,titulo,autor,num_paginas,genero,qtd_total);
    printf("\nLIVRO ADICIONADO");
    qtdl++;
    fclose(fp);
}
int remover_livro(t_livro *str) {//funçaõ de remoção dos livros do acervo
    int x=0, id, aux=0;
    char titulo[40]; char taux[40];
    char linha[130];
    char filename[] = "livros.txt";
    char temp[] = "replace.txt";
    system("cls");
    printf("Digite o ID que deseja excluir: "); scanf("%d",&id);
    FILE *fp=fopen(filename,"a+");
    FILE *ftemp=fopen(temp,"w");
    if(!fp || !ftemp) printf("ERRO NA ABERTURA DO ARQUIVO!");

        while(!feof(fp)){
            if(fgets(linha,sizeof(linha),fp)!= NULL){ //aqui ocorre a exclusão lógica do livro, podendo ser vizualizado pelo administrador mas não pelo usuário.
                sscanf(linha,"ID:%d|Titulo:%[^|]|Autor:%[^|]|Paginas:%d|Genero:%[^|]|Quantidade:%d|Emprestados:%d|Excluido:%d",&str[x].id,str[x].titulo,str[x].autor,&str[x].num_paginas,str[x].genero,&str[x].qtd_total, &str[x].qtd_emprestada,&str[x].excluido);
                    if(str[x].id==id) {
                        fprintf(ftemp,"ID:%d|Titulo:%s|Autor:%s|Paginas:%d|Genero:%s|Quantidade:%d|Emprestados:%d|Excluido:1\n",str[x].id,str[x].titulo,str[x].autor,str[x].num_paginas,str[x].genero,str[x].qtd_total, str[x].qtd_emprestada);
                        aux=1;
                    } else {
                        fprintf(ftemp,linha);
                    }
                x++;
            }
        }
    fclose(fp);
    fclose(ftemp);

    x=0;

    fp = fopen(filename,"w");
    ftemp = fopen(temp,"a+");

    if(!fp || !ftemp) printf("ERRO NA ABERTURA DO ARQUIVO!");
        while(!feof(ftemp)){
            if(fgets(linha,sizeof(linha),ftemp)!= NULL){
                fprintf(fp,linha);
                x++;
            }
        }

    fclose(fp);
    fclose(ftemp);


    if(aux == 1) {
        printf("\nLIVRO REMOVIDO");
    } else
        printf("\nNenhum Livro Encontrado");
    continuar();
    x=0;
}
int pesquisar_livro(t_livro*str){ //pesquisa do livro por titulo e ID
    int opc, x=0;
    int id;
    int aux=0;
    char titulo[40]; char taux[40];
    char linha[130];
    system("cls");
    while(true){
        gotoxy(40,1);printf("PESQUISA DE LIVROS");gotoxy(1,2);
        printf("\n1- Pesquisar por ID\n2- Pesquisar por Nome\n0- Sair\nDigite: ");
        scanf("%d",&opc);
        switch(opc){
            case 1:{ //pesquisa por ID
                system("cls");
                printf("Digite o ID que deseja pesquisar: "); scanf("%d",&id);
                FILE *fp=fopen("livros.txt","a+");
                if(!fp) printf("ERRO NA ABERTURA DO ARQUIVO!");
                    while(!feof(fp)){
                        if(fgets(linha,sizeof(linha),fp)!= NULL){
                            sscanf(linha,"ID:%d|Titulo:%[^|]|Autor:%[^|]|Paginas:%d|Genero:%[^|]|Quantidade:%d|Emprestados:%d|Excluido:%d",&str[x].id,str[x].titulo,str[x].autor,&str[x].num_paginas,str[x].genero,&str[x].qtd_total, &str[x].qtd_emprestada,&str[x].excluido);
                            if(str[x].id==id){
                               printf("\nID: %d|Titulo: %s|Autor: %s|Paginas: %d|Genero: %s|Quantidade: %d|Emprestados: %d \n",str[x].id,str[x].titulo,str[x].autor,str[x].num_paginas,str[x].genero,str[x].qtd_total, str[x].qtd_emprestada,str[x].excluido);
                               aux=1;
                            }
                            x++;
                        }
                    }
                  fclose(fp);
                  if(aux!=1)
                    printf("\nNenhum Livro Encontrado");
                  continuar();
                  x=0;
                 break;
            }
            case 2:{ //pesquisa por titulo, compara parcialmente a string digitada com o título do livro
                system("cls");
                printf("Digite o Titulo que deseja pesquisar: ");
                fflush(stdin);
                scanf("%[^\n]",titulo);
                FILE *fp=fopen("livros.txt","a+");
                if(!fp) printf("ERRO NA ABERTURA DO ARQUIVO!");
                    while(!feof(fp)){
                        if(fgets(linha,sizeof(linha),fp)!= NULL){
                            sscanf(linha,"ID:%d|Titulo:%[^|]|Autor:%[^|]|Paginas:%d|Genero:%[^|]|Quantidade:%d|Emprestados:%d|Excluido:%d",&str[x].id,str[x].titulo,str[x].autor,&str[x].num_paginas,str[x].genero,&str[x].qtd_total, &str[x].qtd_emprestada,&str[x].excluido);
                            if(strstr(str[x].titulo, titulo) != NULL){
                               printf("\nID: %d|Titulo: %s|Autor: %s|Paginas: %d|Genero: %s|Quantidade: %d|Emprestados: %d \n",str[x].id,str[x].titulo,str[x].autor,str[x].num_paginas,str[x].genero,str[x].qtd_total, str[x].qtd_emprestada,str[x].excluido);
                               aux=1;
                            }
                            x++;
                        }
                    }
                  fclose(fp);
                  if(aux!=1)
                    printf("\nNenhum Livro Encontrado");
                  continuar();
                  x=0;
                break;
            }
            case 0:
                return 0;

        }
    }
}
void emprestar_livro(t_livro *str, t_emprestimos *emp) { //função emprestar livro
    int x=0, id, aux=0;
    char titulo[40]; char taux[40];
    char linha[130];
    char filenameEmprestimos[] = "emprestimos.txt";
    char filenameLivros[] = "livros.txt";
    char temp[] = "replace.txt";
    system("cls");
    printf("Digite o ID do livro que deseja emprestar: "); scanf("%d",&id);
    FILE *fp=fopen(filenameEmprestimos,"a+");
    FILE *fpLivros=fopen(filenameLivros,"a+");
    FILE *ftemp=fopen(temp,"w");
    if(!fp || !ftemp || !fpLivros) printf("ERRO NA ABERTURA DO ARQUIVO!");
        while(!feof(fpLivros)){
            if(fgets(linha,sizeof(linha),fpLivros)!= NULL){
                sscanf(linha,"ID:%d|Titulo:%[^|]|Autor:%[^|]|Paginas:%d|Genero:%[^|]|Quantidade:%d|Emprestados:%d|Excluido:%d",&str[x].id,str[x].titulo,str[x].autor,&str[x].num_paginas,str[x].genero,&str[x].qtd_total, &str[x].qtd_emprestada,&str[x].excluido);
                    if(str[x].id==id && str[x].excluido == 0 && str[x].qtd_total-str[x].qtd_emprestada > 0) {
                        fprintf(ftemp,"ID:%d|Titulo:%s|Autor:%s|Paginas:%d|Genero:%s|Quantidade:%d|Emprestados:%d|Excluido:%d\n",str[x].id,str[x].titulo,str[x].autor,str[x].num_paginas,str[x].genero,str[x].qtd_total, str[x].qtd_emprestada+1,str[x].excluido);
                        fprintf(fp,"Livro:%d|Usuario:%d|Devolvido:0",str[x].id,id_u);
                        aux=1;
                    } else {
                        fprintf(ftemp,linha);
                    }
                x++;
            }
        }
    fclose(fp);
    fclose(ftemp);
    fclose(fpLivros);

    x=0;


    //reescrever livros nos arquivos de textos auxiliares para reescrevelos nos principais
    fpLivros = fopen(filenameLivros,"w");
    ftemp = fopen(temp,"a+");

    if(!fpLivros || !ftemp) printf("ERRO NA ABERTURA DO ARQUIVO!");
        while(!feof(ftemp)){
            if(fgets(linha,sizeof(linha),ftemp)!= NULL){
                fprintf(fpLivros,linha);
                x++;
            }
        }

    fclose(fpLivros);
    fclose(ftemp);


    if(aux == 1) {
        printf("\nLIVRO EMPRESTADO");
        qtde++;
    } else
        printf("\nNenhum Livro Encontrado ou livro indisponivel");
    continuar();
    x=0;
}
void devolver_livro(t_livro *str, t_emprestimos *emp) { //funçaõ para devolução dos livros
    int x=0, id, aux=0;
    char titulo[40]; char taux[40];
    char linha[130];
    char filenameEmprestimos[] = "emprestimos.txt";
    char filenameLivros[] = "livros.txt";
    char tempDevolver[] = "devolver.txt";
    char temp[] = "replace.txt";
    system("cls");
    printf("Digite o ID do livro que deseja devolver: "); scanf("%d",&id);
    FILE *fp=fopen(filenameEmprestimos,"a+");
    FILE *fpLivros=fopen(filenameLivros,"a+");
    FILE *fptempLivros=fopen(tempDevolver,"w");
    FILE *ftemp=fopen(temp,"w");

    if(!fp || !ftemp || !fpLivros || !fptempLivros) printf("ERRO NA ABERTURA DO ARQUIVO!");
        while(!feof(fp)){
            if(fgets(linha,sizeof(linha),fp)!= NULL){
                sscanf(linha,"Livro:%d|Usuario:%d|Devolvido:%d",&emp[x].livro,&emp[x].usuario_id,&emp[x].devolvido);
                    if(emp[x].livro==id && emp[x].devolvido == 0 && emp[x].usuario_id == id_u) {
                        fprintf(fptempLivros,"Livro:%d|Usuario:%d|Devolvido:1\n",emp[x].livro,id_u);
                        aux=1;
                    } else {
                        fprintf(fptempLivros,linha);
                    }
                x++;
            }
        }
        x=0;
        while(!feof(fpLivros)){
            if(fgets(linha,sizeof(linha),fpLivros)!= NULL){
                sscanf(linha,"ID:%d|Titulo:%[^|]|Autor:%[^|]|Paginas:%d|Genero:%[^|]|Quantidade:%d|Emprestados:%d|Excluido:%d",&str[x].id,str[x].titulo,str[x].autor,&str[x].num_paginas,str[x].genero,&str[x].qtd_total, &str[x].qtd_emprestada,&str[x].excluido);
                    if(str[x].id==id && str[x].excluido == 0) {
                        fprintf(ftemp,"ID:%d|Titulo:%s|Autor:%s|Paginas:%d|Genero:%s|Quantidade:%d|Emprestados:%d|Excluido:%d\n",str[x].id,str[x].titulo,str[x].autor,str[x].num_paginas,str[x].genero,str[x].qtd_total, str[x].qtd_emprestada-1,str[x].excluido);
                        aux=1;
                    } else {
                        fprintf(ftemp,linha);
                    }
                x++;
            }
        }
    fclose(fp);
    fclose(ftemp);
    fclose(fpLivros);
    fclose(fptempLivros);

    x=0;


    //utilização de 2 arquivos auxiliares para reescrever os arquivos de livros e pessoas
    fpLivros = fopen(filenameLivros,"w");
    ftemp = fopen(temp,"a+");

    if(!fpLivros || !ftemp) printf("ERRO NA ABERTURA DO ARQUIVO!");
        while(!feof(ftemp)){
            if(fgets(linha,sizeof(linha),ftemp)!= NULL){
                fprintf(fpLivros,linha);
                x++;
            }
        }

    fclose(fpLivros);
    fclose(ftemp);
    x=0;
    //reescrever livros
    fp = fopen(filenameEmprestimos,"w");
    fptempLivros = fopen(tempDevolver,"a+");

    if(!fp || !fptempLivros) printf("ERRO NA ABERTURA DO ARQUIVO!");
        while(!feof(fptempLivros)){
            if(fgets(linha,sizeof(linha),fptempLivros)!= NULL){
                fprintf(fp,linha);
                x++;
            }
        }

    fclose(fp);
    fclose(fptempLivros);


    if(aux == 1) {
        printf("\nLIVRO DEVOLVIDO");
        qtde++;
    } else
        printf("\nNenhum Livro Encontrado");
    continuar();
    x=0;
}

int main()
{
    t_livro *acervo;
    t_usuario *pessoas;
    t_emprestimos *emprestimos;
    system("color F0"); //mudar cor de fundo e cor do texto
    system("mode con:cols=120 lines=50"); //fixar linhas e colunas do console

    int menu;
    qtdp=abrir_arq("pessoas.txt");
    qtdl=abrir_arq("livros.txt");
    qtde=abrir_arq("emprestimos.txt");
    printf("%d-%d-%d",qtdl,qtdp,id_u);
    pessoas = (t_usuario *) malloc(qtdp*sizeof(t_usuario));
    acervo = (t_livro *) malloc(qtdl*sizeof(t_livro));
    emprestimos = (t_emprestimos *) malloc(qtde*sizeof(t_emprestimos));

    login(pessoas);
    while(true){
        if(id_u == 0) { //admin
            printf("%d-%d-%d",qtdl,qtdp,id_u);
            gotoxy(40,1);printf("MENU");gotoxy(1,2);
            printf("\n1- Ver Acervo\n2- Adicionar Livro\n3- Remover Livro\n4- Ver emprestimos\n0- Sair\nDigite: ");
            scanf("%d",&menu);
            if(menu<0||menu>4){
                do{
                    printf("Numero fora dos limites, redigite: ");
                    scanf("%d",&menu);
                }while(menu<0||menu>4);
            }
            switch(menu){
                case 1:
                    system("cls");
                    mostrar_acervo(acervo);
                    continuar();
                    break;
                case 2:
                    system("cls");
                    adicionar_livro(acervo);
                    continuar();
                    acervo = (t_livro *) realloc(acervo,qtdl*sizeof(t_livro)); //realocar a quantidade de livros
                    system("cls");
                    break;
                case 3:
                    system("cls");
                    remover_livro(acervo);
                    continuar();
                    acervo = (t_livro *) realloc(acervo,qtdl*sizeof(t_livro));
                    break;
                case 4:
                    system("cls");
                    system("cls");
                    mostrar_emprestimos(emprestimos);
                    continuar();
                    break;
                case 0:
                    exit(0);

            }
        } else { //usuario comum
            printf("%d-%d-%d",qtdl,qtdp,id_u);
            gotoxy(40,1);printf("MENU");gotoxy(1,2);
            printf("\n1- Ver Acervo\n2- Pesquisar Livro\n3- Emprestar Livro\n4- Devolver Livro\n5 - Ver emprestimos\n0- Sair\nDigite: ");
            scanf("%d",&menu);
            if(menu<0||menu>5){
                do{
                    printf("Numero fora dos limites, redigite: ");
                    scanf("%d",&menu);
                }while(menu<0||menu>5);
            }
            switch(menu){
                case 1:
                    system("cls");
                    mostrar_acervo(acervo);
                    continuar();
                    break;
                case 2:
                    system("cls");
                    pesquisar_livro(acervo);
                    system("cls");
                    break;
                case 3:
                    system("cls");
                    emprestar_livro(acervo,emprestimos);
                    continuar();
                    emprestimos = (t_emprestimos *) realloc(emprestimos,qtde*sizeof(t_emprestimos)); //realocar a quantidade de emprestimos
                    break;
                case 4:
                    system("cls");
                    devolver_livro(acervo,emprestimos);
                    continuar();
                    break;
                case 5:
                    system("cls");
                    mostrar_emprestimos(emprestimos);
                    continuar();
                    break;
                case 0:
                    exit(0);

            }
        }
    }




    return 0;
}
