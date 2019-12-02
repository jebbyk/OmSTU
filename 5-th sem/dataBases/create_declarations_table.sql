use omstu_taxes;
go
if exists (select * from sys.tables where name = 'declarations')
drop table declarations;

create table declarations
(
    id int not null,
    creature_date date not null,
    num int not null,
    taxpayer_id int not null,
    getted_summ float not null,
    income_tax float not null,

    constraint pk_declaration_id primary key (id),
    
    constraint fk_taxpayer_id
    FOREIGN key (taxpayer_id) REFERENCES taxpayers(id)
    on update cascade on delete cascade,
);
GO