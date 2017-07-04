#include <sys/types.h>   // Definicao de tipos
#include <sys/socket.h>  // Biblioteca de estrutara de sockets
#include <netinet/in.h>  // Define os padroes de protocolo IP
#include <arpa/inet.h>   // Converte enderecos hosts
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <unistd.h> // Define constantes POSIX
#include <errno.h>  // Utilizado para capturar mensagens de erro
#include "candidato.h"
#include "comum.h"



// qnt de votos brancos
int brancos = 0;
// qnt de votos nulos
int nulos = 0;

/**
 * função itoa
 * retorna uma string a partir do inteiro
 * a string deve ser copiada
 */
char *itoa(int i) {
  static char str[12];
  sprintf(str, "%d", i);
  return str;
}

/**
 * struct que contém todas as informações
 * necessárias para a troca de informação
 * pela rede
 */
typedef struct servidor_t {
  int connected;
} servidor_t;

/**
 * função receber do Comunicador
 */
void atualizarMensagem(servidor_t *servidor, char *data) {
  debug("atualizarMensagem()\n");
  int bytes_recv = recv(servidor->connected, data, 1024, 0);
  data[bytes_recv] = '\0';
}

/**
 * função enviar do comunicador
 */
void enviarMensagem(servidor_t *servidor, char *send_data) {
  debug("enviarMensagem()\n");
  debug("send_data: %s\n", send_data);
  // Função send(int socket, void*buffer, size_t size, int flags)
  send(servidor->connected, send_data, strlen(send_data), 0);
}

/**
 * o que acontece quando uma mensagem é recebida
 * dois casos são tratados
 * quando é 999 e 888
 */
void onMessage(servidor_t *servidor, char *data) {
  debug("onMessage\n");
  debug("data: %s\n", data);
  if(strcmp(data, "999") == 0) {
    debug("Enviando lista de candidatos\n");
    // envia primeiro a quantidade
    enviarMensagem(servidor, itoa(qntCandidatos));
    atualizarMensagem(servidor, data); // para travar
    // para cada candidato
    candidato_t *c;
    for(int i=0; i<qntCandidatos; i++) {
      c = &candidatos[i];
      debug("Enviando '%s'\n", c->nome_candidato);

      enviarMensagem(servidor, itoa(c->codigo_votacao));
      atualizarMensagem(servidor, data); // para travar
      enviarMensagem(servidor, c->nome_candidato);
      atualizarMensagem(servidor, data); // para travar
      enviarMensagem(servidor, c->partido);
      atualizarMensagem(servidor, data); // para travar
    }
  }
  else if(strcmp(data, "888") == 0) {
      printf("Urna encerrada!\n");
      enviarMensagem(servidor, "."); // para destravar
      atualizarMensagem(servidor, data); // recebe número de votos brancos
      enviarMensagem(servidor, "."); // para destravar
      int qntBrancos = atoi(data);
      brancos += qntBrancos;
      printf("Brancos: %d\n", brancos);
      atualizarMensagem(servidor, data); // recebe número de votos nulos
      enviarMensagem(servidor, "."); // para destravar
      int qntNulos = atoi(data);
      nulos += qntNulos;
      printf("Nulos: %d\n", nulos);

      // agr pega pra cada candidato
      for(int i=0; i<qntCandidatos; i++) {
          atualizarMensagem(servidor, data); // recebe número do candidato
          enviarMensagem(servidor, "."); // para destravar
          int num = atoi(data);
          atualizarMensagem(servidor, data); // recebe a quantidade de votos
          enviarMensagem(servidor, "."); // para destravar
          int qnt = atoi(data);

          // encontra o candidato
          // e soma os votos nele
          candidato_t *c;
          for(int i=0; i<qntCandidatos; i++) {
            c = &candidatos[i];            
            if(c->codigo_votacao == num) {
                printCandidato(c);
                c->num_votos += qnt;
                printf("Votos: %d\n\n", c->num_votos);
            }
          }
      }
  }
}

/**
 * thread que é executada quando uma nova conexão
 * é feita
 */
void *conexaoRun(void *param) {
  servidor_t *servidor = cast(servidor_t *, param);

  while(1) {
    // faz uma leitura
    // Funcao recv (int socket, void *buffer, size_t size, int flags)        
    char data[1024];
    atualizarMensagem(servidor, data);
    debug("Recebindo de '%d': '%s'\n", servidor->connected, data);

    // se não tem nada dentro, eu saio
    if(data[0] == 0) {
      // ou seja, n recebi nd
      // conexão encerrada
      close(servidor->connected);
      break;
    }

    // trato a leitura
    onMessage(servidor, data);
  }

}


