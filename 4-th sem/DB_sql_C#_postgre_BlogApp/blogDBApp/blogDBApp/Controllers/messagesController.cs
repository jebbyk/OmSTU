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


namespace blogDBApp.Controllers
{
    public class messagesController : Controller
    {
        private blogDataBaseEntities2 db = new blogDataBaseEntities2();
        // GET: messages
        [Authorize(Roles = "Admin")]
        public ActionResult Index()
        {
            var messages = db.messages.Include(m => m.users).Include(m => m.users1);
            return View(messages.ToList());
        }


        [Authorize(Roles = "Admin")]
        public ActionResult Search()
        {
            return View();
        }

        [Authorize(Roles = "Admin")]
        public ActionResult SearchResult(string senderName, string recName, string title, DateTime? minDate, DateTime? maxDate, string text)
        {
            ViewBag.messages = db.messages.
                Where(m => m.users.name.ToLower().Contains(senderName.ToLower())).
                Where(m => m.users1.name.ToLower().Contains(recName.ToLower())).
                Where(m => m.name.ToLower().Contains(title.ToLower())).
                Where(m => m.date >= minDate && m.date <= maxDate).
                Where(m => m.text.ToLower().Contains(text.ToLower())).
                ToList();
            return View();
        }

        [Authorize]
        public ActionResult FilterByUser()
        {

            string loggedUserName = HttpContext.User.Identity.Name;
            int loggedUserID = db.users.Where(u => u.name == loggedUserName).ToList()[0].id;
            ViewBag.messages = db.messages.
                Where(m => m.receiver == loggedUserID).
                ToList();

            return View();
        }

        // GET: messages/Details/5
        [Authorize]
        public ActionResult Details(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            messages messages = db.messages.Find(id);
            if (messages == null)
            {
                return HttpNotFound();
            }
            return View(messages);
        }

        // GET: messages/Create
        [Authorize]
        public ActionResult Create()
        {
            ViewBag.receiver = new SelectList(db.users, "id", "name");
            ViewBag.sender = new SelectList(db.users, "id", "name");
            return View();
        }

        // POST: messages/Create
        // Чтобы защититься от атак чрезмерной передачи данных, включите определенные свойства, для которых следует установить привязку. Дополнительные 
        // сведения см. в статье https://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Admin")]
        public ActionResult Create([Bind(Include = "id,sender,receiver,name,date,text")] messages messages)
        {
            if (ModelState.IsValid)
            {
                db.messages.Add(messages);
                db.SaveChanges();
                return RedirectToAction("Index");
            }

            ViewBag.receiver = new SelectList(db.users, "id", "name", messages.receiver);
            ViewBag.sender = new SelectList(db.users, "id", "name", messages.sender);
            return View(messages);
        }

        [Authorize]
        public ActionResult Send()
        {
            string loggedUserName = HttpContext.User.Identity.Name;
            int loggedUserID = db.users.Where(u => u.name == loggedUserName).ToList()[0].id;
            ViewBag.receiver = new SelectList(db.users, "id", "name");
            ViewBag.sender = loggedUserID;
            return View();
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize]
        public ActionResult Send([Bind(Include = "id,sender,receiver,name,date,text")] messages messages)
        {

            string loggedUserName = HttpContext.User.Identity.Name;
            int loggedUserID = db.users.Where(u => u.name == loggedUserName).ToList()[0].id;

            if (ModelState.IsValid)
            {
                db.messages.Add(messages);
                db.SaveChanges();
                return RedirectToAction("FilterByUser");
            }

            ViewBag.receiver = new SelectList(db.users, "id", "name", messages.receiver);
            ViewBag.sender = loggedUserID;
            return View(messages);
        }

        // GET: messages/Edit/5
        [Authorize(Roles = "Admin")]
        public ActionResult Edit(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            messages messages = db.messages.Find(id);
            if (messages == null)
            {
                return HttpNotFound();
            }
            ViewBag.receiver = new SelectList(db.users, "id", "name", messages.receiver);
            ViewBag.sender = new SelectList(db.users, "id", "name", messages.sender);
            return View(messages);
        }

        // POST: messages/Edit/5
        // Чтобы защититься от атак чрезмерной передачи данных, включите определенные свойства, для которых следует установить привязку. Дополнительные 
        // сведения см. в статье https://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Admin")]
        public ActionResult Edit([Bind(Include = "id,sender,receiver,name,date,text")] messages messages)
        {
            if (ModelState.IsValid)
            {
                db.Entry(messages).State = EntityState.Modified;
                db.SaveChanges();
                return RedirectToAction("Index");
            }
            ViewBag.receiver = new SelectList(db.users, "id", "name", messages.receiver);
            ViewBag.sender = new SelectList(db.users, "id", "name", messages.sender);
            return View(messages);
        }

        // GET: messages/Delete/5
        [Authorize]
        public ActionResult Delete(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            messages messages = db.messages.Find(id);
            if (messages == null)
            {
                return HttpNotFound();
            }
            return View(messages);
        }

        // POST: messages/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        [Authorize]
        public ActionResult DeleteConfirmed(int id)
        {
            messages messages = db.messages.Find(id);
            db.messages.Remove(messages);
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
