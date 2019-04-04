using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Linq;
using System.Net;
using System.Web;
using System.Web.Mvc;
using blogBDApp.Models;

namespace blogBDApp.Content
{
    public class commentsController : Controller
    {
        private blogDataBaseEntities db = new blogDataBaseEntities();

        // GET: comments
        public ActionResult Index()
        {
            var comments = db.comments.Include(c => c.publications).Include(c => c.users);
            return View(comments.ToList());
        }

        // GET: comments/Details/5
        public ActionResult Details(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            comments comments = db.comments.Find(id);
            if (comments == null)
            {
                return HttpNotFound();
            }
            return View(comments);
        }

        // GET: comments/Create
        public ActionResult Create()
        {
            ViewBag.publication = new SelectList(db.publications, "id", "name");
            ViewBag.user = new SelectList(db.users, "id", "name");
            return View();
        }

        // POST: comments/Create
        // Чтобы защититься от атак чрезмерной передачи данных, включите определенные свойства, для которых следует установить привязку. Дополнительные 
        // сведения см. в статье https://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Create([Bind(Include = "C_id,text,rating,date,publication,user")] comments comments)
        {
            if (ModelState.IsValid)
            {
                db.comments.Add(comments);
                db.SaveChanges();
                return RedirectToAction("Index");
            }

            ViewBag.publication = new SelectList(db.publications, "id", "name", comments.publication);
            ViewBag.user = new SelectList(db.users, "id", "name", comments.user);
            return View(comments);
        }

        // GET: comments/Edit/5
        public ActionResult Edit(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            comments comments = db.comments.Find(id);
            if (comments == null)
            {
                return HttpNotFound();
            }
            ViewBag.publication = new SelectList(db.publications, "id", "name", comments.publication);
            ViewBag.user = new SelectList(db.users, "id", "name", comments.user);
            return View(comments);
        }

        // POST: comments/Edit/5
        // Чтобы защититься от атак чрезмерной передачи данных, включите определенные свойства, для которых следует установить привязку. Дополнительные 
        // сведения см. в статье https://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Edit([Bind(Include = "C_id,text,rating,date,publication,user")] comments comments)
        {
            if (ModelState.IsValid)
            {
                db.Entry(comments).State = EntityState.Modified;
                db.SaveChanges();
                return RedirectToAction("Index");
            }
            ViewBag.publication = new SelectList(db.publications, "id", "name", comments.publication);
            ViewBag.user = new SelectList(db.users, "id", "name", comments.user);
            return View(comments);
        }

        // GET: comments/Delete/5
        public ActionResult Delete(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            comments comments = db.comments.Find(id);
            if (comments == null)
            {
                return HttpNotFound();
            }
            return View(comments);
        }

        // POST: comments/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public ActionResult DeleteConfirmed(int id)
        {
            comments comments = db.comments.Find(id);
            db.comments.Remove(comments);
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
