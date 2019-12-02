use omstu_taxes;
go
if exists (select * from sys.tables where name = 'inspections')
drop table inspections;

create table inspections
(
    id int not null,
    inspection_number varchar(32) not null,
    
    constraint pk_inspection_id primary key (id),
);
GO