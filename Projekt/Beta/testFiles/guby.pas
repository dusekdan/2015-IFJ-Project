var a:string;
	b:boolean;
	c:integer;
	d:real;
begin
	a:= 'jozko'#10'vajda';
	c:=length(a);
	write('Dlzka stringu: ',a,' je ',c,''#10'');
	a:=copy('ondrej caklos',3,5);
	write(a,''#10'')
end.