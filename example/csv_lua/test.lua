local csv = CsvParser:new("config.csv", 2, ',')

for i = 1, csv:row(), 1 do
	local id = csv:getInt32(i, "id")
	local name = csv:getString(i, "name")
	local value = csv:getInt32(i, "value")
	print(id.." | ".. name .. " | ".. value)
end