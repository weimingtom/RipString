ReportFileName = "unpac_report.txt"
freport = assert(io.open(ReportFileName, "w+"))
ListFileName = "list.txt"
flist = assert(io.open(ListFileName, "w+"))
function report(s)
	print(os.date("%c"),s)
	freport:write(os.date("%c")," : ",s,"\n")
end

function list_report(s)
	flist:write(s,"\n")
end

function unpacfile(f,output,start,length)
	local current = f:seek()
	f:seek("set",start)
	local fout = assert(io.open(output,"wb+"))
	local bytes = f:read(length)
	fout:write(bytes)
	report("unpac : " .. output)
	f:seek("set",current)
end


function cal_number(bytes)
	local n = 0
	for i, b in ipairs { string.byte(bytes, 1, -1) } do
		n = n + b*256^(i-1)
	end
	return n
end

function unpac(input)
	local fin = assert(io.open(input,"rb"))
	report("Input file name : " .. input)
	fin:read(3) -- "pf6"
	local total1 = cal_number(fin:read(4)) --0x0122F6=74486
	report("total1 : " .. total1)
	local total2 = cal_number(fin:read(4)) --0x0531=1329
	report("total2 : " .. total2)
	for k = 1,total2 do
		local FileNameLength = cal_number(fin:read(4))
		local FileName = string.format("%s", fin:read(FileNameLength))
		local FileNamePlat = string.gsub(FileName, "\\", "_");
		fin:read(4) -- 0x00 00 00 00
		local FileStartOffset = cal_number(fin:read(4))
		local FileLength = cal_number(fin:read(4))
		report("File Start offset : " .. FileStartOffset)
		report("File length : " .. FileLength)
		report("File Name : " .. FileName)
		unpacfile(fin,FileNamePlat,FileStartOffset,FileLength)
		list_report(FileName)
	end
end

function pac(input,output)
end

function unpac_find(input,output)
	local fin = assert(io.open(input,"rb"))
	local total = cal_number(fin:read(4))
	report("Input file name : " .. input)
	report("Output file name: " .. output)
	local unpac_number = 0
	for k = 1,total do
		local FileStartOffset = cal_number(fin:read(4))
		local FileLength = cal_number(fin:read(4))
		local FileName = string.format("%s",fin:read(56))
		if string.find(FileName,output) then
			unpacfile(fin,FileName,FileStartOffset,FileLength)
			unpac_number = unpac_number + 1
		end
	end
	report("unpac number : " ..unpac_number)
end

unpac("../main.1.jp.windeal.WagaTouch.obb")
