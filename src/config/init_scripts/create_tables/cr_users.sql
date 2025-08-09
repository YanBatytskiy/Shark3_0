create table if not exists users (
id serial primary key,
login varchar(30) not null,
name varchar(30) not null,
email varchar(100),
phone varchar(100)
);
