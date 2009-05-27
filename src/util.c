int isalpha(int c){
    return (('a' <= c &&  c <= 'z' )||('A' <= c && c <= 'Z'));
}

int isdigit(int c){
    return ('0' <= c && c <= '9');
}

int isalphanum(int c){
  return isdigit(c) && isalpha(c);
}
