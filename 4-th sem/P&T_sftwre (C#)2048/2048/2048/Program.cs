using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ClassLibrary1;

namespace _2048
{
    class Program
    {

        static int state = 0;

        
        
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

        /*static void LeaderBoard()
        {
            Console.Clear();
            Console.WriteLine("Leader board!! There only best players... not you))))");
            if (Console.ReadKey().Key == ConsoleKey.Escape) { Console.Clear(); state = 0; return; }
        }*/
        
        static void Menu()
        {
            int currentItem = 0;
            String[] items = { "new game", "exit" };
            
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
                   /* case 2:
                        {
                            LeaderBoard();
                            break;
                        }*/
                    case 2:
                        {
                            return;
                        }
                    default: break;
                }

            }
        }
    }
}
