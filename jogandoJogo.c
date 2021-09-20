#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct 
{
    char tipo[1000];
    int identidade;
    int orientacao;
    int abscissa;
    char ordenada;
    int tipoNumerico;
    int atirou;
} tComponente;

typedef struct 
{
    char jogador[16];
    int tirosErrados;
    int tirosAcertados;
    float mediaAbscissa;
    float mediaOrdenada;
    float desvioPadrao;
    char naviosAtingidos[100][50];
    int numeroExtra;
} tEstatisticas;
typedef struct 
{
    int quandoAcertou;
    char stringTipo[50];
    int identidade;
} tAcertos;


int LinhasIguais(tComponente navio1, tComponente navio2);
void FormaArrayZerada(int i, int array[i]);
void FormaTabuleiroZerado(int tabuleiro[10][10]);
void MatrizHomogenea(int x, int y, char matriz[x][y]);
void StringHomogenea(int x, char string[x]);
tComponente LeCoisas(char * dados);
void ColocaCoisas(tComponente coisas, int tabuleiro[10][10]);
int VerificaTipo(tComponente coisas);
void LeEmbarcados(FILE * tabu, tComponente * navios);
void ReordenaNavios(tComponente * navios, int n);
void Inicializando(FILE * tabuFinal, int tabuleiro[10][10]);
int VerificaTabuleiro(tComponente * navios, int tabuleiro[10][10], int n);
void MontaTabuleiro(tComponente * navios, int tabuleiro[10][10], int n);
int ContaLinhas(FILE * tabu);
void ComparaFrota(tComponente * naviosUm, tComponente * naviosDois, int n, int m, FILE * tabu_verificado);
void ValidandoTabuleiro(char * nomeArquivo, tComponente * navios, int tabuleiro[10][10], int n, FILE * tabu_verificado);

void TomadaDecisao(char resultante[10][10], char jogador[16], char * decisao);
void Lancamento(char * decisao, int qtdJogadas, char jogadas[100][50], int tabuleiro[10][10], char resultante[10][10], tComponente * navios, int n);
int Ganhou(tComponente * navios, int n);
void ImprimirTabuleiroBuffer(char resultante[10][10]);
void ImprimeJogadas_noArquivo(FILE * resultado, int x, char jogadas[100][50], char * jogador);

void CalculaEstatisticas(tEstatisticas estatisticasA, tEstatisticas estatisticasB, FILE * resultado, FILE * arquivoEstatisticas, char * jogadorA, char * jogadorB);
void ImprimeEstatisticas(FILE * estatisticas, tEstatisticas dados, tAcertos * tiros);
void OrdenaCrescente(int qtd, tAcertos * linhas);
int VerificaIdentidadeIgualAntes(int n, tAcertos * linhas);
void FormaStringComNumeros(char * string, int rodada, tAcertos linha);
tEstatisticas InicializaEstatisticas();

