/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package servidor;

import cliente.Candidato;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * 
 */
public class Servidor {

    public static ArrayList<Candidato> candidatos;
    private static ServerSocket server;

    public static int brancos = 0;
    public static int nulos = 0;

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // adiciono todos os candidatos
        candidatos = new ArrayList<>();
        candidatos.add(new Candidato(11, "Java", "PDB"));
        candidatos.add(new Candidato(12, "Server", "SDB"));
        candidatos.add(new Candidato(13, "Olá", "PSB"));

        try {
            server = new ServerSocket(40009);
            while(true) {
                System.out.println("Aguardando conexões");
                Socket socket = server.accept();
                // após a linha de cima, uma conexão foi efetuada
                System.out.println("Conexão aceita");
                // trato ela
                new Comunicador(socket);
            }
        } catch(IOException ex) {
            Logger.getLogger(Servidor.class.getName()).log(Level.SEVERE, null, ex);
        }

    }

}
