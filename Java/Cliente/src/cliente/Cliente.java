/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cliente;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

/**
 *
 * 
 */
public class Cliente {

    private static int brancos = 0;

    private static ArrayList<Candidato> candidatos;
    private static Scanner in = new Scanner(System.in);
    private static int nulos = 0;

    static Comunicador comunicador = new Comunicador();

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // começo recebendo os candidatos
        // se não for possível
        // não inicializo o programa
        if(!receberCandidatos()) {
            System.out.println("Falha ao receber os candidatos.");
            return;
        }
        while(menu());
    }

    /**
     * Envio as informações para o servidor para encerrar a votação
     *
     * @return
     */
    private static boolean enviarInfo() {
        try {
            if(!comunicador.conectar()) {
                return false;
            }
            // começo enviando o código de encerramento
            // especificado na requisição
            comunicador.enviar("888");
            comunicador.receber(); // recebe para destravar, ou seja, sincronização

            System.out.println("Contagem encerrada!");
            System.out.println("Brancos: " + brancos);
            System.out.println("Nulos: " + nulos);

            // envia os brancos e nulos
            int[] BN = { brancos, nulos };
            for(int num : BN) {
                comunicador.enviar(num);
                comunicador.receber();
            }

            for(Candidato candidato : candidatos) {
                // envia o número dele
                // dps envia o número de votos
                System.out.println(candidato);
                System.out.println("Votos: " + candidato.num_votos);
                System.out.println();
                // envio seu código
                comunicador.enviar(candidato.codigo_votacao);
                comunicador.receber(); // destrava
                // envio seus vótos
                comunicador.enviar(candidato.num_votos);
                comunicador.receber(); // destrava
            }

            comunicador.desconectar();
            return true;
        } catch(IOException ex) {
        }
        return false;
    }

    private static int getInt() {
        return in.nextInt();
    }

    /**
     * O menu retorna true para repetir
     *
     * @return
     */
    private static boolean menu() {
        System.out.println("1 - Para votar em um candidato.");
        System.out.println("2 - Para votar branco.");
        System.out.println("3 - Para votar nulo.");
        System.out.println("999 - Para carregar a lista de candidatos.");
        System.out.println("888 - Para finalizar a votação dessa urna.");

        System.out.println("\nCandidatos:");
        // vou imprimir todos os candidatos
        for(Candidato candidato : candidatos) {
            System.out.println(candidato);
        }
        System.out.println();

        while(true) {
            int res = getInt();
            if(res == 1) {
                System.out.println("Digite o número do candidato:");
                int num = getInt();
                voto(num, null);
                pause();
            } else if(res == 2) {
                voto(2, "BRANCO");
                pause();
            } else if(res == 3) {
                voto(3, "NULO");
                pause();
            } else if(res == 888) {
                if(enviarInfo()) {
                    return false;
                } else {
                    System.out.println("Erro ao enviar as informações");
                    pause();
                    return true;
                }
            } else if(res == 999) {
                if(receberCandidatos()) {
                    System.out.println("Candidatos atualizados e zerados!");
                } else {
                    System.out.println("Falha ao atualizar a lista de candidatos.");
                }
                pause();
                return true;
            } else {
                System.out.println("Opção inválida, digite novamente.");
                continue;
            }
            break;
        }

        return true;
    }

    /**
     * faz um pause na tela
     */
    private static void pause() {
        System.out.println("Pressione ENTER para continuar.");
        try {
            System.in.read();
        } catch(IOException ex) {
        }
    }

    private static boolean receberCandidatos() {
        try {
            if(!comunicador.conectar()) {
                return false;
            }
            // envio o código
            // como sugerido na especificação
            comunicador.enviar("999");
            // recebo o número de candidatos primeiro
            int qntCandidatos = comunicador.receberInt();
            comunicador.enviar("."); // para ele destravar, ou seja, sincronização
            candidatos = new ArrayList<>(qntCandidatos);

            for(int i = 0; i < qntCandidatos; i++) {
                // recebo seu código de votação
                int codigo_votacao = comunicador.receberInt();
                comunicador.enviar("."); // para sincronização
                // recebo seu nome
                String nome_candidato = comunicador.receber();
                comunicador.enviar("."); // para sincronização
                // recebo seu partido
                String partido = comunicador.receber();
                comunicador.enviar("."); // para sincronização
                candidatos.add(new Candidato(codigo_votacao, nome_candidato, partido));
            }

            comunicador.desconectar();
            return true;
        } catch(IOException ex) {
        }
        return false;
    }

    /**
     * função que soma um voto
     *
     * @param num  código de votação do candidato
     * @param nome nome do candidato, null para ser reconhecido do BD
     * @return
     */
    private static boolean voto(int num, String nome) {
        if(num == 2) {
            brancos++;
        } else if(num == 3) {
            nulos++;
        } else {
            for(Candidato candidato : candidatos) {
                if(candidato.codigo_votacao == num) {
                    candidato.num_votos++;
                    nome = candidato.nome_candidato;
                }
            }
        }
        if(nome == null) {
            // não encontrou
            System.out.println("Candidato não encontrado.");
            return false;
        }
        System.out.println(nome + "! +1");
        return true;
    }

}
