import java.util.*;

public class Main {
    public static void main(String[] args)
    {


        Scanner sc = new Scanner(System.in);

        System.out.println("Enter a number");
        String line = sc.nextLine();
        char[] numChars = line.toCharArray();
        Stack<Integer> numbers1 = new Stack<>();
        for (char ch : numChars)
        {
            numbers1.push(Character.getNumericValue(ch));
        }

        System.out.println("Enter second number");
        line = sc.nextLine();
        numChars = line.toCharArray();
        Stack<Integer> numbers2 = new Stack<>();
        for (char ch : numChars)
        {
            numbers2.push(Character.getNumericValue(ch));
        }

        Stack<Integer> temp = numbers1;
        numbers1 = numbers2;
        numbers2 = temp;


        System.out.println("numbers in first stack:");
        while(!numbers1.isEmpty())
        {
            System.out.print(numbers1.pop());
        }
        System.out.println("\n numbers in second stack");
        while(!numbers2.isEmpty())
        {
            System.out.print(numbers2.pop());
        }


    }
}
