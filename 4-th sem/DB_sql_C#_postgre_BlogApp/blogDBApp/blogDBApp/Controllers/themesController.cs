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
    public class themesController : Controller
    {
        private blogDataBaseEntities2 db = new blogDataBaseEntities2();

        // GET: themes
        public ActionResult Index()
        {
            return View(db.themes.ToList());
        }

        public ActionResult Search()
        {
            return View();
        }

        public ActionResult SearchResult(string name)
        {
            ViewBag.themes = db.themes.
                Where(t => t.name.ToLower().Contains(name.ToLower())).
                ToList();
            return View();
        }

        // GET: themes/Details/5
        public ActionResult Details(int? id)
        {
            themes themes = db.themes.Find(id);
            return View(themes);
        }

        // GET: themes/Create
        public ActionResult Create()
        {
            return View();
        }

        // POST: themes/Create
        // Чтобы защититься от атак чрезмерной передачи данных, включите определенные свойства, для которых следует установить привязку. Дополнительные 
        // сведения см. в статье https://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Create([Bind(Include = "id,name")] themes themes)
        {
            if (ModelState.IsValid)
            {
                db.themes.Add(themes);
                db.SaveChanges();
                return RedirectToAction("Index");
            }

            return View(themes);
        }

        // GET: themes/Edit/5
        public ActionResult Edit(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            themes themes = db.themes.Find(id);
            if (themes == null)
            {
                return HttpNotFound();
            }
            return View(themes);
        }

        // POST: themes/Edit/5
        // Чтобы защититься от атак чрезмерной передачи данных, включите определенные свойства, для которых следует установить привязку. Дополнительные 
        // сведения см. в статье https://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Edit([Bind(Include = "id,name")] themes themes)
        {
            if (ModelState.IsValid)
            {
                db.Entry(themes).State = EntityState.Modified;
                db.SaveChanges();
                return RedirectToAction("Index");
            }
            return View(themes);
        }

        // GET: themes/Delete/5
        public ActionResult Delete(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            themes themes = db.themes.Find(id);
            if (themes == null)
            {
                return HttpNotFound();
            }
            return View(themes);
        }

        // POST: themes/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public ActionResult DeleteConfirmed(int id)
        {
            themes themes = db.themes.Find(id);
            db.themes.Remove(themes);
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
