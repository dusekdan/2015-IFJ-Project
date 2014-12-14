int errorHandler (int errorType)
{
    fprintf(stderr,"Chyba %d\n",errorType);
    mariuspedersen(&Smetisko);
    DisposeMarius(&Smetisko);
    exit(errorType);
}