using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using BusinessLogic;

namespace SalaryApp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void buttonCalculation_Click(object sender, EventArgs e)
        {
            CompanyEmployee companyEmployee = new CompanyEmployee(textBoxSurname.Text, textBoxName.Text, textBoxPatronymic.Text);
            labelSurnameAndInitials.Text = companyEmployee.SurnameAndInitials();
            if (comboBox.SelectedIndex == 0)
            {
                Engineer engineer = new Engineer(textBoxSurname.Text, textBoxName.Text, textBoxPatronymic.Text, textBoxPremium.Text);
                labelSalary.Text = $"{engineer.EngineerSalary()} р.";
            }
            if (comboBox.SelectedIndex == 1)
            {
                Worker worker = new Worker(textBoxSurname.Text, textBoxName.Text, textBoxPatronymic.Text, Convert.ToDouble(textBoxHours.Text));
                labelSalary.Text = $"{worker.WorkerSalary()} р.";
            }
            if (comboBox.SelectedIndex == 2)
            {
                Manager manager = new Manager(textBoxSurname.Text, textBoxName.Text, textBoxPatronymic.Text, Convert.ToInt32(textBoxBusiness.Text));
                labelSalary.Text = $"{manager.ManagerSalary()} р.";
            }
        }
    }
}