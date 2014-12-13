var a : string;
    intneger : integer;

function writeln (s:string) : integer ;
begin
    write(s,''#10'');
    writeln:=0+666
end;

begin
    a:='hello world!';
    write(a,''#10'','oo ano funguje to',''#10'');
    write('this is an example of a direct write',''#10'',a,a,a,a,''#10'');
    write(''#10'',''#10'',''#10'');


    intneger:=writeln('kokoooooooooooot');
    write('intneger je ',intneger,''#10'')
end.