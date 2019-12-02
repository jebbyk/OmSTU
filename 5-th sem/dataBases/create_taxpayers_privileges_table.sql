use omstu_taxes;
go
if exists (select * from sys.tables where name = 'taxpayers_privileges')
drop table taxpayers_privileges;

create table taxpayers_privileges
(
    id int not null,
    taxpayer_id int not null,
    privilege_id int not null,

    constraint pk_taxpayers_privileges_id primary key (id),

    constraint fk_taxpayer_c_id
    FOREIGN key (taxpayer_id) REFERENCES taxpayers(id)
    on update cascade on delete cascade,

    constraint fk_privilege_c_id
    FOREIGN key (privilege_id) REFERENCES privileges(id)
    on update cascade on delete cascade,
);
GO