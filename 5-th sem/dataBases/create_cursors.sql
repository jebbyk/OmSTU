use omstu_taxes;
go 
declare @name varchar(20), @third_name varchar(20), @last_name varchar(20), @message varchar(50);
print 'list of taxpayers with pasports: ';
declare cur cursor local for 
    select first_name, third_name, last_name 
    from taxpayers 
    where document = 'pasport';
open cur; 
fetch next from cur into @name, @third_name, @last_name;
while @@fetch_status = 0 
begin 
    select @message = @name + ' ' + @third_name + ' ' + @last_name;
    print @message;
    fetch next from cur into @name, @third_name, @last_name; 
end; 
close cur; 
deallocate cur;
print @name;
go 


-- if object_id('has_pasport_filter', 'P') is not null 
--     drop procedure has_pasprot_filter; 
-- go 
-- create procedure has_pasport_filter
-- as 
-- begin  
--     declare @name varchar(20), @third_name varchar(20), @message varchar(50);
--     print 'list of taxpayers with pasports: ';

--     declare cur cursor local for 
--         select first_name, third_name 
--         from taxpayers 
--         where document = 'pasport';

--         open cur; 
--             fetch next from cur into @name, @third_name;
--             while @@fetch_status = 0 
--             begin 
--                 select @message = @name + ' ' + @third_name;
--                 print @message;
--                 fetch next from cur into @name, @third_name; 
--             end; 
--         close cur; 
--     deallocate cur; 
--     print @name;
-- end; 
-- go 


  