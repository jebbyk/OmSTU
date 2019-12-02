use omstu_taxes;
go 

create view list_taxpayers_view
as
select concat(last_name, ' ',
substring(first_name, 1,1), '. ',
substring(third_name, 1,1), '.')
as [initials]
from taxpayers;
go

create view summ_without_taxes_view
as
select taxpayers.last_name, declarations.getted_summ - declarations.income_tax as summ
from taxpayers join  declarations 
on taxpayers.id = declarations.taxpayer_id
go