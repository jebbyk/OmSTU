use omstu_taxes;
go
if exists (select * from sys.tables where name = 'privileges')
drop table privileges;

create table privileges
(
    id int not null,
    privilege_name varchar(256) not null,
    summ float not null,

    constraint pk_privilege_id primary key (id),
);
GO