int main(int argc, char * argv[]){
    //diretorio
    if(argc <= 1){
        printf("ERRO: O diretorio de arquivos de configuracao nao foi informado\n");
        return 1;
    }

    char nomeArquivo1[15] = "tabu_1.txt";
    FILE * tabu_1 = fopen(nomeArquivo1, "r");
    if( ! (tabu_1) ){
        printf("%s nao encontrado no diretorio %s\n", nomeArquivo1, argv[1]);
        return 1;
    }

    char nomeArquivo2[15] = "tabu_2.txt";
    FILE * tabu_2 = fopen(nomeArquivo2, "r");
    if( ! (tabu_2) ){
        printf("%s nao encontrado no diretorio %s\n", nomeArquivo2, argv[1]);
        return 1;
    }
    
    int n = ContaLinhas(tabu_1);
    int m = ContaLinhas(tabu_2);
    fclose(tabu_1);
    fclose(tabu_2);
     
    tComponente naviosUm[n];
    tComponente naviosDois[m];
    int tabuleiro1[10][10], tabuleiro2[10][10];
    tabu_1 = fopen(nomeArquivo1, "r");
    tabu_2 = fopen(nomeArquivo2, "r");
    LeEmbarcados(tabu_1, naviosUm);
    LeEmbarcados(tabu_2, naviosDois);
    fclose(tabu_1); fclose(tabu_2);
    //montando tabuleiro numérico para comparações futuras
    MontaTabuleiro(naviosUm, tabuleiro1, n);
    MontaTabuleiro(naviosDois, tabuleiro2, m);

    //verificando se é valido ou não
    FILE * verificando = fopen("saida/validacao_tabuleiros.txt", "w");
    ValidandoTabuleiro(nomeArquivo1, naviosUm, tabuleiro1, n, verificando);
    fprintf(verificando, "\n");
    ValidandoTabuleiro(nomeArquivo2, naviosDois, tabuleiro2, m, verificando);
    fclose(verificando);

    verificando = fopen("saida/validacao_tabuleiros.txt", "a+");
    ComparaFrota(naviosUm, naviosDois, n, m, verificando);
    fclose(verificando);

    //jogadores
    char jogador1[16];
    char jogador2[16];
    printf("Nome do Jogador 1:\n");
    scanf("%s", jogador1);
    printf("Nome do Jogador 2:\n\n");
    scanf("%*c%s", jogador2);

    //inicializando
    FILE * inicializando = fopen("saida/inicializacao.txt", "w");
    fprintf(inicializando, "%s\n", jogador1);
    Inicializando(inicializando, tabuleiro1);
    fprintf(inicializando, "\n\n%s\n", jogador2);
    Inicializando(inicializando, tabuleiro2);
    fclose(inicializando);

    //jogando
    int ganhou1=0, ganhou2=0, qtdJogadas=0;
    char jogada[3];
    char totalJogadas1[100][50], totalJogadas2[100][50];
    char resultante1[10][10];
    char resultante2[10][10];
    MatrizHomogenea(10, 10, resultante1);
    MatrizHomogenea(10, 10, resultante2);
    while(1){
        TomadaDecisao(resultante1, jogador1, jogada);
        Lancamento(jogada, qtdJogadas, totalJogadas1, tabuleiro2, resultante1, naviosDois, n); //tem que jogar com o tabuleiro do outro
        printf("Tabuleiro atual de %s apos a jogada de %s\n", jogador2, jogador1);
        ImprimirTabuleiroBuffer(resultante1);
        printf("\n");
        ganhou1 = Ganhou(naviosDois, n);
        TomadaDecisao(resultante2, jogador2, jogada);
        Lancamento(jogada, qtdJogadas, totalJogadas2, tabuleiro1, resultante2, naviosUm, m);
        printf("Tabuleiro atual de %s apos a jogada de %s\n", jogador1, jogador2);
        ImprimirTabuleiroBuffer(resultante2);
        printf("\n");
        ganhou2 = Ganhou(naviosUm, m);
        qtdJogadas++;   
        if(ganhou1 || ganhou2) break;    
    }

    //gerando resumo de resultado
    FILE * resultado = fopen("saida/resultado.txt", "w");
    ImprimeJogadas_noArquivo(resultado, qtdJogadas, totalJogadas1, jogador1);
    ImprimeJogadas_noArquivo(resultado, qtdJogadas, totalJogadas2, jogador2);
    if(ganhou1 && ganhou2){
        printf("Empate\n");
        fprintf(resultado, "Empate");
    }
    else{
        fprintf(resultado, "Vencedor: ");
        if(ganhou1 && !(ganhou2)){
            printf("Vencedor: %s\n", jogador1);
            fprintf(resultado, "%s", jogador1);
        }
        else if(!(ganhou1) && ganhou2){
            printf("Vencedor: %s\n", jogador2);
            fprintf(resultado, "%s", jogador2);
        }
    }
    fclose(resultado);

    //calculando estatisticas
    tEstatisticas dados1, dados2;
    FILE * arquivoEstatisticas = fopen("saida/estatisticas.txt", "w");
    resultado = fopen("saida/resultado.txt", "r");
    CalculaEstatisticas(dados1, dados2, resultado, arquivoEstatisticas, jogador1, jogador2);
    fclose(resultado);
    fclose(arquivoEstatisticas);

    return 0;
}

