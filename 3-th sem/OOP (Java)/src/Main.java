import java.util.Scanner;

public class Main {
    public static void main(String[] args)
    {
        System.out.println("Enter your text");
        String text = new String();//inputing text
        Scanner sc = new Scanner(System.in);
        text = sc.nextLine();
        System.out.println(text);//checking

        String[] wordsList = new String[1];
        wordsList[0] = "";// make firs word != null

        for(int i = 0; i < text.length();)
        {
            char c = text.charAt(i); i++;
            if(c == ' ')// if geting a space then add new word to wordsList
            {
                String[] newWordsList = new String[wordsList.length+1];//create new list
                for(int n = 0; n < wordsList.length; n++)
                {
                    newWordsList[n] = wordsList[n];//copy old list to new
                }
                newWordsList[newWordsList.length - 1] = "";// make last word != null
                wordsList = newWordsList;
            }//or add new char to current word
            else {
                wordsList[wordsList.length -1] = wordsList[wordsList.length -1] + c;
            }
        }

        for(int i = 0; i < wordsList.length; i++)//check
        {
            System.out.println(wordsList[i]);
        }

        for(int i = 0; i < wordsList.length; i++ )//counting similar words
        {
            int count = 1;
            for(int n = 0; n < wordsList.length; n++)
            {
                if(n!=i && wordsList[n] != null && wordsList[i] != null && wordsList[i].contentEquals(wordsList[n]) )
                {
                    wordsList[n] = null;// delete word if it counted already
                    count++;
                }
            }
            if(wordsList[i] != null)
            System.out.println("Current word: " + wordsList[i] + " Count: " + count);
        }
    }
}
