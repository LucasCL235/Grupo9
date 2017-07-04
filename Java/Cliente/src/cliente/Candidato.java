/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cliente;

/**
 *
 * 
 */
public class Candidato {

    // classe candidato
    // como visto na especificação
    public int codigo_votacao;
    public String nome_candidato;
    public String partido;
    public int num_votos;

    public Candidato(int codigo_votacao, String nome_candidato, String partido) {
        this.codigo_votacao = codigo_votacao;
        this.nome_candidato = nome_candidato;
        this.partido = partido;
        this.num_votos = 0;
    }

    @Override
    public String toString() {
        return "Codigo: " + codigo_votacao + ", Nome: " + nome_candidato + ", Partido: " + partido;
    }

}
