
-- this script reads the bs list in bslist.cfg 
-- cpp function can be uesed to get all the filenames
-- then the cpp part of engine randomly create a new 
-- bullet storm
function get_bs_list()
	fileCount = 0
	io.input('BulletScript\\bslist.cfg')
	lines = {}
	for line in io.lines() do
		table.insert(lines, line)
		fileCount = fileCount + 1
	end
end