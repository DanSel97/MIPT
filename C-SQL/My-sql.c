#include <string.h>
#include <stdio.h>
#include "sqlite3.h"
#include "ctype.h"
#define MY_DB "my_db.dblite"
#define MAX_NAME 32
const char* SQLcreate = "CREATE TABLE IF NOT EXISTS student (student_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, name_last VARCHAR(20) NOT NULL, name_first VARCHAR(20) NOT NULL, name_second VARCHAR(20)); "\

	"CREATE TABLE IF NOT EXISTS groups (group_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, group_number INTEGER NOT NULL, department_id INTEGER, study_begin DATE NOT NULL, study_end DATE, FOREIGN KEY (department_id) REFERENCES department (department_id));"\

	"CREATE TABLE IF NOT EXISTS department (department_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, department_name VARCHAR(20) NOT NULL);"\

	"CREATE TABLE IF NOT EXISTS education (student_id INTEGER NOT NULL, group_id INTEGER NOT NULL, [year] INTEGER NOT NULL, term INTEGER NOT NULL, PRIMARY KEY(student_id, term, [year]), FOREIGN KEY (group_id) REFERENCES groups (group_id), FOREIGN KEY (student_id) REFERENCES student (student_id));"\

	"CREATE TABLE IF NOT EXISTS teacher (teacher_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, name_last VARCHAR(20) NOT NULL, name_first VARCHAR(20) NOT NULL, name_second VARCHAR(20));"\

	"CREATE TABLE IF NOT EXISTS cathedra(cathedra_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, cathedra_name VARCHAR(40) NOT NULL);"\

	"CREATE TABLE IF NOT EXISTS [subject] (subject_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, subject_name VARCHAR(40) NOT NULL, cathedra_id INTEGER, FOREIGN KEY (cathedra_id) REFERENCES cathedra (cathedra_id));"\

	"CREATE TABLE IF NOT EXISTS type_delivery ([type_id] INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,  [type] VARCHAR(20) NOT NULL );"\

	"CREATE TABLE IF NOT EXISTS subject_delivery (subject_id INTEGER NOT NULL, group_id INTEGER NOT NULL, [date] DATE NOT NULL, [type_id] INTEGER, PRIMARY KEY([date], group_id, subject_id), FOREIGN KEY (group_id) REFERENCES groups (group_id), FOREIGN KEY (subject_id) REFERENCES [subject] (subject_id), FOREIGN KEY ([type_id]) REFERENCES type_delivery ([type_id]));"\

	"CREATE TABLE IF NOT EXISTS post (post_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, post_name VARCHAR(30) NOT NULL);"\

	"CREATE TABLE IF NOT EXISTS cathedra_teacher (teacher_id INTEGER NOT NULL, cathedra_id INTEGER NOT NULL, post_id INTEGER NOT NULL, post_begin DATE NOT NULL, post_end DATE, PRIMARY KEY (teacher_id, post_begin, cathedra_id), FOREIGN KEY (cathedra_id) REFERENCES cathedra (cathedra_id), FOREIGN KEY (teacher_id) REFERENCES teacher (teacher_id), FOREIGN KEY (post_id) REFERENCES post (post_id));"\

	"CREATE TABLE IF NOT EXISTS level_degree (level_degree_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, level_degree VARCHAR(20));"\
 
	"CREATE TABLE IF NOT EXISTS science_branch (science_branch_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, science_branch VARCHAR(20) NOT NULL);"\

	"CREATE TABLE IF NOT EXISTS teacher_degree (teacher_id INTEGER NOT NULL, science_branch_id INTEGER NOT NULL, level_degree_id INTEGER NOT NULL, degree_begin DATE NOT NULL, degree_end DATE, PRIMARY KEY (degree_begin, teacher_id), FOREIGN KEY (science_branch_id) REFERENCES science_branch (science_branch_id), FOREIGN KEY (teacher_id) REFERENCES teacher (teacher_id), FOREIGN KEY (level_degree_id) REFERENCES level_degree (level_degree_id));"\

	"CREATE TABLE IF NOT EXISTS number_class (number_class_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, number_class INTEGER NOT NULL, class_begin TIME NOT NULL, class_end TIME NOT NULL);"\

	"CREATE TABLE IF NOT EXISTS [day] (day_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, day_name VARCHAR(20) NOT NULL);"\

	"CREATE TABLE IF NOT EXISTS cabinet (cabinet_id INTEGER NOT NULL  PRIMARY KEY AUTOINCREMENT, cabinet_name VARCHAR(30) NOT NULL);"\

	"CREATE TABLE IF NOT EXISTS audience (audience_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, audience_name VARCHAR(30) NOT NULL, cabinet_id INTEGER NOT NULL, FOREIGN KEY (cabinet_id) REFERENCES cabinet (cabinet_id));"\

	"CREATE TABLE IF NOT EXISTS type_class (type_class_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, type_class VARCHAR(20) NOT NULL);"\

	"CREATE TABLE IF NOT EXISTS class (schedule_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, [year] INTEGER NOT NULL, term INTEGER NOT NULL, day_id INTEGER NOT NULL, number_class_id INTEGER NOT NULL, audience_id INTEGER NOT NULL, subject_id INTEGER NOT NULL, type_class_id INTEGER NOT NULL, FOREIGN KEY (subject_id) REFERENCES [subject] (subject_id), FOREIGN KEY (type_class_id) REFERENCES type_class (type_class_id), FOREIGN KEY (number_class_id) REFERENCES number_class (number_class_id), FOREIGN KEY (day_id) REFERENCES [day] (day_id), FOREIGN KEY (audience_id) REFERENCES audience (audience_id));"\

	"CREATE TABLE IF NOT EXISTS teacher_schedule (teacher_id INTEGER NOT NULL, schedule_id INTEGER NOT NULL, PRIMARY KEY (teacher_id, schedule_id), FOREIGN KEY (teacher_id) REFERENCES teacher (teacher_id), FOREIGN KEY (schedule_id) REFERENCES class (schedule_id));"\

	"CREATE TABLE IF NOT EXISTS group_schedule (group_id INTEGER NOT NULL, schedule_id INTEGER NOT NULL, PRIMARY KEY (group_id, schedule_id), FOREIGN KEY (group_id) REFERENCES groups (group_id), FOREIGN KEY (schedule_id) REFERENCES class (schedule_id));";

