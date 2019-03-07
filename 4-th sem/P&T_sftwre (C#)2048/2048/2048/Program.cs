using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _2048
{
    class Program
    {

        public class Board
        {
            public int[,] table;
            int size;

            public void AddNumber()
            {
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
            }

            public Board()
            {
                size = 3;
                table = new int[3, 3];
                AddNumber();
                AddNumber();
            }

            //creates a board with size
            public Board(int _size)
            {
                size = _size;
                table = new int[size, size];

                AddNumber();
                AddNumber();
            }

            //returns a string of numbers
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


            public void UpdateTable(ConsoleKey key)
            {
                switch (key)
                {
                    case ConsoleKey.UpArrow:
                        {
                            Console.Clear();
                            Up();
                            Console.WriteLine("UP"); break;
                        }
                    case ConsoleKey.DownArrow:
                        {
                            Console.Clear();
                            Down();
                            Console.WriteLine("DOWN"); break;
                        }
                    case ConsoleKey.RightArrow:
                        {
                            Console.Clear();
                            Right();
                            Console.WriteLine("RIGTHT"); break;
                        }
                    case ConsoleKey.LeftArrow:
                        {
                            Console.Clear();
                            Left();
                            Console.WriteLine("LEFT"); break;
                        }
                    default: break;
                }
                AddNumber();
            }
        }
        public static void Main()
        {
            Board board = new Board(4);

            while (true)
            {
                ConsoleKey key;
                key = Console.ReadKey().Key;
                if (key == ConsoleKey.Escape) return;
                board.UpdateTable(key);
                Console.Write(board.GetTable());
            }

        }
    }
}
