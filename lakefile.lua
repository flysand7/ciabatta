local lake = require 'lakebuild'

local function buildcmd(target)
	local deps = ""
	if target.userdata.add_deps then
		deps = table.concat(target.deps, " ")
	end
	local buildline = table.concat({"clang", target.userdata.flags, target.files[1], "-o", target.name, deps}, " ")
	print(buildline)
	os.execute(buildline)
end


-- build all loose C files other than main.c into object files
local files = lake.glob_combined(".", false, nil, function(_, n) return lake.endswith(n, ".c") and not lake.endswith(n, "main.c") end)
local objects = {}
for _,file in ipairs(files) do
	local name = file[1] .. '.o'
	lake:add_target(name, {file[2]}, buildcmd, {flags="-c", add_deps=false})
	table.insert(objects, name)
end

-- build main.c
lake:add_chain("main.exe", {"main.c"}, objects, buildcmd, {flags="-O2", add_deps=true})

lake:request_build("main.exe")

