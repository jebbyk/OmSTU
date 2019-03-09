using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _2048
{
    class Program
    {

        static int state = 0;

        public class Board
        {
            public int[,] table;
            int size;

            public int AddNumber()
            {
                bool empty = false;
                foreach(int i in table)
                {
                    if (i == 0) { empty = true; break; }
                }
                if (!empty) return -1;
                Random r = new Random();
                int x, y;
                x = r.Next(0, size);
                y = r.Next(0, size);
                while (table[y, x] != 0)
                {
                    x = r.Next(0, size);
                    y = r.Next(0, size);
                }
                table[y, x] = 2;
                return 1;
            }

            public Board()
            {
                size = 3;
                table = new int[3, 3];
                AddNumber();
                AddNumber();
            }

            public string GetTable()
            {
                string s = "";
                for (int i = 0; i < size; i++)
                {
                    for (int j = 0; j < size; j++)
                    {
                        s = s + table[i, j] + " ";
                    }
                    s = s + "\n";
                }
                return s;
            }
            private void Right()
            {
                
                for(int i = 0; i < size; i++)
                {
                    int num = 0;
                    bool flag = true;
                    for (int j = 0; j < size; j++)//combining numbers
                    {
                        if (table[i, j] != 0)
                        {
                            if (flag)
                            {
                                flag = false;
                                num = table[i, j];
                            }
                            else
                            {
                                if(num == table[i,j])
                                {
                                    flag = true;
                                    table[i, j] = num + num;
                                    table[i, j - 1] = 0;
                                    num = 0;
                                }
                                else {
                                    //flag = true;
                                    num = table[i, j];
                                }
                            }
                        }
                    }

                     for(int j = size-1; j >=0; j--)//mooving numbers
                    {
                        if(table[i,j] != 0)
                        {
                            int shift = j;
                            while (shift + 1 < size && table[i, shift+1] == 0 )
                            {

                                table[i, shift+1] = table[i, shift]; 
                                table[i, shift] = 0;
                                shift++;
                            }
                        }
                    }
                }
            }
            private void Left()
            {
                for (int i = 0; i < size; i++)
                {
                    int num = 0;
                    bool flag = true;
                    for (int j = size - 1; j >= 0; j--)//combining numbers
                    {
                        if (table[i, j] != 0)
                        {
                            if (flag)
                            {
                                flag = false;
                                num = table[i, j];
                            }
                            else
                            {
                                if (num == table[i, j])
                                {
                                    flag = true;
                                    table[i, j] = num + num;
                                    table[i, j + 1] = 0;
                                    num = 0;
                                }
                                else
                                {
                                    //flag = true;
                                    num = table[i, j];
                                }
                            }
                        }
                    }

                    for (int j = 0; j < size; j++)//mooving numbers
                    {
                        if (table[i, j] != 0)
                        {
                            int shift = j;
                            while (shift - 1 >= 0 && table[i, shift - 1] == 0)
                            {

                                table[i, shift - 1] = table[i, shift];
                                table[i, shift] = 0;
                                shift--;
                            }
                        }
                    }
                }
            }
            private void Up()
            {
                for (int j = 0; j < size; j++)
                {
                    int num = 0;
                    bool flag = true;
                    for (int i = size-1; i >= 0; i--)
                    {
                        if (table[i, j] != 0)
                        {
                            if (flag)
                            {
                                flag = false;
                                num = table[i, j];
                            }
                            else
                            {
                                if (num == table[i, j])
                                {
                                    flag = true;
                                    table[i, j] = num + num;
                                    table[i + 1, j] = 0;
                                    num = 0;
                                }
                                else
                                {
                                    //flag = true;
                                    num = table[i, j];
                                }
                            }
                        }
                    }

                    for (int i = 0; i < size; i++)//mooving numbers
                    {
                        if (table[i, j] != 0)
                        {
                            int shift = i;
                            while (shift - 1 >= 0 && table[shift - 1, j] == 0)
                            {

                                table[shift - 1, j] = table[shift, j];
                                table[shift, j] = 0;
                                shift--;
                            }
                        }
                    }
                }
            }
            private void Down()
            {
                for(int j = 0; j < size; j++)
                {
                    int num = 0;
                    bool flag = true;
                    for(int i = 0; i < size; i++)
                    {
                        if (table[i, j] != 0)
                        {
                            if (flag)
                            {
                                flag = false;
                                num = table[i, j];
                            }
                            else
                            {
                                if (num == table[i, j])
                                {
                                    flag = true;
                                    table[i, j] = num + num;
                                    table[i-1, j] = 0;
                                    num = 0;
                                }
                                else
                                {
                                    //flag = true;
                                    num = table[i, j];
                                }
                            }
                        }
                    }

                    for (int i = size - 1; i >= 0; i--)//mooving numbers
                    {
                        if (table[i, j] != 0)
                        {
                            int shift = i;
                            while (shift + 1 < size && table[shift + 1, j] == 0)
                            {

                                table[shift + 1, j] = table[shift, j];
                                table[shift, j] = 0;
                                shift++;
                            }
                        }
                    }
                }
            }
            public int UpdateTable(ConsoleKey key)
            {
                switch (key)
                {
                    case ConsoleKey.UpArrow:
                        {
                            Console.Clear();
                            Up();
                            return AddNumber();
                        }
                    case ConsoleKey.DownArrow:
                        {
                            Console.Clear();
                            Down();
                            return AddNumber();
                        }
                    case ConsoleKey.RightArrow:
                        {
                            Console.Clear();
                            Right();
                            return AddNumber();
                        }
                    case ConsoleKey.LeftArrow:
                        {
                            Console.Clear();
                            Left();
                            return AddNumber();
                        }
                    default: { Console.Clear(); return 1; }
                }
                
            }
        }
        
        static void Game()
        {
            Console.Clear();
            Board board = new Board();
            Console.Write(board.GetTable());
            while (true)
            {
                ConsoleKey key;
                key = Console.ReadKey().Key;
                if (key == ConsoleKey.Escape) { state = 0; return; }
                if (board.UpdateTable(key) == 1)
                {
                    Console.Write(board.GetTable());
                }
                else
                {
                    Console.Clear();
                    Console.WriteLine("Game over, no places for new numbers");
                    if (Console.ReadKey().Key == ConsoleKey.Escape ) { state = 0; return; };
                }
            }
        }

        static void LeaderBoard()
        {
            Console.Clear();
            Console.WriteLine("Leader board!! There only best players... not you))))");
            if (Console.ReadKey().Key == ConsoleKey.Escape) { Console.Clear(); state = 0; return; }
        }
        
        static void Menu()
        {
            int currentItem = 0;
            String[] items = { "new game", "leadrs", "exit" };
            
            while(true)
            {
                Console.Clear();
                for (int i = 0; i < items.Length; i++)
                {
                    if (i == currentItem) items[i] = items[i].ToUpper();
                    else items[i] = items[i].ToLower();
                    Console.WriteLine(items[i]);
                }
                ConsoleKey key = Console.ReadKey().Key;
                if(key == ConsoleKey.UpArrow)
                {
                    if(currentItem > 0)
                    {
                        currentItem--;
                    }
                    else
                    {
                        currentItem = items.Length-1;
                    }
                }
                if (key == ConsoleKey.DownArrow)
                {
                    if (currentItem < items.Length - 1)
                    {
                        currentItem++;
                    }
                    else
                    {
                        currentItem = 0;
                    }
                }
                if(key == ConsoleKey.Escape)
                {
                    return;
                }
                if(key == ConsoleKey.Enter)
                {
                    state = currentItem+1;
                    return;
                }

                
            }
        }

        public static void Main()
        {
            while (true)
            {
                
                 switch(state)
                {
                    case 0:
                        {
                            Menu();
                            if (state == 0) return;
                            else break;
                        }
                    case 1:
                        {
                            Game();
                            break;
                        }
                    case 2:
                        {
                            LeaderBoard();
                            break;
                        }
                    case 3:
                        {
                            return;
                        }
                    default: break;
                }

            }
        }
    }
}
