-- This tool is used to generate compact rt.jar file from
-- "java -verbose -jar ..." output information on Windows.
-- It will generate a .bat file, then execute jar.exe to
-- pack .class files into rt.jar
-- It can pack rt.jar more than one time.

-- for example:
-- 1. extract .class files from rt.jar to folder .\rt
-- 2. jre\bin\java -verbose -jar plugins/org.eclipse.equinox.launcher_1.2.0.v20110502.jar > a2.txt 2>&1
-- 3. execute compact_rt_jar.lua
-- 4. execute t2.bat
-- 5. if running error on other machine, repeat 2 to 4

fin = io.open("a2.txt", "r")
fout = io.open("t2.bat", "w+")

fout:write("@echo off\n")
fout:write("@cd /d %cd%\\rt\n")
fout:write("@set path=C:\\Program Files\\Java\\jdk1.6.0_26\\bin;%path%\n")
fout:write("@jar.exe cf ..\\rt.jar META-INF\\MANIFEST.MF\n")

i = 0
while true do
	s = fin:read("*l")
	if not s then break end
	r = s:match("^%[Loaded (.-) from .-rt.jar%]$")
	if r then
		-- print(s)
		r = r:gsub("%.", "\\")
		r = r..".class"
		-- print(r)
		fout:write(string.format("@jar.exe uf ..\\rt.jar %s\n", r))
		fout:write(string.format("@echo progress is %d\n", i))
		i = i + 1
	end
end

fout:write("@pause\n")
fout:write("@echo on\n")

fin:close()
fout:close()
--print(i)
