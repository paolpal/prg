#include "../protocols.h"

// ********************************************
// il protocollo di iscrizione trasmette al server
// la richiesta di iscrizione, il nome utente e
// la password.
// Attende quindi l'esito dal server...
// ********************************************
int signup_protocol_client(int sd, char* user, char* pw){
  int ret;
  uint16_t lmsg;
  char buffer[BUF_LEN];

  // FACCIO RICHIESTA DI ISCRIZIONE
  sprintf(buffer,"%s", "SGN");
  printf("<LOG-M> Invio richiesta di SIGNUP\n");
  ret = send_all(sd, (void*)buffer, REQ_LEN, 0);

  //INVIO LA LUNGHEZZA DEL NOME
  printf("<LOG-M> Invio lo USERNAME\n");
  int len = strlen(user)+1;
  lmsg = htons(len);
  ret = send_all(sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO IL NOME
  sprintf(buffer,"%s", user);
  ret = send_all(sd, (void*) buffer, len, 0);

  //INVIO LA LUNGHEZZA DELLA password
  printf("<LOG-M> Invio la PASSWORD\n");
  len = strlen(pw)+1;
  lmsg = htons(len);
  ret = send_all(sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO LA PASSWORD
  sprintf(buffer,"%s", pw);
  ret = send_all(sd, (void*) buffer, len, 0);

  ret = recv_all(sd, (void*) buffer, ACK_LEN, 0);
  printf("<LOG-M> Attendo il FEEDBACK dal SERVER\n");

  if(strcmp(buffer,"SIGNED")==0) return 1;
  else return 0;
}

// ********************************************
// il protocollo di login trasmette al server
// la richiesta di login, il nome utente,
// la password e la porta di ascolto.
// Attende quindi l'esito dal server...
// ********************************************
int login_protocol_client(int sd, char* user, char* pw, short port){
  int ret;
  uint16_t lmsg;
  char buffer[BUF_LEN];

  // FACCIO RICHIESTA DI LOGIN
  sprintf(buffer,"%s", "LIN");
  printf("<LOG-M> Invio richiesta di LOGIN\n");
  ret = send_all(sd, (void*)buffer, REQ_LEN, 0);


  //INVIO LA LUNGHEZZA DELLO USERNAME
  printf("<LOG-M> Invio lo USERNAME\n");
  int len = strlen(user)+1;
  lmsg = htons(len);
  ret = send_all(sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO LO USERNAME
  sprintf(buffer,"%s", user);
  ret = send_all(sd, (void*) buffer, len, 0);

  //INVIO LA LUNGHEZZA DELLA PASSWORD
  printf("<LOG-M> Invio la PASSWORD\n");
  len = strlen(pw)+1;
  lmsg = htons(len);
  ret = send_all(sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO LA PASSWORD
  sprintf(buffer,"%s", pw);
  ret = send_all(sd, (void*) buffer, len, 0);

  //INVIO PORTA
  lmsg = htons(port);
  printf("<LOG-M> Invio la PORTA di ascolto\n");
  ret = send_all(sd, (void*) &lmsg, sizeof(uint16_t), 0);

  printf("<LOG-M> Attendo il FEEDBACK dal SERVER\n");
  ret = recv_all(sd, (void*) buffer, ACK_LEN, 0);

  if(strcmp(buffer,"LOGGED")==0)return 1;
  else return 0;
}

// ********************************************
// il protocollo di logout trasmette al server
// la richiesta di logout e il nome utente.
// Attende quindi l'esito dal server...
// ********************************************
int logout_protocol_client(int sd, char* user){
  int ret;
  uint16_t lmsg;
  char buffer[BUF_LEN];

  // FACCIO RICHIESTA DI LOGOUT
  sprintf(buffer,"%s", "OUT");
  printf("<LOG-M> Invio richiesta di LOGOUT\n");
  ret = send_all(sd, (void*)buffer, REQ_LEN, 0);

  //INVIO LA LUNGHEZZA DEL NOME
  printf("<LOG-M> Invio lo USERNAME\n");
  int len = strlen(user)+1;
  lmsg = htons(len);
  ret = send_all(sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO IL NOME
  sprintf(buffer,"%s", user);
  ret = send_all(sd, (void*) buffer, len, 0);

  printf("<LOG-M> Attendo il FEEDBACK dal SERVER\n");
  ret = recv_all(sd, (void*) buffer, ACK_LEN, 0);

  if(strcmp(buffer,"EXITED")==0)return 1;
  else return 0;
}

// ************************************
// Ritorna la socket stabilita con il
// destinatario, o 0 se l'invio è fallito.
// [Il messaggio resta pendente sul server.]
// ************************************
int new_chat_protocol_client(int srv_sd, char* my_user, char* dest_user, struct sockaddr_in* dest_addr, char* msg, int* seq_n){
  int ret, len, cht_sd;
  uint16_t lmsg;
  short port;
  char buffer[BUF_LEN];

  sprintf(buffer,"%s", "CHT");
  printf("<LOG-C> Invio richiesta di CHAT\n");
  ret = send_all(srv_sd, (void*)buffer, REQ_LEN, 0);

  //INVIO LA LUNGHEZZA DEL NOME DESTINATARIO
  printf("<LOG-C> Invio lo USERNAME destinatario\n");
  len = strlen(dest_user)+1;
  lmsg = htons(len);
  ret = send_all(srv_sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO IL NOME DESTINATARIO
  sprintf(buffer,"%s", dest_user);
  ret = send_all(srv_sd, (void*) buffer, len, 0);

  //INVIO LA LUNGHEZZA DEL NOME MITTENTE
  printf("<LOG-C> Invio lo USERNAME mittente (IO)\n");
  len = strlen(my_user)+1;
  lmsg = htons(len);
  ret = send_all(srv_sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO IL NOME MITTENTE
  sprintf(buffer,"%s", my_user);
  ret = send_all(srv_sd, (void*) buffer, len, 0);

  //INVIO LA LUNGHEZZA DEL MESSAGGIO
  printf("<LOG-C> Invio il MESSAGGIO\n");
  len = strlen(msg)+1;
  lmsg = htons(len);
  ret = send_all(srv_sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO IL MESSAGGIO
  ret = send_all(srv_sd, (void*) msg, len, 0);

  //INVIO IL NUMERO DI SEQUENZA
  lmsg = htons(*seq_n);
  ret = send_all(srv_sd, (void*) &lmsg, sizeof(uint16_t), 0);
  // se il seq_n non è settato, ne aspetto uno dal server
  // che viene assegnato randomicamente
  if(*seq_n==0){
    printf("<LOG-C> Attendo il nuovo NUMERO SEQUENZIALE \n");
    recv_all(srv_sd, (void*)&lmsg, sizeof(uint16_t), 0);
    *seq_n = ntohs(lmsg);
  }

  //RICEVO LA PORTA DEL CONTATTO
  printf("<LOG-C> Attendo il FEEDBACK dal SERVER\n");
  ret = recv_all(srv_sd, (void*) &lmsg, sizeof(uint16_t), 0);
  port = ntohs(lmsg);

  if(port==0) return 0;

  printf("<LOG-C> Apro una connessione TCP con il CLIENT\n");
  cht_sd = socket(AF_INET, SOCK_STREAM, 0);

  memset(dest_addr, 0, sizeof(*dest_addr));
  dest_addr->sin_family = AF_INET;
  dest_addr->sin_port = htons(port);
  inet_pton(AF_INET, "127.0.0.1", &dest_addr->sin_addr);

  ret = connect(cht_sd, (struct sockaddr*)dest_addr, sizeof(*dest_addr));
  if(ret<0){
    return 0;
  }
  return cht_sd;
}

// ********************************************
// il protocollo di hanging chiede al server
// gli utenti che hanno provato a contattarmi
// quando ero ofline e quanti messaggi mi sono
// stati spediti
// ********************************************
void hanging_protocol_client(int sd, char* user){
  int ret, len, n_msg;
  uint16_t lmsg;
  char buffer[BUF_LEN];
  char *sender;

  //INVIO LA RICHIESTA DI HANGING
  printf("<LOG-M> Invio richiesta di HANGING\n");
  sprintf(buffer,"%s", "HNG");
  ret = send_all(sd, (void*)buffer, REQ_LEN, 0);

  //INVIO LA LUNGHEZZA DEL DESTINATARIO (current client)
  printf("<LOG-M> Invio lo USERNAME (IO)\n");
  len = strlen(user)+1;
  lmsg = htons(len);
  ret = send_all(sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO IL DESTINATARIO
  sprintf(buffer,"%s", user);
  ret = send_all(sd, (void*) buffer, len, 0);

  printf("<LOG-M> Attendo la lista di MITTENTI dal SERVER\n");

  // **********************
  // esco dal ciclo quando
  // ricevo una lunghezza
  // pari a 0
  // **********************
  while(1){
    //RICEVO LA LUNGHEZZA DEL NOME MITTENTE
    printf("<LOG-M> Attendo lo USERNAME MITTENTE\n");
    recv_all(sd, (void*)&lmsg, sizeof(uint16_t), 0);
    len = ntohs(lmsg);
    if(len == 0) break;
    sender = (char*) malloc(len*sizeof(char));
    //RICEVO IL NOME MITTENTE
    recv_all(sd, (void*)buffer, len, 0);
    sscanf(buffer, "%s", sender);

    //RICEVO IL NUMERO DI MESSAGGI
    printf("<LOG-M> Attendo il NUMERO dei MESSAGGI\n");
    recv_all(sd, (void*)&lmsg, sizeof(uint16_t), 0);
    n_msg = ntohs(lmsg);

    // RICEVO LA LUNGHEZZA DEL TIMESTAMP
    printf("<LOG-M> Attendo il TIMESTAMP\n");
    recv_all(sd, (void*)&lmsg, sizeof(uint16_t), 0);
    len = ntohs(lmsg);
    //RICEVO IL TIMESTAMP
    recv_all(sd, (void*)buffer, len, 0);

    printf("%s %d %s", sender, n_msg, buffer);
  }
  printf("<LOG-M> Fine ricezione\n");
}

// ********************************************
// il protocollo di show chiede al server
// i messaggi che uno specifico utente mi
// ha spedito. Una volta ricevuti, mando un'ACK
// di ricezione.
// ********************************************
void show_protocol_client(int sd, char* my_user, char* sender_user, struct chat** l_chat){
  int ret, len, seq_n;
  uint16_t lmsg;
  char buffer[BUF_LEN];
  char *msg_text;
  struct msg* msg;

  //INVIO LA RICHIESTA DI SHOW
  printf("<LOG-M> Invio richiesta di SHOW\n");
  sprintf(buffer,"%s", "SHW");
  ret = send_all(sd, (void*)buffer, REQ_LEN, 0);

  //INVIO LA LUNGHEZZA DEL DESTINATARIO (current client)
  printf("<LOG-M> Invio lo USERNAME destinatario (IO)\n");
  len = strlen(my_user)+1;
  lmsg = htons(len);
  ret = send_all(sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO IL DESTINATARIO
  sprintf(buffer,"%s", my_user);
  ret = send_all(sd, (void*) buffer, len, 0);

  //INVIO LA LUNGHEZZA DEL MITTENTE
  printf("<LOG-M> Invio lo USERNAME mittente\n");
  len = strlen(sender_user)+1;
  lmsg = htons(len);
  ret = send_all(sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO IL MITTENTE
  sprintf(buffer,"%s", sender_user);
  ret = send_all(sd, (void*) buffer, len, 0);

  printf("<LOG-M> Attendo la lista dei MESSAGGI dal SERVER\n");
  while(1){
    //RICEVO LA LUNGHEZZA DEL MESSAGGIO
    printf("<LOG-M> Attendo il MESSAGGIO\n");
    recv_all(sd, (void*)&lmsg, sizeof(uint16_t), 0);
    len = ntohs(lmsg);
    if(len == 0) break;
    msg_text = (char*) malloc(len*sizeof(char));
    //RICEVO IL MESSAGGIO
    recv_all(sd, (void*)buffer, len, 0);
    strcpy(msg_text,buffer);

    //RICEVO IL NUMERO SEQUENZIALE
    printf("<LOG-M> Ricevo il NUMERO di SEQUENZA\n");
    recv_all(sd, (void*)&lmsg, sizeof(uint16_t), 0);
    seq_n = ntohs(lmsg);

    //INVIO LA PROCEDURA DI MESSAGE ACK
    send_msg_ack_protocol_client(sd, my_user, sender_user, seq_n);

    msg = (struct msg*) malloc(sizeof(struct msg));
    msg->dest = NULL;
    len = strlen(sender_user)+1;
    msg->sender = (char*) malloc(len*sizeof(char));
    strcpy(msg->sender, sender_user);
    len = strlen(msg_text)+1;
    msg->text = (char*) malloc(len*sizeof(char));
    strcpy(msg->text, msg_text);
    msg->next = NULL;
    msg->seq_n = seq_n;

    add_msg(l_chat, msg);
    print_msg(msg);
  }
  printf("<LOG-M> Fine ricezione\n");
}

// *************************************
// il protocollo di ricezione del file
// riceve il filename dalla socket,
// quindi richiama l'apposita funzone
// di ricezione...
// *************************************
void receive_file_protocol_client(int sd){
  int ret, len;
  uint16_t lmsg;
  char buffer[BUF_LEN];
  char *filename;

  //RICEVO LA LUNGHEZZA DEL FILENAME
  printf("<LOG-M> RICEVO il FILENAME\n");
  ret = recv_all(sd, (void*)&lmsg, sizeof(uint16_t), 0);
  len = ntohs(lmsg);
  filename = (char*) malloc(len*sizeof(char));
  //RICEVO IL FILENAME
  ret = recv_all(sd, (void*)buffer, len, 0);
  sscanf(buffer, "%s", filename);

  //RICEVO IL FILE
  printf("<LOG-M> RICEVO il FILE\n");
  recv_file_b(sd, filename);

  free(filename);
}

// *********************
// Può diventare un processo a
// se stante per l'invio di
// un file, senza rallenatre la chat
//
// comodo per la chat di gruppo
// *********************
void send_file_protocol_client(struct sockaddr_in* dest_addr, char* filename){
  int ret, len, sd;
  uint16_t lmsg;
  char buffer[BUF_LEN];

  //APRO LA SOCKET DI INVIO
  printf("<LOG> Apro una connessione TCP con il CLIENT per il TRASFERIMENTO\n");
  sd = socket(AF_INET, SOCK_STREAM, 0);
  ret = connect(sd, (struct sockaddr*)dest_addr, sizeof(*dest_addr));
  if(ret<0){
    return;
  }

  //INVIO LA RICHIESTA DI SHARE
  printf("<LOG> Invio richiesta di SHARE FILE\n");
  sprintf(buffer,"%s", "SHR");
  ret = send_all(sd, (void*)buffer, REQ_LEN, 0);

  //INVIO LA LUNGHEZZA DEL FILENAME
  printf("<LOG> Invio il FILENAME\n");
  len = strlen(filename)+1;
  lmsg = htons(len);
  ret = send_all(sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO IL FILENAME
  sprintf(buffer,"%s", filename);
  ret = send_all(sd, (void*) buffer, len, 0);

  //INVIO IL FILE
  printf("<LOG> Invio il FILE\n");
  send_file_b(filename, sd);

  printf("<LOG> Chiudo la connessione TCP con il CLIENT per il TRASFERIMENTO\n");
  close(sd);
}

// ***********************************************
// chiedo al server l'elenco degli utenti ONLINE
// e la stampa...
// ***********************************************
void group_protocol_client(int srv_sd){
  int ret, len;
  uint16_t lmsg;
  char buffer[BUF_LEN];
  char *username;

  //INVIO LA RICHIESTA DI GROUP
  printf("<LOG-M> Invio richiesta di GROUP\n");
  sprintf(buffer,"%s", "GRP");
  ret = send_all(srv_sd, (void*)buffer, REQ_LEN, 0);

  while(1){
    //RICEVO LA LUNGHEZZA DELLO USERNAME
    printf("<LOG-M> Ricevo lo USERNAME\n");
    recv_all(srv_sd, (void*)&lmsg, sizeof(uint16_t), 0);
    len = ntohs(lmsg);
    if(len == 0) break;
    username = (char*) malloc(len*sizeof(char));

    //RICEVO LO USERNAME
    recv_all(srv_sd, (void*)buffer, len, 0);
    strcpy(username, buffer);

    printf("%s\n", username);
  }
  printf("<LOG-M> Fine ricezione\n");
}

// **************************************************
// Il protocollo di richiesta di aggiunta, trasmette
// la richiesta e uno username su una socket.
// La richiesta è trasmessa a tutti gli utenti
// di una chatroom...
// **************************************************
void add_user_request_protocol_client(int cht_sd, char* username){
  int len, ret;
  uint16_t lmsg;
  char buffer[BUF_LEN];

  //INVIO LA RICHIESTA DI AGGIUNTA USERNAME
  printf("<LOG-M> Invio richiesta di ADD USER\n");
  sprintf(buffer,"%s", "ADD");
  ret = send(cht_sd, (void*)buffer, REQ_LEN, 0);

  //INVIO LA LUNGHEZZA DELLO USERNAME
  printf("<LOG-M> Invio lo USERNAME\n");
  len = strlen(username)+1;
  lmsg = htons(len);
  ret = send(cht_sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO LO USERNAME
  sprintf(buffer,"%s", username);
  ret = send(cht_sd, (void*) buffer, len, 0);
}

// **************************************************
// in risposta alla richiesta di aggiunta, ricevo il
// nome utente e contact il CHATTING PROCESS per
// aggiungerlo alla CHATROOM...
// **************************************************
void add_user_protocol_client(int sd, struct user** chatroom_ref, int chatting, char* c_username){
  int ret, len;
  uint16_t lmsg;
  uint32_t len32;
  char buffer[BUF_LEN];
  char *username;

  //RICEVO LA LUNGHEZZA DELLO USERNAME
  printf("<LOG-M> Ricevo lo USERNAME\n");
  ret = recv_all(sd, (void*)&lmsg, sizeof(uint16_t), 0);
  len = ntohs(lmsg);
  username = (char*) malloc(len*sizeof(char));
  //RICEVO LO USERNAME
  ret = recv_all(sd, (void*)buffer, len, 0);
  sscanf(buffer, "%s", username);

  //AGGIUNGO L'UTENTE AL CHATTING PROCESS
  printf("<LOG-M> Aggiungo lo USERNAME alla CHATROOM\n");

  if(chatting){
    if(strcmp(c_username, username)!=0){
      printf("<LOG-C> Aggiungo lo username alla CHATROOM\n");
      append_user(&chatroom, username);
    }
  }
  free(username);
}

// ******************************************
// la richiesta di uscita dalla chattingroom
// manda il proprio username a tutti gli
// utenti in ascolto...
// ******************************************
void leave_chatroom_request_protocol_client(int cht_sd, char* my_username){
  int len, ret;
  uint16_t lmsg;
  char buffer[BUF_LEN];

  //INVIO LA RICHIESTA DI USCITA
  printf("<LOG> Invio richiesta di LEAVE\n");
  sprintf(buffer,"%s", "BEY");
  ret = send(cht_sd, (void*)buffer, REQ_LEN, 0);

  //INVIO LA LUNGHEZZA DELLO USERNAME
  printf("<LOG> Invio lo USERNAME (IO)\n");
  len = strlen(my_username)+1;
  lmsg = htons(len);
  ret = send(cht_sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO LO USERNAME
  sprintf(buffer,"%s", my_username);
  ret = send(cht_sd, (void*) buffer, len, 0);
}

// ******************************************
// la richiesta di uscita è servita con
// questo protocollo, riceve uno USERNAME
// e lo comunica al CHATTING PROCESS che
// si coccupa di rimuoverlo
// ******************************************
void leave_chatroom_protocol_client(int sd, struct user** chatroom_ref, int chatting){
  int ret, len;
  uint16_t lmsg;
  uint32_t len32;
  char buffer[BUF_LEN];
  char *username;

  //RICEVO LA LUNGHEZZA DELLO USERNAME
  printf("<LOG> Ricevo lo USERNAME\n");
  ret = recv_all(sd, (void*)&lmsg, sizeof(uint16_t), 0);
  len = ntohs(lmsg);
  username = (char*) malloc(len*sizeof(char));
  //RICEVO LO USERNAME
  ret = recv_all(sd, (void*)buffer, len, 0);
  sscanf(buffer, "%s", username);

  //RIMUOVO L'UTENTE DALLA CHATTING ROOM
  printf("<LOG> Rimuovo lo USERNAME dalla CHATROOM\n");

  // ***************************************
  // comunico con il CHATTING process
  // tramite le pipe
  // ***************************************
  if(chatting){
    printf("<LOG> Rimuovo lo username dalla CHATROOM\n");
    remove_user(&chatroom, buffer);
  }
  free(username);
}

// **************************************************
// quando un utente è aggiunto ad un gruppo, questo
// non è forzato ad accedere alla chat.
// per accedere ad un gruppo senza privare della
// possibilità di mandare messaggi individuali ai membri
// implemento la procedura di join:
// se apro una chat con un utente, che mi ha aggiunto ad un gruppo,
// posso fare la richiesta di sincronizzazione della chatroom
// altrimenti continuo a scrivere solo a lui
// **************************************************
void join_chatroom_request_protocol_client(int cht_sd, char* my_username, struct user** chatroom_ref){
  int len, ret;
  uint16_t lmsg;
  char buffer[BUF_LEN];
  char* username;

  //INVIO LA RICHIESTA DI UNIONE
  printf("<LOG-C> Invio richiesta di JOIN (SINCRONIZZAZIONE)\n");
  sprintf(buffer,"%s", "JNG");
  ret = send(cht_sd, (void*)buffer, REQ_LEN, 0);

  //INVIO LA LUNGHEZZA DEL MIO USERNAME
  printf("<LOG-C> Invio lo USERNAME (IO)\n");
  len = strlen(my_username)+1;
  lmsg = htons(len);
  ret = send(cht_sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO IL MIO USERNAME
  sprintf(buffer,"%s", my_username);
  ret = send(cht_sd, (void*) buffer, len, 0);

  // ASPETTO LA LISTA DI UTENTI DA AGGIUNGERE ALLA CHATROOM
  printf("<LOG-C> Attendo la lista di UTENTI da aggiungere alla CHATTING ROOM\n");
  while(1){
    printf("<LOG-C> Ricevo lo USERNAME\n");
    //RICEVO LA LUNGHEZZA DELLO USERNAME
    recv_all(cht_sd, (void*)&lmsg, sizeof(uint16_t), 0);
    len = ntohs(lmsg);
    if(len == 0) break;
    username = (char*) malloc(len*sizeof(char));
    //RICEVO LO USERNAME
    recv_all(cht_sd, (void*)buffer, len, 0);
    strcpy(username, buffer);

    // LO AGGIUNGO ALLA CHATROOM
    printf("<LOG-C> Aggiungo lo USERNAME alla CHATROOM\n");
    if(strcmp(username, my_username)!=0)
      append_user(chatroom_ref, username);
  }
}

// ************************************************
// In risposta ad una richiesta di join, devo valutare
// se l'utente è nella chatroom o no.
// In caso affermativo inizio a trasmettere tutti i nomi utente.
// altrimenti chiudo la trasmissione.
// ************************************************
void join_chatroom_protocol_client(int sd, struct user** chatroom_ref, int chatting){
  int ret;
  uint16_t lmsg;
  int len;
  char buffer[BUF_LEN];
  char *username;

  //RICEVO LA LUNGHEZZA DELLO USERNAME
  printf("<LOG> Ricevo lo USERNAME\n");
  ret = recv_all(sd, (void*)&lmsg, sizeof(uint16_t), 0);
  len = ntohs(lmsg);
  username = (char*) malloc(len*sizeof(char));
  //RICEVO LO USERNAME
  ret = recv_all(sd, (void*)buffer, len, 0);
  sscanf(buffer, "%s", username);

  if(chatting && chatting_with(buffer, chatroom)){
    user = *chatroom_ref;
    while(user != NULL){
      len = strlen(user->username)+1;
      lmsg = htons(len);
      printf("<LOG-M> Invio lo USERNAME\n");
      ret = send_all(sd, (void*) &lmsg, sizeof(uint16_t), 0);
      ret = send_all(sd, (void*) buffer, len, 0);
      user=user->next;
    }
    printf("<LOG-M> Fine trasmissione\n");
    lmsg = htons(0);
    ret = send_all(sd, (void*) &lmsg, sizeof(uint16_t), 0);
  }
}

// **********************************************************************
// Quando ricevo un messaggio non so su che port ascolta il mittente.
// il protocollo per mandare gli ACK di ricezione ai messaggi,
// passa attraverso il  server, che inoltra al mittente corretto l'ACK
// **********************************************************************
void send_msg_ack_protocol_client(int srv_sd, char *my_user, char *sender, int seq_n){
  int ret, len;
  uint16_t lmsg;
  char buffer[BUF_LEN];

  // FACCIO RICHIESTA DI MESSAGE ACK
  sprintf(buffer,"%s", "MAK");
  printf("<LOG-M> Invio richiesta di MESSAGE ACK\n");
  ret = send_all(srv_sd, (void*)buffer, REQ_LEN, 0);

  //INVIO LA LUNGHEZZA DELLO USERNAME MITTENTE
  printf("<LOG-M> Invio lo USERNAME mittente\n");
  len = strlen(sender)+1;
  lmsg = htons(len);
  ret = send_all(srv_sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO LO USERNAME MITTENTE
  sprintf(buffer,"%s", sender);
  ret = send_all(srv_sd, (void*) buffer, len, 0);

  //INVIO LA LUNGHEZZA DELLO USERNAME DESTINATARIO
  printf("<LOG-M> Invio lo USERNAME destinatario (IO)\n");
  len = strlen(my_user)+1;
  lmsg = htons(len);
  ret = send_all(srv_sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO LO USERNAME DESTINATARIO
  sprintf(buffer,"%s", my_user);
  ret = send_all(srv_sd, (void*) buffer, len, 0);

  //INVIO IL NUMERO DI SEQUENZA
  printf("<LOG-M> Invio il NUMERO di SEQUENZA\n");
  lmsg = htons(seq_n);
  ret = send_all(srv_sd, (void*) &lmsg, sizeof(uint16_t), 0);
}

// ************************************************
// ricevo la notifica di ricezione dal server
// quindi ricevo uno username e un numero di sequenza.
// Applico l'ACK al messaggio corretto.
// ************************************************
void recv_msg_ack_protocol_client(int sd, struct chat** l_chat_ref){
  int ret, seq_n, len;
  uint16_t lmsg;
  char buffer[BUF_LEN];
  char* username;

  //RICEVO LA LUNGHEZZA DELLO USERNAME DESTINATARIO
  printf("<LOG-M> Ricevo lo USERNAME\n");
  ret = recv_all(sd, (void*)&lmsg, sizeof(uint16_t), 0);
  len = ntohs(lmsg);
  username = (char*) malloc(len*sizeof(char));
  //RICEVO LO USERNAME DESTINATARIO
  ret = recv_all(sd, (void*)buffer, len, 0);
  sscanf(buffer, "%s", username);

  //RICEVO IL NUMERO DI SEQUENZA
  printf("<LOG-M> Ricevo il NUMERO di SEQUENZA\n");
  ret = recv_all(sd, (void*)&lmsg, sizeof(uint16_t), 0);
  seq_n = ntohs(lmsg);

  //APPLICO L'ACK AL MESSAGGIO
  acknoledge_message(l_chat_ref, username, seq_n);
}

// ********************************************************
// controllo se lo user che voglio contattare è online
// ********************************************************
int online_check_protocol_client(int srv_sd, char* username){
  int ret, len, val;
  uint16_t lmsg;
  char buffer[BUF_LEN];

  // FACCIO RICHIESTA DI CHECK ONLINE
  sprintf(buffer,"%s", "ONL");
  printf("<LOG-M> Invio richiesta di CHECK ONLINE\n");
  ret = send_all(srv_sd, (void*)buffer, REQ_LEN, 0);

  //INVIO LA LUNGHEZZA DELLO USERNAME
  printf("<LOG-M> Invio lo USERNAME\n");
  len = strlen(username)+1;
  lmsg = htons(len);
  ret = send_all(srv_sd, (void*) &lmsg, sizeof(uint16_t), 0);
  //INVIO LO USERNAME
  sprintf(buffer,"%s", username);
  ret = send_all(srv_sd, (void*) buffer, len, 0);

  //RICEVO LA VALUTAZIONE DAL SERVER
  printf("<LOG-M> Ricevo la VALUTAZIONE DEL SERVER\n");
  ret = recv_all(srv_sd, (void*)&lmsg, sizeof(uint16_t), 0);
  val = ntohs(lmsg);

  return val;
}
