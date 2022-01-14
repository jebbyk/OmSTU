using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BusinessLogic
{
    public class Manager : CompanyEmployee
    {
        private int constantlySalary = 25000;
        private int bonus = 1500;
        private int amountOfDeals;

        public int AmountOfDeals { get => amountOfDeals; set => amountOfDeals = value; }

        public Manager(string surnameEmployee, string nameEmployee, string patronymicEmployee, int amountOfDeals) :
            base(surnameEmployee, nameEmployee, patronymicEmployee)
        {
            this.amountOfDeals = amountOfDeals;
        }

        public double ManagerSalary()
        {
            return constantlySalary + amountOfDeals * bonus;
        }
    }
}