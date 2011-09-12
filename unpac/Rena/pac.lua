ReportFileName = "pac_report.txt"
freport = assert(io.open(ReportFileName, "w+"))
ListFileName = "list.txt"
flist = assert(io.open(ListFileName, "r+"))
function report(s)
	print(os.date("%c"),s)
	freport:write(os.date("%c")," : ",s,"\n")
end

function number2string(i)
	return string.char(i%256,(i-i%256)/256%256,(i-i%(256^2))/(256^2)%256,i/(256^3))
end

function pac(input, output)
	local files_name = {}
	local files_size = {}
	local files_offset = {}
	local fout = assert(io.open(output,"wb+"))
	local fpac

	while true do
		line = flist:read("*line")
		if line == nil then break end
		--print(line)
		fpac = io.open(line,"rb")
		if fpac ~= nil then
			files_name[#files_name + 1] = line
			local size = fpac:seek("end")
			files_size[#files_size + 1] = size
		end
		fpac:close()
	end

	local offset = 4 + (4+4+56)*(#files_name)
	for i, b in ipairs(files_name) do
		files_offset[#files_offset+1] = offset
		offset = offset + files_size[i]
	end

	local line1 = "test"
	local length = #line1
	while #line1 < 56 do
		line1 = line1 .. '\0'
	end

	report("input list file: " .. input)
	report("output file: " .. output)
	report("files number : " .. #files_name)

	fout:seek("set",0)
	fout:write(number2string(#files_name))
	for i, b in ipairs(files_name) do
		fout:write(number2string(files_offset[i]))
		fout:write(number2string(files_size[i]))
		local line = b
		local length = #line
		while #line < 56 do
			line = line .. '\0'
		end
		fout:write(line)
	end
	for i, b in ipairs(files_name) do
		fpac = assert(io.open(b,"rb"))
		while true do
			local bytes = fpac:read(1024)
			if not bytes then break end
			fout:write(bytes)
		end
		fpac:close()
	end
	report("pac finish")
end

pac("list.txt","out.pac")
