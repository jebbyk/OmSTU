use omstu_taxes;

-- drop trigger restrict_initials, delete_taxpayers;

go 
create trigger restrict_initials on taxpayers 
after insert 
as 
update taxpayers 
set first_name = left(inserted.first_name, 1)
from inserted 
where taxpayers.id = inserted.id;
update taxpayers 
set third_name = left(inserted.third_name, 1)
from inserted 
where taxpayers.id = inserted.id;
go 


use omstu_taxes;
go 
create trigger delete_taxpayers on dbo.taxpayers 
for delete 
as 
if exists (select * from deleted where last_name = 'admin')
begin 
    print 'cannot delete this person'
    rollback TRANSACTION 
end 
go 

enable trigger all 
 on all server ;
