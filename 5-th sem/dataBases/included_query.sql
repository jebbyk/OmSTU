use omstu_taxes;
GO
select inspections.inspection_number from inspections
where not exists(select * from taxpayers
        where taxpayers.inspection_id = inspections.id 
        and taxpayers.third_name = 'Alexandrovich'
     );
go

