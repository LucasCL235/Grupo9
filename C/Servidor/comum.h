#define PORT 40009
#define IP "cosmos.lasdpc.icmc.usp.br"

// um cast que dá menos Warnings
// encontramos na net
#define cast(tipo_t, variavel) ((tipo_t) (intptr_t) variavel)

// função de depuração
// quando estamos testando o código
#ifdef DEBUG
    #define debug(str, ...) printf(str, ##__VA_ARGS__)
#else
    #define debug(str, ...)
#endif