function unpac(output,start,length)
	local f = assert(io.open("main.1.jp.windeal.WagaTouch.obb","rb"))
	local fout = assert(io.open(output,"wb+"))

	local bytes = f:read(start)
	local bytes = f:read(length)
	fout:write(bytes)
end

unpac("1.ogg",0x0122FD,0x017C37)
unpac("1.sli",0x029F34,0xF8)
---0x0122FD
