import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Main {
    public static void main(String[] args)
    {
       List<Integer> numbers = new ArrayList<Integer>();
       for(int i = 0; i < 8; i++)
       {
           Integer a = Integer.valueOf((int)((Math.random() - 0.5)*100));
            numbers.add(a);
       }



       Collections.sort(numbers);
       Collections.reverse(numbers);

       for(Integer num : numbers)
       {
           System.out.println(num);
       }

    }
}
