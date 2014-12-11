var 
    a:integer;
    aa: integer;
    aaa: integer;
    result: integer;

function kokotko (hevno:integer,picus:integer):integer;
begin
    aaa:=aaa+1;
    kokotko:= hevno + picus;
    if aaa < 5 then
        begin
            kokotko:=kokotko(aaa,aa)
        end
        else
        begin
        end
    
end;


begin
	a:=2;
    aa:=5;
    aaa:=0;
    result:=kokotko(a,aa)


{while a < 5
    do
    begin
    a:=a+1
    end}
end.