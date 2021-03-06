#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct 
{
    char tipo[1000]; 
    int identidade;
    int orientacao;
    char linhas;
    int colunas;
    int tipoNumerico;
    int atirou;
} tNavio;

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

//funcoes mais importantes da main, mas nao de diretorio nem de inicializacao de variavel (estao em ordem de aparicao na main)
int ContaLinhas(FILE * tabu);
void LeEmbarcados(FILE * tabu, tNavio * navios);
void MontaTabuleiro(tNavio * navios, int tabuleiro[10][10], int n);
void ValidandoTabuleiro(char * nomeArquivo, tNavio * navios, int tabuleiro[10][10], int n, FILE * tabu_verificado);
void ComparaFrota(tNavio * naviosUm, tNavio * naviosDois, int n, int m, FILE * tabu_verificado);
void TabuleiroInicializado(FILE * tabuFinal, int tabuleiro[10][10]);
void TomadaDecisao(char resultante[10][10], char jogador[16], char * decisao);
void Lancamento(char * decisao, int qtdJogadas, char jogadas[100][50], int tabuleiro[10][10], char resultante[10][10], tNavio * navios, int n);
void ImprimirTabuleiroBuffer(char resultante[10][10]);
int Ganhou(tNavio * navios, int n);
void ImprimeJogadas_noArquivo(FILE * resultado, int x, char jogadas[100][50], char * jogador);
void CalculaEstatisticas(tEstatisticas estatisticasA, tEstatisticas estatisticasB, FILE * resultado, FILE * arquivoEstatisticas, char * jogadorA, char * jogadorB);
//funcoes de utilidades gerais (a partir daqui, funcoes em ordem alfabetica)
char PosicaoLinhaNavio(tNavio coisas);
int PosicaoColunaNavio(tNavio coisas); //aqui eu evitei a ordem alfabetica porque eh muito relacionado com o de cima
int IdentidadeNavio(tNavio coisas);
int LinhasIguais(tNavio navio1, tNavio navio2);
int OrientacaoNavio(tNavio coisas);
int TipoNumericoNavio(tNavio coisas);
int VerificaTipo(tNavio coisas);
tNavio LeCoisas(char * dados);
void FormaArrayZerada(int i, int array[i]);
void FormaTabuleiroZerado(int tabuleiro[10][10]);
void MatrizHomogenea(int x, int y, char matriz[x][y]);
void ReordenaNavios(tNavio * navios, int n);
void StringHomogenea(int x, char string[x]);
//inicialicao e validacao do tabuleiro
int VerificaTabuleiro(tNavio * navios, int tabuleiro[10][10], int n);
void ColocaCoisas(tNavio coisas, int tabuleiro[10][10]);
//jogando
int TirosNoNavio(tNavio navios);
tNavio AtingiuNavio(tNavio navios);
void RegistrandoTiros(tNavio navios, char * jogadas);
//o jogo acabou (estatisticas e resultado)
int VerificaIdentidadeIgualAntes(int n, tAcertos * linhas);
tAcertos FormandoStringTiros(tAcertos tiros, char * acertou, char * atingiu, int n);
tEstatisticas AcertouErrouTiros(tEstatisticas dados, int total, tAcertos * linha);
tEstatisticas CalculaDesvioPadrao(tEstatisticas dados, int total, char linhas[total], int colunas[total]);
tEstatisticas FormaStringComNumeros(tEstatisticas dados, int total, tAcertos * linha);
tEstatisticas InicializaEstatisticas(char * jogador);
tEstatisticas NumeroExtra(tEstatisticas dados, int total);
tEstatisticas MediasCalculadas(tEstatisticas dados, float totalLinhas, float totalColunas, int total);
void ImprimeEstatisticas(FILE * estatisticas, tEstatisticas dados, tAcertos * tiros);
void OrdenaCrescente(int qtd, tAcertos * linhas);
//para testes, funcoes relacionadas ao diretorio
void TestandoTotal(char * diretorio, char * nomeArquivo); 
void RemoveDiretorio(char * nomeArquivo);
void RemovePrimeiraBarra(int argc, char * argv[], char * diretorio);
//parte bonus
void FormaArquivoDeTabuleiro(int argc, char * argv[argc]);

