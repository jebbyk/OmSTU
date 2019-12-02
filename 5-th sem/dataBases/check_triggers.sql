use omstu_taxes; 
go 
-- disable trigger delete_taxpayers on all server;
delete from taxpayers where id = 666;
-- enable trigger delete_taxpayers on all server;
go 
