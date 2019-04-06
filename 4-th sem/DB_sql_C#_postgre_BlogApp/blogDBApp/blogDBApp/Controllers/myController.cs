using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace blogDBApp.Controllers
{
    public class myController : Controller
    {
        // GET: my
        public ActionResult Index()
        {
            ViewBag.data = new[] { 1, 2, 3, 4, 5 };
            return View();
        }
    }
}