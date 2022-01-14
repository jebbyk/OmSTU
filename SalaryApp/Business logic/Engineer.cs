using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BusinessLogic
{
    public class Engineer : CompanyEmployee
    {
        private int constantlySalary = 30000;
        private int premium = 15000;
        private string presenceOfPremium;

        public string PresenceOfPremium { get => presenceOfPremium; set => presenceOfPremium = value; }

        public Engineer(string surnameEmployee, string nameEmployee, string patronymicEmployee, string presenceOfPremium) : 
            base(surnameEmployee, nameEmployee, patronymicEmployee)
        {
            this.presenceOfPremium = presenceOfPremium;
        }

        public double EngineerSalary()
        {
            double result = 0;
            if (presenceOfPremium == "+")
            {
                result = constantlySalary + premium;
            }
            if (presenceOfPremium == "-")
            {
                result = constantlySalary;
            }
            return result;
        }
    }
}