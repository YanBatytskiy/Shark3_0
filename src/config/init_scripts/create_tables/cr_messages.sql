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
