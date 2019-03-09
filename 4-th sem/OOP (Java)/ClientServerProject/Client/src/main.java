import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.Socket;
import java.util.Random;
import java.util.Scanner;

public class main {

        public static void main(String[] args) {
            Socket socket = null;
            try {// получение строки клиентом

                socket = new Socket("localhost", 8030);
                Random r = new Random();
                int i = r.nextInt();

                while (true)
                {
                    BufferedReader dis = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                    while(System.in.available() == 0) {
                        if (dis.ready()) {
                            String msg = dis.readLine();
                            System.out.println(msg);
                        }
                    }

                    Scanner sc = new Scanner(System.in);
                    String text = sc.nextLine();
                    PrintStream ps = new PrintStream(socket.getOutputStream());
                    ps.println(text);
                    ps.flush();
                }
            } catch (IOException e) {
                System.out.println("error: " + e);
            }
        }


}