const char* SQLdrop = "DELETE FROM group_schedule; DROP TABLE group_schedule;"\
	"DELETE FROM teacher_schedule; DROP TABLE teacher_schedule;"\
	"DELETE FROM cathedra_teacher; DROP TABLE cathedra_teacher;"\
	"DELETE FROM teacher_degree; DROP TABLE teacher_degree;"\
	"DELETE FROM education; DROP TABLE education;"\
	"DELETE FROM subject_delivery; DROP TABLE subject_delivery;"\
	"DELETE FROM groups; DROP TABLE groups;"\
	"DELETE FROM class; DROP TABLE class;"\
	"DELETE FROM [subject]; DROP TABLE [subject];"\
	"DELETE FROM number_class; DROP TABLE number_class;"\
	"DELETE FROM type_class; DROP TABLE type_class;"\
	"DELETE FROM [day]; DROP TABLE [day];"\
	"DELETE FROM audience; DROP TABLE audience;"\
	"DELETE FROM cabinet; DROP TABLE cabinet;"\
	"DELETE FROM department; DROP TABLE department;"\
	"DELETE FROM type_delivery; DROP TABLE type_delivery;"\
	"DELETE FROM student; DROP TABLE student;"\
	"DELETE FROM cathedra; DROP TABLE cathedra;"\
	"DELETE FROM post; DROP TABLE post;"\
	"DELETE FROM science_branch; DROP TABLE science_branch;"\
	"DELETE FROM level_degree; DROP TABLE level_degree;"\
	"DELETE FROM teacher; DROP TABLE teacher";

