create table if not exists chatlist (
user_id integer not null,
chat_id integer not null,
constraint user_id_fk foreign key (user_id) references users(id),
constraint chat_id_fk foreign key (chat_id) references chats(id)
);
