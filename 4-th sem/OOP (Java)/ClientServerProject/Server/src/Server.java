import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.LinkedList;
import java.util.List;

public class Server {
    public static void main(String[] args) throws Exception {

            ServerSocket server = new ServerSocket(8030);
            System.out.println("Server started!");

            Client c1 = new Client(server.accept());
            System.out.println("Connected with first!");
            Client c2 = new Client(server.accept());
            System.out.println("Connected with second!");

                while(true) {

                    c1.Send(c2.Recieve());
                    c2.Send(c1.Recieve());
        }
    }
}