const char* SQLinsert = "INSERT INTO [day] (day_name) VALUES ('Понедельник');"\
	"INSERT INTO [day] (day_name) VALUES ('Вторник');"\
	"INSERT INTO [day] (day_name) VALUES ('Среда');"\

	"INSERT INTO [day] (day_name) VALUES ('Четверг');"\
	"INSERT INTO [day] (day_name) VALUES ('Пятница');"\
	"INSERT INTO [day] (day_name) VALUES ('Суббота');"\
	"INSERT INTO [day] (day_name) VALUES ('Воскресенье');"\

	"INSERT INTO number_class (class_begin, class_end, number_class) VALUES ('09:00:00', '10:25:00', 1);"\
	"INSERT INTO number_class (class_begin, class_end, number_class) VALUES ('10:45:00', '12:10:00', 2);"\
	"INSERT INTO number_class (class_begin, class_end, number_class) VALUES ('12:20:00', '13:45:00', 3);"\
	"INSERT INTO number_class (class_begin, class_end, number_class) VALUES ('13:55:00', '15:20:00', 4);"\
	"INSERT INTO number_class (class_begin, class_end, number_class) VALUES ('15:30:00', '16:55:00', 5);"\
	"INSERT INTO number_class (class_begin, class_end, number_class) VALUES ('17:05:00', '18:30:00', 6);"\
	"INSERT INTO number_class (class_begin, class_end, number_class) VALUES ('18:30:00', '19:50:00', 7);"\

    "INSERT INTO type_class (type_class) VALUES ('Семинар');"\
    "INSERT INTO type_class (type_class) VALUES ('Лекция');"\
    "INSERT INTO type_class (type_class) VALUES ('Лабораторная работа');"\

    "INSERT INTO cabinet (cabinet_name) VALUES('Главный Корпус');"\
    "INSERT INTO cabinet (cabinet_name) VALUES('Новый Корпус');"\
    "INSERT INTO cabinet (cabinet_name) VALUES('Лабораторный корпус');"\
    "INSERT INTO cabinet (cabinet_name) VALUES('Корпус прикладной математики');"\
    "INSERT INTO cabinet (cabinet_name) VALUES('Спортивный корпус');"\

    "INSERT INTO audience (audience_name,cabinet_id) Values ('110',4);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('202',3);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('202',2);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('319',3);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('320',3);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('411',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('410',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('413',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('414',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('416',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('415',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('417',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('418',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('419',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('420',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('422',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('424',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('426',1);"\

    "INSERT INTO audience (audience_name,cabinet_id) Values ('432',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('512',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('513',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('516',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('518',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('520',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('522',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('523',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('525',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('526',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('527',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('528',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('531',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('701',4);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('706',4);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('801',4);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('802',4);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('806',4);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('Б. Физ.',3);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('Гл. Физ.',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('Б. Хим.',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('507a',1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('Бассейн',5);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('3 этаж', 2);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('3 этаж', 1);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('239',2);"\
    "INSERT INTO audience (audience_name,cabinet_id) Values ('532',1);"\

    "INSERT INTO cathedra (cathedra_name) VALUES ('Кафедра высшей математики');"\
    "INSERT INTO cathedra (cathedra_name) VALUES ('Кафедра радиоэлектроники и прикладной информатики');"\
    "INSERT INTO cathedra (cathedra_name) VALUES ('Кафедра информатики');"\
    "INSERT INTO cathedra (cathedra_name) VALUES ('Кафедра радиотехники');"\
    "INSERT INTO cathedra (cathedra_name) VALUES ('Кафедра физической культуры');"\

    "INSERT INTO post (post_name) VALUES ('Научный сотрудник');"\
    "INSERT INTO post (post_name) VALUES ('Старший научный сотрудник');"\
    "INSERT INTO post (post_name) VALUES ('Профессор');"\
    "INSERT INTO post (post_name) VALUES ('Доцент');"\
    "INSERT INTO post (post_name) VALUES ('Старший преподаватель');"\
    "INSERT INTO post (post_name) VALUES ('Преподаватель');"\
    "INSERT INTO post (post_name) VALUES ('Студент');"\
    "INSERT INTO post (post_name) VALUES ('Член-корреспондент РАН');"\

    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Романюк', 'Юрий', 'Андреевич');"\
    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Филимонов', 'Антон', 'Валерьевич');"\
    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Гимадеев', 'Ринат', 'Айратович');"\
    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Гудков', 'Кирилл', 'Сергеевич');"\
    "INSERT INTO teacher (name_last, name_first) VALUES ('Хайруллин', 'Ренат');"\
    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Козлов', 'Илья', 'Владимирович');"\
    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Чуканова', 'Ольга', 'Владимировна');"\
    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Рубцов', 'Александр', 'Александрович');"\
    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Тарасов', 'Сергей', 'Павлович');"\
    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Теймуразов', 'Кирилл', 'Борисович');"\
    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Пальян', 'Рубен', 'Левонович');"\
    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Бездушный', 'Анатолий', 'Николаевич');"\
    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Гордеев', 'Эдуард', 'Николаевич');"\
    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Дубинская', 'Вера', 'Юльевна');"\
    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Стожков', 'Владимир', 'Юрьевич');"\
    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Бесов', 'Олег', 'Владимирович');"\
    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Меденников', 'Антон', 'Михайлович');"\
    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Лунёв', 'Денис', 'Владимирович');"\
    "INSERT INTO teacher (name_last, name_first, name_second) VALUES ('Митрофанов', 'Анатолий', 'Владимирович');"\

    "INSERT INTO type_delivery([type]) VALUES ('зачёт');"\
    "INSERT INTO type_delivery ([type]) VALUES ('экзамен');"\

    "INSERT INTO [subject] (subject_name, cathedra_id) VALUES ('Электроника',4);"\
    "INSERT INTO [subject] (subject_name, cathedra_id) VALUES ('Базы данных',3);"\
    "INSERT INTO [subject] (subject_name, cathedra_id) VALUES ('Информатика',3);"\
    "INSERT INTO [subject] (subject_name, cathedra_id) VALUES ('Уравнения математической физики',1);"\
    "INSERT INTO [subject] (subject_name, cathedra_id) VALUES ('Гармонический анализ',1);"\
    "INSERT INTO [subject] (subject_name, cathedra_id) VALUES ('Физическая культура',5);"\
    "INSERT INTO [subject] (subject_name, cathedra_id) VALUES ('Дискретные преобразования сигналов',2);"\
    "INSERT INTO [subject] (subject_name, cathedra_id) VALUES ('Радиофизическая лаборатория',2);"\


    "INSERT INTO student (name_last, name_first, name_second) VALUES ('Беланова','Анастасия', 'Константиновна');"\
    "INSERT INTO student (name_last, name_first) VALUES ('Бондарев', 'Михаил');"\
    "INSERT INTO student (name_last, name_first) VALUES ('Григорик','Николай');"\
    "INSERT INTO student (name_last, name_first) VALUES ('Губанов', 'Сергей');"\
    "INSERT INTO student (name_last, name_first) VALUES ('Киселев','Василий');"\
    "INSERT INTO student (name_last, name_first) VALUES ('Литвинова','Евгения');"\
    "INSERT INTO student (name_last, name_first, name_second) VALUES ('Маркашанская','Дарья', 'Андреевна');"\
    "INSERT INTO student (name_last, name_first) VALUES ('Орлов','Олег');"\
    "INSERT INTO student (name_last, name_first) VALUES ('Орлов','Роман');"\
    "INSERT INTO student (name_last, name_first) VALUES ('Рябцев','Дмитрий');"\
    "INSERT INTO student (name_last, name_first) VALUES ('Соловьев','Александр');"\
    "INSERT INTO student (name_last, name_first) VALUES ('Ус','Александр');"\

    "INSERT INTO department (department_name) VALUES ('ФРТК');"\
    "INSERT INTO department (department_name) VALUES ('ФОПФ');"\
    "INSERT INTO department (department_name) VALUES ('ФАКИ');"\
    "INSERT INTO department (department_name) VALUES ('ФФКЭ');"\
    "INSERT INTO department (department_name) VALUES ('ФМХФ');"\
    "INSERT INTO department (department_name) VALUES ('ФАЛТ');"\
    "INSERT INTO department (department_name) VALUES ('ФУПМ');"\
    "INSERT INTO department (department_name) VALUES ('ФПФЭ');"\
    "INSERT INTO department (department_name) VALUES ('ФИВТ');"\
    "INSERT INTO department (department_name) VALUES ('ФБМФ');"\
    "INSERT INTO department (department_name) VALUES ('ФНБИК');"\

    "INSERT INTO groups (department_id, group_number, study_begin, study_end) VALUES (1, 411, '01/09/2014', '31/08/2017');"\
    "INSERT INTO groups (department_id, group_number, study_begin, study_end) VALUES (1, 412, '01/09/2014', '31/08/2017');"\
    "INSERT INTO groups (department_id, group_number, study_begin, study_end) VALUES (1, 413, '01/09/2014', '31/08/2017');"\
    "INSERT INTO groups (department_id, group_number, study_begin, study_end) VALUES (1, 414, '01/09/2014', '31/08/2017');"\
    "INSERT INTO groups (department_id, group_number, study_begin, study_end) VALUES (1, 415, '01/09/2014', '31/08/2017');"\
    "INSERT INTO groups (department_id, group_number, study_begin, study_end) VALUES (1, 416, '01/09/2014', '31/08/2017');"\
    "INSERT INTO groups (department_id, group_number, study_begin, study_end) VALUES (1, 417, '01/09/2014', '31/08/2017');"\
    "INSERT INTO groups (department_id, group_number, study_begin, study_end) VALUES (1, 418, '01/09/2014', '31/08/2017');"\
    "INSERT INTO groups (department_id, group_number, study_begin, study_end) VALUES (1, 419, '01/09/2014', '31/08/2017');"\

    "INSERT INTO education (student_id, term, [year], group_id) VALUES (1, 2, 2017, 8);"\
    "INSERT INTO education (student_id, term, [year], group_id) VALUES (2, 2, 2017, 8);"\
    "INSERT INTO education (student_id, term, [year], group_id) VALUES (3, 2, 2017, 8);"\
    "INSERT INTO education (student_id, term, [year], group_id) VALUES (4, 2, 2017, 8);"\
    "INSERT INTO education (student_id, term, [year], group_id) VALUES (5, 2, 2017, 8);"\
    "INSERT INTO education (student_id, term, [year], group_id) VALUES (6, 2, 2017, 8);"\
    "INSERT INTO education (student_id, term, [year], group_id) VALUES (7, 2, 2017, 8);"\
    "INSERT INTO education (student_id, term, [year], group_id) VALUES (8, 2, 2017, 8);"\
    "INSERT INTO education (student_id, term, [year], group_id) VALUES (9, 2, 2017, 8);"\
    "INSERT INTO education (student_id, term, [year], group_id) VALUES (10, 2, 2017, 8);"\
    "INSERT INTO education (student_id, term, [year], group_id) VALUES (11, 2, 2017, 8);"\
    "INSERT INTO education (student_id, term, [year], group_id) VALUES (12, 2, 2017, 8);"\

    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (5, 4, '30/05/2017', 1);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (5, 6, '03/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (5, 5, '07/05/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (5, 8, '22/05/2017', 1);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (5, 3, '11/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (5, 1, '15/06/2017', 2);"\

    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (2, 4, '30/05/2017', 1);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (2, 6, '03/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (2, 5, '07/05/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (2, 8, '22/05/2017', 1);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (2, 3, '11/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (2, 1, '15/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (2, 2, '28/05/2017', 1);"\

    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (1, 4, '30/05/2017', 1);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (1, 6, '03/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (1, 5, '07/05/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (1, 8, '22/05/2017', 1);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (1, 3, '11/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (1, 1, '15/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (1, 2, '30/05/2017', 1);"\

    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (3, 4, '30/05/2017', 1);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (3, 6, '03/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (3, 5, '07/05/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (3, 8, '22/05/2017', 1);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (3, 3, '11/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (3, 1, '15/06/2017', 2);"\

    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (4, 4, '30/05/2017', 1);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (4, 6, '03/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (4, 5, '07/05/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (4, 3, '22/05/2017', 1);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (4, 9, '11/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (4, 1, '15/06/2017', 2);"\

    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (6, 4, '30/05/2017', 1);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (6, 6, '03/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (6, 5, '07/05/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (6, 8, '22/05/2017', 1);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (6, 3, '11/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (6, 1, '15/06/2017', 2);"\

    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (7, 4, '30/05/2017', 1);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (7, 6, '03/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (7, 5, '07/05/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (7, 8, '22/05/2017', 1);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (7, 3, '11/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (7, 1, '15/06/2017', 2);"\

    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (8, 4, '30/05/2017', 1);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (8, 6, '03/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (8, 5, '07/05/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (8, 8, '22/05/2017', 1);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (8, 3, '11/06/2017', 2);"\
    "INSERT INTO subject_delivery (group_id, subject_id, [date], [type_id]) VALUES (8, 1, '15/06/2017', 2);"\

    "INSERT INTO science_branch (science_branch) VALUES ('Физика/математика');"\
    "INSERT INTO science_branch (science_branch) VALUES ('Техника');"\
    "INSERT INTO science_branch (science_branch) VALUES ('Педагогика');"\
    "INSERT INTO science_branch (science_branch) VALUES ('Филология');"\

    "INSERT INTO level_degree (level_degree) VALUES ('Кандидат');"\
    "INSERT INTO level_degree (level_degree) VALUES ('Доктор');"\

    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (1, 2, '15/06/2000', 3);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (2, 2, '15/06/2000', 3);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (3, 3, '15/06/2000', 6);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (4, 3, '15/06/2000', 6);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (5, 3, '15/06/2000', 6);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (6, 3, '15/06/2000', 6);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (7, 4, '15/06/2000', 6);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (8, 3, '15/06/2000', 5);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (9, 3, '15/06/2000', 4);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (10, 3, '15/06/2000', 5);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (11, 2, '15/06/2000', 6);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (12, 3, '15/06/2000', 4);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (13, 3, '15/06/2000', 3);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (14, 1, '15/06/2000', 4);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (15, 2, '15/06/2000', 6);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (16, 1, '15/06/2000', 8);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (17, 3, '15/06/2000', 6);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (18, 4, '15/06/2000', 4);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (19, 5, '15/06/2000', 6);"\
    "INSERT INTO cathedra_teacher (teacher_id, cathedra_id, post_begin, post_id) VALUES (8, 1, '15/06/2000', 6);"\

    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (1, '15/06/1970', 1, 2);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (2, '15/06/1970', 1, 1);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (3, '15/06/1970', 1, 1);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (4, '15/06/1970', 1, 1);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (5, '15/06/1970', 1, 1);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (6, '15/06/1970', 1, 1);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (7, '15/06/1970', 1, 1);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (8, '15/06/1970', 1, 1);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (9, '15/06/1970', 1, 1);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (10, '15/06/1970', 1, 1);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (11, '15/06/1970', 1, 1);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (12, '15/06/1970', 1, 1);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (13, '15/06/1970', 1, 2);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (14, '15/06/1970', 1, 1);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (15, '15/06/1970', 1, 1);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (16, '15/06/1970', 1, 2);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (17, '15/06/1970', 1, 1);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (18, '15/06/1970', 1, 1);"\
    "INSERT INTO teacher_degree (teacher_id, degree_begin, science_branch_id, level_degree_id) VALUES (19, '15/06/1970', 1, 1);"\

    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (1, 1, 1, 2, 25, 2017, 2);"\
    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (6, 2, 3, 2, 39, 2017, 2);"\

    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (4, 2, 1, 3, 37, 2017, 2);"\
    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (5, 1, 2, 3, 8, 2017, 2);"\
    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (2, 3, 3, 3, 2, 2017, 2);"\
    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (2, 2, 5, 3, 39, 2017, 2);"\

    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (5, 2, 1, 4, 37, 2017, 2);"\
    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (8, 2, 3, 4, 44, 2017, 2);"\
    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (6, 1, 5, 4, 10, 2017, 2);"\

    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (1, 3, 1, 5, 43, 2017, 2);"\
    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (1, 3, 2, 5, 43, 2017, 2);"\
    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (8, 1, 4, 5, 18, 2017, 2);"\
    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (3, 2, 5, 5, 39, 2017, 2);"\
    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (3, 1, 6, 5, 34, 2017, 2);"\
    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (3, 1, 7, 5, 34, 2017, 2);"\

    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (1, 2, 2, 6, 38, 2017, 2);"\
    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (8, 2, 3, 6, 44, 2017, 2);"\
    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (8, 1, 4, 6, 45, 2017, 2);"\

    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (3, 1, 6, 3, 34, 2017, 2);"\
    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (3, 1, 7, 3, 34, 2017, 2);"\

    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (1, 3, 2, 6, 38, 2017, 2);"\

    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (1, 1, 2, 6, 38, 2017, 2);"\
    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (1, 3, 3, 6, 39, 2017, 2);"\
    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (7, 3, 3, 6, 38, 2017, 2);"\

    "INSERT INTO class (subject_id, type_class_id, number_class_id, day_id, audience_id, [year], term) VALUES (3, 1, 3, 4, 38, 2017, 2);"\

    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (1, 2);"\

    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (2, 1);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (2, 2);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (2, 3);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (2, 4);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (2, 5);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (2, 6);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (2, 7);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (2, 8);"\

    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (3, 1);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (3, 2);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (3, 3);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (3, 4);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (3, 5);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (3, 6);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (3, 7);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (3, 8);"\

    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (4, 2);"\

    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (5, 2);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (5, 1);"\

    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (6, 1);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (6, 2);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (6, 3);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (6, 4);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (6, 5);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (6, 6);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (6, 7);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (6, 8);"\

    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (7, 1);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (7, 2);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (7, 3);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (7, 4);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (7, 5);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (7, 6);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (7, 7);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (7, 8);"\

    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (8, 1);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (8, 2);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (8, 3);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (8, 4);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (8, 5);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (8, 6);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (8, 7);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (8, 8);"\

    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (9, 2);"\

    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (10, 2);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (11, 2);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (12, 2);"\

    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (13, 1);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (13, 2);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (13, 3);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (13, 4);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (13, 5);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (13, 6);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (13, 7);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (13, 8);"\

    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (14, 2);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (15, 2);"\

    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (16, 1);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (16, 2);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (16, 3);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (16, 4);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (16, 5);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (16, 6);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (16, 7);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (16, 8);"\

    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (17, 1);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (17, 2);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (17, 3);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (17, 4);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (17, 5);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (17, 6);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (17, 7);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (17, 8);"\

    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (18, 2);"\

    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (19, 7);"\
    "INSERT INTO group_schedule (schedule_id, group_id) VALUES (20, 7);"\

    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (1, 15);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (2, 16);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (3, 13);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (4, 14);"\

    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (5, 4);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (5, 5);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (5, 10);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (5, 17);"\

    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (6, 12);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (7, 14);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (8, 2);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (9, 14);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (10, 15);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (11, 15);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (12, 11);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (13, 18);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (14, 7);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (15, 7);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (16, 1);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (17, 9);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (18, 3);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (18, 6);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (19, 7);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (20, 7);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (21, 1);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (22, 1);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (23, 1);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (24, 1);"\
    "INSERT INTO teacher_schedule (schedule_id, teacher_id) VALUES (25, 2);";

const char* SQLA = "SELECT [groups].group_number, [groups].study_begin, [groups].study_end, [groups].department_id FROM [groups], department WHERE department_name LIKE 'ФРТК'";

const char* SQLB = "SELECT teacher.name_last, teacher.name_first, teacher.name_second, [subject].subject_name, type_class.type_class, [day].day_name, number_class.number_class, number_class.class_begin, number_class.class_end, audience.audience_name, cabinet.cabinet_name "\
	"FROM teacher, teacher_schedule, class, [subject], type_class, number_class, [day], audience, cabinet"\
	"WHERE teacher.teacher_id = teacher_schedule.teacher_id "\
	"AND teacher_schedule.schedule_id = class.schedule_id "\
	"AND class.subject_id = [subject].subject_id "\
	"AND class.type_class_id = type_class.type_class_id "\
	"AND class.number_class_id = number_class.number_class_id "\
	"AND class.day_id = [day].day_id "\
	"AND class.audience_id = audience.audience_id "\
	"AND audience.cabinet_id = cabinet.cabinet_id "\
	"AND class.[year] = 2017 "\
	"AND class.term = 2 "\
	"AND teacher_schedule.teacher_id = 1 "\
	"AND teacher.teacher_id = teacher_schedule.teacher_id "\
	"AND teacher.name_last = 'Романюк' "\
	"GROUP BY teacher.name_last, teacher.name_first, teacher.name_second, [subject].subject_name, type_class.type_class, [day].day_name, 			number_class.number_class, number_class.class_begin, number_class.class_end, audience.audience_name, cabinet.cabinet_name "\
	"ORDER BY teacher.name_last, teacher.name_first, teacher.name_second, [subject].subject_name, type_class.type_class, [day].day_name, number_class.number_class, number_class.class_begin, number_class.class_end, audience.audience_name, cabinet.cabinet_name";

const char* SQLC = "UPDATE department SET department_name = 'ФРКТ' WHERE department_name = 'ФРТК';";

const char* SQLD = "SELECT [groups].group_number, [groups].study_begin, [groups].study_end, [groups].department_id "\
	"FROM [groups], department "\
	"WHERE[groups].department_id = department.department_id "\
	"AND department_name LIKE 'ФРКТ';";

const char* SQLE = "SELECT * from student;";

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	(void)NotUsed;
	int i;
	for (i = 0; i < argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

void selectE_db(sqlite3 *db)
{
	printf("Студенты в базе данных University: \n");
	printf("------------\n");
	const char* data = "Callback function called";
	char *err = 0;
	if (sqlite3_exec(db, SQLE, callback, (void*)data, &err))
	{
		fprintf(stderr, "Ошибка SQL-select: %s\n", err);
		sqlite3_free(err);
	}
	printf("------------\n");
}

void create_db(sqlite3 *db)
{
	printf("------------\n");
	printf("Создаётся база данных University...\n");
	const char* data = "Callback function called";
	char *err = 0;
	if (sqlite3_exec(db, SQLcreate, callback, (void*)data, &err))
	{
		fprintf(stderr, "Ошибка SQL-create: %s\n", err);
		sqlite3_free(err);
	}
	printf("База данных University создана!\n");
	printf("------------\n");
}

void drop_db(sqlite3 *db)
{
	printf("------------\n");
	printf("База данных University удаляется...\n");
	const char* data = "Callback function called";
	char *err = 0;
	if (sqlite3_exec(db, SQLdrop, callback, (void*)data, &err))
	{
		fprintf(stderr, "Ошибка SQL-drop: %s\n", err);
		sqlite3_free(err);
	}
	printf("База данных University удалена!\n");
	printf("------------\n");
}

void insert_db(sqlite3 *db)
{
	printf("------------\n");
	printf("База данных University заполняется данными...\n");
	char *err = 0;
	if (sqlite3_exec(db, SQLinsert, callback, 0, &err))
	{
		fprintf(stderr, "Ошибка SQL-insert: %s\n", err);
		sqlite3_free(err);
	}
	printf("База данных University заполнена данными\n");
	printf("------------\n");
}

void insert_student_db(sqlite3 *db)
{
	printf("Введите фамилию студента, которого хотите добавить в базу данных:\n");
	char name_last[MAX_NAME];
	fgets(name_last, MAX_NAME, stdin);
	printf("Введите имя студента, которого хотите добавить в базу данных:\n");
	char name_1st[MAX_NAME];
	fgets(name_1st, MAX_NAME, stdin);
	printf("Введите имя отчество, которого хотите добавить в базу данных:\n");
	char name_2nd[MAX_NAME];
	fgets(name_2nd, MAX_NAME, stdin);
	sqlite3_stmt* stmt;
	const char* SQL_insert_student = "INSERT INTO student (name_last, name_first, name_second) VALUES (?,?,?)";
	if (sqlite3_prepare(db,SQL_insert_student, strlen(SQL_insert_student),&stmt,0)!= SQLITE_OK) {
			printf("\nCould not prepare statement.");
			return;
		}
	if (sqlite3_bind_text(stmt, 1, name_last, strlen(name_last), 0) != SQLITE_OK) {
			printf("\nCould not bind text.\n");
			return;
		}
	if (sqlite3_bind_text(stmt, 2, name_1st, strlen(name_1st), 0) != SQLITE_OK) {
			printf("\nCould not bind text.\n");
			return;
		}
	if (sqlite3_bind_text(stmt, 3, name_2nd, strlen(name_2nd), 0) != SQLITE_OK) {
			printf("\nCould not bind text.\n");
			return;
		}
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		printf("\nCould not step (execute) stmt.\n");
		return;
	}
	/*if (sqlite3_exec(db, SQL_insert_student, callback, (void*)data, &err))
	{
		fprintf(stderr, "Ошибка SQL-insert: %s\n", err);
		sqlite3_free(err);
	}*/
	printf("Студент %s %s %s успешно добавлен в базу данных University!\n", name_last, name_1st, name_2nd);
	printf("------------\n");
}


int main()
{
	sqlite3 *db = 0;
	int rc = sqlite3_open("mydb_db.dblite", &db);
	create_db(db);
	insert_db(db);
	selectE_db(db);
	insert_student_db(db);
	selectE_db(db);
	drop_db(db);
	return 0;
}


