using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _2048
{
    class Program
    {

            public class Board {
            List<int> table;
            int size;
            public  Board() { }
            public Board(int _size)
            {
                table = new List<int>();
                size = _size;
            }
            public Board(List<int> _table, int _size)
            {
                table = _table;
                size = _size;
            }




            public String GetBoard()
            {
                String s = "";
                for (int i = 0; i < size; i++)
                {
                    for (int j = 0; j < size; j++)
                    {
                        //Console.Write(table[i * size + j]);
                        s = s + table[i * size + j];
                        //Console.WriteLine("");
                    }
                    s = s + "\n";
                }
                return s;
            }




        }






       static Board InitNewBoard(int size)
        {
            List<int> table = new List<int>();
            table.Capacity = size * size;
            /for (int i = 0; i < size*size; i++)
            {
                table.Append(0);
            }
            int x, y;
            Random r = new Random();
            x = r.Next(1, size);
            y = r.Next(1, size);
            Console.WriteLine(x + " " + y+" " + size);
            table[0] = 2;

            x = r.Next(1,size);
            y = r.Next(1,size);

            table[1] = 2;

            Board board = new Board(table, size);

            return board;
        }



        





        static void Main(string[] args)
        {

            Board mainBoard = InitNewBoard(3);

            Console.WriteLine(mainBoard.GetBoard());
            Console.ReadLine();



        }
    }
}
