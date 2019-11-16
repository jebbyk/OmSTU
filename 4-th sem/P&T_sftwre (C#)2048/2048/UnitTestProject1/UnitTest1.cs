using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using ClassLibrary1;

namespace UnitTestProject1
{
    [TestClass]
    public class UnitTest1
    {
        [DataTestMethod]
        [DataRow(3)]
        [DataRow(4)]
        [DataRow(5)]
        [DataRow(6)]
        [DataRow(7)]
        [DataRow(8)]
        public void AddNumberEmptyTest(int size)
        {
            int count = 0;
            bool check = true;
            Board b = new Board(size);
            while (check)
            {
                if (b.AddNumber() == 1)
                {
                    count++;
                }
                else check = false;
            }
            Assert.AreEqual(size * size - 2, count);
        }
        [TestMethod]
        [DataRow(3)]
        [DataRow(4)]
        [DataRow(5)]
        public void AddNumberFullTest(int size)
        {
            int count = 0;
            bool check = true;
            Board b = new Board(size);

            for (int i = 2; i < size * size; i++)
            {
                b.AddNumber();
            }

            while (check)
            {
                if (b.AddNumber() == 1)
                {
                    count++;
                }
                else check = false;
            }
            Assert.AreEqual(0, count);
        }
        [DataTestMethod]
        [DataRow(new int[] {
            2, 0, 0,
            0, 0, 0,
            0, 0, 0 }, 
            "200" +
            "000" +
            "000")] 
        [DataRow(new int[] {
            0, 2, 0,
            0, 0, 0,
            0, 0, 0 }, 
            "020" +
            "000" +
            "000")]
        [DataRow(new int[] {
            0, 0, 2,
            0, 0, 0,
            0, 0, 0 }, 
            "002" +
            "000" +
            "000")]
        [DataRow(new int[] {
            0, 0, 0,
            2, 0, 0,
            0, 0, 0 }, 
            "200" +
            "000" +
            "000")]
        [DataRow(new int[] {
            0, 0, 0,
            0, 2, 0,
            0, 0, 0 }, 
            "020" +
            "000" +
            "000")]
        [DataRow(new int[] {
            0, 0, 0,
            0, 0, 0,
            0, 0, 2 }, 
            "002" +
            "000" +
            "000")]
        [DataRow(new int[] {
            2, 2, 0,
            2, 0, 0,
            0, 0, 0 },
            "420" +
            "000" +
            "000")]
        [DataRow(new int[] {
            2, 2, 0,
            2, 0, 0,
            4, 0, 0 },
            "420" +
            "400" +
            "000")]
        [DataRow(new int[] {
            4, 2, 0,
            2, 0, 0,
            2, 0, 0 },
            "420" +
            "400" +
            "000")]
        [DataRow(new int[] {
            1024, 2048, 4096,
            1024, 2048, 4096,
            2, 0, 0 },
            "204840968192" +
            "200" +
            "000")]
        public void UpTest(int[] input, string output)
        {
           Board b = new Board(3);
           for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    b.table[i, j] = input[i * 3 + j];
                }
            }
            b.Up();
            string t = "";
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    t = t + b.table[i, j];
                }
            }
            Assert.AreEqual(t, output);
        }


        [DataTestMethod]
        [DataRow(new int[] {
            2, 0, 0,
            0, 0, 0,
            0, 0, 0 },
            "000" +
            "000" +
            "200")]
        [DataRow(new int[] {
            0, 2, 0,
            0, 0, 0,
            0, 0, 0 },
            "000" +
            "000" +
            "020")]
        [DataRow(new int[] {
            0, 0, 2,
            0, 0, 0,
            0, 0, 0 },
            "000" +
            "000" +
            "002")]
        [DataRow(new int[] {
            0, 0, 0,
            2, 0, 0,
            0, 0, 0 },
            "000" +
            "000" +
            "200")]
        [DataRow(new int[] {
            0, 0, 0,
            0, 2, 0,
            0, 0, 0 },
            "000" +
            "000" +
            "020")]
        [DataRow(new int[] {
            0, 0, 0,
            0, 0, 0,
            0, 0, 2 },
            "000" +
            "000" +
            "002")]
        [DataRow(new int[] {
            2, 2, 0,
            2, 0, 0,
            0, 0, 0 },
            "000" +
            "000" +
            "420")]
        [DataRow(new int[] {
            2, 2, 0,
            2, 0, 0,
            4, 0, 0 },
            "000" +
            "400" +
            "420")]
        [DataRow(new int[] {
            4, 2, 0,
            2, 0, 0,
            2, 0, 0 },
            "000" +
            "400" +
            "420")]
        [DataRow(new int[] {
            1024, 2048, 4096,
            1024, 2048, 4096,
            2, 0, 0 },
            "000" +
            "204800" +
            "240968192")]
        public void DownTest(int[] input, string output)
        {
            Board b = new Board(3);
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    b.table[i, j] = input[i * 3 + j];
                }
            }
            b.Down();
            string t = "";
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    t = t + b.table[i, j];
                }
            }
            Assert.AreEqual(t, output);
        }

        [DataTestMethod]
        [DataRow(new int[] {
            2, 0, 0,
            0, 0, 0,
            0, 0, 0 },
            "200" +
            "000" +
            "000")]
        [DataRow(new int[] {
            0, 2, 0,
            0, 0, 0,
            0, 0, 0 },
            "200" +
            "000" +
            "000")]
        [DataRow(new int[] {
            0, 0, 2,
            0, 0, 0,
            0, 0, 0 },
            "200" +
            "000" +
            "000")]
        [DataRow(new int[] {
            0, 0, 0,
            2, 0, 0,
            0, 0, 0 },
            "000" +
            "200" +
            "000")]
        [DataRow(new int[] {
            0, 0, 0,
            0, 2, 0,
            0, 0, 0 },
            "000" +
            "200" +
            "000")]
        [DataRow(new int[] {
            0, 0, 0,
            0, 0, 0,
            0, 0, 2 },
            "000" +
            "000" +
            "200")]
        [DataRow(new int[] {
            2, 2, 0,
            2, 0, 0,
            0, 0, 0 },
            "400" +
            "200" +
            "000")]
        [DataRow(new int[] {
            2, 2, 0,
            2, 0, 0,
            4, 0, 0 },
            "400" +
            "200" +
            "400")]
        [DataRow(new int[] {
            4, 2, 2,
            2, 0, 0,
            2, 0, 0 },
            "440" +
            "200" +
            "200")]
        [DataRow(new int[] {
            1024, 1024, 0,
            2048, 2048, 0,
            4096, 4096, 0 },
            "204800" +
            "409600" +
            "819200")]
        public void LeftTest(int[] input, string output)
        {
            Board b = new Board(3);
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    b.table[i, j] = input[i * 3 + j];
                }
            }
            b.Left();
            string t = "";
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    t = t + b.table[i, j];
                }
            }
            Assert.AreEqual(t, output);
        }

        [DataTestMethod]
        [DataRow(new int[] {
            2, 0, 0,
            0, 0, 0,
            0, 0, 0 },
            "002" +
            "000" +
            "000")]
        [DataRow(new int[] {
            0, 2, 0,
            0, 0, 0,
            0, 0, 0 },
            "002" +
            "000" +
            "000")]
        [DataRow(new int[] {
            0, 0, 2,
            0, 0, 0,
            0, 0, 0 },
            "002" +
            "000" +
            "000")]
        [DataRow(new int[] {
            0, 0, 0,
            2, 0, 0,
            0, 0, 0 },
            "000" +
            "002" +
            "000")]
        [DataRow(new int[] {
            0, 0, 0,
            0, 2, 0,
            0, 0, 0 },
            "000" +
            "002" +
            "000")]
        [DataRow(new int[] {
            0, 0, 0,
            0, 0, 0,
            0, 0, 2 },
            "000" +
            "000" +
            "002")]
        [DataRow(new int[] {
            2, 2, 0,
            2, 0, 0,
            0, 0, 0 },
            "004" +
            "002" +
            "000")]
        [DataRow(new int[] {
            2, 2, 0,
            2, 0, 0,
            4, 0, 0 },
            "004" +
            "002" +
            "004")]
        [DataRow(new int[] {
            4, 2, 2,
            2, 0, 0,
            2, 0, 0 },
            "044" +
            "002" +
            "002")]
        [DataRow(new int[] {
            1024, 1024, 0,
            2048, 2048, 0,
            4096, 4096, 0 },
            "002048" +
            "004096" +
            "008192")]
        public void RightTest(int[] input, string output)
        {
            Board b = new Board(3);
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    b.table[i, j] = input[i * 3 + j];
                }
            }
            b.Right();
            string t = "";
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    t = t + b.table[i, j];
                }
            }
            Assert.AreEqual(t, output);
        }


        [DataTestMethod]
        [DataRow(new int[] { 0,0,0,0,0,0,0,0,0}, "0 0 0 \n0 0 0 \n0 0 0 \n")]
        [DataRow(new int[] { 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152 }, "8192 16384 32768 \n65536 131072 262144 \n524288 1048576 2097152 \n")]
        public void GetTableTest(int[] input, string output)
        {
            Board b = new Board();
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    b.table[i, j] = input[i * 3 + j];
                }
            }
            string t = b.GetTable();
            Assert.AreEqual(t, output);
        }

        [DataTestMethod]
        [DataRow(2)]
        [DataRow(4)]
        [DataRow(16)]
        [DataRow(256)]
        [DataRow(512)]
        [DataRow(1024)]
        [DataRow(2048)]
        [DataRow(4096)]
        [DataRow(8192)]
        public void TableInitTest(int size)
        {
            Board b = new Board(size);
            int key = 0;
            foreach(int i in b.table)
            {
                key++;
                key += i;
            }
            Assert.AreEqual(key, size * size + 4);
        }
    }
}
