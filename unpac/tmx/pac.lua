function number2string(i)
	return string.char(i%256,(i-i%256)/256%256,(i-i%(256^2))/(256^2)%256,i/(256^3))
end

function short2string(i)
	return string.char(i%256,(i-i%256)/256%256)
end

function main(input, output)
	local fout = io.open(output , "wb+")
	local fin = io.open(input , "rb")
	local width_n = 3
	local height_n = 2
	local block_size = 256
	local width = width_n * block_size
	local height = 	height_n * block_size
	local data_size = width * height * 4

	-- magic
	fout:write("BM")
	-- file size
	fout:write(number2string(data_size + 2 + 0x36))
	-- 0
	fout:write(number2string(0))
	-- data address
	fout:write(number2string(0x36))
	-- info size
	fout:write(number2string(0x28))
	-- width
	fout:write(number2string(width))
	-- height
	fout:write(number2string(height))
	-- 1
	fout:write(short2string(1))
	-- depth
	fout:write(short2string(32))
	-- compression
	fout:write(number2string(0))
	-- data size
	fout:write(number2string(data_size + 2))
	-- width ppm
	fout:write(number2string(0xb12))
	-- height ppm
	fout:write(number2string(0xb12))
	-- use color number
	fout:write(number2string(0))
	-- important color number
	fout:write(number2string(0))

	-- pixels

	--bytes number

	fin:seek("set",0x10)
	while true do
		local loc = math.floor((fin:seek() - 0x10)/4)
		--local block_y = math.floor(loc / (width * block_size))
		--local block_x = math.floor(loc / (block_size * block_size)) % width_n
		local in_block_total = loc % (block_size * block_size)
		local in_block_y = math.floor(in_block_total / block_size)
		local in_block_x = in_block_total % block_size
		local block_total = math.floor(loc /(block_size *block_size))
		local block_y = math.floor(block_total / width_n)
		local block_x = block_total % width_n

		local x = block_x * block_size + in_block_x
		local y = height - 1 - (block_y * block_size + in_block_y)
		local bytes = fin:read(4)
		if not bytes then break end
		fout:seek("set", 0x36 + ( y * width + x) * 4)
		fout:write(bytes)
	end

	-- end of file
	fout:write(short2string(0))

	fout:close()
end

--if false then
	for i = 30007,30008,1 do
		main("VIS" .. i .. ".tmx","out" .. i .. ".bmp")
	end
	for i = 30010,30012,1 do
		main("VIS" .. i .. ".tmx","out" .. i .. ".bmp")
	end
	for i = 30014,30019,1 do
		main("VIS" .. i .. ".tmx","out" .. i .. ".bmp")
	end
--end

for i = 30020,30028,1 do
	print(i)
	main("VIS" .. i .. ".tmx","out" .. i .. ".bmp")
end

--fin:seek("set",0x10)
--	while true do
--		local loc = fin:seek()
--		local bytes = fin:read(4)
--		if not bytes then break end
--		fout:write(bytes)
--	end



-- 分块，每块256*256像素，共3*2块
-- 转化为32位色深的bmp文件
--
