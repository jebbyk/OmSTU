using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using BusinessLogic;

namespace UnitTestsSalaryApp
{
    [TestClass]
    public class UnitTest2
    {
        [TestMethod]
        public void EngineerSalaryTest()
        {
            Engineer engineer = new Engineer("Зайцев", "Владимир", "Александрович", "+");
            Assert.AreEqual(45000, engineer.EngineerSalary());
        }
    }
}