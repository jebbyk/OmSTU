using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BusinessLogic
{
    public class CompanyEmployee
    {
        private string nameEmployee;
        private string surnameEmployee;
        private string patronymicEmployee;

        public string NameEmployee { get => nameEmployee; set => nameEmployee = value; }
        public string SurnameEmployee { get => surnameEmployee; set => surnameEmployee = value; }
        public string PatronymicEmployee { get => patronymicEmployee; set => patronymicEmployee = value; }

        public CompanyEmployee() { }

        public CompanyEmployee(string surnameEmployee, string nameEmployee, string patronymicEmployee)
        {
            this.surnameEmployee = surnameEmployee;
            this.nameEmployee = nameEmployee;
            this.patronymicEmployee = patronymicEmployee;
        }

        public string SurnameAndInitials()
        {
            return surnameEmployee + " " + nameEmployee[0] + "." + patronymicEmployee[0] + ".";
        }
    }
}