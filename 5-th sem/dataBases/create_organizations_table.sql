use omstu_taxes;
go
if exists (select * from sys.tables where name = 'organizations')
drop table organizations;

create table organizations
(
    id int not null,
    inn varchar(32) not null,
    accountant_initials varchar(32) not null,
    
    addres varchar(512) not null,
    phone_number varchar(32) not null,

    constraint pk_organization_id primary key (id),
);
GO