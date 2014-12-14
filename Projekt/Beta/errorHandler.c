int errorHandler (int errorType)
{
    fprintf(stderr,"%sError %d%s\n",KRED,errorType,KNRM );
    mariuspedersen(&Smetisko);
    DisposeMarius(&Smetisko);
    exit(errorType);
}