fin = io.open("func.txt", "r")
fout = io.open("commands.txt", "w+")

while true do
	s = fin:read("*l")
	if not s then break end
	r = s:match("^Functions in .-%.as#(%d+)$")
	if r then
		fileID = r
		--print(fileID)
	else
		-- print(s)
		r = s:match("^ .- (%d+)$")
		if r then
			funcID = r
			if false then
				print("b #"..fileID..":"..funcID)
				print("commands")
				print("cont")
				print("end")
			else
				fout:write("b #"..fileID..":"..funcID.."\n")
				if fileID == "1" and funcID == "173" then
				else
					fout:write("commands\n")
					fout:write("silent\n")
					fout:write("cont\n")
					fout:write("end\n")
				end
			end
		end
	end
end

fin:close()
fout:close()
