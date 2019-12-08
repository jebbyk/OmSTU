use omstu_taxes;
go 

insert into taxpayers VALUES
(1000,'111 111 111', 'Mujyk', 'Mujyk', 'Mujyk','pasport', 128, 256, '05/01/2018',
 'Omsk third street', '05/01/2000', 'http://photo/1', 'Omsk thirst street', 1, 1);
 go


select * from taxpayers
where id  = 1000;
go

delete from taxpayers
where id = 1000;
go