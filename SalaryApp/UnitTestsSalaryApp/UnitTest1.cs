using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using BusinessLogic;

namespace UnitTestsSalaryApp
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void SurnameAndInitialsTest()
        {
            CompanyEmployee employee = new CompanyEmployee("Зайцев", "Владимир", "Александрович");
            Assert.AreEqual("Зайцев В.А.", employee.SurnameAndInitials());
        }
    }
}