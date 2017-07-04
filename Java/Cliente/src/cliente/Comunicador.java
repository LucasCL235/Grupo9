package cliente;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.nio.charset.Charset;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Responsável pela comunicação do lado cliente.
 */
public class Comunicador {

    private InputStream in;
    private OutputStream out;

    Socket socket;
    public static String IP = "cosmos.lasdpc.icmc.usp.br";
    public static int PORTA = 40009;

    public Comunicador() {
    }

    /**
     * Estabelece uma conexão com o servidor.
     *
     * @return Se a conexão foi estabelecida.
     */
    public boolean conectar() {
        try {
            socket = new Socket(IP, PORTA);
            in = socket.getInputStream();
            out = socket.getOutputStream();
            return true;
        } catch(IOException ex) {
            return false;
        }
    }

    /**
     * Finaliza a conexão com o servidor.
     */
    public void desconectar() {
        try {
            enviar("");
            socket.close();
        } catch(IOException ex) {
            Logger.getLogger(Comunicador.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    /**
     * envia uma mensagem
     *
     * @param msg
     */
    public void enviar(String msg) throws IOException {

        if(msg.isEmpty()) {
            // se for vazia, envia apenas um 0
            // qr dizer q chegou no fim da conexão
            out.write(0);
        } else {
            out.write(msg.getBytes());
        }
    }

    /**
     * recebe uma mensagem e transforma em Texto, ou seja, String
     *
     * @return
     */
    public String receber() throws IOException {
        byte[] b = new byte[1024];
        int len = in.read(b);
        return new String(b, 0, len, Charset.forName("UTF-8"));
    }

    public int receberInt() throws IOException {
        String str = receber();
        return Integer.valueOf(str);

    }

    void enviar(int i) throws IOException {
        enviar(Integer.toString(i));
    }
}
