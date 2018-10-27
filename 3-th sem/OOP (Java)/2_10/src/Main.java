import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {
    public static void main(String[] args)
    {
        String pass = new String();
        Scanner sc = new Scanner(System.in);
        System.out.println("enter number!");
        pass = sc.nextLine();
        Pattern pat = Pattern.compile("([0-9]{3})([1-9]{3})");


        Matcher m = pat.matcher(pass);

        if(m.matches() )
        {
            System.out.println("it's a normal number");
        }else
        {
            System.out.println("it's not a normal number");
        }
    }
}
