-- print('hello')

local fin = assert(io.open('nosis01_nscript.txt', 'r'));
local total_line = 0;

while true do
	local s = fin:read("*l");
	if s == nil then
		break
	else
		total_line = total_line + 1;
	end
end
print(total_line)

local page_line = total_line / 30
local line = 0;
local page = 1;
local fout = assert(io.open('nosis01_nscript_'..(page < 10 and '0'..page or page)..'.txt', 'w'));
fin:seek('set', 0);
while true do
	local s = fin:read("*l");
	if s == nil then
		break
	else
		line = line + 1;
		if line < page_line then
			fout:write(s..'\n');
		else
			line = 0;
			page = page + 1;
			fout:close();
			fout = assert(io.open('nosis01_nscript_'..(page < 10 and '0'..page or page)..'.txt', 'w'));
		end
	end
end
fout:close()
