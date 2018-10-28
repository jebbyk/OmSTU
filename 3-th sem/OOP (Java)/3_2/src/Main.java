import java.util.*;

public class Main {
    public static void main(String[] args)
    {

        System.out.println("Enter a number");
        Scanner sc = new Scanner(System.in);
        String line = sc.nextLine();
        char[] numChars = line.toCharArray();
        Stack<Integer> numbers = new Stack<>();

        for (char ch : numChars)
        {
            numbers.push(Character.getNumericValue(ch));
        }

       while(!numbers.isEmpty())
       {
           Integer i = numbers.pop();
           System.out.print(i);
       }
    }
}
