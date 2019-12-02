use omstu_taxes;
GO
select taxpayers.first_name, taxpayers.last_name, taxpayers.third_name, declarations.getted_summ
from taxpayers join declarations
on taxpayers.id = declarations.taxpayer_id and declarations.getted_summ > 30000;
go

