var a:string;
	b:boolean;
	c:integer;
	d:real;
	f:string;
begin
	a:= 'jozko'#10'vajda';
	c:=length(a);
	write('Dlzka stringu: ',a,' je ',c,''#10'');
	a:=copy(a,1,5);
	write(a,''#10'');
	a:='ko4896516541biyuigby8uyv7ufi89yoo';
	f:='rv';
	c:= find(a, f);
	
	write(c);
	f:='aqw5dcrygbihnuimjed6rftbhnuij985985298';
	a:=sort(f);
	write(a)
end.