int main(int argc, char * argv[]){
    //diretorio
    if(argc <= 1){
        printf("ERRO: O diretorio de arquivos de configuracao nao foi informado\n");
        return 1;
    }
    FormaArquivoDeTabuleiro(argc, argv);
    //para adequar o diretorio desejado e deixar referenciado em um unico lugar
    int i=0;
    char diretorio[1000];
    RemovePrimeiraBarra(argc, argv, diretorio);
    //printf("%s\n", diretorio);
    char nomeArquivo1[100] = "tabu_1.txt";
    char nomeArquivo2[100] = "tabu_2.txt";
    char fraseVerificada[100] = "saida/validacao_tabuleiros.txt"; 
    char fraseInicializada[100] = "saida/inicializacao.txt";
    char fraseResultado[100] = "saida/resultado.txt";
    char fraseEstatisticas[100]= "saida/estatisticas.txt";

    TestandoTotal(diretorio, nomeArquivo1);
    TestandoTotal(diretorio, nomeArquivo2);
    TestandoTotal(diretorio, fraseVerificada);
    TestandoTotal(diretorio, fraseInicializada);
    TestandoTotal(diretorio, fraseResultado);
    TestandoTotal(diretorio, fraseEstatisticas);

    FILE * tabu_1 = fopen(nomeArquivo1, "r");
    if( ! (tabu_1) ){
        printf("tabu_1.txt nao encontrado no diretorio ");
        for(i=0; i<argc; i++){
            printf("%s", argv[i]);
        }
        printf("\n");
        return 1;
    }

    FILE * tabu_2 = fopen(nomeArquivo2, "r");
    if( ! (tabu_2) ){
        printf("tabu_2.txt nao encontrado no diretorio ");
        for(i=0; i<argc; i++){
            printf("%s", argv[i]);
        }
        printf("\n");
        return 1;
    }
    
    int n = ContaLinhas(tabu_1);
    int m = ContaLinhas(tabu_2);
    fclose(tabu_1);
    fclose(tabu_2);
     
    tNavio naviosUm[n];
    tNavio naviosDois[m];
    int tabuleiro1[10][10], tabuleiro2[10][10]; 
    tabu_1 = fopen(nomeArquivo1, "r");
    tabu_2 = fopen(nomeArquivo2, "r");
    LeEmbarcados(tabu_1, naviosUm);
    LeEmbarcados(tabu_2, naviosDois);
    fclose(tabu_1); fclose(tabu_2);
    //montando tabuleiro numerico para compara????es futuras de posicionamento e para ser imutavel
    MontaTabuleiro(naviosUm, tabuleiro1, n);
    MontaTabuleiro(naviosDois, tabuleiro2, m);

    //verificando validade dos tabuleiros 
    FILE * verificando = fopen(fraseVerificada, "w");
    ValidandoTabuleiro(nomeArquivo1, naviosUm, tabuleiro1, n, verificando);
    fprintf(verificando, "\n");
    ValidandoTabuleiro(nomeArquivo2, naviosDois, tabuleiro2, m, verificando);
    fclose(verificando);
    //comparando os tabuleiros/navios colocados
    verificando = fopen(fraseVerificada, "a+");
    ComparaFrota(naviosUm, naviosDois, n, m, verificando);
    fclose(verificando);

    //jogadores
    char jogador1[16];
    char jogador2[16];
    printf("Nome do Jogador 1:\n");
    scanf("%s%*c", jogador1);
    printf("Nome do Jogador 2:\n\n");
    scanf("%s%*c", jogador2);

    //inicializando
    FILE * inicializando = fopen(fraseInicializada, "w");
    fprintf(inicializando, "%s\n", jogador1);
    TabuleiroInicializado(inicializando, tabuleiro1);
    fprintf(inicializando, "\n\n%s\n", jogador2);
    TabuleiroInicializado(inicializando, tabuleiro2);
    fclose(inicializando);

    //jogando
    int ganhou1=0, ganhou2=0, qtdJogadas=0;
    char jogada[100];
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
    FILE * resultado = fopen(fraseResultado, "w");
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
    FILE * arquivoEstatisticas = fopen(fraseEstatisticas, "w");
    resultado = fopen(fraseResultado, "r"); //manipular um arquivo que o proprio programa montou pareceu melhor
    CalculaEstatisticas(dados1, dados2, resultado, arquivoEstatisticas, jogador1, jogador2);
    fclose(resultado);
    fclose(arquivoEstatisticas);

    return 0;
}

