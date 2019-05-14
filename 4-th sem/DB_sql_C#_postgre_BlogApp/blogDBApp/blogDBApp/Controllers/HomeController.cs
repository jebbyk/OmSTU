using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Linq;
using System.Net;
using System.Web;
using System.Web.Mvc;
using blogDBApp.Models;
using Microsoft.AspNet.Identity;
using Microsoft.AspNet.Identity.Owin;
using Microsoft.Owin.Security;

namespace blogDBApp.Controllers
{
    public class HomeController : Controller
    {
        public ActionResult Index()
        {

            return View();
        }
        
        public ActionResult SearchSelect()
        {
            return View();
        }

        public ActionResult Contact()
        {
            ViewBag.Message = "Zaitsev Vladimir Alexandrovich";

            return View();
        }
    }
}