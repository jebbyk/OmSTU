using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Linq;
using System.Net;
using System.Web;
using System.Web.Mvc;
using blogDBApp.Models;
using blogDBApp.RequestsClasses;
using Npgsql;
using NpgsqlTypes;
using Microsoft.AspNet.Identity;
using Microsoft.AspNet.Identity.Owin;
using Microsoft.Owin.Security;

namespace blogDBApp.Controllers
{
    public class HomeController : Controller
    {
        private blogDataBaseEntities2 db = new blogDataBaseEntities2();
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

        public ActionResult Requests()
        {
            return View();
        }

        [Authorize(Roles = "Admin")]
        public ActionResult Request1(string name)//вывести коменты пользователь с ненуллевым рейтингом
        {
            /*ViewBag.result = db.comments.Where(c => c.users.name == name).
                Where(c => c.rating > 0).ToList();*/
            name = name.Trim();

            using (var db = new blogDataBaseEntities2())
            {
                var param = new NpgsqlParameter("@nm", name)
                {
                    NpgsqlDbType = NpgsqlDbType.Varchar
                };
                string sql = @" select u.name, u.id, c.text, c.rating
                                from comments c
                                inner join users u
                                on u.id = c.user
                                and u.name = @nm
                                and c.rating >0;
                                ";
                ViewBag.result = db.Database.SqlQuery<Request1Columns>(sql, param).ToList();
            }


            return View();
        }
 

        [Authorize(Roles = "Admin")]
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Request2()//коментарии добавленные после создания последней публикации
        {
            using (var db = new blogDataBaseEntities2())
            {
                string sql = @" select u.id, u.name, c.text, c.date
                                from comments c
                                join users u
                                on c.user = u.id
                                where c.date > (select max(p.date)   
                                from publications p)
                                ;";
                ViewBag.result = db.Database.SqlQuery<Request2Columns>(sql).ToList();
            }

            return View();
        }

        [Authorize(Roles = "Admin")]
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Request3()//кол. коментов в каждой теме
        {
            //List<Request3Columns> result;
            using (var db = new blogDataBaseEntities2())
            {
                string sql = @" select t.name, t.id, count(*) 
                                from publications p
                                join comments c
                                on c.publication = p.id
                                join themes t
                                on t.id = p.theme
                                group by t.id
                                order by t.id
                                ;";
                ViewBag.result = db.Database.SqlQuery<Request3Columns>(sql).ToList();
            }

                return View();
        }

        public ActionResult Request4(string theme)//пользователи создавших публикации на тему 
        {
            theme = theme.Trim();
            using (var db = new blogDataBaseEntities2())
            {
                var param = new NpgsqlParameter("@thm", theme)
                {
                    NpgsqlDbType = NpgsqlDbType.Varchar
                };
                string sql = @" select u.name as ""uName"", u.id as ""uID"", p.name as ""pName"", p.id as ""pID"", t.name as ""tName"", t.id as ""tID""
                                from users u
                                join publications p
                                on u.id = p.user
                                join themes t
                                on p.theme = t.id
                                where t.name = @thm;
                                ";
                ViewBag.result = db.Database.SqlQuery<Request45Columns>(sql, param).ToList();
            }



            return View();
        }

        public ActionResult Request5(string theme1, string theme2)//пользователи создавших публикации на тему 
        {
            theme1 = theme1.Trim();
            theme2 = theme2.Trim(); 
            using (var db = new blogDataBaseEntities2())
            {
                var param1 = new NpgsqlParameter("@thm1", theme1)
                {
                    NpgsqlDbType = NpgsqlDbType.Varchar
                };
                var param2 = new NpgsqlParameter("@thm2", theme2)
                {
                    NpgsqlDbType = NpgsqlDbType.Varchar
                };
                string sql = @" select u.name as ""uName"", u.id as ""uID"", p.name as ""pName"", p.id as ""pID"", t.name as ""tName"", t.id as ""tID""
                                from users u
                                join publications p
                                on u.id = p.user
                                join themes t
                                on p.theme = t.id
                                where t.name = @thm1
                                union all
                                select u.name as ""uName"", u.id as ""uID"", p.name as ""pName"", p.id as ""pID"", t.name as ""tName"", t.id as ""tID""
                                from users u
                                join publications p
                                on u.id = p.user
                                join themes t
                                on p.theme = t.id
                                where t.name = @thm2;
                                ";
                ViewBag.result = db.Database.SqlQuery<Request45Columns>(sql, param1, param2).ToList();
            }
            
            return View();
        }