tComponente LeCoisas(char * dados){
    tComponente coisas;
    int i=0;
    coisas.tipoNumerico = 0;
    coisas.atirou = 0;
    sscanf(dados, "%[^;]%*c%d%*c%d%*c%c%d", coisas.tipo, &coisas.identidade, 
    &coisas.orientacao, &coisas.ordenada, &coisas.abscissa);

    coisas.tipoNumerico = VerificaTipo(coisas);

    if(coisas.tipoNumerico == 1) coisas.tipoNumerico = 3;

    return coisas;
}
void LeEmbarcados(FILE * tabu, tComponente * navios){
    char escrito[1000];
    int i=0;
    while( ! (feof(tabu)) ){
        fscanf(tabu, "%[^\n]%*[\n]", escrito);
        navios[i] = LeCoisas(escrito);
        i++;
    }
    ReordenaNavios(navios, i);
}
void Inicializando(FILE * tabuFinal, int tabuleiro[10][10]){
    int i, j;
    for(i=0; i<10; i++){
        for(j=0; j<10; j++){
            if(tabuleiro[i][j] != 0) fprintf(tabuFinal, "%c", 'X');
            else fprintf(tabuFinal, "%c", 'o');
            if(j != 9) fprintf(tabuFinal, " ");
        }
        if(i != 9) fprintf(tabuFinal,"\n");
    }
}
void ColocaCoisas(tComponente coisas, int tabuleiro[10][10]){
    int a=coisas.ordenada - 'a';
    int b=coisas.abscissa - 1;
    int i=0, j=0;
    int x = coisas.tipoNumerico;

    if(coisas.orientacao == 0){
        for(i=a; i < a+x; i++) tabuleiro[i][b] = coisas.identidade;
        
    }
    else if(coisas.orientacao == 1){
        for(j=b; j < b+x; j++) tabuleiro[a][j] = coisas.identidade;
    }
}
int VerificaTabuleiro(tComponente * navios, int tabuleiro[10][10], int n){
    int i=0, j=0, x=0, estaAli=0;
    for(x=0; x<n; x++){
        if(navios[x].ordenada < 'a' || navios[x].ordenada > 'j') return 0;
        if(navios[x].abscissa < 1 || navios[x].abscissa > 10) return 0;
        if(navios[x].tipoNumerico == 0) return 0;
        if(navios[x].orientacao == 0){
            if(navios[x].ordenada + navios[x].tipoNumerico - 1 > 'j') return 0;
        }
        else if(navios[x].orientacao == 1){
            if(navios[x].abscissa + navios[x].tipoNumerico - 1 > 10) return 0;
        }
    }

    for(i=0; i<10; i++){
        for(j=0; j<10; j++){
            if(tabuleiro[i][j] == 0) continue;
            if(i>0 && tabuleiro[i-1][j] != 0 && tabuleiro[i-1][j] != tabuleiro[i][j]) return 0; //verificando o que tá em cima
            if(i<9 && tabuleiro[i+1][j] != 0 && tabuleiro[i+1][j] != tabuleiro[i][j]) return 0; //verificando o que tá em baixo
            if(j>0 && tabuleiro[i][j-1] != 0 && tabuleiro[i][j-1] != tabuleiro[i][j]) return 0; //à esquerda
            if(j<9 && tabuleiro[i][j+1] != 0 && tabuleiro[i][j+1] != tabuleiro[i][j]) return 0; //à direita
            if(i>0 && j>0 && tabuleiro[i-1][j-1] != 0) return 0; //diagonal esquerda superior
            if(i>0 && j<9 && tabuleiro[i-1][j+1] != 0) return 0; //diagonal direita superior
            if(i<9 && j>0 && tabuleiro[i+1][j-1] != 0) return 0; //diagonal esquerda inferior
            if(i<9 && j<9 && tabuleiro[i+1][j+1] != 0) return 0; //diagonal direita superior 
        }
    }
    for(x=0; x<n; x++){
        estaAli = 0;
        for(i=0; i<10; i++){
            for(j=0; j<10; j++){
                if(tabuleiro[i][j] == navios[x].identidade){
                    estaAli = 1; //se o navio não existe ali, ele está todo em baixo de outro
                    break;
                }
            }
            if(estaAli == 1) break;
        }
        if(estaAli == 0) return 0;
    }
    return 1;
}
void ComparaFrota(tComponente * naviosUm, tComponente * naviosDois, int n, int m, FILE * tabu_verificado){
    int i=0, x=0, y=0, k=0;
    int qtdTipo1[5], qtdTipo2[5];
    FormaArrayZerada(5, qtdTipo1);
    FormaArrayZerada(5, qtdTipo2);

    char validade1[10], validade2[10];
    fscanf(tabu_verificado, "%*[^;]%*c%[^\n]", validade1);
    fscanf(tabu_verificado, "%*[^;]%*c%s", validade2);

    if((!(strcmp(validade1, "valido"))) && (!(strcmp(validade2, "valido")))){
        for(i=0; i<n; i++){
            x = VerificaTipo(naviosUm[i]);
            for(k=i+1; k<n; k++){
                if(LinhasIguais(naviosUm[i], naviosUm[k])){
                    qtdTipo1[x-1]--;
                    break;
                }
            }
            qtdTipo1[x-1]++;
        }
        for(i=0; i<m; i++){
            x = VerificaTipo(naviosDois[i]);
            for(k=i+1; k<m; k++){
                if(LinhasIguais(naviosDois[i], naviosDois[k])){
                    qtdTipo2[x-1]--;
                    break;
                }
            }
            qtdTipo2[x-1]++;
        }
        for(i=0; i<5; i++){
            if(qtdTipo1[i] == qtdTipo2[i]) y = 1;
            else{
                y = 0;
                break;
            }
        }
        if(y == 1) fprintf(tabu_verificado, "%s", "\nTabuleiros compativeis entre si");
        else{
            fprintf(tabu_verificado, "%s", "\nTabuleiros incompativeis entre si");
            exit(1);
        }
    }
    else exit(1);
}
int VerificaTipo(tComponente coisas){
    if(!(strcmp(coisas.tipo, "Carrier"))) return 5;
    else if(!(strcmp(coisas.tipo, "Battleship"))) return 4;
    else if(!(strcmp(coisas.tipo, "Cruiser"))) return 3;
    else if(!(strcmp(coisas.tipo, "Destroyer"))) return 2;
    else if(!(strcmp(coisas.tipo, "Submarine"))) return 1;
    else return 0;
}
void FormaTabuleiroZerado(int tabuleiro[10][10]){
    int i=0, j=0;
    for(i=0; i<10; i++){
        for(j=0; j<10; j++) tabuleiro[i][j] = 0;
    }
}
void ReordenaNavios(tComponente * navios, int n){
    int i=0, j=0, k=0, l=0;
    tComponente aux;
    for(i=0; i<n; i++){
        for(j=i+1; j<n; j++){
            if(navios[i].identidade > navios[j].identidade){
                aux = navios[i];
                navios[i] = navios[j];
                navios[j] = aux;
            }
        }
    }
}
void FormaArrayZerada(int i, int array[i]){
    int j=0;
    for(j=0; j < i; j++) array[j] = 0;
}
void MontaTabuleiro(tComponente * navios, int tabuleiro[10][10], int n){
    int x=0;
    FormaTabuleiroZerado(tabuleiro);
    for(x=0; x<n; x++){
        ColocaCoisas(navios[x], tabuleiro);
    }
}
int ContaLinhas(FILE * tabu){
    int i=0;
    char escrito[1000];
    while( ! (feof(tabu)) ){
        fscanf(tabu, "%[^\n]%*[\n]", escrito);
        i++;
    }
    return i;
}
void ValidandoTabuleiro(char * nomeArquivo, tComponente * navios, int tabuleiro[10][10], int n, FILE * tabu_verificado){
    //verificando se é valido ou não
    fprintf(tabu_verificado, "%s", nomeArquivo);
    if(VerificaTabuleiro(navios, tabuleiro, n)) fprintf(tabu_verificado, "%s", ";valido");
    else fprintf(tabu_verificado, "%s", ";invalido");
}
void TomadaDecisao(char resultante[10][10], char jogador[16], char * decisao){
    char ordenada='\0'; int abscissa=0; char restante='\0';
    int a=0;
    int b=0;
    int i=0, j=0, x=0;
    printf("Jogada de %s:\n\n", jogador);
    while(1){
        x=0;
        scanf("%s", decisao);
        sscanf(decisao, "%c%d%c", &ordenada, &abscissa, &restante);

        if((restante != '\0' && restante != '\n')  || ordenada < 'a' || ordenada > 'j' || 
        abscissa < 1 || abscissa > 10) x = 1; //se o comando não está nos conformes

        a = ordenada - 'a';
        b = abscissa - 1;
        if(resultante[a][b] != '.') x = 1;

        if(x == 1){
            printf("%s: Jogada inválida! Jogue novamente %s:\n", decisao, jogador);
            restante = '\0'; ordenada = '\0'; abscissa = 0; //reiniciando as coisas
        }
        else{
            break;
        }
    }
}
void Lancamento(char * decisao, int qtdJogadas, char jogadas[100][50], int tabuleiro[10][10], char resultante[10][10], tComponente * navios, int n){
    char ordenada='\0'; int abscissa=0;
    int i=0, j=0;
    sscanf(decisao, "%c%d", &ordenada, &abscissa);
    int a = ordenada - 'a';
    int b = abscissa - 1;
    printf("%s:", decisao);
    strcpy(jogadas[qtdJogadas], decisao);
    if(tabuleiro[a][b] == 0){
        resultante[a][b] = 'o';
        printf("Agua\n");
        strcat(jogadas[qtdJogadas], " - Agua\0");
    }
    else{
        while(navios[i].identidade != tabuleiro[a][b]) i++;
        navios[i].atirou++;
        resultante[a][b] = 'X';
        strcat(jogadas[qtdJogadas], " - Tiro - ");
        strcat(jogadas[qtdJogadas], navios[i].tipo);
        strcat(jogadas[qtdJogadas], " - ID ");
        if(navios[i].identidade < 10) strcat(jogadas[qtdJogadas], "0"); //cada doido decide a identidade que quiser
        char ID[4];
        sprintf(ID, "%d", navios[i].identidade);
        strcat(ID, "\0");
        strcat(jogadas[qtdJogadas], ID);
        if(navios[i].atirou == navios[i].tipoNumerico) printf("Afundou %s\n", navios[i].tipo);
        else printf("Tiro!\n");
    }
    printf("\n");
}
int Ganhou(tComponente * navios, int n){
    int i=0, x=0;
    for(i=0; i<n; i++){
        if(navios[i].atirou != navios[i].tipoNumerico) return 0;
    }
    return 1;
}
void ImprimirTabuleiroBuffer(char resultante[10][10]){
    int i=0, j=0;
    for(i=0; i<10; i++){
        for(j=0; j<10; j++){
            printf("%c", resultante[i][j]);
            if(j != 9) printf(" ");
        }
        if(i != 9) printf("\n");
    }
    printf("\n");
}
void MatrizHomogenea(int x, int y, char matriz[x][y]){
    int i=0, j=0;
    for(i=0; i<x; i++){
        for(j=0; j<y; j++){
            matriz[i][j] = '.';
        }
    }
}
int LinhasIguais(tComponente navio1, tComponente navio2){
    int qtdIguais=0;
    if(!(strcmp(navio1.tipo, navio2.tipo))){
        if(navio1.identidade == navio2.identidade){
            if(navio1.ordenada == navio2.ordenada){
                if(navio1.abscissa == navio2.abscissa){
                    return 1;
                }
            }
        }
    }
    return 0;
}
void ImprimeJogadas_noArquivo(FILE * resultado, int x, char jogadas[100][50], char * jogador){
    int i=0, j=0;
    fprintf(resultado, "%s", jogador);
    fprintf(resultado, "\n");
    for(i=0; i<x; i++){
        fprintf(resultado, "%s", jogadas[i]);
        fprintf(resultado, "\n");
    }
    fprintf(resultado, "\n");
}
void CalculaEstatisticas(tEstatisticas estatisticasA, tEstatisticas estatisticasB, FILE * resultado, FILE * arquivoEstatisticas, char * jogadorA, char * jogadorB){
    tAcertos tirosA[100], tirosB[100];
    char string[50], acertou[4];
    char casoAtinja[50];
    char quebraEspaco;
    int i=0, j=0;
    char ordenadaA[100]; int abscissaA[100];
    char ordenadaB[100]; int abscissaB[100];
    int totalOrdenadasA=0, totalAbscissasA=0, totalOrdenadasB=0, totalAbscissasB=0;
    float somatorioDesvioA=0.0, somatorioDesvioB=0.0;
    float subtracaoLinhasA=0.0, subtracaoColunasA=0.0;
    float subtracaoLinhasB=0.0, subtracaoColunasB=0.0;

    estatisticasA = InicializaEstatisticas();
    estatisticasB = InicializaEstatisticas();

    fscanf(resultado, "%[^\n]%*[\n]", estatisticasA.jogador);
    
    while(1){
        abscissaA[i] = 0;
        ordenadaA[i] = '\0';
        fscanf(resultado, "%[^\n]%*[\n]", string);
        
        if(!(strcmp(jogadorB, string))) break;
        
        strcpy(casoAtinja, string);
        sscanf(string, "%c%d %*c %[^ ]%*s%*[\n]", &ordenadaA[i], &abscissaA[i], acertou);

        totalAbscissasA += abscissaA[i];
        totalOrdenadasA += ordenadaA[i] - 'a' + 1;

        if( ! (strcmp(acertou, "Tiro") ) ){
            
            sscanf(casoAtinja, 
                "%*[^ ] - %*[^ ] - %[^ ] - ID %d%*c", 
                tirosA[i].stringTipo, 
                &tirosA[i].identidade);

            tirosA[i].quandoAcertou = i+1;
            estatisticasA.tirosAcertados++;
        } 
        else if( ! (strcmp(acertou, "Agua") ) ){
            strcpy(tirosA[i].stringTipo, " Sla\0");
            estatisticasA.tirosErrados++;
            strcpy(estatisticasB.naviosAtingidos[i], acertou);
            strcat(estatisticasB.naviosAtingidos[i], tirosA[i].stringTipo);
            tirosA[i].identidade = 0;
            tirosA[i].quandoAcertou = 0;
        }
        i++;
    }
    estatisticasA.mediaAbscissa = (float) totalAbscissasA/i;
    estatisticasA.mediaOrdenada = (float) totalOrdenadasA/i;
    //printf("\n%d", i);
    //fecha bloco da primeira estatística

    strcpy(estatisticasB.jogador, jogadorB);
    for(j=0; j<i; j++){
        abscissaB[j] = 0;
        ordenadaB[j] = '\0';
        fscanf(resultado, "%[^\n]%*[\n]", string);

        strcpy(casoAtinja, string);
        sscanf(string, "%c%d %*c %[^ ]%*s%*[\n]", &ordenadaB[j], &abscissaB[j], acertou);

        totalAbscissasB += abscissaB[j];
        totalOrdenadasB += ordenadaB[j]  - 'a' + 1;

        if( ! (strcmp(acertou, "Tiro") ) ){
            
            sscanf(casoAtinja, 
                "%*[^ ] - %*[^ ] - %[^ ] - ID %d%*c", 
                tirosB[j].stringTipo, 
                &tirosB[j].identidade);

            tirosB[j].quandoAcertou = j+1;
            
            estatisticasB.tirosAcertados++;
        } 
        else if( ! (strcmp(acertou, "Agua") ) ){
            strcpy(tirosB[j].stringTipo, " Sla\0");
            estatisticasB.tirosErrados++;
            tirosB[j].identidade = 0;
            tirosB[j].quandoAcertou = 0;
        }
    }
    estatisticasB.mediaAbscissa = (float) totalAbscissasB/j;
    estatisticasB.mediaOrdenada = (float) totalOrdenadasB/j;

    //contas para organização
    estatisticasA.numeroExtra = i;
    OrdenaCrescente(i, tirosA);
    int x;
    for(x=0; x<i; x++){
        FormaStringComNumeros(estatisticasB.naviosAtingidos[x], tirosA[x].quandoAcertou, tirosA[x]);
        subtracaoLinhasA = (float) ordenadaA[x] - 'a' + 1 - estatisticasA.mediaOrdenada;
        subtracaoLinhasA = pow(subtracaoLinhasA, 2);
        subtracaoColunasA = (float) abscissaA[x] - estatisticasA.mediaAbscissa;
        subtracaoColunasA = pow(subtracaoColunasA, 2);
        somatorioDesvioA += subtracaoColunasA + subtracaoLinhasA;
    }

    estatisticasB.numeroExtra = j;
    OrdenaCrescente(j, tirosB);
    int y;
    for(y=0; y<j; y++){
        FormaStringComNumeros(estatisticasA.naviosAtingidos[y], tirosB[y].quandoAcertou, tirosB[y]);
        subtracaoLinhasB = (float) ordenadaB[y] - 'a' + 1 - estatisticasB.mediaOrdenada;
        subtracaoLinhasB = pow(subtracaoLinhasB, 2);
        subtracaoColunasB = (float) abscissaB[y] - estatisticasB.mediaAbscissa;
        subtracaoColunasB = pow(subtracaoColunasB, 2);
        somatorioDesvioB += subtracaoColunasB + subtracaoLinhasB;
    }
    somatorioDesvioA /= i;
    somatorioDesvioB /= j;
    somatorioDesvioA = sqrt(somatorioDesvioA);
    somatorioDesvioB = sqrt(somatorioDesvioB);
    
    estatisticasA.desvioPadrao = somatorioDesvioA;
    estatisticasB.desvioPadrao = somatorioDesvioB;

    ImprimeEstatisticas(arquivoEstatisticas, estatisticasA, tirosB);
    fprintf(arquivoEstatisticas, "\n");
    ImprimeEstatisticas(arquivoEstatisticas, estatisticasB, tirosA);
}
void ImprimeEstatisticas(FILE * estatisticas, tEstatisticas dados, tAcertos * tiros){
    int i=0, j=0;
    fprintf(estatisticas, "%s\n", dados.jogador);
    fprintf(estatisticas, "Tiros Errados: %d\n", dados.tirosErrados);
    fprintf(estatisticas, "Tiros Acertados: %d\n", dados.tirosAcertados);
    fprintf(estatisticas, "Localizacao Media: (%.2f,%.2f)\n", dados.mediaOrdenada, dados.mediaAbscissa);
    fprintf(estatisticas, "Desvio Padrao da Localizacao: %.2f\n", dados.desvioPadrao);
    fprintf(estatisticas, "Navios de %s:\n", dados.jogador);
    for(i=0; i<dados.numeroExtra; i++){
        if(tiros[i].quandoAcertou != 0){
            if(!(VerificaIdentidadeIgualAntes(i, tiros))){
                fprintf(estatisticas, "%s\n", dados.naviosAtingidos[i]);
            }
        }
    }
}
void StringHomogenea(int x, char string[x]){
    int i=0;
    for(i=0; i<x; i++) string[i] = '.';
}
void OrdenaCrescente(int qtd, tAcertos * linhas){
    int i, j, k;
    tAcertos aux;
    for(i=0; i<qtd; i++){
        if(linhas[i].quandoAcertou == 0) continue;
        for(j=i+1; j<qtd; j++){
            if(strcmp(linhas[i].stringTipo, linhas[j].stringTipo) > 0){ //se o que está em i for menor que o que está em j
                aux = linhas[i];
                linhas[i] = linhas[j];
                linhas[j] = aux;
            }
            else if( ! (strcmp(linhas[i].stringTipo, linhas[j].stringTipo) ) ){
                if(linhas[i].quandoAcertou > linhas[j].quandoAcertou){
                    aux = linhas[i];
                    linhas[i] = linhas[j];
                    linhas[j] = aux;
                }
            }

        }
    }
}
int VerificaIdentidadeIgualAntes(int n, tAcertos * linhas){
    int x=0;
    for(x=0; x < n; x++){
        if(linhas[x].identidade == linhas[n].identidade) return 1;
    }
    return 0;
}
void FormaStringComNumeros(char * string, int rodada, tAcertos linha){
    char ID[20];
    if(linha.quandoAcertou == 0){
        strcpy(string, "Agua");
        strcat(string, " - ");
        strcat(string, linha.stringTipo);
    }
    else{
        if(linha.quandoAcertou < 10) sprintf(string, "0%d", linha.quandoAcertou);
        else sprintf(string, "%d", linha.quandoAcertou);
        strcat(string, " - ");
        strcat(string, linha.stringTipo);
        strcat(string, " - ID ");
        if(linha.identidade < 10) sprintf(ID, "0%d", linha.identidade);
        else sprintf(ID, "%d", linha.identidade);
        strcat(string, ID);
    }
}
tEstatisticas InicializaEstatisticas(){
    tEstatisticas dados;
    dados.tirosAcertados = 0;
    dados.tirosErrados = 0;
    dados.numeroExtra = 0;
    dados.mediaAbscissa = 0.0;
    dados.mediaOrdenada = 0.0;
    dados.desvioPadrao = 0.0;
    MatrizHomogenea(100, 50, dados.naviosAtingidos);
    StringHomogenea(16, dados.jogador);
    return dados;
}