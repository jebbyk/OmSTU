import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {
    public static void main(String[] args)
    {
        String pass = new String();
        Scanner sc = new Scanner(System.in);
        System.out.println("enter password!");
        pass = sc.nextLine();
        Pattern pat1 = Pattern.compile("[_|[a-z|[A-Z|[0-9]]]]{8,}");
        Pattern pat2 = Pattern.compile("[a-z]+");
        Pattern pat3 = Pattern.compile("[A-Z]+");
        Pattern pat4 = Pattern.compile("[0-9]+");


        Matcher m1 = pat1.matcher(pass);
        Matcher m2 = pat2.matcher(pass);
        Matcher m3 = pat3.matcher(pass);
        Matcher m4 = pat4.matcher(pass);

        if(m1.matches() && m2.find() && m3.find() && m4.find())
        {
            System.out.println("it's a good password");
        }else
        {
            System.out.println("it's not a good password");
        }
    }
}
