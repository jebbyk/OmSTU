use omstu_taxes;
go 
if OBJECT_ID('proc_taxpayers', 'P') is not NULL
    drop procedure proc_taxpayers;
go 
create procedure proc_taxpayers
    @cod int OUTPUT
AS
begin  
    set @cod = (select max(id) from taxpayers);
END
go





if object_id('MyProc', 'P') is not NULL
    drop procedure MyProc; 
go 

create procedure MyProc 
    @taxpayer_id int 
as 
begin 
    declare @user as varchar(50), @firstName varchar(20),
        @lastName varchar(20);
    select @firstName = first_name, @lastName = last_name
    from taxpayers 
    where id = @taxpayer_id;
    set @user = @firstName + ' ' + @LastName + '!'
    if(@firstName like '%a') 
        print 'Dorogaya ' + @user 
    else 
        print 'Dorogoy ' + @user; 
end 
go