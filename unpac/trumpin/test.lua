function unpac(output,start,length)
	local f = assert(io.open("data.bin","rb"))
	local fout = assert(io.open(output,"wb+"))

	local bytes = f:read(start)
	local bytes = f:read(length)
	fout:write(bytes)
end

unpac("1.wav",0x000b00,0x003f00-0x000b00)
--unpac("1.png",0x139e00,0x16ad00-0x139e00)
unpac("1.png",0x139e00,0x030e2e)
