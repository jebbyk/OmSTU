using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace blogDBApp.RequestsClasses
{

    public class Request1Columns
    {
        public string name { get; set; }
        public int id { get; set; }
        public string text { get; set; }
        public int rating { get; set; }
    }

    public class Request2Columns
    {
        public string name { get; set; }
        public int id { get; set; }
        public string text { get; set; }
        public DateTime date { get; set; }
    }

    public class Request3Columns
    {
        public string name{ get; set; }
        public int id { get; set; }
        public int count { get; set; }
    }

    public class Request45Columns
    {
        public string uName { get; set; }
        public int uID { get; set; }
        public string pName { get; set; }
        public int pID { get; set; }
        public string tName { get; set; }
        public int tID { get; set; }
    }

    public class Request6Columns
    {
        public string uName { get; set; }
        public int uID { get; set; }
        public string pName { get; set; }
        public int pID { get; set; }
        public string tName { get; set; }
        public int tID { get; set; }
        public int cRating { get;set; }
        public string cText { get; set; }
    }
}