        public ActionResult Request6(string theme1, string theme2, int? rating)//коменты пользователей написавших их к темам в каких то категориях
        {
            theme1 = theme1.Trim();
            theme2 = theme2.Trim();
            using (var db = new blogDataBaseEntities2())
            {
                var param1 = new NpgsqlParameter("@thm1", theme1)
                {
                    NpgsqlDbType = NpgsqlDbType.Varchar
                };
                var param2 = new NpgsqlParameter("@thm2", theme2)
                {
                    NpgsqlDbType = NpgsqlDbType.Varchar
                };
                var param3 = new NpgsqlParameter("@rtng", rating)
                {
                    NpgsqlDbType = NpgsqlDbType.Integer
                };
                string sql = @" select u.name as ""uName"", u.id as ""uID"", p.name as ""pName"", p.id as ""pID"", t.name as ""tName"", t.id as ""tID"", c.rating as ""cRating"", c.text as ""cText""
                                from publications p
                                join comments c
                                on c.publication = p.id
                                join users u
                                on u.id = c.user
                                join themes t
                                on t.id = p.theme
                                where c.rating > @rtng and t.name = @thm1
                                union all
                                select u.name as ""uName"", u.id as ""uID"", p.name as ""pName"", p.id as ""pID"", t.name as ""tName"", t.id as ""tID"", c.rating as ""cRating"", c.text as ""cText""
                                from publications p
                                join comments c
                                on c.publication = p.id
                                join users u
                                on u.id = c.user
                                join themes t
                                on t.id = p.theme
                                where c.rating > @rtng and t.name = @thm2;
                                ";
                ViewBag.result = db.Database.SqlQuery<Request6Columns>(sql, param1, param2, param3).ToList();
            }
            return View();
        }

        [Authorize(Roles = "Admin")]
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Request7()//кол. коментов в каждой теме
        {
            //List<Request3Columns> result;
            using (var db = new blogDataBaseEntities2())
            {
                string sql = @" select u.name,  c.text, 'comment' as ""type""
                                from users u
                                join comments c
                                on c.user = u.id
                                where c.text LIKE '%пло%' or c.text LIKE '%нолог%'
                                union all
                                select u.name,  m.text, 'message' as ""type""
                                from users u
                                join messages m
                                on m.sender = u.id
                                where m.text LIKE '%пло%' or m.text LIKE '%нолог%'
                                union all
                                select u.name, p.text, 'publication' as ""type""
                                from users u
                                join publications p
                                on p.user = u.id
                                where p.text LIKE '%пло%' or p.text LIKE '%нолог%'

                                ; ";
                ViewBag.result = db.Database.SqlQuery<Request7Columns>(sql).ToList();
            }

            return View();
        }


        [Authorize(Roles = "Admin")]
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Request8()//кол. коментов в каждой теме
        {
            //List<Request3Columns> result;
            using (var db = new blogDataBaseEntities2())
            {
                string sql = @" select t.name, t.id, avg(length(c.text)) as ""avg"", max(length(c.text)) as ""max""
                                from comments c
                                join publications p
                                on p.id = c.publication 
                                join themes t
                                on p.theme = t.id
                                group by t.id
                                ; ";
                ViewBag.result = db.Database.SqlQuery<Request8Columns>(sql).ToList();
            }

            return View();
        }


        [Authorize(Roles = "Admin")]
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Request9()//кол. коментов в каждой теме
        {
            //List<Request3Columns> result;
            using (var db = new blogDataBaseEntities2())
            {
                string sql = @" select t.name as ""name"", t.id as ""id"", avg(a.count) as ""avg""
                                from themes t
                                left
                                join (select count(*) as ""count"", p.theme as ""theme""
                                      from publications p
                                      left outer join comments c
                                      on p.id = c.publication
                                      group by p.id) a
                                      on a.theme = t.id
                                      where a.count > 0
                                    group by t.id
                                ; ";
                ViewBag.result = db.Database.SqlQuery<Request9Columns>(sql).ToList();
            }

            return View();
        }

        [Authorize(Roles = "Admin")]
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Request10()//юзеры которые хотябы что то написали
        {
            //List<Request3Columns> result;
            using (var db = new blogDataBaseEntities2())
            {
                string sql = @" select u.name, u.id
                                from users u
                                join comments c
                                on c.user  = u.id
                                intersect
                                select u.name, u.id
                                from users u
                                join publications p
                                on p.user = u.id
                                intersect 
                                select u.name, u.id
                                from users u
                                join messages m
                                on m.sender = u.id 
                                ; ";
                ViewBag.result = db.Database.SqlQuery<Request10Columns>(sql).ToList();
            }

            return View();
        }

    }
}