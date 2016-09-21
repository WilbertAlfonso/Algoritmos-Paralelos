typedef struct {
    int Escritores;
    int EscritoresEsp;
    int Lectores;
    pthread_mutex_t mutex;
    pthread_cond_t AccesoEs;
    pthread_cond_t AccesoLec;
}mutex_rw;

int mutex_create( mutex_rw* x){
    x->Lectores=0;
    x->Escritores=0;
    x->EscritoresEsp=0;
    pthread_cond_init(&(x->AccesoLec),NULL);
    pthread_cond_init(&(x->AccesoEs),NULL);
    pthread_mutex_init(&(x->mutex),NULL);
    return 1;
}      
void mutex_rdlock( mutex_rw* x){
    pthread_mutex_lock(&(x->mutex));
    while(x->Escritores > 0)
        {
            pthread_cond_wait(&(x->AccesoLec), &(x->mutex));
        }
    x->Lectores++;
    pthread_mutex_unlock(&(x->mutex));   
   
}
void mutex_rdunlock( mutex_rw* x){
    pthread_mutex_lock(&(x->mutex));
        x->Lectores--;
        if(x->Lectores==0 && x->EscritoresEsp>0)
            pthread_cond_signal(&(x->AccesoEs));
        pthread_mutex_unlock(&(x->mutex));
}
void mutex_wrlock( mutex_rw* x)
{
        pthread_mutex_lock(&(x->mutex));
        x->EscritoresEsp++;
        while(x->Lectores>0 || x->Escritores>0)
        {
            pthread_cond_wait(&(x->AccesoEs), &(x->mutex));
        }
        x->EscritoresEsp--; x->Escritores++;
        pthread_mutex_unlock(&(x->mutex));
}
void mutex_wrunlock( mutex_rw* x)
{
        pthread_mutex_lock(&(x->mutex));
        x->Escritores--;
        if(x->EscritoresEsp>0)
            pthread_cond_signal(&(x->AccesoEs));
        pthread_cond_broadcast(&(x->AccesoLec));
        pthread_mutex_unlock(&(x->mutex));
}