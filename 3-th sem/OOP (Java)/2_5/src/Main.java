import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {
    public static void main(String[] args)
    {
        String GUID = new String();
        Scanner sc = new Scanner(System.in);
        System.out.println("enter color indefier!");
        GUID = sc.nextLine();
        Pattern pat = Pattern.compile("#[0-9|[a-f|[A-F]]]{6}");
        Matcher m = pat.matcher(GUID);

        if(m.matches())
        {
            System.out.println("it's a normal color indefier");
        }else
        {
            System.out.println("it's not a normal color indefier");
        }
    }
}
