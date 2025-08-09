create table if not exists users_passhash (
user_id integer not null,
password_hash varchar(255),
constraint user_id_fk foreign key (user_id) references users(id)
);
