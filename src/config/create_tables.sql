create table if not exists chats (
id serial primary key
);

create table if not exists users (
id serial primary key,
login varchar(30) not null,
name varchar(30) not null,
email varchar(100),
phone varchar(100)
);

create table if not exists chatlist (
user_id integer not null,
chat_id integer not null,
constraint user_id_fk foreign key (user_id) references users(id),
constraint chat_id_fk foreign key (chat_id) references chats(id)
);

create table  if not exists messages (
id serial primary key,
chat_id integer not null,
sender_id integer not null,
message_text text not null,
create_date date not null,
time_stamp timestamp not null,
constraint chat_id_fk foreign key (chat_id) references chats(id),
constraint sender_id_fk foreign key (sender_id) references users(id)
);

create table if not exists message_status (
message_id integer not  null,
chat_id integer not null,
user_id integer not null,
status varchar(20) not null,
status_deleted boolean default false,
constraint status_check check (status in ('NOT DELIVERED','DELIVERED', 'READ')),
constraint message_id_fk foreign key (message_id) references messages(id),
constraint user_id_fk foreign key (user_id) references users(id),
constraint chat_id_fk foreign key (chat_id) references chats(id)
);

create table if not exists users_passhash (
user_id integer not null,
password_hash varchar(255),
constraint user_id_fk foreign key (user_id) references users(id)
);

create table if not exists participants (
chat_id integer not null,
user_id integer not null,
last_read_message_id integer not null,
deleted_from_chat boolean default false,
constraint chat_id_fk foreign key (chat_id) references chats(id),
constraint user_id_fk foreign key (user_id) references users(id),
constraint message_id_fk foreign key (last_read_message_id) references messages(id)
)
