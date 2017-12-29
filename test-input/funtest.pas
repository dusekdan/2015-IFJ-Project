{ Program 2: Vypocet faktorialu (rekurzivne) }
var
    a : integer;
    vysl : integer;
    realis : real;
    strigo : string;
{ Definice funkce pro vypocet hodnoty faktorialu }
function factorial(n : integer, n2 : string, n3 : real) : integer; forward;

function placeholderfunction (a:string):string; forward;

function factorial(n : integer, n2 : string, n3 : real) : integer;
var
    temp_result : integer;
    decremented_n : integer;
    bulo: boolean;
begin
    if n < 2 then
begin
    factorial := 1
end
else
    begin
        decremented_n := n - 1;
        temp_result := factorial(decremented_n,n2,realis);
        factorial := n * temp_result
    end
end;

function placeholderfunction (a:string):string; 
var kokotko: integer;
begin while kokotko >= 66
    do begin
    write ('picovina','pojebana');
    a:='hevno'
    end
end;

{ Hlavni telo programu }
begin
    write('Zadejte cislo pro vypocet faktorialu: ');
    readln(a);
    if a < 0 then { Pokracovani hlavniho tela programu }
    begin   
        write('Faktorial nelze spocitat'#10'')
    end
    else
    begin
        vysl := factorial(a,strigo,realis);
        write('Vysledek je: ', vysl, ''#10'')
    end
end.