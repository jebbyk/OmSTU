import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.Socket;

public class Client {

    Socket socket;

    Client(Socket _socket)
    {
      socket = _socket;
    }


    String  Recieve()
    {
        try {
                BufferedReader dis = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                if(dis.ready()) {
                    return dis.readLine();
                } else return null;
        }catch (IOException e) {
            return ("error: " + e);
        }
    }


    void Send( String text)
    {
        try {
            if(text != null) {
                PrintStream ps = new PrintStream(socket.getOutputStream());
                ps.println(text);
                ps.flush();
            }
        }catch (IOException e) {
            System.out.println("error: " + e);
        }
    }
}