//funcoes mais importantes da main, mas nao de diretorio nem de inicializacao de variavel 
int ContaLinhas(FILE * tabu){
    int i=0;
    char escrito[1000];
    while( ! (feof(tabu)) ){
        fscanf(tabu, "%[^\n]%*[\n]", escrito);
        i++;
    }
    return i;
}
void LeEmbarcados(FILE * tabu, tNavio * navios){
    char escrito[1000];
    int i=0;
    while( ! (feof(tabu)) ){
        fscanf(tabu, "%[^\n]%*[\n]", escrito);
        navios[i] = LeCoisas(escrito);
        i++;
    }
    ReordenaNavios(navios, i);
}
void MontaTabuleiro(tNavio * navios, int tabuleiro[10][10], int n){
    int x=0;
    FormaTabuleiroZerado(tabuleiro);
    for(x=0; x<n; x++){
        ColocaCoisas(navios[x], tabuleiro);
    }
}
void ValidandoTabuleiro(char * nomeArquivo, tNavio * navios, int tabuleiro[10][10], int n, FILE * tabu_verificado){
    //verificando se ?? valido ou n??o
    RemoveDiretorio(nomeArquivo);
    fprintf(tabu_verificado, "%s", nomeArquivo);
    if(VerificaTabuleiro(navios, tabuleiro, n)) fprintf(tabu_verificado, "%s", ";valido");
    else fprintf(tabu_verificado, "%s", ";invalido");
}
void ComparaFrota(tNavio * naviosUm, tNavio * naviosDois, int n, int m, FILE * tabu_verificado){
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
            qtdTipo1[x-1]++;
        }
        for(i=0; i<m; i++){
            x = VerificaTipo(naviosDois[i]);
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
void TabuleiroInicializado(FILE * tabuFinal, int tabuleiro[10][10]){
    int i, j;
    for(i=0; i<10; i++){
        for(j=0; j<10; j++){
            if(tabuleiro[i][j] != 0){
                //char criarLinha = 'a' + i;
                //int criarColuna = j + 1;
                //fprintf(tabuFinal, "%d", tabuleiro[i][j]);
                fprintf(tabuFinal, "X");
            }
            else fprintf(tabuFinal, "%c", 'o');
            if(j != 9) fprintf(tabuFinal, " ");
        }
        if(i != 9) fprintf(tabuFinal,"\n");
    }
}
void TomadaDecisao(char resultante[10][10], char jogador[16], char * decisao){
    char ordenada='\0'; int abscissa=0; char restante='\0';
    int a=0;
    int b=0;
    int i=0, j=0, x=0;
    printf("Jogada de %s:\n\n", jogador);
    while(1){
        x=0;
        scanf("%s%*[^\n]", decisao);
        sscanf(decisao, "%c%d%c", &ordenada, &abscissa, &restante);

        if((restante != '\0' && restante != '\n')  || ordenada < 'a' || ordenada > 'j' || 
        abscissa < 1 || abscissa > 10) x = 1; //se o comando n??o est?? nos conformes

        a = ordenada - 'a';
        b = abscissa - 1;
        if(resultante[a][b] != '.') x = 1;

        if(x == 1){
            printf("%s:Jogada invalida! Jogue novamente %s:\n\n", decisao, jogador);
            restante = '\0'; ordenada = '\0'; abscissa = 0; //reiniciando as coisas
        }
        else{
            break;
        }
    }
}
void Lancamento(char * decisao, int qtdJogadas, char jogadas[100][50], int tabuleiro[10][10], char resultante[10][10], tNavio * navios, int n){
    char linha='\0'; int coluna=0;
    int i=0, j=0;
    sscanf(decisao, "%c%d", &linha, &coluna);
    int a = linha - 'a';
    int b = coluna - 1;
    printf("%s:", decisao);
    strcpy(jogadas[qtdJogadas], decisao);
    if(tabuleiro[a][b] == 0){
        resultante[a][b] = 'o';
        printf("Agua\n");
        strcat(jogadas[qtdJogadas], " - Agua\0");
    }
    else{
        while(IdentidadeNavio(navios[i]) != tabuleiro[a][b]) i++;
        navios[i] = AtingiuNavio(navios[i]);
        resultante[a][b] = 'X';
        RegistrandoTiros(navios[i], jogadas[qtdJogadas]);
        if(TirosNoNavio(navios[i]) == TipoNumericoNavio(navios[i])){
            printf("Afundou %s\n", navios[i].tipo);
        }
        else printf("Tiro!\n");
    }
    printf("\n");
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
int Ganhou(tNavio * navios, int n){
    int i=0, x=0;
    for(i=0; i<n; i++){
        if(TirosNoNavio(navios[i]) != TipoNumericoNavio(navios[i])) return 0;
    }
    return 1;
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
    int total=0, i=0;
    char linhasA[100]; int colunasA[100];
    char linhasB[100]; int colunasB[100];
    int totalLinhasA=0, totalColunasA=0, totalLinhasB=0, totalColunasB=0;
    float somatorioDesvioA=0.0, somatorioDesvioB=0.0;
    float subtracaoLinhasA=0.0, subtracaoColunasA=0.0;
    float subtracaoLinhasB=0.0, subtracaoColunasB=0.0;

    estatisticasA = InicializaEstatisticas(jogadorA);
    estatisticasB = InicializaEstatisticas(jogadorB);
    
    while(1){
        colunasA[total] = 0; //inicializando o valor
        linhasA[total] = '\0';
        fscanf(resultado, "%[^\n]%*[\n]", string);

        if(!(strcmp(jogadorA, string))) continue;
        if(!(strcmp(jogadorB, string))) break;
        
        strcpy(casoAtinja, string);
        sscanf(string, "%c%d %*c %[^ ]%*s%*[\n]", &linhasA[total], &colunasA[total], acertou);

        totalLinhasA += linhasA[total] - 'a' + 1;
        totalColunasA += colunasA[total];

        tirosA[total] = FormandoStringTiros(tirosA[i], acertou, casoAtinja, total);
        
        total++; //vai determinar o final do loop da segunda parte do arquivo
    }
    
    //fecha bloco da primeira estat??stica

    for(i=0; i<total; i++){
        colunasB[i] = 0;
        linhasB[i] = '\0';
        fscanf(resultado, "%[^\n]%*[\n]", string);

        strcpy(casoAtinja, string);
        sscanf(string, "%c%d %*c %[^ ]%*s%*[\n]", &linhasB[i], &colunasB[i], acertou);

        totalColunasB += colunasB[i];
        totalLinhasB += linhasB[i]  - 'a' + 1;

        tirosB[i] = FormandoStringTiros(tirosB[i], acertou, casoAtinja, i);
    }
    estatisticasA = MediasCalculadas(estatisticasA, totalLinhasA, totalColunasA, total);
    estatisticasB = MediasCalculadas(estatisticasB, totalLinhasB, totalColunasB, total);

    estatisticasA = NumeroExtra(estatisticasA, total); //atribuindo total ao tipo de dado tEstatistica
    estatisticasB = NumeroExtra(estatisticasB, total);
    estatisticasA = FormaStringComNumeros(estatisticasA, total, tirosB); //strings dos navios atingidos
    estatisticasB = FormaStringComNumeros(estatisticasB, total, tirosA);

    estatisticasA = AcertouErrouTiros(estatisticasA, total, tirosA); //agora o tipo de dados serve para o mesmo referencial
    estatisticasB = AcertouErrouTiros(estatisticasB, total, tirosB);

    estatisticasA = CalculaDesvioPadrao(estatisticasA, total, linhasA, colunasA);
    estatisticasB = CalculaDesvioPadrao(estatisticasB, total, linhasB, colunasB);
    
    ImprimeEstatisticas(arquivoEstatisticas, estatisticasA, tirosB);
    fprintf(arquivoEstatisticas, "\n");
    ImprimeEstatisticas(arquivoEstatisticas, estatisticasB, tirosA);
}
//funcoes mais gerais
int LinhasIguais(tNavio navio1, tNavio navio2){
    int qtdIguais=0;
    if(!(strcmp(navio1.tipo, navio2.tipo))){
        if(IdentidadeNavio(navio1) == IdentidadeNavio(navio2)){
            if(PosicaoLinhaNavio(navio1) == PosicaoLinhaNavio(navio2)){
                if(PosicaoColunaNavio(navio1) == PosicaoColunaNavio(navio2)){
                    return 1;
                }
            }
        }
    }
    return 0;
}
void FormaArrayZerada(int i, int array[i]){
    int j=0;
    for(j=0; j < i; j++) array[j] = 0;
}
void FormaTabuleiroZerado(int tabuleiro[10][10]){
    int i=0, j=0;
    for(i=0; i<10; i++){
        for(j=0; j<10; j++) tabuleiro[i][j] = 0;
    }
}
void MatrizHomogenea(int x, int y, char matriz[x][y]){
    int i=0, j=0;
    for(i=0; i<x; i++){
        for(j=0; j<y; j++){
            matriz[i][j] = '.';
        }
    }
}
void StringHomogenea(int x, char string[x]){
    int i=0;
    for(i=0; i<x; i++) string[i] = '.';
}
int VerificaTipo(tNavio coisas){
    if(!(strcmp(coisas.tipo, "Carrier"))) return 5;
    else if(!(strcmp(coisas.tipo, "Battleship"))) return 4;
    else if(!(strcmp(coisas.tipo, "Cruiser"))) return 3;
    else if(!(strcmp(coisas.tipo, "Destroyer"))) return 2;
    else if(!(strcmp(coisas.tipo, "Submarine"))) return 1;
    else return 0;
}
void ReordenaNavios(tNavio * navios, int n){
    int i=0, j=0, k=0, l=0;
    tNavio aux;
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
tNavio LeCoisas(char * dados){
    tNavio coisas;
    int i=0;
    coisas.tipoNumerico = 0;
    coisas.atirou = 0;
    sscanf(dados, "%[^;]%*c%d%*c%d%*c%c%d", coisas.tipo, &coisas.identidade, 
    &coisas.orientacao, &coisas.linhas, &coisas.colunas);

    coisas.tipoNumerico = VerificaTipo(coisas);

    if(coisas.tipoNumerico == 1) coisas.tipoNumerico = 3;

    return coisas;
}
int TipoNumericoNavio(tNavio coisas){
    return coisas.tipoNumerico;
}
int IdentidadeNavio(tNavio coisas){
    return coisas.identidade;
}
int OrientacaoNavio(tNavio coisas){
    return coisas.orientacao;
}
char PosicaoLinhaNavio(tNavio coisas){
    return coisas.linhas;
}
int PosicaoColunaNavio(tNavio coisas){
    return coisas.colunas;
}
//inicialicao e validacao do tabuleiro
void ColocaCoisas(tNavio coisas, int tabuleiro[10][10]){
    int a= PosicaoLinhaNavio(coisas) - 'a';
    int b= PosicaoColunaNavio(coisas) - 1;
    int i=0, j=0;
    int x = TipoNumericoNavio(coisas);

    if(OrientacaoNavio(coisas) == 0){
        for(i=a; i < a+x; i++) tabuleiro[i][b] = IdentidadeNavio(coisas); //extraindo do dado estruturado
        
    }
    else if(OrientacaoNavio(coisas) == 1){
        for(j=b; j < b+x; j++) tabuleiro[a][j] = IdentidadeNavio(coisas);
    }
}
int VerificaTabuleiro(tNavio * navios, int tabuleiro[10][10], int n){
    int i=0, j=0, x=0, estaAli=0;
    for(x=0; x<n; x++){
        if(PosicaoLinhaNavio(navios[x]) < 'a' || PosicaoLinhaNavio(navios[x]) > 'j') return 0;
        if(PosicaoColunaNavio(navios[x]) < 1 || PosicaoColunaNavio(navios[x]) > 10) return 0;
        if(TipoNumericoNavio(navios[x]) == 0) return 0;
        if(OrientacaoNavio(navios[x]) == 0){
            if(PosicaoLinhaNavio(navios[x]) + TipoNumericoNavio(navios[x]) - 1 > 'j') return 0;
        }
        else if(OrientacaoNavio(navios[x]) == 1){
            if(PosicaoColunaNavio(navios[x]) + TipoNumericoNavio(navios[x]) - 1 > 10) return 0;
        }
    }

    for(i=0; i<10; i++){
        for(j=0; j<10; j++){
            if(tabuleiro[i][j] == 0) continue;
            if(i>0 && tabuleiro[i-1][j] != 0 && tabuleiro[i-1][j] != tabuleiro[i][j]) return 0; //verificando o que t?? em cima
            if(i<9 && tabuleiro[i+1][j] != 0 && tabuleiro[i+1][j] != tabuleiro[i][j]) return 0; //verificando o que t?? em baixo
            if(j>0 && tabuleiro[i][j-1] != 0 && tabuleiro[i][j-1] != tabuleiro[i][j]) return 0; //?? esquerda
            if(j<9 && tabuleiro[i][j+1] != 0 && tabuleiro[i][j+1] != tabuleiro[i][j]) return 0; //?? direita
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
                if(tabuleiro[i][j] == IdentidadeNavio(navios[x])){
                    estaAli = 1; //se o navio existe ali, ele nao esta inteiramente em baixo de outro
                    break;
                }
            }
            if(estaAli == 1) break;
        }
        if(estaAli == 0) return 0;
        if(LinhasIguais(navios[x], navios[x+1])) return 0;
    }
    return 1;
}
//jogando
tNavio AtingiuNavio(tNavio navios){
    navios.atirou++;
    return navios;
}
int TirosNoNavio(tNavio navios){
    return navios.atirou;
}
void RegistrandoTiros(tNavio navios, char * jogadas){
    strcat(jogadas, " - Tiro - ");
    strcat(jogadas, navios.tipo); //nao sei usar funcao que retorne string
    strcat(jogadas, " - ID ");
    if(IdentidadeNavio(navios) < 10) strcat(jogadas, "0");
    char ID[4];
    sprintf(ID, "%d", IdentidadeNavio(navios));
    strcat(ID, "\0");
    strcat(jogadas, ID);
}
//o jogo acabou
int VerificaIdentidadeIgualAntes(int n, tAcertos * linhas){
    int x=0;
    for(x=0; x < n; x++){
        if(linhas[x].identidade == linhas[n].identidade) return 1;
    }
    return 0;
}    
tAcertos FormandoStringTiros(tAcertos tiros, char * acertou, char * atingiu, int n){
    if( ! (strcmp(acertou, "Tiro") ) ){
        sscanf(atingiu, "%*[^ ] - %*[^ ] - %[^ ] - ID %d%*c", 
                tiros.stringTipo, 
                &tiros.identidade);

        tiros.quandoAcertou = n+1;
    }
    else if( ! strcmp(acertou, "Agua") ){
        strcpy(tiros.stringTipo, " Sla\0");
        tiros.identidade = 0;
        tiros.quandoAcertou = 0;
    }
    return tiros;
}
tEstatisticas AcertouErrouTiros(tEstatisticas dados, int total, tAcertos * linha){
    int i=0;
    for(i=0; i<total; i++){
        if(linha[i].quandoAcertou == 0){
            dados.tirosErrados++;
        }
        else dados.tirosAcertados++;
    }
    return dados;
}
tEstatisticas CalculaDesvioPadrao(tEstatisticas dados, int total, char linhas[total], int colunas[total]){
    int i=0;
    float subtracaoLinhas, subtracaoColunas;
    float somatorioDesvio = 0.0;
    for(i=0; i<total; i++){
        subtracaoLinhas = (float) linhas[i] - 'a' + 1 - dados.mediaOrdenada;
        subtracaoLinhas = pow(subtracaoLinhas, 2);
        subtracaoColunas = (float) colunas[i] - dados.mediaAbscissa;
        subtracaoColunas = pow(subtracaoColunas, 2);
        somatorioDesvio += subtracaoColunas + subtracaoLinhas;
    }
    somatorioDesvio /= total;
    somatorioDesvio = sqrt(somatorioDesvio);
    dados.desvioPadrao = somatorioDesvio;
    return dados;
}
tEstatisticas FormaStringComNumeros(tEstatisticas dados, int total, tAcertos * linha){
    char ID[20];
    char string[50];
    int i=0;
    OrdenaCrescente(total, linha);
    for(i=0; i<total; i++){
        if(linha[i].quandoAcertou == 0){
            strcpy(string, "Agua");
            strcat(string, " - ");
            strcat(string, linha[i].stringTipo);
        }
        else{
            if(linha[i].quandoAcertou < 10) sprintf(string, "0%d", linha[i].quandoAcertou);
            else sprintf(string, "%d", linha[i].quandoAcertou);
            strcat(string, " - ");
            strcat(string, linha[i].stringTipo);
            strcat(string, " - ID ");
            if(linha[i].identidade < 10) sprintf(ID, "0%d", linha[i].identidade);
            else sprintf(ID, "%d", linha[i].identidade);
            strcat(string, ID);
        }
        strcpy(dados.naviosAtingidos[i], string);
    }
    return dados;
}
tEstatisticas InicializaEstatisticas(char * jogador){
    tEstatisticas dados;
    strcpy(dados.jogador, jogador);
    dados.tirosAcertados = 0;
    dados.tirosErrados = 0;
    dados.numeroExtra = 0;
    dados.mediaAbscissa = 0.0;
    dados.mediaOrdenada = 0.0;
    dados.desvioPadrao = 0.0;
    MatrizHomogenea(100, 50, dados.naviosAtingidos);
    return dados;
}
tEstatisticas MediasCalculadas(tEstatisticas dados, float totalLinhas, float totalColunas, int total){
    totalLinhas /= (float) total;
    totalColunas /= (float) total;
    dados.mediaAbscissa = totalColunas;
    dados.mediaOrdenada = totalLinhas;
    return dados;
}
tEstatisticas NumeroExtra(tEstatisticas dados, int total){
    dados.numeroExtra = total;
    return dados;
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
            if(!(VerificaIdentidadeIgualAntes(i, tiros))){ //nao eh para printar navios indevidos
                fprintf(estatisticas, "%s\n", dados.naviosAtingidos[i]);
            }
        }
    }
}
void OrdenaCrescente(int qtd, tAcertos * linhas){
    int i, j, k;
    tAcertos aux;
    for(i=0; i<qtd; i++){
        if(linhas[i].quandoAcertou == 0) continue;
        for(j=i+1; j<qtd; j++){
            if(strcmp(linhas[i].stringTipo, linhas[j].stringTipo) > 0){ //se o que est?? em i for menor que o que est?? em j
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
//para testes, funcoes relacionadas ao diretorio
void TestandoTotal(char * diretorio, char * nomeArquivo){
    char soPosicao[50];
    strcpy(soPosicao, diretorio);
    strcat(soPosicao, nomeArquivo); //concatena as strings do diretorio e do arquivo em questao
    strcpy(nomeArquivo, soPosicao); //declara o nome do arquivo
    //printf("%s\n", nomeArquivo);
}
void RemoveDiretorio(char * nomeArquivo){
    char nomeArquivo2[30];
    int i=0, j=0;
    while(nomeArquivo[i] != '\0'){
        if(nomeArquivo[i] == '/'){
            j=0;
            nomeArquivo2[j] = 0;
        }
        i++;
        nomeArquivo2[j] = nomeArquivo[i];
        j++;
    }
    strcpy(nomeArquivo, nomeArquivo2);
    //printf("%s\n", nomeArquivo);
}
void RemovePrimeiraBarra(int argc, char * argv[], char * diretorio){
    int i=0, x=0;
    strcpy(diretorio, argv[1]);
    while(argv[1][i] != '\0'){
        i++;
    }
    for(x=0; x<i; x++){
        diretorio[x] = diretorio[x+1];
    }
    for(i=2; i<argc; i++){
        strcat(diretorio, argv[i]);
    }
    strcat(diretorio, "/");
    //printf("%d\n%s\n", x, diretorio);
}
void FormaArquivoDeTabuleiro(int argc, char * argv[argc]){
    int i=0, x=0, numeroAleatorio=0, orientacaoAleatoria=0, colunaAleatoria=0;
    char linhaAleatoria='\0';
    char diretorio[1000];
    strcpy(diretorio, argv[1]);
    //printf("%s\n", diretorio);
    
    if( ! (strcmp(diretorio, "-gt")) ){
        strcpy(diretorio, argv[2]);
        RemoveDiretorio(diretorio);
        //printf("%s\n", diretorio);
        for(i=3; i<argc; i++) strcat(diretorio, argv[i]);
        //printf("%s\n", diretorio);
        FILE * tabuleiroFormado;
        char string[5][20];
        char caractereEspecial[3];
        tNavio navios[5];
        int tabuleiro[10][10];
        while(1){
            srand(time(NULL)); //para que os valores aleatorios mudem
            
            for(i=0; i<5; i++){
                if(i == 0) strcpy(string[i], "Carrier;1;");
                else if(i == 1) strcpy(string[i], "Battleship;2;");
                else if(i == 2) strcpy(string[i], "Cruiser;3;");
                else if(i == 3) strcpy(string[i], "Submarine;4;");
                else if(i == 4) strcpy(string[i], "Destroyer;5;");
                numeroAleatorio = rand() % 100;
                if(numeroAleatorio % 2 == 0) strcat(string[i], "0;");
                else strcat(string[i], "1;");
                numeroAleatorio = rand() % 9;
                linhaAleatoria = 'a' + numeroAleatorio;
                sprintf(caractereEspecial, "%c", linhaAleatoria);
                strcat(string[i], caractereEspecial);
                numeroAleatorio = rand() % 9;
                colunaAleatoria = numeroAleatorio + 1;
                sprintf(caractereEspecial, "%d", colunaAleatoria);
                strcat(string[i], caractereEspecial);
                //printf("%s\n", string[i]);
                navios[i] = LeCoisas(string[i]);
            }
            MontaTabuleiro(navios, tabuleiro, 5);
            if(VerificaTabuleiro(navios, tabuleiro, 5)) break; //so para quando for valido
            printf("HAHA OTARIO\n"); //comando apenas para entretenimento do programador
            //mentira, nao foi so por isso. O terminal vazio estava me dando agonia
        }
        printf("Viu soh? Nem demorou tanto\n"); //ai vai la e demorou tres horas. Mas o que importa eh que chega a algum lugar
        tabuleiroFormado = fopen(diretorio, "w");
        for(i=0; i<5; i++){
            fprintf(tabuleiroFormado, "%s\n", string[i]);
        }
        fclose(tabuleiroFormado);
        exit(1);
    }
}