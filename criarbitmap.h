/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Arquivo de cabeçalho da biblioteca para criar imagens do tipo bitmap (.bmp) *
 *                                                                             *
 * Autor: Vitor Henrique Andrade Helfensteller Satraggiotti Silva              *
 * Iniciado em: 18/04/2020                                                     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/***************************** MACROS *****************************************/
//resolução em pixel por metro (39,3701 * DPI)
#define RESOLUCAO_X   2835
#define RESOLUCAO_Y   2835
//numero de cores utilizadas na tabela
#define TABELA_COMPLETA  0
#define SEM_TABELA       0

/************************ REDEFINIÇÕES DE RÓTULOS *****************************/
//redefinição de rotulos das estruturas
typedef struct header                header_t;
typedef struct pixel_tabela_cores    pixeltabela_t;
typedef struct pixel_24bpp           pixel_t;

/******************************* FUNÇÕES ***************************************/
//inicializando arquivo de imagem (retorna estrutura de cabeçalho)
header_t inicializa_bmp(unsigned short int Precisao, unsigned int Largura, unsigned int Altura, unsigned int NumCoresTabela);
//retorna a tabela de cores em forma de vetor após receber vetor de cores
pixeltabela_t* tabela_cores(header_t cabecalho, pixel_t* Cores)

/*************************** ESTRUTURAS ***************************************/
//configura temporariamente o alinhamento de memoria para 1 byte
#pragma pack(push, 1)
//estrutura de cabeçalho do arquivo (14 bytes)
struct header{
  unsigned char IdentidadeChar_1;           //Código para identificação de arquivo BMP
  unsigned char IdentidadeChar_2;           //Código para identificação de arquivo BMP
  unsigned int TamanhoArquivo;              //Tmanho total do arquivo (headers + tabela + matriz pixel) em byte
  unsigned short int Reservado_1;           //Espaço reservado para utilização por outros programas
  unsigned short int Reservado_2;           //Espaço reservado para utilização por outros programas
  unsigned int OffsetMatrizPixel;           //Desvio em bytes do inicio da matriz de pixel (header_1 + header_2 + tabela)
  //------------------------------------------------------------------------------------
  //estrutura de caeçalho da imagem versão BITMAPINFOHEADER (40 bytes)
  unsigned int TamanhoHeader2;            //Tamanho do header_2 em bytes
  unsigned int Largura;                   //Largura da imagem em pixel (esquerda ==> direita)
  unsigned int Altura;                    //Altura da imagem em pixel (positivo de baixo ==> cima)
  unsigned short int Planos;              //Numero de planos de cor sendo utilizados
  unsigned short int Precisao;            //Quantos Bits por pixel (1, 4, 8, 16, 24, 32)
  unsigned int Compressao;                //Tipo de compressão utilizada (0 ==> sem compressão)
  unsigned int TamanhoMatrizPixel;        //Tamanho em bytes da matriz de pixel incluindo padding
  unsigned int ResolucaoX;                //Resolução de impressão em pixel por metro (39,3701 * DPI) em X
  unsigned int ResolucaoY;                //Resolução de impressão em pixel por metro (39,3701 * DPI) em Y
  unsigned int NumCoresTabela;            //Numero de cores presente na tabela de cores (usado quando bpp<=8 e =0 se usar todas)
  unsigned int CoresImportantes;          //Numero de cores importantes (normalmente zero)
};

//estrutura de pixel de 24 bits para tabela de cores
struct pixel_tabela_cores{
  unsigned char Vermelho;
  unsigned char Verde;
  unsigned char Azul;
  unsigned char Reservado;
};

//estrutura de pixel de 24 bits
struct pixel_24bpp{
  unsigned char Vermelho;
  unsigned char Verde;
  unsigned char Azul;
};
#pragma pack(pop)
