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
    public class commentsController : Controller
    {
        private blogDataBaseEntities2 db = new blogDataBaseEntities2();

        public ActionResult Search()
        {
            return View();
        }

        public ActionResult SearchResult(string text, string user, string publication, int? rating, DateTime? dateMin, DateTime? dateMax)
        {
            ViewBag.comments = db.comments.
                 Where(p => p.text.ToLower().Contains(text.ToLower())).
                 Where(p => p.users.name.ToLower().Contains(user.ToLower())).
                 Where(p => p.publications.name.ToLower().Contains(publication.ToLower())).
                 Where(p => p.rating >= rating).
                 Where(p => p.date >= dateMin && p.date <= dateMax).
                 ToList();
            return View(ViewBag);

        }

        // GET: comments
        [Authorize(Roles = "Admin")]
        public ActionResult Index()
        {
            var comments = db.comments.Include(c => c.publications).Include(c => c.users);
            return View(comments.ToList());
        }

        public ActionResult FilterByPublication(int? id)
        {
            ViewBag.comments = db.comments.
                Include(c => c.users).
                Where(c => c.publication == id).
                ToList();

            var p = db.publications.Find(id);
            ViewBag.publicationName = p.name;
            ViewBag.publicationText = p.text;
            ViewBag.publicationRating = p.rating;
            ViewBag.publicationDate = p.date;
            int pUserID = db.publications.Find(id).user;
            ViewBag.publicationUser = db.users.Find(pUserID).name;
            ViewBag.publicationID = id;

            return View();
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
        [Authorize]
        public ActionResult Create()
        {
            ViewBag.publication = new SelectList(db.publications, "id", "name");
            ViewBag.user = new SelectList(db.users, "id", "name");
            return View();
        }



        [Authorize]
        public ActionResult AddNew(int? id)
        {
            ViewBag.publication = id;
            ViewBag.user =db.users.Where(u => u.name == HttpContext.User.Identity.Name).ToList()[0].id;
            return View();
        }

        // POST: comments/Create
        // Чтобы защититься от атак чрезмерной передачи данных, включите определенные свойства, для которых следует установить привязку. Дополнительные 
        // сведения см. в статье https://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize]
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




        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize]
        public ActionResult AddNew([Bind(Include = "C_id,text,rating,date,publication,user")] comments comments, int? id)
        {
            if (ModelState.IsValid)
            {
                db.comments.Add(comments);
                db.SaveChanges();
                return RedirectToAction("Index","themes");
            }

            ViewBag.publication = id;
            ViewBag.user = db.users.Where(u => u.name == HttpContext.User.Identity.Name).ToList()[0].id;
            return View(comments);
        }



        // GET: comments/Edit/5
        [Authorize]
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
        [Authorize]
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
        [Authorize]
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
        [Authorize]
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
