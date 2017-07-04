
// classe candidato
// como visto na especificação

typedef struct candidato_t {
    int codigo_votacao;
    char *nome_candidato;
    char *partido;
    int num_votos;
} candidato_t;


/**
 * lista com todos os candidatos
 */
candidato_t candidatos[] = {
    { .codigo_votacao = 13, .nome_candidato = "Lula", .partido = "PT", .num_votos = 0 },
    { .codigo_votacao = 51, .nome_candidato = "Pirassununga", .partido = ":D", .num_votos = 0 },
    { .codigo_votacao = 45, .nome_candidato = "Aécio", .partido = "PSDB", .num_votos = 0 }
};
int qntCandidatos = sizeof(candidatos) / sizeof(candidato_t);


/**
 * imprime um candidato, ou seja, toString()
 */
void printCandidato(candidato_t *c) {
    printf("Codigo: %d, Nome: %s, Partido: %s\n", c->codigo_votacao, c->nome_candidato, c->partido);
}