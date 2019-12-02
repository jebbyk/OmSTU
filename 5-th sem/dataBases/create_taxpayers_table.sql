use omstu_taxes;
go
if exists (select * from sys.tables where name = 'taxpayers')
drop table taxpayers;

create table taxpayers
(
    id int not null,
    inn varchar(32) not null,
    first_name varchar(32) not null,
    last_name varchar(32) not null,
    third_name varchar(32) not null,
    document varchar(64) not null,
    series int not null,
    num int not null,
    date_of_creature DATE not null,
    creature_place VARCHAR(1024) not null,
    date_of_born date not null,
    photo_link varchar(1024) not null,
    leaving_addres varchar(512) not null,
    inspection_id int not null,
    organization_id int not null,

    constraint pk_taxpayer_id primary key (id),
    
    constraint fk_inspection_id
    FOREIGN key (inspection_id) REFERENCES inspections(id)
    on update cascade on delete cascade,

    constraint fk_organization_id
    FOREIGN key (organization_id) REFERENCES organizations(id)
    on update cascade on delete cascade
);
GO