int main(int argc, char *argv[ ])
{
  
   /*
    Estrutura de um socket

    struct sockaddr_in {
           short int sin_family; -> Familia do endereço do socket: 
        1) AF_INET- ARPA INTERNET  PROTOCOLS
        2) AF_UNIX- UNIX INTERNET PROTOCOLS
        3)AF_ISSO- ISO PROTOCOLS
        4)AF_NS- XEROX NETWORK SYSTEM PROTOCOLS
           
       unsigned short int sin_port; -> Numero da porta de comunicacao
           struct in_addr sin_addr; -> Endereco IP
           unsigned char sin_zero[8]; -> Zerar a estrutura do socket - nao utilizado
    
     Um socket (AF_INET) pode ter três tipos de estilo: 
    "SOCK_STREAM" (TCP - Stream de Dados - Stream Socket), 
    "SOCK_DGRAM" (UDP - Datagrama - Datagram Sockets) ou 
    "SOCK_RAW" (Baixo Nivel de Rede, normalmente nao utilizado)
   */

   
   //Variaveis
   int sock, connected, bytes_recv, i, trueValue = 1;
   char send_data [1024] , recv_data[1024];
   struct sockaddr_in server_addr, client_addr;
   int sin_size;

   /* Funcao socket(sin_family,socket_type,protocol_number) retorna um inteiro (socket descriptor), caso erro retorna -1
   
      O numero do protocolo (protocol_number) pode ser algum dos seguintes:
        0 - IP - Internet Protocol (Default)
            1 - ICMP - Internet Control Message Protocol
            2 - IGMP - Internet Group Multicast Protocol
            3 - GGP - Gateway-Gateway Protocol
            6 - TCP - Transmission Control Protocol
            17 - UDP - User Datagram Protocol   
   */ 

   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
     perror("Erro no Socket");
     exit(1);
   }

 
   /* Funcao setsockopt(int socket, int level, int optname, void*optval, size_t optlen)
   
    Esta funcao seta o valor (optval) de uma opcao (optname) em um certo nivel (level) de camada de protocolo no socket
    
    int socket = descriptor do socket
    
    int level = nivel da camada do protocolo (SOL_SOCKET = Constante de nivel para o socket, outros: IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP)
    
    int optname = Opcao desejada para a alteracao
   
    optval = valor da opcao
   
    optlen = tamanho do valor

    Neste exemplo iremos alterar o valor no nivel de socket para a opcao SO_REUSEADDR. Por default um socket criado aceita apenas
    uma conexao por endereco, ou seja o valor de SO_REUSEADDR é igual FALSE (0). Para alterar esse valor e permitirmos que o
    mesmo endereco possa receber varias conexoes devemos alterar o valor da opcao SO_REUSEADDR para TRUE (1).

   */

   if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &trueValue ,sizeof(int)) == -1)
   {
      perror("Erro Setsockopt");
      exit(1);
   }


   // Configura o endereco de destino
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(PORT);
   server_addr.sin_addr.s_addr = INADDR_ANY;
   bzero(&(server_addr.sin_zero),8);


   /* Uma vez com o socket criado precisamos informar o endereço ao socket. Para isso utilizamos a funcao bind
   
    Funcao bind(int socket, struct sockaddr*addr, size_t length)

    int socket = descriptor do socket

    struct sockaddr*addr = endereco de destino

    size_t length = tamanho do endereco

    A funcao bind retorna 0 em caso de sucesso e -1 em caso de erro
   */

   if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
   {
      perror("Nao foi possivel realizar o bind");
      exit(1);
   }


   /* Como estamos criando um servidor que ira receber solicitacoes este socket deve ficar "ouvindo" (aguardando conexoes) na
      porta especificada. A funcao listen realiza essa tarefa.

    Funcao listen(int socket, unsigned int n) onde;
    
    int socket = descriptor do socket

    unsigned int n = tamanho da fila de conexoes pendentes
   
    Obs: Quando utilizamos o listen devemos utilizar a funcao accept que veremos mais adiante no codigo

   */

   if (listen(sock, 10) == -1)
   {
      perror("Erro de Listen");
      exit(1);
   }

   printf("\nServidor TCP esperando por conexoes na porta %d\n", PORT);


   // Loop para receber varias solicitacoes
   while(1)
   {

      // Variavel para armazenar o tamanho de endereco do cliente conectado
      sin_size = sizeof(struct sockaddr_in);

      /* Funcao accept(int socket, struct sockaddr*addr, size_t*length_ptr)
    A funcao accept aceita uma conexao e cria um novo socket para esta conexao
        
    int socket = descriptor do socket
      
    struct sockaddr*addr = endereco de destino (cliente)
      
    size_t*length_ptr = tamanho do endereco de destino

        Obs: A funcao accept por padrão fica aguardando a chegada de um pedido de conexao. Para que ela nao fique, devemos
        configurar o socket no modo sem bloqueio (nonblocking mode set). Neste exemplo ficaremos com o modo padrao (bloqueante)

        A funcao accept retorna  

      */
      
      connected = accept(sock, (struct sockaddr *)&client_addr, &sin_size);
      printf("\nConexão aceita de (%s , %d)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

      servidor_t *servidor = (servidor_t *) malloc(sizeof(servidor_t) * 1);
      servidor->connected = connected;
      pthread_t t;
      pthread_create(&t, NULL, conexaoRun, cast(void *, servidor));
   }
   
   close(sock);
   return 0;
}
