var 
    a:integer;
    result: integer;

function kokotko ():integer;
begin
    kokotko:=123;
    a:=a+1;
    if a < 3 then
            begin
                kokotko:=kokotko()
            end
            else

            begin
            end;
   kokotko:=kokotko+a
end;


begin
	a:=0;
    result:=kokotko()
end.


{132 bre}