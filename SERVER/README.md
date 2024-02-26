Pentru compilarea sursei ---> g++ server.cpp -o server `mysql_config --libs` -fno-stack-protector

Tabelele din baza de date 

Tabel User:
+----------+-------------+------+-----+---------+----------------+
| Field    | Type        | Null | Key | Default | Extra          |
+----------+-------------+------+-----+---------+----------------+
| id       | int         | NO   | PRI | NULL    | auto_increment |
| username | varchar(50) | YES  |     | NULL    |                |
| password | varchar(50) | YES  |     | NULL    |                |
| status   | tinyint     | YES  |     | NULL    |                |
+----------+-------------+------+-----+---------+----------------+

Tables Messages
+------------------+-------------+------+-----+---------+----------------+
| Field            | Type        | Null | Key | Default | Extra          |
+------------------+-------------+------+-----+---------+----------------+
| id               | int         | NO   | PRI | NULL    | auto_increment |
| message          | text        | YES  |     | NULL    |                |
| id_writer        | varchar(50) | YES  |     | NULL    |                |
| id_receiver      | varchar(50) | YES  |     | NULL    |                |
| timestamp        | datetime    | YES  |     | NULL    |                |
| reply_status     | int         | NO   |     | NULL    |                |
| ancestor_message | text        | YES  |     | NULL    |                |
+------------------+-------------+------+-----+---------+----------------+

