local function readctags(filename)
	local f = assert(io.open(filename, "r"))
	local t = {}
	while true do
		local s = f:read("*l")
		if s == nil then break end
		if s:sub(1, 1) ~= '!' then
			t[#t+1] = {
				["fname"] = s:match("[A-Za-z0-9_]+"),
				["lineno"] = tonumber(s:match("line:(%d+)")),
			}
			--print(t[#t].fname..":"..t[#t].lineno)
			--print(s)
		end
	end
	f:close()
	return t
end

local function nearestfunc(fline, t)
	for i, record in ipairs(t) do
		if record["lineno"] == fline then
			return nil
		elseif record["lineno"] > fline then
			return t[i-1]["fname"]
		end
	end
	return t[#t]["fname"]
end

local function putrelation(t, nname, fname)
	if t[nname] == nil then
		t[nname] = {}
	end
	local isfound = false
	for i, v in ipairs(t[nname]) do
		if v == fname then
			isfound = true
			break
		end
	end
	if not isfound then
		t[nname][#(t[nname])+1] = fname
	end
end

local function readsource(filename, t)
	local result = {}
	local f = assert(io.open(filename, "r"))
	for _, record in ipairs(t) do
		f:seek("set")
		local fname = record["fname"]
		local fline = 1
		--print("========>search for "..fname)
		while true do
			local s = f:read("*l")
			if s == nil then break end
			if s:find(fname) then
				--print(fline.." : "..nearestfunc(fline, t))
				local nname = nearestfunc(fline, t)
				if nname then
					--print("\""..nname.."\" -> \""..fname.."\"")
					putrelation(result, nname, fname)
				end
			end
			fline = fline + 1
		end
	end
	f:close()
	return result
end

local function main()

	local t = readctags("lcode.txt")
	local s = readsource("lcode.c", t)
	print("digraph android_graphics {")
	for k, v in pairs(s) do
		for _, r in ipairs(v) do
			print("\""..k.."\" -> \""..r.."\"")
		end
	end
	print("}")
end

main()
