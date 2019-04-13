using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Linq;
using System.Net;
using System.Web;
using System.Web.Mvc;
using blogDBApp.Models;
using Npgsql;

namespace blogDBApp.Controllers
{
    public class themesListController : Controller
    {

        private blogDataBaseEntities db = new blogDataBaseEntities();

        // GET: thmesList
        public ActionResult themesList()
        {
            return View(db.themes.ToList());
        }

        //[HttpPost]
        //[ValidateAntiForgeryToken]
        public ActionResult themePublications(int? id)
        {
           //const string Query = "SELECT * FROM pubications WHERE publications.id = @id ";

           // TempData["result"] = db.Database.SqlQuery<publications>(Query, new NpgsqlParameter("@id", "%" + id + "%")).ToList();


            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            themes theme = db.themes.Find(id);
            if (theme == null)
            {
                return HttpNotFound();
            }

            var result = db.publications.ToList();

            return View(result);
            
        }
    }
}