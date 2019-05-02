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
    public class messagesController : Controller
    {
        private blogDataBaseEntities2 db = new blogDataBaseEntities2();

        // GET: messages
        public ActionResult Index()
        {
            var messages = db.messages.Include(m => m.users).Include(m => m.users1);
            return View(messages.ToList());
        }

        public ActionResult Search()
        {
            return View();
        }

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

        // GET: messages/Details/5
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

        // GET: messages/Edit/5
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
