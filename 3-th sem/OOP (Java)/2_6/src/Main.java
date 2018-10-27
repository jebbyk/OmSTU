import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {
    public static void main(String[] args)
    {
        String GUID = new String();
        Scanner sc = new Scanner(System.in);
        System.out.println("enter date!");
        GUID = sc.nextLine();
        Pattern pat = Pattern.compile("((0[1-9])|([1-2][0-9])|(3[0-1]))/((0[1-9])|(1[0-2]))/((1[6-9][0-9][0-9])|([2-9][0-9][0-9][0-9]))");
        Matcher m = pat.matcher(GUID);

        if(m.matches())
        {
            System.out.println("it's a normal date");
        }else
        {
            System.out.println("it's not a normal date");
        }
    }
}
