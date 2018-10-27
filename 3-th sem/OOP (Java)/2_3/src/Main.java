import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {
    public static void main(String[] args)
    {
        String GUID = new String();
        Scanner sc = new Scanner(System.in);
        System.out.println("enter GUID indefier!");
        GUID = sc.nextLine();
        Pattern pat = Pattern.compile("(([0-9|[a-f]]{2}):){5}([0-9|[a-f]]{2})");
        Matcher m = pat.matcher(GUID);

        if(m.matches())
        {
            System.out.println("it's a normal MAC-address");
        }else
        {
            System.out.println("it's not a normal MAC-address");
        }
    }
}
