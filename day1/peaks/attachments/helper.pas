program helper;

function query(x, y, z : longint) : longint;
begin
	writeln('? ', x, ' ', y, ' ', z);
	Flush(Output);
	readln(query);
	if query = -1 then halt
end;

procedure guess(x, y, z : longint);
begin
	writeln('! ', x, ' ', y, ' ', z);
	Flush(Output);
	halt
end;


var
	n, m, k, q : longint;
begin
	readln(n, m, k, q);

	{ TODO do something smart }

	query(1, 1, 1);
	query(n, m, k);
	guess(1, 1, 1);
end.
