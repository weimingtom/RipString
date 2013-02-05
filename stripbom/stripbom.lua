require"lfs"

function trans(filename)
	local fin = assert(io.open(filename, "rb"))
	local bytes = fin:read("*all")
	fin:close()
	local fout = assert(io.open(filename, "wb"))

	local magic = string.sub(bytes, 1, 3)
	--print("magic:"..string.format("%02X%02X%02X", string.byte(magic, 1, 3)))

	local BOM = string.char(0xEF, 0xBB, 0xBF)

	local isBOM = (magic == BOM)
	--print("isBOM:"..tostring(isBOM), #magic, #BOM)

	for i = 1, string.len(bytes) do
		local byte = string.sub(bytes, i, i)
		if i < 4 and isBOM then
			-- do nothing
		else
			fout:write(byte);
		end
	end

	fout:close()
end

function attrdir(path)
    for file in lfs.dir(path) do
        if file ~= "." and file ~= ".." then
            local f = path..'/'..file
            --print ("\t "..f)
            local attr = lfs.attributes (f)
            assert (type(attr) == "table")
            if attr.mode == "directory" then
                attrdir (f)
            else
			--[[
                for name, value in pairs(attr) do
                    print (name, value)
                end
			--]]
				if (string.sub(f, -4) == '.txt') then
					print(f)
					trans(f)
				end
            end
        end
    end
end

attrdir (".")
