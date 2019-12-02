use omstu_taxes;
go 

declare @new_cod as int; 
exec proc_taxpayers @new_cod output; 
print @new_cod 
go 

exec MyProc 1; 
exec MyProc 2;
exec MyProc 3;