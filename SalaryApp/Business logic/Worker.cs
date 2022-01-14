using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BusinessLogic
{
    public class Worker : CompanyEmployee
    {
        private double workingHour = 150;
        private double hours;

        public double Hours { get => hours; set => hours = value; }

        public Worker(string surnameEmployee, string nameEmployee, string patronymicEmployee, double hours) : 
            base(surnameEmployee, nameEmployee, patronymicEmployee)
        {
            this.hours = hours;
        }

        public double WorkerSalary()
        {
            double fullSalary = workingHour * 128;
            double result = 0;
            if (hours > 128)
            {
                result = fullSalary + (hours * (workingHour * 2));
            }
            if (hours <= 128)
            {
                result = hours * workingHour;
            }
            return result;
        }
    }
}