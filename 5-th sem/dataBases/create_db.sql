use master;
go 
create database omstu_taxes;
go
select file_id as 'id',
type as 'type',
type_desc as 'description',
name as 'name',
state as 'state',
state_desc as 'state_desc',
size as 'size'
from sys.master_files 
where database_id = DB_ID('omstu_taxes');
go 
