use omstu_taxes;
go 
begin tran my_tran 
insert privileges values(4, 'veteran truda', 23453.25);
insert privileges values(5, 'jertva chernobylya', 37845.77);
save tran point_one;
insert privileges values(6, 'sirota', 9378.35);
rollback tran point_one;
commit tran my_tran;
select * from privileges;
go 
