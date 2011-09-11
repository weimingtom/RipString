local fin = assert(io.open("test.scr", "rb"))
local fout = assert(io.open("out.swf", "wb+"))
local bytes = fin:read("*all")

local fileMagic = 'CWS'
local fileMagic2 = 'BM6v'
local fileMagicLen = string.len(fileMagic)
local fileMagicLen2 = string.len(fileMagic2)
local trynumMax = 1
local trynum = 0
local isFoundMagic = false

for i = 0, string.len(bytes) do
	local magic = string.sub(bytes, i, i + fileMagicLen - 1)
	local magic2 = string.sub(bytes, i, i + fileMagicLen2 - 1)
	local byte = string.sub(bytes, i, i)

	if (isFoundMagic == false) and (magic == fileMagic) then
		print("start:"..string.format("%02X", i))
		isFoundMagic = true
	end

	if (isFoundMagic == true) and (magic2 == fileMagic2) then
		trynum = trynum + 1
		if trynum >= trynumMax then
			print("end:"..string.format("%02X", i))
			isFoundMagic = false
		end
	end

	if isFoundMagic == true then
		fout:write(byte);
	end
end

fin:close()
fout:close()

