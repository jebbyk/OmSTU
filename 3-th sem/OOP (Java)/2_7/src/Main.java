import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {
    public static void main(String[] args)
    {
        String GUID = new String();
        Scanner sc = new Scanner(System.in);
        System.out.println("enter your email address!");
        GUID = sc.nextLine();
        Pattern pat = Pattern.compile("([a-z|[A-Z|[0-9]]]{3,})@([a-z]+).([a-z]+)");
        Matcher m = pat.matcher(GUID);

        if(m.matches())
        {
            System.out.println("it's a normal email");
        }else
        {
            System.out.println("it's not a normal email");
        }
    }
}
