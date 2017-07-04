/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package servidor;

import cliente.Candidato;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * 
 */
public class Comunicador extends Thread {

    private InputStream in = null;
    private OutputStream out = null;

    private Socket socket = null;

    Comunicador(Socket socket) {
        try {
            this.socket = socket;
            this.in = socket.getInputStream();
            this.out = socket.getOutputStream();
            // inicializa a própria thread
            this.start();
        } catch(IOException ex) {
            System.out.println("Erro inesperado");
        }
    }

    @Override
    public void run() {
        // começo recebendo um valor
        String str = receber();

        if(str.equals("999")) {
            System.out.println("Enviando lista de candidatos");
            // envia primeiro a quantidade
            enviar(Servidor.candidatos.size());
            receber(); // para travar, ou seja, sincronização
            // para cada candidato
            for(Candidato candidato : Servidor.candidatos) {
                // envio seu número
                enviar(candidato.codigo_votacao);
                receber(); // para travar
                // envio seu nome
                enviar(candidato.nome_candidato);
                receber(); // para travar
                // envio seu partido
                enviar(candidato.partido);
                receber(); // para travar
            }

        } else if(str.equals("888")) {
            // caso de encerramento da urna
            System.out.println("Urna encerrada!");
            enviar("."); // para destravar, ou seja, sincronização
            // recebo os votos brancos, dps os nulos
            int qntBrancos = receberInt();
            enviar("."); // para destravar
            Servidor.brancos += qntBrancos;
            System.out.println("Brancos: " + Servidor.brancos);
            int qntNulos = receberInt();
            enviar("."); // para destravar
            Servidor.nulos += qntNulos;
            System.out.println("Nulos: " + Servidor.nulos);

            // agr pega pra cada candidato
            // recebo os votos deles
            for(int i = 0; i < Servidor.candidatos.size(); i++) {
                int num = receberInt(); // numero do candidato
                enviar("."); // para destravar

                int qnt = receberInt(); // votos do candidato
                enviar("."); // para destravar
                // encontra o candidato
                // e soma os votos nele
                for(Candidato candidato : Servidor.candidatos) {
                    if(candidato.codigo_votacao == num) {
                        System.out.println(candidato);
                        candidato.num_votos += qnt;
                        System.out.println("Votos: " + candidato.num_votos + "\n");
                    }
                }
            }
        }
        System.out.println("Conexão encerrada.");
    }

    /**
     * Função que envia uma mensagem
     *
     * @param msg
     */
    public void enviar(String msg) {
        try {
            if(msg.isEmpty()) {
                out.write(0);
            } else {
                out.write(msg.getBytes());
            }
        } catch(IOException ex) {
            Logger.getLogger(Comunicador.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    /**
     * Função que recebe um quantidade de bytes e retorna eles como Texto
     *
     * @return
     */
    public String receber() {
        try {
            byte[] b = new byte[1024];
            int len = in.read(b);
            return new String(b, 0, len);
        } catch(IOException ex) {
            Logger.getLogger(Comunicador.class.getName()).log(Level.SEVERE, null, ex);
        }
        return "";
    }

    public int receberInt() {
        String str = receber();
        return Integer.valueOf(str);
    }

    void enviar(int i) {
        enviar(Integer.toString(i));
    }
}
