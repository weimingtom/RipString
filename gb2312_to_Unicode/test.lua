require "gb2312"

function gb2un(s)
  local su = ""
  local i = 1
  while true do
    if(i + 1 > #s) then break end
    local c1,c2 = string.byte(s,i),string.byte(s,i+1)
    if(c1 < 128) then
      if(c1 == 0xa) then
        su = su .. string.char(0x0d,0x00,math.floor(c1),0x00)
      else
        su = su .. string.char(math.floor(c1),0x00)
      end
      i = i + 1
    else
      local cu = gb2312.to_unicode(c1*256+c2)
      if(cu ~= nil) then
          su = su .. string.char(math.floor(cu/256),math.floor(cu%256))
          i = i + 2
      else
          i = i + 1
      end
    end
  end
  return su
end

f=io.open("output.txt","wb+")
f:write(string.char(0xff),string.char(0xfe))
f:write(gb2un("°¡ksÂð\ndisaoasdfsdfÈö»ÑÄÇ¸ö\n\t\n"))

--print(gb2312.to_unicode(0xF7FE))
