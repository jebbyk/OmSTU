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

        
        public ActionResult themePublications(int? id)
        {
            ViewBag.publications = db.publications.
                Include(p => p.themes).
                Where(p => p.theme == id).
                ToList();
            ViewBag.theme = "Theme: " + db.themes.Find(id).name;
            return View(ViewBag);
        }

        public ActionResult publicationComments(int? id)
        {
            ViewBag.comments = db.comments.
                Include(c => c.publications).
                Where(c => c.publication == id).
                ToList();
            ViewBag.publicationName = db.publications.Find(id).name;
            ViewBag.publicationText = db.publications.Find(id).text;
            ViewBag.publicationRating = db.publications.Find(id).rating;
            ViewBag.publicationDate = db.publications.Find(id).date;

            return View(ViewBag);
        }
    }
}