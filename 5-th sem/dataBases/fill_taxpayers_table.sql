use omstu_taxes;
GO
delete from taxpayers;
insert into taxpayers VALUES
(1,'111 111 111', 'Churkin', 'Boris', 'Sergeevich','pasport', 128, 256, '05/01/2018',
 'Omsk third street', '05/01/2000', 'http://photo/1', 'Omsk thirst street', 1, 1),

(2, '222 222 222', 'Gorbunov', 'Ivan', 'Petrovich', 'pasport', 256, 512, '04/01/2018',
 'Omsk fourth street', '04/01/2000', 'http://photo/2', 'Omsk second street', 2, 2),

(3, '333 333 333', 'Ivanov', 'Alexey', 'Alexandrovich', 'Ne pasport', 512, 256, '03/01/2018', 
'Moskow third street', '03/01/2000', 'http://photo/3', 'Moskow first street', 3, 3),

(4, '444 444 444', 'Smirnov', 'Sergey', 'Alexandrovich', 'Bumajka', 256, 128, '02/01/2018', 
'Moskow fourth street', '02/01/2000', 'http://photo/4', 'Moskow second street',4, 4),

(5, '555 555 555', 'Zaitsev', 'Vladimir', 'Alexandrovich', 'pasport', 256, 256, '01/01/2018',
 'Other derevnya street', '01/01/2000', 'http://photo/5', 'Derevnya street', 5, 5);

GO