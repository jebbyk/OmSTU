import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {
    public static void main(String[] args)
    {
        String GUID = new String();
        Scanner sc = new Scanner(System.in);
        System.out.println("enter IP address!");
        GUID = sc.nextLine();
        Pattern pat = Pattern.compile("(((1?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5])).){3}((1?[0-9]{1,2})|(2[0-4][0-9])|(25[0-5]))");
        Matcher m = pat.matcher(GUID);

        if(m.matches())
        {
            System.out.println("it's a normal IP");
        }else
        {
            System.out.println("it's not a normal IP");
        }
    }
}
