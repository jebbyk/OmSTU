using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Linq;
using System.Net;
using System.Web;
using System.Web.Mvc;
using blogDBApp.Models;

namespace blogDBApp.Controllers
{
    public class publicationsController : Controller
    {
        private blogDataBaseEntities2 db = new blogDataBaseEntities2();

        // GET: publications
        public ActionResult Index()
        {
            var publications = db.publications.Include(p => p.themes).Include(p => p.users);
            return View(publications.ToList());
        }

        public ActionResult FilterByTheme(int? id)
        {
            ViewBag.publications = db.publications.
              Include(p => p.themes).
              Where(p => p.theme == id).
              ToList();
            ViewBag.theme = "Theme: " + db.themes.Find(id).name;
            return View(ViewBag);
        }

        public ActionResult Search()
        {
            return View();
        }

        public ActionResult SearchResult(string search)
        {
            ViewBag.publications = db.publications.
             Where(p => p.name.ToLower().Contains(search.ToLower())).
             ToList();
            return View(ViewBag);

        }


        // GET: publications/Details/5
        public ActionResult Details(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            publications publications = db.publications.Find(id);
            if (publications == null)
            {
                return HttpNotFound();
            }
            return View(publications);
        }

        // GET: publications/Create
        public ActionResult Create()
        {
            ViewBag.theme = new SelectList(db.themes, "id", "name");
            ViewBag.user = new SelectList(db.users, "id", "name");
            return View();
        }

        // POST: publications/Create
        // Чтобы защититься от атак чрезмерной передачи данных, включите определенные свойства, для которых следует установить привязку. Дополнительные 
        // сведения см. в статье https://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Create([Bind(Include = "id,name,text,rating,date,user,theme")] publications publications)
        {
            if (ModelState.IsValid)
            {
                db.publications.Add(publications);
                db.SaveChanges();
                return RedirectToAction("Index");
            }

            ViewBag.theme = new SelectList(db.themes, "id", "name", publications.theme);
            ViewBag.user = new SelectList(db.users, "id", "name", publications.user);
            return View(publications);
        }

        // GET: publications/Edit/5
        public ActionResult Edit(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            publications publications = db.publications.Find(id);
            if (publications == null)
            {
                return HttpNotFound();
            }
            ViewBag.theme = new SelectList(db.themes, "id", "name", publications.theme);
            ViewBag.user = new SelectList(db.users, "id", "name", publications.user);
            return View(publications);
        }

        // POST: publications/Edit/5
        // Чтобы защититься от атак чрезмерной передачи данных, включите определенные свойства, для которых следует установить привязку. Дополнительные 
        // сведения см. в статье https://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Edit([Bind(Include = "id,name,text,rating,date,user,theme")] publications publications)
        {
            if (ModelState.IsValid)
            {
                db.Entry(publications).State = EntityState.Modified;
                db.SaveChanges();
                return RedirectToAction("Index");
            }
            ViewBag.theme = new SelectList(db.themes, "id", "name", publications.theme);
            ViewBag.user = new SelectList(db.users, "id", "name", publications.user);
            return View(publications);
        }

        // GET: publications/Delete/5
        public ActionResult Delete(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            publications publications = db.publications.Find(id);
            if (publications == null)
            {
                return HttpNotFound();
            }
            return View(publications);
        }

        // POST: publications/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public ActionResult DeleteConfirmed(int id)
        {
            publications publications = db.publications.Find(id);
            db.publications.Remove(publications);
            db.SaveChanges();
            return RedirectToAction("Index");
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                db.Dispose();
            }
            base.Dispose(disposing);
        }
    }
}
