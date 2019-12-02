set implicit_transactions on;
go 
use omstu_taxes;
go 

insert privileges values(10, 'another one privilege', 666.66);
insert privileges values(11, 'The last privilege', 333.33);
save tran point_one;
insert privileges values(12, 'fake privilege', 999999999.99);
rollback tran point_one;
commit tran;
select * from privileges;
go