-- see Programming in Lua Second Edition (20)

local test = [[
int x; /*
		* x
        */
//hello
int y; /* y */
char name[] = "///* */";
]]

-- DONNOT USE .*, USE .-
-- http://stackoverflow.com/questions/241327/python-snippet-to-remove-c-and-c-comments
-- http://code.google.com/p/cpp-decomment/
test = string.gsub(test, "/%*.-%*/", "<COMMENT>")
test = string.gsub(test, "//.-\n", "<LINE_COMMENT>\n")
print(test)

