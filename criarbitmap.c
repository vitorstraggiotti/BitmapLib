/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Codigo fonte da biblioteca para criar imagens do tipo bitmap (.bmp)         *
 *                                                                             *
 * Autor: Vitor Henrique Andrade Helfensteller Satraggiotti Silva              *
 * Iniciado em: 18/04/2020                                                     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//undigned char = 1
//unsigned short int  = 2
//unsignde int = 4
#include<stdlib.h>
#include<stdio.h>
#include"criarbitmap.h"

/********************************************************************************/
//função que inicializa o arquivo de imagem retornando o cabeçalho
header_t inicializa_bmp(unsigned short int Precisao, unsigned int Largura, unsigned int Altura, unsigned int NumCoresTabela){
  header_t cabecalho;
  unsigned int BytesTabela;       //tamanho em bytes da tabela de cores

  //verificação de tamanho das variaveis
  if(sizeof(unsigned char) != 1){
    printf("Erro ao criar arquivo de cabecalho! Sem compatibilidade nas variaveis do sistema.\n");
    exit(1);
  }elseif(sizeof(unsigned short int) != 2){
    printf("Erro ao criar arquivo de cabecalho! Sem compatibilidade nas variaveis do sistema.\n");
    exit(1);
  }elseif(sizeof(unsigned int) != 4){
    printf("Erro ao criar arquivo de cabecalho! Sem compatibilidade nas variaveis do sistema.\n");
    exit(1);
  }

  //Preenchendo valores fixos do cabeçalho
  //----------------------------------------------------------------------------
  cabecalho.IdentidadeChar_1 = 'B';
  cabecalho.IdentidadeChar_2 = 'M';
  cabecalho.Reservado_1 = 0;
  cabecalho.Reservado_2 = 0;
  cabecalho.TamanhoHeader2 = 40;
  cabecalho.Largura = Largura;     ////impor limites as dimenções da imagem
  cabecalho.Altura = Altura;
  cabecalho.Planos = 1;
  cabecalho.Compressao = 0;
  cabecalho.ResolucaoX = RESOLUCAO_X;
  cabecalho.ResolucaoY = RESOLUCAO_Y;
  cabecalho.CoresImportantes = 0;

  //verificação de entrada de variavel "Precisão"
  if((Precisao==1)||(Precisao==4)||(Precisao==8)||(Precisao==16)||(Precisao==24)){
    cabecalho.Precisao = Precisao;
  }else{
    printf("Erro! Valor de bits por pixel incorreto.\n");
    exit(1);
  }
  //verificação de entrada de variavel "NumCoresTabela"
  if(((Precisao==16)||(Precisao==24)) && (NumCoresTabela!=0)){
    printf("Erro! 16 e 24 bits por pixel nao aceita tabela de cores.\n");
    exit(1);
  }elseif((Precisao==1) && (NumCoresTabela>2)){
    printf("Erro! 1 bit por pixel não aceita mais que 2 cores na tabela.\n");
    exit(1);
  }elseif((Precisao==4) && (NumCoresTabela>16)){
    printf("Erro! 4 bits por pixel não aceita mais que 16 cores na tabela.\n");
    exit(1);
  }elseif((Precisao==8) && (NumCoresTabela>256)){
    printf("Erro! 8 bits por pixel não aceita mais que 256 cores na tabela.\n");
    exit(1);
  }else{
    cabecalho.NumCoresTabela = NumCoresTabela;
  }

  //Preenchendo valores calculaveis do cabeçalho
  //----------------------------------------------------------------------------
  //Encontrando tamanho da tabela de cores
  if(NumCoresTabela==0){
    if(Precisao==1) BytesTabela = 8;
    if(Precisao==4) BytesTabela = 64;
    if(Precisao==8) BytesTabela = 1024;
    if(Precisao==16) BytesTabela = 0;
    if(Precisao==24) BytesTabela = 0;
  }else{
    BytesTabela = 4 * NumCoresTabela;
  }
  cabecalho.OffsetMatrizPixel = 54 + BytesTabela;

  //Encontrando tamanho da matriz de pixel
  unsigned int BitsLinha;         //tamanho em bits de uma linha da imagem
  BitsLinha = Largura * Precisao; //sem padding
  if((BitsLinha%32)!=0){
    BitsLinha = BitsLinha + 32 - (BitsLinha%32); //adicionando bits para alinhamento de memoria
  }
  cabecalho.TamanhoMatrizPixel = (BitsLinha/8) * Altura;

  //Encontrando tamanho total do arquivo
  cabecalho.TamanhoArquivo = 54 + BytesTabela + cabecalho.TamanhoMatrizPixel;

  return cabecalho;
}
/*******************************************************************************/
//retorna a tabela de cores em forma de vetor após receber vetor de cores
pixeltabela_t* tabela_cores(header_t cabecalho, pixel_t* Cores){
  //cabecalho.NumCoresTabela;
  //cabecalho.Precisao;
  unsigned int Aux;
  pixeltabela_t *TabelaCores;
  if((cabecalho.Precisao==16)||(cabecalho.Precisao==24)){
    printf("Erro! A funcao \"tabela_cores\" nao pode ser chamada se precisao for 16 ou 24 bpp\n");
    exit(1);
  }
  //ajustando variavel de auxiliar de iteração
  if(cabecalho.NumCoresTabela==0){
    if(cabecalho.Precisao==1) Aux = 2;
    if(cabecalho.Precisao==4) Aux = 16;
    if(cabecalho.Precisao==8) Aux = 256;
  }else{
    Aux = cabecalho.NumCoresTabela;
  }
  //alocando memoria para tabela de Cores
  TabelaCores = (pixeltabela_t*)malloc(Aux * sizeof(pixeltabela_t));
  //construindo tabela de cores
  for(unsigned int i=0; i<Aux; i++){
    TabelaCores[i].Vermelho = Cores[i].Vermelho;
    TabelaCores[i].Verde = Cores[i].Verde;
    TabelaCores[i].Azul = Cores[i].Azul;
    TabelaCores[i].Reservado = 0;
  }
}
/*******************************************************************************/
//cria a imagem com base no cabeçalho tabela e matriz de pixel informada
void 
