#!c:\Ruby192\bin\ruby

#
#c:\Ruby192\bin\ruby test.rb
# .tables
# .schema tb_img
#

require 'sqlite3'

db = SQLite3::Database.new("mmshirt.data.mp3")
db.execute("select id, order_no, img_bin from tb_img") do |row|
	print(row[0], ",", row[1], "\n")
	File.open(row[0].to_s + ".jpg", "wb") do |f|
		f.puts(row[2])
	end
end
